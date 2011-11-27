/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/animation/series.h"
#include "joy/bubble.h"

G_DEFINE_TYPE(JoyAnimationSeries, joy_animation_series, JOY_TYPE_ANIMATION)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_ANIMATION_SERIES, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyAnimationSeries *)instance)->priv)

struct Private {
	GQueue *children;
	GList *current;
};

static void
joy_animation_series_init(JoyAnimationSeries *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->children) {
		for (GList *node = g_queue_peek_head_link(priv->children);
				node; node = node->next) {
			g_object_unref(node->data);
		}
		g_queue_free(priv->children);
		priv->children = NULL;
	}
	G_OBJECT_CLASS(joy_animation_series_parent_class)->dispose(base);
}

static void
start(JoyAnimation *self, JoyBubble *widget)
{
	struct Private *priv = GET_PRIVATE(self);
	priv->current = g_queue_peek_head_link(priv->children);
	joy_animation_start(priv->current->data);
}

static void
stop(JoyAnimation *self, JoyBubble *widget)
{
	struct Private *priv = GET_PRIVATE(self);
	joy_animation_stop(priv->current->data);
}

static void
pause(JoyAnimation *self, JoyBubble *widget)
{
	struct Private *priv = GET_PRIVATE(self);
	joy_animation_pause(priv->current->data);
}

static void
joy_animation_series_class_init(JoyAnimationSeriesClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	JoyAnimationClass *animation_class = JOY_ANIMATION_CLASS(klass);
	animation_class->start = start;
	animation_class->stop = stop;
	animation_class->pause = pause;
	g_type_class_add_private(klass, sizeof(struct Private));
}

JoyAnimation *
joy_animation_series_new(void)
{
	return g_object_new(JOY_TYPE_ANIMATION_SERIES,
			"duration", 0.,
			NULL);
}

static void
on_stop(JoyAnimation *child, JoyBubble *widget, JoyAnimation *self)
{
	struct Private *priv = GET_PRIVATE(self);
	priv->current = priv->current->next;
	if (priv->current)  {
		joy_animation_start(priv->current->data);
	} else {
		joy_animation_stop(self);
	}
}

void
joy_animation_series_add(JoyAnimation *self, JoyAnimation *child)
{
	g_return_if_fail(JOY_IS_ANIMATION(self));
	g_return_if_fail(JOY_IS_ANIMATION(child));
	g_signal_connect(child, "stop", G_CALLBACK(on_stop), self);
	g_queue_push_tail(GET_PRIVATE(self)->children, g_object_ref(child));
}
