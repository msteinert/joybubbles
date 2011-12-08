#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# library definition
noinst_LTLIBRARIES = libjoybubbles-iterator.la
libjoybubbles_iterator_la_SOURCES = \
	hash.c \
	list.c \
	ptr.c \
	ptr-array.c \
	queue.c
# include directories & compiler switches
libjoybubbles_iterator_la_CFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(PANGO_CFLAGS)
# install development headers
pkgincludedir = $(includedir)/joybubbles-$(series)/joy/iterator
pkginclude_HEADERS = \
	hash.h \
	list.h \
	ptr.h \
	ptr-array.h \
	queue.h \
	types.h
