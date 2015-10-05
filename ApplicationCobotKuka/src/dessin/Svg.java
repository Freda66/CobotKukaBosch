package dessin;

import static com.kuka.roboticsAPI.motionModel.BasicMotions.linRel;

import java.util.ArrayList;

import org.json.JSONException;

import application.BezierCurve;
import application.Vector2;

import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.geometricModel.ObjectFrame;
import com.kuka.roboticsAPI.motionModel.RelativeLIN;
import com.kuka.roboticsAPI.motionModel.Spline;

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
	 * @return
	 * @throws JSONException 
	 */
	public RelativeLIN[] draw(org.json.JSONObject jSvgObject) throws JSONException{
		// ArrayList
		ArrayList<RelativeLIN> LinMovement = new ArrayList<RelativeLIN>();
		
		// Récupère le M
		org.json.JSONArray jMArray = jSvgObject.getJSONArray("M");
		
		//LinMovement.add(this.JsonKeyc());
		return this.JsonKeyc(jMArray, jSvgObject);
		
		/*
			//  On approche de la feuille à 10 au dessus du point init
			splineArray[0] = linRel(getTranslationFromFrame(new Frame(paperApproach.getX(),paperApproach.getY(), paperApproach.getZ()), firstFrame), paperBase);
			// Le stylo touche la feuille
			splineArray[1] = linRel(getTranslationFromFrame(firstFrame, firstFrameOnPaper), paperBase);
			
			for (i = 0; i < frames.length-1; i++)
			{
				RelativeLIN moveLin = linRel(getTranslationFromFrame(frames[i], frames[i+1]),paperBase);
				splineArray[i+2] = moveLin;
			}
			
		
		return ;*/
	}
	
	private RelativeLIN[] JsonKeyc(org.json.JSONArray jMArray, org.json.JSONObject jSvgObject){
		// On créé la première frame
		Frame firstFrame = new Frame(jMArray.getInt(0) * widthSheet / scaleX, heightSheet - jMArray.getInt(1) * heightSheet / scaleY, 10.0);
		// On créé la frame correspondante sur le papier
		Frame firstFrameOnPaper = new Frame(jMArray.getInt(0) * widthSheet / scaleX, heightSheet - jMArray.getInt(1) * heightSheet / scaleY, -3.0);
		
		
		// On récupère l'Array des points de la courbe de bezier
		org.json.JSONArray jCArray = jSvgObject.getJSONArray("c");
		// On récupère l'ensemble des points qu'on stock dans un tableau de Vector2
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
		// On crée des frames robot Kuka depuis notre courbe
		frames = new Frame[trajectory.length];
		for (i = 0; i < trajectory.length; i++)
		{	
			frames[i] = new Frame(trajectory[i].x, trajectory[i].y, 0.0);
		}
		
		RelativeLIN [] splineArray = new RelativeLIN[frames.length+1];
		//  On approche de la feuille à 10 au dessus du point init
		splineArray[0] = linRel(getTranslationFromFrame(new Frame(paperApproach.getX(),paperApproach.getY(), paperApproach.getZ()), firstFrame), paperBase);
		// Le stylo touche la feuille
		splineArray[1] = linRel(getTranslationFromFrame(firstFrame, firstFrameOnPaper), paperBase);
		
		for (i = 0; i < frames.length-1; i++)
		{
			RelativeLIN moveLin = linRel(getTranslationFromFrame(frames[i], frames[i+1]),paperBase);
			splineArray[i+2] = moveLin;
		}
		
		return splineArray;
	}
}
