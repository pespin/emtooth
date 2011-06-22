using org;
using openobex;

[DBus (name = "org.openobex.Agent")]
public class ObexClientAgent : Object {

	ObexTransfer transfer;
	Timer t;
	uint64 size;
	string name;
	string filename;
	TransferDialogUI dialog;
// DBUS METHODS:

    public void release() {
		
		stderr.printf("ObexAgent: release\n");
		
	}
	
	
	public string request(ObjectPath transfer_path) {
		stderr.printf("ObexClientAgent: request start\n");
		
		try {
			transfer = Bus.get_proxy_sync (BusType.SESSION, "org.openobex.client", transfer_path);
		} catch (IOError e) {
			stderr.printf ("ObexClientAgent: Error on request: %s\n", e.message);
		}
		
		try {
			var hash = transfer.get_properties_();
			this.size = (uint64) hash.lookup("Size");
			this.name = (string) hash.lookup("Name");
			this.filename = (string) hash.lookup("Filename");
		} catch(IOError e) {
				stderr.printf ("ObexClientAgent: Error on get_properties: %s\n", e.message);
		}
		
		dialog = new TransferDialogUI();
		dialog.create(name, this.size);
		t = new Timer();
		t.reset();
		t.start();

		stderr.printf("ObexClientAgent: request end\n");
		//TODO: we may want to reject files...
		return "";
	}
	
	
	public void progress(ObjectPath transfer_path, uint64 bytes) {
		uint64 seconds = (uint64) t.elapsed();
		if(seconds==0) seconds++; //avoid dividing by 0 ;)
		uint64 speed = bytes / (seconds * 1000);
		
		/* stderr.printf( @"ObexClientAgent: progress ($bytes bytes transfered, $speed KB/s)\n"); */
		dialog.refresh(bytes/1000, speed);
	}
	
	public void complete(ObjectPath transfer_path) {
		stderr.printf("ObexClientAgent: complete\n");
		dialog.complete();
	}
	
	public void error(ObjectPath transfer_path, string message) {
		stderr.printf("ObexClientAgent: error -> %s\n", message);
	}
    	
}
