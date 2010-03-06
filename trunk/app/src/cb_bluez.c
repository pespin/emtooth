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
#include "obex_agent.h"



/* Auxiliar functions used below: */

 static  int cb_device_found_helper_addr(RemoteDevice *s, const char *new_RemoteDevice) {
	if (!s) return -1;
	return strcmp(s->addr, new_RemoteDevice);
}
 
 static  int cb_device_found_helper_device(Elm_List_Item* s, const char *new_RemoteDevice) {
	if (!s) return -1;
	RemoteDevice* device = (RemoteDevice*) elm_list_item_data_get(s);
	return cb_device_found_helper_addr(device, new_RemoteDevice);
}
 

 
/* CALLBACK FUNCTIONS */

void cb_get_default_adapter(void *data, DBusMessage *replymsg, DBusError *error) {
	
	DBUSLOG(error);
	
	if(!replymsg) { /* no org.bluez, fso still loading it [fso bug] */
		fprintf(stderr, "ERROR: there's no org.bluez service!!! (probably fso hasn't loaded it yet).\n");
		
		if(bluez_error_counter<6) {
			if(bluez_error_counter==0)
					gui_alert_create("Sorry, it seems bluez dbus interface is not<br>" \
					"running at the moment.<br>Retrying some more times before exiting...");
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
	obex_agent_create();

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
	
	//attach signals:
	bluez_remote_device_attach_signals(device);
	
	//get remote device info:
	bluez_get_remote_device_info(device);
}


void cb_create_remote_device_path (void *data, DBusMessage *replymsg, DBusError *error) {

	RemoteDevice* device = (RemoteDevice*) data;
	
	DBUSLOG(error);
	if(!replymsg) return;
	
	fprintf(stderr, "Creating remote device [%s] dbus path...\n", device->addr);
	
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
	
	
	//attach signals:
	bluez_remote_device_attach_signals(device);
	
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



void cb_get_remote_device_properties_device (void *data, DBusMessage *replymsg, DBusError *error) {
	
	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Updating remote device [%s][%s] Device info... ", device->addr, device->path);
	
	DBUSLOG(error);
	if(!replymsg) return;	
	
	DBusMessageIter array_iter, dict_iter;

	//open dict:
	dbus_message_iter_init(replymsg, &array_iter);
	dbus_message_iter_recurse(&array_iter, &dict_iter);
	
	StructDbus* ret;
	int type = dbus_message_iter_get_arg_type (&dict_iter);
	
	bool can_free = TRUE; //used for dynamic types as strings and arrays
	
	//foreach pair...:
	while (type != DBUS_TYPE_INVALID) {
		ret = dbus_get_next_struct_in_dict(&dict_iter);

        //Depending on key, save value where it should go:
		if(!strcmp(ret->key,"Alias")) {
			device->alias = ret->value.value_string;
					
		} else if(!strcmp(ret->key,"Class")) {
			device->class = ret->value.value_int;
			
		} else if(!strcmp(ret->key,"Connected")) {
			device->connected_device = ret->value.value_int;
			
		} else if(!strcmp(ret->key,"Icon")) {
			device->icon = ret->value.value_string;
			
		} else if(!strcmp(ret->key,"Name")) {
			device->name = ret->value.value_string;
			
		} else if(!strcmp(ret->key,"Paired")) {
			device->paired = ret->value.value_int;
			
		} else if(!strcmp(ret->key,"Trusted")) {
			device->trusted = ret->value.value_int;
			
		} else if(!strcmp(ret->key,"UUIDs")) {
			char** tmp = device->UUIDs; 
			device->UUIDs = ret->value.value_array;
			array_free(tmp);
			can_free = FALSE;
		}
		
		//free stuff: key, as we are not using it, and struct.
		if(ret->key) free(ret->key);
		if(ret && can_free) free(ret);
		
		dbus_message_iter_next (&dict_iter);
		type = dbus_message_iter_get_arg_type (&dict_iter);
	}
	
	fprintf(stderr, "Done!\n");
	
	/* Now we have all info, append device to device GUI list if necessary */
	const Eina_List* li = elm_list_items_get(DL->li);
	li = eina_list_search_unsorted_list(li, (Eina_Compare_Cb)cb_device_found_helper_device, device->addr);
	
	if(!li) gui_device_list_append(device);
}

void cb_get_remote_device_properties_input(void *data, DBusMessage *replymsg, DBusError *error) {
	
	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Updating remote device [%s][%s] Input info... ", device->addr, device->path);
	
	DBUSLOG(error);
	if(!replymsg) return;	
	
	//open dict:
	DBusMessageIter array_iter, dict_iter, key_iter;
	dbus_message_iter_init(replymsg, &array_iter);
	dbus_message_iter_recurse(&array_iter, &dict_iter);
	dbus_message_iter_recurse(&dict_iter, &key_iter);
	StructDbus* ret = dbus_message_iter_get_dict_pair(&key_iter);
	
	device->connected_input = ret->value.value_int;
	
	struct_dbus_free(ret);
}


void cb_get_remote_device_properties_audio(void *data, DBusMessage *replymsg, DBusError *error) {
	
	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Updating remote device [%s][%s] Audio info... ", device->addr, device->path);
	
	DBUSLOG(error);
	if(!replymsg) return;
		
	//open dict:
	DBusMessageIter array_iter, dict_iter, key_iter;
	dbus_message_iter_init(replymsg, &array_iter);
	dbus_message_iter_recurse(&array_iter, &dict_iter);
	dbus_message_iter_recurse(&dict_iter, &key_iter);
	StructDbus* ret = dbus_message_iter_get_dict_pair(&key_iter);
	
	if(!strcmp(ret->value.value_string, "connected")) {
		device->connected_audio = TRUE;
	} else {
		device->connected_audio = FALSE;
	}
	
	struct_dbus_free(ret);
}


void cb_create_remote_paired_device(void *data, DBusMessage *replymsg, DBusError *error) {
	
	RemoteDevice* device = (RemoteDevice*) data;
	
	DBUSLOG(error);
	char buf[512];
	
	if(!replymsg) {
		snprintf(buf, 511, "<b>Pairing failed with error:</b><br><br>%s<br>%s", error->name, error->message);
		gui_alert_create(buf);	
		return;
	}
	
	char *path;

	dbus_message_get_args(replymsg, error,
	                      DBUS_TYPE_OBJECT_PATH, &path, DBUS_TYPE_INVALID);
	
	DBUSLOG(error);

	//device->path = strdup(path);
	fprintf(stderr, "Paired to device [%s] successfully!\n", path);
	
	sprintf(buf, "Paired to device %s successfully!\n", device->addr);
	gui_alert_create(buf);

}

void cb_device_found (void *data, DBusMessage *msg) {

	char *dev_addr=NULL;
	DBusError *error=NULL;
	
	dbus_message_get_args(msg, error,
	                      DBUS_TYPE_STRING, &dev_addr, DBUS_TYPE_INVALID);
	
	DBUSLOG(error);
	if(dev_addr) {
		dev_addr = strdup(dev_addr);
	} else return;
	
	fprintf(stderr, "SIGNAL: DeviceFound --> %s\n", dev_addr);
	
	/* see if the RemoteDevice is already in the list before adding */
	Eina_List* li = eina_list_search_unsorted_list(DL->devices, (Eina_Compare_Cb)cb_device_found_helper_addr, dev_addr);
	if(!li) {
		RemoteDevice* device = remote_device_new(dev_addr);
		DL->devices = eina_list_append(DL->devices, device);
		/* Call  org.bluez.Adapter.CreateDevice xx:xx:xx:xx:xx:xx to get its path */
		bluez_get_remote_device_path(device);
	}
	free(dev_addr);
	
}

void cb_device_disappeared (void *data, DBusMessage *msg) {

	char *dev_addr=NULL;
	DBusError *error=NULL;
	Eina_List* li=NULL;
	
	dbus_message_get_args(msg, error,
	                      DBUS_TYPE_STRING, &dev_addr, DBUS_TYPE_INVALID);
	
	DBUSLOG(error);
	
	fprintf(stderr, "SIGNAL: DeviceDissapeared --> %s\n", dev_addr);
	
	li = eina_list_search_unsorted_list(DL->devices, (Eina_Compare_Cb)cb_device_found_helper_addr, dev_addr);
	if(li) {
		RemoteDevice* device = eina_list_data_get(li);
		if(!device->paired) {
			fprintf(stderr, "Removing device [%s] from list\n", device->addr);
			gui_device_list_remove(device);
		}
	}
	
}

void cb_property_changed(void *data, DBusMessage *msg) {
	
	StructDbus* ret = dbus_message_get_dict_pair(msg);
	
	const char* iface = dbus_message_get_interface (msg);
	
	if(ret->value_type==DBUS_TYPE_STRING || ret->value_type == DBUS_TYPE_OBJECT_PATH) {
		fprintf(stderr, "SIGNAL: PropertyChanged (%s == %s) on interface %s\n", ret->key, ret->value.value_string, iface);
	} else if(ret->value_type==DBUS_TYPE_UINT32 || ret->value_type == DBUS_TYPE_BOOLEAN) {
		fprintf(stderr, "SIGNAL: PropertyChanged (%s == %d) on interface %s\n", ret->key, ret->value.value_int, iface);
	} else {
		fprintf(stderr, "SIGNAL: PropertyChanged (%s == UNKNOWN)  on interface %s... returning\n", ret->key, iface);
		return;
		}
	
	//dbus_dict_pair_debug(ret);
	
	/* TODO: finish handling here: depending on interface (Input, Device) */
	if(!data) {
		 bluez_get_local_device_info();
	} else {
		RemoteDevice* device = (RemoteDevice*) data;
		if(!strcmp(iface, "org.bluez.Device")) {
			bluez_get_remote_device_properties_device(device);
		} else if(!strcmp(iface, "org.bluez.Audio")) {
			bluez_get_remote_device_properties_audio(device);
		} else { //org.bluez.Input
			bluez_get_remote_device_properties_input(device);
		}
	}
	
	struct_dbus_free(ret);
}



void cb_device_created(void *data, DBusMessage *msg) {
	
	char *path=NULL;
	DBusError *error=NULL;
	
	dbus_message_get_args(msg, error,
	                      DBUS_TYPE_OBJECT_PATH, &path, DBUS_TYPE_INVALID);
	
	DBUSLOG(error);
	if(path) {
		path = strdup(path);
	} else return;
	
	fprintf(stderr, "SIGNAL: DeviceCreated --> [%s]\n", path);

	free(path);
}


void cb_device_removed(void *data, DBusMessage *msg) {
	
	char *path=NULL;
	DBusError *error=NULL;
	
	dbus_message_get_args(msg, error,
	                      DBUS_TYPE_OBJECT_PATH, &path, DBUS_TYPE_INVALID);
	
	DBUSLOG(error);
	if(path) {
		path = strdup(path);
	} else return;
	
	fprintf(stderr, "SIGNAL: DeviceRemoved --> [%s]\n", path);
	free(path);
}
