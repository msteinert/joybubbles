/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Move animation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_ANIMATION_MOVE_H
#define JOY_ANIMATION_MOVE_H

#include <joy/animation/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_ANIMATION_MOVE \
	(joy_animation_move_get_type())

#define JOY_ANIMATION_MOVE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_ANIMATION_MOVE, \
		JoyAnimationMove))

#define JOY_IS_ANIMATION_MOVE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_ANIMATION_MOVE))

#define JOY_ANIMATION_MOVE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_ANIMATION_MOVE, \
		JoyAnimationMoveClass))

#define JOY_IS_ANIMATION_MOVE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_ANIMATION_MOVE))

#define JOY_ANIMATION_MOVE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_ANIMATION_MOVE, \
		JoyAnimationMoveClass))

typedef struct JoyAnimationMoveClass_ JoyAnimationMoveClass;

struct JoyAnimationMove_ {
	/*< private >*/
	JoyAnimation parent_instance;
	gpointer priv;
};

struct JoyAnimationMoveClass_ {
	/*< private >*/
	JoyAnimationClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_animation_move_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new move animation.
 *
 * \param widget [in] The widget to move.
 * \param x [in] The new X-coordinate for \e widget.
 * \param y [in] The new Y-coordinate for \e widget.
 *
 * \return A new move animation object.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyAnimation *
joy_animation_move_new(JoyBubble *widget, gint x, gint y);

/**
 * \brief Set the X-coordinate.
 *
 * \param self [in] A fade animation object.
 * \param x [in] The new X-coordinate for \e self.
 */
void
joy_animation_move_set_x(JoyAnimation *self, gint x);

/**
 * \brief Set the Y-coordinate.
 *
 * \param self [in] A fade animation object.
 * \param y [in] The new Y-coordinate for \e self.
 */
void
joy_animation_move_set_y(JoyAnimation *self, gint y);

G_END_DECLS

#endif // JOY_ANIMATION_MOVE_H
