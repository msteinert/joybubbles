#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# library definition
noinst_LTLIBRARIES = libjoybubbles-easing.la
libjoybubbles_easing_la_SOURCES = \
	back.c \
	bounce.c \
	circular.c \
	curve.c \
	elastic.c \
	exponential.c \
	polynomial.c \
	sinusoidal.c
# include directories & compiler switches
libjoybubbles_easing_la_CFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS)
# install development headers
pkgincludedir = $(includedir)/joybubbles-$(series)/joy/easing
pkginclude_HEADERS = \
	back.h \
	bounce.h \
	circular.h \
	curve.h \
	elastic.h \
	exponential.h \
	polynomial.h \
	sinusoidal.h
