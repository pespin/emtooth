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

void gui_create() {

   Evas_Object *win, *bg, *vbox, *fr, *header, *device_list, *hbox, *hbox1, *bt, *bt_start, *bt_stop;

	ObjCb* Cb;

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
	header = elm_label_add(win);
	elm_label_label_set(header, "Discovering Devices...");
	elm_frame_content_set(fr, header);
	evas_object_show(header);  
	
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

	DL->li = device_list;
	DL->header = header;	
   gui_device_list_populate();
  
   evas_object_show(win);
	
}


void gui_device_list_remove(RemoteDevice* device) {
	fprintf(stderr, "Removing RemoteDevice %s from list...\n", device->addr);
	
	DL->devices = eina_list_remove(DL->devices, device);
	
	/* TODO: just remove one item instead of rewriting all the list */
	elm_list_clear(DL->li);
	gui_device_list_populate();
	
}


void gui_device_list_append(RemoteDevice* device) {

	fprintf(stderr, "Adding RemoteDevice %s to list...\n", device->addr);
	
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

   Evas_Object *win, *bg, *vbox, *vbox_in, *vbox_fr, *fr, *lb, *hbox, *bt, *entry, *tg;

   win = elm_win_add(NULL, "settings_dialog", ELM_WIN_BASIC);
   elm_win_title_set(win, "emtooth - Settings");
   elm_win_autodel_set(win, TRUE);
   //evas_object_smart_callback_add(win, "delete,request", cb_close_win, win);

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, 1.0, 1.0);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);
   
   	evas_object_resize(win, 480, 600);	
   
   	//add vbox
	vbox = elm_box_add(win);
	elm_win_resize_object_add(win, vbox);
	evas_object_size_hint_weight_set(vbox, 1.0, 1.0);
	evas_object_show(vbox);

	// add a frame
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "outdent_top");
	evas_object_size_hint_weight_set(fr, 0.0, 0.0);
	evas_object_size_hint_align_set(fr, 0.5, 0.5);
	elm_box_pack_end(vbox, fr);
	evas_object_show(fr);

	//ADDRESS
	sprintf(buf, "<b>Address:</b> %s", ADAPTER->addr);
	lb = elm_label_add(win);
	elm_label_label_set(lb, buf);
	elm_frame_content_set(fr, lb);
	evas_object_show(lb);
	
	//HERE STARTS ALL THE OPTIONS LIST:
	
	vbox_in = elm_box_add(win);
	elm_win_resize_object_add(win, vbox_in);
	evas_object_size_hint_align_set(vbox_in, -1.0, 0.0);
	evas_object_size_hint_weight_set(vbox_in, 1.0, 1.0);
	elm_box_pack_end(vbox, vbox_in);
	evas_object_show(vbox_in);

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
	/* TODO: callback that saves modified name on unfocus: entry_changed signal */
	
	// DISCOVERABLE TOGGLE + TIMEOUT:
	// add a frame
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "default");
	evas_object_size_hint_weight_set(fr, 1.0, 0.0);
	evas_object_size_hint_align_set(fr, -1.0, -1.0);
	elm_box_pack_end(vbox_in, fr);
	evas_object_show(fr);
	
	vbox_fr = elm_box_add(win);
	elm_win_resize_object_add(win, vbox_fr);
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
	evas_object_smart_callback_add(tg, "changed", cb_discoverable_changed, NULL);
	
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
	/* TODO: callback that saves modified name on unfocus: entry_changed signal  */
	
	
	// PAIRABLE TOGGLE + TIMEOUT:
	// add a frame
	fr = elm_frame_add(win);
	elm_object_style_set(fr, "default");
	evas_object_size_hint_weight_set(fr, 1.0, 0.0);
	evas_object_size_hint_align_set(fr, -1.0, -1.0);
	elm_box_pack_end(vbox_in, fr);
	evas_object_show(fr);
	
	vbox_fr = elm_box_add(win);
	elm_win_resize_object_add(win, vbox_fr);
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
	evas_object_smart_callback_add(tg, "changed", cb_pairable_changed, NULL);
	
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
	/* TODO: callback that saves modified name on unfocus: entry_changed signal */
	
	
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



void gui_alert_create(const char *message)
{
	Evas_Object *win, *bg, *inwin, *lb, *bt1;

	win = elm_win_add(NULL, "inwin", ELM_WIN_DIALOG_BASIC);
	elm_win_title_set(win, "Alert!");
	elm_win_autodel_set(win, 1);

	bg = elm_bg_add(win);
	elm_win_resize_object_add(win, bg);
	evas_object_size_hint_weight_set(bg, 1.0, 1.0);
	evas_object_show(bg);

	inwin = elm_win_inwin_add(win);
	elm_object_style_set(inwin, "minimal_vertical");
	evas_object_show(inwin);

	lb = elm_label_add(win);
	elm_label_label_set(lb,message);
	elm_win_inwin_content_set(inwin, lb);
	evas_object_show(lb);
	
	//add close button
	bt1 = elm_button_add(win);
	elm_button_label_set(bt1, "Ok");
	evas_object_size_hint_weight_set(bt1, 1.0, 1.0);
	evas_object_size_hint_align_set(bt1, -1.0, -1.0);
	elm_box_pack_end(inwin, bt1);
	evas_object_show(bt1);
	evas_object_smart_callback_add(bt1, "clicked", cb_alert_bt, win);
	
	evas_object_resize(win, 320, 240);
	evas_object_show(win);
}
