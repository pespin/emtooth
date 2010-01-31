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
#include <unistd.h> /* sleep() */
#include "gui.h"
#include "cb_bluez.h"
#include "dbus.h"



/* Auxiliar functions used below: */
 
 static  int cb_device_found_helper(RemoteDevice *s, const char *new_RemoteDevice) {
	if (!s) return -1;
	return strcmp(s->addr, new_RemoteDevice);
}
 
 static  int cb_device_found_helper2(Elm_List_Item* s, const char *new_RemoteDevice) {
	if (!s) return -1;
	RemoteDevice* device = (RemoteDevice*) elm_list_item_data_get(s);
	return cb_device_found_helper(device, new_RemoteDevice);
}
 

 
/* CALLBACK FUNCTIONS */

void cb_get_default_adapter(void *data, DBusMessage *replymsg, DBusError *error) {
	
	DBUSLOG(error);
	
	if(!replymsg) { /* no org.bluez, fso still loading it [fso bug] */
		fprintf(stderr, "ERROR ON BLUEZ-DBUS!\n");
		
		if(bluez_error_counter<6) {
			if(bluez_error_counter==0)
					gui_alert_create("Sorry, it seems bluez dbus interface is not" \
					"running at the moment. Retrying some more times before exiting...");
			bluez_error_counter++;
			sleep(2);					
			bluez_get_default_adapter();
			return;
		} else exit(EXIT_FAILURE);
	}
	char *path = NULL;
	dbus_message_get_args(replymsg, error,
	                      DBUS_TYPE_OBJECT_PATH, &path, DBUS_TYPE_INVALID);
	
	DBUSLOG(error);
	
	if(!path) {
		fprintf(stderr, "Could not get bluez dbus device path. EXIT NOW!\n");
		gui_alert_create("Could not get bluez dbus device path. Exiting...\n");
		sleep(5);
		return;
	}
	
	ADAPTER->path = strdup(path);
	fprintf(stderr, "Using path '%s' to connect to bluez dbus daemon...\n", ADAPTER->path);
	
	/* connect to ChangeProperty signal */
	e_dbus_signal_handler_add(
	DBUSCONN->sysconn,
	"org.bluez", 
	ADAPTER->path,
	"org.bluez.Adapter",
	"PropertyChanged",
	cb_property_changed,
	NULL);
	
	/* Now we have adapter path,
	get local device info, register agent and start signals on that interface: */
	
	bluez_agent_create();

	bluez_get_local_device_info();
	bluez_discovery_start();
}




void cb_get_remote_device_path (void *data, DBusMessage *replymsg, DBusError *error) {

	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Getting remote device [%s] dbus path...\n", device->addr);
	
	if(!replymsg) {
		/* As no one has created a path to this device, we create it us: */
		fprintf(stderr, "Could not get a path to remote device [%s], creating it...\n", device->addr);
		bluez_create_remote_device_path(device);
		return;
	}
	
	char *path = NULL;
	
	dbus_message_get_args(replymsg, error,
	                      DBUS_TYPE_OBJECT_PATH, &path, DBUS_TYPE_INVALID);
	
	DBUSLOG(error);
	

	if(!path) { 
		fprintf(stderr, "Could not get a path to remote device [%s], probably to a dbus error... do nothing\n", device->addr);
		return;
	}
	
	device->path = strdup(path);
	fprintf(stderr, "Using path '%s' to connect to remote device [%s]...\n", device->path, device->addr);
	
	//Connect to PropertyChanged signal:
	e_dbus_signal_handler_add(
	DBUSCONN->sysconn,
	"org.bluez", 
	device->path,
	"org.bluez.Device",
	"PropertyChanged",
	cb_property_changed,
	device);
	
	
	//get remote device info:
	bluez_get_remote_device_info(device);
}


void cb_create_remote_device_path (void *data, DBusMessage *replymsg, DBusError *error) {

	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Creating remote device [%s] dbus path...\n", device->addr);
	
	DBUSLOG(error);
	
	char *path = NULL;
	
	dbus_message_get_args(replymsg, error,
	                      DBUS_TYPE_OBJECT_PATH, &path, DBUS_TYPE_INVALID);
	
	DBUSLOG(error);
	
	if(!path) {
		fprintf(stderr, "Could not create a path to remote device [%s]\n", device->addr);
		return;
	}
	
	device->path = strdup(path);
	fprintf(stderr, "Using path '%s' to connect to remote device [%s]...\n", device->path, device->addr);
	
	
		//Connect to PropertyChanged signal:
	e_dbus_signal_handler_add(
	DBUSCONN->sysconn,
	"org.bluez", 
	device->path,
	"org.bluez.Device",
	"PropertyChanged",
	cb_property_changed,
	device);
	
	
	//get remote device info:
	bluez_get_remote_device_info(device);
}




void cb_get_local_device_info (void *data, DBusMessage *replymsg, DBusError *error) {
		
	fprintf(stderr, "Updating local device info... ");	
	
	DBUSLOG(error);
		
	DBusMessageIter array_iter, dict_iter;
	//open dict:
	dbus_message_iter_init(replymsg, &array_iter);
	dbus_message_iter_recurse(&array_iter, &dict_iter);
	
	StructDbus* ret;
	int type = dbus_message_iter_get_arg_type (&dict_iter);
	
	//foreach pair...:
	while (type != DBUS_TYPE_INVALID) {
		
		ret = dbus_get_next_struct_in_dict(&dict_iter);	
        
        //Depending on key, save value where it should go:
		if(!strcmp(ret->key,"Address")) {
			ADAPTER->addr =  ret->value.value_string;
					
		} else if(!strcmp(ret->key,"Class")) {
			ADAPTER->class = ret->value.value_int;
			
		} else if(!strcmp(ret->key,"Discoverable")) {
			ADAPTER->discoverable = ret->value.value_int;
			
		} else if(!strcmp(ret->key,"DiscoverableTimeout")) {
			ADAPTER->discoverable_timeout = ret->value.value_int;
			
		} else if(!strcmp(ret->key,"Discovering")) {
			ADAPTER->discovering = ret->value.value_int;
			
		} else if(!strcmp(ret->key,"Name")) {
			ADAPTER->name = ret->value.value_string;
			
		} else if(!strcmp(ret->key,"Pairable")) {
			ADAPTER->pairable = ret->value.value_int;
			
		} else if(!strcmp(ret->key,"PairableTimeout")) {
			ADAPTER->pairable_timeout = ret->value.value_int;

		} else if(!strcmp(ret->key,"Powered")) {
			ADAPTER->powered = ret->value.value_int;
		}
		
		//free stuff: key, as we are not using it, and struct.
		if(ret->key) free(ret->key);
		if(ret) free(ret);
		
		dbus_message_iter_next (&dict_iter);
		type = dbus_message_iter_get_arg_type (&dict_iter);
	}
	
	fprintf(stderr, "Done!\n");
}



void cb_get_remote_device_info (void *data, DBusMessage *replymsg, DBusError *error) {
	
	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Updating remote device [%s][%s] info... ", device->addr, device->path);
	
	DBUSLOG(error);
	if(!replymsg) return;	
	
	DBusMessageIter array_iter, dict_iter;

	//open dict:
	dbus_message_iter_init(replymsg, &array_iter);
	dbus_message_iter_recurse(&array_iter, &dict_iter);
	
	StructDbus* ret;
	int type = dbus_message_iter_get_arg_type (&dict_iter);
	
	//foreach pair...:
	while (type != DBUS_TYPE_INVALID) {
		ret = dbus_get_next_struct_in_dict(&dict_iter);

        //Depending on key, save value where it should go:
		if(!strcmp(ret->key,"Alias")) {
			device->alias = ret->value.value_string;
					
		} else if(!strcmp(ret->key,"Class")) {
			device->class = ret->value.value_int;
			
		} else if(!strcmp(ret->key,"Connected")) {
			device->connected = ret->value.value_int;
			
		} else if(!strcmp(ret->key,"Icon")) {
			device->icon = ret->value.value_string;
			
		} else if(!strcmp(ret->key,"Name")) {
			device->name = ret->value.value_string;
			
		} else if(!strcmp(ret->key,"Paired")) {
			device->paired = ret->value.value_int;
			
		} else if(!strcmp(ret->key,"Trusted")) {
			device->trusted = ret->value.value_int;
		}
		
		//free stuff: key, as we are not using it, and struct.
		if(ret->key) free(ret->key);
		if(ret) free(ret);
		
		dbus_message_iter_next (&dict_iter);
		type = dbus_message_iter_get_arg_type (&dict_iter);
	}
	
	fprintf(stderr, "Done!\n");
	
	/* Now we have all info, append device to device GUI list if necessary */
	const Eina_List* li = elm_list_items_get(DL->li);
	li = eina_list_search_unsorted_list(li, (Eina_Compare_Cb)cb_device_found_helper2, device->addr);
	
	if(!li) gui_device_list_append(device);
}

void cb_create_remote_paired_device(void *data, DBusMessage *replymsg, DBusError *error) {
	
	RemoteDevice* device = (RemoteDevice*) data;
	
	//fprintf(stderr, "Pairing to device [%s]...\n", device->addr);
	
	DBUSLOG(error);
	
	if(!replymsg) return;
	
	char *path;

	dbus_message_get_args(replymsg, error,
	                      DBUS_TYPE_OBJECT_PATH, &path, DBUS_TYPE_INVALID);
	
	DBUSLOG(error);

	//device->path = strdup(path);
	fprintf(stderr, "Paired to device [%s] successfully!\n", path); 

}

void cb_device_found (void *data, DBusMessage *msg) {

	char *dev_addr=NULL;
	DBusError *error=NULL;
	
	dbus_message_get_args(msg, error,
	                      DBUS_TYPE_STRING, &dev_addr, DBUS_TYPE_INVALID);
	
	DBUSLOG(error);
	
	fprintf(stderr, "SIGNAL: DeviceFound --> %s\n", dev_addr);
	
	/* see if the RemoteDevice is already in the list before adding */
	Eina_List* li = eina_list_search_unsorted_list(DL->devices, (Eina_Compare_Cb)cb_device_found_helper, dev_addr);
	if(!li) {
		RemoteDevice* device = malloc(sizeof(RemoteDevice));
		device->addr = dev_addr;
		device->alias = NULL;
		device->password = NULL; //set to null, used for pairing agent later.
		DL->devices = eina_list_append(DL->devices, device);
		/* Call  org.bluez.Adapter.CreateDevice xx:xx:xx:xx:xx:xx to get its path */
		bluez_get_remote_device_path(device);
	}
	
}

void cb_device_disappeared (void *data, DBusMessage *msg) {

	char *dev_addr=NULL;
	DBusError *error=NULL;
	Eina_List* li=NULL;
	
	dbus_message_get_args(msg, error,
	                      DBUS_TYPE_STRING, &dev_addr, DBUS_TYPE_INVALID);
	
	DBUSLOG(error);
	
	fprintf(stderr, "SIGNAL: DeviceDissapeared --> %s\n", dev_addr);
	
	li = eina_list_search_unsorted_list(DL->devices, (Eina_Compare_Cb)cb_device_found_helper, dev_addr);
	if(li) {
		RemoteDevice* device = eina_list_data_get(li);
		fprintf(stderr, "\nRemoving device [%s] from list\n", device->addr);
		gui_device_list_remove(device);
	}
	
}

void cb_property_changed(void *data, DBusMessage *msg) {
	
	StructDbus* ret = dbus_message_get_dict_pair(msg);
	
	if(ret->value_type==DBUS_TYPE_STRING || ret->value_type == DBUS_TYPE_OBJECT_PATH) {
		fprintf(stderr, "SIGNAL: PropertyChanged (%s == %s)\n", ret->key, ret->value.value_string);
	} else if(ret->value_type==DBUS_TYPE_UINT32 || ret->value_type == DBUS_TYPE_BOOLEAN) {
		fprintf(stderr, "SIGNAL: PropertyChanged (%s == %d)\n", ret->key, ret->value.value_int);
	} else {
		fprintf(stderr, "SIGNAL: PropertyChanged (%s == UNKNOWN)... returning\n", ret->key);
		return;
		}
	
	//dbus_dict_pair_debug(ret);
	
	/* TODO: finish handling here */
	if(!data) bluez_get_local_device_info();
	else {
		RemoteDevice* device = (RemoteDevice*) data;
		bluez_get_remote_device_info(device);
	}
	
}


void cb_device_removed(void *data, DBusMessage *msg) {
	
	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "DeviceRemoved signal: [%s]\n", device->addr);
	
	gui_device_list_remove(device);
	
}