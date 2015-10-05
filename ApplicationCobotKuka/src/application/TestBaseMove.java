package application;


import java.util.ArrayList;
import org.json.JSONException;

import serveur.TCPServer;

import com.kuka.generated.ioAccess.MediaFlangeIOGroup;
import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.*;
import com.kuka.roboticsAPI.controllerModel.Controller;
import com.kuka.roboticsAPI.controllerModel.sunrise.ISafetyState;
import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.deviceModel.OperationMode;
import com.kuka.roboticsAPI.geometricModel.CartDOF;
import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.geometricModel.ObjectFrame;
import com.kuka.roboticsAPI.geometricModel.Tool;
import com.kuka.roboticsAPI.geometricModel.math.Transformation;
import com.kuka.roboticsAPI.motionModel.RelativeLIN;
import com.kuka.roboticsAPI.motionModel.Spline;
import com.kuka.roboticsAPI.motionModel.SplineJP;
import com.kuka.roboticsAPI.motionModel.controlModeModel.CartesianSineImpedanceControlMode;
/**
 * Classe TestBaseMove
 * @author Bosch, Berriche, Cano, Danjoux, Durand, Olivieri
 * @date 29/09/2015
 */
public class TestBaseMove extends RoboticsAPIApplication {
	
	/**
	 * Variables 
	 */
	private TCPServer serveur; // Objet Serveur
	
	private Controller kuka_Sunrise_Cabinet_1;
	private LBR lbr_iiwa_14_R820_1;

	private MediaFlangeIOGroup ioFlange;
	
	private CartesianSineImpedanceControlMode impedanceControlMode;
	
	private Tool penTool;
	private ObjectFrame penToolTCP;
	
	private ObjectFrame paperBase;
	private ObjectFrame paperApproach;
	
	//private BezierCurve curve
	
	//private Frame[][] trajectory;
	
	
	//private Frame[] frames;
	
	
	
	/*private Transformation getTranslationWithSpecifiedZ(ObjectFrame frameBefore, ObjectFrame frameDestination, double z)
	{
		return Transformation.ofTranslation(
				frameDestination.getX()-frameBefore.getX(), 
				frameDestination.getY()-frameBefore.getY(), 
				z
				);
	}*/
	
	/**
	 * 
	 * @param frameBefore
	 * @param frameDestination
	 * @return
	 */
	private Transformation getTranslationFromFrame(Frame frameBefore, Frame frameDestination)
	{
		return Transformation.ofTranslation(
				frameDestination.getX()-frameBefore.getX(), 
				frameDestination.getY()-frameBefore.getY(), 
				frameDestination.getZ()-frameBefore.getZ()
				);
	}
	
	/**
	 * 
	 */
	public void initialize() {
		// Créer l'objet serveur tcp pour recevoir les commandes de dessin
		serveur = new TCPServer(30001);
		
		kuka_Sunrise_Cabinet_1 = getController("KUKA_Sunrise_Cabinet_1");
		lbr_iiwa_14_R820_1 = (LBR) getDevice(kuka_Sunrise_Cabinet_1, "LBR_iiwa_14_R820_1");

		ioFlange = new MediaFlangeIOGroup(kuka_Sunrise_Cabinet_1);
	
		// On crée la compliance pour rendre le bras "mou"
		impedanceControlMode = new CartesianSineImpedanceControlMode();
		impedanceControlMode.parametrize(CartDOF.X).setStiffness(3000);
		impedanceControlMode.parametrize(CartDOF.Y).setStiffness(3000);
		impedanceControlMode.parametrize(CartDOF.Z).setStiffness(3000);
		
		impedanceControlMode.parametrize(CartDOF.A).setStiffness(200);
		impedanceControlMode.parametrize(CartDOF.B).setStiffness(200);
		impedanceControlMode.parametrize(CartDOF.C).setStiffness(200);
		
		// On crée l'outil stylo, on l'attache au flange et on récupére le point en bout de stylo "penToolTCP"
		penTool = getApplicationData().createFromTemplate("penTool");
		penTool.attachTo(lbr_iiwa_14_R820_1.getFlange() );
		penToolTCP = penTool.getFrame("/penToolTCP");
		
		
		// On charge la base de l'application
		paperBase = getApplicationData().getFrame("/Paper");
		
		// On charge les points
		paperApproach = getApplicationData().getFrame("/Paper/PaperApproach");
		
		/*// On définit les points du parcours
		P1 = new Frame(0.0, 0.0, 0.0);
		P2 = new Frame(0.0, 0.0, 0.0);
		P3 = new Frame(0.0, 0.0, 0.0);
		P4 = new Frame(0.0, 0.0, 0.0);
		P5 = new Frame(0.0, 0.0, 0.0);
		
		trajectory = new Frame[3];
		trajectory2 = new Frame[2];
		
		trajectory[0] = P1;
		trajectory[1] = P2;
		trajectory[2] = P3;
		
		trajectory2[0] = P4;
		trajectory2[0] = P5;*/
		
		
		
		/*curve = new BezierCurve(P1, P2, P3, P0);
		
		trajectory = curve.getTrajectory(40);
		
		// On crée des frames robot Kuka depuis notre courbe
		frames = new Frame[trajectory.length];
		for (int i=0; i < trajectory.length; i++)
		{
//			getLogger().info("" + trajectory[i].x + " "+ trajectory[i].y);
			frames[i] = new Frame(trajectory[i].x, trajectory[i].y, 0);
		}*/
				
		getLogger().info("Initialization OK");
	}

	/**
	 * 
	 */
	public void run() {
		try {
			//Paper approach
			double velocity = 0.2;
			
			String message = "";
			
			int zOffsetForPen = 10;
			int zOffsetForPaper = -3;
			
			ArrayList<RelativeLIN> alSpline = new ArrayList<RelativeLIN>();
			RelativeLIN[] relAlSpline = null; 
			ArrayList<ArrayList<Frame>> trajectories = new ArrayList<ArrayList<Frame>>();
			
			ISafetyState currentState = lbr_iiwa_14_R820_1.getSafetyState();
			OperationMode mode = currentState.getOperationMode();
			if (mode==OperationMode.AUT)
			{
				//******* ATTENTION : AVANT D'ACTIVE LE MODE AUTO, VERIFIER LES TRAJECTOIRES ********//
				// Si on est en mode AUTO, pleine vitesse activée
				
				velocity = 1;
			}
			
			lbr_iiwa_14_R820_1.move(ptpHome());
			
			
			ioFlange.setLEDBlue(true);
			
			
			// Approche de la base "Paper" en PTP puis en LIN
			
			getLogger().info("Move near Paper");
					
			penToolTCP.move(
					ptp(paperApproach).setJointVelocityRel(velocity)
				);
			
			// Boolean qui indique la fin du dessin et du programme
			boolean end = false;
			// Tant que le boolean est faux on reste dans la boucle
			while(!end)
			{
				getLogger().info("Serveur en attente de commandes");
				
				// Attend la connexion et le message du client
				this.serveur.run();
				
				message = this.serveur.getMessage();
				
				// Verifie le message du client
				if(message == "stop") {
					this.serveur.closeServer(); // Arrete le serveur
					end = true; // Indique que la boucle est terminée
				} 
				// Si le message est différent de vide on rentre dans la condition / Sinon on attend un nouveau message du client
				else if (message != ""){
	
					getLogger().info("Message du client : " + this.serveur.getMessage());
					
					try {
						// On récupère la chaîne de caractère qu'on converti en JSON
						org.json.JSONObject jMainObject  = new org.json.JSONObject(message);
						// On récupère l'objet principale pointé par la key "svg"
						org.json.JSONArray jArray = jMainObject.getJSONArray("svg");
						// On parcours chaque tableau de l'objet principal
						for (int j = 0; j < jArray.length(); j++) {
							// On récupère le tableau en question
							org.json.JSONArray jArray2 = jArray.getJSONArray(j);
							// On créé l'ArrayList qui va récupérer les Frames
							ArrayList<Frame> trajectories2 = new ArrayList<Frame>();
							// On parcours chaque élément du tableau des frames
							for (int i = 0; i < jArray2.length(); i++) {
								// On récupère le dit-élément
								org.json.JSONObject jArrayObject = jArray2.getJSONObject(i);
								// On créé les Frames (points) qu'on stock dans une ArrayList
								trajectories2.add(new Frame(jArrayObject.getInt("x"), jArrayObject.getInt("y"), 0.0));
								
								/* On traite la remonte du stylo entre chaque segment à dessiner
								 * On ajoute 3 mouvements :
								 *			- La remonté du stylo
								 *			- La translation vers le nouveau point
								 * 			- La descente du stylo vers le papier
								 */
								if (j != 0 && i == 0) {
									// On récupère le dernier segment ajouté
									ArrayList<Frame> lastSegment = trajectories.get(trajectories.size() - 1);
									// On récupère la dernière frame ajoutée
									Frame lastFrame = lastSegment.get(lastSegment.size() - 1);
									getLogger().info("lastFrame " + j + " " + i + " " + lastFrame.toString());
									// On créé le nouveau point offset en Z pour remonter le stylo
									Frame offsetLastFrame = new Frame(lastFrame.getX(), lastFrame.getY(), zOffsetForPen);
									getLogger().info("offsetLastFrame " + j + " " + i + " " + offsetLastFrame.toString());
									// On remonte le stylo
									alSpline.add(linRel(getTranslationFromFrame(lastFrame, offsetLastFrame), paperBase));
									// On récupère la dernière frame ajoutée
									Frame aimFrame = trajectories2.get(trajectories2.size() - 1);
									getLogger().info("aimFrame " + j + " " + i + " " + aimFrame.toString());
									// On créer le nouveau point offset du point d'arrivé
									Frame offsetAimFrame = new Frame(aimFrame.getX(), aimFrame.getY(), zOffsetForPen);
									getLogger().info("offsetAimFrame " + j + " " + i + " " + offsetAimFrame.toString());
									// On ajoute la translation vers le nouveau point
									alSpline.add(linRel(getTranslationFromFrame(offsetLastFrame, offsetAimFrame), paperBase));
									// On créer le point sur le papier
									Frame aimFrameOnPaper = new Frame (aimFrame.getX(), aimFrame.getY(), zOffsetForPaper);
									getLogger().info("aimFrameOnPaper " + j + " " + i + " " + aimFrameOnPaper.toString());
									// On ajoute la translation suivant Z pour toucher le papier
									alSpline.add(linRel(getTranslationFromFrame(offsetAimFrame, aimFrameOnPaper), paperBase));
								} else if (j == 0 && i == 0) {
									// On récupère le permier point pour se placer au-dessus
									Frame firstFrame = trajectories2.get(0);
									getLogger().info("firstFrame " + j + " " + i + " " + firstFrame.toString());
									// On créé le point offset de firstFrame
									Frame offsetFirstFrame = new Frame(firstFrame.getX(), firstFrame.getY(), zOffsetForPen);
									getLogger().info("offsetFirstFrame " + j + " " + i + " " + offsetFirstFrame.toString());
									// On ajoute le déplacement vers le premier point
									alSpline.add(linRel(getTranslationFromFrame(new Frame(paperApproach.getX(), paperApproach.getY(), paperApproach.getZ()), offsetFirstFrame), paperBase));
									// On créé le premier point sur le papier
									Frame firstFrameOnPaper = new Frame(offsetFirstFrame.getX(), offsetFirstFrame.getY(), zOffsetForPaper);
									getLogger().info("firstFrameOnPaper " + j + " " + i + " " + firstFrameOnPaper.toString());
									// On ajoute le mouvement de descente sur le papier
									alSpline.add(linRel(getTranslationFromFrame(offsetFirstFrame, firstFrameOnPaper), paperBase));
								}
							}
							
							// On stock les frames séparées dans chaque segment
							trajectories.add(trajectories2);
								
							// On enregistre les mouvements dans une ArrayList
							for (int i = 0; i < trajectories2.size() - 1; i++) {
								alSpline.add(linRel(getTranslationFromFrame(trajectories2.get(i), trajectories2.get(i + 1)), paperBase));
							}
						}
					} catch (JSONException e) {
						e.printStackTrace();
					}
					
					relAlSpline = new RelativeLIN[alSpline.size()];
					
					for (int i = 0; i < alSpline.size(); i++)
					{
						relAlSpline[i] = alSpline.get(i);
						getLogger().info(relAlSpline[i].toString());
					}
					Spline linMovement = new Spline(relAlSpline);
					
					penToolTCP.move(linMovement.setJointVelocityRel(velocity));
					
					/*getLogger().info("Move on paper");
					
					linMovement = new Spline(linRel(getTranslationFromFrame(new Frame(P1.getX(), P1.getY(), P1.getZ()), new Frame(P1.getX(), P1.getY(), -3)), paperBase));
					
					penToolTCP.move(
							linMovement.setJointVelocityRel(0.05)
							);
					
					
					// On dessine au dessus du papier les points P0 / P1 / P2 / P3 :
					
					getLogger().info("Move on Paper");
					
					RelativeLIN [] splineArray = new RelativeLIN[frames.length-1];
					
					for (int i=0; i < frames.length-1; i++)
					{
						RelativeLIN moveLin2 = linRel(getTranslationFromFrame(frames[i], frames[i+1]),paperBase);
						
						splineArray[i] = moveLin2;
					}
					
					Spline linMovement2 = new Spline(splineArray);
					
					penToolTCP.move(
							linMovement2.setJointVelocityRel(velocity)
						);
					
					linMovement = new Spline(
							
							// On bouge en relatif
							
							// On va ensuite à P1, P2, P3 et P0, en spécifiant une translation Z nulle 
							linRel( getTranslationFromFrame(P1, P2), paperBase),
							linRel( getTranslationFromFrame(P2, P3), paperBase)
						);
					
					getLogger().info("draw first spline");
					
					penToolTCP.move(
							linMovement.setJointVelocityRel(velocity)
						);
					
					getLogger().info("Move to second spline");
					
					linMovement = new Spline(
							
							// On remonte le stylo
							linRel( getTranslationFromFrame(P3, new Frame(P3.getX(), P3.getY(), 10)), paperBase),
							// On déplace vers le point de la spline suivante
							linRel( getTranslationFromFrame(new Frame(P3.getX(), P3.getY(), 10), new Frame(P4.getX(), P4.getY(), 10)), paperBase),
							// On rabaisse le stylo
							linRel( getTranslationFromFrame(new Frame(P4.getX(), P4.getY(), 10), new Frame(P4.getX(), P4.getY(), -3)), paperBase)
						);
					
					penToolTCP.move(
							linMovement.setJointVelocityRel(0.05)
						);
					
					getLogger().info("draw second spline");
					
					linMovement = new Spline(
							
							// On remonte le stylo
							linRel( getTranslationFromFrame(P4, P5), paperBase)
						);
					
					penToolTCP.move(
							linMovement.setJointVelocityRel(velocity)
						);
					
					// On rmeonte le stylo
					linMovement = new Spline(
							
							// On remonte le stylo
							linRel( getTranslationFromFrame(P5, new Frame(P5.getX(), P5.getY(), 10)), paperBase)
						);
					
					penToolTCP.move(
							linMovement.setJointVelocityRel(velocity)
						);
					
					// Vide la valeur du message du serveur
					this.serveur.setMessage("");
				}*/
			
				// Initialise la position du robot
				getLogger().info("Retour position initiale");
				
				penToolTCP.move(lin(paperApproach).setJointVelocityRel(velocity));
				
				SplineJP moveBackToHome = new SplineJP( ptpHome());
				
				getLogger().info("Move Back");
				lbr_iiwa_14_R820_1.move(moveBackToHome.setJointVelocityRel(velocity));
				
				ioFlange.setLEDBlue(false);
				}
			}
			
			// Ferme le serveur
			serveur.closeServer();
			
		} catch(Exception e){ getLogger().info("Erreur methode run : " + e); serveur.closeServer(); }
	}

	/**
	 * Entrée principale du programme
	 * Lancement de l'application 
	 * @param args
	 */
	public static void main(String[] args) {
		TestBaseMove app = new TestBaseMove();
		app.runApplication();
	}
}
