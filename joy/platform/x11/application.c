/* Copyright 2011, 2012 Michael Steinert
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *
 *     2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *
 *     3. Neither the name of the Joybubbles project nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "joy/error.h"
#include "joy/iterator/ptr-array.h"
#include "joy/platform/x11/application.h"
#include "joy/platform/x11/keyboard.h"
#include "joy/platform/x11/mouse.h"
#include "joy/platform/x11/screen.h"
#include "joy/platform/x11/source.h"
#include <X11/Xutil.h>

G_DEFINE_TYPE(JoyX11Application, joy_x11_application, JOY_TYPE_APPLICATION)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_X11_APPLICATION, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyX11Application *)instance)->priv)

struct Private {
	Display *display;
	GPtrArray *screens;
	GHashTable *xids;
	JoyDevice *keyboard;
	JoyDevice *mouse;
	gchar *display_name;
	gdouble refresh;
};

static void
joy_x11_application_init(JoyX11Application *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->xids = g_hash_table_new(g_direct_hash, g_direct_equal);
	priv->refresh = JOY_REFRESH;
}

static void
destroy(gpointer object)
{
	g_object_run_dispose(G_OBJECT(object));
	g_object_unref(object);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->screens) {
		g_ptr_array_free(priv->screens, TRUE);
		priv->screens = NULL;
	}
	if (priv->keyboard) {
		g_object_run_dispose(G_OBJECT(priv->keyboard));
		g_object_unref(priv->keyboard);
		priv->keyboard = NULL;
	}
	if (priv->mouse) {
		g_object_run_dispose(G_OBJECT(priv->mouse));
		g_object_unref(priv->mouse);
		priv->mouse = NULL;
	}
	G_OBJECT_CLASS(joy_x11_application_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->xids) {
		g_hash_table_destroy(priv->xids);
		priv->xids = NULL;
	}
	if (priv->display) {
		XCloseDisplay(priv->display);
	}
	if (priv->display_name) {
		g_free(priv->display_name);
	}
	G_OBJECT_CLASS(joy_x11_application_parent_class)->finalize(base);
}

static JoyScreen *
get_screen(JoyApplication *self, guint n)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->screens && (n < priv->screens->len))) {
		return g_ptr_array_index(priv->screens, n);
	}
	return NULL;
}

static JoyScreen *
get_default_screen(JoyApplication *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->display)) {
		return NULL;
	}
	gint n = DefaultScreen(priv->display);
	if (G_LIKELY(priv->screens && (n < (gint)priv->screens->len))) {
		return g_ptr_array_index(priv->screens, n);
	}
	return NULL;
}

static JoyIterator *
begin(JoyApplication *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->screens)) {
		return joy_iterator_ptr_array_begin(priv->screens);
	}
	return NULL;
}

static gboolean
arg_display_cb(G_GNUC_UNUSED const gchar *key,
	       const gchar *value,
	       gpointer self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->display_name) {
		g_free(priv->display_name);
	}
	priv->display_name = g_strdup(value);
	return TRUE;
}

static gboolean
arg_refresh_cb(G_GNUC_UNUSED const gchar *key,
	       const gchar *value,
	       gpointer self)
{
	struct Private *priv = GET_PRIVATE(self);
	errno = 0;
	double refresh = g_ascii_strtod(value, NULL);
	if (errno) {
		g_message(Q_("%s: not a valid refresh rate"), value);
		return FALSE;
	}
	priv->refresh = refresh;
	return TRUE;
}

static const GOptionEntry const x11_arguments[] = {
	{
		"x11-display",
		'\0',
		0,
		G_OPTION_ARG_CALLBACK,
		arg_display_cb,
		N_("The X display to use"),
		N_("NAME")
	},
	{
		"x11-refresh",
		'\0',
		0,
		G_OPTION_ARG_CALLBACK,
		arg_refresh_cb,
		N_("The refresh rate [" G_STRINGIFY(JOY_REFRESH) " Hz]"),
		N_("RATE")
	},
	{ NULL, '\0', 0, G_OPTION_ARG_NONE, NULL, NULL, NULL }
};

static gboolean
post_hook(G_GNUC_UNUSED GOptionContext *context,
	  G_GNUC_UNUSED GOptionGroup *group,
	  gpointer self,
	  GError **error)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->display) {
		return TRUE;
	}
	const gchar *display_name = NULL;
	if (priv->display_name) {
		display_name = priv->display_name;
	} else if (g_getenv("DISPLAY")) {
		display_name = g_getenv("DISPLAY");
	}
	priv->display = XOpenDisplay(display_name);
	if (G_UNLIKELY(!priv->display)) {
		g_set_error(error, JOY_ERROR, JOY_ERROR_FAILURE,
				"x11: Failed to open display '%s'",
				display_name);
		return FALSE;
	}
	// initialize screen(s)
	priv->screens = g_ptr_array_sized_new(ScreenCount(priv->display));
	if (priv->screens) {
		g_ptr_array_set_free_func(priv->screens, destroy);
		for (gint n = 0; n < ScreenCount(priv->display); ++n) {
			JoyScreen *screen = joy_x11_screen_new(self, n);
			joy_x11_screen_set_refresh(screen, priv->refresh);
			g_ptr_array_add(priv->screens, screen);
		}
	}
	joy_application_add_source(self, joy_x11_source_new(self));
	priv->keyboard = joy_x11_keyboard_new();
	priv->mouse = joy_x11_mouse_new();
	joy_device_keyboard_set_mouse(priv->keyboard, priv->mouse);
	joy_device_mouse_set_keyboard(priv->mouse, priv->keyboard);
	return TRUE;
}

static void
add_options(JoyApplication *self, GOptionContext *context)
{
	GOptionGroup *group = g_option_group_new("x11",
			Q_("X11 Options"),
			Q_("Show X11 Options"), self, NULL);
	if (!group) {
		return;
	}
	g_option_group_set_parse_hooks(group, NULL, post_hook);
	g_option_group_add_entries(group, x11_arguments);
	g_option_context_add_group(context, group);
}

static void
joy_x11_application_class_init(JoyX11ApplicationClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	JoyApplicationClass *application_class = JOY_APPLICATION_CLASS(klass);
	application_class->get_screen = get_screen;
	application_class->get_default_screen = get_default_screen;
	application_class->begin = begin;
	application_class->add_options = add_options;
	g_type_class_add_private(klass, sizeof(struct Private));
}

JoyApplication *
joy_platform_application_new(void)
{
	return g_object_new(JOY_TYPE_X11_APPLICATION,
			NULL);
}

Display *
joy_x11_application_get_display(JoyApplication *self)
{
	g_return_val_if_fail(JOY_IS_X11_APPLICATION(self), NULL);
	return GET_PRIVATE(self)->display;
}

void
joy_x11_application_insert_xid(JoyApplication *self, XID id,
		gpointer data)
{
	g_return_if_fail(JOY_IS_X11_APPLICATION(self));
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->xids)) {
		return;
	}
	if (G_UNLIKELY(g_hash_table_lookup(priv->xids, (gpointer)id))) {
		g_message(Q_("x11: duplicate XID"));
	}
	g_hash_table_insert(priv->xids, (gpointer)id, data);
}

void
joy_x11_application_remove_xid(JoyApplication *self, XID id)
{
	g_return_if_fail(JOY_IS_X11_APPLICATION(self));
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->xids)) {
		return;
	}
	g_hash_table_remove(priv->xids, (gpointer)id);
}

gpointer
joy_x11_application_lookup_xid(JoyApplication *self, XID id)
{
	g_return_val_if_fail(JOY_IS_X11_APPLICATION(self), NULL);
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->xids)) {
		return NULL;
	}
	return g_hash_table_lookup(priv->xids, (gpointer)id);
}

JoyDevice *
joy_x11_application_get_mouse(JoyApplication *self)
{
	g_return_val_if_fail(JOY_IS_X11_APPLICATION(self), NULL);
	return GET_PRIVATE(self)->mouse;
}

JoyDevice *
joy_x11_application_get_keyboard(JoyApplication *self)
{
	g_return_val_if_fail(JOY_IS_X11_APPLICATION(self), NULL);
	return GET_PRIVATE(self)->keyboard;
}
