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

#include <pthread.h>
#include <Elementary.h>
#include <E_DBus.h>
#include "gui.h"
#include "dbus.h"

EAPI int
elm_main(int argc, char **argv)
{ 
	
	/* init GLOBALS */
	DBUSCONN = (DbusConn*) malloc(sizeof(DbusConn));
	
	ADAPTER = (LocalDevice*) malloc(sizeof(LocalDevice));
	ADAPTER->addr = NULL;
	ADAPTER->name = NULL;
	ADAPTER->path = NULL;
	
	DL = (DeviceList*) malloc(sizeof(DeviceList));
	DL->devices =  NULL;
	DL->li = NULL;
	
	/* Thread created OnStart:
	 * 1- Start dbus connection
	 * 2- Start bluetooth using frameworkd if it is present
	 * 3- Start bluez session + get local device info
	 * 4- Start device discovery + TODO: create openobex server to receive files
	 */
	//start the dbus thread
	int ret;
	pthread_t t_id;
	ret = pthread_create(&t_id, NULL, (void *)dbus_init_session, NULL);
   
   //start the GUI:
   gui_create();
  
   elm_run();
   //cb_safe_exit() is called on close main win
   elm_shutdown();
   return 0;
}
ELM_MAIN()

