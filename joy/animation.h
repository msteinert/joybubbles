/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief The main interface
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_ANIMATION_H
#define JOY_ANIMATION_H

#include <glib-object.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_ANIMATION \
	(joy_animation_get_type())

#define JOY_ANIMATION(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_ANIMATION, \
		JoyAnimation))

#define JOY_IS_ANIMATION(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_ANIMATION))

#define JOY_ANIMATION_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_ANIMATION, \
		JoyAnimationClass))

#define JOY_IS_ANIMATION_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_ANIMATION))

#define JOY_ANIMATION_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_ANIMATION, \
		JoyAnimationClass))

typedef struct JoyAnimationClass_ JoyAnimationClass;

struct JoyAnimation_ {
	/*< private >*/
	GObject parent_instance;
	gpointer priv;
};

typedef void
(*JoyAnimationStart)(JoyAnimation *self, JoyBubble *widget);

typedef void
(*JoyAnimationStop)(JoyAnimation *self, JoyBubble *widget);

typedef void
(*JoyAnimationPause)(JoyAnimation *self, JoyBubble *widget);

typedef void
(*JoyAnimationFrame)(JoyAnimation *self, JoyBubble *widget, gdouble percent);

struct JoyAnimationClass_ {
	/*< private >*/
	GObjectClass parent_class;
	/*< public >*/
	JoyAnimationStart start;
	JoyAnimationStop stop;
	JoyAnimationPause pause;
	JoyAnimationFrame frame;
};

G_GNUC_NO_INSTRUMENT
GType
joy_animation_get_type(void) G_GNUC_CONST;

/**
 * \brief Set the animation duration.
 */
void
joy_animation_set_duration(JoyAnimation *self, gdouble seconds);

/**
 * \brief Set infinite looping.
 */
void
joy_animation_set_looping(JoyAnimation *self);

/**
 * \brief Set the number of times the animation should loop.
 */
void
joy_animation_set_loop_count(JoyAnimation *self, gint count);

/**
 * \brief Start an animation.
 */
void
joy_animation_start(JoyAnimation *self);

/**
 * \brief Stop an animation.
 */
void
joy_animation_stop(JoyAnimation *self);

/**
 * \brief Stop an animation.
 */
void
joy_animation_stop(JoyAnimation *self);

/**
 * \brief Pause an animation.
 */
void
joy_animation_pause(JoyAnimation *self);

/**
 * \brief Advance the frame.
 */
void
joy_animation_frame(JoyAnimation *self);

G_END_DECLS

#endif // JOY_ANIMATION_H
