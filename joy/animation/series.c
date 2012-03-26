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
start(JoyAnimation *self, G_GNUC_UNUSED JoyBubble *widget)
{
	struct Private *priv = GET_PRIVATE(self);
	priv->current = g_queue_peek_head_link(priv->children);
	joy_animation_start(priv->current->data);
}

static void
stop(JoyAnimation *self, G_GNUC_UNUSED JoyBubble *widget)
{
	struct Private *priv = GET_PRIVATE(self);
	joy_animation_stop(priv->current->data);
}

static void
pause(JoyAnimation *self, G_GNUC_UNUSED JoyBubble *widget)
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
on_stop(G_GNUC_UNUSED JoyAnimation *child,
	G_GNUC_UNUSED JoyBubble *widget,
	JoyAnimation *self)
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
