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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/button.h"
#include "joy/label.h"
#include "joy/theme/default/button.h"

G_DEFINE_TYPE(JoyStyleDefaultButton, joy_style_default_button, JOY_TYPE_STYLE)

static void
joy_style_default_button_init(G_GNUC_UNUSED JoyStyleDefaultButton *self)
{
}

static gboolean
set_font_source(G_GNUC_UNUSED JoyStyle *self, cairo_t *cr)
{
	cairo_set_source_rgb(cr, 0., 0., 0.);
	return TRUE;
}

static void
draw_background(G_GNUC_UNUSED JoyStyle *self, JoyBubble *widget, cairo_t *cr)
{
	const gdouble width = (gdouble)joy_bubble_get_width(widget);
	const gdouble height = (gdouble)joy_bubble_get_height(widget);
	static const gdouble degrees = G_PI / 180.;
	const gdouble radius = 6.;
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
	JoyBubbleState state = joy_bubble_get_state(widget);
	switch (state) {
	case JOY_BUBBLE_STATE_FOCUSED:
		red *= 1.2;
		green *= 1.2;
		blue *= 1.2;
		break;
	default:
		break;
	}
	static const gdouble factor = 4.;
	cairo_pattern_add_color_stop_rgb(pattern, 0., red, green, blue);
	cairo_pattern_add_color_stop_rgb(pattern, 1., red / factor,
			green / factor, blue / factor);
	cairo_set_source(cr, pattern);
	cairo_pattern_destroy(pattern);
	cairo_fill_preserve(cr);
	cairo_set_source_rgb(cr, red / factor, green / factor, blue / factor);
	cairo_stroke(cr);
	switch (state) {
	case JOY_BUBBLE_STATE_FOCUSED:
	case JOY_BUBBLE_STATE_ACTIVE:
		cairo_rectangle(cr, 4., 4., width - 8., height - 8.);
		cairo_set_source_rgb(cr, 0., 0., 0.);
		const gdouble dashes[] = { 1., 2. };
		cairo_set_dash(cr, dashes, 2, 0.);
		cairo_stroke(cr);
		break;
	default:
		break;
	}
}

static void
joy_style_default_button_class_init(JoyStyleDefaultButtonClass *klass)
{
	JoyStyleClass *style_class = JOY_STYLE_CLASS(klass);
	style_class->set_font_source = set_font_source;
	style_class->draw_background = draw_background;
}

JoyStyle *
joy_style_default_button_new(G_GNUC_UNUSED JoyBubble *button)
{
	return g_object_new(JOY_TYPE_STYLE_DEFAULT_BUTTON,
			NULL);
}
