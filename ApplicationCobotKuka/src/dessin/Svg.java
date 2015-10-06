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
	private ObjectFrame paperApproach; 
	private ObjectFrame paperBase;
	private Vector2[] bezierControlPoints;
	private BezierCurve curve;
	private Vector2[] trajectory;
	private Frame[] frames;
	private int widthSheet; 
	private int heightSheet;
	private double scaleX;
	private double scaleY;
	
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
	public Svg(ObjectFrame pA, ObjectFrame pB, int wS, int hS, double sX, double sY){		
		paperApproach = pA;
		paperBase = pB;
		widthSheet = wS;
		heightSheet = hS;
		scaleX = sX;
		scaleY = sY;
	}
	
	/**
	 * Fonction qui dessine
	 * @param jSvgObject
	 * @return RelativeLIN[]
	 * @throws JSONException 
	 */
	public RelativeLIN[] draw(JSONObject jSvgObject) throws JSONException {
		// ArrayList
		ArrayList<RelativeLIN[]> linMovements = new ArrayList<RelativeLIN[]>();
		RelativeLIN[] linMovement = null;

		// R�cup�re le M
		JSONArray jMArray = jSvgObject.getJSONArray("M");
		
		System.out.println("Avant parcours JSON");
		
		// Parcours les objets JSON 
		for (@SuppressWarnings("rawtypes") Iterator iterator = jSvgObject.keys(); iterator.hasNext();) {
			// R�cup�re la cl� (M, c, l,...)    
			String cle = String.valueOf(iterator.next());
			
			System.out.println("Cle : " + cle);
			
			// R�cup�re le tableau de valeur de la cle
			JSONArray jArray = jSvgObject.getJSONArray(cle);
			
			if     (cle.equals("c")) linMovements.add(this.JsonKeyc(jMArray, jArray));
			//else if(cle.equals("l")) linMovements.add(this.JsonKeyl(jMArray, val));
			//else if(cle.equals("m")) linMovements.add(this.JsonKeym(jMArray, val));
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
	
	private RelativeLIN[] JsonKeyc(JSONArray jMArray, JSONArray jCArray){
		RelativeLIN[] splineArray = null;
		
		System.out.println("Debut JsonKeyc");
		
		try {
			// On cr�� la premi�re frame
			Frame firstFrame = new Frame(jMArray.getInt(0) * widthSheet / scaleX, heightSheet - jMArray.getInt(1) * heightSheet / scaleY, 10.0);
			System.out.println(firstFrame.toString());
			
			// On cr�� la frame correspondante sur le papier
			Frame firstFrameOnPaper = new Frame(jMArray.getInt(0) * widthSheet / scaleX, heightSheet - jMArray.getInt(1) * heightSheet / scaleY, -3.0);
			System.out.println(firstFrameOnPaper.toString());
			
			// On r�cup�re l'ensemble des points qu'on stock dans un tableau de Vector2
			bezierControlPoints = new Vector2[jCArray.length() / 2];
			
			int i = 0;
			int cpt = 0;
			while (cpt < jCArray.length()) {
				bezierControlPoints[i] = new Vector2((jMArray.getInt(0) + jCArray.getInt(cpt)) * widthSheet / scaleX, heightSheet - (jMArray.getInt(1) + jCArray.getInt(++cpt)) * heightSheet / scaleY);
				cpt++;
				i++;
			}
			
			curve = new BezierCurve(bezierControlPoints);
			trajectory = curve.getTrajectory(40);
			// On cr�e des frames robot Kuka depuis notre courbe
			frames = new Frame[trajectory.length];
			for (i = 0; i < trajectory.length; i++) {	
				frames[i] = new Frame(trajectory[i].x, trajectory[i].y, 0.0);
			}

			splineArray = new RelativeLIN[frames.length+1];
			// On approche de la feuille � 10 au dessus du point init
			splineArray[0] = linRel(getTranslationFromFrame(new Frame(paperApproach.getX(),paperApproach.getY(), paperApproach.getZ()), firstFrame), paperBase);
			// Le stylo touche la feuille
			splineArray[1] = linRel(getTranslationFromFrame(firstFrame, firstFrameOnPaper), paperBase);
			
			for (i = 0; i < frames.length-1; i++) {
				RelativeLIN moveLin = linRel(getTranslationFromFrame(frames[i], frames[i+1]),paperBase);
				splineArray[i+2] = moveLin;
			}

			/*
			//  On approche de la feuille � 10 au dessus du point init
			splineArray[0] = linRel(getTranslationFromFrame(new Frame(paperApproach.getX(),paperApproach.getY(), paperApproach.getZ()), firstFrame), paperBase);
			// Le stylo touche la feuille
			splineArray[1] = linRel(getTranslationFromFrame(firstFrame, firstFrameOnPaper), paperBase);
			
			for (i = 0; i < frames.length-1; i++)
			{
				RelativeLIN moveLin = linRel(getTranslationFromFrame(frames[i], frames[i+1]),paperBase);
				splineArray[i+2] = moveLin;
			}
			*/
			
		} catch (JSONException e) {	e.printStackTrace(); }
		
		System.out.println("Fin JsonKeyc");
		
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
