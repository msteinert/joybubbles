#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# library definition
noinst_LTLIBRARIES = libjoybubbles-platform.la
libjoybubbles_platform_la_SOURCES = \
	application.c \
	application.h \
	keyboard.c \
	keyboard.h \
	mouse.c \
	mouse.h \
	screen.c \
	screen.h \
	source.c \
	source.h \
	types.h \
	window.c \
	window.h
# include directories & compiler switches
libjoybubbles_platform_la_CFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(CAIRO_CFLAGS) \
	$(PANGO_CFLAGS) \
	$(X11_CFLAGS) \
	$(XCOMPOSITE_CFLAGS) \
	$(CAIRO_XLIB_CFLAGS)
libjoybubbles_platform_la_LDFLAGS = \
	$(X11_LIBS) \
	$(XCOMPOSITE_LIBS) \
	$(CAIRO_XLIB_LIBS)
