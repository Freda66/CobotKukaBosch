package serveur;

import java.io.IOException;
import java.io.ObjectInputStream;
//import java.io.ObjectOutputStream;
import java.lang.ClassNotFoundException;
import java.net.ServerSocket;
import java.net.Socket;
import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;

/**
 * Classe Serveur
 * @author Bosch, Berriche, Cano, Danjoux, Durand, Olivieri
 * @date 01/10/2015
 */
public class TCPServer extends RoboticsAPIApplication {
     
	/**
	 * Attributs
	 */
    private ServerSocket server;
    private int port = 30004;
    private String message = "";
    
    /**
     * Constructeur
     */
    public  TCPServer() {
        // Créer l'objet serveur socket
        try { 
        	server = new ServerSocket(port); 
        } catch (IOException e) { this.closeServer(); e.printStackTrace(); }    
    }

    /**
     * Fonction qui lance le serveur en attente d'un client
     */
	@Override
	public void run() {
		try {
	        //getLogger().info("Attente d'une requete du client"); // Log le message d'attente d'une requete
			// Attend une connexion d'un client
	        Socket socket = this.server.accept();
	        
	        // Créer un objet qui permet de lire dans la socket
	        ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
	        // Lit dans la socket et converti l'objet en string
	        this.setMessage((String) ois.readObject());
	        //getLogger().info("Message du client : " + this.getMessage()); // Log le message recu par le serveur du client
	   
	        // Crée un objet qui permet d'écrire dans la socket vers le client
	        //ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
	        // Ecrit dans la socket
	        //oos.writeObject("Ok");
	        
	        // Ferme les ressources
	        //oos.close();
	        ois.close();
	        socket.close();
		} catch (IOException e) { e.printStackTrace(); } catch (ClassNotFoundException e) { this.closeServer(); e.printStackTrace(); }
	}
 
	/**
	 * Fonction qui arrete le serveur
	 */
    public void closeServer()
    {
		try { 
			this.server.close(); 
		} catch (IOException e) { e.printStackTrace(); }
    }
	
	/**
	 * Setteur Message
	 */
    public void setMessage(String m)
    {
    	this.message = m;
    }
    
    /**
     * Accesseur Get
     * @return message : String
     */
    public String getMessage()
    {
    	return this.message;
    }	
}