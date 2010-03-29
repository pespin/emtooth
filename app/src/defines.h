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

/* DEFINES */

#define DISPLAY_WIDTH		480
#define DISPLAY_HEIGHT		600

#define BLUEZ_AGENT_PATH 	"/org/emtooth/bluez/agent"
#define BLUEZ_AGENT_IFACE	"org.bluez.Agent"

#define OBEX_AGENT_PATH 	"/org/emtooth/obex/agent"
#define OBEX_AGENT_IFACE	"org.openobex.Agent"


//Input:
#define HID_UUID			"00001124-0000-1000-8000-00805f9b34fb"

//Audio:
#define HSP_HS_UUID			"00001108-0000-1000-8000-00805f9b34fb"
#define HSP_AG_UUID			"00001112-0000-1000-8000-00805f9b34fb"
#define HFP_HS_UUID			"0000111e-0000-1000-8000-00805f9b34fb"
#define HFP_AG_UUID			"0000111f-0000-1000-8000-00805f9b34fb"
#define A2DP_SOURCE_UUID	"0000110a-0000-1000-8000-00805f9b34fb"
#define A2DP_SINK_UUID		"0000110b-0000-1000-8000-00805f9b34fb"
#define AVRCP_TARGET_UUID 	"0000110c-0000-1000-8000-00805f9b34fb"
		

#define DBUSLOG(error) if (error && dbus_error_is_set(error)) fprintf(stderr, "Error: %s - %s\n", error->name, error->message);


/* STRUCTS-typedefs */

typedef int bool;

typedef struct _DeviceList {
	Eina_List* devices;
	Evas_Object* li;
	Evas_Object* header;
} DeviceList;

typedef union _Variant {
	char* value_string;
	int value_int;
	char** value_array;
} Variant;

typedef struct _DictEntry {
	char* key;
	int value_type;
	Variant value;
} DictEntry;

typedef struct _RemoteDevice {
	//org.bluez.Device iface:
	char* path;
	char* addr;
	char* name;
	int class;
	bool connected_device;
	char* icon;
	char* alias;
	bool paired;
	bool trusted;
	char** UUIDs;
	E_DBus_Signal_Handler* signal_PropertyChanged_device;
	
	//org.bluez.Input iface:
	bool connected_input;
	E_DBus_Signal_Handler* signal_PropertyChanged_input;
	
	//org.bluez.Audio iface:
	bool connected_audio;
	E_DBus_Signal_Handler* signal_PropertyChanged_audio;
	
	//internal
	char* password;
	bool valid;
	
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
	E_DBus_Signal_Handler* DeviceFound;
	E_DBus_Signal_Handler* DeviceDissapeared;
	E_DBus_Signal_Handler* DeviceCreated;
	E_DBus_Signal_Handler* DeviceRemoved;
} LocalDevice;


typedef struct _DbusConn {
	E_DBus_Connection* sysconn;
	E_DBus_Connection* sessionconn;
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

/* misc functions in defines.c */
GuiCb* init_cb_struct(const char* property, const char* path, const char* iface);

RemoteDevice* remote_device_new(const char* addr);

bool remote_device_free(RemoteDevice* device);

bool dict_entry_free(DictEntry* ret);

bool array_free(char** array);

#endif
