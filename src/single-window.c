
/*
 * pidgin-single-window - Merge buddy list and conversations into a single window
 *
 * Copyright © 2011-2012 Quentin "Sardem FF7" Glidic
 *
 * This file is part of pidgin-single-window.
 *
 * pidgin-single-window is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pidgin-single-window is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with pidgin-single-window. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "config.h"
#include <glib.h>
#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>
#include <purple.h>
#include <gtkplugin.h>
#include <gtkblist.h>
#include <gtkconv.h>

static void _pidgin_single_window_init(PurplePlugin *plugin);
static gboolean _pidgin_single_window_load(PurplePlugin *plugin);
static gboolean _pidgin_single_window_unload(PurplePlugin *plugin);


static PurplePluginInfo _pidgin_single_window_plugin_info = {
    .magic          = PURPLE_PLUGIN_MAGIC,
    .major_version  = PURPLE_MAJOR_VERSION,
    .minor_version  = PURPLE_MINOR_VERSION,
    .type           = PURPLE_PLUGIN_STANDARD,
    .ui_requirement = PIDGIN_PLUGIN_TYPE,
    .flags          = 0,
    .dependencies   = NULL,
    .priority       = PURPLE_PRIORITY_DEFAULT,

    .id             = PIDGIN_SINGLE_WINDOW_PLUGIN_ID,
    .name           = NULL,
    .version        = PACKAGE_VERSION,
    .summary        = NULL,
    .description    = NULL,
    .author         = "Quentin \"Sardem FF7\" Glidic <sardemff7+pidgin@sardemff7.net>",
    .homepage       = "http://" PACKAGE_TARNAME ".sardemff7.net/",

    .load           = _pidgin_single_window_load,
    .unload         = _pidgin_single_window_unload,
    .destroy        = NULL,

    .ui_info        = NULL,
    .extra_info     = NULL,
    .prefs_info     = NULL,
    .actions        = NULL
};

PURPLE_INIT_PLUGIN(purple_events, _pidgin_single_window_init, _pidgin_single_window_plugin_info)

static void
_pidgin_single_window_init(PurplePlugin *plugin)
{
#if ENABLE_NLS
    bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
#endif /* ENABLE_NLS */

    _pidgin_single_window_plugin_info.name = _("Single window mode");
    _pidgin_single_window_plugin_info.summary = _("Merge the conversation window to the buddy");
    _pidgin_single_window_plugin_info.description = _("Allows the so-called “single window” mode for Pidgin");
}

static PidginBuddyList *blist = NULL;
static GtkWidget *single_window_hbox = NULL;
static PidginWindow *conv_window = NULL;
static GtkWidget *conv_vbox = NULL;

static void
_pidgin_single_conv_placement(PidginConversation *conv)
{
    if ( conv_window == NULL )
        return;

    pidgin_conv_window_add_gtkconv(conv_window, conv);
    gtk_widget_show(conv_vbox);
}

static void
_pidgin_single_window_hide_buddy_list_window(GtkWidget *widget, gpointer user_data)
{
    gtk_widget_hide(widget);
}

static void
_pidgin_single_window_gtkblist_created(PurpleBuddyList *purple_blist)
{
    if ( blist != NULL )
    {
        /* Cleanup */
    }

    blist = PIDGIN_BLIST(purple_blist);

    gtk_widget_reparent(blist->main_vbox, single_window_hbox);
    gtk_widget_reparent(conv_vbox, single_window_hbox);

    gtk_container_add(GTK_CONTAINER(conv_window->window), single_window_hbox);
    gtk_widget_show(single_window_hbox);
    gtk_widget_show(conv_window->window);


    g_signal_connect(blist->window, "show", G_CALLBACK(_pidgin_single_window_hide_buddy_list_window), NULL);
    gtk_widget_hide(blist->window);
}

static gboolean
_pidgin_single_window_load(PurplePlugin *plugin)
{
    void *blist_handle = pidgin_blist_get_handle();

    purple_signal_connect(
        blist_handle, "gtkblist-created", plugin,
        PURPLE_CALLBACK(_pidgin_single_window_gtkblist_created), NULL
    );

    pidgin_conv_placement_add_fnc("single-window", _("Next to the buddy list"), _pidgin_single_conv_placement);
    purple_prefs_trigger_callback(PIDGIN_PREFS_ROOT "/conversations/placement");

    single_window_hbox = gtk_hbox_new(TRUE, 0);
    conv_window = pidgin_conv_window_new();

    gtk_window_set_title(GTK_WINDOW(conv_window->window), _("Pidgin single window"));

    conv_vbox = g_object_ref(gtk_widget_get_parent(conv_window->notebook));
    gtk_widget_hide(conv_vbox);

    return TRUE;
}

static gboolean
_pidgin_single_window_unload(PurplePlugin *plugin)
{
    void *blist_handle = purple_blist_get_handle();

    if ( blist != NULL )
        gtk_widget_reparent(blist->main_vbox, blist->window);

    g_object_unref(conv_vbox);
    pidgin_conv_window_destroy(conv_window);
    gtk_widget_unref(single_window_hbox);

    pidgin_conv_placement_remove_fnc("single-window");
    purple_prefs_trigger_callback(PIDGIN_PREFS_ROOT "/conversations/placement");

    purple_signal_disconnect(
        blist_handle, "gtkblist-created", plugin,
        PURPLE_CALLBACK(_pidgin_single_window_gtkblist_created)
    );

    return TRUE;
}
