#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# include directories
INCLUDES = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(CAIRO_CFLAGS) \
	$(PANGO_CFLAGS)
if JOY_DBUS
INCLUDES += $(DBUS_CFLAGS)
endif
# add libraries
LIBS = \
	$(top_builddir)/joy/libjoybubbles-$(joy_series).la
# add unit tests
bin_PROGRAMS = \
	joybubbles-test \
	iterator-test
TESTS = $(check_PROGRAMS)
# add source files
joybubbles_test_SOURCES = joybubbles-test.c
iterator_test_SOURCES = iterator-test.c
# generate XML report
joybubbles.xml: $(check_PROGRAMS)
	gtester -k -o $@ $(check_PROGRAMS)
# generate HTML report
joybubbles.html: joybubbles.xml
	gtester-report $< > $@
CLEANFILES = \
	joybubbles.xml
DISTCLEANFILES = \
	joybubbles.html core core.*
