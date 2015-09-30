package Serveur;
import java.io.*;

//** Classe qui g�re les commandes tap�es dans la console **
// impl�mentation de l'interface Runnable (une des 2 m�thodes pour cr�er un thread)
class Commandes implements Runnable
{
  Serveur _serv; // pour utilisation des m�thodes de la classe principale
  BufferedReader _in; // pour gestion du flux d'entr�e (celui de la console)
  String _strCommande=""; // contiendra la commande tap�e
  Thread _t; // contiendra le thread

  //** Constructeur : initialise les variables n�cessaires **
  Commandes(Serveur blablaServ)
  {
    _serv=blablaServ; // passage de local en global
    // le flux d'entr�e de la console sera g�r� plus pratiquement dans un BufferedReader
    _in = new BufferedReader(new InputStreamReader(System.in));
    _t = new Thread(this); // instanciation du thread
    _t.start(); // demarrage du thread, la fonction run() est ici lanc�e
  }

  //** Methode : attend les commandes dans la console et ex�cute l'action demand�e **
  public void run() // cette m�thode doit obligatoirement �tre impl�ment�e � cause de l'interface Runnable
  {
    try
    {
      // si aucune commande n'est tap�e, on ne fait rien (bloquant sur _in.readLine())
      while ((_strCommande=_in.readLine())!=null)
      {
        if (_strCommande.equalsIgnoreCase("quit")) // commande "quit" detect�e ...
          System.exit(0); // ... on ferme alors le serveur
        else if(_strCommande.equalsIgnoreCase("total")) // commande "total" detect�e ...
        {
          // ... on affiche le nombre de clients actuellement connect�s
          System.out.println("Nombre de connectes : "+_serv.getNbClients());
          System.out.println("--------");
        }
        else
        {
          // si la commande n'est ni "total", ni "quit", on informe l'utilisateur et on lui donne une aide
          System.out.println("Cette commande n'est pas supportee");
          System.out.println("Quitter : \"quit\"");
          System.out.println("Nombre de connectes : \"total\"");
          System.out.println("--------");
        }
        System.out.flush(); // on affiche tout ce qui est en attente dans le flux
      }
    }
    catch (IOException e) {}
  }
}