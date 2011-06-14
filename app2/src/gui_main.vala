using org;
using bluez;



public class MainUI : Page {
	
		public Elm.Label header;
		public Elm.List li;
			
		private Elm.Box hbox;
		private Elm.Frame fr;
		private Elm.Box hbox1;
		private Elm.Button bt_start;
		private Elm.Button bt_stop;
		private Elm.Button bt;

		public HashTable<string,Elm.ListItem> rdevices_ui_list; 
		
		
		public MainUI() {
				//super();
				rdevices_ui_list = new HashTable<string,Elm.ListItem>(str_hash, str_equal);
		}
		

	public unowned Elm.Object create(Elm.Win win) {
		
		//add vbox
		vbox = new Elm.Box(win);
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
	
		return vbox;
	}
	
	public void add_rdevice_to_ui(BluezRemoteDevice rdevice) {
		
		
		message("Adding rdevice " + rdevice.path + " to ui-list");
		string label = "["+ rdevice.addr + "] " + rdevice.alias;
		
		var opener = new WinOpener(rdevice.path);
		Elm.ListItem item;
		item = this.li.append(label, null, null, opener.go);
		opener.item = item; // this is needed to unselect ListItem to be able to press it again.
		rdevices_ui_list.insert(rdevice.path, (owned) item);
		this.li.go();
	}
	
	public void remove_rdevice_from_ui(string path) {

		message("Removing rdevice " + path + " from ui-list\n");
		rdevices_ui_list.remove(path);
		this.li.go();
	}
	
	
	public void discovery_start() {
		bt_start.hide();
		hbox1.unpack(bt_start);
		
		hbox1.pack_start(bt_stop);
		bt_stop.show();
		
		header.label_set("Discovering Devices...");
		
		ADAPTER.start_discovery();		
	}
	
	public void discovery_stop() {
		bt_stop.hide();
		hbox1.unpack(bt_stop);
		
		hbox1.pack_start(bt_start);
		bt_start.show();
		
		header.label_set(ADAPTER.num_devices_found.to_string()+" Devices Found:");
		
		ADAPTER.stop_discovery();		
	}
	
	private void cb_bt_start_clicked() {
		stdout.printf("Start Discovery button pressed.\n");
		discovery_start();
	}
	
	private void cb_bt_stop_clicked() {
		stdout.printf("Stop Discovery button pressed.\n");
		discovery_stop();
	}
	
	
	private void cb_bt_settings_clicked() {
		stdout.printf("Settings button pressed.\n");
		
		SettingsUI settings_ui = new SettingsUI();
		settings_ui.create(ui.win);
		ui.push_page(settings_ui);
	}
	
	
	public override string get_page_sid() {
			return PAGE_SID_MAIN; 
	}
	
	public override string? get_page_title() {
			return "Emtooth - settings"; 
	}
	
	public override void refresh_content() {
			stderr.printf("NOT IMPLEMENTED: refresh_content() on MainUI\n");
	}

}


//we all love dirty hacks!
//this is needed because of vala internals. It's necessary to pass an object to callback
private class WinOpener : Object {
	
		public string path;
		public unowned Elm.ListItem item;
		
		public WinOpener(string str) {
			this.ref();
			path = str;
		}
		
		
		public void go () { 
		stderr.printf ("PATH=" + this.path + ";\n"); 
		stderr.printf ("label=" + this.item.label_get() + ";\n");
		this.item.selected_set(false);
		open_rdevice_win(this.path); 
		//this.unref ();
		//^ we don't want this to be unrefd, as it has to be kept alive to be called by clicked signal
	}
	
		private void open_rdevice_win(string path) {
			message("Opening win for rdevice "+path+"...\n");
			BluezRemoteDevice rdevice = ADAPTER.get_rdevice_by_path(path);
			if(rdevice==null) {
				warning("Trying to open NULL device!\n");
				return;
			}
			
			//if true, this means probably that rdevice.ref_count==0
			if(rdevice.path==null) warning("rdevice.path is null!!!\n");
			
			BluezRemoteDeviceUI device_ui = new BluezRemoteDeviceUI(rdevice);
			device_ui.create(ui.win);
			ui.push_page(device_ui);

		}
		
	
}

