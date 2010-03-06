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
#include <stdio.h>
#include "defines.h"

DBusMessage* obex_agent_method_Authorize(E_DBus_Object *obj, DBusMessage *msg) {

	
	fprintf(stderr, "AgentCb: Authorization method !!!!\n");
	return dbus_message_new_method_return(msg);
}



DBusMessage* obex_agent_method_Cancel(E_DBus_Object *obj, DBusMessage *msg) {
	
	 fprintf(stderr, "AgentCb: Cancel()!\n");
	 
	 gui_alert_create("Last action from remote<br>device has been canceled!");
	 
	return dbus_message_new_method_return(msg);
}
