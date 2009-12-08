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
Foundation, Inc., 51 Franklin Stre et, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <E_DBus.h>
#include "cb_dbus.h"

void dbus_init_session(DeviceList* DL);
void dbus_get_remote_device_path(RemoteDevice* device);
void dbus_create_remote_device_path(RemoteDevice* device);

void dbus_get_local_device_info();
void dbus_get_remote_device_info(RemoteDevice* device);

void dbus_discovery_start(DeviceList* DL);

void dbus_discovery_stop(DeviceList* DL);
