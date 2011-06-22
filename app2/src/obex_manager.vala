using org;
using openobex;


public class ObexManager : Object {
	
	private ObexDBusManager dbus_obj;
	
	public ObexManager() {
		
				/* Start obex_agent */
		try {
			dbus_obj = Bus.get_proxy_sync (BusType.SESSION, "org.openobex", "/");
			stderr.printf ("service org.emtooth on session bus created correctly\n");
		} catch (Error e) {
			stderr.printf ("Could not create service org.emtooth on session bus: %s\n", e.message);
		}
		
		dbus_obj.transfer_started.connect (() => { stderr.printf("Manager: transfer_started\n"); } );
		dbus_obj.transfer_completed.connect (() => { stderr.printf("Manager: transfer_completed\n"); } );
        dbus_obj.session_created.connect (() => { stderr.printf("Manager: session_created\n"); } );
        dbus_obj.session_removed.connect (() => { stderr.printf("Manager: session_removed\n"); } );
		
		
	}
	
	
	public void register_agent(string agent_path) {
		/* register obexd server agent */
		try {
			dbus_obj.register_agent((ObjectPath) agent_path);
		} catch (IOError e) {
			stderr.printf ("Could not get access to org.openobex: %s\n", e.message);
			var dialog = new DialogUI();
			dialog.create("Could not get access to org.openobex:<br>"+e.message+"<br><br>"+
			"Please be sure obexd is running.");
		} 
	
	}
	
	
	public void send_files(BluezRemoteDevice rdevice, string[] files) {
			string path = EMTOOTH_OBEX_AGENT_PATH+"/"+rdevice.addr.replace(":", "_");
			Variant destination = new Variant.string(rdevice.addr);
			
			try {
				SESCONN.register_object((ObjectPath) path, new ObexClientAgent ());
				stderr.printf ("ObexClientAgent created correctly (%s)\n", path);
			} catch (IOError e) {
				stderr.printf ("Could not create ObexClientAgent: %s\n", e.message);
			}
			
			var hash = new HashTable<string, GLib.Variant>(str_hash, str_equal);
			hash.insert("Destination", destination);

			try {
				org.openobex.ObexClient cli = Bus.get_proxy_sync (BusType.SESSION, "org.openobex.client", "/");
				cli.send_files(hash, files, (ObjectPath) path);								
			} catch (IOError e) {
				stderr.printf ("Error on send_files: %s\n", e.message);
			}
	
	
	}
	
}
