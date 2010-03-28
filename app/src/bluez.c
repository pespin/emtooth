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
#include "gui.h"
#include "cb_dbus.h"
#include "cb_bluez.h"
#include "bluez.h"

void bluez_get_default_adapter() {
	
	fprintf(stderr, "Getting default bluetooth adapter from bluez...\n");
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		"/",
		"org.bluez.Manager",
		"DefaultAdapter");
	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_get_default_adapter, -1, NULL);
	dbus_message_unref(msg);
		
}


void bluez_get_remote_device_path(RemoteDevice* device) {

	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		ADAPTER->path,
		"org.bluez.Adapter",
		"FindDevice");
			
	dbus_message_append_args (msg,
		DBUS_TYPE_STRING, &device->addr,
		DBUS_TYPE_INVALID);

	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_get_remote_device_path, -1, device);
	dbus_message_unref(msg);
	
}

void bluez_create_remote_device_path(RemoteDevice* device) {

	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		ADAPTER->path,
		"org.bluez.Adapter",
		"CreateDevice");
			
	dbus_message_append_args (msg,
		DBUS_TYPE_STRING, &device->addr,
		DBUS_TYPE_INVALID);

	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_create_remote_device_path, -1, device);
	dbus_message_unref(msg);
	
}


void bluez_create_remote_paired_device(RemoteDevice* device) {
	
	fprintf(stderr, "Pairing to device [%s][%s]...\n", device->addr, device->path);
	
	const char* capabilities = "";
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		ADAPTER->path,
		"org.bluez.Adapter",
		"CreatePairedDevice");
	
	dbus_message_append_args (msg,
		DBUS_TYPE_STRING, &device->addr,
		DBUS_TYPE_OBJECT_PATH, &device->path,
		DBUS_TYPE_STRING, &capabilities,
		DBUS_TYPE_INVALID);

	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_create_remote_paired_device, -1, device);
	dbus_message_unref(msg);
	
	
}

void bluez_adapter_remove_remote_device(RemoteDevice* device) {
	
	fprintf(stderr, "Removing remote device [%s][%s]...\n", device->addr, device->path);
	
	const char* capabilities = "";
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		ADAPTER->path,
		"org.bluez.Adapter",
		"RemoveDevice");
	
	dbus_message_append_args (msg,
		DBUS_TYPE_OBJECT_PATH, &device->path,
		DBUS_TYPE_INVALID);

	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_dbus_generic, -1, NULL);
	dbus_message_unref(msg);
	
}



void bluez_get_local_device_info() {

	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		ADAPTER->path,
		"org.bluez.Adapter",
		"GetProperties");
	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_get_local_device_info, -1, NULL);
	dbus_message_unref(msg);
	
}


void bluez_get_remote_device_info(RemoteDevice* device) {

	bluez_get_remote_device_properties_device(device);
	
	if(bluez_remote_device_has_service_audio(device))
		bluez_get_remote_device_properties_audio(device);
	
	if(bluez_remote_device_has_service_input(device))
		bluez_get_remote_device_properties_input(device);
	
}

void bluez_get_remote_device_properties_device(RemoteDevice* device) {
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		device->path,
		"org.bluez.Device",
		"GetProperties");
	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_get_remote_device_properties_device, -1, device);
	dbus_message_unref(msg);
	
	
}

void bluez_get_remote_device_properties_input(RemoteDevice* device) {

	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		device->path,
		"org.bluez.Input",
		"GetProperties");
	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_get_remote_device_properties_input, -1, device);
	dbus_message_unref(msg);
	
}


void bluez_get_remote_device_properties_audio(RemoteDevice* device) {

	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		device->path,
		"org.bluez.Audio",
		"GetProperties");
	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_get_remote_device_properties_audio, -1, device);
	dbus_message_unref(msg);
	
}

bool bluez_remote_device_has_service_input(RemoteDevice* device) {
	
	if(!device->UUIDs) { 
	/* This can happen when discovering a device, since we first 
	 * need to fetch data from Device iface to have UUIDs list*/
		fprintf(stderr, "UUIDs list for device [%s] is not set, waiting" \
		 " to display Input info...\n", device->addr);
		return FALSE;
	}
	int i = 0;
	while(device->UUIDs[i] != NULL) {
		if(!strcasecmp(device->UUIDs[i], HID_UUID)) return TRUE;
		i++;
	}
	return FALSE;
}

bool bluez_remote_device_has_service_audio(RemoteDevice* device) {
	
	if(!device->UUIDs) { 
	/* This can happen when discovering a device, since we first 
	 * need to fetch data from Device iface to have UUIDs list*/
		fprintf(stderr, "UUIDs list for device [%s] is not set, waiting" \
		 " to display Audio info...\n", device->addr);
		return FALSE;
	}
	int i = 0;
	while(device->UUIDs[i] != NULL) {
		if(!strcasecmp(device->UUIDs[i], HSP_HS_UUID) || 
			!strcasecmp(device->UUIDs[i], HSP_AG_UUID) ||
			!strcasecmp(device->UUIDs[i], HFP_HS_UUID) ||
			!strcasecmp(device->UUIDs[i], HFP_AG_UUID) ||
			!strcasecmp(device->UUIDs[i], A2DP_SOURCE_UUID) ||
			!strcasecmp(device->UUIDs[i], A2DP_SINK_UUID) ||
			!strcasecmp(device->UUIDs[i], AVRCP_TARGET_UUID)
		) {
			return TRUE;
		}

		i++;
	}
	return FALSE;
}


void bluez_remote_device_input_connect(RemoteDevice* device) {
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		device->path,
		"org.bluez.Input",
		"Connect");
	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_dbus_generic_remote_gui_alert, -1, device);
	dbus_message_unref(msg);
	
}

void bluez_remote_device_input_disconnect(RemoteDevice* device) {
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		device->path,
		"org.bluez.Input",
		"Disconnect");
	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_dbus_generic_remote_gui_alert, -1, device);
	dbus_message_unref(msg);
	
}


void bluez_remote_device_audio_connect(RemoteDevice* device) {
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		device->path,
		"org.bluez.Audio",
		"Connect");
	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_dbus_generic_remote_gui_alert, -1, device);
	dbus_message_unref(msg);
	
}

void bluez_remote_device_audio_disconnect(RemoteDevice* device) {
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		device->path,
		"org.bluez.Audio",
		"Disconnect");
	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_dbus_generic_remote_gui_alert, -1, device);
	dbus_message_unref(msg);
	
}

void bluez_remote_device_attach_signals(RemoteDevice* device) {
	
	//Connect to PropertyChanged signal:
	device->signal_PropertyChanged_device = 
	e_dbus_signal_handler_add(
	DBUSCONN->sysconn,
	"org.bluez", 
	device->path,
	"org.bluez.Device",
	"PropertyChanged",
	cb_property_changed,
	device);
	
		device->signal_PropertyChanged_input =
		e_dbus_signal_handler_add(
		DBUSCONN->sysconn,
		"org.bluez", 
		device->path,
		"org.bluez.Input",
		"PropertyChanged",
		cb_property_changed,
		device);
	
	
	//Connect to PropertyChanged signal:
	device->signal_PropertyChanged_audio = 
	e_dbus_signal_handler_add(
	DBUSCONN->sysconn,
	"org.bluez", 
	device->path,
	"org.bluez.Audio",
	"PropertyChanged",
	cb_property_changed,
	device);
}

int bluez_remote_device_list_count() {
	
	RemoteDevice* device;
	Eina_List* cur;
	
	int n = 0;
	EINA_LIST_FOREACH(DL->devices, cur, device) {
		if(device->valid) n++;
	}
	
	return n;
}


void bluez_register_agent(const char *capabilities) {
	
	fprintf(stderr, "Registering bluez agent...\n");
	
	DBusMessage *msg;

	msg = dbus_message_new_method_call(
	"org.bluez", 
	ADAPTER->path,
	"org.bluez.Adapter", 
	"RegisterAgent");

	char* tmp = strdup(BLUEZ_AGENT_PATH);
	dbus_message_append_args(msg, 
					DBUS_TYPE_OBJECT_PATH, &tmp,
					DBUS_TYPE_STRING, &capabilities,
					DBUS_TYPE_INVALID);
	free(tmp);
	
	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_dbus_generic, -1, NULL);
	
	dbus_message_unref(msg);

}



void bluez_discovery_start() {

/* now connect to signals and send StartDiscovery message */

	ADAPTER->DeviceFound = e_dbus_signal_handler_add(
    DBUSCONN->sysconn,
	"org.bluez",
	ADAPTER->path,
	"org.bluez.Adapter",
	"DeviceFound",
	cb_device_found,
	NULL);
	
	ADAPTER->DeviceDissapeared = e_dbus_signal_handler_add(
	DBUSCONN->sysconn,
	"org.bluez", 
	ADAPTER->path,
	"org.bluez.Adapter",
	"DeviceDisappeared",
	cb_device_disappeared,
	NULL);
	
	ADAPTER->DeviceCreated = e_dbus_signal_handler_add(
    DBUSCONN->sysconn,
	"org.bluez",
	ADAPTER->path,
	"org.bluez.Adapter",
	"DeviceCreated",
	cb_device_created,
	NULL);
	
	ADAPTER->DeviceRemoved = e_dbus_signal_handler_add(
	DBUSCONN->sysconn,
	"org.bluez", 
	ADAPTER->path,
	"org.bluez.Adapter",
	"DeviceRemoved",
	cb_device_removed,
	NULL);
	
	/* send start message */
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		ADAPTER->path,
		"org.bluez.Adapter",
		"StartDiscovery");
	
	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_dbus_generic, -1, NULL);
	dbus_message_unref(msg);

   //e_dbus_connection_close(conn); 
}


void bluez_discovery_stop() {
	
	/* disconnect from discovery signals, as other apps can be discovering too */
	e_dbus_signal_handler_del(DBUSCONN->sysconn, ADAPTER->DeviceFound);
	e_dbus_signal_handler_del(DBUSCONN->sysconn, ADAPTER->DeviceDissapeared);
	e_dbus_signal_handler_del(DBUSCONN->sysconn, ADAPTER->DeviceCreated);
	e_dbus_signal_handler_del(DBUSCONN->sysconn, ADAPTER->DeviceRemoved);
	
	/* send stop message */
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		ADAPTER->path,
		"org.bluez.Adapter",
		"StopDiscovery");
	
	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_dbus_generic, -1, NULL);
	dbus_message_unref(msg);
}


void bluez_set_property(DictEntry* info, const char* path, const char* interface) {
	
	DBusMessageIter iter;
	
	DBusMessage *msg;
	msg = dbus_message_new_method_call(
		"org.bluez",
		path,
		interface,
		"SetProperty");
	
	//start with args:
	dbus_message_iter_init_append(msg, &iter); 	
	
	//do boring things in this func... blahblahblah
	dbus_append_pair_to_dict(&iter, info->key, info->value_type, info->value);
	
	
	//finish with args
	dbus_message_append_args (msg, DBUS_TYPE_INVALID);

	e_dbus_message_send(DBUSCONN->sysconn, msg, cb_dbus_generic, -1, NULL);
	dbus_message_unref(msg);
	
	free(info);
	
}
