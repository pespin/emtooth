public class BluezRemoteDeviceUI {
	
		private BluezRemoteDevice rdevice;
		
		private FrameBox fr_general;
		private FrameBox fr_conn;
		private FrameBox fr_audio;
		private FrameBox fr_input;
		
		private LabelBox address;
		private LabelBox name;
		private EntryBox alias;
	
		private Elm.Box vbox;
		private Elm.Scroller sc;
		private Elm.Box vbox_in;
		private Elm.Box hbox;
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
		
	public unowned Elm.Object create(Elm.Win win) {
		
		//add vbox (page)
		vbox = new Elm.Box(win);
		//pager.content_push(vbox);
		vbox.size_hint_weight_set(1.0, 1.0);
		vbox.show();
		
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
		
		//HERE STARTS ALL THE OPTIONS LIST:
		
		fr_general = new FrameBox(win, vbox_in, "General settings");
		fr_general.show();
		
		//ADDRESS
		
		address = new LabelBox(win, fr_general.box, "Address", rdevice.addr);
		address.show();
		
		//NAME
		
		name = new LabelBox(win, fr_general.box, "Name", rdevice.name);
		name.show();
	
		
		// ALIAS:
		
		alias = new EntryBox(win, fr_general.box, "Alias", rdevice.alias);
		alias.show();
		
		alias.val.smart_callback_add("changed", () => {Variant val = alias.val_get();
													rdevice.set_property_device("Alias", val); }); 
		
		
		
		fr_conn = new FrameBox(win, vbox_in, "Connectivity settings");
		fr_conn.show();
		
		// CONNECTED TOGGLE:
		
		tg_con = new Elm.Toggle(win);
		tg_con.label_set("Connected:");
		tg_con.states_labels_set("Yes", "No");
		tg_con.state_set(rdevice.connected);
		tg_con.size_hint_align_set(-1.0, 0.0);
		tg_con.disabled_set(true);
		fr_conn.box.pack_end(tg_con);
		tg_con.show();
		
		tg_con.smart_callback_add("changed", () => {Variant val = tg_con.state_get();
											rdevice.set_property_device("Connected", val); });
		
		// PAIRED TOGGLE:

		
		tg_pair = new Elm.Toggle(win);
		tg_pair.label_set("Paired:");
		tg_pair.states_labels_set("Yes", "No");
		tg_pair.state_set(rdevice.paired);
		tg_pair.disabled_set(rdevice.paired);
		tg_pair.size_hint_align_set(-1.0, 0.0);
		fr_conn.box.pack_end(tg_pair);
		tg_pair.show();
		
		tg_pair.smart_callback_add("changed", 
									() => {		
										if(tg_pair.state_get()==true) {
											tg_pair.disabled_set(true);
											ADAPTER.pair_rdevice(rdevice);
										}
										} );

		
		// TRUSTED TOGGLE:

		
		tg_trust = new Elm.Toggle(win);
		tg_trust.label_set("Trusted:");
		tg_trust.states_labels_set("Yes", "No");
		tg_trust.state_set(rdevice.trusted);
		tg_trust.size_hint_align_set(-1.0, 0.0);
		fr_conn.box.pack_end(tg_trust);
		tg_trust.show();
		
		tg_trust.smart_callback_add("changed", () => {Variant val = tg_trust.state_get();
											rdevice.set_property_device("Trusted", val); });
											
											
											
		//AUDIO TOGGLE:
		if(rdevice.has_service_audio()) {
			
			fr_audio = new FrameBox(win, vbox_in, "Audio settings");
			fr_audio.show();
				
			tg_audio = new Elm.Toggle(win);
			tg_audio.label_set("Connect");
			tg_audio.states_labels_set("Yes", "No");
			tg_audio.state_set(rdevice.connected_audio);
			tg_audio.size_hint_align_set(-1.0, 0.0);
			fr_audio.box.pack_end(tg_audio);
			tg_audio.show();
			tg_audio.smart_callback_add("changed", () => { if(tg_audio.state_get()==true)
															rdevice.connect_audio(); 
														   else 
															rdevice.disconnect_audio();
														});
		}
		
		//INPUT TOGGLE:
		if(rdevice.has_service_input()) {
			
			fr_input = new FrameBox(win, vbox_in, "Input settings");
			fr_input.show();
				
			tg_input = new Elm.Toggle(win);
			tg_input.label_set("Connect");
			tg_input.states_labels_set("Yes", "No");
			tg_input.state_set(rdevice.connected_input);
			tg_input.size_hint_align_set(-1.0, 0.0);
			fr_input.box.pack_end(tg_input);
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
		
		bt_rm.smart_callback_add( "clicked", () => {
											ADAPTER.remove_rdevice((GLib.ObjectPath)rdevice.path);
											this.close();
											} );

	
		//BOTTOM:
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
		
		return vbox;

	}
	
	public void close() {
		stdout.printf("Closing device window %s\n", rdevice.path);
		ui.pop_rdeviceui(vbox, rdevice.path);

	}
	


}
