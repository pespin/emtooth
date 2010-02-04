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
#include "defines.h"


GuiCb* init_cb_struct(const char* property, const char* path, const char* iface) {
	
		GuiCb* cb = malloc(sizeof(GuiCb));
		
		cb->property = strdup(property);
		
		if(!path || path==ADAPTER->path) {
			cb->path = ADAPTER->path;
			if(!iface) cb->interface = strdup("org.bluez.Adapter");
			else cb->interface = strdup(iface);
		} else {
			cb->path = strdup(path);
			cb->interface = strdup(iface);
		}
		
		return cb;
}

RemoteDevice* remote_device_new(const char* addr) {
	
	RemoteDevice* device = malloc(sizeof(RemoteDevice));
		//org.bluez.Device iface:
		device->path = NULL;
		device->addr = strdup(addr);
		device->name = NULL;
		device->class = 0;
		device->connected_device = 0;
		device->icon = NULL;
		device->alias = NULL;
		device->paired = 0;
		device->trusted = 0;
		device->UUIDs = NULL;
		
		//org.bluez.Input iface:
		device->connected_input = 0;
		
		//org.bluez.Audio iface:
		device->connected_audio = 0;
		
		//internal:
		device->password = NULL; //set to null, used for pairing agent later.
		
	return device;
}



bool struct_dbus_free(StructDbus* ret) {
	if(!ret) return FALSE;
	
	free(ret->key);
	
	if(ret->value_type==DBUS_TYPE_STRING || ret->value_type == DBUS_TYPE_OBJECT_PATH) {
		if(ret->value.value_string) free(ret->value.value_string);
	}
	free(ret);
	
	return TRUE;
}

bool array_free(char** array) {
	
	if(!array) return FALSE;
	int i = 0;
	while(array[i]) {
		free(array[i]);
		i++;
	}
	free(array);
	
	return TRUE;
}

