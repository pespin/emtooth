using org;
using bluez;



public class SettingsUI : Page {
	
		Elm.Object[] gui_container;

		
		private FrameBox fr_general;
		private FrameBox fr_disc;
		private FrameBox fr_pair;
		
		private LabelBox address;
		private EntryBox name;
		private EntryBox discoverable_timeout;
		private EntryBox pairable_timeout;
			
		private Elm.Scroller sc;
		private Elm.Box vbox_in;
		private Elm.Box hbox;
		private Elm.Toggle tg_disc;
		private Elm.Toggle tg_pair;
		
		private Elm.Button bt_close;
		
		
	public override string get_page_sid() {
			return PAGE_SID_SETTINGS; 
	}
	
	public override string? get_page_title() {
			return "Emtooth - settings"; 
	}
	
	public override void refresh_content() {
			stderr.printf("NOT IMPLEMENTED: refresh_content() on SettingsUI\n");
	}
	
	
	public unowned Elm.Object create(Elm.Win win) {
		
		//add vbox
		vbox = new Elm.Box(win);
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
		address = new LabelBox(win, fr_general.box, "Address", ADAPTER.addr);
		address.show();
		
		// NAME:
		name = new EntryBox(win, fr_general.box, "Name", ADAPTER.name);
		name.show();
		
		name.val.smart_callback_add("changed", () => {Variant val = name.val_get();
													ADAPTER.set_property_("Name", val);
													});
		
		// DISCOVERABLE TOGGLE + TIMEOUT:
		
		fr_disc = new FrameBox(win, vbox_in, "Discovery settings");
		fr_disc.show();
		
		tg_disc = new Elm.Toggle(win);
		tg_disc.label_set("Discoverable:");
		tg_disc.states_labels_set("On", "Off");
		tg_disc.state_set(ADAPTER.discoverable);
		tg_disc.size_hint_align_set(-1.0, 0.0);
		fr_disc.box.pack_end(tg_disc);
		tg_disc.show();
		
		tg_disc.smart_callback_add("changed", () => {Variant val = tg_disc.state_get();
											ADAPTER.set_property_("Discoverable", val); });
		//endl
		
		discoverable_timeout = new EntryBox(win, fr_disc.box, "Discoverable timeout", ADAPTER.discoverable_timeout.to_string());
		discoverable_timeout.show();
		
		discoverable_timeout.val.smart_callback_add("changed", () => {Variant val = (uint) int.parse(discoverable_timeout.val_get());
									ADAPTER.set_property_("DiscoverableTimeout", val); });
	
		
		// PAIRABLE TOGGLE + TIMEOUT:
		
		fr_pair = new FrameBox(win, vbox_in, "Pairing settings");
		fr_pair.show();
		
		tg_pair = new Elm.Toggle(win);
		tg_pair.label_set("Pairable:");
		tg_pair.states_labels_set("On", "Off");
		tg_pair.state_set(ADAPTER.pairable);
		tg_pair.size_hint_align_set(-1.0, 0.0);
		fr_pair.box.pack_end(tg_pair);
		tg_pair.show();
		
		tg_pair.smart_callback_add("changed", () => {Variant val = tg_pair.state_get();
									ADAPTER.set_property_("Pairable", val); });
		
		//endl
		
		pairable_timeout = new EntryBox(win, fr_pair.box, "Pairable timeout", ADAPTER.pairable_timeout.to_string());
		pairable_timeout.show();
		
		discoverable_timeout.val.smart_callback_add("changed", () => {Variant val = (uint) int.parse(pairable_timeout.val_get());
												ADAPTER.set_property_("PairableTimeout", val); });
		
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

		return vbox;
	}

	
	public void close() {
		ui.pop_page(this);
	}
	



}
