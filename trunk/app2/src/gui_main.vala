using org;
using bluez;



public class EmtoothUI {
	
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
		
		public HashTable<string,BluezRemoteDeviceUI> opened_wins;
		public HashTable<string,Elm.ListItem> rdevices_ui_list; 
		
		
		public EmtoothUI() {
				opened_wins = new HashTable<string,BluezRemoteDeviceUI>(str_hash, str_equal);
				rdevices_ui_list = new HashTable<string,Elm.ListItem>(str_hash, str_equal);
		}
		

	public void main_create() {
		
		win = new Elm.Win( null, "main_win", Elm.WinType.BASIC );
		win.title_set( "emtooth" );

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
		bt.smart_callback_add( "clicked", cb_bt_settings_clicked );
	

	}
	
	
	public void add_rdevice_to_ui(BluezRemoteDevice rdevice) {
		
		
		message("Adding rdevice " + rdevice.path + " to ui-list");
		string label = "["+ rdevice.addr + "] " + rdevice.alias;
		
		var opener = new WinOpener(rdevice.path);
		Elm.ListItem item;
		item = this.li.append(label, null, null, opener.go);
		
		rdevices_ui_list.insert(rdevice.path, (owned) item);
		this.li.go();
	}
	
	public void remove_rdevice_from_ui(string path) {
		
		
		message("Removing rdevice " + path + " from ui-list\n");
		rdevices_ui_list.remove(path);
		ui.opened_wins.remove(path);
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
		
		header.label_set(ADAPTER.num_devices_found.to_string()+" Devices Found:");
		
		ADAPTER.stop_discovery();
	}
	
	
		private void cb_bt_settings_clicked() {
		stdout.printf("Settings button pressed.\n");
		
		settings_ui = new SettingsUI();
		settings_ui.create();
		settings_ui.show();
	}
	
	
	public void main_show() {
		win.show();
	} 

}




	/* PIN DIALOG */
public class PinDialogUI {	
	
	Elm.Win win;
	Elm.Win inwin;
	Elm.Bg bg;
	Elm.Box vbox;
	Elm.Label lb;
	Elm.Entry entry;
	Elm.Button bt_ok;
	BluezRemoteDevice rdevice;
	
	public void create(BluezRemoteDevice rdevice) {
		this.rdevice = rdevice;
		
		win = new Elm.Win(null, rdevice.addr+"_pin", Elm.WinType.DIALOG_BASIC);
		win.title_set("Request Pin:");
		win.autodel_set(true);
		
		
		bg = new Elm.Bg(win);
		bg.size_hint_weight_set(1.0, 1.0);
		win.resize_object_add(bg);
		bg.show();
		
		inwin = win.inwin_add();
		inwin.show();
		
		vbox = new Elm.Box(win);
		inwin.inwin_content_set(vbox);
		vbox.show();
		
		// add a label
		lb = new Elm.Label(win);
		lb.label_set("Set the password for device "+rdevice.addr+"<br>and press the button below to proceed:");
		vbox.pack_end(lb);
		lb.show();
		
		entry = new Elm.Entry(win);
		entry.single_line_set(true);
		entry.entry_set("1234");
		vbox.pack_end(entry);
		entry.show();
		
		bt_ok = new Elm.Button(win);
		bt_ok.label_set("Ok");
		bt_ok.size_hint_align_set(-1.0, -1.0);
		bt_ok.size_hint_weight_set(1.0, 1.0);
		vbox.pack_end(bt_ok);
		bt_ok.show();
		bt_ok.smart_callback_add("clicked", () => { this.rdevice.password = this.entry.entry_get(); this.close(); } );
				
	}

	public void show() {
		win.show();
	} 
	
	public void close() {
		stdout.printf("Closing pinDialog window\n");
		win.del();
	}
}

//we all love dirty hacks!
//this is needed because of vala internals. It's necessary to pass an object to callback
private class WinOpener : Object {
	
		public string path;
		
		public WinOpener(string str) {
			this.ref();
			path = str;
		}
		
		
		public void go () { 
		stderr.printf ("PATH=" + this.path + ";\n"); 
		open_rdevice_win(this.path); 
		//this.unref ();
		//^ we don't want this to be unrefd, as it has to be kept alive to be called by clicked signal
	}
	
		private void open_rdevice_win(string path) {
			message("Opening win for rdevice "+path+"...\n");
			unowned BluezRemoteDevice rdevice = ADAPTER.get_rdevice_by_path(path);
			if(rdevice==null) {
				warning("Trying to open NULL device!\n");
				return;
			}
			
			var device_ui = ui.opened_wins.lookup(rdevice.path);
			if( device_ui == null ) {
				device_ui = new BluezRemoteDeviceUI(rdevice);
				ui.opened_wins.insert(rdevice.path, device_ui);
				device_ui.create();
			}
				device_ui.show(); //TODO: !null then don't show, but focus the win
		}
		
	
}

