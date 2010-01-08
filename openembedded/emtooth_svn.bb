DESCRIPTION = "A bluetooth/bluez manager written in elementary for embedded systems"
AUTOR = "Pau Espin Pedrol (Sharwin_F) <pespin.shar@gmail.com>"
HOMEPAGE = "http://code.google.com/p/emtooth/"
SECTION = "x11/applications"
LICENSE = "GPLv2"
DEPENDS = "elementary eina edbus"

PV = "0.1+svnr${SRCDATE}"
PR = "r0"

# upstream does not yet publish any release so we have to fetch last working version from SVN
SRC_URI = "svn://emtooth.googlecode.com/svn/trunk;module=./;proto=svn;date=now"

S = "${WORKDIR}"

inherit pkgconfig autotools

