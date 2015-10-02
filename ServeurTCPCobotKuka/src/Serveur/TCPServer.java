package Serveur;

import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.ClassNotFoundException;
import java.net.ServerSocket;
import java.net.Socket;
 
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
    private boolean isRun = false; // Boolean qui indique si le serveur est en marche
    
    /**
     * Constructeur par defaut
     * @throws IOException 
     */
    public TCPServer() throws IOException {
        port = 9191; // Port socket du serveur
        message = ""; // Initialisation du message
        sizeChar = 4096; // Fixe la taille du nombre de caractere
        characters = new char[sizeChar]; // Chaine de caractere
        // Demarre le serveur
        try {
			server = new ServerSocket(port); // Créer un objet ServerSocket
			isRun = true; // Serveur en marche
	        System.out.println("Serveur connecté"); // Log le démarrage du serveur socket
		} catch (IOException e) { closeServer(); System.out.println("Erreur : " + e); } 
    }
    
    /**
     * Constructeur surcharge 
     * @param port : port d'ecoute du serveur pour le client IHM
     * @param sizeBuffer : taille du tableau de char pour la lecture du buffer
     * @throws IOException
     */
    public TCPServer(int p, int sizeBuffer) throws IOException {
        port = p; // Port socket du serveur
        message = ""; // Initialisation du message
        sizeChar = sizeBuffer; // Fixe la taille du nombre de caractere
        characters = new char[sizeChar]; // Chaine de caractere
        // Demarre le serveur
        try {
			server = new ServerSocket(port); // Créer un objet ServerSocket  
			isRun = true; // Serveur en marche
	        System.out.println("Serveur connecté"); // Log le démarrage du serveur socket
		} catch (IOException e) { closeServer(); System.out.println("Erreur : " + e); }   
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
	        int bytesread = isr.read(characters);

	        // Converti le char en string
		    message = new String(characters);
		    // Recupère la sous chaine du message
		    message = message.substring(0, bytesread);
		    
		    // Initialise le char
		    characters = new char[sizeChar]; 
		    // Ferme les ressources
	        isr.close();
	        socket.close();
	
	        System.out.println("Message du client (IHM) : " +  getMessage()); // Log le message
        } catch(Exception e){
        	message = ""; // Vide le message
        	System.out.println("Erreur lors de la réception du message du client (IHM) : " + e); // Log le message d'erreur
        }
    }
   
    /**
     * Accesseurs get message
     * @return String (message client)
     */
	public String getMessage() {
		return message;
	}
    
    /**
     * Accesseurs get isRun
     * @return boolean (serveur en cours ?)
     */
	public boolean getIsRun() {
		return isRun;
	}
    
	/**
	 * Fonction qui ferme le serveur tcp
	 * @throws IOException
	 */
    public void closeServer() throws IOException{
        System.out.println("Serveur arreté"); // Log l'arret du serveur socket
    	isRun = false; // Serveur en marche
    	server.close(); // Arrete le serveur
    }
}