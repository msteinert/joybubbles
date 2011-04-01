#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# library definition
noinst_LTLIBRARIES = libjoybubbles-style.la
libjoybubbles_style_la_SOURCES = \
	window.c
# include directories & compiler switches
libjoybubbles_style_la_CFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(CAIRO_CFLAGS) \
	$(PANGO_CFLAGS)
# install development headers
pkgincludedir = $(includedir)/joybubbles-$(joy_series)/joy/style
pkginclude_HEADERS = \
	window.h \
	types.h
