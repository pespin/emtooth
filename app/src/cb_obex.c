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
#include "cb_obex.h"

/* signals */


void cb_obex_manager_SessionCreated(void *data, DBusMessage *msg) {
	
	fprintf(stderr, "SignalObex: SessionCreated!\n");
	
}

void cb_obex_manager_SessionRemoved(void *data, DBusMessage *msg) {

		fprintf(stderr, "SignalObex: SessionRemoved!\n");
	
}

void cb_obex_manager_TransferStarted(void *data, DBusMessage *msg) {

		fprintf(stderr, "SignalObex: TransferStarted!\n");
	
}

void cb_obex_manager_TransferCompleted(void *data, DBusMessage *msg) {

			fprintf(stderr, "SignalObex: TransferCompleted!\n");
	
}
