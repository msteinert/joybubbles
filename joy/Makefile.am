#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

SUBDIRS = animation device easing filter iterator platform source theme
# generate marshalling routines
$(srcdir)/marshal.h: marshal.list
	$(AM_V_GEN)$(GLIB_GENMARSHAL) --prefix=joy_marshal --header $< > $@
$(srcdir)/marshal.c: marshal.list
	$(AM_V_GEN)$(GLIB_GENMARSHAL) --prefix=joy_marshal --body $< > $@
# library definition
lib_LTLIBRARIES = libjoybubbles-@series_major@.@series_minor@.la
libjoybubbles_@series_major@_@series_minor@_la_SOURCES = \
	accumulator.c \
	accumulator.h \
	animation.c \
	application.c \
	bubble.c \
	buffer.c \
	buffer.h \
	button.c \
	container.c \
	cursor.c \
	device.c \
	error.c \
	filter.c \
	image.c \
	iterator.c \
	label.c \
	macros.h \
	marshal.c \
	marshal.h \
	screen.c \
	sink.c \
	sink.h \
	style.c \
	sketch.c \
	theme.c \
	source.c \
	window.c
BUILT_SOURCES = \
	marshal.c \
	marshal.h
# include directories & compiler switches
libjoybubbles_@series_major@_@series_minor@_la_CFLAGS = \
	-DJOY_LOCALEDIR=\"$(localedir)\" \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(CAIRO_CFLAGS) \
	$(PANGO_CFLAGS) \
	$(PANGOCAIRO_CFLAGS) \
	$(GDK_PIXBUF_CFLAGS)
libjoybubbles_@series_major@_@series_minor@_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	$(CAIRO_LIBS) \
	$(PANGO_LIBS) \
	$(PANGOCAIRO_LIBS) \
	$(GDK_PIXBUF_LIBS)
libjoybubbles_@series_major@_@series_minor@_la_LIBADD = \
	animation/libjoybubbles-animation.la \
	device/libjoybubbles-device.la \
	easing/libjoybubbles-easing.la \
	filter/libjoybubbles-filter.la \
	iterator/libjoybubbles-iterator.la \
	platform/$(with_platform)/libjoybubbles-platform.la \
	source/libjoybubbles-source.la \
	theme/default/libdefault.la
# install development headers
pkgincludedir = $(includedir)/joybubbles-$(series)/joy
pkginclude_HEADERS = \
	animation.h \
	application.h \
	bubble.h \
	bubbles.h \
	button.h \
	container.h \
	cursor.h \
	device.h \
	easing.h \
	error.h \
	filter.h \
	image.h \
	iterator.h \
	label.h \
	screen.h \
	source.h \
	style.h \
	theme.h \
	sketch.h \
	types.h \
	window.h
if JOY_DBUS
libjoybubbles_@series_major@_@series_minor@_la_SOURCES += \
	dbus.c
libjoybubbles_@series_major@_@series_minor@_la_CFLAGS += \
	$(DBUS_CFLAGS)
libjoybubbles_@series_major@_@series_minor@_la_LDFLAGS += \
	$(DBUS_LIBS)
pkginclude_HEADERS += \
	dbus.h
endif
EXTRA_DIST = \
	marshal.list

