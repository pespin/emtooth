public class BluezRemoteDeviceUI {
	
		private BluezRemoteDevice rdevice;
	
		Elm.Object[] gui_container;
	
		public Elm.Win win;
		
		private Elm.Label lb;
		
			
		private Elm.Bg	bg;
		private Elm.Box vbox;
		private Elm.Scroller sc;
		private Elm.Box vbox_in;
		private Elm.Box vbox_fr;
		private Elm.Frame fr;
		private Elm.Box hbox;
		private Elm.Entry entry_name;
		private Elm.Entry entry_disc;
		private Elm.Entry entry_pair;
		private Elm.Toggle tg_disc;
		private Elm.Toggle tg_pair;
		
		private Elm.Button bt_close;
		
	public BluezRemoteDeviceUI(BluezRemoteDevice device) {
			this.rdevice = device;
	 }
		
	public void create() {
		
		gui_container = {};
		
		win = new Elm.Win(null, rdevice.addr+"_win", Elm.WinType.BASIC);
		win.title_set("emtooth - "+rdevice.addr);
		win.autodel_set(true);
		win.smart_callback_add( "delete-request", this.close );
		
		bg = new Elm.Bg(win);
		bg.size_hint_weight_set(1.0, 1.0);
		win.resize_object_add(bg);
		bg.show();
		
		win.resize( DISPLAY_WIDTH, DISPLAY_HEIGHT );
		
		//add vbox
		vbox = new Elm.Box(win);
		win.resize_object_add(vbox);
		vbox.size_hint_weight_set(1.0, 1.0);
		vbox.show();
		

		
		//BOTTOM:
		
		gui_container += (owned) hbox;
		hbox = new Elm.Box(win);
		hbox.horizontal_set(true);
		hbox.size_hint_weight_set(1.0, 0.0);
		hbox.size_hint_align_set(-1.0, 0.0);
		vbox.pack_end(hbox);
		hbox.show();
		
		bt_close = new Elm.Button(win);
		bt_close.label_set("Close");
		bt_close.size_hint_weight_set(1.0, 1.0);
		bt_close.size_hint_align_set(-1.0, -1.0);
		hbox.pack_end(bt_close);
		bt_close.show();
		bt_close.smart_callback_add( "clicked", this.close );

	}


	public void show() {
		win.show();
	}
	
	public void close() {
		stdout.printf("Clossing device window %s\n", rdevice.path);
		win.del();
		ui.hash.remove(rdevice.path); //this removes this object


	}
	



}
