using org;
using bluez;



public class SettingsUI {
	
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
		private Elm.Entry entry;
		private Elm.Toggle tg;
		
		private Elm.Button bt;
		
	public void create() {
		
		gui_container = {};
		
		win = new Elm.Win(null, "settings_win", Elm.WinType.BASIC);
		win.title_set("emtooth - Settings");
		win.autodel_set(true);
		
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
		
		//ADDRESS	
		// add a frame
		fr = new Elm.Frame(win);
		fr.style_set("outdent_top");
		fr.size_hint_weight_set(0.0, 0.0);
		fr.size_hint_align_set(0.5, 0.5);
		vbox_in.pack_end(fr);
		fr.show();
		
		lb = new Elm.Label(win);
		lb.label_set("<b>Address:</b> "+ADAPTER.addr);
		fr.content_set(lb);
		lb.show();
	
		
		// NAME:
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
		hbox.size_hint_align_set(1.0, 0.0);
		hbox.size_hint_weight_set(-1.0, 0.0);
		fr.content_set(hbox);
		hbox.show();
		
		gui_container += (owned) lb;
		lb = new Elm.Label(win);
		lb.label_set("<b>Name:</b> ");
		hbox.pack_end(lb);
		lb.show();
		
		entry = new Elm.Entry(win);
		entry.single_line_set(true);
		entry.entry_set(ADAPTER.name);
		hbox.pack_end(entry);
		entry.show(); 
		//TODO: add callback
		
		// DISCOVERABLE TOGGLE + TIMEOUT:
		// add a frame
		gui_container += (owned) fr;
		fr = new Elm.Frame(win);
		fr.style_set("default");
		fr.size_hint_weight_set(1.0, 0.0);
		fr.size_hint_align_set(-1.0, -1.0);
		vbox_in.pack_end(fr);
		fr.show();
		
		vbox_fr = new Elm.Box(win);
		vbox_fr.size_hint_align_set(-1.0, 0.0);
		vbox_fr.size_hint_weight_set(1.0, 1.0);
		fr.content_set(vbox_fr);
		vbox_fr.show();
		
		gui_container += (owned) hbox;
		hbox = new Elm.Box(win);
		hbox.horizontal_set(true);
		hbox.size_hint_align_set(1.0, 0.0);
		hbox.size_hint_weight_set(-1.0, 0.0);
		vbox_fr.pack_end(hbox);
		hbox.show();
		
		gui_container += (owned) lb;
		lb = new Elm.Label(win);
		lb.label_set("<b>Discoverable:</b> ");
		hbox.pack_end(lb);
		lb.show();
		
		tg = new Elm.Toggle(win);
		tg.states_labels_set("On", "Off");
		tg.state_set(ADAPTER.discoverable);
		hbox.pack_end(tg);
		tg.show();
		
		//endl
		
		gui_container += (owned) hbox;
		hbox = new Elm.Box(win);
		hbox.horizontal_set(true);
		hbox.size_hint_align_set(1.0, 0.0);
		hbox.size_hint_weight_set(-1.0, 0.0);
		vbox_fr.pack_end(hbox);
		hbox.show();
		
		gui_container += (owned) lb;
		lb = new Elm.Label(win);
		lb.label_set("<b>Discovey timeout:</b> ");
		hbox.pack_end(lb);
		lb.show();
		
		gui_container += (owned) entry;
		entry = new Elm.Entry(win);
		entry.single_line_set(true);
		entry.entry_set(ADAPTER.discoverable_timeout.to_string());
		hbox.pack_end(entry);
		entry.show(); 
		
		// PAIRABLE TOGGLE + TIMEOUT:
		// add a frame
		gui_container += (owned) fr;
		fr = new Elm.Frame(win);
		fr.style_set("default");
		fr.size_hint_weight_set(1.0, 0.0);
		fr.size_hint_align_set(-1.0, -1.0);
		vbox_in.pack_end(fr);
		fr.show();
		
		gui_container += (owned) vbox_fr;
		vbox_fr = new Elm.Box(win);
		vbox_fr.size_hint_align_set(-1.0, 0.0);
		vbox_fr.size_hint_weight_set(1.0, 1.0);
		fr.content_set(vbox_fr);
		vbox_fr.show();
		
		gui_container += (owned) hbox;
		hbox = new Elm.Box(win);
		hbox.horizontal_set(true);
		hbox.size_hint_align_set(1.0, 0.0);
		hbox.size_hint_weight_set(-1.0, 0.0);
		vbox_fr.pack_end(hbox);
		hbox.show();
		
		gui_container += (owned) lb;
		lb = new Elm.Label(win);
		lb.label_set("<b>Pairable:</b> ");
		hbox.pack_end(lb);
		lb.show();
		
		gui_container += (owned) tg;
		tg = new Elm.Toggle(win);
		tg.states_labels_set("On", "Off");
		tg.state_set(ADAPTER.pairable);
		hbox.pack_end(tg);
		tg.show();
		
		//endl
		
		gui_container += (owned) hbox;
		hbox = new Elm.Box(win);
		hbox.horizontal_set(true);
		hbox.size_hint_align_set(1.0, 0.0);
		hbox.size_hint_weight_set(-1.0, 0.0);
		vbox_fr.pack_end(hbox);
		hbox.show();
		
		gui_container += (owned) lb;
		lb = new Elm.Label(win);
		lb.label_set("<b>Pairing timeout:</b> ");
		hbox.pack_end(lb);
		lb.show();
		
		gui_container += (owned) entry;
		entry = new Elm.Entry(win);
		entry.single_line_set(true);
		entry.entry_set(ADAPTER.pairable_timeout.to_string());
		hbox.pack_end(entry);
		entry.show();
		
		//BOTTOM:
		
		gui_container += (owned) hbox;
		hbox = new Elm.Box(win);
		hbox.horizontal_set(true);
		hbox.size_hint_align_set(1.0, 0.0);
		hbox.size_hint_weight_set(-1.0, 0.0);
		vbox.pack_end(hbox);
		hbox.show();
		
		bt = new Elm.Button(win);
		bt.label_set("Close");
		bt.size_hint_align_set(1.0, 1.0);
		bt.size_hint_weight_set(-1.0, -1.0);
		hbox.pack_end(bt);
		bt.show();
		bt.smart_callback_add( "clicked", this.close );

	}


	public void show() {
		win.show();
	}
	
	public void close() {
		win.del();
	}
	

}