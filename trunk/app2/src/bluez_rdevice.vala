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
	
	private string[]? UUIDs;
	
	public bool connected_audio {get; set; default=false;}
	public bool connected_input {get; set; default=false;}
	
	public string? password {get; set; default=null;}
	
	public bool online {get; set; default=false;}
	
	
	private Device dbus_device;
	private Audio? dbus_audio;
	private Input? dbus_input;
	
	public BluezRemoteDevice(GLib.ObjectPath obj_path) {
		
		this.path = obj_path;
		
		UUIDs = null;
		
		dbus_audio = null;
		dbus_input = null;
		
		try {
			dbus_device = Bus.get_proxy_sync (BusType.SYSTEM, "org.bluez", this.path);
		} catch (IOError err) {
			stderr.printf("ERR: Could not get remote device with path %s: %s\n", obj_path, err.message);
			var dialog = new DialogUI();
			dialog.create("Could not get remote device with path<br>"+(string)obj_path+"<br>"+err.message);
			dialog.show();
			return;
		}
		
		
		dbus_device.property_changed.connect(property_changed_device_sig);
		dbus_device.disconnect_requested.connect(disconnect_requested_sig);
		
		this.update_properties_device();
		
		if(this.has_service_audio()) {
			try {
				dbus_audio = Bus.get_proxy_sync (BusType.SYSTEM, "org.bluez", this.path);
				dbus_audio.property_changed.connect(property_changed_audio_sig);
			} catch (IOError e) {
				stderr.printf ("ERR: Could not get access to dbus_audio: org.bluez [%s]: %s\n",
				this.path, e.message);
			}
		}
		
		if(this.has_service_input()) {
			try {
				dbus_input = Bus.get_proxy_sync (BusType.SYSTEM, "org.bluez", this.path);
				dbus_input.property_changed.connect(property_changed_input_sig);
			} catch (IOError e) {
				stderr.printf ("ERR: Could not get access to dbus_input: org.bluez [%s]: %s\n",
				this.path, e.message);
			}
		}
		
		stdout.printf("Remote device (%s) created successfully.\n", this.path);
		
	}

	public void update_properties() {
		update_properties_device();
		if(this.has_service_audio())
			update_properties_audio();
		if(this.has_service_input())
			update_properties_input();
	}
	
	public void update_properties_device() {
		stdout.printf("Updating properties for remote device %s...", this.path);
		
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
			stdout.printf("done\n");
			
		} catch (IOError err) {
			stderr.printf("\nERR: Could not get device properties from remote device %s: %s\n", this.path, err.message);
		}

		this.debug_info();
	}
	
	
	public void update_properties_audio() {
		if(dbus_audio==null) return;
		try {
			var hash = dbus_audio.get_properties();
			this.connected_audio = (bool) hash.lookup("State");
		} catch (IOError err) {
			stderr.printf("ERR: Could not get audio properties from remote device %s: %s\n", this.path, err.message);
		}
	}
	
	public void update_properties_input() {
		if(dbus_input==null) return;
		try {
			var hash = dbus_input.get_properties();
			this.connected_input = (bool) hash.lookup("State");
		} catch (IOError err) {
			stderr.printf("ERR: Could not get input properties from remote device %s: %s\n", this.path, err.message);
		}
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
	
	public void connect_audio() {
		if(dbus_audio==null) return;
		stdout.printf("Connecting audio with device %s.\n", this.path);
		try {
			dbus_audio.connect();
		} catch (IOError err) {
			stderr.printf("ERR: Could not connect audio with device %s: %s\n", this.path, err.message);
			var dialog = new DialogUI();
			dialog.create("Could not connect audio with device:<br>"+this.path+"<br>"+err.message);
			dialog.show();
		}
	}
	
	public void disconnect_audio() {
		if(dbus_audio==null) return;
		stdout.printf("Disconnecting audio from device %s.\n", this.path);
		try {
			dbus_audio.disconnect();
		} catch (IOError err) {
			stderr.printf("ERR: Could not disconnect audio from device %s: %s\n", this.path, err.message);
			var dialog = new DialogUI();
			dialog.create("Could not disconnect audio from device:<br>"+this.path+"<br>"+err.message);
			dialog.show();
		}
	}
	
	public void connect_input() {
		if(dbus_input==null) return;
		stdout.printf("Connecting input with device %s.\n", this.path);
		try {
			dbus_input.connect();
		} catch (IOError err) {
			stderr.printf("ERR: Could not connect input with device %s: %s\n", this.path, err.message);
			var dialog = new DialogUI();
			dialog.create("Could not connect input with device:<br>"+this.path+"<br>"+err.message);
			dialog.show();
		}
	}
	
	public void disconnect_input() {
		if(dbus_input==null) return;
		stdout.printf("Disconnecting audio from device %s.\n", this.path);
		try {
			dbus_input.disconnect();
		} catch (IOError err) {
			stderr.printf("ERR: Could not disconnect input from device %s: %s\n", this.path, err.message);
			var dialog = new DialogUI();
			dialog.create("Could not disconnect input from device:<br>"+this.path+"<br>"+err.message);
			dialog.show();
		}
	}
	
	
	public bool has_service_input() {
		if(this.UUIDs==null) return false;
		
		foreach(var uid in this.UUIDs) 
			if (uid == HID_UUID) 
				return true;
		
		return false;
	}	


	public bool has_service_audio() {
		if(this.UUIDs==null) return false;
		
		foreach(var uid in this.UUIDs) {
			if ( uid == HSP_HS_UUID ||
				 uid ==  HSP_AG_UUID ||
				 uid ==  HFP_HS_UUID ||
				 uid ==  HFP_AG_UUID ||
				 uid ==  A2DP_SOURCE_UUID ||
				 uid ==  A2DP_SINK_UUID ||
				 uid ==  AVRCP_TARGET_UUID ) 
				return true;
		}
		
		return false;
	}	
	
	

	/*
	 * 
	 * SIGNALS
	 * 
	 */
	 
	 private void property_changed_audio_sig(string name, GLib.Variant val) {
		stdout.printf("SIGNAL: AudioProperty changed on remote device %s: %s = %s;\n", path, name, val.get_type_string());
		
		if(name=="Connected") {
			this.connected_audio = (bool) val;
			if(this.connected_audio==true) {
				var dialog = new DialogUI();
				dialog.create("Audio connection with device<br>"+this.addr+"<br>was successful!");
				dialog.show();
			}
		} else {
			stderr.printf("Unknown Audio property %s\n", name);
		}
	 
	 }
	 
	 
	 private void property_changed_input_sig(string name, GLib.Variant val) {
		stdout.printf("SIGNAL: InputProperty changed on remote device %s: %s = %s;\n", path, name, val.get_type_string());
	 
		if(name=="Connected") {
			this.connected_input = (bool) val;
			if(this.connected_input==true) {
				var dialog = new DialogUI();
				dialog.create("Input connection with device<br>"+this.addr+"<br>was successful!");
				dialog.show();
			}
		} else {
			stderr.printf("Unknown Input property %s\n", name);
		}
	 
	 
	 }
	 
	 private void property_changed_device_sig(string name, GLib.Variant val) {
		stdout.printf("SIGNAL: DeviceProperty changed on remote device %s: %s = %s;\n", path, name, val.get_type_string());
		 
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
			case "Connected":
				this.connected = (bool) val;	
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
			case "UUIDs":
				this.UUIDs = get_dbus_array(val);
				if(this.has_service_audio() && dbus_audio==null) {
					try {
						dbus_audio = Bus.get_proxy_sync (BusType.SYSTEM, "org.bluez", this.path);
					} catch (IOError e) {
						stderr.printf ("ERR: Could not get access to dbus_audio: org.bluez [%s]: %s\n",
						this.path, e.message);
					}
				} if(this.has_service_input()  && dbus_input==null) {
					try {
						dbus_input = Bus.get_proxy_sync (BusType.SYSTEM, "org.bluez", this.path);
					} catch (IOError e) {
						stderr.printf ("ERR: Could not get access to dbus_input: org.bluez [%s]: %s\n",
						this.path, e.message);
					}
				}
				break;
			default:
				stderr.printf("Unknown property %s\n", name);
				break;	
		}
	}

	 private void disconnect_requested_sig() {
		 stdout.printf("SIGNAL: DisconnectRequested on remote device %s\n", path);
	}

	
}
