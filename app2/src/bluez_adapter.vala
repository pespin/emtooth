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
	private string[]? devices;
	private string[]? UUIDs;
	
	public uint num_devices_found {get; private set; default=0;}
	
	private Adapter dbus_obj;
	
	private HashTable<string,BluezRemoteDevice> rdevice_hash;
	
	public BluezAdapter(GLib.ObjectPath obj_path) {
		
		this.path = obj_path;
		
		devices = null;
		UUIDs = null;
		
		try {
			dbus_obj = Bus.get_proxy_sync (BusType.SYSTEM, "org.bluez", obj_path);

		} catch (IOError err) {
			stderr.printf("ERR: Could not get local adapter with path %s: %s\n", obj_path, err.message);
			var dialog = new DialogUI();
			dialog.create("Could not get local adapter with path "+(string)obj_path+":<br>"+err.message);
			return;
		}
		
		rdevice_hash = new HashTable<string,BluezRemoteDevice>(str_hash, str_equal);
		
		dbus_obj.device_found.connect (device_found_sig);
		dbus_obj.device_disappeared.connect (device_disappeared_sig);
        dbus_obj.device_created.connect (device_created_sig);
        dbus_obj.device_removed.connect (device_removed_sig);
        dbus_obj.property_changed.connect (property_changed_sig);
		
		stdout.printf("Local bluez adapter (%s) created successfully.\n", obj_path);
		
		//power on the device:
		Variant v = true;
		this.set_property_("Powered", v); 
		
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
	
	public bool remove_rdevice(GLib.ObjectPath device_path) {
		stdout.printf("Removing device with path "+ (string) device_path+"...\n");
		try {
			dbus_obj.remove_device(device_path);
		} catch (IOError err) {
			stderr.printf("ERR: Could not remove device %s: %s\n", (string) device_path, err.message);
			var dialog = new DialogUI();
			dialog.create("Could not remove device "+(string)device_path+":<br>"+err.message);
			return false;
		}
		
		bool found;
		//ui.remove_rdevice_from_ui(device_path);
		found = this.rdevice_hash.remove(device_path);
		
		if(found) this.num_devices_found--;
		
		
		return true;
		
	}
	
	public void register_agent(string agent_path) {
		stdout.printf("Registering agent "+agent_path+" on adapter "+this.path+"...\n");
		try {
			dbus_obj.register_agent((ObjectPath) agent_path, "DisplayYesNo");
		} catch (IOError e) {
			stderr.printf ("ERR: Could not register agent: %s\n", e.message);
			var dialog = new DialogUI();
			dialog.create("Could not register agent:<br>"+e.message);
		}
	}
	
	public void pair_rdevice(BluezRemoteDevice rdevice) {
		stdout.printf("Trying to pair with  "+rdevice.path+"...\n");
		this.pair_device_async(rdevice);
	}
	
	private async void pair_device_async(BluezRemoteDevice rdevice) {
		try {
			yield dbus_obj.create_paired_device(rdevice.addr, (ObjectPath) rdevice.path, "");
		} catch (IOError e) {
			stderr.printf ("ERR: Could not create paired device: %s\n", e.message);
			var dialog = new DialogUI();
			dialog.create("Could not create paired device:<br>"+e.message);
		}	
		
	}
	
	public void update_properties() {
		stdout.printf("Updating properties for local adapter %s...", this.path);
		
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
			
			stdout.printf("done\n");

		} catch (IOError err) {
			stderr.printf("\nERR: Could not get properties from device %s: %s\n", this.path, err.message);
		}
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
			return rdevice_hash.lookup(path);
	}
	
	public BluezRemoteDevice? get_rdevice_by_addr(string addr) {
			List<unowned BluezRemoteDevice> list;
			list = this.rdevice_hash.get_values();
			foreach(var device in list) {
				if(addr == device.addr) return device;
			} 
			return null;
	}
	
	
	
	
	
	/* SIGNALS */
	private void property_changed_sig(string name, GLib.Variant val) {
		stdout.printf ("SIGNAL: Property changed on adapter %s -> %s = %s;\n", path,  name, val.get_type_string());
		
		if(val==null) {
			warning("bluez_adapter.property_changed_sig() -> "+name+" was null!");		
		}
		
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
			case "UUIDs":
				this.UUIDs = get_dbus_array(val);
				break;
			case "Devices":
				this.devices = get_dbus_array(val);
				break;
			default:
				stdout.printf("Unknown property %s\n", name);
				break;	
		}
		

	}
	
	private void device_created_sig (string address) {
		stdout.printf ("SIGNAL: Remote device created (%s)\n",  address);
	}

	private void device_removed_sig (string path) {
		stdout.printf ("SIGNAL: Remote device removed (%s)\n", path);
		
		var device = this.rdevice_hash.lookup(path);
		if(device!=null) {
			stdout.printf ("device is cached, time to remove it from everywhere\n");
			this.rdevice_hash.remove(path);
			this.num_devices_found--;
		}
		ui.mui.remove_rdevice_from_ui(path);
	}
	
	private void device_found_sig (string address, HashTable<string, GLib.Variant?> properties) {
		stdout.printf ("SIGNAL: Remote device found (%s)\n",  address);
		device_found_sig_async.begin(address, properties);
		return;
		
	}
	
	private async void device_found_sig_async(string address, HashTable<string, GLib.Variant?> properties) {
		
		GLib.ObjectPath path = null;
			
			try {
				path = dbus_obj.find_device(address);
				
			} catch (GLib.Error err) {
				stderr.printf ("Could not find object path for device  %s: %s.\t Creating it\n", address, err.message);
				try {
					path = yield dbus_obj.create_device(address);
				}  catch (IOError err2) {
					stderr.printf ("ERR: Could not create object path for device  %s: %s.\n", address, err2.message);
					var dialog = new DialogUI();
					dialog.create("Could not create object path for device "+address+":<br>"+err2.message);
					return;
				}
			}
			
			stdout.printf("Object path for device with addr %s is %s\n", address, path);
			
			BluezRemoteDevice device;
			device = this.rdevice_hash.lookup(path);
			if(device == null) {
				device = new BluezRemoteDevice(path);
				this.rdevice_hash.insert(device.path, (owned) device);
				device = this.rdevice_hash.lookup(path); //glib CRITICAL errors without this line
			} 
			
			
			if(device.online==false) {
				device.online = true;
				this.num_devices_found++;
				ui.mui.add_rdevice_to_ui(device);
			}
		
	}

	private void device_disappeared_sig (string address) {
		stdout.printf ("Remote device disappeared (%s)\n", address);
		
		var device = get_rdevice_by_addr(address);
		if(device==null) return;
			
			
		device.online = false;
		ui.mui.remove_rdevice_from_ui(device.path);
		this.num_devices_found--;
	}
}
