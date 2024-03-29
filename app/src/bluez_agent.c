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

#include <stdio.h>
#include <E_DBus.h>
#include "cb_dbus.h"
#include "bluez_agent.h"
#include "cb_bluez_agent.h"

void bluez_agent_create() {
	
	fprintf(stderr, "Creating bluez agent [%s]...\n", BLUEZ_AGENT_PATH);
	
	E_DBus_Object* obj;
	E_DBus_Interface* iface;
	
	
	obj = e_dbus_object_add(DBUSCONN->sysconn, BLUEZ_AGENT_PATH, NULL);
	iface = e_dbus_interface_new(BLUEZ_AGENT_IFACE);
	if(!iface || !obj) {
		fprintf(stderr, "ERROR: bluez agent creation: iface or obj == NULL\n");
		return;
	}
	
	e_dbus_object_interface_attach(obj, iface);
	
	e_dbus_interface_method_add(iface, "RequestPinCode",
	 "o", "s", bluez_agent_method_RequestPinCode);
	 
	e_dbus_interface_method_add(iface, "RequestPassKey",
	 "o", "u", bluez_agent_method_RequestPassKey);
	 
	e_dbus_interface_method_add(iface, "DisplayPasskey",
	 "ouq", "", bluez_agent_method_DisplayPasskey);
	 
	e_dbus_interface_method_add(iface, "RequestConfirmation",
	 "ou", "", bluez_agent_method_RequestConfirmation);
	
	e_dbus_interface_method_add(iface, "Authorize",
	 "os", "", bluez_agent_method_Authorize);

	e_dbus_interface_method_add(iface, "ConfirmModeChange",
	"s", "", bluez_agent_method_ConfirmModeChange);

	e_dbus_interface_method_add(iface, "Cancel",
	 "", "", bluez_agent_method_Cancel);
	 
	 e_dbus_interface_method_add(iface, "Release",
	 "", "", bluez_agent_method_Release);
	

	bluez_register_agent("");
}
