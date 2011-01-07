DESCRIPTION = "A bluetooth/bluez manager written in elementary for embedded systems. Second version written now in Vala"
AUTHOR = "Pau Espin Pedrol (Sharwin_F) <pespin.shar@gmail.com>"
HOMEPAGE = "http://code.google.com/p/emtooth/"
SECTION = "x11/applications"
LICENSE = "GPLv2"
DEPENDS = "elementary"
RDEPENDS_${PN} = "bluez4 obexd"

SRCREV = "100"
PV = "0.1+svnr${SRCPV}"
PR = "r0"

SRC_URI = "svn://emtooth.googlecode.com/svn/trunk;module=app2;proto=http"

S = "${WORKDIR}/app2"

inherit autotools vala

