using org;
using bluez;



public class MainUI : Page {
	
		public unowned Elm.Label header;
		public unowned Elm.List li;
		
		private unowned Elm.Win win;
			
		private unowned Elm.Box hbox;
		private unowned Elm.Frame fr;
		private unowned Elm.Box hbox1;
		private unowned Elm.Button bt_start;
		private unowned Elm.Button bt_stop;
		private unowned Elm.Button bt;

		public HashTable<string,ListItemHandler> rdevices_ui_list; 
		
		public MainUI() {
				//super();
				rdevices_ui_list = new HashTable<string,ListItemHandler>(str_hash, str_equal);
		}
		

	public unowned Elm.Object create(Elm.Win win) {
		
		this.win = win;
		
		//add vbox
		vbox = Elm.Box.add(win);
		vbox.size_hint_weight_set( 1.0, 1.0 );
		vbox.show();
		
		//add button hbox
		hbox = Elm.Box.add(win);
		hbox.horizontal_set(true);	
		hbox.size_hint_weight_set( 1.0, 0.0 );
		hbox.size_hint_align_set( -1.0, 0.0 );
		vbox.pack_end(hbox);
		hbox.show();
		
		// add a frame
		fr = Elm.Frame.add(win);
		fr.style_set("outdent_top");
		fr.size_hint_weight_set(0.0, 0.0);
		fr.size_hint_align_set(0.0, -1.0);
		hbox.pack_end(fr);
		fr.show();
		
		// add a label
		header = Elm.Label.add(win);
		header.text_set("Discovering Devices...");
		fr.content_set(header);
		header.show();

		//add list
		li = Elm.List.add(win);
		li.scale_set(1.0);
		li.size_hint_weight_set(1.0, 1.0);
		li.size_hint_align_set(-1.0, -1.0);
		vbox.pack_end(li);
		//li.smart_callback_add( "clicked", cb_device_list_selected );
		li.show();
	
		//add button hbox1
		hbox1 = Elm.Box.add(win);
		hbox1.horizontal_set(true);	
		hbox1.size_hint_weight_set( 1.0, 0.0 );
		hbox1.size_hint_align_set( -1.0, 0.0 );
		vbox.pack_end(hbox1);
		hbox1.show();

		//add buttons to hbox1
		bt_start = Elm.Button.add(win);
		bt_start.text_set("Start Discovery");
		bt_start.size_hint_weight_set( 1.0, 1.0 );
		bt_start.size_hint_align_set( -1.0, -1.0 );
		bt_start.smart_callback_add( "clicked", cb_bt_start_clicked );
		
		
		bt_stop = Elm.Button.add(win);
		bt_stop.text_set("Stop Discovery");
		bt_stop.size_hint_weight_set( 1.0, 1.0 );
		bt_stop.size_hint_align_set( -1.0, -1.0 );
		bt_stop.smart_callback_add( "clicked", cb_bt_stop_clicked );
		hbox1.pack_end(bt_stop);
		bt_stop.show();
	
	
		bt = Elm.Button.add(win);
		bt.text_set("Settings");
		bt.size_hint_weight_set( 1.0, 1.0 );
		bt.size_hint_align_set( -1.0, -1.0 );
		hbox1.pack_end(bt);
		bt.show();
		bt.smart_callback_add( "clicked", cb_bt_settings_clicked );
	
		return vbox;
	}
	
	public void add_rdevice_to_ui(BluezRemoteDevice rdevice) {
		
		
		message("Adding rdevice " + rdevice.path + " to ui-list");
		
		var opener = new ListItemHandler(win, rdevice);
		opener.item = this.li.item_append(opener.format_item_label(rdevice), null, opener.icon, opener.go);
		rdevices_ui_list.insert(rdevice.path, (owned) opener);
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
		
		header.text_set("Discovering Devices...");
		
		ADAPTER.start_discovery();		
	}
	
	public void discovery_stop() {
		bt_stop.hide();
		hbox1.unpack(bt_stop);
		
		hbox1.pack_start(bt_start);
		bt_start.show();
		
		header.text_set(ADAPTER.num_devices_found.to_string()+" Devices Found:");
		
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
			//stderr.printf("NOT IMPLEMENTED: refresh_content() on MainUI\n");
			
			
		HashTableIter<string,ListItemHandler> it = HashTableIter<string,ListItemHandler>(rdevices_ui_list);
		
		unowned string? path;
		unowned ListItemHandler? handler;
		while(it.next(out path, out handler)) {
			handler.refresh_content();
		}
		
		li.go();
		
	}

}
