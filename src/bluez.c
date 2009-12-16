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
#include "bluez.h"

void bluez_init_session() {
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		"/",
		"org.bluez.Manager",
		"DefaultAdapter");
	e_dbus_message_send(DBUSCONN->conn, msg, cb_bluez_init_session, -1, NULL);
	dbus_message_unref(msg);
		
}


void bluez_get_remote_device_path(RemoteDevice* device) {

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

void bluez_create_remote_device_path(RemoteDevice* device) {

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

void bluez_get_local_device_info() {

	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		DBUSCONN->path,
		"org.bluez.Adapter",
		"GetProperties");
	e_dbus_message_send(DBUSCONN->conn, msg, cb_get_local_device_info, -1, NULL);
	dbus_message_unref(msg);
	
}


void bluez_get_remote_device_info(RemoteDevice* device) {

	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		device->path,
		"org.bluez.Device",
		"GetProperties");
	e_dbus_message_send(DBUSCONN->conn, msg, cb_get_remote_device_info, -1, device);
	dbus_message_unref(msg);
	
}



void bluez_discovery_start() {

/* now connect to signals and send StartDiscovery message */

	DBUSCONN->DeviceFound = e_dbus_signal_handler_add(
    DBUSCONN->conn,
	"org.bluez",
	DBUSCONN->path,
	"org.bluez.Adapter",
	"DeviceFound",
	cb_device_found,
	NULL);
	
	DBUSCONN->DeviceDissapeared = e_dbus_signal_handler_add(
	DBUSCONN->conn,
	"org.bluez", 
	DBUSCONN->path,
	"org.bluez.Adapter",
	"DeviceDisappeared",
	cb_device_disappeared,
	NULL);
	
	/* send start message */
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		DBUSCONN->path,
		"org.bluez.Adapter",
		"StartDiscovery");
	
	e_dbus_message_send(DBUSCONN->conn, msg, cb_discovery_start_msg, -1, NULL);
	dbus_message_unref(msg);

   //e_dbus_connection_close(conn); 
}


void bluez_discovery_stop() {
	
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
	
	e_dbus_message_send(DBUSCONN->conn, msg, cb_discovery_stop_msg, -1, NULL);
	dbus_message_unref(msg);
	
}
