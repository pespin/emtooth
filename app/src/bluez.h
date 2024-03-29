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
#include "cb_bluez.h"
#include "dbus.h"

void bluez_wait_until_service_is_up(Evas_Object* dialog);
void bluez_get_default_adapter();

void bluez_get_remote_device_path(RemoteDevice* device);
void bluez_create_remote_device_path(RemoteDevice* device);

void bluez_create_remote_paired_device(RemoteDevice* device);
void bluez_adapter_remove_remote_device(RemoteDevice* device);

void bluez_get_local_device_info();
void bluez_get_remote_device_info(RemoteDevice* device);

void bluez_get_remote_device_properties_device(RemoteDevice* device);
void bluez_get_remote_device_properties_input(RemoteDevice* device);
void bluez_get_remote_device_properties_audio(RemoteDevice* device);

bool bluez_remote_device_has_service_input(RemoteDevice* device);
bool bluez_remote_device_has_service_audio(RemoteDevice* device);

void bluez_remote_device_input_connect(RemoteDevice* device);
void bluez_remote_device_input_disconnect(RemoteDevice* device);

void bluez_remote_device_audio_connect(RemoteDevice* device);
void bluez_remote_device_audio_disconnect(RemoteDevice* device);

void bluez_remote_device_attach_signals(RemoteDevice* device);

int bluez_remote_device_list_count();

void bluez_register_agent(const char *capabilities);

void bluez_discovery_start();
void bluez_discovery_stop();

void bluez_set_property(DictEntry* info, const char* path, const char* interface);
