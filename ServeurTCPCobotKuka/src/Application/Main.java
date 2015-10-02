package Application;

import java.io.IOException;
import java.lang.ClassNotFoundException;

import Serveur.*;
import Client.*;

/**
 * Point d'entr� de l'application (d�but du programme)
 * @author Bosch, Berriche, Cano, Danjoux, Durand, Olivieri
 * @date 01/10/2015
 */
public class Main {
    
    public static void main(String args[]) throws IOException, ClassNotFoundException {
    	// Cr�er le serveur pour �couter les commandes de dessin du client
    	TCPServer server = new TCPServer(9191, 4096); // Port du serveur (moi), longueur du char pour le buffer 
    	// Cr�er le client pour communiquer avec le serveur kuka
    	TCPClient client = new TCPClient("192.168.1.7", 30004); // Ip et port du robot kuka
    	
    	// Gestion du serveur
    	if(server.getIsRun()){
    		// Tant qu'on ne demande pas d'arreter le robot et le serveur
	    	while(server.getMessage() != "stop"){
	    		// Attente d'un message par le client (IHM)
	    		server.listenSocket();
	    		// Envoi le message au serveur kuka
	    		if(server.getMessage() != "") client.sendMessage(server.getMessage());
	    	}
    	}
    	
    	// Ferme le server
    	server.closeServer();
    }
     
}