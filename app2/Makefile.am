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
	src/gui_known.vala \
	src/gui_main.vala \
	src/gui_settings.vala \
	src/gui_rdevice.vala \
	src/obex_agent.vala \
	src/obex_client_agent.vala \
	src/obex_manager.vala \
	src/org-bluez.vala \
	src/org-openobex.vala
	
emtooth2_LDADD = @EMTOOTH2_LIBS@
emtooth2_CFLAGS =
VALAFLAGS = @EMTOOTH2_PACKAGES@ @EMTOOTH2_VALAFLAGS@

desktopdir = $(datadir)/applications
desktop_DATA = data/emtooth2.desktop

pixmapsdir = $(datadir)/pixmaps
pixmaps_DATA =  data/emtooth2.png

imagesdir = $(datadir)/emtooth2/images
images_DATA = \
	data/audio-card-offline.png \
	data/audio-card-online.png \
	data/camera-photo-offline.png \
	data/camera-photo-online.png \
	data/camera-video-offline.png \
	data/camera-video-online.png \
	data/computer-offline.png \
	data/computer-online.png \
	data/input-gaming-offline.png \
	data/input-gaming-online.png \
	data/input-keyboard-offline.png \
	data/input-keyboard-online.png \
	data/input-mouse-offline.png \
	data/input-mouse-online.png \
	data/input-table-offline.png \
	data/input-table-online.png \
	data/modem-offline.png \
	data/modem-online.png \
	data/network-wireless-offline.png \
	data/network-wireless-online.png \
	data/phone-offline.png \
	data/phone-online.png \
	data/printer-offline.png \
	data/printer-online.png \
	data/unknown-offline.png \
	data/unknown-online.png

EXTRA_DIST = $(images_DATA)
