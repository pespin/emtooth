using org;
using bluez;



public class BluezAdapter : Object {
	
	public string path {get; private set; default="unkown";}
	public string addr {get; set; default="unkown";}
	public string name {get; set; default="unkown";}
	public uint klass {get; set; default=0;}
	public bool discoverable {get; set; default=false;}
	public uint discoverable_timeout {get; set; default=0;}
	public bool discovering {get; set; default=false;}
	public bool pairable {get; set; default=false;}
	public uint pairable_timeout {get; set; default=0;}
	public bool powered {get; set; default=false;}
	
	private Adapter dbus_obj;
	private List<BluezRemoteDevice> rdevices;
	
	public BluezAdapter(DBus.ObjectPath obj_path) {
		
		this.path = obj_path;
		try {
			var conn = DBus.Bus.get (DBus.BusType.SYSTEM);
			dbus_obj = (Adapter) conn.get_object ("org.bluez", obj_path);
		} catch (DBus.Error err) {
			stderr.printf("ERR: Could not get local adapter with path %s: %s\n", obj_path, err.message);
			return;
		}
		
		rdevices = new List<BluezRemoteDevice>();
		
		dbus_obj.device_found.connect (device_found_sig);
		dbus_obj.device_disappeared.connect (device_disappeared_sig);
        dbus_obj.device_created.connect (device_created_sig);
        dbus_obj.device_removed.connect (device_removed_sig);
        dbus_obj.property_changed.connect (property_changed_sig);
		
		stdout.printf("Local bluez adapter (%s) created successfully.\n", obj_path);
		
		this.update_properties();
		
		this.start_discovery();
		
	}
	
	public void start_discovery() {
		try {
			dbus_obj.start_discovery();
		} catch (DBus.Error err) {
			stderr.printf("ERR: Could not start device discovery: %s\n", err.message);
		}
	}
	
	public void stop_discovery() {
		try {
			dbus_obj.stop_discovery();
		} catch (DBus.Error err) {
			stderr.printf("ERR: Could not stop device discovery: %s\n", err.message);
		}
	}
	
	public void set_property_(string name, GLib.Value val) {
		stdout.printf("Setting property "+name+" to "+val.strdup_contents()+"\n");
		try {
			dbus_obj.set_property_(name, val);
		} catch (DBus.Error err) {
			stderr.printf("ERR: Could not stop device discovery: %s\n", err.message);
		}
		
	}
	
	
	public void update_properties() {
		stdout.printf("Updating properties for local adapter %s...\n", this.path);
		
		try {
			var hash = dbus_obj.get_properties();
			
			this.addr = hash.lookup("Address").dup_string();
			this.name = hash.lookup("Name").dup_string();
			this.klass = hash.lookup("Class").get_uint();
			this.discoverable = hash.lookup("Discoverable").get_boolean();
			this.discoverable_timeout = hash.lookup("DiscoverableTimeout").get_uint();
			this.pairable = hash.lookup("Pairable").get_boolean();
			this.pairable_timeout = hash.lookup("PairableTimeout").get_uint();
			this.discovering = hash.lookup("Discovering").get_boolean();
			this.powered = hash.lookup("Powered").get_boolean();

		} catch (DBus.Error err) {
			stderr.printf("ERR: Could not get properties from device %s: %s\n", this.path, err.message);
		}
		//TODO: catch Devices:
		//<Lethalman> pespin_, just do get_pointer()... get_object() every one of them until you don't get a critical :P
	
	//"Devices": [ op'/org/bluez/5333/hci0/dev_00_06_6E_19_16_28', op'/org/bluez/5333/hci0/dev_00_19_2D_11_62_09' ], "UUIDs": [ "00001112-0000-1000-8000-00805f9b34fb", "0000111f-0000-1000-8000-00805f9b34fb", "0000110a-0000-1000-8000-00805f9b34fb", "0000110c-0000-1000-8000-00805f9b34fb", "0000110e-0000-1000-8000-00805f9b34fb", "00001105-0000-1000-8000-00805f9b34fb", "00001106-0000-1000-8000-00805f9b34fb" ] } )
		
		this.debug_info();
	}
	
	public void debug_info() {
		stdout.printf("path = "+path+";\n"+
					"addr = "+addr+";\n"+
					"name = "+name+";\n"+
					"class = "+klass.to_string()+";\n"+
					"discoverable = "+discoverable.to_string()+";\n"+
					"discoverable_timeout = "+discoverable_timeout.to_string()+";\n"+
					"pairable = "+pairable.to_string()+";\n"+
					"pairable_timeout = "+pairable_timeout.to_string()+";\n"+
					"powered = "+powered.to_string()+";\n\n");
	}
	
	
	public uint num_devices_found() {
		return this.rdevices.length();
	}
	
	
	/* SIGNALS */
	private void property_changed_sig(string name, Value val) {
		stdout.printf ("SIGNAL: Property changed on adapter %s -> %s = %s;\n", path,  name, val.strdup_contents());
		switch(name) {
			case "Address":
				this.addr = val.dup_string();	
				break;
			case "Name":
				this.name = val.dup_string();	
				break;
			case "Class":
				this.klass = val.get_uint();	
				break;
			case "Discoverable":
				this.discoverable = val.get_boolean();	
				break;
			case "DiscoverableTimeout":
				this.discoverable_timeout = val.get_uint();	
				break;
			case "Pairable":
				this.pairable = val.get_boolean();	
				break;
			case "PairableTimeout":
				this.pairable_timeout = val.get_uint();	
				break;
			case "Discovering":
				this.discovering = val.get_boolean();	
				break;
			case "Powered":
				this.powered = val.get_boolean();	
				break;
			default:
				stdout.printf("Unknown property %s\n", name);
				break;	
		}
		

	}
	
	private void device_created_sig (string address) {
		stdout.printf ("SIGNAL: Remote device created (%s)\n",  address);
	}

	private void device_removed_sig (string address) {
		stdout.printf ("SIGNAL: Remote device removed(%s)\n", address);
	}
	
	private void device_found_sig (string address, HashTable<string, GLib.Value?> properties) {
		stdout.printf ("SIGNAL: Remote device found (%s)\n",  address);
		
		DBus.ObjectPath path = null;
		
		//TODO: check in list wether we have this addr in list
		
		
		try {
			path = dbus_obj.find_device(address);
		} catch (DBus.Error err) {
			stderr.printf ("ERR: Could not find object path for device  %s: %s.\t Creating it\n", address, err.message);
			try {
				path = dbus_obj.create_device(address);
			}  catch (DBus.Error err2) {
				stderr.printf ("ERR: Could not crete object path for device  %s: %s.\n", address, err2.message);
				return;
			}
		}
		
		stdout.printf("Object path for device with addr %s is %s\n", address, path);
		
		var device = new BluezRemoteDevice(path);
		
		unowned List<BluezRemoteDevice> tmpli;
		tmpli = rdevices.find_custom (device, (CompareFunc) sort_rdevices);
		if(tmpli == null) {
			device.update_properties();
			this.rdevices.append(device);
			ui.add_rdevice_to_list(device);
		}
		
	}

	private void device_disappeared_sig (string address) {
		stdout.printf ("Remote device disappeared (%s)\n", address);
	}
	
	
}

public int sort_rdevices(BluezRemoteDevice a, BluezRemoteDevice b) {
	
	return strcmp(a.path, b.path);
	
}
