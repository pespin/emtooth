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
#include "defines.h"

void cb_alert_bt(void *data, Evas_Object *obj, void *event_info);

void cb_safe_exit(void *data, Evas_Object *obj, void *event_info);
void cb_close_win(void *data, Evas_Object *obj, void *event_info);

void cb_discovery_start_clicked(void *data, Evas_Object *obj, void *event_info);
void cb_discovery_stop_clicked(void *data, Evas_Object *obj, void *event_info);
void cb_device_list_selected(void *data, Evas_Object *obj, void *event_info);

void cb_settings_dialog(void *data, Evas_Object *obj, void *event_info);

void cb_entry_value_integer_changed(void *data, Evas_Object *obj, void *event_info);
void cb_entry_value_string_changed(void *data, Evas_Object *obj, void *event_info);
void cb_toggle_value_changed(void *data, Evas_Object *obj, void *event_info);

void cb_remote_paired_changed(void *data, Evas_Object *obj, void *event_info);
void cb_toggle_input_connect(void *data, Evas_Object *obj, void *event_info);
void cb_toggle_audio_connect(void *data, Evas_Object *obj, void *event_info);

void cb_request_pin(void *data, Evas_Object *obj, void *event_info);
