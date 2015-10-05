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
	
	private BezierCurve curve;
	
	private Vector2[] trajectory;
		
	private Frame[] frames;
	
	private Vector2[] bezierControlPoints;
	
	
	
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
		// Cr�er l'objet serveur tcp pour recevoir les commandes de dessin
		serveur = new TCPServer(30002);
		
		kuka_Sunrise_Cabinet_1 = getController("KUKA_Sunrise_Cabinet_1");
		lbr_iiwa_14_R820_1 = (LBR) getDevice(kuka_Sunrise_Cabinet_1, "LBR_iiwa_14_R820_1");

		ioFlange = new MediaFlangeIOGroup(kuka_Sunrise_Cabinet_1);
	
		// On cr�e la compliance pour rendre le bras "mou"
		impedanceControlMode = new CartesianSineImpedanceControlMode();
		impedanceControlMode.parametrize(CartDOF.X).setStiffness(3000);
		impedanceControlMode.parametrize(CartDOF.Y).setStiffness(3000);
		impedanceControlMode.parametrize(CartDOF.Z).setStiffness(3000);
		
		impedanceControlMode.parametrize(CartDOF.A).setStiffness(200);
		impedanceControlMode.parametrize(CartDOF.B).setStiffness(200);
		impedanceControlMode.parametrize(CartDOF.C).setStiffness(200);
		
		// On cr�e l'outil stylo, on l'attache au flange et on r�cup�re le point en bout de stylo "penToolTCP"
		penTool = getApplicationData().createFromTemplate("penTool");
		penTool.attachTo(lbr_iiwa_14_R820_1.getFlange() );
		penToolTCP = penTool.getFrame("/penToolTCP");
		
		
		// On charge la base de l'application
		paperBase = getApplicationData().getFrame("/Paper");
		
		// On charge les points
		paperApproach = getApplicationData().getFrame("/Paper/PaperApproach");
				
		getLogger().info("Initialization OK");
	}

	/**
	 * 
	 */
	public void run() {
		Spline linMovement = null;
		try {
			double velocity = 0.2;
			
			String message = "";
			
			/*int zOffsetForPen = 10;
			int zOffsetForPaper = -3;
			
			ArrayList<RelativeLIN> alSpline = new ArrayList<RelativeLIN>();
			RelativeLIN[] relAlSpline = null; 
			ArrayList<ArrayList<Frame>> trajectories = new ArrayList<ArrayList<Frame>>();*/
			
			int cpt = 0;
			
			ISafetyState currentState = lbr_iiwa_14_R820_1.getSafetyState();
			OperationMode mode = currentState.getOperationMode();
			if (mode==OperationMode.AUT)
			{
				//******* ATTENTION : AVANT D'ACTIVE LE MODE AUTO, VERIFIER LES TRAJECTOIRES ********//
				// Si on est en mode AUTO, pleine vitesse activ�e
				
				velocity = 1;
			}
			
			//Init position
			lbr_iiwa_14_R820_1.move(ptpHome());
			
			
			ioFlange.setLEDBlue(true);
			
			
			// Approche de la base "Paper" en PTP
			
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
					end = true; // Indique que la boucle est termin�e
				} 
				// Si le message est diff�rent de vide on rentre dans la condition / Sinon on attend un nouveau message du client
				else if (message != ""){
	
					getLogger().info("Message du client : " + this.serveur.getMessage());
					
					try {
						// On r�cup�re la cha�ne de caract�re qu'on converti en JSON
						org.json.JSONObject jObject = new org.json.JSONObject(message);
						// On r�cup�re l'objet point� par "svg"
						org.json.JSONObject jSvgObject = jObject.getJSONObject("svg");
						// On r�cup�re l'objet point� par "M"
						org.json.JSONArray jMArray = jSvgObject.getJSONArray("M");
						// On cr�� la premi�re frame
						Frame firstFrame = new Frame(jMArray.getInt(0) * 297 / 4000, jMArray.getInt(1) * 210 / 4000, 10.0);
						// On cr�� la frame correspondante sur le papier
						Frame firstFrameOnPaper = new Frame(jMArray.getInt(0) * 297 / 4000, 210 - jMArray.getInt(1) * 210 / 4000, -3.0);
						// On r�cup�re l'Array des points de la courbe de bezier
						org.json.JSONArray jCArray = jSvgObject.getJSONArray("c");
						// On r�cup�re l'ensemble des points qu'on stock dans un tableau de Vector2
						bezierControlPoints = new Vector2[jCArray.length() / 2];
						int i = 0;
						while (cpt < jCArray.length()) {
							bezierControlPoints[i].x = (jMArray.getInt(0) + jCArray.getInt(cpt)) * 297 / 4000;
							bezierControlPoints[i].y = 210 - (jMArray.getInt(1) + jCArray.getInt(++cpt)) * 210 / 4000;
							cpt++;
							i++;
						}
						curve = new BezierCurve(bezierControlPoints);
						trajectory = curve.getTrajectory(40);
						// On cr�e des frames robot Kuka depuis notre courbe
						frames = new Frame[trajectory.length];
						for (i = 0; i < trajectory.length; i++)
						{	
							frames[i] = new Frame(trajectory[i].x, trajectory[i].y, 0.0);
						}
						RelativeLIN [] splineArray = new RelativeLIN[frames.length+1];
						//  On approche de la feuille � 10 au dessus du point init
						splineArray[0] = linRel(getTranslationFromFrame(new Frame(paperApproach.getX(),paperApproach.getY(), paperApproach.getZ()), firstFrame), paperBase);
						// Le stylo touche la feuille
						splineArray[1] = linRel(getTranslationFromFrame(firstFrame, firstFrameOnPaper), paperBase);
						
						for (i = 0; i < frames.length-1; i++)
						{
							RelativeLIN moveLin = linRel(getTranslationFromFrame(frames[i], frames[i+1]),paperBase);
							splineArray[i+2] = moveLin;
						}
						linMovement = new Spline(splineArray);
						
						
						
						/*
						// On r�cup�re l'objet principale point� par la key "svg"
						org.json.JSONArray jArray = jMainObject.getJSONArray("svg");
						// On parcours chaque tableau de l'objet principal
						for (int j = 0; j < jArray.length(); j++) {
							// On r�cup�re le tableau en question
							org.json.JSONArray jArray2 = jArray.getJSONArray(j);
							// On cr�� l'ArrayList qui va r�cup�rer les Frames
							ArrayList<Frame> trajectories2 = new ArrayList<Frame>();
							// On parcours chaque �l�ment du tableau des frames
							for (int i = 0; i < jArray2.length(); i++) {
								// On r�cup�re le dit-�l�ment
								org.json.JSONObject jArrayObject = jArray2.getJSONObject(i);
								// On cr�� les Frames (points) qu'on stock dans une ArrayList
								trajectories2.add(new Frame(jArrayObject.getInt("x"), jArrayObject.getInt("y"), 0.0));
								
								/* On traite la remonte du stylo entre chaque segment � dessiner
								 * On ajoute 3 mouvements :
								 *			- La remont� du stylo
								 *			- La translation vers le nouveau point
								 * 			- La descente du stylo vers le papier
								 
								if (j != 0 && i == 0) {
									// On r�cup�re le dernier segment ajout�
									ArrayList<Frame> lastSegment = trajectories.get(trajectories.size() - 1);
									// On r�cup�re la derni�re frame ajout�e
									Frame lastFrame = lastSegment.get(lastSegment.size() - 1);
									getLogger().info("lastFrame " + j + " " + i + " " + lastFrame.toString());
									// On cr�� le nouveau point offset en Z pour remonter le stylo
									Frame offsetLastFrame = new Frame(lastFrame.getX(), lastFrame.getY(), zOffsetForPen);
									getLogger().info("offsetLastFrame " + j + " " + i + " " + offsetLastFrame.toString());
									// On remonte le stylo
									alSpline.add(linRel(getTranslationFromFrame(lastFrame, offsetLastFrame), paperBase));
									// On r�cup�re la derni�re frame ajout�e
									Frame aimFrame = trajectories2.get(trajectories2.size() - 1);
									getLogger().info("aimFrame " + j + " " + i + " " + aimFrame.toString());
									// On cr�er le nouveau point offset du point d'arriv�
									Frame offsetAimFrame = new Frame(aimFrame.getX(), aimFrame.getY(), zOffsetForPen);
									getLogger().info("offsetAimFrame " + j + " " + i + " " + offsetAimFrame.toString());
									// On ajoute la translation vers le nouveau point
									alSpline.add(linRel(getTranslationFromFrame(offsetLastFrame, offsetAimFrame), paperBase));
									// On cr�er le point sur le papier
									Frame aimFrameOnPaper = new Frame (aimFrame.getX(), aimFrame.getY(), zOffsetForPaper);
									getLogger().info("aimFrameOnPaper " + j + " " + i + " " + aimFrameOnPaper.toString());
									// On ajoute la translation suivant Z pour toucher le papier
									alSpline.add(linRel(getTranslationFromFrame(offsetAimFrame, aimFrameOnPaper), paperBase));
									
								/* On se place au dessus du premier point � dessiner
								 
								} else if (j == 0 && i == 0) {
									// On r�cup�re le permier point pour se placer au-dessus
									Frame firstFrame = trajectories2.get(0);
									getLogger().info("firstFrame " + j + " " + i + " " + firstFrame.toString());
									// On cr�� le point offset de firstFrame
									Frame offsetFirstFrame = new Frame(firstFrame.getX(), firstFrame.getY(), zOffsetForPen);
									getLogger().info("offsetFirstFrame " + j + " " + i + " " + offsetFirstFrame.toString());
									// On ajoute le d�placement vers le premier point
									alSpline.add(linRel(getTranslationFromFrame(new Frame(paperApproach.getX(), paperApproach.getY(), paperApproach.getZ()), offsetFirstFrame), paperBase));
									// On cr�� le premier point sur le papier
									Frame firstFrameOnPaper = new Frame(offsetFirstFrame.getX(), offsetFirstFrame.getY(), zOffsetForPaper);
									getLogger().info("firstFrameOnPaper " + j + " " + i + " " + firstFrameOnPaper.toString());
									// On ajoute le mouvement de descente sur le papier
									alSpline.add(linRel(getTranslationFromFrame(offsetFirstFrame, firstFrameOnPaper), paperBase));
								}
							}
							
							// On stock les frames s�par�es dans chaque segment
							trajectories.add(trajectories2);
								
							// On enregistre les mouvements dans une ArrayList
							for (int i = 0; i < trajectories2.size() - 1; i++) {
								alSpline.add(linRel(getTranslationFromFrame(trajectories2.get(i), trajectories2.get(i + 1)), paperBase));
							}
						}*/
					} catch (JSONException e) {
						e.printStackTrace();
					}
					
					/*relAlSpline = new RelativeLIN[alSpline.size()];
					
					for (int i = 0; i < alSpline.size(); i++)
					{
						relAlSpline[i] = alSpline.get(i);
						getLogger().info(relAlSpline[i].toString());
					}
					Spline linMovement = new Spline(relAlSpline);*/
					
					penToolTCP.move(linMovement.setJointVelocityRel(velocity));
			
					// On retourne � la position initiale du robot
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
	 * Entr�e principale du programme
	 * Lancement de l'application 
	 * @param args
	 */
	public static void main(String[] args) {
		TestBaseMove app = new TestBaseMove();
		app.runApplication();
	}
}
