AC_PREREQ([2.65])
AC_INIT([pidgin-single-window], [0.3])

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([.aux])

AM_INIT_AUTOMAKE([1.11 subdir-objects dist-xz no-dist-gzip tar-ustar foreign])

AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_DISABLE_STATIC
LT_INIT
PKG_PROG_PKG_CONFIG

GETTEXT_PACKAGE=$PACKAGE_NAME
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Gettext Package])
AC_SUBST(GETTEXT_PACKAGE)
IT_PROG_INTLTOOL([0.40.0])

AC_PROG_INSTALL


PACKAGE_DEVELOPER="sardemff7"
AC_DEFINE_UNQUOTED([PIDGIN_SINGLE_WINDOW_PLUGIN_ID], ["gtk-$PACKAGE_DEVELOPER-$PACKAGE_NAME"], [libpurple plugin id])

AM_CFLAGS="-Wall -Wextra -Wno-unused-parameter"
AM_LDFLAGS=""

#
# Check for GLib
#
PKG_CHECK_MODULES(GLIB, glib-2.0)

#
# Check for GTK+
#
PKG_CHECK_MODULES(GTK, gtk+-2.0)

#
# Check for pidgin
#
AC_ARG_WITH(purple-plugindir, AS_HELP_STRING([--with-pidgin-plugindir=DIR], [Directory for Pidgin plugin]), [], with_pidgin_plugindir=yes)
case "$with_pidgin_plugindir" in
    no|"") AC_MSG_ERROR([*** You must define pidgin-plugindir]) ;;
    yes) plugindir="`$PKG_CONFIG --variable=plugindir pidgin`" ;;
    *) plugindir="$with_pidgin_plugindir" ;;
esac
PKG_CHECK_MODULES(PIDGIN, pidgin >= 2.6.0)
AC_SUBST(plugindir)


AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_LDFLAGS)

AC_SUBST(shrext_cmds)

AC_CONFIG_FILES([
    Makefile
    po/Makefile.in
])
AC_OUTPUT
