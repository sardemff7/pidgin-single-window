AM_DISTCHECK_CONFIGURE_FLAGS = \
	--disable-debug \
	--with-pidgin-plugindir="$$dc_install_base/$(plugindir)"

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
AM_MAKEFLAGS = --no-print-directory

SUBDIRS = po

AM_CFLAGS += \
	-DLOCALEDIR=\"$(localedir)\" \
	-I $(srcdir)/include

# Plugin
plugin_LTLIBRARIES = \
	pidgin-single-window.la

pidgin_single_window_la_SOURCES = \
	include/config.h \
	src/single-window.c

pidgin_single_window_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(PIDGIN_CFLAGS) \
	$(GTK_CFLAGS) \
	$(GLIB_CFLAGS)

pidgin_single_window_la_LDFLAGS = \
	-no-undefined \
	-avoid-version -module \
	-export-symbols-regex 'purple_init_(pidgin_single_window_)?plugin'

pidgin_single_window_la_LIBADD = \
	$(PIDGIN_LIBS) \
	$(GTK_LIBS) \
	$(GLIB_LIBS)


#
# Hooks
#

install-data-hook: la-files-install-hook
uninstall-hook: la-files-uninstall-hook

# *.la files cleanup
la-files-install-hook:
	cd $(DESTDIR)$(plugindir) && \
		rm $(plugin_LTLIBRARIES)

# Remove shared object files at uninstall since
# we remove *.la files at install
la-files-uninstall-hook:
	cd $(DESTDIR)$(plugindir) && \
		rm $(plugin_LTLIBRARIES:.la=$(shrext_cmds))
	-rmdir $(DESTDIR)$(plugindir)
