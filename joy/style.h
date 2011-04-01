/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Widget styles
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_STYLE_H
#define JOY_STYLE_H

#include <cairo.h>
#include <glib-object.h>
#include <joy/types.h>
#include <pango/pango.h>

G_BEGIN_DECLS

#define JOY_TYPE_STYLE \
	(joy_style_get_type())

#define JOY_STYLE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_STYLE, JoyStyle))

#define JOY_IS_STYLE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_STYLE))

#define JOY_STYLE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_STYLE, JoyStyleClass))

#define JOY_IS_STYLE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_STYLE))

#define JOY_STYLE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_STYLE, \
		JoyStyleClass))

typedef struct JoyStyleClass_ JoyStyleClass;

struct JoyStyle_ {
	/*< private >*/
	GObject parent_instance;
	gpointer priv;
};

typedef gboolean
(*JoyStyleDraw)(JoyStyle *self, JoyBubble *widget, cairo_t *cr);

struct JoyStyleClass_ {
	/*< private >*/
	GObjectClass parent_class;
	/*< public >*/
	JoyStyleDraw draw;
};

G_GNUC_NO_INSTRUMENT
GType
joy_style_get_type(void) G_GNUC_CONST;

/**
 * \brief Draw a widget using the specified style.
 *
 * \param self [in] A style object.
 * \param widget [in] The widget to draw.
 * \param cr [in] The cairo context to draw to.
 *
 * \return TRUE if \e widget was drawn, FALSE otherwise.
 */
gboolean
joy_style_draw(JoyStyle *self, JoyBubble *widget, cairo_t *cr);

JoyTheme *
joy_style_get_theme(JoyStyle *self);

/**
 * \brief Get the font description for the specified style.
 *
 * \param self [in] A style object.
 *
 * \return The font description for \e self. If no font description is
 *         associated with this style then the default font description for
 *         the theme will be returned.
 */
PangoFontDescription *
joy_style_get_font_description(JoyStyle *self);

void
joy_style_set_font_description(JoyStyle *self,
		const PangoFontDescription *desc);

cairo_pattern_t *
joy_style_get_font_pattern(JoyStyle *self);

void
joy_style_set_font_pattern(JoyStyle *self, cairo_pattern_t *font);

G_GNUC_WARN_UNUSED_RESULT
PangoLayout *
joy_style_pango_layout_create(JoyStyle *self);

gboolean
joy_style_cairo_set_source_font(JoyStyle *self, cairo_t *cr);

G_END_DECLS

#endif // JOY_STYLE_H
