package application;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.lang.ClassNotFoundException;
import java.net.ServerSocket;
import java.net.Socket;
 

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
 * Classe Serveur
 * @author Bosch, Berriche, Cano, Danjoux, Durand, Olivieri
 *
 */
public class TCPServer extends RoboticsAPIApplication {
     
    //static ServerSocket variable
    private  ServerSocket server;
    //socket server port on which it will listen
    private  int port = 30004;
    private String message="";
    public  TCPServer() throws IOException{
        //create the socket server object
        server = new ServerSocket(port);    
    }
    
    public void setMessage(String newvalue)
    {
    	this.message=newvalue;
    }
    
    public String getMessage()
    {
    	return this.message;
    }
    
    public void closeServer()
    {
    	try {
			this.server.close();
		} catch (IOException e) {
			// TODO Bloc catch généré automatiquement
			e.printStackTrace();
		}
    }

	@Override
	public void run() {
		//keep listens indefinitely until receives 'exit' call or program terminates
		try {
	        getLogger().info("Waiting for FRED client request");
	        //creating socket and waiting for client connection
	        Socket socket = this.server.accept();
	        
	        //read from socket to ObjectInputStream object
	        ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
	        //convert ObjectInputStream object to String
	        String lemessage = (String) ois.readObject();
	        getLogger().info("Message Received from Fred PC : " + lemessage);
	   
	        this.setMessage(message);
	        //create ObjectOutputStream object
	        ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
	        //write object to Socket
	        oos.writeObject(message);
	        //close resources
	        ois.close();
	        oos.close();
	        socket.close();
	        
	        //terminate the server if client sends exit request
	        if(message.equalsIgnoreCase("exit")) return;
	    
	        getLogger().info("Shutting down Socket server!!");
	        
		} catch (IOException e) {
			// TODO Bloc catch généré automatiquement
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			// TODO Bloc catch généré automatiquement
			e.printStackTrace();
		}
	}
       
	
}