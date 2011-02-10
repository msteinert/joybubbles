/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/animation/scale.h"
#include "joy/bubble.h"

G_DEFINE_TYPE(JoyAnimationScale, joy_animation_scale, JOY_TYPE_ANIMATION)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_ANIMATION_SCALE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyAnimationScale *)instance)->priv)

struct Private {
	gdouble factor;
	gint width;
	gint end_width;
	gint height;
	gint end_height;
};

static void
joy_animation_scale_init(JoyAnimationScale *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

enum Properties {
	PROP_0 = 0,
	PROP_FACTOR,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_FACTOR:
		priv->factor = g_value_get_double(value);
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
	priv->width = joy_bubble_get_width(widget);
	priv->end_width = priv->width * priv->factor;
	priv->height = joy_bubble_get_height(widget);
	priv->end_height = priv->height * priv->factor;
}

static void
frame(JoyAnimation *self, JoyBubble *widget, gdouble percent)
{
	struct Private *priv = GET_PRIVATE(self);
	gint width = (gdouble)(priv->end_width - priv->width) * percent;
	gint height = (gdouble)(priv->end_height - priv->height) * percent;
	joy_bubble_resize(widget, priv->width + width, priv->height + height);
}

static void
joy_animation_scale_class_init(JoyAnimationScaleClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = set_property;
	JoyAnimationClass *animation_class = JOY_ANIMATION_CLASS(klass);
	animation_class->start = start;
	animation_class->frame = frame;
	g_type_class_add_private(klass, sizeof(struct Private));
	g_object_class_install_property(object_class, PROP_FACTOR,
		g_param_spec_double("factor", Q_("Factor"),
			Q_("The amount the widget should be scaled"),
			0., G_MAXDOUBLE, 0.,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoyAnimation *
joy_animation_scale_new(JoyBubble *widget, gdouble factor)
{
	return g_object_new(JOY_TYPE_ANIMATION_SCALE,
			"widget", widget,
			"factor", factor,
			NULL);
}
