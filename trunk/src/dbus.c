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

void dbus_init_session(DeviceList* data) {
	
	/* First we get device dbus interface */
	DBUSCONN = e_dbus_bus_get(DBUS_BUS_SYSTEM); 
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		"/",
		"org.bluez.Manager",
		"DefaultAdapter");
	e_dbus_message_send(DBUSCONN, msg, cb_get_dbus_path, -1, data);
	dbus_message_unref(msg);
		
}

void dbus_update_local_device_info(DeviceList* data) {
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		BLUEZPATH,
		"org.bluez.Adapter",
		"GetProperties");
	e_dbus_message_send(DBUSCONN, msg, cb_update_local_device_info, -1, data);
	dbus_message_unref(msg);
	
}

void dbus_start_discovery(DeviceList* data) {
	
/* now connect to signals and send StartDiscovery message */

    e_dbus_signal_handler_add(DBUSCONN,
	"org.bluez",
	BLUEZPATH,
	"org.bluez.Adapter",
	"DeviceFound",
	cb_device_found,
	data);
	
	e_dbus_signal_handler_add(DBUSCONN,
	"org.bluez", 
	BLUEZPATH,
	"org.bluez.Adapter",
	"DeviceDisappeared",
	cb_device_disappeared,
	data);

	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		BLUEZPATH,
		"org.bluez.Adapter",
		"StartDiscovery");
	
	e_dbus_message_send(DBUSCONN, msg, cb_start_discovery, -1, NULL);
	dbus_message_unref(msg);

   //e_dbus_connection_close(conn); 
}
