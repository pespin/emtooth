public class BluezRemoteDeviceUI {
	
		private BluezRemoteDevice rdevice;
	
		Elm.Object[] gui_container;
	
		public Elm.Win win;
	
		private Elm.Bg	bg;
		private Elm.Box vbox;
		private Elm.Scroller sc;
		private Elm.Box vbox_in;
		private Elm.Box vbox_fr;
		private Elm.Frame fr;
		private Elm.Box hbox;
		private Elm.Label lb;
		private Elm.Entry entry_alias;
		private Elm.Toggle tg_con;
		private Elm.Toggle tg_pair;
		private Elm.Toggle tg_trust;
		
		private Elm.Toggle tg_audio;
		private Elm.Toggle tg_input;
		
		private Elm.Button bt_rm;
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
		

		
		//HERE STARTS ALL THE OPTIONS LIST:
		
		sc = new Elm.Scroller(win);
		sc.size_hint_weight_set(1.0, 1.0);
		sc.size_hint_align_set(-1.0, -1.0);
		sc.bounce_set(false, true);
		vbox.pack_end(sc);
		sc.show();
		
		vbox_in = new Elm.Box(win);
		vbox_in.size_hint_align_set(-1.0, -1.0);
		vbox_in.size_hint_weight_set(1.0, 1.0);
		sc.content_set(vbox_in);
		vbox_in.show();
		
		//ADDRESS + NAME
		// add a frame
		fr = new Elm.Frame(win);
		fr.style_set("outdent_top");
		fr.size_hint_weight_set(0.0, 0.0);
		fr.size_hint_align_set(0.5, 0.5);
		vbox_in.pack_end(fr);
		fr.show();
		
		vbox_fr = new Elm.Box(win);
		vbox_fr.size_hint_align_set(-1.0, -1.0);
		vbox_fr.size_hint_weight_set(1.0, 1.0);
		fr.content_set(vbox_fr);
		vbox_fr.show();
		
		lb = new Elm.Label(win);
		lb.label_set("<b>Address:</b> "+rdevice.addr);
		vbox_fr.pack_end(lb);
		lb.show();
		
		gui_container += (owned) lb;
		lb = new Elm.Label(win);
		lb.label_set("<b>Name:</b> "+rdevice.name);
		vbox_fr.pack_end(lb);
		lb.show();
	
		
		// ALIAS:
		// add a frame	
		gui_container += (owned) fr;
		fr = new Elm.Frame(win);
		fr.style_set("default");
		fr.size_hint_weight_set(1.0, 0.0);
		fr.size_hint_align_set(-1.0, -1.0);
		vbox_in.pack_end(fr);
		fr.show();
		
		hbox = new Elm.Box(win);
		hbox.horizontal_set(true);
		hbox.size_hint_align_set(-1.0, 0.0);
		hbox.size_hint_weight_set(1.0, 0.0);
		fr.content_set(hbox);
		hbox.show();
		
		gui_container += (owned) lb;
		lb = new Elm.Label(win);
		lb.label_set("<b>Alias:</b> ");
		hbox.pack_end(lb);
		lb.show();
		
		entry_alias = new Elm.Entry(win);
		entry_alias.single_line_set(true);
		entry_alias.entry_set(rdevice.alias);
		hbox.pack_end(entry_alias);
		entry_alias.show();
		
		entry_alias.smart_callback_add("changed", () => {Variant val = entry_alias.entry_get();
													rdevice.set_property_device("Alias", val); }); 
		
		// CONNECTED TOGGLE:
		// add a frame
		gui_container += (owned) fr;
		fr = new Elm.Frame(win);
		fr.style_set("default");
		fr.size_hint_weight_set(1.0, 0.0);
		fr.size_hint_align_set(-1.0, -1.0);
		vbox_in.pack_end(fr);
		fr.show();
		
		gui_container += (owned) hbox;
		hbox = new Elm.Box(win);
		hbox.horizontal_set(true);
		hbox.size_hint_align_set(-1.0, 0.0);
		hbox.size_hint_weight_set(1.0, 0.0);
		fr.content_set(hbox);
		hbox.show();
		
		gui_container += (owned) lb;
		lb = new Elm.Label(win);
		lb.label_set("<b>Connected:</b> ");
		hbox.pack_end(lb);
		lb.show();
		
		tg_con = new Elm.Toggle(win);
		tg_con.states_labels_set("Yes", "No");
		tg_con.state_set(rdevice.connected);
		tg_con.disabled_set(true);
		hbox.pack_end(tg_con);
		
		tg_con.show();
		tg_con.smart_callback_add("changed", () => {Variant val = tg_con.state_get();
											rdevice.set_property_device("Connected", val); });
		
		// PAIRED TOGGLE:
		// add a frame
		gui_container += (owned) fr;
		fr = new Elm.Frame(win);
		fr.style_set("default");
		fr.size_hint_weight_set(1.0, 0.0);
		fr.size_hint_align_set(-1.0, -1.0);
		vbox_in.pack_end(fr);
		fr.show();
		
		gui_container += (owned) hbox;
		hbox = new Elm.Box(win);
		hbox.horizontal_set(true);
		hbox.size_hint_align_set(-1.0, 0.0);
		hbox.size_hint_weight_set(1.0, 0.0);
		fr.content_set(hbox);
		hbox.show();
		
		gui_container += (owned) lb;
		lb = new Elm.Label(win);
		lb.label_set("<b>Paired:</b> ");
		hbox.pack_end(lb);
		lb.show();
		
		tg_pair = new Elm.Toggle(win);
		tg_pair.states_labels_set("Yes", "No");
		tg_pair.state_set(rdevice.paired);
		tg_pair.disabled_set(rdevice.paired);
		hbox.pack_end(tg_pair);
		tg_pair.show();
		tg_pair.smart_callback_add("changed", 
									() => {		
										if(tg_pair.state_get()==true) {
											tg_pair.disabled_set(true);
											//TODO: call CreatePairedDevice(addr, path, "");
										}
										} );

		
		// TRUSTED TOGGLE:
		// add a frame
		gui_container += (owned) fr;
		fr = new Elm.Frame(win);
		fr.style_set("default");
		fr.size_hint_weight_set(1.0, 0.0);
		fr.size_hint_align_set(-1.0, -1.0);
		vbox_in.pack_end(fr);
		fr.show();
		
		gui_container += (owned) hbox;
		hbox = new Elm.Box(win);
		hbox.horizontal_set(true);
		hbox.size_hint_align_set(-1.0, 0.0);
		hbox.size_hint_weight_set(1.0, 0.0);
		fr.content_set(hbox);
		hbox.show();
		
		gui_container += (owned) lb;
		lb = new Elm.Label(win);
		lb.label_set("<b>Trusted:</b> ");
		hbox.pack_end(lb);
		lb.show();
		
		tg_trust = new Elm.Toggle(win);
		tg_trust.states_labels_set("Yes", "No");
		tg_trust.state_set(rdevice.trusted);
		hbox.pack_end(tg_trust);
		tg_trust.show();
		tg_trust.smart_callback_add("changed", () => {Variant val = tg_trust.state_get();
											rdevice.set_property_device("Trusted", val); });
											
											
											
		//AUDIO TOGGLE:
		if(rdevice.has_service_audio()) {
			gui_container += (owned) fr;
			fr = new Elm.Frame(win);
			fr.style_set("default");
			fr.size_hint_weight_set(1.0, 0.0);
			fr.size_hint_align_set(-1.0, -1.0);
			vbox_in.pack_end(fr);
			fr.show();
				
			gui_container += (owned) hbox;
			hbox = new Elm.Box(win);
			hbox.horizontal_set(true);
			hbox.size_hint_align_set(-1.0, 0.0);
			hbox.size_hint_weight_set(1.0, 0.0);
			fr.content_set(hbox);
			hbox.show();
				
			gui_container += (owned) lb;
			lb = new Elm.Label(win);
			lb.label_set("<b>Connect Audio:</b> ");
			hbox.pack_end(lb);
			lb.show();
				
			tg_audio = new Elm.Toggle(win);
			tg_audio.states_labels_set("Yes", "No");
			tg_audio.state_set(rdevice.connected_audio);
			hbox.pack_end(tg_audio);
			tg_audio.show();
			tg_audio.smart_callback_add("changed", () => { if(tg_audio.state_get()==true)
															rdevice.connect_audio(); 
														   else 
															rdevice.disconnect_audio();
														});
		}
		
		//AUDIO TOGGLE:
		if(rdevice.has_service_input()) {
			gui_container += (owned) fr;
			fr = new Elm.Frame(win);
			fr.style_set("default");
			fr.size_hint_weight_set(1.0, 0.0);
			fr.size_hint_align_set(-1.0, -1.0);
			vbox_in.pack_end(fr);
			fr.show();
				
			gui_container += (owned) hbox;
			hbox = new Elm.Box(win);
			hbox.horizontal_set(true);
			hbox.size_hint_align_set(-1.0, 0.0);
			hbox.size_hint_weight_set(1.0, 0.0);
			fr.content_set(hbox);
			hbox.show();
				
			gui_container += (owned) lb;
			lb = new Elm.Label(win);
			lb.label_set("<b>Connect Input:</b> ");
			hbox.pack_end(lb);
			lb.show();
				
			tg_input = new Elm.Toggle(win);
			tg_input.states_labels_set("Yes", "No");
			tg_input.state_set(rdevice.connected_input);
			hbox.pack_end(tg_input);
			tg_input.show();
			tg_input.smart_callback_add("changed", () => { if(tg_input.state_get()==true)
															rdevice.connect_input(); 
														   else
															rdevice.disconnect_input();
													} );
											
		}									
		
		//RM BUTTON:
		bt_rm = new Elm.Button(win);
		bt_rm.label_set("Reset/Remove Device");
		bt_rm.size_hint_weight_set(0, 0);
		bt_rm.size_hint_align_set(-1.0, -1.0);
		vbox_in.pack_end(bt_rm);
		bt_rm.show();
		
		bt_rm.smart_callback_add( "clicked", () => {ADAPTER.remove_rdevice((GLib.ObjectPath)rdevice.path);} );

	
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
		stdout.printf("Closing device window %s\n", rdevice.path);
		win.del();
		ui.opened_wins.remove(rdevice.path); //this removes/frees this object (ui)


	}
	



}
