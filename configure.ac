#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Initialize autoconf
AC_PREREQ([2.59])
# Library versioning
# Making releases:
#	micro_version += 1
#	interface_age += 1
# If functions have been added or backward-compatibility has been broken:
#	interface_age = 0
m4_define([major_version], [0])
m4_define([minor_version], [2])
m4_define([micro_version], [3])
m4_define([interface_age], [2])
# Initialize autoconf & automake
AC_INIT([Joybubbles], [major_version.minor_version.micro_version],
    [https://github.com/msteinert/joybubbles/issues], [joybubbles])
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
	AX_TRY_CFLAGS([-std=c99], [AX_CFLAGS([-std=c99])])])
PKG_PROG_PKG_CONFIG([0.17])
AM_PATH_PYTHON([2.7],,
	[AC_MSG_WARN([Python 2.7 is required to regenerate key syms])])
# Enable compiler flags
AX_TRY_CFLAGS([-Wall], [AX_CFLAGS([-Wall])])
AX_TRY_CFLAGS([-Wextra], [AX_CFLAGS([-Wextra])])
AX_TRY_CFLAGS([-Wstrict-prototypes], [AX_CFLAGS([-Wstrict-prototypes])])
AX_TRY_CFLAGS([-Wcast-align], [AX_CFLAGS([-Wcast-align])])
AX_TRY_CFLAGS([-fno-common], [AX_CFLAGS([-fno-common])])
# Pkg-config versioning
series_major=major_version
series_minor=minor_version
series=$series_major.$series_minor
AC_SUBST([series_major])
AC_SUBST([series_minor])
AC_SUBST([series])
# Libtool versioning
LT_CURRENT=m4_eval(100 * minor_version + micro_version - interface_age)
LT_REVISION=interface_age
LT_AGE=m4_eval(m4_eval(100 * minor_version + micro_version) - interface_age)
AC_SUBST([LT_CURRENT])
AC_SUBST([LT_REVISION])
AC_SUBST([LT_AGE])
# Checks for dependencies
AC_SUBST([requires]) # pkg-config Requires
AC_SUBST([requires_private]) # pkg-config Requires.private
glib_minimum_version=2.24
glib_version="glib-2.0 >= $glib_minimum_version"
AC_SUBST([glib_version])
PKG_CHECK_MODULES([GLIB], [$glib_version])
gobject_version="gobject-2.0 >= $glib_minimum_version"
AC_SUBST([gobject_version])
PKG_CHECK_MODULES([GOBJECT], [$gobject_version])
cairo_version="cairo >= 1.10"
AC_SUBST([cairo_version])
PKG_CHECK_MODULES([CAIRO], [$cairo_version])
pango_minimum_version=1.28
pango_version="pango >= $pango_minimum_version"
AC_SUBST([pango_version])
PKG_CHECK_MODULES([PANGO], [$pango_version])
pangocairo_version="pangocairo >= $pango_minimum_version"
AC_SUBST([pangocairo_version])
PKG_CHECK_MODULES([PANGOCAIRO], [$pangocairo_version])
gdk_pixbuf_version="gdk-pixbuf-2.0"
AC_SUBST([gdk_pixbuf_version])
PKG_CHECK_MODULES([GDK_PIXBUF], [$gdk_pixbuf_version])
# Check for glib-genmarshal
AC_ARG_VAR([GLIB_GENMARSHAL], [path to the glib-genmarshal(1) utility])
AS_IF([test "x$GLIB_GENMARSHAL" = "x"],
	[PKG_CHECK_EXISTS([$glib_version],
		[GLIB_GENMARSHAL=`$PKG_CONFIG --variable=glib_genmarshal $glib_version`])])
# Debug option
AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug],
		[enable runtime debugging features @<:@default=no@:>@])],,
	[enable_debug=no])
AS_IF([test "x$enable_debug" = "xno"],
	[AC_DEFINE([G_DISABLE_CHECKS], [1],
		[Define to 1 to disable parameter checks.])
	AC_DEFINE([NDEBUG], [1],
		[Define to 1 if debugging is not enabled.])],
	[AC_DEFINE([JOY_DEBUG], [1],
		[Define to 1 to enable runtime debugging.])
	AX_TRY_CFLAGS([-g], [AX_CFLAGS([-g])])
	AX_TRY_CFLAGS([-Werror], [AX_CFLAGS([-Werror])])])
# Tracing option
AC_ARG_ENABLE([trace],
	[AS_HELP_STRING([--enable-trace],
		[enable runtime tracing features @<:@default=no@:>@])],,
	[enable_trace=no])
AS_IF([test "x$enable_trace" = "xno"],,
	[AS_IF([test "x$enable_debug" = "xno"],
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
	AX_TRY_CFLAGS([-finstrument-functions],
		[AX_CFLAGS([-finstrument-functions])],
		[AC_MSG_ERROR(['-finstrument-functions' is required for tracing.])])
	AC_DEFINE([JOY_TRACE], [1], [Define to 1 to enable runtime tracing.])])
# Default refresh rate
AC_ARG_ENABLE([refresh],
	[AS_HELP_STRING([--enable-refresh],
		[set the screen refresh rate @<:@default=60.0@:>@])],,
	[enable_refresh=60.0])
AC_DEFINE_UNQUOTED([JOY_REFRESH], [$enable_refresh],
	[Set to the default refresh rate.])
# D-Bus
AC_ARG_ENABLE([dbus],
	[AS_HELP_STRING([--enable-dbus],
		[enable D-Bus bindings @<:@default=auto@:>@])],,
	[enable_dbus=auto])
AS_IF([test "x$enable_dbus" != "xno"],
	[dbus_version="dbus-1 >= 1.1" # should be 1.2
	PKG_CHECK_MODULES([DBUS], [$dbus_version],
		[AC_DEFINE([JOY_HAVE_DBUS], [1],
			[Define to 1 if D-Bus is available.])
		requires+="$dbus_version "
		enable_dbus=yes],
		[enable_dbus=no])],
	[enable_dbus=no])
AM_CONDITIONAL([JOY_DBUS], [test "x$enable_dbus" = "xyes"])
# Platform option
default_platform=x11
AC_ARG_WITH([platform],
	[AS_HELP_STRING([--with-platform],
		[select the graphics platform @<:@x11|gfx3d@:>@])],,
	[AC_MSG_WARN([Default platform selected @<:@$default_platform@:>@])
	with_platform=$default_platform])
AC_SUBST([with_platform])
AS_CASE([$with_platform],
[x11],
	[requires_private+="x11 "
	PKG_CHECK_MODULES([X11], [x11])
	PKG_CHECK_MODULES([CAIRO_XLIB], [cairo-xlib])
	PKG_CHECK_MODULES([XCOMPOSITE], [xcomposite],
		[AC_DEFINE([HAVE_XCOMPOSITE], [1],
			[Define to 1 if XComposite is available.])])
	AC_DEFINE([JOY_PLATFORM_X11], [1],
		[Define to 1 if the platform is x11.])],
[gfx3d],
	[GFX3D_DIR="/vobs/drivers/linux/broadcom/74xxD"
	AX_LDFLAGS([-lc]) # Workaround for uClibc bug
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
	[AC_MSG_ERROR([Unknown platform @<:@$with_platform:>@])])
# Easing support
AC_ARG_ENABLE([easing],
	[AS_HELP_STRING([--disable-easing],
		[disable easing support for animation tweening])],,
	[enable_easing=yes])
AS_IF([test "x$enable_easing" = "xyes"],
	[AC_DEFINE([JOY_ENABLE_EASING], [1],
	[Define to 1 to enable easing functions.])])
# Native language support
AC_ARG_ENABLE([nls],
	[AS_HELP_STRING([--enable-nls],
		[enable native language support @<:@default=no@:>@])],,
	[enable_nls=no])
AS_IF([test "x$enable_nls" = "xyes"],
	[AC_DEFINE([ENABLE_NLS], [1], [Define to 1 to enable i18n.])
	AC_CHECK_FUNCS([bind_textdomain_codeset])])
# Set input files for keysyms
JOY_KEYSYMDEF_H="http://cgit.freedesktop.org/xorg/proto/x11proto/plain/keysymdef.h"
JOY_XF86KEYSYMS_H="http://cgit.freedesktop.org/xorg/proto/x11proto/plain/XF86keysym.h"
AC_ARG_VAR([JOY_KEYSYM_INPUT_H],
	[path(s) and/or URL(s) to generate <joy/device/keysym.h>])
AS_IF([test "x$JOY_KEYSYM_INPUT_H" = "x"],
	[JOY_KEYSYM_INPUT_H="$JOY_KEYSYMDEF_H $JOY_XF86KEYSYMS_H"])
# Configure theme directory
themedir="\$(libdir)/joybubbles-$series/theme"
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
	joy/theme/Makefile
	joy/theme/default/Makefile
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
	Platform: $with_platform
	D-Bus: $enable_dbus
	Easing Functions: $enable_easing
	Native Language Support: $enable_nls
	Debugging: $enable_debug
	Tracing: $enable_trace
])
