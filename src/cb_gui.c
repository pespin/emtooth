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
#include "cb_gui.h"

void cb_alert_bt(void *data, Evas_Object *obj, void *event_info){
	Evas_Object* win = (Evas_Object*) data;
	evas_object_del(win);
}

void cb_safe_exit(void *data, Evas_Object *obj, void *event_info) {
	fso_disable_bluetooth();
	elm_exit();
}
void cb_close_win(void *data, Evas_Object *obj, void *event_info) {
	evas_object_del(data);	
}


void cb_discovery_start_clicked(void *data, Evas_Object *obj, void *event_info) {
	fprintf(stderr, "Start Discovery button pressed.\n");
	ObjCb* Cb = (ObjCb*) data;
   
	evas_object_hide(obj);
	elm_box_unpack(Cb->parent, obj);
	
	elm_box_pack_start(Cb->parent, Cb->obj);
	evas_object_show(Cb->obj);
	
	//clear device list:
	DL->devices = eina_list_free (DL->devices);
	elm_list_clear(DL->li);
	
	elm_label_label_set(DL->header, "Discovering Devices...");
	
	bluez_discovery_start();
}


void cb_discovery_stop_clicked(void *data, Evas_Object *obj, void *event_info) {
   fprintf(stderr, "Stop Discovery button pressed.\n");
   
   ObjCb* Cb = (ObjCb*) data;
   char buf[255];
   
	evas_object_hide(obj);
	elm_box_unpack(Cb->parent, obj);

	elm_box_pack_start(Cb->parent, Cb->obj);
	evas_object_show(Cb->obj);
	
	sprintf(buf, "%d Devices Found:", eina_list_count(DL->devices));
	elm_label_label_set(DL->header, buf);
	
	bluez_discovery_stop();
}


void cb_device_list_selected(void *data, Evas_Object *obj, void *event_info) {
   
	Elm_List_Item  *item = elm_list_selected_item_get(DL->li);
	if(!item) return;
	RemoteDevice* device = elm_list_item_data_get(item);
	fprintf(stderr, "\nDevice %s selected!\npath:%s;\nalias:%s;\nTrusted:%d;\n\n", 
	device->addr, device->path, device->alias,device->trusted);

}


void cb_settings_dialog(void *data, Evas_Object *obj, void *event_info) {

		gui_settings_dialog_create();
}


void cb_discoverable_changed(void *data, Evas_Object *obj, void *event_info) {

	StructDbus* info = malloc(sizeof(StructDbus));
	
	info->key = strdup("Discoverable");
	info->value_type=DBUS_TYPE_BOOLEAN;
	info->value.value_int = elm_toggle_state_get(obj);
	ADAPTER->discoverable = info->value.value_int;
	fprintf(stderr, "Discoverable status changed to %d\n", ADAPTER->discoverable);
	bluez_set_property(info);
	
}

void cb_pairable_changed(void *data, Evas_Object *obj, void *event_info) {
	
	StructDbus* info = malloc(sizeof(StructDbus));
	
	info->key = strdup("Pairable");
	info->value_type=DBUS_TYPE_BOOLEAN;
	info->value.value_int = elm_toggle_state_get(obj);
	ADAPTER->discoverable = info->value.value_int;
	fprintf(stderr, "Pairable status changed to %d\n", ADAPTER->discoverable);
	bluez_set_property(info);
	
}
