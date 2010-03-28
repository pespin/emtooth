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
	
	sprintf(buf, "%d Devices Found:", bluez_remote_device_list_count());
	elm_label_label_set(DL->header, buf);
	
	bluez_discovery_stop();
}


void cb_device_list_selected(void *data, Evas_Object *obj, void *event_info) {
   
	Elm_List_Item  *item = elm_list_selected_item_get(DL->li);
	if(!item) return;
	RemoteDevice* device = elm_list_item_data_get(item);
	fprintf(stderr, "\nDevice %s selected!\npath:%s;\nalias:%s;\nTrusted:%d;\n\n", 
	device->addr, device->path, device->alias,device->trusted);
	
	gui_remote_device_info_create(device);

}


void cb_settings_dialog(void *data, Evas_Object *obj, void *event_info) {

		gui_settings_dialog_create();
}


void cb_toggle_value_changed(void *data, Evas_Object *obj, void *event_info) {
	
	GuiCb* cb = (GuiCb*) data;
	
	StructDbus* info = malloc(sizeof(StructDbus));
	
	info->key = cb->property;
	info->value_type=DBUS_TYPE_BOOLEAN;
	info->value.value_int = elm_toggle_state_get(obj);
	
	fprintf(stderr, "Callback: change ->  %s=%d\n", info->key, info->value.value_int);
	bluez_set_property(info, cb->path, cb->interface);	
	
}


void cb_entry_value_integer_changed(void *data, Evas_Object *obj, void *event_info) {
	
	GuiCb* cb = (GuiCb*) data;
	
	StructDbus* info = malloc(sizeof(StructDbus));
	
	info->key = cb->property;
	info->value_type=DBUS_TYPE_UINT32;
	info->value.value_int = atoi(elm_entry_entry_get(obj));
	
	fprintf(stderr, "Callback: change ->  %s=%d\n", info->key, info->value.value_int);
	bluez_set_property(info, cb->path, cb->interface);	
	
}

void cb_entry_value_string_changed(void *data, Evas_Object *obj, void *event_info) {
	
	GuiCb* cb = (GuiCb*) data;
	
	StructDbus* info = (StructDbus*) malloc(sizeof(StructDbus));
	
	info->key = cb->property;
	info->value_type=DBUS_TYPE_STRING;
	info->value.value_string = strdup(elm_entry_entry_get(obj));
	
	fprintf(stderr, "Callback: change ->  %s=%s\n", info->key, info->value.value_string);
	bluez_set_property(info, cb->path, cb->interface);
}

void cb_remote_paired_changed(void *data, Evas_Object *obj, void *event_info) {
	
	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Callback: Paired change ->  value=%d\n", elm_toggle_state_get(obj));
	
	if(elm_toggle_state_get(obj)) {
		elm_object_disabled_set(obj, TRUE);
		bluez_create_remote_paired_device(device);
	}
	//TODO: remove pairing, how?
	//else
	//	bluez_remove_remote_device(device);
}


void cb_toggle_input_connect(void *data, Evas_Object *obj, void *event_info) {
	
	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Callback: InputConnect change ->  value=%d\n", elm_toggle_state_get(obj));
	
	if(elm_toggle_state_get(obj)) {
		bluez_remote_device_input_connect(device);
	} else {
		bluez_remote_device_input_disconnect(device);
	}
	
}

void cb_toggle_audio_connect(void *data, Evas_Object *obj, void *event_info) {
	
	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Callback: AudioConnect change ->  value=%d\n", elm_toggle_state_get(obj));
	
	if(elm_toggle_state_get(obj)) {
		bluez_remote_device_audio_connect(device);
	} else {
		bluez_remote_device_audio_disconnect(device);
	}
	
}

void cb_send_files_clicked(void *data, Evas_Object *obj, void *event_info) {
	gui_request_file_path_create(data, cb_fileselector_send_file);
}

void cb_remove_device_clicked(void *data, Evas_Object *obj, void *event_info) {
	 
	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Callback: RemoveDevice button on device [%s]\n", device->addr);
		 
	bluez_adapter_remove_remote_device(device);
	gui_device_list_remove(device);

}


void cb_request_pin(void *data, Evas_Object *obj, void *event_info) {
	
	DialogCb* Dialog = (DialogCb*) data;
	Dialog->device->password = strdup(elm_entry_entry_get(Dialog->entry));
	evas_object_del(Dialog->win);
	free(Dialog);
}



void cb_fileselector_send_file(void *data, Evas_Object *obj, void *event_info) {
	   /* event_info conatin the full path of the selected file
    * or NULL if none is selected or cancel is pressed */
   const char *selected = event_info;
  
	Evas_Object* parent = elm_object_parent_widget_get(obj); //vbox
	parent = parent = elm_object_parent_widget_get(parent);
	evas_object_del(parent);
	
   if (selected) {
		fprintf(stderr,"Selected file: %s\n", selected);

		char** array = (char**) malloc(sizeof(char*)*2);
		
		array[0] = strdup(selected);
		array[1] = NULL;
		
		obex_client_SendFiles(data, array);
		array_free(array);
	}	
}
