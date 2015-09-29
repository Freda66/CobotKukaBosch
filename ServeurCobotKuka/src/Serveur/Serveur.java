package Serveur;
import java.net.*;
import java.io.*;
import java.util.*;
import WebService.PublisherWS;

//** Classe principale du serveur, g�re les infos globales **
public class Serveur
{
  private Vector _tabClients = new Vector(); // contiendra tous les flux de sortie vers les clients
  private int _nbClients=0; // nombre total de clients connect�s

  //** Methode : la premi�re m�thode ex�cut�e, elle attend les connections **
  public static void main(String args[])
  {
    Serveur serv = new Serveur(); // instance de la classe principale
    try
    {
      Integer port;
      if(args.length<=0) port=new Integer("18000"); // si pas d'argument : port 18000 par d�faut
      else port = new Integer(args[0]); // sinon il s'agit du num�ro de port pass� en argument
      
      new Commandes(serv); // lance le thread de gestion des commandes

      ServerSocket ss = new ServerSocket(port.intValue()); // ouverture d'un socket serveur sur port
      printWelcome(port);
      while (true) // attente en boucle de connexion (bloquant sur ss.accept)
      {
        new ThreadServeur(ss.accept(),serv); // un client se connecte, un nouveau thread client est lanc�
      }
    }
    catch (Exception e) { }
  }

  //** Methode : affiche le message d'accueil **
  static private void printWelcome(Integer port)
  {
    System.out.println("--------");
    System.out.println("BlablaServeur : Par Minosis - Julien Defaut");
    System.out.println("Copyright : 2004 - Minosis.com");
    System.out.println("Derniere version : 10/04/2004");
    System.out.println("--------");
    System.out.println("Demarre sur le port : "+port.toString());
    System.out.println("--------");
    System.out.println("Quitter : tapez \"quit\"");
    System.out.println("Nombre de connectes : tapez \"total\"");
    System.out.println("--------");
  }


  //** Methode : envoie le message � tous les clients **
  synchronized public void sendAll(String message,String sLast)
  {
    PrintWriter out; // declaration d'une variable permettant l'envoi de texte vers le client
    for (int i = 0; i < _tabClients.size(); i++) // parcours de la table des connect�s
    {
      out = (PrintWriter) _tabClients.elementAt(i); // extraction de l'�l�ment courant (type PrintWriter)
      if (out != null) // s�curit�, l'�l�ment ne doit pas �tre vide
      {
      	// ecriture du texte pass� en param�tre (et concat�nation d'une string de fin de chaine si besoin)
        out.print(message+sLast);
        out.flush(); // envoi dans le flux de sortie
      }
    }
  }

  //** Methode : d�truit le client no i **
  synchronized public void delClient(int i)
  {
    _nbClients--; // un client en moins ! snif
    if (_tabClients.elementAt(i) != null) // l'�l�ment existe ...
    {
      _tabClients.removeElementAt(i); // ... on le supprime
    }
  }

  //** Methode : ajoute un nouveau client dans la liste **
  synchronized public int addClient(PrintWriter out)
  {
    _nbClients++; // un client en plus ! ouaaaih
    _tabClients.addElement(out); // on ajoute le nouveau flux de sortie au tableau
    return _tabClients.size()-1; // on retourne le num�ro du client ajout� (size-1)
  }

  //** Methode : retourne le nombre de clients connect�s **
  synchronized public int getNbClients()
  {
    return _nbClients; // retourne le nombre de clients connect�s
  }

}