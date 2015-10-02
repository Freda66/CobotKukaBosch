package Serveur;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.lang.ClassNotFoundException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Arrays;
 
/**
 * Classe du serveur TCP
 * @author Bosch, Berriche, Cano, Danjoux, Durand, Olivieri
 * @date 01/10/2015
 */
public class TCPServer {
    
	/**
	 * Attributs
	 */
    private ServerSocket server; // Variable static du ServerSocket 
    private int port; // Port d'ecoute du serveur socket 
    private String message; // Message client
    private char[] characters; // Message du buffer
    private int sizeChar; // Taille max du message
    
    /**
     * Constructeur
     * @throws IOException 
     */
    public TCPServer() throws IOException {
        port = 9191; // Port socket du serveur
        message = ""; // Initialisation du message
        server = new ServerSocket(port); // Créer un objet ServerSocket
        sizeChar = 4096; // Fixe la taille du nombre de caractere
        characters = new char[sizeChar]; // Chaine de caractere
        System.out.println("Serveur connecté"); // Log le démarrage du serveur socket
    }
    
    /**
     * Fonction qui attend une connexion et ecoute le message du client
     * @throws ClassNotFoundException 
     * @throws IOException 
     */
    public void listenSocket() throws ClassNotFoundException, IOException {
        try {
        	// Créer la socket du serveur et attend une connexion du client
	        Socket socket = server.accept();
	        
	        // Créer un objet pour lire le message du client dans la socket
	        InputStreamReader isr = new InputStreamReader(socket.getInputStream(),"utf-8");
	        // Lit le buffer
	        isr.read(characters);
	        // Converti le char en string
		    message = new String(characters);
		    
		    // Initialise le char
		    characters = new char[sizeChar]; 
		    // Ferme les ressources
	        isr.close();
	        socket.close();
	
	        System.out.println("Message du client (IHM) : " +  this.getMessage()); // Log le message
        } catch(Exception e){
        	 this.setMessage(""); // Vide le message
        	System.out.println("Erreur lors de la réception du message du client (IHM) : " + e); // Log le message d'erreur
        }
    }
   
    /**
     * Accesseurs get
     * @return String (message client)
     */
	public String getMessage() {
		return message;
	}

	/**
	 * Accesseur set
	 * @param message
	 */
	public void setMessage(String message) {
		this.message = message;
	}
    
	/**
	 * Fonction qui ferme le serveur tcp
	 * @throws IOException
	 */
    public void closeServer() throws IOException{
    	this.server.close();
        System.out.println("Serveur arreté"); // Log l'arret du serveur socket
    }
}