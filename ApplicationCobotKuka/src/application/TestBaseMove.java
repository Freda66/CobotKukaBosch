package application;

import dessin.*;

import org.json.*;

import serveur.TCPServer;

import com.kuka.generated.ioAccess.MediaFlangeIOGroup;
import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.*;
import com.kuka.roboticsAPI.controllerModel.Controller;
import com.kuka.roboticsAPI.controllerModel.sunrise.ISafetyState;
import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.deviceModel.OperationMode;
import com.kuka.roboticsAPI.geometricModel.CartDOF;
import com.kuka.roboticsAPI.geometricModel.ObjectFrame;
import com.kuka.roboticsAPI.geometricModel.Tool;
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

	private static int widthSheet = 297;
	private static int heightSheet = 210;
	
	/**
	 * 
	 */
	public void initialize() {
		// Créer l'objet serveur tcp pour recevoir les commandes de dessin
		serveur = new TCPServer(30004);
		
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
				
		getLogger().info("Initialization OK");
	}

	/**
	 * 
	 */
	public void run() {
		Spline linMovement = null;
		
		try {
			double velocity = 0.2;
			
			ISafetyState currentState = lbr_iiwa_14_R820_1.getSafetyState();
			OperationMode mode = currentState.getOperationMode();
			if (mode==OperationMode.AUT)
			{
				//******* ATTENTION : AVANT D'ACTIVE LE MODE AUTO, VERIFIER LES TRAJECTOIRES ********//
				velocity = 1; // Si on est en mode AUTO, pleine vitesse activée
			}
			
			//Init position
			lbr_iiwa_14_R820_1.move(ptpHome());
			
			
			ioFlange.setLEDBlue(true);
			
			
			// Approche de la base "Paper" en PTP
			
			getLogger().info("Move near Paper");
					
			penToolTCP.move(
					ptp(paperApproach).setJointVelocityRel(velocity)
				);
			
			// Message JSON du serveur
			String message = "";
			// Boolean qui indique la fin du dessin et du programme
			boolean end = false;
			// Tant que le boolean est faux on reste dans la boucle
			while(!end)
			{
				getLogger().info("Serveur en attente de commandes");
				
				// Attend la connexion et le message du client
				this.serveur.run();
				
				// Récupère le message du serveur 
				message = this.serveur.getMessage();
				
				// Verifie le message du client
				if(message == "stop") {
					this.serveur.closeServer(); // Arrete le serveur
					end = true; // Indique que la boucle est terminée
				} 
				// Si le message est différent de vide on rentre dans la condition / Sinon on attend un nouveau message du client
				else if (message != ""){
	
					getLogger().info("Message du client : " + this.serveur.getMessage());
					
					// Initialise le mouvement à faire
					linMovement = null;
					
					try {
						// On récupère la chaîne de caractère qu'on converti en JSON
						JSONObject jObject = new JSONObject(message);
						// On récupère l'objet pointé par "scale"
						JSONArray jScaleArray = null;
						if(jObject.has("scale")) jScaleArray = jObject.getJSONArray("scale");
						// On récupère l'objet pointé par "translate"
						//JSONArray jTranslateArray = jObject.getJSONArray("translate");
						
						// On récupère l'objet pointé par "pt" ou "px"
						JSONArray jTypeArray= null;
						double px = 1;
						if(jObject.has("pt")) jTypeArray = jObject.getJSONArray("pt");
						else {
							jTypeArray = jObject.getJSONArray("px");
							px = 0.75;
						}
						// On récupère l'objet pointé par "width"
						double width = jTypeArray.getDouble(0);
						// On récupère l'objet pointé par "height"
						double height = jTypeArray.getDouble(1);
						// Calcul le scaleX et scaleY
						double scaleX = height * px;
						double scaleY = width * px;
						if(jScaleArray != null){
							scaleX = scaleX / Math.abs(jScaleArray.getDouble(0));
							scaleY = scaleY / Math.abs(jScaleArray.getDouble(1));
						}
						
						// Log
						getLogger().info("WidthSheet : " + widthSheet + " | HeightSheet : " + heightSheet + " | ScaleX : " + scaleX + " | ScaleY : " + scaleY + "Px : " + px + " | Height dessin : " + height + " Width dessin : " + width);
						
						// On récupère l'objet pointé par "svg"
						if(jObject.has("svg")){
							// Crée un objet SVG
							Svg svg = new Svg(paperApproach, paperBase, widthSheet, heightSheet, scaleX, scaleY);
							
							// Récupère l'objet SVG
							JSONObject jSvgObject = jObject.getJSONObject("svg");
							
							// Appel la fonction pour dessiner l'image
							linMovement = new Spline(svg.draw(jSvgObject));
							
						} else if(jObject.has("webcam")){
							// Crée un objet CAM
							Webcam cam = new Webcam(paperApproach, paperBase, widthSheet, heightSheet, scaleX, scaleY);
							
							// Récupère l'objet CAM
							JSONArray jCamArray = jObject.getJSONArray("webcam");
							
							// Appel la fonction pour dessiner l'image
							linMovement = new Spline(cam.draw(jCamArray));	
							
						}
					} catch (JSONException e) {
						e.printStackTrace();
					}					
					
					// Dessine le message
					if(linMovement != null) penToolTCP.move(linMovement.setJointVelocityRel(velocity));
				}
			}
			
			// On retourne à la position initiale du robot
			getLogger().info("Retour position initiale");
			
			penToolTCP.move(lin(paperApproach).setJointVelocityRel(velocity));
			
			SplineJP moveBackToHome = new SplineJP( ptpHome());
			
			getLogger().info("Move Back");
			lbr_iiwa_14_R820_1.move(moveBackToHome.setJointVelocityRel(velocity));
			
			ioFlange.setLEDBlue(false);
			
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
