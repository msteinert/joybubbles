/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/button.h"
#include "joy/label.h"
#include "joy/theme/default/button.h"

G_DEFINE_TYPE(JoyStyleDefaultButton, joy_style_default_button, JOY_TYPE_STYLE)

static void
joy_style_default_button_init(JoyStyleDefaultButton *self)
{
}

static gboolean
set_font_source(JoyStyle *self, cairo_t *cr)
{
	cairo_set_source_rgb(cr, 0., 0., 0.);
	return TRUE;
}

static void
draw_background(JoyStyle *self, JoyBubble *widget, cairo_t *cr)
{
	gdouble width = (gdouble)joy_bubble_get_width(widget);
	gdouble height = (gdouble)joy_bubble_get_height(widget);
	gdouble radius = (gdouble)MIN(width, height) * .15;
	const gdouble degrees = G_PI / 180.;
	cairo_set_line_width(cr, 1.);
	cairo_new_sub_path(cr);
	cairo_arc(cr, width - radius, radius, radius, -90 * degrees, 0);
	cairo_arc(cr, width - radius, height - radius, radius, 0,
			90 * degrees);
	cairo_arc(cr, radius, height - radius, radius, 90 * degrees,
			180 * degrees);
	cairo_arc(cr, radius, radius, radius, 180 * degrees, 270 * degrees);
	cairo_close_path(cr);
	cairo_pattern_t *pattern =
		cairo_pattern_create_linear(0., 0., 0., height);
	gdouble red = .83, green = .74, blue = .63;
	switch (joy_bubble_get_state(widget)) {
	case JOY_BUBBLE_STATE_FOCUSED:
		red *= 1.2;
		green *= 1.2;
		blue *= 1.2;
		break;
	default:
		break;
	}
	gdouble factor = 4.;
	cairo_pattern_add_color_stop_rgb(pattern, 0., red, green, blue);
	cairo_pattern_add_color_stop_rgb(pattern, 1., red / factor,
			green / factor, blue / factor);
	cairo_set_source(cr, pattern);
	cairo_pattern_destroy(pattern);
	cairo_fill_preserve(cr);
	cairo_set_source_rgb(cr, red / factor, green / factor, blue / factor);
	cairo_stroke(cr);
}

static void
joy_style_default_button_class_init(JoyStyleDefaultButtonClass *klass)
{
	JoyStyleClass *style_class = JOY_STYLE_CLASS(klass);
	style_class->set_font_source = set_font_source;
	style_class->draw_background = draw_background;
}

JoyStyle *
joy_style_default_button_new(JoyBubble *button)
{
	return g_object_new(JOY_TYPE_STYLE_DEFAULT_BUTTON,
			NULL);
}
