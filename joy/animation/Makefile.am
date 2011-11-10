#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# library definition
noinst_LTLIBRARIES = libjoybubbles-animation.la
libjoybubbles_animation_la_SOURCES = \
	fade.c \
	move.c \
	parallel.c \
	resize.c \
	series.c
# include directories & compiler switches
libjoybubbles_animation_la_CFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(CAIRO_CFLAGS) \
	$(PANGO_CFLAGS)
# install development headers
pkgincludedir = $(includedir)/joybubbles-$(series)/joy/animation
pkginclude_HEADERS = \
	fade.h \
	move.h \
	parallel.h \
	resize.h \
	series.h \
	types.h

