using org;
using bluez;



public class BluezRemoteDevice : Object {
	
	public string path {get; set; default="unkown";}
	public string addr {get; set; default="unkown";}
	public string name {get; set; default="unkown";}
	public string alias {get; set; default="unkown";}
	public string adapter {get; set; default="unkown";}
	public string icon {get; set; default="unkown";}
	public uint klass {get; set; default=0;}
	
	public bool paired {get; set; default=false;}
	public bool trusted {get; set; default=false;}
	public bool blocked {get; set; default=false;}
	public bool connected {get; set; default=false;}
	
	private string[] UUIDs;
	
	public bool online {get; set; default=false;}
	
	
	private Device dbus_device;
	
	public BluezRemoteDevice(GLib.ObjectPath obj_path) {
		
		this.path = obj_path;
		try {
			dbus_device = Bus.get_proxy_sync (BusType.SYSTEM, "org.bluez", obj_path);
		} catch (IOError err) {
			stderr.printf("ERR: Could not get remote device with path %s: %s\n", obj_path, err.message);
			return;
		}
		
		
		dbus_device.property_changed.connect(property_changed_device_sig);
		dbus_device.disconnect_requested.connect(disconnect_requested_sig);
		
		this.update_properties();
		
		
		stdout.printf("Remote device (%s) created successfully.\n", obj_path);
		
	}

	
	public void update_properties() {
		stdout.printf("Updating properties for remote device %s...\n", this.path);
		
		try {
			var hash = dbus_device.get_properties();
			
			this.addr = (string) hash.lookup("Address");
			this.name = (string) hash.lookup("Name");
			this.alias = (string) hash.lookup("Alias");
			this.adapter = (string) hash.lookup("Adapter");
			this.icon = (string) hash.lookup("Icon");
			this.klass = (uint) hash.lookup("Class");
			this.paired = (bool) hash.lookup("Paired");
			this.trusted = (bool)hash.lookup("Trusted");
			this.blocked = (bool) hash.lookup("Blocked");
			this.connected = (bool) hash.lookup("Connected");
			
			this.UUIDs = get_dbus_array(hash.lookup("UUIDs"));
			
		} catch (IOError err) {
			stderr.printf("ERR: Could not get properties from remote device %s: %s\n", this.path, err.message);
		}

		this.debug_info();
	}
	
	public void debug_info() {
		stdout.printf("path = "+path+";\n"+
					"addr = "+addr+";\n"+
					"name = "+name+";\n"+
					"alias = "+alias+";\n"+
					"adapter = "+adapter+";\n"+
					"icon = "+icon+";\n"+
					"class = "+klass.to_string()+";\n"+
					"paired = "+paired.to_string()+";\n"+
					"trusted = "+trusted.to_string()+";\n"+
					"blocked = "+blocked.to_string()+";\n"+
					"connected = "+connected.to_string()+";\n\n");
	}
	
	public void set_property_device(string name, GLib.Variant val) {
		stdout.printf("Setting property "+name+" to "+val.get_type_string() +" on rdevice "+path+".\n");
		try {
			dbus_device.set_property_(name, val);
		} catch (IOError err) {
			stderr.printf("ERR: Could not set property %s on device %s: %s\n",name, this.path, err.message);
		}
	}
	
	
	public bool has_service_input() {
	
		foreach(var uid in this.UUIDs) 
			if (uid == HID_UUID) 
				return true;
		
		return false;
}
	
	

	/*
	 * 
	 * SIGNALS
	 * 
	 */
	 
	 private void property_changed_device_sig(string name, GLib.Variant val) {
		 stdout.printf("SIGNAL: Property changed on remote device %s: %s = %s;\n", path, name, val.get_type_string());
		 
		 switch(name) {
			case "Address":
				this.addr = (string) val;	
				break;
			case "Name":
				this.name = (string) val;	
				break;
			case "Alias":
				this.alias = (string) val;	
				break;
			case "Adapter":
				this.adapter = (string) val;	
				break;
			case "Icon":
				this.icon = (string) val;	
				break;
			case "Class":
				this.klass = (uint) val;	
				break;
			case "Paired":
				this.paired = (bool) val;	
				break;
			case "Trusted":
				this.trusted = (bool) val;	
				break;
			case "Blocked":
				this.blocked = (bool) val;	
				break;
			case "Powered":
				this.connected = (bool) val;	
				break;
			default:
				stdout.printf("Unknown property %s\n", name);
				break;	
		}
	}

	 private void disconnect_requested_sig() {
		 stdout.printf("SIGNAL: DisconnectRequested on remote device %s\n", path);
	}

	
}
