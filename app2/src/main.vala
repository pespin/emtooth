using org;
using bluez;
using openobex;


void on_bus_system_acquired (DBusConnection conn) {
	
	SYSCONN = conn;
	
	/* Start bluez_agent */
	try {
		
		conn.register_object (EMTOOTH_BLUEZ_AGENT_PATH, new BluezAgent ());
		stderr.printf ("service org.emtooth on system bus created correctly\n");
	} catch (IOError e) {
		stderr.printf ("Could not create service org.emtooth on system bus: %s\n", e.message);
	}
	
	    /* Get default bluez adapter and register agent */
	try {
		BluezDBusManager root_manager = Bus.get_proxy_sync (BusType.SYSTEM, "org.bluez", "/");

		var adapter_path = root_manager.default_adapter();
		stdout.printf("Default adapter path -> %s\n", adapter_path);
		ADAPTER = new BluezAdapter(adapter_path);
		ADAPTER.register_agent(EMTOOTH_BLUEZ_AGENT_PATH);
	} catch (Error e) {
		stderr.printf ("Could not get access to org.bluez: %s\n", e.message);
		var dialog = new DialogUI();
		dialog.create("Could not get access to org.bluez:<br>"+e.message+"<br><br>"+
		"Please be sure bluetoothd is running.");
		//exit(1);
	} 

}


void on_bus_session_acquired (DBusConnection conn) {
	
	SESCONN = conn;
	
	/* Start obex_agent */
	try {
		conn.register_object (EMTOOTH_OBEX_AGENT_PATH, new ObexAgent ());
		stderr.printf ("ObexAgent on session bus created correctly\n");
	} catch (IOError e) {
		stderr.printf ("Could not create ObexAgent on session bus: %s\n", e.message);
	}
	
	
	MANAGER = new ObexManager();
	if(MANAGER.enabled)
		MANAGER.register_agent(EMTOOTH_OBEX_AGENT_PATH);

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
    
    
#if _FSO_
    /* Get Bluetooth resource if fso is running */
    try {
			stderr.printf ("Requesting \"Bluetooth\" resource to org.freesmartphone.ousaged...\n");
			fso = Bus.get_proxy_sync (BusType.SYSTEM, "org.freesmartphone.ousaged", "/org/freesmartphone/Usage");
			fso.request_resource("Bluetooth");
		} catch (IOError e) {
			stderr.printf ("ERR: Could not get access to org.freesmartphone.ousaged: %s\n", e.message);
		}
	
	Posix.sleep(5);
#endif

	   /* Start bluez_agent */
   Bus.own_name (BusType.SYSTEM, EMTOOTH_SERVICE_NAME, BusNameOwnerFlags.NONE,
			  on_bus_system_acquired,
			  () => stderr.printf ("System Bus name acquired\n"),
			  () => stderr.printf ("Could not acquire system bus name\n"));

	   /* Start obex_agent */
   Bus.own_name (BusType.SESSION, EMTOOTH_SERVICE_NAME, BusNameOwnerFlags.NONE,
			  on_bus_session_acquired,
			  () => stderr.printf ("Session Bus name acquired\n"),
			  () => stderr.printf ("Could not acquire session bus name\n"));
    

	/* Start ui */
	ui = new EmtoothUI();
	ui.create();


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
