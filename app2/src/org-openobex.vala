/* Generated by vala-dbus-binding-tool 0.3.3. Do not modify! */
/* Generated with: vala-dbus-binding-tool --directory=. */
using GLib;

namespace org {

	namespace openobex {

		[DBus (name = "org.openobex.Manager", timeout = 120000)]
		public interface ObexDBusManager : GLib.Object {

			public abstract void register_agent(GLib.ObjectPath param0) throws IOError;

			public abstract void unregister_agent(GLib.ObjectPath param0) throws IOError;

			public signal void transfer_started(GLib.ObjectPath param0);

			public signal void transfer_completed(GLib.ObjectPath param0, bool param1);

			public signal void session_created(GLib.ObjectPath param0);

			public signal void session_removed(GLib.ObjectPath param0);
		}
	
	}
	
		[DBus (name = "org.openobex.Transfer", timeout = 120000)]
		public interface ObexDBusTransfer : GLib.Object {

			public abstract void cancel() throws IOError;

			public signal void progress(int32 total, int32 transfered);
		}

}
