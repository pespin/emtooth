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
#include "gui.h"
#include "cb_dbus.h"


void cb_dbus_init_session(void *data, DBusMessage *replymsg, DBusError *error) {
	
	char *path = NULL;
	
	dbus_message_get_args(replymsg, error,
	                      DBUS_TYPE_OBJECT_PATH, &path, DBUS_TYPE_INVALID);
	
	if (dbus_error_is_set(error)) {
		fprintf(stderr, "Error: %s - %s\n", error->name, error->message);
	}
	
	if(!path) {
		fprintf(stderr, "Could not get bluez dbus RemoteDevice path. EXIT NOW!\n");
		exit(EXIT_FAILURE);
	}
	
	DBUSCONN->path = strdup(path);
	fprintf(stderr, "Using path '%s' to connect to bluez dbus daemon...\n", DBUSCONN->path);
	
	//get local device info and start signals on that interface:
	dbus_get_local_device_info(data);
	dbus_discovery_start(data);
}




void cb_get_remote_device_path (void *data, DBusMessage *replymsg, DBusError *error) {

	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Getting remote device [%s] dbus path...\n", device->addr);
	
	if(!replymsg) {
		/* As no one has created a path to this device, we create it us: */
		fprintf(stderr, "Could not get a path to remote device [%s], creating it...\n", device->addr);
		dbus_create_remote_device_path(device);
		return;
	}
	
	char *path = NULL;
	
	dbus_message_get_args(replymsg, error,
	                      DBUS_TYPE_OBJECT_PATH, &path, DBUS_TYPE_INVALID);
	
	if (dbus_error_is_set(error)) {
		fprintf(stderr, "Error: %s - %s\n", error->name, error->message);
	}
	

	if(!path) { 
		fprintf(stderr, "Could not get a path to remote device [%s], probably to a dbus error... do nothing\n", device->addr);
		return;
	}
	
	device->path = strdup(path);
	fprintf(stderr, "Using path '%s' to connect to remote device [%s]...\n", device->path, device->addr);
	
	//get remote device info:
	dbus_get_remote_device_info(device);
}


void cb_create_remote_device_path (void *data, DBusMessage *replymsg, DBusError *error) {

	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Creating remote device [%s] dbus path...\n", device->addr);
	
	char *path = NULL;
	
	dbus_message_get_args(replymsg, error,
	                      DBUS_TYPE_OBJECT_PATH, &path, DBUS_TYPE_INVALID);
	
	if (dbus_error_is_set(error)) {
		printf("Error: %s - %s\n", error->name, error->message);
	}
	
	if(!path) {
		fprintf(stderr, "Could not create a path to remote device [%s]\n", device->addr);
		return;
	}
	
	device->path = strdup(path);
	fprintf(stderr, "Using path '%s' to connect to remote device [%s]...\n", device->path, device->addr);
	
	//get remote device info:
	dbus_get_remote_device_info(device);
}




void cb_get_local_device_info (void *data, DBusMessage *replymsg, DBusError *error) {
		
	fprintf(stderr, "Updating local device info... ");	
	
	if (dbus_error_is_set(error)) {
		printf("Error: %s - %s\n", error->name, error->message);
	}
		
	DBusMessageIter array_iter, dict_iter, key_iter, value_iter;
	int tmp = 0;

	//open dict:
	dbus_message_iter_init(replymsg, &array_iter);
	dbus_message_iter_recurse(&array_iter, &dict_iter);
	
	//foreach pair...:
	while (dbus_message_iter_get_arg_type (&dict_iter) != DBUS_TYPE_INVALID) {
		const char *key = NULL;
        
        //get key:
        dbus_message_iter_recurse(&dict_iter, &key_iter);
        dbus_message_iter_get_basic(&key_iter, &key);
        
        //set value_iter to point to next value:
        dbus_message_iter_next (&key_iter);
        dbus_message_iter_recurse (&key_iter, &value_iter);

        //Depending on key, save value where it should go:
		if(!strcmp(key,"Address")) {
			dbus_message_iter_get_basic(&value_iter, &ADAPTER->addr);
			ADAPTER->addr = strdup(ADAPTER->addr); //make a copy
					
		} else if(!strcmp(key,"Class")) {
			dbus_message_iter_get_basic(&value_iter, &tmp);
			ADAPTER->class = tmp;
			
		} else if(!strcmp(key,"Discoverable")) {
			dbus_message_iter_get_basic(&value_iter, &tmp);
			ADAPTER->discoverable = tmp;
			
		} else if(!strcmp(key,"DiscoverableTimeout")) {
			dbus_message_iter_get_basic(&value_iter, &tmp);
			ADAPTER->discoverable_timeout = tmp;
			
		} else if(!strcmp(key,"Discovering")) {
			dbus_message_iter_get_basic(&value_iter, &tmp);
			ADAPTER->discovering = tmp;
			
		} else if(!strcmp(key,"Name")) {
			dbus_message_iter_get_basic(&value_iter, &ADAPTER->name);
			ADAPTER->name = strdup(ADAPTER->name);
			
		} else if(!strcmp(key,"Pairable")) {
			dbus_message_iter_get_basic(&value_iter, &tmp);
			ADAPTER->pairable = tmp;
			
		} else if(!strcmp(key,"PairableTimeout")) {
			dbus_message_iter_get_basic(&value_iter, &tmp);
			ADAPTER->pairable_timeout = tmp;
			
		} else if(!strcmp(key,"Powered")) {
			dbus_message_iter_get_basic(&value_iter, &tmp);
			ADAPTER->powered = tmp;
		}
		
		dbus_message_iter_next (&dict_iter);
	}
	fprintf(stderr, "Done!\n");
}



void cb_get_remote_device_info (void *data, DBusMessage *replymsg, DBusError *error) {
	
	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Updating remote device [%s][%s] info... ", device->addr, device->path);
	
	if (dbus_error_is_set(error)) {
		fprintf(stderr, "Error: %s - %s\n", error->name, error->message);
	}
		
	DBusMessageIter array_iter, dict_iter, key_iter, value_iter;
	int tmp = 0;

	//open dict:
	dbus_message_iter_init(replymsg, &array_iter);
	dbus_message_iter_recurse(&array_iter, &dict_iter);
	
	//foreach pair...:
	while (dbus_message_iter_get_arg_type (&dict_iter) != DBUS_TYPE_INVALID) {
		const char *key = NULL;
        //get key:
        dbus_message_iter_recurse(&dict_iter, &key_iter);
        dbus_message_iter_get_basic(&key_iter, &key);
        
        //set value_iter to point to next value:
        dbus_message_iter_next (&key_iter);
        dbus_message_iter_recurse (&key_iter, &value_iter);

        //Depending on key, save value where it should go:
		if(!strcmp(key,"Alias")) {
			dbus_message_iter_get_basic(&value_iter, &device->alias);
			device->alias = strdup(device->alias); //make a copy
					
		} else if(!strcmp(key,"Class")) {
			dbus_message_iter_get_basic(&value_iter, &tmp);
			device->class = tmp;
			
		} else if(!strcmp(key,"Connected")) {
			dbus_message_iter_get_basic(&value_iter, &device->connected);
			device->connected = tmp;
			
		} else if(!strcmp(key,"Icon")) {
			dbus_message_iter_get_basic(&value_iter, &device->icon);
			device->icon = strdup(device->icon);
			
		} else if(!strcmp(key,"Name")) {
			dbus_message_iter_get_basic(&value_iter, &device->name);
			device->name = strdup(device->name);
			
		} else if(!strcmp(key,"Paired")) {
			dbus_message_iter_get_basic(&value_iter, &tmp);
			device->paired = tmp;
			
		} else if(!strcmp(key,"Trusted")) {
			dbus_message_iter_get_basic(&value_iter, &tmp);
			device->trusted = tmp;
		}
		
		dbus_message_iter_next (&dict_iter);
	}
	fprintf(stderr, "Done!\n");
	
}


void cb_discovery_start_msg(void *data, DBusMessage *replymsg, DBusError *error) {
	if (dbus_error_is_set(error)) {
		fprintf(stderr, "Error: %s - %s\n", error->name, error->message);
	} else {
	//set LocalDevice->discovering to True manually... lot faster :)
	ADAPTER->discovering = TRUE;	
	}
}


void cb_discovery_stop_msg(void *data, DBusMessage *replymsg, DBusError *error) {
	if (dbus_error_is_set(error)) {
		fprintf(stderr, "Error: %s - %s\n", error->name, error->message);
	} else {
	//set LocalDevice->discovering to False manually... lot faster :)
	ADAPTER->discovering = FALSE;	
	}
}


static  int cb_device_found_helper(RemoteDevice *s, const char *new_RemoteDevice) {
	if (!s) return -1;
	return strcmp(s->addr, new_RemoteDevice);
}

void cb_device_found (void *data, DBusMessage *msg) {
	
	DeviceList* DL = (DeviceList*) data;

	char *dev_addr=NULL;
	DBusError *error=NULL;
	
	dbus_message_get_args(msg, error,
	                      DBUS_TYPE_STRING, &dev_addr, DBUS_TYPE_INVALID);
	
	if (error &&  dbus_error_is_set(error)) {
		fprintf(stderr, "Error: %s - %s\n", error->name, error->message);
	}
	
	fprintf(stderr, "RemoteDevice FOUND SIGNAL --> %s\n", dev_addr);
	
	/* see if the RemoteDevice is already in the list before adding */
	int *cmp = eina_list_search_unsorted(DL->devices, (Eina_Compare_Cb)cb_device_found_helper, dev_addr);
	if(cmp==0) {
		RemoteDevice* device = malloc(sizeof(RemoteDevice));
		device->addr = dev_addr;
		gui_device_list_append(DL, device);
		
		/* Call  org.bluez.Adapter.CreateDevice xx:xx:xx:xx:xx:xx to get its path */
		dbus_get_remote_device_path(device);
	}
	
}

void cb_device_disappeared (void *data, DBusMessage *msg) {
	
	DeviceList* DL = (DeviceList*) data;

	char *dev_addr=NULL;
	DBusError *error=NULL;
	
	dbus_message_get_args(msg, error,
	                      DBUS_TYPE_STRING, &dev_addr, DBUS_TYPE_INVALID);
	
	if (error && dbus_error_is_set(error)) {
		fprintf(stderr, "Error: %s - %s\n", error->name, error->message);
	}
	
	fprintf(stderr, "RemoteDevice DISSAPEARED! --> %s\n", dev_addr);
	
	int *cmp = eina_list_search_unsorted(DL->devices, (Eina_Compare_Cb)cb_device_found_helper, dev_addr);
	if(cmp!=0) {
		/*TODO create a func to initizalize RemoteDevices */
		RemoteDevice* device = malloc(sizeof(RemoteDevice));
		device->addr = dev_addr;
		gui_device_list_remove(DL, device);
	}
	
}

