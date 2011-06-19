AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -I$(top_srcdir) @EMTOOTH2_CFLAGS@
AM_CPPFLAGS = "-DIMAGESDIR=\"$(datadir)/emtooth2/images\""

bin_PROGRAMS      = emtooth2

emtooth2_SOURCES = \
	src/main.vala \
	src/shared.vala \
	src/fso.vala \
	src/bluez_adapter.vala \
	src/bluez_rdevice.vala \
	src/bluez_agent.vala \
	src/gui.vala \
	src/gui_main.vala \
	src/gui_settings.vala \
	src/gui_rdevice.vala \
	src/org-bluez.vala
	
emtooth2_LDADD = @EMTOOTH2_LIBS@
emtooth2_CFLAGS =
VALAFLAGS = @EMTOOTH2_PACKAGES@ @EMTOOTH2_VALAFLAGS@

desktopdir = $(datadir)/applications
desktop_DATA = data/emtooth2.desktop

pixmapsdir = $(datadir)/pixmaps
pixmaps_DATA =  data/emtooth2.png

imagesdir = $(datadir)/emtooth2/images
images_DATA = \
data/offline.png \
data/online.png

EXTRA_DIST = $(images_DATA)
