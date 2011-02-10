/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Scale animation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_ANIMATION_SCALE_H
#define JOY_ANIMATION_SCALE_H

#include <joy/animation/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_ANIMATION_SCALE \
	(joy_animation_scale_get_type())

#define JOY_ANIMATION_SCALE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_ANIMATION_SCALE, \
		JoyAnimationScale))

#define JOY_IS_ANIMATION_SCALE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_ANIMATION_SCALE))

#define JOY_ANIMATION_SCALE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_ANIMATION_SCALE, \
		JoyAnimationScaleClass))

#define JOY_IS_ANIMATION_SCALE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_ANIMATION_SCALE))

#define JOY_ANIMATION_SCALE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_ANIMATION_SCALE, \
		JoyAnimationScaleClass))

typedef struct JoyAnimationScaleClass_ JoyAnimationScaleClass;

struct JoyAnimationScale_ {
	/*< private >*/
	JoyAnimation parent_instance;
	gpointer priv;
};

struct JoyAnimationScaleClass_ {
	/*< private >*/
	JoyAnimationClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_animation_scale_get_type(void) G_GNUC_CONST;

JoyAnimation *
joy_animation_scale_new(JoyBubble *widget, gdouble factor);

G_END_DECLS

#endif // JOY_ANIMATION_SCALE_H
