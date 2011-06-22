#!/bin/bash

# GENERATE BINDINGS FOR DBUS OBJECTS AUTOMATICALLY

###########
# FUNCTIONS
###########
DEFAULT_BLUEZ_ADAPTER=`mdbus2 -s org.bluez /  org.bluez.Manager.DefaultAdapter | sed "s#( op'##" | sed "s#' )##"`


function gen_binding {
	XMLFILE="`echo "$2" | sed "s#/#_#g"`.xml"
	echo "generating vala bindings for org.bluez $1 on file $XMLFILE..."
	mdbus2 -s "$1" "$2" org.freedesktop.DBus.Introspectable.Introspect | sed "s#( \"##" | grep -v " )" > $XMLFILE
	
	if [ "$?" != "0" ]; then
		mdbus2 "$1" "$2" org.freedesktop.DBus.Introspectable.Introspect | sed "s#( \"##" | grep -v " )" > $XMLFILE
	fi
	sed '1d' -i "${XMLFILE}"
	sed '1d' -i "${XMLFILE}"
	sed '$d' -i "${XMLFILE}"
}



#-----------------------------------------------

gen_binding "org.openobex" "/"
#gen_binding $DEFAULT_BLUEZ_ADAPTER
#gen_binding "/org/bluez/5333/hci0/dev_00_19_2D_11_62_09"

vala-dbus-binding-tool --directory=.
#mv org-bluez.vala ../src/

#rm *.xml org-freedesktop-dbus.vala
