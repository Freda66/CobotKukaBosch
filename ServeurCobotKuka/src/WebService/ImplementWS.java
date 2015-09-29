package WebService;
import javax.jws.WebService;

@WebService(endpointInterface="WebService.InterfaceWS")
public class ImplementWS implements InterfaceWS {

	public boolean positionInit() {
		// Log de la commande
		System.out.println("Appel fonction : positionInit");
		
		// Envoi la commande par le socket au robot
		
		return true;
	}

}
