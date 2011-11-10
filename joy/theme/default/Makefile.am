#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# library definition
theme_LTLIBRARIES = libdefault.la
libdefault_la_SOURCES = \
	button.c \
	button.h \
	default.c
# include directories & compiler switches
libdefault_la_CFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(CAIRO_CFLAGS) \
	$(PANGO_CFLAGS)
# install development headers
pkgincludedir = $(includedir)/joybubbles-$(series)/joy/theme
pkginclude_HEADERS = \
	default.h

