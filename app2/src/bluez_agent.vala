[DBus (name = EMTOOTH_BLUEZ_AGENT_IFACE)]
public class BluezAgent : Object {
    //private int counter;
    
    public void release() {
		
	}
    
    public string request_pin_code(ObjectPath path) {
		return "a";
		
	}
	
	public uint32 request_pass_key(ObjectPath path) {
		return 1234;
		
	}
	
	public void display_pass_key(ObjectPath path, uint32 passkey, uint8 entered) {
		
		
	}
	
	public void request_confirmation(ObjectPath path, uint32 passkey) {
		
		
	}
    
    public void authorize(ObjectPath path, string uuid) {
		
		
	}
    
    public void confirm_mode_change(string mode) {
		
		
	}
	
	public void cancel() {
		
		
	}
    	
}
