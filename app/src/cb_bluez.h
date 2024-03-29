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
#include <E_DBus.h>
#include "defines.h"

void cb_bluez_wait_until_service_is_up(void *data, DBusMessage *replymsg, DBusError *error);

void cb_get_default_adapter(void *data, DBusMessage *replymsg, DBusError *error);
void cb_create_remote_device_path(void *data, DBusMessage *replymsg, DBusError *error);
void cb_get_remote_device_path(void *data, DBusMessage *replymsg, DBusError *error);

void cb_create_remote_paired_device(void *data, DBusMessage *replymsg, DBusError *error);

void cb_get_local_device_info (void *data, DBusMessage *replymsg, DBusError *error);
void cb_get_remote_device_properties_device(void *data, DBusMessage *replymsg, DBusError *error);
void cb_get_remote_device_properties_input(void *data, DBusMessage *replymsg, DBusError *error);
void cb_get_remote_device_properties_audio(void *data, DBusMessage *replymsg, DBusError *error);


/* signals */
void cb_device_disappeared(void *data, DBusMessage *msg);
void cb_device_found(void *data, DBusMessage *msg);
void cb_property_changed(void *data, DBusMessage *msg);
void cb_device_created(void *data, DBusMessage *msg);
void cb_device_removed(void *data, DBusMessage *msg);

