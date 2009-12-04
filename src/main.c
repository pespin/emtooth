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

#include <Elementary.h>

#include "gui.h"


EAPI int
elm_main(int argc, char **argv)
{
   Evas_Object *win, *bg, *lb;

   win = elm_win_add(NULL, "main_win", ELM_WIN_BASIC);
   elm_win_title_set(win, "emtooth");
   evas_object_smart_callback_add(win, "delete,request", app_exit, NULL);

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, 1.0, 1.0);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   lb = elm_label_add(win);
   elm_label_label_set(lb, "Bluetooth Manager");
   evas_object_size_hint_weight_set(lb, 1.0, 1.0);
   elm_win_resize_object_add(win, lb);
   evas_object_show(lb);

   evas_object_show(win);

   elm_run();
   elm_shutdown();
   return 0;
}
ELM_MAIN()

