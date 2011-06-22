
[DBus (name = "org.openobex.Agent")]
public class ObexAgent : Object {


	public string authorize(ObjectPath transfer_path, string bt_address, string name, 
													string type, int32 length, int32 time) {
		
		stderr.printf("ObexAgent: authorize\n");
		
		return "received_file.txt";
		
	}
	
	public void cancel() {
		stderr.printf("ObexAgent: cancel\n");		
	}
    	
}
