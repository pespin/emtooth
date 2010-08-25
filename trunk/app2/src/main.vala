using org;
using bluez;


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
			fso = Bus.get_proxy_sync (BusType.SYSTEM, "org.freesmartphone.ousaged", "/org/freesmartphone/Usage");
			fso.request_resource("Bluetooth");
		} catch (IOError e) {
			stderr.printf ("Could not get access to org.freesmartphone.ousaged: %s\n", e.message);
		}
#endif
    
   
	/* Start bluez_agent */
	try {
		var conn = Bus.get_sync (BusType.SYSTEM);
		//TODO: enable this. there's a vala bug which doesn't let compile
		//conn.register_object (EMTOOTH_BLUEZ_AGENT_PATH, new BluezAgent ());
	
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
		return 1;
	} 

	/* Start ui */
	ui = new EmtoothUI();
	ui.main_create();
	ui.main_show();


	/* ENTER MAIN LOOP */
    Elm.run();
    Elm.shutdown();
    
    
#if _FSO_    
	try {
		fso.release_resource("Bluetooth");
	} catch (IOError e) {
		stderr.printf ("Could not get access to org.freesmartphone.ousaged: %s\n", e.message);
	}
#endif   
    
    return 0;

}
