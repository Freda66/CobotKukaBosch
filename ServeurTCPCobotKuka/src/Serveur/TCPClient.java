package Serveur;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
 
/**
 * This class implements java socket client
 * @author pankaj
 *
 */
public class TCPClient {
 
    public static void main(String[] args) throws UnknownHostException, IOException, ClassNotFoundException, InterruptedException{
        //get the localhost IP address, if server is running on some other IP, you need to use that
        InetAddress host = InetAddress.getLocalHost();
        Socket socket = null;
        ObjectOutputStream oos = null;
        ObjectInputStream ois = null;
        
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String strLine;

        while ((strLine = br.readLine()) != null) {
        	// do something with the line
        	
        	//System.out.print("MESSAGE: "+strLine+"\n");
        	socket = new Socket("172.30.1.125", 30004); // host.getHostName(); 
            //write to socket using ObjectOutputStream
            oos = new ObjectOutputStream(socket.getOutputStream());
            //System.out.println("Sending request to Socket Server");
            
            
             oos.writeObject(strLine);
            //read the server response message
            ois = new ObjectInputStream(socket.getInputStream());
            String message = (String) ois.readObject();
            System.out.println("Message from server: " + message+"\n");
            //close resources
            ois.close();
            oos.close();
        }
        
    }
}