#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# library definition
noinst_LTLIBRARIES = libjoybubbles-source.la
libjoybubbles_source_la_SOURCES =
# include directories & compiler switches
libjoybubbles_source_la_CFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS)
libjoybubbles_source_la_LDFLAGS =
# install development headers
pkgincludedir = $(includedir)/joybubbles-$(series)/joy/source
pkginclude_HEADERS =
if JOY_DBUS
libjoybubbles_source_la_SOURCES  += dbus.c
libjoybubbles_source_la_CFLAGS += $(DBUS_CFLAGS)
libjoybubbles_source_la_LDFLAGS += $(DBUS_LIBS)
pkginclude_HEADERS += dbus.h
endif
