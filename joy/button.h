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
 * \brief A push-button
 */

#ifndef JOY_BUTTON_H
#define JOY_BUTTON_H

#include <joy/container.h>

G_BEGIN_DECLS

#define JOY_TYPE_BUTTON \
	(joy_button_get_type())

#define JOY_BUTTON(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_BUTTON, JoyButton))

#define JOY_IS_BUTTON(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_BUTTON))

#define JOY_BUTTON_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_BUTTON, JoyButtonClass))

#define JOY_IS_BUTTON_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_BUTTON))

#define JOY_BUTTON_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_BUTTON, \
		JoyButtonClass))

typedef struct JoyButtonClass_ JoyButtonClass;

struct JoyButton_ {
	/*< private >*/
	JoyContainer parent_instance;
	gpointer priv;
};

typedef void
(*JoyButtonClicked)(JoyButton *self);

struct JoyButtonClass_ {
	/*< private >*/
	JoyContainerClass parent_class;
	/*< public >*/
	JoyButtonClicked clicked;
};

G_GNUC_NO_INSTRUMENT
GType
joy_button_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new button.
 *
 * \param text [in] The button text (may be NULL).
 *
 * \return A new button object.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyBubble *
joy_button_new(const gchar *text);

/**
 * \brief Set the text for a button.
 *
 * \param self [in] A button object.
 * \param text [in] The new text for \e self.
 */
void
joy_button_set_text(JoyBubble *self, const gchar *text);

/**
 * \brief Get the text for a button.
 *
 * \param self [in] A button object.
 *
 * \return The text for \e self or NULL if no text is set.
 */
const gchar *
joy_button_get_text(const JoyBubble *self);

G_END_DECLS

#endif // JOY_BUTTON_H
