
[DBus (name = "org.openobex.Agent")]
public class ObexClientAgent : Object {

    public void release() {
		
		stderr.printf("ObexAgent: release\n");
		
	}
	
	
	public string request(ObjectPath transfer_path) {
		
		stderr.printf("ObexClientAgent: request\n");
		return "hello.txt";
	}
	
	
	public void progress(ObjectPath transfer_path, uint64 bytes) {
		stderr.printf("ObexClientAgent: progress\n");
		
	}
	
	public void complete(ObjectPath transfer_path) {
		stderr.printf("ObexClientAgent: complete\n");
		
	}
	
	public void error(ObjectPath transfer_path, string message) {
		stderr.printf("ObexClientAgent: error -> %s\n", message);
	}
    	
}
