using org;
using bluez;



public class KnownUI : Page {
		private unowned Elm.Box hbox;
		private unowned Elm.Box hbox1;
		private unowned Elm.Button bt;
		
		
		private unowned Elm.Win win;
		
		public unowned Elm.List li;	

		public HashTable<string,ListItemHandler> rdevices_ui_list; 
		
		public KnownUI() {
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
	
		bt = Elm.Button.add(win);
		bt.text_set("Close");
		bt.size_hint_weight_set( 1.0, 1.0 );
		bt.size_hint_align_set( -1.0, -1.0 );
		hbox1.pack_end(bt);
		bt.show();
		bt.smart_callback_add( "clicked", this.close );
	
		populate_list.begin();
	
		return vbox;
	}
	
	public void add_rdevice_to_ui(BluezRemoteDevice rdevice) {
		
		
		message("Adding rdevice " + rdevice.path + " to known ui-list");
		
		var opener = new ListItemHandler(win, rdevice);
		opener.item = this.li.item_append(opener.format_item_label(rdevice), null, opener.icon, opener.go);
		rdevices_ui_list.insert(rdevice.path, (owned) opener);
		this.li.go();
	}
	
	public void remove_rdevice_from_ui(string path) {

		message("Removing rdevice " + path + " from known ui-list\n");
		rdevices_ui_list.remove(path);
		this.li.go();
	}
	
	
	private async void populate_list() {
			stderr.printf("populate_List() started.\n");
			List<unowned BluezRemoteDevice> l = ADAPTER.get_all_rdevices();
			foreach(var device in l) {
				stderr.printf("foreach: device %s.\n", device.path);
				if(rdevices_ui_list.lookup(device.path)==null)
					add_rdevice_to_ui(device);
			}
	}
	

	public override string get_page_sid() {
		return PAGE_SID_KNOWN; 
	}
	
	public override string? get_page_title() {
		return "Emtooth - known devices"; 
	}
	
	public override void refresh_content() {
		rdevices_ui_list = new HashTable<string,ListItemHandler>(str_hash, str_equal);
		populate_list.begin();
	}

}
