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
 * \brief Theme engine interface
 */

#ifndef JOY_THEME_H
#define JOY_THEME_H

#include <cairo.h>
#include <joy/style.h>
#include <joy/types.h>
#include <pango/pango.h>

G_BEGIN_DECLS

#define JOY_TYPE_THEME \
	(joy_theme_get_type())

#define JOY_THEME(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_THEME, JoyTheme))

#define JOY_IS_THEME(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_THEME))

#define JOY_THEME_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_THEME, JoyThemeClass))

#define JOY_IS_THEME_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_THEME))

#define JOY_THEME_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_THEME, \
		JoyThemeClass))

typedef struct JoyThemeClass_ JoyThemeClass;

struct JoyTheme_ {
	/*< private >*/
	JoyStyle parent_instance;
	gpointer priv;
};

typedef JoyStyle *
(*JoyThemeStyleCreate)(JoyStyle *self, JoyBubble *widget);

struct JoyThemeClass_ {
	/*< private >*/
	JoyStyleClass parent_class;
	/*< public >*/
	JoyThemeStyleCreate style_create;
};

G_GNUC_NO_INSTRUMENT
GType
joy_theme_get_type(void) G_GNUC_CONST;

/**
 * \brief Specify a style for a widget type or name.
 *
 * The specified style will be returned for calls to joy_theme_get_style()
 * for widgets with a name or type that matches \e name.
 *
 * \param self [in] A theme object.
 * \param name [in] A widget type or name.
 * \param style [in] The style to specify for \e name.
 */
void
joy_theme_set_style(JoyStyle *self, const gchar *name, JoyStyle *style);

/**
 * \brief Get the specified style for a widget.
 *
 * If no style is specified for \e widget and \e self has a parent style
 * then the parent theme will be queried. If the parent theme has a style
 * specified for \e widget then this theme will cache the specified style
 * for performance reasons.
 *
 * \param self [in] A theme object.
 * \param widget [in] A widget to get the style for.
 *
 * \return A style object for drawing \e widget, or NULL.
 */
JoyStyle *
joy_theme_get_style(JoyStyle *self, JoyBubble *widget);

/**
 * \brief Create a new style object for the specified widget.
 *
 * Sub-classes must implement this function. This function should return
 * the style for \e widget or NULL if one is not implemented.
 *
 * \param self [in] A theme object.
 * \param widget [in] A widget object.
 *
 * \return A new style object for drawing \e widget, or NULL.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyStyle *
joy_theme_style_create(JoyStyle *self, JoyBubble *widget);

G_END_DECLS

#endif // JOY_THEME_H
