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
#include "dbus.h"
#include "cb_dbus.h"
#include "bluez.h"

static DBusMessage* dbus_emtooth_method_Up(E_DBus_Object *obj, DBusMessage *msg)
{
   return dbus_message_new_method_return(msg);
}

static bool dbus_emtooth_is_running() {  
	DBusMessage* msg;
	DBusMessage*reply;
	DBusConnection* conn;
	
	DBusError* err = (DBusError*) malloc(sizeof(DBusError));
	dbus_error_init(err);
	
	conn = dbus_bus_get(DBUS_BUS_SESSION, err);
	if (err && dbus_error_is_set(err)) {
		fprintf(stderr, "Error1: %s - %s\n", err->name, err->message);
		dbus_error_free(err);
		return FALSE;
	}
	
	
	msg = dbus_message_new_method_call(
		"org.emtooth.services",
		"/",
		"org.emtooth",
		"Up");
	
	reply = dbus_connection_send_with_reply_and_block (
	conn, 
	msg, 
	-1, 
	err);
	
	dbus_error_free(err);
	
	return (bool) reply;
}

void dbus_init_session() {
	
	/* First we get dbus sessions (system->bluez, session->obex) */
	e_dbus_object_init();
	DBUSCONN->sysconn = e_dbus_bus_get(DBUS_BUS_SYSTEM); 
	DBUSCONN->sessionconn = e_dbus_bus_get(DBUS_BUS_SESSION);
	
	if(dbus_emtooth_is_running()) {
		fprintf(stderr, "Emtooth is already running, exiting.\n");
		 exit(0); 
	} else fprintf(stderr, "Setting up org.emtooth.Up\n");
	
	e_dbus_request_name(DBUSCONN->sessionconn, "org.emtooth.services",
					     0, cb_dbus_generic, NULL);
	e_dbus_request_name(DBUSCONN->sysconn, "org.emtooth.services",
					     0, cb_dbus_generic, NULL);				     	     
	/* Implement service to know if there's already an emtooth instance running */
	E_DBus_Object* obj;
	E_DBus_Interface* iface;
	
	obj = e_dbus_object_add(DBUSCONN->sessionconn, "/", NULL);
	iface = e_dbus_interface_new("org.emtooth");
	if(!iface || !obj) {
		fprintf(stderr, "ERROR: org.emtooth creation: iface or obj == NULL\n");
		return;
	}
	
	e_dbus_object_interface_attach(obj, iface);
	e_dbus_interface_method_add(iface, "Up",
	 "", "", dbus_emtooth_method_Up);
			     
					     
	//run intermediate pass to request bluetooth resource
	fso_enable_bluetooth();

		
}


void dbus_dict_pair_debug(StructDbus* ret) {
	
	fprintf(stderr, "DICTIONARY STRUCT READ: %c\n", ret->value_type);
	if(ret->value_type==DBUS_TYPE_STRING || ret->value_type == DBUS_TYPE_OBJECT_PATH) {
		fprintf(stderr, "string: '%s'\n", ret->value.value_string);
		
	} else if(ret->value_type==DBUS_TYPE_UINT32 || ret->value_type == DBUS_TYPE_BOOLEAN) {
		fprintf(stderr, "integer: '%d'\n", ret->value.value_int);
		
	} else fprintf(stderr, "UNKNOWN VALUE!\n");
}



void dbus_message_get_variant(DBusMessageIter* iter, StructDbus* ret) {
	
	DBusMessageIter value_iter;
	
	dbus_message_iter_recurse (iter, &value_iter);
        
    int tmp = dbus_message_iter_get_arg_type(&value_iter);
    ret->value_type = tmp;
        
    if(ret->value_type == DBUS_TYPE_STRING || ret->value_type == DBUS_TYPE_OBJECT_PATH) {
		dbus_message_iter_get_basic(&value_iter, &ret->value.value_string);
		ret->value.value_string = strdup(ret->value.value_string);
	}
	else if(ret->value_type == DBUS_TYPE_UINT32 || ret->value_type == DBUS_TYPE_BOOLEAN) {
		dbus_message_iter_get_basic(&value_iter, &tmp);
		ret->value.value_int = tmp;
	} /*else { here comes array types, no need to handle them
			 fprintf(stderr, "\n\nUNKNOWN VALUE RECEIVED fROM DBUS: int:%d; char:%c;\n\n", ret->value_type, ret->value_type);
		} */
	
}


StructDbus* dbus_message_iter_get_dict_pair(DBusMessageIter *key_iter) {
	
	StructDbus* ret = malloc(sizeof(StructDbus));
	ret->value.value_int=0;
	ret->value.value_string=NULL;
	int tmp = DBUS_TYPE_INVALID;
	ret->value_type = DBUS_TYPE_INVALID; 
		
	dbus_message_iter_get_basic(key_iter, &ret->key);
	ret->key = strdup(ret->key);
	
	//set value_iter to point to next value:
    dbus_message_iter_next (key_iter);
        
    dbus_message_get_variant(key_iter, ret);
        
    return ret;
        
}


StructDbus* dbus_message_get_dict_pair(DBusMessage *msg) {
	
	DBusMessageIter key_iter;
	
	dbus_message_iter_init(msg, &key_iter);
	
	return dbus_message_iter_get_dict_pair(&key_iter);
}


StructDbus* dbus_get_next_struct_in_dict(DBusMessageIter *dict_iter) {
		DBusMessageIter key_iter;
		
		dbus_message_iter_recurse(dict_iter, &key_iter);
	
		return dbus_message_iter_get_dict_pair(&key_iter);
}


void dbus_append_variant(DBusMessageIter* iter, int value_type, DbusReturn value) {
	
	DBusMessageIter sub;
	
	char* buf = malloc(64);
	sprintf(buf, "%c", value_type);
	
	dbus_message_iter_open_container(iter,
		DBUS_TYPE_VARIANT,
		buf,
		&sub);
	
	switch (value_type){
	
	case DBUS_TYPE_BOOLEAN:
	case DBUS_TYPE_UINT32:		
		dbus_message_iter_append_basic(&sub, value_type, &value.value_int); 	
		break;
	case DBUS_TYPE_STRING:
	case DBUS_TYPE_OBJECT_PATH:
		dbus_message_iter_append_basic(&sub, value_type, &value.value_string);
		break;		
		
	}
	
	dbus_message_iter_close_container(iter, &sub);
	
}

void dbus_append_pair_to_dict(DBusMessageIter* iter, char* key, int value_type, DbusReturn value) {

	dbus_message_iter_append_basic(iter, DBUS_TYPE_STRING, &key);
	
	char* buf = malloc(64);
	sprintf(buf, "%c", value_type);
	
	dbus_append_variant(iter, value_type, value);

	
}

