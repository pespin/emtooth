using org;
using openobex;


public class ObexManager : Object {
	
	private ObexDBusManager dbus_obj;
	
	public bool enabled {get; private set; default=false;}
	
	private HashTable<string,ObexTransfer> transfers;
	
	public ObexManager() {
		
		transfers = new HashTable<string,ObexTransfer>(str_hash, str_equal);
		
				/* Start obex_agent */
		try {
			dbus_obj = Bus.get_proxy_sync (BusType.SESSION, "org.openobex", "/");
			stderr.printf ("ObexDBusManager on session bus created correctly\n");
		} catch (Error e) {
			stderr.printf ("ObexDBusManager creation error: %s\n", e.message);
			return;
		}
		
		dbus_obj.transfer_started.connect ((transfer) => { 
						stderr.printf("Manager: transfer_started [%s]\n", transfer); 
						transfers.insert(transfer, new ObexTransfer(transfer));
					} );
		dbus_obj.transfer_completed.connect ((transfer) => { 
						stderr.printf("Manager: transfer_completed [%s]\n", transfer); 
						unowned ObexTransfer t = transfers.lookup(transfer);
						if(t!=null) {
							t.complete();
							transfers.remove(transfer);
						}
					} );
        
        
        dbus_obj.session_created.connect (() => { stderr.printf("Manager: session_created\n"); } );
        dbus_obj.session_removed.connect (() => { stderr.printf("Manager: session_removed\n"); } );
		
		this.enabled=true;
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


public class ObexTransfer : Object {
	
	public string path {get; private set;}
	private ObexDBusTransfer dbus_obj;
	TransferDialogUI dialog;
	Timer t;
	
	public ObexTransfer(string path) {
		this.path = path;
		
		try {
			dbus_obj = Bus.get_proxy_sync (BusType.SESSION, "org.openobex", path);
			stderr.printf ("transfer %s created correctly\n", path);
		} catch (Error e) {
			stderr.printf ("Could not create transfer %s: %s\n", path, e.message);
		}
		
		dialog = new TransferDialogUI();
		
		dialog.create("...", 0);
		t = new Timer();
		t.reset();
		t.start();

		dbus_obj.progress.connect ((total, bytes) => {
				dialog.size = total/1000;

				uint64 seconds = (uint64) t.elapsed();
				if(seconds==0) seconds++; //avoid dividing by 0 ;)
				uint64 speed = bytes / (seconds * 1000);
	
			/* stderr.printf( @"ObexTransfer [$path]: progress ($bytes bytes transfered, $speed KB/s)\n"); */
			dialog.refresh(bytes/1000, speed);
		} );

	}
	
	public void complete() {
		stderr.printf("ObexTransfer [%s]: complete\n", path);
		dialog.complete();
	}
}
