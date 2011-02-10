#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# library definition
noinst_LTLIBRARIES = libjoybubbles-animation.la
libjoybubbles_animation_la_SOURCES = \
	grow.c \
	scale.c
# include directories & compiler switches
libjoybubbles_animation_la_CFLAGS = \
	-I$(top_srcdir) \
	$(CAIRO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS)
# install development headers
pkgincludedir = $(includedir)/joybubbles-$(joy_series)/joy/animation
pkginclude_HEADERS = \
	grow.h \
	scale.h \
	types.h
