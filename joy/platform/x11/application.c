/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/iterator/ptr-array.h"
#include "joy/macros.h"
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
};

static void
joy_x11_application_init(JoyX11Application *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->xids = g_hash_table_new(g_direct_hash, g_direct_equal);
}

static void
destroy(gpointer object)
{
	g_object_run_dispose(G_OBJECT(object));
	g_object_unref(object);
}

static void
constructed(GObject *base)
{
	JoyApplication *self = JOY_APPLICATION(base);
	struct Private *priv = GET_PRIVATE(base);
	priv->display = XOpenDisplay(NULL);
	if (G_UNLIKELY(!priv->display)) {
		goto exit;
	}
	// initialize screen(s)
	priv->screens = g_ptr_array_sized_new(ScreenCount(priv->display));
	if (priv->screens) {
		g_ptr_array_set_free_func(priv->screens, destroy);
		for (gint n = 0; n < ScreenCount(priv->display); ++n) {
			JoyBubble *screen = joy_x11_screen_new(self, n);
			g_ptr_array_add(priv->screens, screen);
		}
	}
	joy_application_add_source(self, joy_x11_source_new(self));
	priv->keyboard = joy_x11_keyboard_new();
	priv->mouse = joy_x11_mouse_new();
	joy_device_keyboard_set_mouse(priv->keyboard, priv->mouse);
	joy_device_mouse_set_keyboard(priv->mouse, priv->keyboard);
exit:
	if (G_OBJECT_CLASS(joy_x11_application_parent_class)->constructed) {
		G_OBJECT_CLASS(joy_x11_application_parent_class)->
			constructed(base);
	}
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
	if (G_LIKELY(priv->screens && (n < priv->screens->len))) {
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
arg_display_cb(const gchar *key, const gchar *value, gpointer self)
{
	JOY_UNIMPLEMENTED;
	return TRUE;
}

static const GOptionEntry const x11_arguments[] = {
	{ "x11-display", '\0', 0,
		G_OPTION_ARG_CALLBACK, arg_display_cb,
		N_("The X11 display to use"),
		N_("NAME") },
	{ NULL }
};

static void
add_options(JoyApplication *self, GOptionContext *context)
{
	GOptionGroup *group = g_option_group_new("x11",
			Q_("X11 Options"),
			Q_("Show X11 Options"), self, NULL);
	if (!group) {
		return;
	}
	g_option_group_add_entries(group, x11_arguments);
	g_option_context_add_group(context, group);
}

static void
joy_x11_application_class_init(JoyX11ApplicationClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructed = constructed;
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
