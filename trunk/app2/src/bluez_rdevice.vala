using org;
using bluez;



public class BluezRemoteDevice : Object {
	
	public string path {get; private set; default="unkown";}
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
	
	//TODO: handle UUIDS
	
	
	
//"UUIDs": [ "00001105-0000-1000-8000-00805f9b34fb", "00001106-0000-1000-8000-00805f9b34fb", "0000110a-0000-1000-8000-00805f9b34fb", "0000110c-0000-1000-8000-00805f9b34fb", "0000110e-0000-1000-8000-00805f9b34fb", "00001112-0000-1000-8000-00805f9b34fb", "00001115-0000-1000-8000-00805f9b34fb", "00001116-0000-1000-8000-00805f9b34fb", "00001117-0000-1000-8000-00805f9b34fb", "0000111f-0000-1000-8000-00805f9b34fb", "0000112f-0000-1000-8000-00805f9b34fb" ], "Adapter": op'/org/bluez/5333/hci0' } )

	
	
	private Device dbus_device;
	
	public BluezRemoteDevice(DBus.ObjectPath obj_path) {
		
		this.path = obj_path;
		try {
			var conn = DBus.Bus.get (DBus.BusType.SYSTEM);
			dbus_device = get_device_proxy(conn, "org.bluez", obj_path);
		} catch (DBus.Error err) {
			stderr.printf("ERR: Could not get remote device with path %s: %s\n", obj_path, err.message);
			return;
		}
		
		
		dbus_device.property_changed.connect(property_changed_sig);
		dbus_device.property_changed.connect(disconnect_requested_sig);
		
		
		stdout.printf("Remote device (%s) created successfully.\n", obj_path);
		
	}

	
	public void update_properties() {
		stdout.printf("Updating properties for remote device %s...\n", this.path);
		
		try {
			var hash = dbus_device.get_properties();
			
			this.addr = hash.lookup("Address").dup_string();
			this.name = hash.lookup("Name").dup_string();
			this.alias = hash.lookup("Alias").dup_string();
			this.adapter = hash.lookup("Adapter").dup_string();
			this.icon = hash.lookup("Icon").dup_string();
			this.klass = hash.lookup("Class").get_uint();
			this.paired = hash.lookup("Paired").get_boolean();
			this.trusted = hash.lookup("Trusted").get_boolean();
			this.blocked = hash.lookup("Blocked").get_boolean();
			this.connected = hash.lookup("Connected").get_boolean();
			
		} catch (DBus.Error err) {
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
	

	/*
	 * 
	 * SIGNALS
	 * 
	 */
	 
	 private void property_changed_sig(string name, GLib.Value val) {
		 stdout.printf("SIGNAL: Property changed on remote device %s: %s = %s;\n", path, name, val.strdup_contents());
	}

	 private void disconnect_requested_sig() {
		 stdout.printf("SIGNAL: DisconnectRequested on remote device %s\n", path);
	}

	
}