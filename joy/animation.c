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
#include "joy/animation.h"
#include "joy/bubble.h"
#include "joy/marshal.h"
#include "joy/screen.h"

G_DEFINE_ABSTRACT_TYPE(JoyAnimation, joy_animation, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_ANIMATION, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyAnimation *)instance)->priv)

typedef enum {
	JOY_ANIMATION_START,
	JOY_ANIMATION_STOP,
	JOY_ANIMATION_PAUSE
} JoyAnimationState;

enum Signals {
	SIGNAL_START,
	SIGNAL_STOP,
	SIGNAL_PAUSE,
	SIGNAL_ADVANCE,
	SIGNAL_LAST
};

static guint signals[SIGNAL_LAST] = { 0 };

struct Private {
	JoyBubble *widget;
	JoyAnimationState state;
	gulong elapsed;
	gulong duration;
	gboolean looping;
	gint loop;
	gint count;
	gpointer object;
	JoyAnimationEasing function;
};

static void
joy_animation_init(JoyAnimation *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->state = JOY_ANIMATION_STOP;
	priv->duration = .25 * 1e6;
}

enum Properties {
	PROP_0 = 0,
	PROP_WIDGET,
	PROP_DURATION,
	PROP_LOOP_COUNT,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_WIDGET:
		priv->widget = g_value_get_object(value);
		break;
	case PROP_DURATION:
		priv->duration = g_value_get_double(value);
		break;
	case PROP_LOOP_COUNT:
		priv->loop = g_value_get_int(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
joy_animation_class_init(JoyAnimationClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = set_property;
	g_type_class_add_private(klass, sizeof(struct Private));
	// JoyAnimation::start
	signals[SIGNAL_START] =
		g_signal_new(g_intern_static_string("start"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(JoyAnimationClass, start),
			NULL, NULL, joy_marshal_VOID__OBJECT, G_TYPE_NONE,
			1, G_TYPE_OBJECT);
	// JoyAnimation::stop
	signals[SIGNAL_STOP] =
		g_signal_new(g_intern_static_string("stop"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(JoyAnimationClass, stop),
			NULL, NULL, joy_marshal_VOID__OBJECT, G_TYPE_NONE,
			1, G_TYPE_OBJECT);
	// JoyAnimation::pause
	signals[SIGNAL_PAUSE] =
		g_signal_new(g_intern_static_string("pause"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(JoyAnimationClass, pause),
			NULL, NULL, joy_marshal_VOID__OBJECT, G_TYPE_NONE,
			1, G_TYPE_OBJECT);
	// JoyAnimation::advance
	signals[SIGNAL_ADVANCE] =
		g_signal_new(g_intern_static_string("advance"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(JoyAnimationClass, advance),
			NULL, NULL, joy_marshal_VOID__OBJECT_DOUBLE,
			G_TYPE_NONE, 2, G_TYPE_OBJECT, G_TYPE_DOUBLE);
	g_object_class_install_property(object_class, PROP_WIDGET,
		g_param_spec_object("widget", Q_("Widget"),
			Q_("The widget this animation is operating on"),
			JOY_TYPE_BUBBLE,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_DURATION,
		g_param_spec_double("duration", Q_("Duration"),
			Q_("The duration of this animation"),
			0., G_MAXDOUBLE, .25, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_LOOP_COUNT,
		g_param_spec_int("loop-count", Q_("Loop Count"),
			Q_("The number times this animation should loop"),
			G_MININT, G_MAXINT, 1, G_PARAM_WRITABLE));
}

JoyBubble *
joy_animation_get_widget(JoyAnimation *self)
{
	g_return_val_if_fail(JOY_IS_ANIMATION(self), NULL);
	return GET_PRIVATE(self)->widget;
}

void
joy_animation_set_duration(JoyAnimation *self, gdouble seconds)
{
	g_return_if_fail(JOY_IS_ANIMATION(self));
	g_return_if_fail(0. < seconds);
	GET_PRIVATE(self)->duration = seconds * 1e6;
}

gdouble
joy_animation_get_duration(JoyAnimation *self)
{
	g_return_val_if_fail(JOY_IS_ANIMATION(self), 0.);
	return 1.e-6 * GET_PRIVATE(self)->duration;
}

void
joy_animation_set_looping(JoyAnimation *self, gboolean looping)
{
	g_return_if_fail(JOY_IS_ANIMATION(self));
	GET_PRIVATE(self)->looping = looping;
}

void
joy_animation_set_easing(JoyAnimation *self, JoyAnimationEasing function,
		gpointer object)
{
	g_return_if_fail(JOY_IS_ANIMATION(self));
	struct Private *priv = GET_PRIVATE(self);
	priv->function = function;
	priv->object = object;
}

void
joy_animation_set_loop_count(JoyAnimation *self, gint count)
{
	g_return_if_fail(JOY_IS_ANIMATION(self));
	g_return_if_fail(count);
	GET_PRIVATE(self)->loop = count;
}

void
joy_animation_start(JoyAnimation *self)
{
	g_return_if_fail(JOY_IS_ANIMATION(self));
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->widget)) {
		return;
	}
	if (JOY_ANIMATION_START != priv->state) {
		if (JOY_ANIMATION_STOP == priv->state) {
			JoyScreen *screen;
			screen = joy_bubble_get_screen(priv->widget);
			if (G_LIKELY(screen)) {
				joy_screen_add_animation(screen, self);
			}
		}
		g_signal_emit(self, signals[SIGNAL_START], 0, priv->widget);
		priv->state = JOY_ANIMATION_START;
		priv->elapsed = 0.;
	}
}

void
joy_animation_stop(JoyAnimation *self)
{
	g_return_if_fail(JOY_IS_ANIMATION(self));
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->widget)) {
		return;
	}
	if (JOY_ANIMATION_STOP != priv->state) {
		JoyScreen *screen;
		screen = joy_bubble_get_screen(priv->widget);
		if (G_LIKELY(screen)) {
			joy_screen_remove_animation(screen, self);
		}
		g_signal_emit(self, signals[SIGNAL_STOP], 0, priv->widget);
		priv->state = JOY_ANIMATION_STOP;
	}
}

void
joy_animation_pause(JoyAnimation *self)
{
	g_return_if_fail(JOY_IS_ANIMATION(self));
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->widget)) {
		return;
	}
	if (JOY_ANIMATION_START == priv->state) {
		g_signal_emit(self, signals[SIGNAL_PAUSE], 0, priv->widget);
		priv->state = JOY_ANIMATION_PAUSE;
	}
}

void
joy_animation_advance(JoyAnimation *self, gulong frame)
{
	g_return_if_fail(JOY_IS_ANIMATION(self));
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(0 == priv->duration)) {
		return;
	}
	if (G_UNLIKELY(JOY_ANIMATION_START != priv->state)) {
		return;
	}
	if (G_UNLIKELY(!priv->widget)) {
		return;
	}
	priv->elapsed += frame;
	gdouble percent = (gdouble)priv->elapsed / (gdouble)priv->duration;
	if (G_UNLIKELY(1. < percent)) {
		percent = 1.;
	}
	g_signal_emit(self, signals[SIGNAL_ADVANCE], 0, priv->widget,
			priv->function
			? priv->function(priv->object, percent)
			: percent);
	if (G_UNLIKELY(1. == percent)) {
		if (++priv->count >= priv->loop && !priv->looping) {
			joy_animation_stop(self);
		} else {
			joy_animation_start(self);
			if (G_MAXINT == priv->count) {
				priv->count = 0;
			}
		}
	}
}
