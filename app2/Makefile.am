AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -I$(top_srcdir) @EMTOOTH_CFLAGS@

bin_PROGRAMS      = emtooth

emtooth_SOURCES = \
	src/main.vala \
	src/shared.vala \
	src/fso.vala \
	src/bluez_adapter.vala \
	src/bluez_rdevice.vala \
	src/bluez_agent.vala \
	src/gui_main.vala \
	src/gui_settings.vala \
	src/gui_rdevice.vala \
	src/org-bluez.vala
	
emtooth_LDADD = @EMTOOTH_LIBS@
emtooth_CFLAGS =
VALAFLAGS = @EMTOOTH_PACKAGES@ @EMTOOTH_VALAFLAGS@

desktopdir = $(datadir)/applications
desktop_DATA = data/emtooth.desktop

pixmapsdir = $(datadir)/pixmaps
pixmaps_DATA =  data/emtooth.png

