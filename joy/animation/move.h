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

/**
 * \file
 * \brief Move animation
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
