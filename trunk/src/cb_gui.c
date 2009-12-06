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

#include <Elementary.h>
#include "gui.h"

void cb_safe_exit(void *data, Evas_Object *obj, void *event_info) {
   elm_exit();
}


void cb_device_list_refresh(void *data, Evas_Object *obj, void *event_info) {
   fprintf(stderr, "Refresh button pressed.\n");
   DeviceList* DL = (DeviceList*) data;
   gui_device_list_clear(DL->li);
   gui_device_list_populate(DL);
}


void cb_device_list_selected(void *data, Evas_Object *obj, void *event_info) {
   DeviceList* DL = (DeviceList*) data;
   
	Elm_List_Item  *item = elm_list_selected_item_get(DL->li);
	if(!item) return;
	char* device = elm_list_item_data_get(item);
	fprintf(stderr, "Device %s selected!\n", device);
   /* TODO: here we call a function inside gui.c to create a win with 
    * info/functions on the selected device */
}

