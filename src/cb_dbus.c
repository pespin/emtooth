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

void cb_start_discovery(void *data, DBusMessage *replymsg, DBusError *error) {
	if (dbus_error_is_set(error)) {
		printf("Error: %s - %s\n", error->name, error->message);
	}
}


static  int cb_device_found_helper(char *s, const char *new_device) {
	if (!s) return -1;
	return strcmp(s, new_device);
}

void cb_device_found (void *data, DBusMessage *msg) {
	
	DeviceList* DL = (DeviceList*) data;

	char *device=NULL;
	DBusError *error=NULL;
	
	dbus_message_get_args(msg, error,
	                      DBUS_TYPE_STRING, &device, DBUS_TYPE_INVALID);
	
	if (dbus_error_is_set(error)) {
		printf("Error: %s - %s\n", error->name, error->message);
	}
	
	fprintf(stderr, "DEVICE FOUND SIGNAL --> %s\n", device);
	
	/* see if the device is already in the list before adding */
	int *cmp = eina_list_search_unsorted(DL->devices, (Eina_Compare_Cb)cb_device_found_helper, device);
	if(cmp==0) {
		 //DL->devices = eina_list_append(DL->devices, device);
		 gui_device_list_append(DL, device);
	}
	
}

void cb_device_disappeared (void *data, DBusMessage *msg) {
	
	DeviceList* DL = (DeviceList*) data;

	char *device=NULL;
	DBusError *error=NULL;
	
	dbus_message_get_args(msg, error,
	                      DBUS_TYPE_STRING, &device, DBUS_TYPE_INVALID);
	
	if (dbus_error_is_set(error)) {
		printf("Error: %s - %s\n", error->name, error->message);
	}
	
	fprintf(stderr, "DEVICE DISSAPEARED! --> %s\n", device);
	
	int *cmp = eina_list_search_unsorted(DL->devices, (Eina_Compare_Cb)cb_device_found_helper, device);
	if(cmp!=0) {
		 gui_device_list_remove(DL, device);
	}
	
}

