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


void cb_get_dbus_path(void *data, DBusMessage *replymsg, DBusError *error) {
	
	char *path = NULL;
	
	dbus_message_get_args(replymsg, error,
	                      DBUS_TYPE_OBJECT_PATH, &path, DBUS_TYPE_INVALID);
	
	if (dbus_error_is_set(error)) {
		printf("Error: %s - %s\n", error->name, error->message);
	}
	
	if(!path) {
		fprintf(stderr, "Could not get bluez dbus RemoteDevice path. EXIT NOW!\n", path);
		exit(EXIT_FAILURE);
	}
	
	BLUEZPATH = path;
	fprintf(stderr, "Using path '%s' to connect to bluez dbus daemon...\n", BLUEZPATH);
	
	//start signals on that interface:
	dbus_start_discovery(data);
}



void cb_start_discovery(void *data, DBusMessage *replymsg, DBusError *error) {
	if (dbus_error_is_set(error)) {
		printf("Error: %s - %s\n", error->name, error->message);
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
	
	if (dbus_error_is_set(error)) {
		printf("Error: %s - %s\n", error->name, error->message);
	}
	
	fprintf(stderr, "RemoteDevice FOUND SIGNAL --> %s\n", dev_addr);
	
	/* see if the RemoteDevice is already in the list before adding */
	int *cmp = eina_list_search_unsorted(DL->devices, (Eina_Compare_Cb)cb_device_found_helper, dev_addr);
	if(cmp==0) {
		/*TODO create a func to initizalize RemoteDevices */
		RemoteDevice* device = malloc(sizeof(RemoteDevice));
		device->addr = dev_addr;
		gui_device_list_append(DL, device);
	}
	
}

void cb_device_disappeared (void *data, DBusMessage *msg) {
	
	DeviceList* DL = (DeviceList*) data;

	char *dev_addr=NULL;
	DBusError *error=NULL;
	
	dbus_message_get_args(msg, error,
	                      DBUS_TYPE_STRING, &dev_addr, DBUS_TYPE_INVALID);
	
	if (dbus_error_is_set(error)) {
		printf("Error: %s - %s\n", error->name, error->message);
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

