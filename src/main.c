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
	/*
	BLUEZID = get_bluez_pid();
	if(BLUEZID==0) {
		fprintf(stderr, "Could not get bluez dbus daemon pid");
		return EXIT_FAILURE;
	}
	BLUEZPATH = malloc(50);
	memset(BLUEZPATH, 0, sizeof(BLUEZPATH));
	sprintf(BLUEZPATH, "/org/bluez/%i/hci0", BLUEZID);
	fprintf(stderr, "Using path '%s' to connect to bluez dbus daemon...", BLUEZPATH);
	*/
	
	DeviceList DL;
	DL.devices =  NULL;
	DL.li = NULL;
	
	int ret;
	pthread_t t_id;
	//start the dbus thread
	ret = pthread_create(&t_id, NULL, (void *)dbus_init_session, &DL);
   
   gui_create(&DL);
  
   elm_run();
   elm_shutdown();
   return 0;
}
ELM_MAIN()

