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
#include <glib-object.h>
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
	GObject parent_instance;
	gpointer priv;
};

typedef JoyStyle *
(*JoyThemeGetStyle)(JoyTheme *self, JoyBubble *widget);

struct JoyThemeClass_ {
	/*< private >*/
	GObjectClass parent_class;
	/*< public >*/
	JoyThemeGetStyle get_style;
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
 * \return A style object for \e widget or NULL if none is available.
 */
JoyStyle *
joy_theme_get_style(JoyTheme *self, JoyBubble *widget);

void
joy_theme_set_font_description(JoyTheme *self,
		const PangoFontDescription *desc);

G_GNUC_WARN_UNUSED_RESULT
PangoLayout *
joy_theme_pango_layout_create(JoyTheme *self);

gboolean
joy_theme_cairo_set_source_font(JoyTheme *self, cairo_t *cr);

G_END_DECLS

#endif // JOY_THEME_H
