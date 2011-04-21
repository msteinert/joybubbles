/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Theme engine interface
 * \author Mike Steinert <michael.steinert@echostar.com>
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
 * \brief Get a style object for a given widget.
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
 * Sub-classes must implement this function.
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
