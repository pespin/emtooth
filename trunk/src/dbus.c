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
#include "bluez.h"

void dbus_init_session() {
	
	/* First we get device dbus interface */
	DBUSCONN->conn = e_dbus_bus_get(DBUS_BUS_SYSTEM); 

	//run intermediate pass to request bluetooth resource
	fso_enable_bluetooth();

		
}

StructDbus* dbus_get_next_struct_in_dict(DBusMessageIter *dict_iter) {
		DBusMessageIter key_iter, value_iter;
	
		StructDbus* ret = malloc(sizeof(StructDbus));
		ret->value.value_int=0;
		ret->value.value_string=NULL;
		int tmp = DBUS_TYPE_INVALID;
		ret->value_type = DBUS_TYPE_INVALID; 
		
	    //get key:
        dbus_message_iter_recurse(dict_iter, &key_iter);
        dbus_message_iter_get_basic(&key_iter, &ret->key);
        ret->key = strdup(ret->key); //make copy
        
        //set value_iter to point to next value:
        dbus_message_iter_next (&key_iter);
        dbus_message_iter_recurse (&key_iter, &value_iter);
        
        tmp = dbus_message_iter_get_arg_type(&value_iter);
        ret->value_type = tmp;
        
        if(ret->value_type == DBUS_TYPE_STRING || ret->value_type == DBUS_TYPE_OBJECT_PATH) {
			dbus_message_iter_get_basic(&value_iter, &ret->value.value_string);
			ret->value.value_string = strdup(ret->value.value_string);
		}
		else  if(ret->value_type == DBUS_TYPE_UINT32 || ret->value_type == DBUS_TYPE_BOOLEAN) {
			dbus_message_iter_get_basic(&value_iter, &tmp);
			ret->value.value_int = tmp;
		} /*else { here comes array types, no need to handle them
			 fprintf(stderr, "\n\nUNKNOWN VALUE RECEIVED fROM DBUS: int:%d; char:%c;\n\n", ret->value_type, ret->value_type);
		} */
		
	return ret;
}
