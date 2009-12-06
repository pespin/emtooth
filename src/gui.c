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

#include "gui.h"

void gui_create(DeviceList* DL) {

   Evas_Object *win, *bg, *vbox, *fr, *lb, *device_list, *hbox, *hbox1, *bt;

   win = elm_win_add(NULL, "main_win", ELM_WIN_BASIC);
   elm_win_title_set(win, "emtooth");
   evas_object_smart_callback_add(win, "delete,request", cb_safe_exit, NULL);

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, 1.0, 1.0);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);
   
   	evas_object_resize(win, 480, 600);	
   
   	//add vbox 4
	vbox = elm_box_add(win);
	elm_win_resize_object_add(win, vbox);
	evas_object_size_hint_weight_set(vbox, 1.0, 1.0);
	evas_object_show(vbox);

	// add button hbox
	hbox = elm_box_add(win);
	
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox, -1.0, 0.0);
	elm_box_pack_end(vbox, hbox);
	evas_object_show(hbox);

	// add a frame
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "outdent_top");
	evas_object_size_hint_weight_set(fr, 0.0, 0.0);
	evas_object_size_hint_align_set(fr, 0.0, -1.0);
	elm_box_pack_end(hbox, fr);
	evas_object_show(fr);

	// add a label
	lb = elm_label_add(win);
	elm_label_label_set(lb, "RemoteDevices");
	elm_frame_content_set(fr, lb);
	evas_object_show(lb);  
	
	device_list = elm_list_add(win);
	elm_object_scale_set(device_list,1.0);
	elm_scroller_bounce_set(device_list, 0 ,0);
	evas_object_size_hint_weight_set(device_list, 1.0, 1.0);
	evas_object_size_hint_align_set(device_list, -1.0, -1.0);
	elm_genlist_multi_select_set(device_list, 0); 
	elm_box_pack_end(vbox, device_list);
	evas_object_smart_callback_add(device_list, "clicked", cb_device_list_selected, DL); 
	evas_object_show(device_list);

	// add button hbox1
	hbox1 = elm_box_add(win);
	elm_box_horizontal_set(hbox1, 1);
	evas_object_size_hint_weight_set(hbox1, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox1, -1.0, 0.0);
	elm_box_pack_end(vbox, hbox1);
	evas_object_show(hbox1);

	//add buttons to hbox1
	bt = elm_button_add(win);
	elm_button_label_set(bt, "Refresh");
	evas_object_size_hint_weight_set(bt, 1.0, 1.0);
	evas_object_size_hint_align_set(bt, -1.0, -1.0);
	elm_box_pack_end(hbox1, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", cb_device_list_refresh, DL);

	bt = elm_button_add(win);
	elm_button_label_set(bt, "Settings");
	evas_object_size_hint_weight_set(bt, 1.0, 1.0);
	evas_object_size_hint_align_set(bt, -1.0, -1.0);
	elm_box_pack_end(hbox1, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", cb_settings_dialog, DL);


	DL->li = device_list;	

   gui_device_list_populate(DL);
  
   evas_object_show(win);
	
}


void gui_device_list_remove(DeviceList* DL, RemoteDevice* device) {
	
	fprintf(stderr, "Removing RemoteDevice %s from list...\n", device->addr);
	
	DL->devices = eina_list_remove(DL->devices, device);
	
	/* TODO: just remove one item instead of rewriting all the list */
	gui_device_list_clear(DL->li);
	gui_device_list_populate(DL);
	
}


void gui_device_list_append(DeviceList* DL, RemoteDevice* device) {

	fprintf(stderr, "Adding RemoteDevice %s to list...\n", device->addr);
	
	//Add last RemoteDevice to eina list.
	DL->devices = eina_list_append(DL->devices, device);
	
	//last data:
	//char* label = eina_list_data_get(eina_list_last(DL->RemoteDevices));
	
	Evas_Object *ic;
	ic = elm_icon_add(DL->li);
	elm_icon_standard_set(ic, "arrow_left");
    elm_icon_scale_set(ic, 0, 1);
    elm_list_item_append(DL->li, device->addr, ic, NULL,  NULL, device);
	evas_object_show(ic);
	
	elm_list_go(DL->li);
}


void gui_device_list_populate(DeviceList* DL) {
	
	fprintf(stderr, "Populating list with RemoteDevices already catched...\n");
	Evas_Object *ic;
	RemoteDevice* device;
	Eina_List* cur;
	
	EINA_LIST_FOREACH(DL->devices, cur, device) {
		ic = elm_icon_add(DL->li);
        elm_icon_standard_set(ic, "arrow_left");
        elm_icon_scale_set(ic, 0, 1);
		elm_list_item_append(DL->li, device->addr, ic, NULL,  NULL, device);
		evas_object_show(ic);
	}

	elm_list_go(DL->li);
}



void gui_device_list_clear(Evas_Object *li) {

fprintf(stderr, "Clearing list of RemoteDevices...\n");
	const Eina_List *items;
	
	while ((items = elm_list_items_get(li))) {
        elm_list_item_del(items->data);
     }
	
	//is this necessary?
	elm_list_go(li);
}


void gui_settings_dialog_create() {

   Evas_Object *win, *bg, *vbox, *fr, *lb, *hbox, *hbox1, *bt;

   win = elm_win_add(NULL, "settings_dialog", ELM_WIN_BASIC);
   elm_win_title_set(win, "emtooth - Settings");
   evas_object_smart_callback_add(win, "delete,request", cb_close_win, win);

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, 1.0, 1.0);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);
   
   	evas_object_resize(win, 480, 600);	
   
   	//add vbox 4
	vbox = elm_box_add(win);
	elm_win_resize_object_add(win, vbox);
	evas_object_size_hint_weight_set(vbox, 1.0, 1.0);
	evas_object_show(vbox);

	// add button hbox
	hbox = elm_box_add(win);
	
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox, -1.0, 0.0);
	elm_box_pack_end(vbox, hbox);
	evas_object_show(hbox);

	// add a frame
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "outdent_top");
	evas_object_size_hint_weight_set(fr, 0.0, 0.0);
	evas_object_size_hint_align_set(fr, 0.0, -1.0);
	elm_box_pack_end(hbox, fr);
	evas_object_show(fr);

	// add a label
	lb = elm_label_add(win);
	elm_label_label_set(lb, "SETTTTTIIINGS");
	elm_frame_content_set(fr, lb);
	evas_object_show(lb);  

	// add button hbox1
	hbox1 = elm_box_add(win);
	elm_box_horizontal_set(hbox1, 1);
	evas_object_size_hint_weight_set(hbox1, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox1, -1.0, 0.0);
	elm_box_pack_end(vbox, hbox1);
	evas_object_show(hbox1);

	//add buttons to hbox1
	bt = elm_button_add(win);
	elm_button_label_set(bt, "Close");
	evas_object_size_hint_weight_set(bt, 1.0, 1.0);
	evas_object_size_hint_align_set(bt, -1.0, -1.0);
	elm_box_pack_end(hbox1, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", cb_close_win, win);
  
   evas_object_show(win);
	
}
