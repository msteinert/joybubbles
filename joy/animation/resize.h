/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Resize animation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_ANIMATION_RESIZE_H
#define JOY_ANIMATION_RESIZE_H

#include <joy/animation/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_ANIMATION_RESIZE \
	(joy_animation_resize_get_type())

#define JOY_ANIMATION_RESIZE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_ANIMATION_RESIZE, \
		JoyAnimationResize))

#define JOY_IS_ANIMATION_RESIZE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_ANIMATION_RESIZE))

#define JOY_ANIMATION_RESIZE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_ANIMATION_RESIZE, \
		JoyAnimationResizeClass))

#define JOY_IS_ANIMATION_RESIZE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_ANIMATION_RESIZE))

#define JOY_ANIMATION_RESIZE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_ANIMATION_RESIZE, \
		JoyAnimationResizeClass))

typedef struct JoyAnimationResizeClass_ JoyAnimationResizeClass;

struct JoyAnimationResize_ {
	/*< private >*/
	JoyAnimation parent_instance;
	gpointer priv;
};

struct JoyAnimationResizeClass_ {
	/*< private >*/
	JoyAnimationClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_animation_resize_get_type(void) G_GNUC_CONST;

JoyAnimation *
joy_animation_resize_new(JoyBubble *widget, gint width, gint height);

void
joy_animation_resize_set_width(JoyAnimation *self, gint width);

void
joy_animation_resize_set_height(JoyAnimation *self, gint height);

G_END_DECLS

#endif // JOY_ANIMATION_RESIZE_H
