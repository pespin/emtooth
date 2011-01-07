AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -I$(top_srcdir) @EMTOOTH2_CFLAGS@

bin_PROGRAMS      = emtooth2

emtooth2_SOURCES = \
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
	
emtooth2_LDADD = @EMTOOTH2_LIBS@
emtooth2_CFLAGS =
VALAFLAGS = @EMTOOTH2_PACKAGES@ @EMTOOTH2_VALAFLAGS@

desktopdir = $(datadir)/applications
desktop_DATA = data/emtooth.desktop

pixmapsdir = $(datadir)/pixmaps
pixmaps_DATA =  data/emtooth.png

