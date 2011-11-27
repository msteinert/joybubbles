/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/animation/parallel.h"
#include "joy/bubble.h"

G_DEFINE_TYPE(JoyAnimationParallel, joy_animation_parallel, \
	JOY_TYPE_ANIMATION)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_ANIMATION_PARALLEL, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyAnimationParallel *)instance)->priv)

struct Private {
	GQueue *children;
	guint count;
};

static void
joy_animation_parallel_init(JoyAnimationParallel *self)
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
	G_OBJECT_CLASS(joy_animation_parallel_parent_class)->dispose(base);
}

static void
start(JoyAnimation *self, JoyBubble *widget)
{
	struct Private *priv = GET_PRIVATE(self);
	for (GList *node = g_queue_peek_head_link(priv->children); node;
			node = node->next) {
		joy_animation_start(node->data);
	}
}

static void
stop(JoyAnimation *self, JoyBubble *widget)
{
	struct Private *priv = GET_PRIVATE(self);
	for (GList *node = g_queue_peek_head_link(priv->children); node;
			node = node->next) {
		joy_animation_stop(node->data);
	}
}

static void
pause(JoyAnimation *self, JoyBubble *widget)
{
	struct Private *priv = GET_PRIVATE(self);
	for (GList *node = g_queue_peek_head_link(priv->children); node;
			node = node->next) {
		joy_animation_pause(node->data);
	}
}

static void
joy_animation_parallel_class_init(JoyAnimationParallelClass *klass)
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
joy_animation_parallel_new(void)
{
	return g_object_new(JOY_TYPE_ANIMATION_PARALLEL,
			"duration", 0.,
			NULL);
}

static void
on_stop(JoyAnimation *child, JoyBubble *widget, JoyAnimation *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (++priv->count == g_queue_get_length(priv->children)) {
		joy_animation_stop(self);
	}
}

void
joy_animation_parallel_add(JoyAnimation *self, JoyAnimation *child)
{
	g_return_if_fail(JOY_IS_ANIMATION(self));
	g_return_if_fail(JOY_IS_ANIMATION(child));
	g_signal_connect(child, "stop", G_CALLBACK(on_stop), self);
	g_queue_push_tail(GET_PRIVATE(self)->children, g_object_ref(child));
}
