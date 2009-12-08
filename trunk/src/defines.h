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
#include <E_DBus.h>

typedef int bool;

typedef struct _DeviceList {
	Eina_List* devices;
	Evas_Object* li;
} DeviceList;

typedef struct _DeviceListCb {
	DeviceList* DL;
	Evas_Object* obj;
	Evas_Object* parent;
} DeviceListCb;

typedef struct _RemoteDevice {
	char* addr;
	char* name;
	int enabled;
} RemoteDevice;


typedef struct _LocalDevice {
	char* addr;
	int class;
	bool discoverable;
	int discoverable_timeout;
	bool discovering;
	char* name;
	bool pairable;
	int pairable_timeout;
	bool powered;
} LocalDevice;


typedef struct _DbusConn {
	char* path;
	E_DBus_Connection* conn;
	E_DBus_Signal_Handler* DeviceFound;
	E_DBus_Signal_Handler* DeviceDissapeared;
} DbusConn;


//char* BLUEZPATH;
//E_DBus_Connection* DBUSCONN;

LocalDevice* ADAPTER;
DbusConn* DBUSCONN;