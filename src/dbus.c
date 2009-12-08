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
#include "dbus.h"

void dbus_init_session(DeviceList* DL) {

	/* First we get device dbus interface */
	DBUSCONN->conn = e_dbus_bus_get(DBUS_BUS_SYSTEM); 
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		"/",
		"org.bluez.Manager",
		"DefaultAdapter");
	e_dbus_message_send(DBUSCONN->conn, msg, cb_dbus_init_session, -1, DL);
	dbus_message_unref(msg);
		
}


void dbus_get_remote_device_path(RemoteDevice* device) {

	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		DBUSCONN->path,
		"org.bluez.Adapter",
		"FindDevice");
			
	dbus_message_append_args (msg,
		DBUS_TYPE_STRING, &device->addr,
		DBUS_TYPE_INVALID);

	e_dbus_message_send(DBUSCONN->conn, msg, cb_get_remote_device_path, -1, device);
	dbus_message_unref(msg);
	
}


void dbus_create_remote_device_path(RemoteDevice* device) {

	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		DBUSCONN->path,
		"org.bluez.Adapter",
		"CreateDevice");
			
	dbus_message_append_args (msg,
		DBUS_TYPE_STRING, &device->addr,
		DBUS_TYPE_INVALID);

	e_dbus_message_send(DBUSCONN->conn, msg, cb_create_remote_device_path, -1, device);
	dbus_message_unref(msg);
	
}



void dbus_get_local_device_info() {

	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		DBUSCONN->path,
		"org.bluez.Adapter",
		"GetProperties");
	e_dbus_message_send(DBUSCONN->conn, msg, cb_get_local_device_info, -1, NULL);
	dbus_message_unref(msg);
	
}


void dbus_get_remote_device_info(RemoteDevice* device) {

	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		device->path,
		"org.bluez.Device",
		"GetProperties");
	e_dbus_message_send(DBUSCONN->conn, msg, cb_get_remote_device_info, -1, device);
	dbus_message_unref(msg);
	
}



void dbus_discovery_start(DeviceList* DL) {

/* now connect to signals and send StartDiscovery message */

	DBUSCONN->DeviceFound = e_dbus_signal_handler_add(
    DBUSCONN->conn,
	"org.bluez",
	DBUSCONN->path,
	"org.bluez.Adapter",
	"DeviceFound",
	cb_device_found,
	DL);
	
	DBUSCONN->DeviceDissapeared = e_dbus_signal_handler_add(
	DBUSCONN->conn,
	"org.bluez", 
	DBUSCONN->path,
	"org.bluez.Adapter",
	"DeviceDisappeared",
	cb_device_disappeared,
	DL);

	/* TODO: here free Eina devices list to start discovery from zero */
	DL->devices = eina_list_free (DL->devices);
	gui_device_list_clear(DL->li);
	
	/* send start message */
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		DBUSCONN->path,
		"org.bluez.Adapter",
		"StartDiscovery");
	
	e_dbus_message_send(DBUSCONN->conn, msg, cb_discovery_start_msg, -1, DL);
	dbus_message_unref(msg);

   //e_dbus_connection_close(conn); 
}


void dbus_discovery_stop(DeviceList* DL) {
	
	/* disconnect from discovery signals, as other apps can be discovering too */
	e_dbus_signal_handler_del(DBUSCONN->conn, DBUSCONN->DeviceFound);
	e_dbus_signal_handler_del(DBUSCONN->conn, DBUSCONN->DeviceDissapeared);
	
	/* send stop message */
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		DBUSCONN->path,
		"org.bluez.Adapter",
		"StopDiscovery");
	
	e_dbus_message_send(DBUSCONN->conn, msg, cb_discovery_stop_msg, -1, DL);
	dbus_message_unref(msg);
	
}
