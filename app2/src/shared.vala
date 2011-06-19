/*
 * 
 * Global variables:
 *
 */
public GLib.MainLoop gmain;
public BluezAdapter ADAPTER;
public EmtoothUI ui;

public Evas.Coord DISPLAY_WIDTH = 320;
public Evas.Coord DISPLAY_HEIGHT = 400;


#if _FSO_   
public FSOusaged fso;
#endif


/*
 * 
 * CONSTANTS/DEFINES:
 *
 */
 
const string EMTOOTH_BLUEZ_AGENT_NAME = "org.emtooth";
const string EMTOOTH_BLUEZ_AGENT_PATH = "/org/emtooth/bluez/agent";
const string EMTOOTH_BLUEZ_AGENT_IFACE = "org.bluez.Agent";

extern const string IMAGESDIR;


//Pager:
const string PAGE_SID_SETTINGS	= "settings";
const string PAGE_SID_MAIN		= "main";
 
//Input:
const string HID_UUID = "00001124-0000-1000-8000-00805f9b34fb";

//Audio:
const string HSP_HS_UUID = "00001108-0000-1000-8000-00805f9b34fb";
const string HSP_AG_UUID = "00001112-0000-1000-8000-00805f9b34fb";
const string HFP_HS_UUID = "0000111e-0000-1000-8000-00805f9b34fb";
const string HFP_AG_UUID = "0000111f-0000-1000-8000-00805f9b34fb";
const string A2DP_SOURCE_UUID = "0000110a-0000-1000-8000-00805f9b34fb";
const string A2DP_SINK_UUID = "0000110b-0000-1000-8000-00805f9b34fb";
const string AVRCP_TARGET_UUID = "0000110c-0000-1000-8000-00805f9b34fb";


/*
 * 
 * MISC FUNCS
 * 
 * 
 */

public string[]? get_dbus_array(Variant? bar) {
	//stdout.printf("CREATING GLIST FROM DBUS...\n"); 
	if(bar==null || bar.is_container()==false) return null;
	
	string[] list;

	size_t max = bar.n_children();
	list = new string[max];
	
	for(size_t i=0; i<max; i++) {
		var item = (string) bar.get_child_value(i);
				list[i] = item;
				stdout.printf("ListAdded: %s;\n",(string) item);
	}
	
	return list;
	
}
