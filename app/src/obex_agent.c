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
#include "obex_agent.h"
#include "cb_obex_agent.h"

void obex_agent_create() {
	
	fprintf(stderr, "Creating obex agent [%s]...\n", OBEX_AGENT_PATH);
	
	E_DBus_Object* obj;
	E_DBus_Interface* iface;
	
	
	obj = e_dbus_object_add(DBUSCONN->sessionconn, "/org/emtooth/mehe", NULL);
	iface = e_dbus_interface_new("org.mehe");
	if(!iface || !obj) {
		fprintf(stderr, "ERROR: obex agent creation: iface or obj == NULL\n");
		return;
	}
	
	e_dbus_object_interface_attach(obj, iface);
	
	
	e_dbus_interface_method_add(iface, "Authorize",
	 "osssuu", "s", obex_agent_method_Authorize);
	 
	e_dbus_interface_method_add(iface, "Cancel",
	 "", "", obex_agent_method_Cancel);

	obex_register_agent();
}
