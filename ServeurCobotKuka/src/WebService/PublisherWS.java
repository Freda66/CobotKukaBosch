package WebService;
import javax.xml.ws.Endpoint;

public class PublisherWS {
		
	public PublisherWS(String uri){
		if(uri == null || uri == "") uri = "http://172.30.1.125:9191/cobotkuka";
		
		ImplementWS impl = new ImplementWS();
		
		Endpoint endpoint = Endpoint.publish(uri, impl);
		
		boolean status = endpoint.isPublished();
		
		System.out.println("Web Service disponible ? " + status);
	}
}
