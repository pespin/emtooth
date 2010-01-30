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

#ifndef _EMTOOTH_DEFINES_H
#define _EMTOOTH_DEFINES_H       1

/* defines */

#define DBUSLOG(error) if (error && dbus_error_is_set(error)) fprintf(stderr, "Error: %s - %s\n", error->name, error->message);


/* STRUCTS-typedefs */

typedef int bool;

typedef struct _DeviceList {
	Eina_List* devices;
	Evas_Object* li;
	Evas_Object* header;
} DeviceList;

typedef union _DbusReturn {
	char* value_string;
	int value_int;
} DbusReturn;

typedef struct _StructDbus {
	char* key;
	int value_type;
	DbusReturn value;
} StructDbus;

typedef struct _RemoteDevice {
	char* path;
	char* addr;
	char* name;
	int class;
	bool connected;
	char* icon;
	char* alias;
	bool paired;
	bool trusted;
	char* password;
} RemoteDevice;




typedef struct _LocalDevice {
	char* path;
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
	E_DBus_Connection* sysconn;
	E_DBus_Connection* sessionconn;
	E_DBus_Signal_Handler* DeviceFound;
	E_DBus_Signal_Handler* DeviceDissapeared;
} DbusConn;



typedef struct _ObjCb {
	Evas_Object* obj;
	Evas_Object* parent;
} ObjCb;

typedef struct _GuiCb {
	char* property;
	char* path;
	char* interface;
} GuiCb;

typedef struct _DialogCb {
	RemoteDevice* device;
	Evas_Object* entry;
	Evas_Object* win;
} DialogCb;



/* GLOBAL VARIABLES */

int bluez_error_counter; //times tried to acces bluez (while fso still loading)

LocalDevice* ADAPTER;
DbusConn* DBUSCONN;
DeviceList* DL;

char* BLUEZ_AGENT_PATH;



/* misc functions in defines.c */
GuiCb* init_cb_struct(const char* property, const char* path);


#endif
