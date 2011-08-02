
[DBus (name = "org.openobex.Agent")]
public class ObexAgent : Object {


	public string authorize(ObjectPath transfer_path, string bt_address, string name, 
													string type, int32 length, int32 time) {
		
		stderr.printf("ObexAgent: authorize\n");
		stderr.printf("ObexAgent: transfer_path: %s\n\tbt_address: %s\n\tname: %s\n\ttype: %s\n\tlength: %d\n\ttime: %d\n", transfer_path, bt_address, name, type, length, time);
		
		
		FileDialogReceiveUI fd = new FileDialogReceiveUI();
		
		fd.create();
		
		while(fd.path==null) {
				Ecore.MainLoop.iterate();
		}

		stderr.printf("file: %s\n", Path.build_filename(fd.path, name));
		
		//TODO: throw error when no file was given
		//if(fd.path=="") throw new IOError org.openobex.Error.Rejected

		
		if(FileUtils.test(fd.path, FileTest.IS_DIR))
			return Path.build_filename(fd.path, name);
		else
			return fd.path;
		
	}
	
	public void cancel() {
		stderr.printf("ObexAgent: cancel\n");		
	}
    	
}
