#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# library definition
theme_LTLIBRARIES = libsimple.la
libsimple_la_SOURCES = \
	simple.c \
	window.c \
	window.h
# include directories & compiler switches
libjoybubbles_platform_la_CFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(CAIRO_CFLAGS)
libjoybubbles_platform_la_LDFLAGS = \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	$(CAIRO_LIBS)
# install development headers
pkgincludedir = $(includedir)/joybubbles-$(joy_series)/joy/theme
pkginclude_HEADERS = \
	simple.h
