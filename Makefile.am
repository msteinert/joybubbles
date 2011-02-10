#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

SUBDIRS = joy tests
ACLOCAL_AMFLAGS = -I m4
# install pkg-config script
joybubbles-$(joy_series).pc: joybubbles.pc
	cp $< $@
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = joybubbles-$(joy_series).pc
# cleanup
DISTCLEANFILES = \
    joybubbles-$(joy_series).pc
