package dessin;

import static com.kuka.roboticsAPI.motionModel.BasicMotions.linRel;

import java.util.ArrayList;
import java.util.Iterator;

import org.json.*;

import application.BezierCurve;
import application.Vector2;

import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.geometricModel.ObjectFrame;
import com.kuka.roboticsAPI.geometricModel.math.Transformation;
import com.kuka.roboticsAPI.motionModel.RelativeLIN;

public class Svg {

	/**
	 * Attributs
	 */
	private ObjectFrame paperBase;
	private ObjectFrame paperApproach;
	private int widthSheet; 
	private int heightSheet;
	private double scaleX;
	private double scaleY;
	private Frame currentPoint;
	
	/**
	 * Constructeur par defaut
	 */
	public Svg(){
		
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
	public Svg(ObjectFrame pA, ObjectFrame pB, int wS, int hS, double sX, double sY) {
		paperApproach = pA;
		paperBase = pB;
		widthSheet = wS;
		heightSheet = hS;
		scaleX = sX;
		scaleY = sY;
		currentPoint = new Frame (pA.getX(), pA.getY(), pA.getZ());
	}
	
	/**
	 * Fonction qui dessine
	 * @param jSvgObject
	 * @return RelativeLIN[]
	 * @throws JSONException 
	 */
	@SuppressWarnings("rawtypes")
	public RelativeLIN[] draw(JSONObject jSvgObject) throws JSONException {
		// ArrayList
		ArrayList<RelativeLIN[]> linMovements = new ArrayList<RelativeLIN[]>();
		RelativeLIN[] linMovement = null;

		/*// Récupère le M
		JSONArray jMArray = jSvgObject.getJSONArray("M1");
		// On créé la première frame
		Frame firstFrame = new Frame(jMArray.getInt(0) * widthSheet / scaleX, heightSheet - jMArray.getInt(1) * heightSheet / scaleY, 10.0);
		// On créé la frame correspondante sur le papier
		Frame firstFrameOnPaper = new Frame(jMArray.getInt(0) * widthSheet / scaleX, heightSheet - jMArray.getInt(1) * heightSheet / scaleY, -3.0);
		currentPoint = firstFrameOnPaper;
		RelativeLIN[] splineArray = new RelativeLIN[2];
		// On approche de la feuille à 10 au dessus du point init
		splineArray[0] = linRel(getTranslationFromFrame(new Frame(paperApproach.getX(),paperApproach.getY(), paperApproach.getZ()), firstFrame), paperBase);
		// Le stylo touche la feuille
		splineArray[1] = linRel(getTranslationFromFrame(firstFrame, firstFrameOnPaper), paperBase);
		// Ajoute la spline
		linMovements.add(splineArray);*/
		
		System.out.println("Début parcours JSON");
		
		// Parcours les objets JSON 
		for (Iterator iterator = jSvgObject.sortedKeys(); iterator.hasNext();) {
			// Récupère la clé (M, c, l,...)    
			String cle = String.valueOf(iterator.next());
			
			System.out.println("Cle : " + cle);
			
			// Récupère le tableau de valeur de la cle
			JSONArray jArray = jSvgObject.getJSONArray(cle);
			
			// Appel la fonction de traitement 
			if     (cle.contains("c")) linMovements.add(this.JsonKeyc(jArray));
			else if(cle.contains("l")) linMovements.add(this.JsonKeyl(jArray));
			else if(cle.contains("M")) linMovements.add(this.JsonKeyM(jArray));
			else if(cle.contains("m")) linMovements.add(this.JsonKeym(jArray));
		}
		
		System.out.println("Fin parcours JSON");
		
		// Recupere le nombre d'element
		int nbElements = 0;
		for(RelativeLIN[] relLin : linMovements){				
			nbElements += relLin.length;
		}
		
		//
		linMovement = new RelativeLIN[nbElements];
		for(RelativeLIN[] relLin : linMovements){
			for (int i = 0; i < relLin.length; i++) {
				linMovement[i] = relLin[i];
			}
		}
		
		System.out.println("Fin ajout des RelativeLIN");
	    
		return linMovement;
	}
	
	/**
	 * Traitement de la lettre M
	 * @param jMArray
	 * @return RelativeLIN[]
	 */	
	private RelativeLIN[] JsonKeyM(JSONArray jMArray) {
		RelativeLIN[] splineArray = null;
		
		System.out.println("Debut JsonKeyM");
		
		try {
			splineArray = new RelativeLIN[3];
			
			// On créé la première frame
			//Frame upFrame = new Frame(currentPoint.getX(), currentPoint.getY(), 10.0);
			Frame upFrame = new Frame(paperApproach.getX(), paperApproach.getY(), 10.0);
			// On créé la frame du premier point M à 10 au dessus du papier
			Frame firstFrame = new Frame(jMArray.getInt(0) * widthSheet / scaleX, heightSheet - jMArray.getInt(1) * heightSheet / scaleY, 10.0);
			// On créé la frame de M correspondante sur le papier
			Frame firstFrameOnPaper = new Frame(jMArray.getInt(0) * widthSheet / scaleX, heightSheet - jMArray.getInt(1) * heightSheet / scaleY, -3.0);
			// Met à jours le current point 
			currentPoint = firstFrameOnPaper;
			
			// On approche de la feuille à 10 au dessus 
			splineArray[0] = linRel(getTranslationFromFrame(currentPoint, upFrame), paperBase);
			// On se deplace vers le premier point toujours à 10 au dessus
			splineArray[1] = linRel(getTranslationFromFrame(upFrame, firstFrame), paperBase);			
			// Le stylo touche la feuille
			splineArray[2] = linRel(getTranslationFromFrame(firstFrame, firstFrameOnPaper), paperBase);
			
		} catch (JSONException e) {	e.printStackTrace(); }
		
		System.out.println("Fin JsonKeyM");

		return splineArray;
	}
	
	/**
	 * Traitement de la lettre M
	 * @param jmArra
	 * @return RelativeLIN[]
	 */	
	private RelativeLIN[] JsonKeym(JSONArray jmArray) {
		RelativeLIN[] splineArray = null;
		
		System.out.println("Debut JsonKeym");
		
		try {
			splineArray = new RelativeLIN[3];
			
			// On créé la première frame
			Frame upFrame = new Frame(currentPoint.getX(), currentPoint.getY(), 10.0);
			// On créé la frame du premier point M à 10 au dessus du papier
			Frame firstFrame = new Frame(jmArray.getInt(0) * widthSheet / scaleX, heightSheet - jmArray.getInt(1) * heightSheet / scaleY, 10.0);
			// On créé la frame de M correspondante sur le papier
			Frame firstFrameOnPaper = new Frame(jmArray.getInt(0) * widthSheet / scaleX, heightSheet - jmArray.getInt(1) * heightSheet / scaleY, -3.0);
			// Met à jours le current point 
			currentPoint = firstFrameOnPaper;
			
			// On approche de la feuille à 10 au dessus 
			splineArray[0] = linRel(getTranslationFromFrame(currentPoint, upFrame), paperBase);
			// On se deplace vers le premier point toujours à 10 au dessus
			splineArray[1] = linRel(getTranslationFromFrame(upFrame, firstFrame), paperBase);			
			// Le stylo touche la feuille
			splineArray[2] = linRel(getTranslationFromFrame(firstFrame, firstFrameOnPaper), paperBase);
			
		} catch (JSONException e) {	e.printStackTrace(); }
		
		System.out.println("Fin JsonKeym");

		return splineArray;
	}

	/**
	 * Traitement de la lettre c
	 * @param jCArray
	 * @return RelativeLIN[]
	 */
	private RelativeLIN[] JsonKeyc(JSONArray jCArray){
		RelativeLIN[] splineArray = null;
		
		System.out.println("Debut JsonKeyc");
		
		try {
			// On récupère l'ensemble des points qu'on stock dans un tableau de Vector2
			Vector2[] bezierControlPoints = new Vector2[jCArray.length() / 2];
			
			int i = 0;
			int cpt = 0;
			while (cpt < jCArray.length()) {
				bezierControlPoints[i] = new Vector2(currentPoint.getX() + jCArray.getInt(cpt) * widthSheet / scaleX, heightSheet - (currentPoint.getY() + jCArray.getInt(++cpt) * heightSheet / scaleY));
				cpt++;
				i++;
			}
			
			BezierCurve curve = new BezierCurve(bezierControlPoints);
			Vector2[] trajectory = curve.getTrajectory(40);
			// On crée des frames robot Kuka depuis notre courbe
			Frame[] frames = new Frame[trajectory.length];
			for (i = 0; i < trajectory.length; i++) {	
				frames[i] = new Frame(trajectory[i].x, trajectory[i].y, 0.0);
			}

			splineArray = new RelativeLIN[frames.length-1];
			for (i = 0; i < frames.length-1; i++) {
				splineArray[i] = linRel(getTranslationFromFrame(frames[i], frames[i+1]),paperBase);
			}
			
		} catch (JSONException e) {	e.printStackTrace(); }
		
		System.out.println("Fin JsonKeyc");
		
		return splineArray;
	}
	
	/**
	 * Traitement de la lettre l
	 * @param jLArray
	 * @return RelativeLIN[]
	 */
	private RelativeLIN[] JsonKeyl(JSONArray jLArray){
		RelativeLIN[] splineArray = null;
		
		System.out.println("Debut JsonKeyl");
		
		try {
			// On récupère l'ensemble des points qu'on stock dans un tableau de Vector2
			Frame[] linePoints = new Frame[jLArray.length() / 2];
			
			int i = 0;
			int cpt = 0;
			while (cpt < jLArray.length()) {
				linePoints[i] = new Frame(currentPoint.getX() + jLArray.getInt(cpt) * widthSheet / scaleX, heightSheet - (currentPoint.getY() + jLArray.getInt(++cpt) * heightSheet / scaleY), 0.0);
				currentPoint = linePoints[i];
				cpt++;
				i++;
			}

			splineArray = new RelativeLIN[linePoints.length-1];
			for (i = 0; i < linePoints.length-1; i++) {
				splineArray[i] = linRel(getTranslationFromFrame(linePoints[i], linePoints[i+1]),paperBase);
			}
			
		} catch (JSONException e) {	e.printStackTrace(); }
		
		System.out.println("Fin JsonKeyl");
		
		return splineArray;
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
}

