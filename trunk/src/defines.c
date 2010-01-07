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
#include "defines.h"


GuiCb* init_cb_struct(const char* property, const char* path) {
	
		GuiCb* cb = malloc(sizeof(GuiCb));
		
		fprintf(stderr, "property: %s\n", property);
		
		cb->property = strdup(property);
		
		if(!path || path==ADAPTER->path) {
			cb->path = ADAPTER->path;
			cb->interface = strdup("org.bluez.Adapter");
		} else { 
			cb->path = strdup(path);
			cb->interface = strdup("org.bluez.Device");
		}
		
		return cb;
}

