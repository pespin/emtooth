using org;
using bluez;



public class EmtoothUI {
	
		//used to take refs from list items... needs better solution
		private Elm.ListItem[] item_container; 
	
		public Elm.Win win;
		public Elm.Label header;
		public Elm.List li;
			
		private Elm.Bg	bg;
		private Elm.Box vbox;
		private Elm.Box hbox;
		private Elm.Frame fr;
		private Elm.Box hbox1;
		private Elm.Button bt_start;
		private Elm.Button bt_stop;
		private Elm.Button bt;
		
		
		//Elm.Icon ico;
		//Elm.ListItem item;
	public void main_create() {
		
		win = new Elm.Win( null, "main_win", Elm.WinType.BASIC );
		win.title_set( "emtooth" );
		//win.autodel_set( true );
		win.smart_callback_add( "delete-request", Elm.exit );
		
		bg = new Elm.Bg(win);
		bg.size_hint_weight_set( 1.0, 1.0 );
		bg.show();
		win.resize_object_add(bg);
		
		win.resize( DISPLAY_WIDTH, DISPLAY_HEIGHT );
		
		//add vbox
		vbox = new Elm.Box(win);
		win.resize_object_add(vbox);
		vbox.size_hint_weight_set( 1.0, 1.0 );
		vbox.show();
		
		//add button hbox
		hbox = new Elm.Box(win);
		hbox.horizontal_set(true);	
		hbox.size_hint_weight_set( 1.0, 0.0 );
		hbox.size_hint_align_set( -1.0, 0.0 );
		vbox.pack_end(hbox);
		hbox.show();
		
		// add a frame
		fr = new Elm.Frame(win);
		fr.style_set("outdent_top");
		fr.size_hint_weight_set(0.0, 0.0);
		fr.size_hint_align_set(0.0, -1.0);
		hbox.pack_end(fr);
		fr.show();
		
		// add a label
		header = new Elm.Label(win);
		header.label_set("Discovering Devices...");
		fr.content_set(header);
		header.show();

		//add list
		li = new Elm.List(win);
		li.scale_set(1.0);
		//li.bounce_set(false,false);
		li.size_hint_weight_set(1.0, 1.0);
		li.size_hint_align_set(-1.0, -1.0);
		vbox.pack_end(li);
		//li.smart_callback_add( "clicked", cb_device_list_selected );
		li.show();
	
		//add button hbox1
		hbox1 = new Elm.Box(win);
		hbox1.horizontal_set(true);	
		hbox1.size_hint_weight_set( 1.0, 0.0 );
		hbox1.size_hint_align_set( -1.0, 0.0 );
		vbox.pack_end(hbox1);
		hbox1.show();

		//add buttons to hbox1
		bt_start = new Elm.Button(win);
		bt_start.label_set("Start Discovery");
		bt_start.size_hint_weight_set( 1.0, 1.0 );
		bt_start.size_hint_align_set( -1.0, -1.0 );
		bt_start.smart_callback_add( "clicked", cb_bt_start_clicked );
		
		
		bt_stop = new Elm.Button(win);
		bt_stop.label_set("Stop Discovery");
		bt_stop.size_hint_weight_set( 1.0, 1.0 );
		bt_stop.size_hint_align_set( -1.0, -1.0 );
		bt_stop.smart_callback_add( "clicked", cb_bt_stop_clicked );
		hbox1.pack_end(bt_stop);
		bt_stop.show();
	
	
		//TODO: add button callbacks
		bt = new Elm.Button(win);
		bt.label_set("Settings");
		bt.size_hint_weight_set( 1.0, 1.0 );
		bt.size_hint_align_set( -1.0, -1.0 );
		hbox1.pack_end(bt);
		bt.show();
		//bt.smart_callback_add( "clicked", cb_settings_dialog );
	

	}
	
	
	public void add_rdevice_to_list(BluezRemoteDevice rdevice) {
		
		
		message("Adding rdevice " + rdevice.addr + " to ui-list");
		string label = "["+ rdevice.addr + "] " + rdevice.alias;
		
		Elm.ListItem item = this.li.append(label, null, null, null);
		item_container += (owned) item;
		this.li.go();
	}
	
	
	private void cb_bt_start_clicked() {
		stdout.printf("Start Discovery button pressed.\n");
		bt_start.hide();
		hbox1.unpack(bt_start);
		
		hbox1.pack_start(bt_stop);
		bt_stop.show();
		
		header.label_set("Discovering Devices...");
		
		ADAPTER.start_discovery();
	}
	
	private void cb_bt_stop_clicked() {
		stdout.printf("Stop Discovery button pressed.\n");
		bt_stop.hide();
		hbox1.unpack(bt_stop);
		
		hbox1.pack_start(bt_start);
		bt_start.show();
		
		header.label_set(ADAPTER.num_devices_found().to_string()+" Devices Found:");
		
		ADAPTER.stop_discovery();
	}
	
	
	
	public void main_show() {
		win.show();
	} 

}
