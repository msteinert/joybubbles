/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Fade animation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_ANIMATION_FADE_H
#define JOY_ANIMATION_FADE_H

#include <joy/animation/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_ANIMATION_FADE \
	(joy_animation_fade_get_type())

#define JOY_ANIMATION_FADE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_ANIMATION_FADE, \
		JoyAnimationFade))

#define JOY_IS_ANIMATION_FADE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_ANIMATION_FADE))

#define JOY_ANIMATION_FADE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_ANIMATION_FADE, \
		JoyAnimationFadeClass))

#define JOY_IS_ANIMATION_FADE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_ANIMATION_FADE))

#define JOY_ANIMATION_FADE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_ANIMATION_FADE, \
		JoyAnimationFadeClass))

typedef struct JoyAnimationFadeClass_ JoyAnimationFadeClass;

struct JoyAnimationFade_ {
	/*< private >*/
	JoyAnimation parent_instance;
	gpointer priv;
};

struct JoyAnimationFadeClass_ {
	/*< private >*/
	JoyAnimationClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_animation_fade_get_type(void) G_GNUC_CONST;

JoyAnimation *
joy_animation_fade_new(JoyBubble *widget, gdouble alpha);

void
joy_animation_fade_set_alpha(JoyAnimation *self, gdouble alpha);

G_END_DECLS

#endif // JOY_ANIMATION_FADE_H
