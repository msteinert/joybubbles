/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/animation/resize.h"
#include "joy/bubble.h"

G_DEFINE_TYPE(JoyAnimationResize, joy_animation_resize, JOY_TYPE_ANIMATION)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_ANIMATION_RESIZE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyAnimationResize *)instance)->priv)

struct Private {
	gint start_width;
	gint width;
	gint start_height;
	gint height;
	gboolean buffered;
};

static void
joy_animation_resize_init(JoyAnimationResize *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

enum Properties {
	PROP_0 = 0,
	PROP_WIDTH,
	PROP_HEIGHT,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_WIDTH:
		priv->width = g_value_get_int(value);
		break;
	case PROP_HEIGHT:
		priv->height = g_value_get_int(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
start(JoyAnimation *self, JoyBubble *widget)
{
	struct Private *priv = GET_PRIVATE(self);
	priv->start_width = joy_bubble_get_width(widget);
	priv->start_height = joy_bubble_get_height(widget);
	priv->buffered = joy_bubble_get_buffered(widget);
	if (priv->buffered) {
		joy_bubble_set_buffered(widget, FALSE);
	}
}

static void
stop(JoyAnimation *self, JoyBubble *widget)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->buffered) {
		joy_bubble_set_buffered(widget, TRUE);
	}
}

static void
frame(JoyAnimation *self, JoyBubble *widget, gdouble percent)
{
	struct Private *priv = GET_PRIVATE(self);
	gint width = priv->start_width +
		((gdouble)(priv->width - priv->start_width) * percent);
	gint height = priv->start_height +
		((gdouble)(priv->height - priv->start_height) * percent);
	joy_bubble_resize(widget, width, height);
}

static void
joy_animation_resize_class_init(JoyAnimationResizeClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = set_property;
	JoyAnimationClass *animation_class = JOY_ANIMATION_CLASS(klass);
	animation_class->start = start;
	animation_class->stop = stop;
	animation_class->frame = frame;
	g_type_class_add_private(klass, sizeof(struct Private));
	g_object_class_install_property(object_class, PROP_WIDTH,
		g_param_spec_int("width", Q_("Width"),
			Q_("The width to resize to"),
			0, G_MAXINT, 0,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_HEIGHT,
		g_param_spec_int("height", Q_("Height"),
			Q_("The height to resize to"),
			0, G_MAXINT, 0,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoyAnimation *
joy_animation_resize_new(JoyBubble *widget, gint width, gint height)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(widget), NULL);
	return g_object_new(JOY_TYPE_ANIMATION_RESIZE,
			"widget", widget,
			"width", width,
			"height", height,
			NULL);
}

void
joy_animation_resize_set_width(JoyAnimation *self, gint width)
{
	g_return_if_fail(JOY_IS_ANIMATION_RESIZE(self));
	g_return_if_fail(width >= 0);
	GET_PRIVATE(self)->width = width;
}

void
joy_animation_resize_set_height(JoyAnimation *self, gint height)
{
	g_return_if_fail(JOY_IS_ANIMATION_RESIZE(self));
	g_return_if_fail(height >= 0);
	GET_PRIVATE(self)->height = height;
}
