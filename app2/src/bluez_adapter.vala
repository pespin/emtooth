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
	private string[] devices;
	private string[] UUIDs;
	
	public uint num_devices_found {get; private set; default=0;}
	
	private Adapter dbus_obj;
	
	private HashTable<string,BluezRemoteDevice> hash;
	
	public BluezAdapter(GLib.ObjectPath obj_path) {
		
		this.path = obj_path;
		try {
			dbus_obj = Bus.get_proxy_sync (BusType.SYSTEM, "org.bluez", obj_path);

		} catch (IOError err) {
			stderr.printf("ERR: Could not get local adapter with path %s: %s\n", obj_path, err.message);
			return;
		}
		
		hash = new HashTable<string,BluezRemoteDevice>(str_hash, str_equal);
		
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
		} catch (IOError err) {
			stderr.printf("ERR: Could not start device discovery: %s\n", err.message);
		}
	}
	
	public void stop_discovery() {
		try {
			dbus_obj.stop_discovery();
		} catch (IOError err) {
			stderr.printf("ERR: Could not stop device discovery: %s\n", err.message);
		}
	}
	
	public void set_property_(string name, GLib.Variant val) {
		stdout.printf("Setting property "+name+" to "+val.get_type_string() +"\n");
		try {
			dbus_obj.set_property_(name, val);
		} catch (IOError err) {
			stderr.printf("ERR: Could not set property %s on local adapter %s: %s\n",name, this.path, err.message);
		}
		
	}
	
	public void remove_rdevice(GLib.ObjectPath device_path) {
		stdout.printf("Removing device with path "+ (string) device_path+"...\n");
		try {
			dbus_obj.remove_device(device_path);
		} catch (IOError err) {
			stderr.printf("ERR: Could not remove device %s: %s\n", (string) device_path, err.message);
		}
		
		this.hash.remove(device_path);
		
	}
	
	
	public void update_properties() {
		stdout.printf("Updating properties for local adapter %s...\n", this.path);
		
		try {
			var hash = dbus_obj.get_properties();
			
			this.addr = (string) hash.lookup("Address");
			this.name = (string) hash.lookup("Name");
			this.klass = (uint) hash.lookup("Class");
			this.discoverable = (bool) hash.lookup("Discoverable");
			this.discoverable_timeout = (uint) hash.lookup("DiscoverableTimeout");
			this.pairable = (bool) hash.lookup("Pairable");
			this.pairable_timeout = (uint) hash.lookup("PairableTimeout");
			this.discovering = (bool) hash.lookup("Discovering");
			this.powered = (bool) hash.lookup("Powered");
			
			this.devices = get_dbus_array(hash.lookup("Devices"));
			this.UUIDs = get_dbus_array(hash.lookup("UUIDs"));

		} catch (IOError err) {
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
	
	
	public unowned BluezRemoteDevice? get_rdevice_by_path(string path) {
			return hash.lookup(path);
	}
	
	public BluezRemoteDevice? get_rdevice_by_addr(string addr) {
			List<BluezRemoteDevice> list;
			list = this.hash.get_values();
			foreach(var device in list) {
				if(addr == device.addr) return device;
			} 
			return null;
	}
	
	
	
	
	
	/* SIGNALS */
	private void property_changed_sig(string name, GLib.Variant val) {
		stdout.printf ("SIGNAL: Property changed on adapter %s -> %s = %s;\n", path,  name, val.get_type_string());
		switch(name) {
			case "Address":
				this.addr = (string) val;	
				break;
			case "Name":
				this.name = (string) val;	
				break;
			case "Class":
				this.klass = (uint) val;	
				break;
			case "Discoverable":
				this.discoverable = (bool) val;	
				break;
			case "DiscoverableTimeout":
				this.discoverable_timeout = (uint) val;	
				break;
			case "Pairable":
				this.pairable = (bool) val;	
				break;
			case "PairableTimeout":
				this.pairable_timeout = (uint) val;	
				break;
			case "Discovering":
				this.discovering = (bool) val;	
				break;
			case "Powered":
				this.powered = (bool) val;	
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
		
		GLib.ObjectPath path = null;
		
		try {
			path = dbus_obj.find_device(address);
			
		} catch (GLib.Error err) {
			stderr.printf ("ERR: Could not find object path for device  %s: %s.\t Creating it\n", address, err.message);
			try {
				path = dbus_obj.create_device(address);
			}  catch (IOError err2) {
				stderr.printf ("ERR: Could not crete object path for device  %s: %s.\n", address, err2.message);
				return;
			}
		}
		
		stdout.printf("Object path for device with addr %s is %s\n", address, path);
		
		unowned BluezRemoteDevice tmp;
		tmp = hash.lookup(path);
		if(tmp == null) {
			var device = new BluezRemoteDevice(path);
			this.hash.insert(device.path, device);
			this.num_devices_found++;
			device.online = true;
			ui.add_rdevice_to_ui(device);
		} else {
				tmp.online=true;
		}
		
	}

	private void device_disappeared_sig (string address) {
		stdout.printf ("Remote device disappeared (%s)\n", address);
		
		var device = get_rdevice_by_addr(address);
		if(device!=null)
			device.online = false;
		
	}
	
	
}
