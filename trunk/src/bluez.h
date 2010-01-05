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

void bluez_init_session();
void bluez_get_remote_device_path(RemoteDevice* device);
void bluez_create_remote_device_path(RemoteDevice* device);

void bluez_get_local_device_info();
void bluez_get_remote_device_info(RemoteDevice* device);

void bluez_discovery_start();
void bluez_discovery_stop();

void bluez_set_property(StructDbus* info, const char* path, const char* interface);
