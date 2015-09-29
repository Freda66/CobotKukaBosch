package WebService;
import javax.xml.ws.Endpoint;

public class PublisherWS {
	
	public static final String URI = "http://localhost:8888/cobotkuka" ;
			
	public static void main(String[] args) {
		ImplementWS impl = new ImplementWS();
		
		Endpoint endpoint = Endpoint.publish(URI, impl);
		
		boolean status = endpoint.isPublished();
		
		System.out.println("Web Service disponible ? " + status);
	}
}
