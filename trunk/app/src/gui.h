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

#include "defines.h"
#include "cb_gui.h"

Evas_Object* gui_create();

void gui_device_list_remove(RemoteDevice* device);

void gui_device_list_append(RemoteDevice* device);

void gui_device_list_populate();

//void gui_device_list_clear(Evas_Object *li);

void gui_settings_dialog_create();

void gui_remote_device_info_create();

void gui_request_file_path_create(RemoteDevice* device, Evas_Smart_Cb cb_file_selector_done);

void gui_request_pin_create(RemoteDevice* device);

Evas_Object* gui_alert_create(const char *message);
