/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <ctype.h>
#include <glib.h>
#include "joy/bubbles.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define NAME "Joybubbles Example"

static void
on_draw(JoyBubble *window, cairo_t *cr, gpointer data)
{
	gint width = joy_bubble_get_width(window) - 2;
	gint height = joy_bubble_get_height(window) - 2;
	double aspect = 0.75;
	double corner_radius = height / 30.;
	double radius = corner_radius / aspect;
	double degrees = M_PI / 180.;
	cairo_translate(cr, 1., 1.);
	/* background */
	cairo_arc(cr, width - radius, radius, radius, -90 * degrees, 0);
	cairo_arc(cr, width - radius, height - radius, radius, 0,
			90 * degrees);
	cairo_arc(cr, radius, height - radius, radius, 90 * degrees,
			180 * degrees);
	cairo_arc(cr, radius, radius, radius, 180 * degrees, 270 * degrees);
	cairo_close_path(cr);
	cairo_pattern_t *pat = cairo_pattern_create_linear(0, 0, 0, height);
	cairo_pattern_add_color_stop_rgba(pat, 0., 1., 1., 1., 0.85);
	cairo_pattern_add_color_stop_rgba(pat, 0.25, 0.39, 0.58, 0.93, 0.85);
	cairo_set_source(cr, pat);
	cairo_pattern_destroy(pat);
	cairo_fill_preserve(cr);
	/* outline */
	cairo_set_source_rgba(cr, 1., 1., 1., 1.);
	cairo_set_line_width(cr, 2.);
	cairo_stroke(cr);
}

static void
on_key_down(JoyBubble *window, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyKeySym sym, gpointer data)
{
	gint dx = 0;
	gint dy = 0;
	switch (sym) {
	case JOY_KEY_w:
	case JOY_KEY_k:
		dy = -5;
		break;
	case JOY_KEY_a:
	case JOY_KEY_h:
		dx = -5;
		break;
	case JOY_KEY_s:
	case JOY_KEY_j:
		dy = 5;
		break;
	case JOY_KEY_d:
	case JOY_KEY_l:
		dx = 5;
		break;
	default:
		break;
	}
	if (dx || dy) {
		for (JoyIterator *iter = joy_container_begin(window);
				iter; iter = joy_iterator_next(iter)) {
			JoyBubble *widget = joy_iterator_item(iter);
			joy_bubble_translate(widget, dx, dy);
		}
	}
}

static void
on_key_up(JoyBubble *window, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyKeySym sym, gpointer data)
{
	switch (sym) {
	case JOY_KEY_q:
	case JOY_KEY_Q:
		{
			JoyApplication *app =
				joy_bubble_get_application(window);
			if (G_UNLIKELY(!app)) {
				break;
			}
			joy_application_quit(app, EXIT_SUCCESS);
		}
		break;
	default:
		break;
	}
}

static void
on_button_up(JoyBubble *window, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyButton button, gpointer data)
{
}

static void
on_enter(JoyBubble *window, JoyDevice *device, gulong timestamp,
		gint x, gint y, gpointer data)
{
	JoyAnimation *grow = (JoyAnimation *)data;
	joy_animation_start(grow);
}

static void
on_leave(JoyBubble *window, JoyDevice *device, gulong timestamp,
		gint x, gint y, gpointer data)
{
	JoyAnimation *shrink = (JoyAnimation *)data;
	joy_animation_start(shrink);
}

static void
on_scroll(JoyBubble *window, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyScroll direction, gpointer data)
{
	switch (direction) {
	case JOY_SCROLL_UP:
		for (JoyIterator *iter = joy_container_begin(window);
				iter; iter = joy_iterator_next(iter)) {
			JoyBubble *widget = joy_iterator_item(iter);
			gint width = joy_bubble_get_width(widget);
			gint height = joy_bubble_get_height(widget);
			joy_bubble_resize(widget, width + 10, height + 5);
		}
		break;
	case JOY_SCROLL_DOWN:
		for (JoyIterator *iter = joy_container_begin(window);
				iter; iter = joy_iterator_next(iter)) {
			JoyBubble *widget = joy_iterator_item(iter);
			gint width = joy_bubble_get_width(widget);
			gint height = joy_bubble_get_height(widget);
			joy_bubble_resize(widget, width - 10, height - 5);
		}
		break;
	default:
		break;
	}
}

int
main(int argc, char *argv[])
{
	GError *error = NULL;
	GOptionContext *context = NULL;
	JoyAnimation *grow = NULL;
	JoyAnimation *shrink = NULL;
	JoyApplication *app = joy_application_new();
	if (!app) {
		goto error;
	}
	joy_application_set_name(app, NAME);
	context = g_option_context_new("- " NAME);
	if (!context) {
		goto error;
	}
	GOptionEntry entries[] = {
		{ NULL }
	};
	g_option_context_add_main_entries(context, entries, GETTEXT_PACKAGE);
	joy_application_add_options(app, context);
	if (!g_option_context_parse(context, &argc, &argv, &error)) {
		g_message("%s", error->message);
		goto error;
	}
	g_option_context_free(context);
	context = NULL;
	JoyScreen *screen = joy_application_get_screen(app, 0);
	if (!screen) {
		goto error;
	}
	JoyBubble *window = joy_window_new(screen);
	if (!window) {
		goto error;
	}
	joy_bubble_resize(window, 400, 200);
	g_signal_connect(window, "key-down", G_CALLBACK(on_key_down), NULL);
	g_signal_connect(window, "key-up", G_CALLBACK(on_key_up), NULL);
	g_signal_connect(window, "button-up", G_CALLBACK(on_button_up), NULL);
	g_signal_connect(window, "scroll", G_CALLBACK(on_scroll), NULL);
	JoyBubble *sketch = joy_sketch_new();
	if (!sketch) {
		goto error;
	}
	joy_container_add(window, sketch);
	joy_bubble_set_buffered(sketch, TRUE);
	joy_bubble_set_expand(sketch, FALSE);
	joy_bubble_move(sketch, 100, 50);
	joy_bubble_resize(sketch, 200, 100);
	g_signal_connect(sketch, "draw", G_CALLBACK(on_draw), NULL);
	grow = joy_animation_grow_new(sketch, 220, 110);
	if (!grow) {
		goto error;
	}
	g_signal_connect(sketch, "enter", G_CALLBACK(on_enter), grow);
	shrink = joy_animation_grow_new(sketch, 200, 100);
	if (!shrink) {
		goto error;
	}
	g_signal_connect(sketch, "leave", G_CALLBACK(on_leave), shrink);
	joy_bubble_show(window);
	return joy_application_run(app, screen);
error:
	if (error) {
		g_error_free(error);
	}
	if (context) {
		g_option_context_free(context);
	}
	if (grow) {
		g_object_unref(grow);
	}
	if (shrink) {
		g_object_unref(shrink);
	}
	if (app) {
		g_object_unref(app);
	}
	return EXIT_FAILURE;
}
