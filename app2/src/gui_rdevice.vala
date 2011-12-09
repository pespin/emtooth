public class BluezRemoteDeviceUI : Page {
	
		private BluezRemoteDevice rdevice;
		
		private FrameBox fr_general;
		private FrameBox fr_conn;
		private FrameBox fr_audio;
		private FrameBox fr_input;
		
		private LabelBox address;
		private LabelBox name;
		private EntryBox alias;

		private unowned Elm.Scroller sc;
		private unowned Elm.Box vbox_in;
		private unowned Elm.Box hbox;
		private unowned Elm.Toggle tg_con;
		private unowned Elm.Toggle tg_pair;
		private unowned Elm.Toggle tg_trust;
		
		private unowned Elm.Toggle tg_audio;
		private unowned Elm.Toggle tg_input;
		
		private unowned Elm.Button bt_rm;
		private unowned Elm.Button bt_send;
		private unowned Elm.Button bt_close;
		
	public BluezRemoteDeviceUI(BluezRemoteDevice device) {
			this.rdevice = device;
	 }
	 
	public override string get_page_sid() {
			return this.rdevice.path; 
	}
	
	public override string? get_page_title() {
			return (rdevice==null ? "Emtooth - unknown" : "Emtooth - "+rdevice.name); 
	}
		
	public unowned Elm.Object create(Elm.Win win) {
		
		//add vbox (page)
		vbox = Elm.Box.add(win);
		//pager.content_push(vbox);
		vbox.size_hint_weight_set(1.0, 1.0);
		vbox.show();
		
		sc = Elm.Scroller.add(win);
		sc.size_hint_weight_set(1.0, 1.0);
		sc.size_hint_align_set(-1.0, -1.0);
		sc.bounce_set(false, true);
		vbox.pack_end(sc);
		sc.show();
		
		vbox_in = Elm.Box.add(win);
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
		
		tg_con = Elm.Toggle.add(win);
		tg_con.text_set("Connected:");
		tg_con.states_labels_set("Yes", "No");
		tg_con.state_set(rdevice.connected);
		tg_con.size_hint_align_set(-1.0, 0.0);
		tg_con.disabled_set(true);
		fr_conn.box.pack_end(tg_con);
		tg_con.show();
		
		tg_con.smart_callback_add("changed", () => {Variant val = tg_con.state_get();
											rdevice.set_property_device("Connected", val); });
		
		// PAIRED TOGGLE:

		
		tg_pair = Elm.Toggle.add(win);
		tg_pair.text_set("Paired:");
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

		
		tg_trust = Elm.Toggle.add(win);
		tg_trust.text_set("Trusted:");
		tg_trust.states_labels_set("Yes", "No");
		tg_trust.state_set(rdevice.trusted);
		tg_trust.size_hint_align_set(-1.0, 0.0);
		fr_conn.box.pack_end(tg_trust);
		tg_trust.show();
		
		tg_trust.smart_callback_add("changed", () => {Variant val = tg_trust.state_get();
											rdevice.set_property_device("Trusted", val); });
											
											
											
		//AUDIO TOGGLE:
			
		fr_audio = new FrameBox(win, vbox_in, "Audio settings");
		fr_audio.show();	
		tg_audio = Elm.Toggle.add(win);
		tg_audio.text_set("Connect");
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
		tg_audio.disabled_set(!rdevice.has_service_audio());
		
		//INPUT TOGGLE:
			
			fr_input = new FrameBox(win, vbox_in, "Input settings");
			fr_input.show();	
			tg_input = Elm.Toggle.add(win);
			tg_input.text_set("Connect");
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
		tg_input.disabled_set(!rdevice.has_service_input());								

		//SEND FILE BUTTON:
		bt_send = Elm.Button.add(win);
		bt_send.text_set("Send file");
		bt_send.size_hint_weight_set(0, 0);
		bt_send.size_hint_align_set(-1.0, -1.0);
		bt_send.disabled_set(!(MANAGER.enabled));
		vbox_in.pack_end(bt_send);
		bt_send.show();
		
		bt_send.smart_callback_add( "clicked", () => {							
							var fd = new FileDialogSendUI();
							fd.create(rdevice);
												} );

		
		//RM BUTTON:
		bt_rm = Elm.Button.add(win);
		bt_rm.text_set("Reset/Remove Device");
		bt_rm.size_hint_weight_set(0, 0);
		bt_rm.size_hint_align_set(-1.0, -1.0);
		vbox_in.pack_end(bt_rm);
		bt_rm.show();
		
		bt_rm.smart_callback_add( "clicked", () => {
											ADAPTER.remove_rdevice((GLib.ObjectPath)rdevice.path);
											this.close();
											} );

	
		//BOTTOM:
		hbox = Elm.Box.add(win);
		hbox.horizontal_set(true);
		hbox.size_hint_weight_set(1.0, 0.0);
		hbox.size_hint_align_set(-1.0, 0.0);
		vbox.pack_end(hbox);
		hbox.show();
		
		bt_close = Elm.Button.add(win);
		bt_close.text_set("Close");
		bt_close.size_hint_weight_set(1.0, 1.0);
		bt_close.size_hint_align_set(-1.0, -1.0);
		hbox.pack_end(bt_close);
		bt_close.show();
		bt_close.smart_callback_add( "clicked", this.close );
		
		return vbox;

	}

	public override void refresh_content() {
		
			address.val_set(rdevice.addr);
			name.val_set(rdevice.name);
			tg_con.state_set(rdevice.connected);
			tg_pair.state_set(rdevice.paired);
			tg_trust.state_set(rdevice.trusted);
			tg_audio.state_set(rdevice.connected_audio);
			tg_input.state_set(rdevice.connected_input);
						
			tg_pair.disabled_set(rdevice.paired);
			
			tg_audio.disabled_set(!rdevice.has_service_audio());
			
			tg_input.disabled_set(!rdevice.has_service_input());
	}


}
