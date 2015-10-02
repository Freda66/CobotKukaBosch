package Client;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
 
/**
 * Classe du client TCP
 * @author Bosch, Berriche, Cano, Danjoux, Durand, Olivieri
 * @date 01/10/2015
 */
public class TCPClient {
 
	/**
	 * Attributs
	 */
    Socket socket;
    ObjectOutputStream oos;
    ObjectInputStream ois;
    String ipServeurKuka;
    int portSocket;
    
    /**
     * Constructeur par defaut
     */
    public TCPClient(){
    	this.ipServeurKuka = "192.168.1.7"; 
    	this.portSocket = 30004;
    	socket = null;
    	oos = null;
    	ois = null;
    }
    
    /**
     * Constructeur surcharge
     * @param ip : IP du robot kuka
     * @param port : Port de communication du robot kuka
     */
    public TCPClient(String ip, int port){
    	this.ipServeurKuka = ip; 
    	this.portSocket = port;
    	socket = null;
    	oos = null;
    	ois = null;
    }
    
    /**
     * Fonction qui envoi un message au serveur
     * @throws IOException 
     * @throws UnknownHostException 
     * @throws ClassNotFoundException 
     */
    public void sendMessage(String message) throws UnknownHostException, IOException, ClassNotFoundException{
    	try {
    		// Créer la socket
	        socket = new Socket(ipServeurKuka, portSocket); 
	        
	        // Créer un objet OutputStream pour ecrire dans la socket
	        oos = new ObjectOutputStream(socket.getOutputStream());
	        // Ecrit le message
	        oos.writeObject(message);
	
	        // Ferme les ressources
	        oos.flush();
	        oos.close();
	        socket.close(); 
    	} catch(Exception e) {
        	System.out.println("Erreur lors de la connexion ou l'envoi du message au serveur kuka : " + e); // Log le message d'erreur
    	}
    }
}