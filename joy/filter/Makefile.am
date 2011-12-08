#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# library definition
noinst_LTLIBRARIES = libjoybubbles-filter.la
libjoybubbles_filter_la_SOURCES = \
	gaussian.c
# include directories & compiler switches
libjoybubbles_filter_la_CFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(CAIRO_CFLAGS) \
	$(PANGO_CFLAGS)
# install development headers
pkgincludedir = $(includedir)/joybubbles-$(series)/joy/filter
pkginclude_HEADERS = \
	gaussian.h \
	types.h
