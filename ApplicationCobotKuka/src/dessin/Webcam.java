package dessin;

import static com.kuka.roboticsAPI.motionModel.BasicMotions.linRel;

import java.util.ArrayList;

import org.json.*;

import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.geometricModel.ObjectFrame;
import com.kuka.roboticsAPI.geometricModel.math.Transformation;
import com.kuka.roboticsAPI.motionModel.RelativeLIN;

public class Webcam {

    /**
    * Attributs
    */
    private ObjectFrame paperApproach; 
    private ObjectFrame paperBase;
    private int widthSheet; 
    private int heightSheet;
    private double scaleX;
    private double scaleY;
    private double translateX;
    private double translateY;
    
    /**
    * Constructeur par defaut
    */
    public Webcam(){
                   
    }
    
    /**
    * Constructeur surcharge
    * @param pA
    * @param pB
    * @param wS
    * @param hS
    * @param sX
    * @param sY
    */
    public Webcam(ObjectFrame pA, ObjectFrame pB, int wS, int hS, double sX, double sY, double tX, double tY){                               
       paperApproach = pA;
       paperBase = pB;
       widthSheet = wS;
       heightSheet = hS;
       scaleX = sX;
       scaleY = sY;
       translateX = tX;
       translateY = tY;
    }

    public RelativeLIN[] draw (JSONArray jCamArray) {
    	RelativeLIN[] linMovement = null;
    	try {
    		ArrayList<RelativeLIN> t_linMovement = new ArrayList<RelativeLIN>();
	    	Frame t_firstPoint = null;
			Frame t_secondPoint = null;

			// On parcours les courbes
	    	for (int i = 0; i < jCamArray.length(); i++) {
	    		// On récupère la courbe i
	    		JSONArray jCamArray2 = jCamArray.getJSONArray(i);
	    		// On parcours les coordonnées de la courbe i
	    		for (int j = 0; j < jCamArray2.length() - 4; j += 2) {
	    			// Si on est au premier point du dessin, on se déplace vers lui...
	    			if (i == 0 && j == 0) {
	    				//...d'abord à 10 du papier à partir du paperApproach...
				    	t_firstPoint = new Frame(paperApproach.getX(), paperApproach.getY(), paperApproach.getZ());
				    	t_secondPoint = new Frame(scalePointX(widthSheet - jCamArray2.getInt(0)), scalePointY(jCamArray2.getInt(1)), 10.0);
				    	t_linMovement.add(linRel(getTranslationFromFrame(t_firstPoint, t_secondPoint), paperBase));
				    	//...ensuite en faisant toucher le stylo au papier
				    	t_firstPoint = t_secondPoint;
				    	t_secondPoint = new Frame(scalePointX(widthSheet - jCamArray2.getInt(0)), scalePointY(jCamArray2.getInt(1)), -3.0);
				    	t_linMovement.add(linRel(getTranslationFromFrame(t_firstPoint, t_secondPoint), paperBase));
				    // Si on est au début de la courbe suivante
	    			} else if (i != 0 && j == 0) {
	    				// On remonte le stylo
	    				t_firstPoint = t_secondPoint;
	    				t_secondPoint = new Frame(t_secondPoint.getX(), t_secondPoint.getY(), 10.0);;
	    				t_linMovement.add(linRel(getTranslationFromFrame(t_firstPoint, t_secondPoint), paperBase));
	    				// On avance vers le point suivante
	    				t_firstPoint = t_secondPoint;
				    	t_secondPoint = new Frame(scalePointX(widthSheet - jCamArray2.getInt(0)), scalePointY(jCamArray2.getInt(1)), 10.0);
	    				t_linMovement.add(linRel(getTranslationFromFrame(t_firstPoint, t_secondPoint), paperBase));
	    				// On redescend vers le point init de la courbe suivante
	    				t_firstPoint = t_secondPoint;
				    	t_secondPoint = new Frame(scalePointX(widthSheet - jCamArray2.getInt(0)), scalePointY(jCamArray2.getInt(1)), -3.0);
	    				t_linMovement.add(linRel(getTranslationFromFrame(t_firstPoint, t_secondPoint), paperBase));
	    			}
	    			// Dans tous les cas on parcours les coordonnées pour créer les liaisons entre les points des courbes
	    			t_firstPoint = new Frame(scalePointX(widthSheet - jCamArray2.getInt(j)), scalePointY(jCamArray2.getInt(j + 1)), 0.0);
	    			t_secondPoint = new Frame(scalePointX(widthSheet - jCamArray2.getInt(j + 2)), scalePointY(jCamArray2.getInt(j + 3)), 0.0);
	    			t_linMovement.add(linRel(getTranslationFromFrame(t_firstPoint, t_secondPoint), paperBase));
	    			
	    			// Si on est au dernier point de la courbe on releve le stylo en restant ou l'on se trouve (si non trace sur le papier)
	    			if(j == jCamArray2.length()-2){
		    			t_linMovement.add(linRel(getTranslationFromFrame(
		    					new Frame(scalePointX(widthSheet - jCamArray2.getInt(j + 2)), scalePointY(jCamArray2.getInt(j + 2)), 0.0),
		    					new Frame(scalePointX(widthSheet - jCamArray2.getInt(j + 2)), scalePointY(jCamArray2.getInt(j + 2)), 10.0)
		    					), paperBase));
	    			}
				}
				// On reboucle à la fin entre le dernier point de la courbe en cours et le point init de cette meme courbe
				t_firstPoint = new Frame(scalePointX(widthSheet - jCamArray2.getInt(jCamArray2.length() - 1)), scalePointY(jCamArray2.getInt(jCamArray2.length() - 1)), 0.0);
				t_secondPoint = new Frame(scalePointX(widthSheet - jCamArray2.getInt(0)), scalePointY(jCamArray2.getInt(1)), 0.0);
				t_linMovement.add(linRel(getTranslationFromFrame(t_firstPoint, t_secondPoint), paperBase));
	    	}
	    	
		    linMovement = new RelativeLIN[t_linMovement.size()];
		    for (int i = 0; i < t_linMovement.size(); i++) {
				linMovement[i] = t_linMovement.get(i);
			}
		    
    	} catch (JSONException e) { e.printStackTrace(); }
    	
		return linMovement;
    }
    
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
	
	private double scalePointX(double brutPointX) {
		return brutPointX * widthSheet / scaleX; 
	}
	
	private double scalePointY(double brutPointY) {
		return brutPointY * heightSheet / scaleY; 
	}
}
