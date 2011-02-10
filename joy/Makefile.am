#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

SUBDIRS = animation device iterator platform
# generate marshalling routines
$(srcdir)/marshal.h: marshal.list
	$(AM_V_GEN)$(GLIB_GENMARSHAL) --prefix=joy_marshal --header $< > $@
$(srcdir)/marshal.c: marshal.list
	$(AM_V_GEN)$(GLIB_GENMARSHAL) --prefix=joy_marshal --body $< > $@
# library definition
lib_LTLIBRARIES = libjoybubbles-@joy_series_major@.@joy_series_minor@.la
libjoybubbles_@joy_series_major@_@joy_series_minor@_la_SOURCES = \
	animation.c \
	application.c \
	bubble.c \
	buffer.c \
	buffer.h \
	container.c \
	cursor.c \
	device.c \
	iterator.c \
	marshal.c \
	marshal.h \
	screen.c \
	sink.c \
	sink.h \
	sketch.c \
	source.c \
	window.c
BUILT_SOURCES = \
	marshal.c \
	marshal.h
# include directories & compiler switches
libjoybubbles_@joy_series_major@_@joy_series_minor@_la_CFLAGS = \
	-DJOY_LOCALEDIR=\"$(localedir)\" \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(CAIRO_CFLAGS)
libjoybubbles_@joy_series_major@_@joy_series_minor@_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	$(CAIRO_LIBS)
libjoybubbles_@joy_series_major@_@joy_series_minor@_la_LIBADD = \
	animation/libjoybubbles-animation.la \
	device/libjoybubbles-device.la \
	iterator/libjoybubbles-iterator.la \
	platform/$(joy_platform)/libjoybubbles-platform.la
# install development headers
pkgincludedir = $(includedir)/joybubbles-$(joy_series)/joy
pkginclude_HEADERS = \
	animation.h \
	application.h \
	bubble.h \
	bubbles.h \
	container.h \
	cursor.h \
	device.h \
	iterator.h \
	screen.h \
	source.h \
	sketch.h \
	types.h \
	window.h
EXTRA_DIST = \
	marshal.list
