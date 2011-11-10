/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/screen.h"
#include "joy/window.h"

G_DEFINE_ABSTRACT_TYPE(JoyWindow, joy_window, JOY_TYPE_CONTAINER)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_WINDOW, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyWindow *)instance)->priv)

struct Private {
	JoyScreen *screen;
};

static void
joy_window_init(JoyWindow *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->screen) {
		g_object_unref(priv->screen);
		priv->screen = NULL;
	}
	G_OBJECT_CLASS(joy_window_parent_class)->dispose(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_SCREEN,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_SCREEN:
		priv->screen = g_value_dup_object(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static JoyApplication *
get_application(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->screen)) {
		return joy_screen_get_application(priv->screen);
	}
	return NULL;
}

static JoyScreen *
get_screen(JoyBubble *self)
{
	return GET_PRIVATE(self)->screen;
}

static JoyBubble *
get_window(JoyBubble *self)
{
	return self;
}

static void
set_buffered(JoyBubble *self, gboolean buffered)
{
	// windows are always double-buffered
}

static gboolean
get_buffered(JoyBubble *self)
{
	return TRUE;
}

static gboolean
draw(JoyBubble *self, cairo_t *cr)
{
	// clear exposed areas
	cairo_save(cr);
	cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
	cairo_paint(cr);
	cairo_restore(cr);
	return JOY_BUBBLE_CLASS(joy_window_parent_class)->draw(self, cr);
}

static void
joy_window_class_init(JoyWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->set_property = set_property;
	JoyBubbleClass *bubble_class = JOY_BUBBLE_CLASS(klass);
	bubble_class->get_application = get_application;
	bubble_class->get_screen = get_screen;
	bubble_class->get_window = get_window;
	bubble_class->set_buffered = set_buffered;
	bubble_class->get_buffered = get_buffered;
	bubble_class->draw = draw;
	g_type_class_add_private(klass, sizeof(struct Private));
	// properties
	g_object_class_install_property(object_class, PROP_SCREEN,
		g_param_spec_object("screen", Q_("Screen"),
			Q_("The screen this window is on"), JOY_TYPE_SCREEN,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoyBubble *
joy_window_new(JoyScreen *screen)
{
	return joy_screen_window_create(screen);
}

cairo_surface_t *
joy_window_cairo_surface_create(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_WINDOW(self), NULL);
	return JOY_WINDOW_GET_CLASS(self)->cairo_surface_create(self);
}

void
joy_window_raise(JoyBubble *self)
{
	g_return_if_fail(JOY_IS_WINDOW(self));
	JOY_WINDOW_GET_CLASS(self)->raise(self);
}

void
joy_window_lower(JoyBubble *self)
{
	g_return_if_fail(JOY_IS_WINDOW(self));
	JOY_WINDOW_GET_CLASS(self)->raise(self);
}

