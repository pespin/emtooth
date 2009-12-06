/*
emtooth, A bluetooth/bluez manager written in elementary
Copyright (C) 2009  Pau Espin Pedrol (Sharwin_F) <pespin.shar@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <E_DBus.h>
#include "gui.h"
#include "cb_dbus.h"

void dbus_init_session(void* data) {
	
	
	E_DBus_Connection* conn;
    conn = e_dbus_bus_get(DBUS_BUS_SYSTEM); 
      
    e_dbus_signal_handler_add(
	conn,
	"org.bluez", //const char *sender 
	"/org/bluez/1486/hci0", // const char *path 
	"org.bluez.Adapter", // const char *interface
	"DeviceFound",
	cb_device_found, //E_DBus_Signal_Cb cb_signal
	data);
	
	/*
	    e_dbus_signal_handler_add(
	conn,
	"org.bluez", //const char *sender 
	"/org/bluez/1486/hci0", // const char *path 
	"org.bluez.Adapter", // const char *interface
	"DeviceCreated",
	cb_device_found, //E_DBus_Signal_Cb cb_signal
	data);
	*/
	
		    e_dbus_signal_handler_add(
	conn,
	"org.bluez", //const char *sender 
	"/org/bluez/1486/hci0", // const char *path 
	"org.bluez.Adapter", // const char *interface
	"DeviceDisappeared",
	cb_device_disappeared, //E_DBus_Signal_Cb cb_signal
	data);

	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		"/org/bluez/1486/hci0",
		"org.bluez.Adapter",
		"StartDiscovery");
	
	e_dbus_message_send(conn, msg, cb_start_discovery, -1, NULL);
	dbus_message_unref(msg);

   //e_dbus_connection_close(conn); 	
}










/*
 	
	
	
	/*	
     dbus_g_proxy_add_signal(obj, "RemoteDeviceFound", G_TYPE_STRING, G_TYPE_UINT, G_TYPE_INT, G_TYPE_INVALID);
        dbus_g_proxy_connect_signal(obj, "RemoteDeviceFound", G_CALLBACK(remote_device_found), bus, NULL);

dbus_g_proxy_new_for_name(bus, "org.bluez", "/org/bluez/hci0", "org.bluez.Adapter");

		dbus_g_proxy_new_for_name           (DBusGConnection *connection,
                                                         const char *name,
                                                         const char *path,
                                                         const char *interface);

		
		
	e_dbus_signal_handler_add(
	conn,
	"org.bluez", //const char *sender 
	"/org/bluez/1486/hci0", // const char *path 
	"org.bluez.Adapter", // const char *interface
	const char *member,
	E_DBus_Signal_Cb cb_signal,
	void *data);
   	
	
    
   
   E_DBus_Object* obj;
   obj = e_dbus_object_add(conn, "/org/bluez/1486/hci0", NULL);
   

   E_DBus_Interface* iface;
   iface = e_dbus_interface_new("/org/bluez/1486/hci0");
   
   e_dbus_object_interface_attach(obj, iface);
 
   Eina_List* li = e_dbus_object_interfaces_get(obj);
   
   Eina_List* cur;
   char* row;
   EINA_LIST_FOREACH(li, cur, row) {
	 
	 fprintf(stderr, "%s\n", row);
	   
	}
	
	
	   */
   
