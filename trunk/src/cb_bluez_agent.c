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
#include <stdio.h>
#include "defines.h"


DBusMessage* bluez_agent_method_RequestPinCode(E_DBus_Object *obj, DBusMessage *msg) {
	
	DBusMessage* reply;
	
	 fprintf(stderr, "AgentCb: someone requested PIN!\n");

	//here we we'll call a popup to get the pin
	
	reply = dbus_message_new_method_return(msg);
	
	const char* tmp_pin = "1234";
	dbus_message_append_args(reply, 
					DBUS_TYPE_STRING, &tmp_pin,
					DBUS_TYPE_INVALID);
	 
	return reply;
}


DBusMessage* bluez_agent_method_RequestPassKey(E_DBus_Object *obj, DBusMessage *msg) {
	
	DBusMessage* reply;
	
	 fprintf(stderr, "AgentCb: someone requested PASSKEY!\n");

	//here we we'll call a popup to get the pin
	
	reply = dbus_message_new_method_return(msg);
	
	int tmp_pass =  1234;
	dbus_message_append_args(reply, 
					DBUS_TYPE_UINT32, &tmp_pass,
					DBUS_TYPE_INVALID);
	 
	return reply;
}


DBusMessage* bluez_agent_method_DisplayPasskey(E_DBus_Object *obj, DBusMessage *msg) {
	 
	 char* device;
	 int passkey;
	 int entered;
	 DBusError* err = (DBusError*) malloc(sizeof(DBusError));
	dbus_error_init(err);
	 dbus_message_get_args(msg, err,
						DBUS_TYPE_OBJECT_PATH, &device,
	                    DBUS_TYPE_UINT32, &passkey,
	                    DBUS_TYPE_UINT16, &entered,	                      
	                    DBUS_TYPE_INVALID);
	DBUSLOG(err);
	dbus_error_free(err);
	
	fprintf(stderr, "AgentCb: here we Display Passkey: device:%s; passkey:%d; entered:%d;\n", device, passkey, entered); 
	 
	return dbus_message_new_method_return(msg);
}

DBusMessage* bluez_agent_method_RequestConfirmation(E_DBus_Object *obj, DBusMessage *msg) {
	 
	 char* device;
	 int passkey;
	 int entered;
	 DBusError* err = (DBusError*) malloc(sizeof(DBusError));
	dbus_error_init(err);
	 dbus_message_get_args(msg, err,
						DBUS_TYPE_OBJECT_PATH, &device,
	                    DBUS_TYPE_UINT32, &passkey,                    
	                    DBUS_TYPE_INVALID);
	DBUSLOG(err);
	dbus_error_free(err);
	
	fprintf(stderr, "AgentCb: here Confirmation is Requested: device:%s; passkey:%d;\n", device, passkey); 
	 
	return dbus_message_new_method_return(msg);
}


DBusMessage* bluez_agent_method_Authorize(E_DBus_Object *obj, DBusMessage *msg) {
	 
	 char* device;
	 char* uuid;
	 DBusError* err = (DBusError*) malloc(sizeof(DBusError));
	dbus_error_init(err);
	
	 dbus_message_get_args(msg, err,
						DBUS_TYPE_OBJECT_PATH, &device,
	                    DBUS_TYPE_STRING, &uuid,                    
	                    DBUS_TYPE_INVALID);
	DBUSLOG(err);
	dbus_error_free(err);
	
	fprintf(stderr, "AgentCb: Authorization (still don't know what's this for :P): device:%s; uuid:%s;\n", device, uuid); 
	 
	return dbus_message_new_method_return(msg);
}


DBusMessage* bluez_agent_method_ConfirmModeChange(E_DBus_Object *obj, DBusMessage *msg) {
	 
	 char* mode;
	 DBusError* err = (DBusError*) malloc(sizeof(DBusError));
	dbus_error_init(err);
	
	 dbus_message_get_args(msg, err,
	                    DBUS_TYPE_STRING, &mode,                    
	                    DBUS_TYPE_INVALID);
	DBUSLOG(err);
	dbus_error_free(err);
	
	fprintf(stderr, "AgentCb: ConfirmModeChange: mode:%s;\n", mode); 
	 
	return dbus_message_new_method_return(msg);
}

DBusMessage* bluez_agent_method_Cancel(E_DBus_Object *obj, DBusMessage *msg) {
	
	 fprintf(stderr, "AgentCb: Cancel()!\n");
	 
	return dbus_message_new_method_return(msg);
}


DBusMessage* bluez_agent_method_Release(E_DBus_Object *obj, DBusMessage *msg) {
	
	 fprintf(stderr, "AgentCb: someone released the agent!\n");
	 
	return dbus_message_new_method_return(msg);
}
