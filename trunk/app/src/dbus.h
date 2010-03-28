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
#include "defines.h"

void dbus_init_session();

void dbus_dict_pair_debug(DictEntry* ret);

DictEntry* dbus_message_iter_get_dict_pair(DBusMessageIter *key_iter);
DictEntry* dbus_message_get_dict_pair(DBusMessage *msg);

int dbus_message_iter_get_array_size(DBusMessageIter* array_iter);
char** dbus_message_iter_get_array(DBusMessageIter* array_iter, int size);
void dbus_message_iter_append_array(DBusMessageIter* array_iter, const char** array);

DictEntry* dbus_get_next_struct_in_dict(DBusMessageIter *dict_iter);
void dbus_message_get_variant(DBusMessageIter* iter, DictEntry* ret);

void dbus_append_variant(DBusMessageIter* iter, int value_type, Variant value);
void dbus_append_pair_to_dict(DBusMessageIter* iter, char* key, int value_type, Variant value);

