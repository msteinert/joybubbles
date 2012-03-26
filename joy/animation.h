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
 * \brief Animation framework
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

typedef gdouble
(*JoyAnimationEasing)(gpointer self, gdouble time);

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
(*JoyAnimationAdvance)(JoyAnimation *self, JoyBubble *widget, gdouble percent);

struct JoyAnimationClass_ {
	/*< private >*/
	GObjectClass parent_class;
	/*< public >*/
	JoyAnimationStart start;
	JoyAnimationStop stop;
	JoyAnimationPause pause;
	JoyAnimationAdvance advance;
};

G_GNUC_NO_INSTRUMENT
GType
joy_animation_get_type(void) G_GNUC_CONST;

/**
 * \brief Get the widget this animation is operating on.
 *
 * \param self [in] An animation object.
 *
 * \param The widget \e self is operating on.
 */
JoyBubble *
joy_animation_get_widget(JoyAnimation *self);

/**
 * \brief Set the animation duration.
 *
 * The default value is 250 milliseconds (0.25 seconds).
 *
 * \param self [in] An animation object.
 * \param seconds [in] The new duration for \e self.
 */
void
joy_animation_set_duration(JoyAnimation *self, gdouble seconds);

/**
 * \brief Get the current animation duration.
 *
 * \param self [in] An animation object.
 *
 * \return The current duration for \e self.
 */
gdouble
joy_animation_get_duration(JoyAnimation *self);

/**
 * \brief Set the easing function.
 *
 * If no easing function is set then tweening will be linear.
 *
 * \param self [in] An animation object.
 * \param function [in] The new easing function for \e self.
 * \param object [in] Data passed as the first parameter to \e function.
 */
void
joy_animation_set_easing(JoyAnimation *self, JoyAnimationEasing function,
		gpointer object);

/**
 * \brief Set infinite looping.
 *
 * The animation will loop infinitely or until one of joy_animation_stop()
 * or joy_animation_pause() is called.
 *
 * \param self [in] An animation object.
 * \param looping [in] The new new looping setting.
 */
void
joy_animation_set_looping(JoyAnimation *self, gboolean looping);

/**
 * \brief Set the number of times the animation should loop.
 *
 * The default loop count is one.
 *
 * \param self [in] An animation object.
 * \param count [in] The number of times to loop.
 */
void
joy_animation_set_loop_count(JoyAnimation *self, gint count);

/**
 * \brief Start an animation.
 *
 * \param self [in] An animation object.
 */
void
joy_animation_start(JoyAnimation *self);

/**
 * \brief Stop an animation.
 *
 * \param self [in] An animation object.
 */
void
joy_animation_stop(JoyAnimation *self);

/**
 * \brief Pause an animation.
 *
 * \param self [in] An animation object.
 */
void
joy_animation_pause(JoyAnimation *self);

/**
 * \brief Advance the frame.
 *
 * This function is only useful in screen implementations.
 *
 * \param self [in] An animation object.
 * \param frame [in] Duration of the previous frame.
 */
void
joy_animation_advance(JoyAnimation *self, gulong frame);

G_END_DECLS

#endif // JOY_ANIMATION_H
