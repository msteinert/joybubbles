#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# library definition
noinst_LTLIBRARIES = libjoybubbles-device.la
libjoybubbles_device_la_SOURCES = \
	keyboard.c \
	mouse.c
# include directories & compiler switches
libjoybubbles_device_la_CFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(CAIRO_CFLAGS) \
	$(PANGO_CFLAGS)
# generate keysyms
JOY_GEN_KEYSYMS = $(srcdir)/joy-gen-keysyms.py
$(top_srcdir)/joy/device/keysym.h: $(JOY_GEN_KEYSYMS)
	$(AM_V_GEN)$(PYTHON) $(JOY_GEN_KEYSYMS) -o $@ $(JOY_KEYSYM_INPUT_H)
EXTRA_DIST = $(JOY_GEN_KEYSYMS)
# install development headers
pkgincludedir = $(includedir)/joybubbles-$(series)/joy/device
pkginclude_HEADERS = \
	keyboard.h \
	keysym.h \
	mouse.h \
	types.h

