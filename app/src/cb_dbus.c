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

#include "cb_dbus.h"

void cb_dbus_generic(void *data, DBusMessage *replymsg, DBusError *error) {
	DBUSLOG(error);
}

void cb_dbus_generic_remote_gui_alert(void *data, DBusMessage *replymsg, DBusError *error) {
	
	RemoteDevice* device = (RemoteDevice*) data;
	
	fprintf(stderr, "Cb: DbusGenericRemoteGuiAlert on device [%s]...\n", device->addr);
	
	DBUSLOG(error);
	
	if(!replymsg) {
		char buf[512];
		snprintf(buf, 511, "<b>Last operation failed with error:</b><br><br>%s<br>%s", error->name, error->message);
		gui_alert_create(buf);
	}
	
}
