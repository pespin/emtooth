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
Foundation, Inc., 51 Franklin Stre et, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <E_DBus.h>
#include "defines.h"


/* callbacks */

void cb_fso_enable_bluetooth(void *data, DBusMessage *replymsg, DBusError *error) {
	
	if (dbus_error_is_set(error)) 
		fprintf(stderr, "Error: %s - %s\n", error->name, error->message);

	fprintf(stderr, "Request sent to frameworkd to enable bluetooth resource.\n");
	
	bluez_init_session();
	
}

void cb_fso_disable_bluetooth(void *data, DBusMessage *replymsg, DBusError *error) {
	
	if (dbus_error_is_set(error)) 
		fprintf(stderr, "Error: %s - %s\n", error->name, error->message);

	fprintf(stderr, "Request sent to frameworkd to disable bluetooth resource.\n");
	
}


/* functions */

void fso_enable_bluetooth() {
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.freesmartphone.ousaged",
		"/org/freesmartphone/Usage",
		"org.freesmartphone.Usage",
		"RequestResource");
	
	char* str = strdup("Bluetooth");
	dbus_message_append_args (msg,
		DBUS_TYPE_STRING, &str,
		DBUS_TYPE_INVALID);
		
	e_dbus_message_send(DBUSCONN->conn, msg, cb_fso_enable_bluetooth, -1, NULL);
	dbus_message_unref(msg);
	free(str);
		
}


void fso_disable_bluetooth() {
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.freesmartphone.ousaged",
		"/org/freesmartphone/Usage",
		"org.freesmartphone.Usage",
		"ReleaseResource");
		
	char* str = strdup("Bluetooth");
	dbus_message_append_args (msg,
		DBUS_TYPE_STRING, &str,
		DBUS_TYPE_INVALID);
		
	e_dbus_message_send(DBUSCONN->conn, msg, cb_fso_disable_bluetooth, -1, NULL);
	dbus_message_unref(msg);
	free(str);
		
}
