AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -I$(top_srcdir) @ELEMENTARY_CFLAGS@

bin_PROGRAMS      = emtooth

emtooth_SOURCES = \
	src/main.c \
	src/defines.h \
	src/cb_bluez.c \
	src/cb_bluez.h \
	src/cb_dbus.c \
	src/cb_dbus.h \
	src/cb_gui.c \
	src/cb_gui.h \
	src/bluez.c \
	src/bluez.h \
	src/dbus.c \
	src/dbus.h \
	src/gui.c \
	src/gui.h
emtooth_LDADD = @ELEMENTARY_LIBS@
emtooth_CFLAGS =
