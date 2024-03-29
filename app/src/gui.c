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

Evas_Object* gui_create() {

   Evas_Object *win, *bg, *vbox, *fr, *hbox, *hbox1, *bt, *bt_start, *bt_stop;

	ObjCb* Cb;

   win = elm_win_add(NULL, "main_win", ELM_WIN_BASIC);
   elm_win_title_set(win, "emtooth");
   evas_object_smart_callback_add(win, "delete,request", cb_safe_exit, NULL);

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, 1.0, 1.0);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);
   
   	evas_object_resize(win, DISPLAY_WIDTH, DISPLAY_HEIGHT);	
   
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
	
	DL->header = elm_label_add(win);
	elm_label_label_set(DL->header, "Discovering Devices...");
	elm_frame_content_set(fr, DL->header);
	evas_object_show(DL->header);  
	
	DL->li = elm_list_add(win);
	elm_object_scale_set(DL->li,1.0);
	elm_scroller_bounce_set(DL->li, 0 ,0);
	evas_object_size_hint_weight_set(DL->li, 1.0, 1.0);
	evas_object_size_hint_align_set(DL->li, -1.0, -1.0);
	//after last E bump, this segfaults for me:
	//elm_genlist_multi_select_set(DL->li, 0); 
	elm_box_pack_end(vbox, DL->li);
	evas_object_smart_callback_add(DL->li, "clicked", cb_device_list_selected, DL); 
	evas_object_show(DL->li);

	// add button hbox1
	hbox1 = elm_box_add(win);
	elm_box_horizontal_set(hbox1, 1);
	evas_object_size_hint_weight_set(hbox1, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox1, -1.0, 0.0);
	elm_box_pack_end(vbox, hbox1);
	evas_object_show(hbox1);

	//add buttons to hbox1
	bt_start = elm_button_add(win);
	elm_button_label_set(bt_start, "Start Discovery");
	evas_object_size_hint_weight_set(bt_start, 1.0, 1.0);
	evas_object_size_hint_align_set(bt_start, -1.0, -1.0);
	//elm_box_pack_end(hbox1, bt_start);
	//evas_object_show(bt_start);
	
	bt_stop = elm_button_add(win);
	elm_button_label_set(bt_stop, "Stop Discovery");
	evas_object_size_hint_weight_set(bt_stop, 1.0, 1.0);
	evas_object_size_hint_align_set(bt_stop, -1.0, -1.0);
	elm_box_pack_end(hbox1, bt_stop);
	evas_object_show(bt_stop);
	
	//button callbacks:
	Cb = malloc(sizeof(ObjCb));
	Cb->parent = hbox1;
	Cb->obj = bt_stop;
	evas_object_smart_callback_add(bt_start, "clicked", cb_discovery_start_clicked, Cb);
	
	Cb = malloc(sizeof(ObjCb));
	Cb->parent = hbox1;
	Cb->obj = bt_start;
	evas_object_smart_callback_add(bt_stop, "clicked", cb_discovery_stop_clicked, Cb);

	bt = elm_button_add(win);
	elm_button_label_set(bt, "Settings");
	evas_object_size_hint_weight_set(bt, 1.0, 1.0);
	evas_object_size_hint_align_set(bt, -1.0, -1.0);
	elm_box_pack_end(hbox1, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", cb_settings_dialog, DL);
  return win;
	
}


void gui_device_list_remove(RemoteDevice* device) {
	fprintf(stderr, "Removing RemoteDevice %s from list...\n", device->addr);
	
	DL->devices = eina_list_remove(DL->devices, device);
	
	/* TODO: just remove one item instead of rewriting all the list */
	elm_list_clear(DL->li);
	remote_device_free(device);
	gui_device_list_populate();
	
}


void gui_device_list_append(RemoteDevice* device) {

	fprintf(stderr, "Adding RemoteDevice %s to list...\n", device->addr);
	
	device->valid = TRUE;
	
	char buf[255];
	if(device->path) snprintf(buf, 255, "[%s] %s", device->addr, device->alias);
	else snprintf(buf, 255, "[%s] (unknown)", device->addr); 
	
	Evas_Object *ic;
	ic = elm_icon_add(DL->li);
	elm_icon_standard_set(ic, "arrow_left");
    elm_icon_scale_set(ic, 0, 1);
    elm_list_item_append(DL->li, buf, ic, NULL,  NULL, device);
	evas_object_show(ic);
	
	elm_list_go(DL->li);
}


void gui_device_list_populate() {
	
	fprintf(stderr, "Populating list with RemoteDevices already catched...\n");
	
	char buf[255];
	
	Evas_Object *ic;
	RemoteDevice* device;
	Eina_List* cur;
	
	EINA_LIST_FOREACH(DL->devices, cur, device) {
		if(!device->valid) continue;
		
		if(device->path) snprintf(buf, 255, "[%s] %s", device->addr, device->alias);
		else snprintf(buf, 255, "[%s] (unknown)", device->addr); 
		ic = elm_icon_add(DL->li);
        elm_icon_standard_set(ic, "arrow_left");
        elm_icon_scale_set(ic, 0, 1);
		elm_list_item_append(DL->li, buf, ic, NULL,  NULL, device);
		evas_object_show(ic);
	}
	elm_list_go(DL->li);
}


/*
void gui_device_list_clear(Evas_Object *li) {

fprintf(stderr, "\nClearing list of RemoteDevices...\n");
	const Eina_List *items;
	/ *
	while ((items = elm_list_items_get(li))) {
        elm_list_item_del(items->data);
     } * /


	elm_list_clear(li);
	
} */


void gui_settings_dialog_create() {

	char buf[255];

   Evas_Object *win, *bg, *vbox, *sc, *vbox_in, *vbox_fr, *fr, *lb, *hbox, *bt, *entry, *tg;

   win = elm_win_add(NULL, "settings_dialog", ELM_WIN_BASIC);
   elm_win_title_set(win, "emtooth - Settings");
   elm_win_autodel_set(win, TRUE);

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, 1.0, 1.0);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);
   
   	evas_object_resize(win, DISPLAY_WIDTH, DISPLAY_HEIGHT);
   	   
   	//add vbox
	vbox = elm_box_add(win);
	elm_win_resize_object_add(win, vbox);
	evas_object_size_hint_weight_set(vbox, 1.0, 1.0);
	evas_object_show(vbox);
	
	//HERE STARTS ALL THE OPTIONS LIST:
	
	
   	sc = elm_scroller_add(win);
   	evas_object_size_hint_weight_set(sc, 1, 1);
	evas_object_size_hint_align_set(sc, -1, -1);
	elm_scroller_bounce_set(sc, 0 ,1);
	elm_box_pack_end(vbox, sc);
	evas_object_show(sc);	

	
	vbox_in = elm_box_add(win);
	evas_object_size_hint_align_set(vbox_in, -1.0, -1.0);
	evas_object_size_hint_weight_set(vbox_in, 1.0, 1.0);
	elm_scroller_content_set(sc, vbox_in);
	evas_object_show(vbox_in);
	
	//ADDRESS	
	// add a frame
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "outdent_top");
	evas_object_size_hint_weight_set(fr, 0.0, 0.0);
	evas_object_size_hint_align_set(fr, 0.5, 0.5);
	elm_box_pack_end(vbox_in, fr);
	evas_object_show(fr);

	sprintf(buf, "<b>Address:</b> %s", ADAPTER->addr);
	lb = elm_label_add(win);
	elm_label_label_set(lb, buf);
	elm_frame_content_set(fr, lb);
	evas_object_show(lb);

	// NAME:
	// add a frame
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "default");
	evas_object_size_hint_weight_set(fr, 1.0, 0.0);
	evas_object_size_hint_align_set(fr, -1.0, -1.0);
	elm_box_pack_end(vbox_in, fr);
	evas_object_show(fr);
	
	hbox = elm_box_add(win);
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox, -1.0, 0.0);
	elm_frame_content_set(fr, hbox);
	evas_object_show(hbox);

	lb = elm_label_add(win);
	elm_label_label_set(lb, "<b>Name:</b> ");
	elm_box_pack_end(hbox, lb);
	evas_object_show(lb);
	
	entry = elm_entry_add(win);
	elm_entry_single_line_set(entry, TRUE);
	elm_entry_entry_set(entry, ADAPTER->name);
	elm_box_pack_end(hbox, entry);
	evas_object_show(entry);
	evas_object_smart_callback_add(entry, "changed", cb_entry_value_string_changed,
									init_cb_struct("Name", NULL, NULL));
	
	// DISCOVERABLE TOGGLE + TIMEOUT:
	// add a frame
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "default");
	evas_object_size_hint_weight_set(fr, 1.0, 0.0);
	evas_object_size_hint_align_set(fr, -1.0, -1.0);
	elm_box_pack_end(vbox_in, fr);
	evas_object_show(fr);
	
	vbox_fr = elm_box_add(win);
	evas_object_size_hint_align_set(vbox_fr, -1.0, 0.0);
	evas_object_size_hint_weight_set(vbox_fr, 1.0, 1.0);
	elm_frame_content_set(fr, vbox_fr);
	evas_object_show(vbox_in);
	
	hbox = elm_box_add(win);
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox, -1.0, 0.0);
	elm_box_pack_end(vbox_fr, hbox);
	evas_object_show(hbox);
	
	lb = elm_label_add(win);
	elm_label_label_set(lb, "<b>Discoverable:</b> ");
	elm_box_pack_end(hbox, lb);
	evas_object_show(lb);
	
	tg = elm_toggle_add(win);
	elm_toggle_states_labels_set(tg, "On", "Off");
	elm_toggle_state_set(tg, ADAPTER->discoverable);
	elm_box_pack_end(hbox, tg);
	evas_object_show(tg);
	evas_object_smart_callback_add(tg, "changed", cb_toggle_value_changed,
								init_cb_struct("Discoverable", NULL, NULL));
	
	//endl
	
	hbox = elm_box_add(win);
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox, -1.0, 0.0);
	elm_box_pack_end(vbox_fr, hbox);
	evas_object_show(hbox);
	
	lb = elm_label_add(win);
	elm_label_label_set(lb, "<b>Discoverable timeout:</b> ");
	elm_box_pack_end(hbox, lb);
	evas_object_show(lb);
	
	entry = elm_entry_add(win);
	elm_entry_single_line_set(entry, TRUE);
	sprintf(buf, "%d", ADAPTER->discoverable_timeout);
	elm_entry_entry_set(entry, buf);
	elm_box_pack_end(hbox, entry);
	evas_object_show(entry);
	evas_object_smart_callback_add(entry, "changed", cb_entry_value_integer_changed, 
										init_cb_struct("DiscoverableTimeout", NULL, NULL));
	
	
	// PAIRABLE TOGGLE + TIMEOUT:
	// add a frame
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "default");
	evas_object_size_hint_weight_set(fr, 1.0, 0.0);
	evas_object_size_hint_align_set(fr, -1.0, -1.0);
	elm_box_pack_end(vbox_in, fr);
	evas_object_show(fr);
	
	vbox_fr = elm_box_add(win);
	evas_object_size_hint_align_set(vbox_fr, -1.0, 0.0);
	evas_object_size_hint_weight_set(vbox_fr, 1.0, 1.0);
	elm_frame_content_set(fr, vbox_fr);
	evas_object_show(vbox_in);
	
	hbox = elm_box_add(win);
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox, -1.0, 0.0);
	elm_box_pack_end(vbox_fr, hbox);
	evas_object_show(hbox);
	
	lb = elm_label_add(win);
	elm_label_label_set(lb, "<b>Pairable: </b>");
	elm_box_pack_end(hbox, lb);
	evas_object_show(lb);
	
	tg = elm_toggle_add(win);
	elm_toggle_states_labels_set(tg, "On", "Off");
		elm_toggle_state_set(tg, ADAPTER->pairable);
	elm_box_pack_end(hbox, tg);
	evas_object_show(tg);
	evas_object_smart_callback_add(tg, "changed", cb_toggle_value_changed,
										init_cb_struct("Pairable", NULL, NULL));

	//endl
	
	hbox = elm_box_add(win);
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox, -1.0, 0.0);
	elm_box_pack_end(vbox_fr, hbox);
	evas_object_show(hbox);
	
	lb = elm_label_add(win);
	elm_label_label_set(lb, "<b>Pairable timeout:</b> ");
	elm_box_pack_end(hbox, lb);
	evas_object_show(lb);
	
	entry = elm_entry_add(win);
	elm_entry_single_line_set(entry, TRUE);
	sprintf(buf, "%d", ADAPTER->pairable_timeout);
	elm_entry_entry_set(entry, buf);
	elm_box_pack_end(hbox, entry);
	evas_object_show(entry);
	evas_object_smart_callback_add(entry, "changed", cb_entry_value_integer_changed,
									init_cb_struct("PairableTimeout", NULL, NULL));
	
	//BOTTOM:
	// add button hbox
	hbox = elm_box_add(win);
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox, -1.0, 0.0);
	elm_box_pack_end(vbox, hbox);
	evas_object_show(hbox);

	//add buttons to hbox
	bt = elm_button_add(win);
	elm_button_label_set(bt, "Close");
	evas_object_size_hint_weight_set(bt, 1.0, 1.0);
	evas_object_size_hint_align_set(bt, -1.0, -1.0);
	elm_box_pack_end(hbox, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", cb_close_win, win);
  
   evas_object_show(win);
	
}



void gui_remote_device_info_create(RemoteDevice* device) {

	char buf[255];

   Evas_Object *win, *bg, *vbox, *sc, *vbox_in, *vbox_fr, *fr, *lb, *hbox, *bt, *entry, *tg;

   snprintf(buf, 255, "emtooth - %s", device->name);
   win = elm_win_add(NULL, "remote_device", ELM_WIN_BASIC);
   elm_win_title_set(win, buf);
   elm_win_autodel_set(win, TRUE);

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, 1.0, 1.0);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);
   
   	evas_object_resize(win, DISPLAY_WIDTH, DISPLAY_HEIGHT);
   
   	//add vbox
	vbox = elm_box_add(win);
	elm_win_resize_object_add(win, vbox);
	evas_object_size_hint_weight_set(vbox, 1.0, 1.0);
	evas_object_show(vbox);
	
	//HERE STARTS ALL THE OPTIONS LIST:
	
	
   	sc = elm_scroller_add(win);
   	evas_object_size_hint_weight_set(sc, 1, 1);
	evas_object_size_hint_align_set(sc, -1, -1);
	elm_scroller_bounce_set(sc, 0 ,1);
	elm_box_pack_end(vbox, sc);
	evas_object_show(sc);	

	
	vbox_in = elm_box_add(win);
	evas_object_size_hint_align_set(vbox_in, -1.0, -1.0);
	evas_object_size_hint_weight_set(vbox_in, 1.0, 1.0);
	elm_scroller_content_set(sc, vbox_in);
	evas_object_show(vbox_in);
	
	//ADDRESS:
	// add a frame
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "outdent_top");
	evas_object_size_hint_weight_set(fr, 0.0, 0.0);
	evas_object_size_hint_align_set(fr, 0.5, 0.5);
	elm_box_pack_end(vbox_in, fr);
	evas_object_show(fr);
	
	vbox_fr = elm_box_add(win);
	evas_object_size_hint_align_set(vbox_fr, -1.0, -1.0);
	evas_object_size_hint_weight_set(vbox_fr, 1.0, 1.0);
	elm_frame_content_set(fr, vbox_fr);
	evas_object_show(vbox_fr);

	snprintf(buf, 255, "<b>MAC Adress:</b> %s", device->addr);
	lb = elm_label_add(win);
	elm_label_label_set(lb, buf);
	elm_box_pack_end(vbox_fr, lb);
	evas_object_show(lb);
	
	snprintf(buf, 255, "<b>Name:</b> %s", device->name);
	lb = elm_label_add(win);
	elm_label_label_set(lb, buf);
	elm_box_pack_end(vbox_fr, lb);
	evas_object_show(lb);

	// ALIAS:
	// add a frame
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "default");
	evas_object_size_hint_weight_set(fr, 1.0, 0.0);
	evas_object_size_hint_align_set(fr, -1.0, -1.0);
	elm_box_pack_end(vbox_in, fr);
	evas_object_show(fr);
	
	hbox = elm_box_add(win);
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox, -1.0, 0.0);
	elm_frame_content_set(fr, hbox);
	evas_object_show(hbox);

	lb = elm_label_add(win);
	elm_label_label_set(lb, "<b>Alias:</b> ");
	elm_box_pack_end(hbox, lb);
	evas_object_show(lb);
	
	entry = elm_entry_add(win);
	elm_entry_single_line_set(entry, TRUE);
	elm_entry_entry_set(entry, device->alias);
	elm_box_pack_end(hbox, entry);
	evas_object_show(entry);
	evas_object_smart_callback_add(entry, "changed", cb_entry_value_string_changed,
									init_cb_struct("Alias", device->path, "org.bluez.Device"));
										
	// CONNECTED TOGGLE
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "default");
	evas_object_size_hint_weight_set(fr, 1.0, 0.0);
	evas_object_size_hint_align_set(fr, -1.0, -1.0);
	elm_box_pack_end(vbox_in, fr);
	evas_object_show(fr);
	
	
	hbox = elm_box_add(win);
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox, -1.0, 0.0);
	elm_frame_content_set(fr, hbox);
	evas_object_show(hbox);
	
	lb = elm_label_add(win);
	elm_label_label_set(lb, "<b>Connected:</b> ");
	elm_box_pack_end(hbox, lb);
	evas_object_show(lb);
	
	tg = elm_toggle_add(win);
	elm_toggle_states_labels_set(tg, "Yes", "No");
	elm_toggle_state_set(tg, device->connected_device);
	elm_box_pack_end(hbox, tg);
	elm_object_disabled_set(tg, TRUE);
	evas_object_show(tg);
	//TODO: callback. on activate -> connect
	
	
	// PAIRED TOGGLE
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "default");
	evas_object_size_hint_weight_set(fr, 1.0, 0.0);
	evas_object_size_hint_align_set(fr, -1.0, -1.0);
	elm_box_pack_end(vbox_in, fr);
	evas_object_show(fr);
	
	hbox = elm_box_add(win);
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox, -1.0, 0.0);
	elm_frame_content_set(fr, hbox);
	evas_object_show(hbox);
	
	lb = elm_label_add(win);
	elm_label_label_set(lb, "<b>Paired:</b> ");
	elm_box_pack_end(hbox, lb);
	evas_object_show(lb);
	
	tg = elm_toggle_add(win);
	elm_toggle_states_labels_set(tg, "Yes", "No");
	elm_toggle_state_set(tg, device->paired);
	elm_object_disabled_set(tg, device->paired);
	elm_box_pack_end(hbox, tg);
	evas_object_show(tg);
	//TODO: callback. on activate -> pair
	evas_object_smart_callback_add(tg, "changed", cb_remote_paired_changed,
									device);
	
	// TRUSTED TOGGLE
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "default");
	evas_object_size_hint_weight_set(fr, 1.0, 0.0);
	evas_object_size_hint_align_set(fr, -1.0, -1.0);
	elm_box_pack_end(vbox_in, fr);
	evas_object_show(fr);
	
	
	hbox = elm_box_add(win);
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox, -1.0, 0.0);
	elm_frame_content_set(fr, hbox);
	evas_object_show(hbox);
	
	lb = elm_label_add(win);
	elm_label_label_set(lb, "<b>Trusted:</b> ");
	elm_box_pack_end(hbox, lb);
	evas_object_show(lb);
	
	tg = elm_toggle_add(win);
	elm_toggle_states_labels_set(tg, "Yes", "No");
	elm_toggle_state_set(tg, device->trusted);
	elm_box_pack_end(hbox, tg);
	evas_object_show(tg);
	evas_object_smart_callback_add(tg, "changed", cb_toggle_value_changed,
										init_cb_struct("Trusted", device->path, "org.bluez.Device"));
										
	//AUDIO TOGGLE:
	if(bluez_remote_device_has_service_audio(device)) {
		fr = elm_frame_add(win);
		elm_object_style_set(fr, "default");
		evas_object_size_hint_weight_set(fr, 1.0, 0.0);
		evas_object_size_hint_align_set(fr, -1.0, -1.0);
		elm_box_pack_end(vbox_in, fr);
		evas_object_show(fr);
		
		
		hbox = elm_box_add(win);
		elm_box_horizontal_set(hbox, 1);
		evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
		evas_object_size_hint_align_set(hbox, -1.0, 0.0);
		elm_frame_content_set(fr, hbox);
		evas_object_show(hbox);
		
		lb = elm_label_add(win);
		elm_label_label_set(lb, "<b>Connect Audio:</b> ");
		elm_box_pack_end(hbox, lb);
		evas_object_show(lb);
		
		tg = elm_toggle_add(win);
		elm_toggle_states_labels_set(tg, "Yes", "No");
		elm_toggle_state_set(tg, device->connected_audio);
		elm_box_pack_end(hbox, tg);
		evas_object_show(tg);
		evas_object_smart_callback_add(tg, "changed", cb_toggle_audio_connect,
										device);
	}
	
	// InputConnect TOGGLE
	if(bluez_remote_device_has_service_input(device)) {
		fr = elm_frame_add(win);
		elm_object_style_set(fr, "default");
		evas_object_size_hint_weight_set(fr, 1.0, 0.0);
		evas_object_size_hint_align_set(fr, -1.0, -1.0);
		elm_box_pack_end(vbox_in, fr);
		evas_object_show(fr);
		
		
		hbox = elm_box_add(win);
		elm_box_horizontal_set(hbox, 1);
		evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
		evas_object_size_hint_align_set(hbox, -1.0, 0.0);
		elm_frame_content_set(fr, hbox);
		evas_object_show(hbox);
		
		lb = elm_label_add(win);
		elm_label_label_set(lb, "<b>Connect Input:</b> ");
		elm_box_pack_end(hbox, lb);
		evas_object_show(lb);
		
		tg = elm_toggle_add(win);
		elm_toggle_states_labels_set(tg, "Yes", "No");
		elm_toggle_state_set(tg, device->connected_input);
		elm_box_pack_end(hbox, tg);
		evas_object_show(tg);
		evas_object_smart_callback_add(tg, "changed", cb_toggle_input_connect,
										device);
	}	
	
	//add SendFiles button.
	bt = elm_button_add(win);
	elm_button_label_set(bt, "Send Files [NOT IMPLEMENTED]");
	evas_object_size_hint_weight_set(bt, 0, 0);
	evas_object_size_hint_align_set(bt, -1.0, -1.0);
	elm_box_pack_end(vbox_in, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", cb_send_files_clicked, device);
		
	
	//add Remove Button
	bt = elm_button_add(win);
	elm_button_label_set(bt, "Reset/Remove Device");
	evas_object_size_hint_weight_set(bt, 0, 0);
	evas_object_size_hint_align_set(bt, -1.0, -1.0);
	elm_box_pack_end(vbox_in, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", cb_close_win, win);
	evas_object_smart_callback_add(bt, "clicked", cb_remove_device_clicked, device);
							
	//BOTTOM:
	// add button hbox
	hbox = elm_box_add(win);
	elm_box_horizontal_set(hbox, 1);
	evas_object_size_hint_weight_set(hbox, 1.0, 0.0);
	evas_object_size_hint_align_set(hbox, -1.0, 0.0);
	elm_box_pack_end(vbox, hbox);
	evas_object_show(hbox);

	//add buttons to hbox
	bt = elm_button_add(win);
	elm_button_label_set(bt, "Close");
	evas_object_size_hint_weight_set(bt, 1.0, 1.0);
	evas_object_size_hint_align_set(bt, -1.0, -1.0);
	elm_box_pack_end(hbox, bt);
	evas_object_show(bt);
	evas_object_smart_callback_add(bt, "clicked", cb_close_win, win);
  
   evas_object_show(win);
	
}



void gui_request_file_path_create(RemoteDevice* device, Evas_Smart_Cb cb_file_selector_done) {
	
	Evas_Object *win, *fs, *bg, *vbox;
	
	char buf[255];
	snprintf(buf, 255, "Select File - %s", device->name);
	win = elm_win_add(NULL, "remote_device_file_selector", ELM_WIN_BASIC);
	elm_win_title_set(win, buf);
	elm_win_autodel_set(win, TRUE);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	evas_object_resize(win, DISPLAY_WIDTH, DISPLAY_HEIGHT);

	//add vbox
	vbox = elm_box_add(win);
	elm_win_resize_object_add(win, vbox);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(vbox);
	
	fs = elm_fileselector_add(win);
   /* enable the fs file name entry */
   elm_fileselector_is_save_set(fs, EINA_TRUE);
   /* make the file list a tree with dir expandable in place */
   elm_fileselector_expandable_set(fs, EINA_FALSE);
   /* start the fileselector in the home dir */
   elm_fileselector_path_set(fs, getenv("HOME"));
   /* allow fs to expand in x & y */
   evas_object_size_hint_weight_set(fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fs, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(vbox, fs);
   evas_object_show(fs);
	
	evas_object_smart_callback_add(fs, "done", cb_file_selector_done, device);
	
	 evas_object_show(win);
}


void gui_request_pin_create(RemoteDevice* device)
{
	Evas_Object *win, *bg, *inwin, *vbox, *lb, *entry, *bt1;
	
	char buf[255];
	snprintf(buf, 254, "pin-%s", device->addr);
	win = elm_win_add(NULL, buf, ELM_WIN_DIALOG_BASIC);
	elm_win_title_set(win, "Request Pin:");
	elm_win_autodel_set(win, 1);

	bg = elm_bg_add(win);
	elm_win_resize_object_add(win, bg);
	evas_object_size_hint_weight_set(bg, 1.0, 1.0);
	evas_object_show(bg);

	inwin = elm_win_inwin_add(win);
	evas_object_show(inwin);

	vbox = elm_box_add(win);
	elm_win_inwin_content_set(inwin, vbox);
	evas_object_show(vbox);
	
	snprintf(buf, 254, "Set the password for device %s<br>and press the button below to proceed:", device->addr);
	lb = elm_label_add(win);
	elm_label_label_set(lb, buf);
	evas_object_size_hint_weight_set(bt1, 1.0, 1.0);
	evas_object_size_hint_align_set(bt1, -1.0, -1.0);
	elm_box_pack_end(vbox, lb);
	evas_object_show(lb);
	
	entry = elm_entry_add(win);
	elm_entry_single_line_set(entry, TRUE);
	elm_entry_entry_set(entry, "1234");
	elm_box_pack_end(vbox, entry);
	evas_object_show(entry);
	
	//add close button
	bt1 = elm_button_add(win);
	elm_button_label_set(bt1, "Ok");
	evas_object_size_hint_weight_set(bt1, 1.0, 1.0);
	evas_object_size_hint_align_set(bt1, -1.0, -1.0);
	elm_box_pack_end(vbox, bt1);
	evas_object_show(bt1);
	
	DialogCb* cb = (DialogCb*) malloc(sizeof(DialogCb));
	cb->device = device;
	cb->entry = entry;
	cb->win = win;
	evas_object_smart_callback_add(bt1, "clicked", cb_request_pin, cb);
	
	evas_object_show(win);
}



Evas_Object* gui_alert_create(const char *message)
{
	Evas_Object *win, *bg, *inwin, *vbox, *lb, *bt1;

	win = elm_win_add(NULL, "inwin", ELM_WIN_DIALOG_BASIC);
	elm_win_title_set(win, "Alert!");
	elm_win_autodel_set(win, 1);

	bg = elm_bg_add(win);
	elm_win_resize_object_add(win, bg);
	evas_object_size_hint_weight_set(bg, 1.0, 1.0);
	evas_object_show(bg);

	inwin = elm_win_inwin_add(win);
	evas_object_show(inwin);

	vbox = elm_box_add(win);
	elm_win_inwin_content_set(inwin, vbox);
	evas_object_show(vbox);
	
	lb = elm_label_add(win);
	elm_label_label_set(lb,message);
	evas_object_size_hint_weight_set(bt1, 1.0, 1.0);
	evas_object_size_hint_align_set(bt1, -1.0, -1.0);
	elm_box_pack_end(vbox, lb);
	evas_object_show(lb);
	
	//add close button
	bt1 = elm_button_add(win);
	elm_button_label_set(bt1, "Ok");
	evas_object_size_hint_weight_set(bt1, 1.0, 1.0);
	evas_object_size_hint_align_set(bt1, -1.0, -1.0);
	elm_box_pack_end(vbox, bt1);
	evas_object_show(bt1);
	evas_object_smart_callback_add(bt1, "clicked", cb_close_win, win);
	
	evas_object_show(win);
	return win;
}
