
//(name = EMTOOTH_BLUEZ_AGENT_IFACE)
[DBus (name = "org.bluez.Agent")]
public class BluezAgent : Object {
    //private int counter;
    
    public void release() {
		
		stderr.printf("Agent: release\n");
		
	}
    
    public string request_pin_code(ObjectPath path) {
		
		stderr.printf("Agent: request_pin_code\n");
		
		var rdevice = ADAPTER.get_rdevice_by_path(path);
		
		Elm.Win win = ui.create_pin_dialog(rdevice);
		win.show();
		
		while(rdevice.password==null) { //wait till we have password
			Ecore.MainLoop.iterate();
		}
		
		return rdevice.password;
		
		//TODO: set password to null so it is asked on next pair on that device
		
	}
	
	public uint32 request_pass_key(ObjectPath path) {
		stderr.printf("Agent: request_pass_key\n");
		return 1234;
		
	}
	
	public void display_pass_key(ObjectPath path, uint32 passkey, uint8 entered) {
		stderr.printf("Agent: display_pass_key\n");
		
	}
	
	public void request_confirmation(ObjectPath path, uint32 passkey) {
		stderr.printf("Agent: request_confirmation\n");
		
	}
    
    public void authorize(ObjectPath path, string uuid) {
		stderr.printf("Agent: authorize\n");
		
	}
    
    public void confirm_mode_change(string mode) {
		stderr.printf("Agent: confirm_mode_change\n");
		
	}
	
	public void cancel() {
		stderr.printf("Agent: cancel\n");
		
	}
    	
}
