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
    
    
	try {
		Manager root_manager = Bus.get_proxy_sync (BusType.SYSTEM, "org.bluez", "/");

		var adapter_path = root_manager.default_adapter();
		stdout.printf("Default adapter path -> %s\n", adapter_path);
		ADAPTER = new BluezAdapter(adapter_path);
	} catch (IOError e) {
		stderr.printf ("Could not get access to org.bluez: %s\n", e.message);
		return 1;
	} 


	ui = new EmtoothUI();
	ui.main_create();
	ui.main_show();


    Elm.run();
    Elm.shutdown();
    return 0;

}
