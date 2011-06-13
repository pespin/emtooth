using org;
using bluez;



public class EmtoothUI {
	
		public Elm.Win win;
		
		private Elm.Pager pager;
			
		private Elm.Bg	bg;
		public MainUI  mui;
		
		public List<Page> page_stack;
		
		
		public EmtoothUI() {
				page_stack = new List<Page>();
				mui = new MainUI();
		}
		

	public void create() {
		
			win = new Elm.Win( null, "main_win", Elm.WinType.BASIC );
			win.title_set( "emtooth" );
			win.smart_callback_add( "delete,request", Elm.exit );
			
			bg = new Elm.Bg(win);
			bg.size_hint_weight_set( 1.0, 1.0 );
			bg.show();
			win.resize_object_add(bg);
			
			win.resize( DISPLAY_WIDTH, DISPLAY_HEIGHT );
			
			pager = new Elm.Pager( win );
			win.resize_object_add( pager );
			pager.size_hint_weight_set( 1.0, 1.0 );
			pager.show();
			
			unowned Elm.Object page;
			page = mui.create(win);
			pager.content_push(page);
			
			win.show();
	
	}
	
	public void pop_page(Page page) {
		stderr.printf("pop_page() started!\n");
		//if( obj == pager.content_top_get() ) { //this segfaults...
			pager.content_pop();
			page_stack.remove(page);
			
			string? last_title = get_last_title();
			if(last_title != null)
				win.title_set(last_title);
			else
				win.title_set(mui.get_page_title());
		//}
		stderr.printf("pop_page() finished!\n");
	}


	public void push_page(Page obj) {
		
		page_stack.prepend(obj);
		
		unowned Elm.Object? page = obj.get_page();
		if(page!=null) {
			
			pager.content_push(page);
			
			string title = obj.get_page_title();
			if(title!=null)
				win.title_set(title);
		
		} else 
			stderr.printf("push_page(): pager.content_push(NULL)!!!\n");
	}
	
	
	private string? get_last_title() {
		
		string title;
		unowned List<Page> l = page_stack;
		
		while(l!=null) {
			title = l.data.get_page_title();
			if(title!=null) return title;
		} 
		
		return null;
		
	}

}

/* PAGE: all UIs inherit from this, and is used by EmtoothUI */
public abstract class Page : Object {
	
	protected Elm.Box vbox;
	
	public Page() {
			vbox = null;
	}
	
	public unowned Elm.Object? get_page() {
			return vbox;
	}
	
	public abstract string get_page_sid();
	
	public abstract string? get_page_title();
	
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


