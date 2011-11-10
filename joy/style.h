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

typedef PangoLayout *
(*JoyStylePangoLayoutCreate)(JoyStyle *self);

typedef gboolean
(*JoyStyleSetFontSource)(JoyStyle *self, cairo_t *cr);

typedef void
(*JoyStyleDraw)(JoyStyle *self, JoyBubble *widget, cairo_t *cr);

struct JoyStyleClass_ {
	/*< private >*/
	GObjectClass parent_class;
	/*< public >*/
	JoyStylePangoLayoutCreate pango_layout_create;
	JoyStyleSetFontSource set_font_source;
	JoyStyleDraw draw_background;
	JoyStyleDraw draw_foreground;
};

G_GNUC_NO_INSTRUMENT
GType
joy_style_get_type(void) G_GNUC_CONST;

/**
 * \brief Set the parent, i.e., the them, of this style.
 *
 * \param self [in] A style object.
 * \param parent [in] The theme to set for \e self.
 */
void
joy_style_set_parent(JoyStyle *self, JoyStyle *parent);

/**
 * \brief Get the parent, i.e., the theme, of this style.
 *
 * \param self [in] A style object.
 *
 * \return The theme of \e self or NULL.
 */
JoyStyle *
joy_style_get_parent(JoyStyle *self);

/**
 * \brief Set the font description for the specified style.
 *
 * \param self [in] A style object.
 * \param desc [in] The new font
 */
void
joy_style_set_font_description(JoyStyle *self,
		const PangoFontDescription *desc);

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

/**
 * \brief Set the Cairo source this style will use to paint fonts.
 *
 * \param self [in] A style object.
 * \param font [in] A Cairo pattern to use a source for fonts.
 */
void
joy_style_set_font_source(JoyStyle *self, cairo_pattern_t *source);

/**
 * \brief Set an RGB Cairo source this style will use to paint fonts.
 *
 * \param self [in] A style object.
 * \param red [in] The red component.
 * \param green [in] The green component.
 * \param blue [in] The blue component.
 */
void
joy_style_set_font_source_rgb(JoyStyle *self, double red, double green,
		double blue);

/**
 * \brief Set an RGBA Cairo source this style will use to paint fonts.
 *
 * \param self [in] A style object.
 * \param red [in] The red component.
 * \param green [in] The green component.
 * \param blue [in] The blue component.
 * \param alpha [in] The alpha component.
 */
void
joy_style_set_font_source_rgba(JoyStyle *self, double red, double green,
		double blue, double alpha);

/**
 * \brief Set a Cairo surface as the source this style will use to paint fonts.
 *
 * \param self [in] A style object.
 * \param surface [in] The surface to use as a source for fonts.
 */
void
joy_style_set_font_source_surface(JoyStyle *self, cairo_surface_t *surface);

/**
 * \brief Get the Cairo source a style is using to paint fonts.
 *
 * \param self [in] A style object.
 *
 * \return The current Cairo source \e self is using to paint fonts.
 */
cairo_pattern_t *
joy_style_get_font_source(JoyStyle *self);

/**
 * \brief Create a Pango layout using style defaults.
 *
 * If font defaults were not set for this style then the theme defaults
 * will be used.
 *
 * \param self [in] A style object.
 *
 * \return A new Pango layout object.
 */
G_GNUC_WARN_UNUSED_RESULT
PangoLayout *
joy_style_pango_layout_create(JoyStyle *self);

/**
 * \brief Set the default Cairo source for this style.
 *
 * This function should be called before painting fonts. If no source is set
 * for this style then the theme defaults will be used.
 *
 * \param self [in] A theme object.
 * \param cr [in] A Cairo handle.
 *
 * \return TRUE if the source was set for \e cr, FALSE otherwise.
 */
gboolean
joy_style_cairo_set_font_source(JoyStyle *self, cairo_t *cr);

void
joy_style_draw_background(JoyStyle *self, JoyBubble *widget, cairo_t *cr);

void
joy_style_draw_foreground(JoyStyle *self, JoyBubble *widget, cairo_t *cr);

G_END_DECLS

#endif // JOY_STYLE_H

