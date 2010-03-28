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
	
	fprintf(stderr, "Func remote_device_new() called on device [%s]\n", addr);
	
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
		device->signal_PropertyChanged_device = NULL;
		
		//org.bluez.Input iface:
		device->connected_input = 0;
		device->signal_PropertyChanged_input = NULL;
		
		//org.bluez.Audio iface:
		device->connected_audio = 0;
		device->signal_PropertyChanged_audio = NULL;
				
		//internal:
		device->password = NULL; //set to null, used for pairing agent later.
		device->valid = FALSE; //set to true when we have minimal info
		
	return device;
}



bool remote_device_free(RemoteDevice* device) {
	
	if(!device) return FALSE;
	
	if(device->path) 		free(device->path);
	if(device->addr) 		free(device->addr);
	if(device->name) 		free(device->name);
	if(device->icon) 		free(device->icon);
	if(device->alias) 		free(device->alias);
	if(device->password)	free(device->password);
	
	array_free(device->UUIDs);
	
	if(device->signal_PropertyChanged_device)
		e_dbus_signal_handler_del(DBUSCONN->sysconn, device->signal_PropertyChanged_device);
	if(device->signal_PropertyChanged_input)
		e_dbus_signal_handler_del(DBUSCONN->sysconn, device->signal_PropertyChanged_input);
	if(device->signal_PropertyChanged_audio)
		e_dbus_signal_handler_del(DBUSCONN->sysconn, device->signal_PropertyChanged_audio);
	
	return TRUE;	
}


bool dict_entry_free(DictEntry* ret) {
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

