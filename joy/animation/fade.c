/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/animation/fade.h"
#include "joy/bubble.h"

G_DEFINE_TYPE(JoyAnimationFade, joy_animation_fade, JOY_TYPE_ANIMATION)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_ANIMATION_FADE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyAnimationFade *)instance)->priv)

struct Private {
	gdouble alpha;
	gdouble start_alpha;
	gboolean buffered;
};

static void
joy_animation_fade_init(JoyAnimationFade *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

enum Properties {
	PROP_0 = 0,
	PROP_ALPHA,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_ALPHA:
		priv->alpha  = g_value_get_double(value);
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
	if (joy_bubble_get_visible(widget)) {
		priv->start_alpha = joy_bubble_get_alpha(widget);
	} else {
		priv->start_alpha = 0.;
	}
	priv->buffered = joy_bubble_get_buffered(widget);
	if (!priv->buffered) {
		joy_bubble_set_buffered(widget, TRUE);
	}
}

static void
stop(JoyAnimation *self, JoyBubble *widget)
{
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->buffered) {
		joy_bubble_set_buffered(widget, FALSE);
	}
}

static void
frame(JoyAnimation *self, JoyBubble *widget, gdouble percent)
{
	struct Private *priv = GET_PRIVATE(self);
	gdouble alpha = priv->start_alpha +
		((priv->alpha - priv->start_alpha) * percent);
	joy_bubble_set_alpha(widget, alpha);
}

static void
joy_animation_fade_class_init(JoyAnimationFadeClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = set_property;
	JoyAnimationClass *animation_class = JOY_ANIMATION_CLASS(klass);
	animation_class->start = start;
	animation_class->stop = stop;
	animation_class->frame = frame;
	g_type_class_add_private(klass, sizeof(struct Private));
	g_object_class_install_property(object_class, PROP_ALPHA,
		g_param_spec_double("alpha", Q_("Alpha"),
			Q_("The alpha value to fade to"),
			0., 1., 0., G_PARAM_WRITABLE));
}

JoyAnimation *
joy_animation_fade_new(JoyBubble *widget, gdouble alpha)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(widget), NULL);
	return g_object_new(JOY_TYPE_ANIMATION_FADE,
			"widget", widget,
			"alpha", alpha,
			NULL);
}

void
joy_animation_fade_set_alpha(JoyAnimation *self, gdouble alpha)
{
	g_return_if_fail(JOY_IS_ANIMATION_FADE(self));
	g_return_if_fail(alpha >= 0. && 1. >= alpha);
	GET_PRIVATE(self)->alpha = alpha;
}
