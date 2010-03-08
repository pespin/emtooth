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
#include "cb_dbus.h"
#include "cb_obex.h"
#include "dbus.h"
#include "defines.h"
#include "obex.h"


void obex_register_agent() {
	
	fprintf(stderr, "Registering obex agent...\n");
	
	DBusMessage *msg;

	msg = dbus_message_new_method_call(
	"org.openobex", 
	"/",
	"org.openobex.Manager", 
	"RegisterAgent");

	char* tmp = strdup(OBEX_AGENT_PATH);
	dbus_message_append_args(msg, 
					DBUS_TYPE_OBJECT_PATH, &tmp,
					DBUS_TYPE_INVALID);
	free(tmp);			
				
	e_dbus_message_send(DBUSCONN->sessionconn, msg, cb_dbus_generic, -1, NULL);

	dbus_message_unref(msg);

}


void obex_manager_attach_signals() {
	
	e_dbus_signal_handler_add(
	DBUSCONN->sessionconn,
	"org.openobex", 
	"/",
	"org.openobex.Manager",
	"SessionCreated",
	cb_obex_manager_SessionCreated,
	NULL);
	
	e_dbus_signal_handler_add(
    DBUSCONN->sessionconn,
	"org.openobex",
	"/",
	"org.openobex.Manager",
	"SessionRemoved",
	cb_obex_manager_SessionRemoved,
	NULL);
	
	e_dbus_signal_handler_add(
	DBUSCONN->sessionconn,
	"org.openobex", 
	"/",
	"org.openobex.Manager",
	"TransferStarted",
	cb_obex_manager_TransferStarted,
	NULL);
	
	e_dbus_signal_handler_add(
    DBUSCONN->sessionconn,
	"org.openobex",
	"/",
	"org.openobex.Manager",
	"TransferCompleted",
	cb_obex_manager_TransferCompleted,
	NULL);
		
}


/* client */

void obex_client_SendFiles(RemoteDevice* device, const char** files_array) {
	
	DBusMessageIter iter, sub;
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.openobex.client",
		"/",
		"org.openobex.Client",
		"SendFiles");
	
	//start with args:
	dbus_message_iter_init_append(msg, &iter); 	
	
	DbusReturn ret;
	ret.value_string = device->addr;
	dbus_append_pair_to_dict(&iter, "Destination", DBUS_TYPE_STRING, ret);
	
	//TODO: implement this:
	dbus_message_iter_append_array(&iter, files_array);
	fprintf(stderr, "FINISHED appending array\n");
	
	char* tmp = strdup(OBEX_AGENT_PATH);
	dbus_message_append_args (msg,
	DBUS_TYPE_STRING, &tmp,
	DBUS_TYPE_INVALID);
	free(tmp);
	
		fprintf(stderr, "FINISHED appending all args\n");

	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_dbus_generic, -1, NULL);
	dbus_message_unref(msg);
	
}
