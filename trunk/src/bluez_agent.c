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

#include <stdio.h>
#include <E_DBus.h>
#include "cb_dbus.h"
#include "bluez_agent.h"


static DBusHandlerResult bluez_agent_message(DBusConnection *conn, DBusMessage *msg, void *data) {
/*	if (dbus_message_is_method_call(msg, "org.bluez.Agent",
							"RequestPinCode"))
		return request_pincode_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent",
							"RequestPasskey"))
		return request_passkey_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent",
							"RequestConfirmation"))
		return request_confirmation_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent", "Authorize"))
		return authorize_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent", "Cancel"))
		return cancel_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent", "Release"))
		return release_message(conn, msg, data);*/

	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

/*
static const DBusObjectPathVTable agent_table = {
	.message_function = bluez_agent_message,
}; */


void bluez_agent_create_object_path(const char* agent_path) {
	
	/* return dbus_connection_register_object_path(DBUSCONN->sysconn, agent_path,
							&agent_table, NULL); */

	e_dbus_object_add(DBUSCONN->sysconn, 
						"/org/bluez/test_agent", 
						NULL);
}

void bluez_agent_register(const char *agent_path, const char *capabilities) {
	
	fprintf(stderr, "Registering bluez agent...\n");
	
	DBusMessage *msg;

	dbus_message_new_method_call(
	"org.bluez", 
	ADAPTER->path,
	"org.bluez.Adapter", 
	"RegisterAgent");

	dbus_message_append_args(msg, DBUS_TYPE_OBJECT_PATH, &agent_path,
					DBUS_TYPE_STRING, &capabilities,
					DBUS_TYPE_INVALID);

	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_dbus_generic, -1, NULL);

	dbus_message_unref(msg);

}
