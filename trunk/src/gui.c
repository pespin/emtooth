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

   Evas_Object *win, *bg, *vbox, *fr, *lb, *device_list, *hbox, *hbox1, *refresh_button;

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
	elm_label_label_set(lb, "Devices");
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

	//add add_album button to hbox1
	refresh_button = elm_button_add(win);
	elm_button_label_set(refresh_button, "Refresh");
	evas_object_size_hint_weight_set(refresh_button, 1.0, 1.0);
	evas_object_size_hint_align_set(refresh_button, -1.0, -1.0);
	elm_box_pack_end(hbox1, refresh_button);
	evas_object_show(refresh_button);

	DL->li = device_list;

	evas_object_smart_callback_add(refresh_button, "clicked", cb_device_list_refresh, DL);	

   gui_device_list_populate(DL);
   
   

   evas_object_show(win);
	
}


void gui_device_list_remove(DeviceList* DL, const char* label) {
	
	fprintf(stderr, "Removing device %s from list...\n", label);
	
	DL->devices = eina_list_remove(DL->devices, label);
	
	/* TODO: just remove one item instead of rewriting all the list */
	gui_device_list_clear(DL->li);
	gui_device_list_populate(DL);
	
}


void gui_device_list_append(DeviceList* DL, const char* label) {

	fprintf(stderr, "Adding device %s to list...\n", label);
	
	//Add last device to eina list.
	DL->devices = eina_list_append(DL->devices, label);
	
	//last data:
	//char* label = eina_list_data_get(eina_list_last(DL->devices));
	
	Evas_Object *ic;
	ic = elm_icon_add(DL->li);
	elm_icon_standard_set(ic, "arrow_left");
    elm_icon_scale_set(ic, 0, 1);
    elm_list_item_append(DL->li, label, ic, NULL,  NULL, label);
	evas_object_show(ic);
	
	elm_list_go(DL->li);
}


void gui_device_list_populate(DeviceList* DL) {
	
	fprintf(stderr, "Populating list with devices already catched...\n");
	Evas_Object *ic;
	char* device;
	Eina_List* cur;
	
	EINA_LIST_FOREACH(DL->devices, cur, device) {
		ic = elm_icon_add(DL->li);
        elm_icon_standard_set(ic, "arrow_left");
        elm_icon_scale_set(ic, 0, 1);
		elm_list_item_append(DL->li, device, ic, NULL,  NULL, device);
		evas_object_show(ic);
	}

	elm_list_go(DL->li);
}



void gui_device_list_clear(Evas_Object *li) {

fprintf(stderr, "Clearing list of devices...\n");
	const Eina_List *items;
	
	while ((items = elm_list_items_get(li)))
     {
        //also free the path string attached to the data value of the item
        char *path = (char *)elm_list_item_data_get(items->data);
        if (path) free(path);
        //delete the item
        elm_list_item_del(items->data);
     }
	
	//is this necessary?
	elm_list_go(li);
}
