#!/bin/bash

DEV_LIST=`mdbus -s org.bluez | grep "/org/bluez/$1/hci0/"`

for dev in $DEV_LIST; do
echo "Removing device $dev ..."
mdbus -s org.bluez /org/bluez/$1/hci0 org.bluez.Adapter.RemoveDevice $dev
done
