package application;


import java.io.IOException;

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


/* Test des mouvements du robot Kuka avec la base "Paper"
 * 
 */

public class TestBaseMove extends RoboticsAPIApplication {
	private Controller kuka_Sunrise_Cabinet_1;
	private LBR lbr_iiwa_14_R820_1;

	private MediaFlangeIOGroup ioFlange;
	
	private CartesianSineImpedanceControlMode impedanceControlMode;
	
	private Tool penTool;
	private ObjectFrame penToolTCP;
	
	private ObjectFrame paperBase;
	private Frame P1;
	private Frame P2;
	private Frame P3;
	private Frame P4;
	private Frame P5;
	
	/*private Vector2 P1;
	private Vector2 P2;
	private Vector2 P3;
	private Vector2 P4;
	private Vector2 P5;*/
	
	private ObjectFrame nearPaper0;
	private ObjectFrame paperApproach;
	
	//private BezierCurve curve;
	
	private Frame[] trajectory;
	private Frame[] trajectory2;
	
	
	//private Frame[] frames;
	
	private TCPServer serveur;
	private int arret =0;
	
	
	/*private Transformation getTranslationWithSpecifiedZ(ObjectFrame frameBefore, ObjectFrame frameDestination, double z)
	{
		return Transformation.ofTranslation(
				frameDestination.getX()-frameBefore.getX(), 
				frameDestination.getY()-frameBefore.getY(), 
				z
				);
	}*/
	
	private Transformation getTranslationFromFrame(Frame frameBefore, Frame frameDestination)
	{
		return Transformation.ofTranslation(
				frameDestination.getX()-frameBefore.getX(), 
				frameDestination.getY()-frameBefore.getY(), 
				frameDestination.getZ()-frameBefore.getZ()
				);
	}
	
	public void initialize() {
		
		// Créer l'objet serveur tcp pour recevoir les commandes de dessin
		try {
			TCPServer serveur = new TCPServer();
		} catch (IOException e) {
			// TODO Bloc catch généré automatiquement
			e.printStackTrace();
		}
		
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
		nearPaper0 = getApplicationData().getFrame("/Paper/NearPaper0");
		paperApproach = getApplicationData().getFrame("/Paper/PaperApproach");
		
		// On définit les points du parcours
		P1 = new Frame(0.0, 0.0, 0.0);
		P2 = new Frame(20.0, 40.0, 0.0);
		P3 = new Frame(40.0, 0.0, 0.0);
		P4 = new Frame(10.0, 20.0, 0.0);
		P5 = new Frame(30.0, 20.0, 0.0);
		
		trajectory = new Frame[3];
		trajectory2 = new Frame[2];
		
		trajectory[0] = P1;
		trajectory[1] = P2;
		trajectory[2] = P3;
		
		trajectory2[0] = P4;
		trajectory2[0] = P5; 
		
		
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

	public void run() {
		
		//Paper approach
		double velocity = 0.2;
		
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
		
		boolean end = false;
		while(!end)
		{
			// Attend message client
			this.serveur.run();
			
			// Verifie le message du client
			if(this.serveur.getMessage() == "stop") {
				this.serveur.closeServer();
				end = true;
			} else if (this.serveur.getMessage() != ""){
				
				Spline linMovement = new Spline(linRel(getTranslationFromFrame(new Frame(paperApproach.getX(), paperApproach.getY(), paperApproach.getZ()), new Frame(P1.getX(), P1.getY(), P1.getZ())), paperBase));
				
				penToolTCP.move(
						linMovement.setJointVelocityRel(velocity)
					);
				
				getLogger().info("Move on paper");
				
				linMovement = new Spline(linRel(getTranslationFromFrame(new Frame(P1.getX(), P1.getY(), P1.getZ()), new Frame(P1.getX(), P1.getY(), -3)), paperBase));
				
				penToolTCP.move(
						linMovement.setJointVelocityRel(0.05)
						);
				
				
				// On dessine au dessus du papier les points P0 / P1 / P2 / P3 :
				
				getLogger().info("Move on Paper");
				
				/*RelativeLIN [] splineArray = new RelativeLIN[frames.length-1];
				
				for (int i=0; i < frames.length-1; i++)
				{
					RelativeLIN moveLin2 = linRel(getTranslationFromFrame(frames[i], frames[i+1]),paperBase);
					
					splineArray[i] = moveLin2;
				}
				
				Spline linMovement2 = new Spline(splineArray);
				
				penToolTCP.move(
						linMovement2.setJointVelocityRel(velocity)
					);*/
				
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
				
				this.serveur.setMessage("");
			}
		}
		
		// Initialise la position du robot
		getLogger().info("Go back to home");
		
		penToolTCP.move( lin(paperApproach).setJointVelocityRel(velocity));
		
		SplineJP moveBackToHome = new SplineJP( ptpHome());
		
		getLogger().info("Move Back");
		lbr_iiwa_14_R820_1.move(
				moveBackToHome.setJointVelocityRel(velocity)
			);
		
		ioFlange.setLEDBlue(false);
	}

	
	public static void main(String[] args) {
		TestBaseMove app = new TestBaseMove();
		app.runApplication();
		
	}
}
