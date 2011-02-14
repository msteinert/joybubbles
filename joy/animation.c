/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
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
	SIGNAL_FRAME,
	SIGNAL_LAST
};

static guint signals[SIGNAL_LAST] = { 0 };

struct Private {
	JoyBubble *widget;
	JoyAnimationState state;
	gdouble duration;
	GTimer *timer;
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
	priv->duration = .25;
	priv->timer = g_timer_new();
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->widget) {
		g_object_unref(priv->widget);
		priv->widget = NULL;
	}
	G_OBJECT_CLASS(joy_animation_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->timer) {
		g_timer_destroy(priv->timer);
	}
	G_OBJECT_CLASS(joy_animation_parent_class)->finalize(base);
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
		priv->widget = g_value_dup_object(value);
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
	object_class->dispose = dispose;
	object_class->finalize = finalize;
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
	// JoyAnimation::frame
	signals[SIGNAL_FRAME] =
		g_signal_new(g_intern_static_string("frame"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(JoyAnimationClass, frame),
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
	GET_PRIVATE(self)->duration = seconds;
}

gdouble
joy_animation_get_duration(JoyAnimation *self)
{
	g_return_val_if_fail(JOY_IS_ANIMATION(self), 0.);
	return GET_PRIVATE(self)->duration;
}

void
joy_animation_add_duration(JoyAnimation *self, gdouble seconds)
{
	g_return_if_fail(JOY_IS_ANIMATION(self));
	GET_PRIVATE(self)->duration += seconds;
}

void
joy_animation_set_looping(JoyAnimation *self)
{
	g_return_if_fail(JOY_IS_ANIMATION(self));
	GET_PRIVATE(self)->loop = -1;
}

#include "joy/easing.h"

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
	if (JOY_ANIMATION_START != priv->state) {
		if (JOY_ANIMATION_STOP == priv->state) {
			if (G_LIKELY(priv->widget)) {
				JoyScreen *screen;
				screen = joy_bubble_get_screen(priv->widget);
				if (G_LIKELY(screen)) {
					joy_screen_add_animation(screen, self);
				}
			}
		}
		g_timer_start(priv->timer);
		g_signal_emit(self, signals[SIGNAL_START], 0, priv->widget);
		priv->state = JOY_ANIMATION_START;
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
		if (G_LIKELY(priv->widget)) {
			JoyScreen *screen;
			screen = joy_bubble_get_screen(priv->widget);
			if (G_LIKELY(screen)) {
				joy_screen_remove_animation(screen, self);
			}
		}
		g_timer_stop(priv->timer);
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
		g_timer_stop(priv->timer);
		g_signal_emit(self, signals[SIGNAL_PAUSE], 0, priv->widget);
		priv->state = JOY_ANIMATION_PAUSE;
	}
}

void
joy_animation_frame(JoyAnimation *self)
{
	g_return_if_fail(JOY_IS_ANIMATION(self));
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(0. == priv->duration)) {
		return;
	}
	if (G_UNLIKELY(JOY_ANIMATION_START != priv->state)) {
		return;
	}
	if (G_UNLIKELY(!priv->widget)) {
		return;
	}
	gdouble elapsed = g_timer_elapsed(priv->timer, NULL) / priv->duration;
	if (G_UNLIKELY(1. < elapsed)) {
		elapsed = 1.;
	}
	g_signal_emit(self, signals[SIGNAL_FRAME], 0, priv->widget,
			priv->function
			? priv->function(priv->object, elapsed)
			: elapsed);
	if (G_UNLIKELY(1. == elapsed)) {
		if (++priv->count >= priv->loop) {
			joy_animation_stop(self);
		} else {
			joy_animation_start(self);
			if (G_MAXINT == priv->count) {
				priv->count = 0;
			}
		}
	}
}
