/* Generated by vala-dbus-binding-tool 0.3.2. Do not modify! */
/* Generated with: vala-dbus-binding-tool --directory=. */
using DBus;
using GLib;

namespace org {

	namespace bluez {

		/*
		 * 
		 * /org/bluez/XXXX/hci0
		 * 
		 */


		[DBus (name = "org.bluez.Adapter", timeout = 120000)]
		public interface Adapter : GLib.Object {

			public abstract GLib.HashTable<string, GLib.Value?> get_properties() throws DBus.Error;

			[DBus (name = "SetProperty")] public abstract void set_property_(string param0, GLib.Value param1) throws DBus.Error;

			public abstract void request_session() throws DBus.Error;

			public abstract void release_session() throws DBus.Error;

			public abstract void start_discovery() throws DBus.Error;

			public abstract void stop_discovery() throws DBus.Error;

			public abstract DBus.ObjectPath[] list_devices() throws DBus.Error;

			public abstract DBus.ObjectPath create_device(string param0) throws DBus.Error;

			public abstract DBus.ObjectPath create_paired_device(string param0, DBus.ObjectPath param1, string param2) throws DBus.Error;

			public abstract void cancel_device_creation(string param0) throws DBus.Error;

			public abstract void remove_device(DBus.ObjectPath param0) throws DBus.Error;

			public abstract DBus.ObjectPath find_device(string param0) throws DBus.Error;

			public abstract void register_agent(DBus.ObjectPath param0, string param1) throws DBus.Error;

			public abstract void unregister_agent(DBus.ObjectPath param0) throws DBus.Error;

			public signal void property_changed(string param0, GLib.Value param1);

			public signal void device_created(DBus.ObjectPath param0);

			public signal void device_removed(DBus.ObjectPath param0);

			public signal void device_found(string param0, GLib.HashTable<string, GLib.Value?> param1);

			public signal void device_disappeared(string param0);
		}

		public Adapter get_adapter_proxy(DBus.Connection con, string busname, DBus.ObjectPath path) {
			return con.get_object(busname, path) as Adapter;
		}
		[DBus (name = "org.bluez.Adapter", timeout = 120000)]
		public interface AdapterSync : GLib.Object {

			public abstract GLib.HashTable<string, GLib.Value?> get_properties() throws DBus.Error;

			[DBus (name = "SetProperty")] public abstract void set_property_(string param0, GLib.Value param1) throws DBus.Error;

			public abstract void request_session() throws DBus.Error;

			public abstract void release_session() throws DBus.Error;

			public abstract void start_discovery() throws DBus.Error;

			public abstract void stop_discovery() throws DBus.Error;

			public abstract DBus.ObjectPath[] list_devices() throws DBus.Error;

			public abstract DBus.ObjectPath create_device(string param0) throws DBus.Error;

			public abstract DBus.ObjectPath create_paired_device(string param0, DBus.ObjectPath param1, string param2) throws DBus.Error;

			public abstract void cancel_device_creation(string param0) throws DBus.Error;

			public abstract void remove_device(DBus.ObjectPath param0) throws DBus.Error;

			public abstract DBus.ObjectPath find_device(string param0) throws DBus.Error;

			public abstract void register_agent(DBus.ObjectPath param0, string param1) throws DBus.Error;

			public abstract void unregister_agent(DBus.ObjectPath param0) throws DBus.Error;

			public signal void property_changed(string param0, GLib.Value param1);

			public signal void device_created(DBus.ObjectPath param0);

			public signal void device_removed(DBus.ObjectPath param0);

			public signal void device_found(string param0, GLib.HashTable<string, GLib.Value?> param1);

			public signal void device_disappeared(string param0);
		}

		[DBus (name = "org.bluez.Service", timeout = 120000)]
		public interface Service : GLib.Object {

			public abstract uint add_record(string param0) throws DBus.Error;

			public abstract void update_record(uint param0, string param1) throws DBus.Error;

			public abstract void remove_record(uint param0) throws DBus.Error;

			public abstract void request_authorization(string param0, uint param1) throws DBus.Error;

			public abstract void cancel_authorization() throws DBus.Error;
		}

		public Service get_service_proxy(DBus.Connection con, string busname, DBus.ObjectPath path) {
			return con.get_object(busname, path) as Service;
		}
		[DBus (name = "org.bluez.Service", timeout = 120000)]
		public interface ServiceSync : GLib.Object {

			public abstract uint add_record(string param0) throws DBus.Error;

			public abstract void update_record(uint param0, string param1) throws DBus.Error;

			public abstract void remove_record(uint param0) throws DBus.Error;

			public abstract void request_authorization(string param0, uint param1) throws DBus.Error;

			public abstract void cancel_authorization() throws DBus.Error;
		}

		[DBus (name = "org.bluez.NetworkServer", timeout = 120000)]
		public interface NetworkServer : GLib.Object {

			public abstract void register_(string param0, string param1) throws DBus.Error;

			public abstract void unregister(string param0) throws DBus.Error;
		}

		public NetworkServer get_network_server_proxy(DBus.Connection con, string busname, DBus.ObjectPath path) {
			return con.get_object(busname, path) as NetworkServer;
		}
		[DBus (name = "org.bluez.NetworkServer", timeout = 120000)]
		public interface NetworkServerSync : GLib.Object {

			public abstract void register_(string param0, string param1) throws DBus.Error;

			public abstract void unregister(string param0) throws DBus.Error;
		}

		[DBus (name = "org.bluez.SerialProxyManager", timeout = 120000)]
		public interface SerialProxyManager : GLib.Object {

			public abstract string create_proxy(string param0, string param1) throws DBus.Error;

			public abstract string[] list_proxies() throws DBus.Error;

			public abstract void remove_proxy(string param0) throws DBus.Error;

			public signal void proxy_created(string param0);

			public signal void proxy_removed(string param0);
		}

		public SerialProxyManager get_serial_proxy_manager_proxy(DBus.Connection con, string busname, DBus.ObjectPath path) {
			return con.get_object(busname, path) as SerialProxyManager;
		}
		[DBus (name = "org.bluez.SerialProxyManager", timeout = 120000)]
		public interface SerialProxyManagerSync : GLib.Object {

			public abstract string create_proxy(string param0, string param1) throws DBus.Error;

			public abstract string[] list_proxies() throws DBus.Error;

			public abstract void remove_proxy(string param0) throws DBus.Error;

			public signal void proxy_created(string param0);

			public signal void proxy_removed(string param0);
		}
		
		/*
		 * 
		 * 
		 *  /
		 * 
		 */ 

		[DBus (name = "org.bluez.Manager", timeout = 120000)]
		public interface Manager : GLib.Object {

			public abstract GLib.HashTable<string, GLib.Value?> get_properties() throws DBus.Error;

			public abstract DBus.ObjectPath default_adapter() throws DBus.Error;

			public abstract DBus.ObjectPath find_adapter(string param0) throws DBus.Error;

			public abstract DBus.ObjectPath[] list_adapters() throws DBus.Error;

			public signal void property_changed(string param0, GLib.Value param1);

			public signal void adapter_added(DBus.ObjectPath param0);

			public signal void adapter_removed(DBus.ObjectPath param0);

			public signal void default_adapter_changed(DBus.ObjectPath param0);
		}

		public Manager get_manager_proxy(DBus.Connection con, string busname, DBus.ObjectPath path) {
			return con.get_object(busname, path) as Manager;
		}
		[DBus (name = "org.bluez.Manager", timeout = 120000)]
		public interface ManagerSync : GLib.Object {

			public abstract GLib.HashTable<string, GLib.Value?> get_properties() throws DBus.Error;

			public abstract DBus.ObjectPath default_adapter() throws DBus.Error;

			public abstract DBus.ObjectPath find_adapter(string param0) throws DBus.Error;

			public abstract DBus.ObjectPath[] list_adapters() throws DBus.Error;

			public signal void property_changed(string param0, GLib.Value param1);

			public signal void adapter_added(DBus.ObjectPath param0);

			public signal void adapter_removed(DBus.ObjectPath param0);

			public signal void default_adapter_changed(DBus.ObjectPath param0);
		}
		
		
		/*
		 * 
		 * /org/bluez/XXXX/hci0/dev_00_19_2D_11_62_09
		 * 
		 */
		
		[DBus (name = "org.bluez.Device", timeout = 120000)]
		public interface Device : GLib.Object {

			public abstract GLib.HashTable<string, GLib.Value?> get_properties() throws DBus.Error;
			
			[DBus (name = "SetProperty")]
			public abstract void set_property_(string param0, GLib.Value param1) throws DBus.Error;

			public abstract GLib.HashTable<uint, string> discover_services(string param0) throws DBus.Error;

			public abstract void cancel_discovery() throws DBus.Error;

			public abstract void disconnect() throws DBus.Error;

			public signal void property_changed(string param0, GLib.Value param1);

			public signal void disconnect_requested();
		}

		public Device get_device_proxy(DBus.Connection con, string busname, DBus.ObjectPath path) {
			return con.get_object(busname, path) as Device;
		}
		[DBus (name = "org.bluez.Device", timeout = 120000)]
		public interface DeviceSync : GLib.Object {

			public abstract GLib.HashTable<string, GLib.Value?> get_properties() throws DBus.Error;

			[DBus (name = "SetProperty")]
			public abstract void set_property_(string param0, GLib.Value param1) throws DBus.Error;

			public abstract GLib.HashTable<uint, string> discover_services(string param0) throws DBus.Error;

			public abstract void cancel_discovery() throws DBus.Error;

			public abstract void disconnect() throws DBus.Error;

			public signal void property_changed(string param0, GLib.Value param1);

			public signal void disconnect_requested();
		}

		[DBus (name = "org.bluez.Audio", timeout = 120000)]
		public interface Audio : GLib.Object {

			public abstract void connect() throws DBus.Error;

			public abstract void disconnect() throws DBus.Error;

			public abstract GLib.HashTable<string, GLib.Value?> get_properties() throws DBus.Error;

			public signal void property_changed(string param0, GLib.Value param1);
		}

		public Audio get_audio_proxy(DBus.Connection con, string busname, DBus.ObjectPath path) {
			return con.get_object(busname, path) as Audio;
		}
		[DBus (name = "org.bluez.Audio", timeout = 120000)]
		public interface AudioSync : GLib.Object {

			public abstract void connect() throws DBus.Error;

			public abstract void disconnect() throws DBus.Error;

			public abstract GLib.HashTable<string, GLib.Value?> get_properties() throws DBus.Error;

			public signal void property_changed(string param0, GLib.Value param1);
		}

		[DBus (name = "org.bluez.Serial", timeout = 120000)]
		public interface Serial : GLib.Object {

			public abstract string connect(string param0) throws DBus.Error;

			public abstract void disconnect(string param0) throws DBus.Error;
		}

		public Serial get_serial_proxy(DBus.Connection con, string busname, DBus.ObjectPath path) {
			return con.get_object(busname, path) as Serial;
		}
		[DBus (name = "org.bluez.Serial", timeout = 120000)]
		public interface SerialSync : GLib.Object {

			public abstract string connect(string param0) throws DBus.Error;

			public abstract void disconnect(string param0) throws DBus.Error;
		}
		
	}
}