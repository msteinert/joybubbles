#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Initialize autoconf
AC_PREREQ([2.59])
# Library versioning
# Making releases:
#   joy_micro_version += 1
#   joy_interface_age += 1
# If functions have been added or backward-compatibility has been broken:
#   joy_interface_age = 0
m4_define([joy_major_version], [0])
m4_define([joy_minor_version], [2])
m4_define([joy_micro_version], [0])
m4_define([joy_interface_age], [0])
# Initialize autoconf & automake
AC_INIT([Joybubbles], [joy_major_version.joy_minor_version.joy_micro_version],
    [https://inv-lx-apps.echostar.com/mantis/], [joybubbles])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.9 foreign -Wno-portability])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_SRCDIR([joy/bubbles.h])
# Initialize Libtool
LT_PREREQ([2.2])
LT_INIT([disable-static])
# Checks for programs
m4_ifdef([AC_PROG_CC_C99], [AC_PROG_CC_C99],
	[AC_MSG_WARN([Attemping c99 workaround due to old version of autoconf])
	AC_PROG_CC
	JOY_TRY_CC_FLAGS([-std=c99], [JOY_CFLAGS([-std=c99])])])
PKG_PROG_PKG_CONFIG([0.17])
AM_PATH_PYTHON([2.7],,
	[AC_MSG_WARN([Python 2.7 is required to regenerate key syms])])
# Enable compiler flags
JOY_TRY_CC_FLAGS([-Wall], [JOY_CFLAGS([-Wall])])
JOY_TRY_CC_FLAGS([-Wstrict-prototypes], [JOY_CFLAGS([-Wstrict-prototypes])])
JOY_TRY_CC_FLAGS([-fno-common], [JOY_CFLAGS([-fno-common])])
# Pkg-config versioning
joy_series_major=joy_major_version
joy_series_minor=joy_minor_version
joy_series=$joy_series_major.$joy_series_minor
AC_SUBST([joy_series_major])
AC_SUBST([joy_series_minor])
AC_SUBST([joy_series])
# Libtool versioning
LT_CURRENT=m4_eval(100 * joy_minor_version + joy_micro_version - joy_interface_age)
LT_REVISION=joy_interface_age
LT_AGE=m4_eval(m4_eval(100 * joy_minor_version + joy_micro_version) - joy_interface_age)
AC_SUBST([LT_CURRENT])
AC_SUBST([LT_REVISION])
AC_SUBST([LT_AGE])
# Checks for dependencies
AC_SUBST([joy_requires]) # pkg-config Requires
AC_SUBST([joy_requires_private]) # pkg-config Requires.private
joy_glib_minimum_version=2.24
joy_glib_version="glib-2.0 >= $joy_glib_minimum_version"
AC_SUBST([joy_glib_version])
PKG_CHECK_MODULES([GLIB], [$joy_glib_version])
joy_gobject_version="gobject-2.0 >= $joy_glib_minimum_version"
AC_SUBST([joy_gobject_version])
PKG_CHECK_MODULES([GOBJECT], [$joy_gobject_version])
joy_cairo_version="cairo >= 1.10"
AC_SUBST([joy_cairo_version])
PKG_CHECK_MODULES([CAIRO], [$joy_cairo_version])
joy_pango_minimum_version=1.28
joy_pango_version="pango >= $joy_pango_minimum_version"
AC_SUBST([joy_pango_version])
PKG_CHECK_MODULES([PANGO], [$joy_pango_version])
joy_pangocairo_version="pangocairo >= $joy_pango_minimum_version"
AC_SUBST([joy_pangocairo_version])
PKG_CHECK_MODULES([PANGOCAIRO], [$joy_pangocairo_version])
joy_gdk_pixbuf_version="gdk-pixbuf-2.0"
AC_SUBST([joy_gdk_pixbuf_version])
PKG_CHECK_MODULES([GDK_PIXBUF], [$joy_gdk_pixbuf_version])
# Check for glib-genmarshal
AC_ARG_VAR([GLIB_GENMARSHAL], [path to the glib-genmarshal(1) utility])
AS_IF([test "x$GLIB_GENMARSHAL" = "x"],
	[PKG_CHECK_EXISTS([$joy_glib_version],
		[GLIB_GENMARSHAL=`$PKG_CONFIG --variable=glib_genmarshal $joy_glib_version`])])
# Debug option
AC_ARG_ENABLE([debug],
	[AC_HELP_STRING([--enable-debug],
		[enable runtime debugging features @<:@default=no@:>@])],
	[AC_DEFINE([JOY_DEBUG], [1], [Define to 1 to enable runtime debugging.])
	JOY_TRY_CC_FLAGS([-g], [JOY_CFLAGS([-g])])
	joy_debug=yes],
	[AC_DEFINE([G_DISABLE_CHECKS], [1],
		[Define to 1 to disable parameter checks.])
	joy_debug=no])
# Tracing option
AC_ARG_ENABLE([trace],
	[AC_HELP_STRING([--enable-trace],
		[enable runtime tracing features @<:@default=no@:>@])],
	[AS_IF([test "x$joy_debug" = "xno"],
		[AC_MSG_ERROR([Debugging must be enabled to enable tracing.])])
	AC_SEARCH_LIBS([bfd_openr], [bfd],
		[AC_MSG_WARN([Linking with libbfd makes this library subject to the terms of the GPL!])
		AC_DEFINE([HAVE_BFD], [1],
			[Define to 1 if libbfd is available.])])
	AC_SEARCH_LIBS([dladdr], [dl],
		[AC_DEFINE([_GNU_SOURCE], [1],
			[Define to 1 to enable GNU extensions.])
		AC_DEFINE([HAVE_DLADDR], [1],
			[Define to 1 dladdr is available.])])
	JOY_TRY_CC_FLAGS([-finstrument-functions],
		[JOY_CFLAGS([-finstrument-functions])],
		[AC_MSG_ERROR(['-finstrument-functions' is required for tracing.])])
	AC_DEFINE([JOY_TRACE], [1], [Define to 1 to enable runtime tracing.])
	joy_trace=yes],
	[joy_trace=no])
# D-Bus
AC_ARG_WITH([dbus],
	[AC_HELP_STRING([--with-dbus],
		[enable D-Bus bindings @<:@default=auto@:>@])],,
	[withval=auto])
AC_CACHE_CHECK([for D-Bus support], [joy_cv_with_dbus],
	[joy_cv_with_dbus="$withval"])
AS_IF([test "x$joy_cv_with_dbus" != "xno"],
	[joy_dbus_version="dbus-1 >= 1.1" # should be 1.2
	AC_DEFINE([JOY_HAVE_DBUS], [1], [Define to 1 if D-Bus is available.])
	PKG_CHECK_MODULES([DBUS], [$joy_dbus_version], # should be 1.2
		[joy_requires+="$joy_dbus_version "
		joy_dbus=yes],
		[joy_dbus=no])],
	[joy_dbus=no])
AM_CONDITIONAL([JOY_DBUS], [test "x$joy_dbus" = "xyes"])
# Platform option
joy_platform_default=x11
AC_ARG_WITH([platform],
	[AC_HELP_STRING([--with-platform],
		[select the graphics platform @<:@x11|gfx3d@:>@])],
	[AS_CASE([$withval],
	[x11|gfx3d],
		[joy_platform=$withval],
	[*],
		[AC_MSG_WARN([Unknown platform selected @<:@$withval:>@, defaulting to @<:@$joy_platform_default@:>@])
		joy_platform=$joy_platform_default])],
	[joy_platform=$joy_platform_default])
AC_DEFINE_UNQUOTED([JOY_PLATFORM], ["$joy_platform"],
	[Define to the platform.])
AC_SUBST([joy_platform])
# Easing support
AC_ARG_ENABLE([easing],
	[AC_HELP_STRING([--disable-easing],
		[disable easing support for animation tweening])],
	[joy_easing=$enableval],
	[joy_easing=yes])
AS_IF([test "x$joy_easing" = "xyes"],
	[AC_DEFINE([JOY_ENABLE_EASING], [1],
	[Define to 1 to enable easing functions.])])
# Native language support
AC_ARG_ENABLE([nls],
	[AC_HELP_STRING([--enable-nls],
		[enable native language support @<:@default=no@:>@])],
	[joy_nls=$enableval],
	[joy_nls=no])
AS_IF([test "x$joy_nls" = "xyes"],
	[AC_DEFINE([ENABLE_NLS], [1], [Define to 1 to enable i18n.])
	AC_CHECK_FUNCS([bind_textdomain_codeset])])
# Platform-specific setup
AS_CASE([$joy_platform],
[x11],
	[joy_requires_private+="x11 "
	PKG_CHECK_MODULES([X11], [x11])
	PKG_CHECK_MODULES([CAIRO_XLIB], [cairo-xlib])
	PKG_CHECK_MODULES([XCOMPOSITE], [xcomposite],
		[AC_DEFINE([HAVE_XCOMPOSITE], [1],
			[Define to 1 if XComposite is available.])],
		[:])
	AC_DEFINE([JOY_PLATFORM_X11], [1],
		[Define to 1 if the platform is x11.])],
[gfx3d],
	[GFX3D_DIR="/vobs/drivers/linux/broadcom/74xxD"
	AC_ARG_VAR([GFX3D_CFLAGS], [C compiler flags for GFX3D, overriding defaults])
	AS_IF([test "x$GFX3D_CFLAGS" = "x"],
		[GFX3D_CFLAGS="-I$GFX3D_DIR/app/lib/gfx3d -I$GFX3D_DIR/pi/vip922/basemodules/std/types/linuxuser"])
	AC_ARG_VAR([GFX3D_LIBS], [linker flags for GFX3D, overriding defaults])
	AS_IF([test "x$GFX3D_LIBS" = "x"],
		[GFX3D_LIBS="-L$GFX3D_DIR/build/bin/vip922 -lbcmgfx"])
	AC_ARG_VAR([INPUT_MGR_CFLAGS], [C compiler flags for INPUT_MGR, overriding defaults])
	AS_IF([test "x$INPUT_MGR_CFLAGS" = "x"],
		[INPUT_MGR_CFLAGS="-I/vobs/input_mgr/include"])
	AC_DEFINE([JOY_PLATFORM_GFX3D], [1],
		[Define to 1 if the platform is gfx3d.])],
[*],
	[:])
# Set input files for keysyms
JOY_KEYSYMDEF_H="http://cgit.freedesktop.org/xorg/proto/x11proto/plain/keysymdef.h"
JOY_XF86KEYSYMS_H="http://cgit.freedesktop.org/xorg/proto/x11proto/plain/XF86keysym.h"
AC_ARG_VAR([JOY_KEYSYM_INPUT_H],
	[path(s) and/or URL(s) to generate <joy/device/keysym.h>])
AS_IF([test "x$JOY_KEYSYM_INPUT_H" = "x"],
	[JOY_KEYSYM_INPUT_H="$JOY_KEYSYMDEF_H $JOY_XF86KEYSYMS_H"])
# Configure theme directory
themedir="\$(libdir)/joybubbles-$joy_series/theme"
AC_SUBST(themedir)
# GLib configuration
AC_DEFINE_UNQUOTED([G_LOG_DOMAIN], ["$PACKAGE_NAME"],
	[The GLib log domain for this project.])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$PACKAGE"],
	[The i18n domain for this project.])
AH_BOTTOM(
[#ifdef ENABLE_NLS
/* Include GLib i18n header file */
#include <glib/gi18n-lib.h>
#else // ENABLE_NLS
#define Q_(s) s
#define N_(s) s
#endif // ENABLE_NLS
])dnl
# Output files
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	joy/animation/Makefile
	joy/device/Makefile
	joy/easing/Makefile
	joy/filter/Makefile
	joy/iterator/Makefile
	joy/platform/Makefile
	joy/platform/gfx3d/Makefile
	joy/platform/x11/Makefile
	joy/source/Makefile
	joy/style/Makefile
	joy/Makefile
	pkgconfig/joybubbles.pc
	pkgconfig/joybubbles-dbus.pc
	pkgconfig/Makefile
	tests/Makefile
	Makefile
])
AC_OUTPUT
AC_MSG_RESULT([
$PACKAGE_NAME build options:
	Host: $host
	Version: $VERSION
	Platform: $joy_platform
	D-Bus: $joy_dbus
	Easing Functions: $joy_easing
	Native Language Support: $joy_nls
	Debugging: $joy_debug
	Tracing: $joy_trace
])
