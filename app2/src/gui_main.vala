using org;
using bluez;



public class EmtoothUI {
	
		public Elm.Win win;
		public Elm.Label header;
		public Elm.List li;
		
		public Elm.Pager pager;
			
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
		
		pager = new Elm.Pager( win );
		win.resize_object_add( pager );
		pager.size_hint_weight_set( 1.0, 1.0 );
		pager.show();
		
		//add vbox
		vbox = new Elm.Box(win);
		//win.resize_object_add(vbox);
		pager.content_push( vbox );
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
	
	public void pop_page(Elm.Object obj) {
		stderr.printf("pop_page() started!\n");
		//if( obj == pager.content_top_get() ) { //this segfaults...
			pager.content_pop();
			obj.del();
		//}
		stderr.printf("pop_page() finished!\n");
	}
	
	public void pop_rdeviceui(Elm.Object obj, string path) {
		pop_page(obj);
		opened_wins.remove(path); //this removes/frees this object (ui)
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
		unowned Elm.Object page = settings_ui.create(win);
		pager.content_push(page);
	}
	
	
	public void main_show() {
		win.show();
	} 

}


	/* PIN DIALOG */
public class DialogUI : Object {	
	
	Elm.Win inwin;
	Elm.Box vbox;
	Elm.Box vbox_in;
	Elm.Anchorblock lb;
	Elm.Button bt_ok;
	Elm.Scroller sc;
	
	public void create(string text) {
		this.ref(); //let it be unless someone presses the kill button
		
		inwin = ui.win.inwin_add();
		inwin.show();
		
		vbox = new Elm.Box(ui.win);
		inwin.inwin_content_set(vbox);
		vbox.show();
		
		sc = new Elm.Scroller(ui.win);
		sc.size_hint_weight_set(1.0, 1.0);
		sc.size_hint_align_set(-1.0, -1.0);
		sc.bounce_set(false, true);
		vbox.pack_end(sc);
		sc.show();
		
		vbox_in = new Elm.Box(ui.win);
		vbox_in.size_hint_align_set(-1.0, -1.0);
		vbox_in.size_hint_weight_set(1.0, 1.0);
		sc.content_set(vbox_in);
		vbox_in.show();
		
		// add a label
		lb = new Elm.Anchorblock(ui.win);
		lb.text_set(text);
		lb.size_hint_weight_set(1.0, 1.0);
		lb.size_hint_align_set(-1.0, -1.0);
		vbox_in.pack_end(lb);
		lb.show();
		
		bt_ok = new Elm.Button(ui.win);
		bt_ok.label_set("Ok");
		bt_ok.size_hint_align_set(-1.0, -1.0);
		bt_ok.size_hint_weight_set(1.0, 1.0);
		vbox_in.pack_end(bt_ok);
		bt_ok.show();
		bt_ok.smart_callback_add("clicked", () => { this.close(); } );
		
			
	}
	
	public void close() {
		stdout.printf("Closing Dialog window\n");
		//win.del();
		this.unref();
	}
}




	/* PIN DIALOG */
public class PinDialogUI {	
	
	Elm.Win inwin;
	Elm.Box vbox;
	Elm.Box vbox_in;
	Elm.Anchorblock lb;
	Elm.Entry entry;
	Elm.Button bt_ok;
	Elm.Scroller sc;
	
	BluezRemoteDevice rdevice;
	
	public void create(BluezRemoteDevice rdevice) {
		this.rdevice = rdevice;
		
		inwin = ui.win.inwin_add();
		inwin.show();
		
		vbox = new Elm.Box(ui.win);
		inwin.inwin_content_set(vbox);
		vbox.show();
		
		sc = new Elm.Scroller(ui.win);
		sc.size_hint_weight_set(1.0, 1.0);
		sc.size_hint_align_set(-1.0, -1.0);
		sc.bounce_set(false, true);
		vbox.pack_end(sc);
		sc.show();
		
		vbox_in = new Elm.Box(ui.win);
		vbox_in.size_hint_align_set(-1.0, -1.0);
		vbox_in.size_hint_weight_set(1.0, 1.0);
		sc.content_set(vbox_in);
		vbox_in.show();
		
		// add a label
		lb = new Elm.Anchorblock(ui.win);
		lb.text_set("Set the password for device "+rdevice.addr+"<br>and press the button below to proceed:");
		lb.size_hint_weight_set(1.0, 1.0);
		lb.size_hint_align_set(-1.0, -1.0);
		vbox_in.pack_end(lb);
		lb.show();
		
		entry = new Elm.Entry(ui.win);
		entry.single_line_set(true);
		entry.entry_set("1234");
		vbox_in.pack_end(entry);
		entry.show();
		
		bt_ok = new Elm.Button(ui.win);
		bt_ok.label_set("Ok");
		bt_ok.size_hint_align_set(-1.0, -1.0);
		bt_ok.size_hint_weight_set(1.0, 1.0);
		vbox_in.pack_end(bt_ok);
		bt_ok.show();
		bt_ok.smart_callback_add("clicked", () => { this.rdevice.password = this.entry.entry_get(); this.close(); } );
				
	}
	
	public void close() {
		stdout.printf("Closing pinDialog window\n");
		//win.del();
	}
}


public class LabelBox {
	
	private Elm.Label lb;
	private Elm.Label val;
	private Elm.Box box;
	
	public LabelBox(Elm.Win win, Elm.Box parent, string label, string Value) {
		
		box = new Elm.Box(win);
		box.horizontal_set(true);
		box.size_hint_align_set(0.0, 0.0);	
		parent.pack_end(box);
		
		lb = new Elm.Label(win);
		lb.label_set("<b>"+label+":</b>");
		box.pack_end(lb);
		
		val = new Elm.Label(win);
		val.label_set(Value);
		box.pack_end(val);
		
	}
	
	public void show() {
		box.show();
		lb.show();
		val.show();
	}
	
	public string val_get() {
		return this.val.label_get();
	}
	public void val_set(string Value) {
		this.val.label_set(Value);
	}
	
}

public class EntryBox {
	
	private Elm.Label lb;
	public Elm.Entry val;
	private Elm.Frame fr;
	private Elm.Box box;
	
	public EntryBox(Elm.Win win, Elm.Box parent, string label, string Value) {
		
		box = new Elm.Box(win);
		box.horizontal_set(true);
		box.size_hint_weight_set(1.0, 0.0);
        box.size_hint_align_set(-1.0, 0.0);
		parent.pack_end(box);
		
		lb = new Elm.Label(win);
		lb.label_set("<b>"+label+":</b>");
		box.pack_end(lb);
		
		fr = new Elm.Frame(win);
        fr.size_hint_align_set(-1.0, 0.0);
        fr.size_hint_weight_set(1.0, 0.0);
        fr.style_set("outdent_top");
        box.pack_end(fr);
		
		val = new Elm.Entry(win);
		val.size_hint_align_set(-1.0, 0.0);
        val.size_hint_weight_set(1.0, 0.0);
        val.single_line_set(true);
		val.entry_set(Value);
		fr.content_set(val);
		
	}
	
	public void show() {
		box.show();
		lb.show();
		fr.show();
		val.show();
	}
	
	public string val_get() {
		return this.val.entry_get();
	}
	public void val_set(string Value) {
		this.val.entry_set(Value);
	}
	
}



public class FrameBox {
	
	public Elm.Frame fr;
	public Elm.Box box;
	
	public FrameBox(Elm.Win win, Elm.Box parent, string label) {
		
		fr = new Elm.Frame(win);
		fr.label_set(label);
        fr.size_hint_align_set(-1.0, 0.0);
        fr.size_hint_weight_set(1.0, 0.0);
        parent.pack_end(fr);
		
		box = new Elm.Box(win);
		box.size_hint_weight_set(1.0, 0.0);
        box.size_hint_align_set(-1.0, 0.0);
		fr.content_set(box);
		
	}
	
	public void show() {
		box.show();
		fr.show();
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
			
			var device_ui = ui.opened_wins.lookup(rdevice.path);
			if( device_ui == null ) {
				device_ui = new BluezRemoteDeviceUI(rdevice);
				ui.opened_wins.insert(rdevice.path, device_ui);
				unowned Elm.Object page = device_ui.create(ui.win);
				ui.pager.content_push(page);
			} else {
				//device_ui.win.focus_set(true);  //FIXME: (focusing the active win instead of creating it) doesn't work
			}
				
		}
		
	
}

