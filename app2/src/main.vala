using org;
using bluez;


void on_bus_acquired (DBusConnection conn) {
	/* Start bluez_agent */
	try {
		
		conn.register_object (EMTOOTH_BLUEZ_AGENT_PATH, new BluezAgent ());
		stderr.printf ("service org.emtooth created correctly\n");
	} catch (IOError e) {
		stderr.printf ("Could not create service org.emtooth: %s\n", e.message);
	}
	
	    /* Get default bluez adapter and register agent */
	try {
		Manager root_manager = Bus.get_proxy_sync (BusType.SYSTEM, "org.bluez", "/");

		var adapter_path = root_manager.default_adapter();
		stdout.printf("Default adapter path -> %s\n", adapter_path);
		ADAPTER = new BluezAdapter(adapter_path);
		ADAPTER.register_agent(EMTOOTH_BLUEZ_AGENT_PATH);
	} catch (IOError e) {
		stderr.printf ("Could not get access to org.bluez: %s\n", e.message);
		var dialog = new DialogUI();
		dialog.create("Could not get access to org.bluez:<br>"+e.message+"<br><br>"+
		"Please be sure bluetoothd is running.");
		dialog.show();
		//exit(1);
	} 

}


int main(string[] args) {

	Elm.init(args);
	stdout.printf("Emtooth started!\n");
		
		/* create a glib mainloop */
    gmain = new GLib.MainLoop( null, false );

    /* integrate glib mainloop into ecore mainloop */
    if ( Ecore.MainLoop.glib_integrate() )
    {
        message( "glib mainloop integration successfully completed" );
    }
    else
    {
        warning( "could not integrate glib mainloop. did you compile glib mainloop support into ecore?" );
    }
    
    
   /* Start bluez_agent */
   Bus.own_name (BusType.SYSTEM, EMTOOTH_BLUEZ_AGENT_NAME, BusNameOwnerFlags.NONE,
			  on_bus_acquired,
			  () => stderr.printf ("Bus name acquired\n"),
			  () => stderr.printf ("Could not acquire bus name\n"));
    
    
#if _FSO_
    /* Get Bluetooth resource if fso is running */
    try {
			stderr.printf ("Requesting \"Bluetooth\" resource to org.freesmartphone.ousaged...\n");
			fso = Bus.get_proxy_sync (BusType.SYSTEM, "org.freesmartphone.ousaged", "/org/freesmartphone/Usage");
			fso.request_resource("Bluetooth");
		} catch (IOError e) {
			stderr.printf ("ERR: Could not get access to org.freesmartphone.ousaged: %s\n", e.message);
		}
#endif
    

	/* Start ui */
	ui = new EmtoothUI();
	ui.main_create();
	ui.main_show();


	/* ENTER MAIN LOOP */
    Elm.run();
    Elm.shutdown();
    
    
#if _FSO_    
	try {
		stderr.printf ("Releasing \"Bluetooth\" resource...\n");
		fso.release_resource("Bluetooth");
	} catch (IOError e) {
		stderr.printf ("Could not get access to org.freesmartphone.ousaged: %s\n", e.message);
	}
#endif   
    
    return 0;

}
