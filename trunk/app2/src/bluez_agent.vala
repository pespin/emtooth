[DBus (name = EMTOOTH_BLUEZ_AGENT_IFACE)]
public class BluezAgent : Object {
    //private int counter;
    
    public void release() {
		
	}
    
    public string request_pin_code(ObjectPath path) {
		
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
