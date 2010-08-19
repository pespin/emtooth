#!/bin/bash

# GENERATE BINDINGS FOR DBUS OBJECTS AUTOMATICALLY

###########
# FUNCTIONS
###########
DEFAULT_BLUEZ_ADAPTER=`mdbus2 -s org.bluez /  org.bluez.Manager.DefaultAdapter | sed "s#( op'##" | sed "s#' )##"`


function gen_binding {
	XMLFILE="`echo "$1" | sed "s#/#_#g"`.xml"
	echo "generating vala bindings for org.bluez $1 on file $XMLFILE..."
	mdbus2 -s org.bluez "$1" org.freedesktop.DBus.Introspectable.Introspect | sed "s#( \"##" | grep -v " )" > $XMLFILE
}



#-----------------------------------------------

#gen_binding "/"
#gen_binding $DEFAULT_BLUEZ_ADAPTER
gen_binding "/org/bluez/5333/hci0/dev_00_19_2D_11_62_09"

vala-dbus-binding-tool --directory=.
#mv org-bluez.vala ../src/

rm *.xml org-freedesktop-dbus.vala
