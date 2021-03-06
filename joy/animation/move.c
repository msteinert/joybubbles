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
#include "joy/animation/move.h"
#include "joy/bubble.h"

G_DEFINE_TYPE(JoyAnimationMove, joy_animation_move, JOY_TYPE_ANIMATION)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_ANIMATION_MOVE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyAnimationMove *)instance)->priv)

struct Private {
	gint start_x;
	gint x;
	gint start_y;
	gint y;
	gboolean buffered;
};

static void
joy_animation_move_init(JoyAnimationMove *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

enum Properties {
	PROP_0 = 0,
	PROP_X_COORDINATE,
	PROP_Y_COORDINATE,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_X_COORDINATE:
		priv->x = g_value_get_int(value);
		break;
	case PROP_Y_COORDINATE:
		priv->y = g_value_get_int(value);
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
	priv->start_x = joy_bubble_get_x(widget);
	priv->start_y = joy_bubble_get_y(widget);
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
advance(JoyAnimation *self, JoyBubble *widget, gdouble percent)
{
	struct Private *priv = GET_PRIVATE(self);
	gint x = priv->start_x +
		((gdouble)(priv->x - priv->start_x) * percent);
	gint y = priv->start_y +
		((gdouble)(priv->y - priv->start_y) * percent);
	joy_bubble_move(widget, x, y);
}

static void
joy_animation_move_class_init(JoyAnimationMoveClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = set_property;
	JoyAnimationClass *animation_class = JOY_ANIMATION_CLASS(klass);
	animation_class->start = start;
	animation_class->stop = stop;
	animation_class->advance = advance;
	g_type_class_add_private(klass, sizeof(struct Private));
	g_object_class_install_property(object_class, PROP_X_COORDINATE,
		g_param_spec_int("x-coordinate", Q_("X-Coordinate"),
			Q_("The x-coordinate to move to"),
			G_MININT, G_MAXINT, 0, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_Y_COORDINATE,
		g_param_spec_int("y-coordinate", Q_("Y-Coordinate"),
			Q_("The y-coordinate to move to"),
			G_MININT, G_MAXINT, 0, G_PARAM_WRITABLE));
}

JoyAnimation *
joy_animation_move_new(JoyBubble *widget, gint x, gint y)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(widget), NULL);
	return g_object_new(JOY_TYPE_ANIMATION_MOVE,
			"widget", widget,
			"x-coordinate", x,
			"y-coordinate", y,
			NULL);
}

void
joy_animation_move_set_x(JoyAnimation *self, gint x)
{
	g_return_if_fail(JOY_IS_ANIMATION_MOVE(self));
	GET_PRIVATE(self)->x = x;
}

void
joy_animation_move_set_y(JoyAnimation *self, gint y)
{
	g_return_if_fail(JOY_IS_ANIMATION_MOVE(self));
	GET_PRIVATE(self)->y = y;
}
