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

static gboolean
on_draw(JoyBubble *sketch, cairo_t *cr, gpointer data)
{
	gint width = joy_bubble_get_width(sketch) - 2;
	gint height = joy_bubble_get_height(sketch) - 2;
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
	return TRUE;
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
	case JOY_KEY_Up:
		dy = -5;
		break;
	case JOY_KEY_a:
	case JOY_KEY_h:
	case JOY_KEY_Left:
		dx = -5;
		break;
	case JOY_KEY_s:
	case JOY_KEY_j:
	case JOY_KEY_Down:
		dy = 5;
		break;
	case JOY_KEY_d:
	case JOY_KEY_l:
	case JOY_KEY_Right:
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
	case JOY_KEY_Cancel:
		{
			JoyApplication *app =
				joy_bubble_get_application(window);
			if (app) {
				joy_application_quit(app, EXIT_SUCCESS);
			}
		}
		break;
	default:
		break;
	}
}

struct ButtonDown {
	JoyAnimation *move;
	JoyAnimation *fade;
	gboolean in;
};

static void
on_button_down(JoyBubble *window, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyButton button, gpointer data)
{
	struct ButtonDown *down = (struct ButtonDown *)data;
	if (JOY_BUTTON_LEFT == button) {
		JoyAnimation *fade = down->fade;
		joy_animation_pause(fade);
		gdouble alpha;
		if (down->in) {
			joy_animation_set_easing(fade, joy_easing_out_cubic,
					NULL);
			down->in = FALSE;
			alpha = 0.;
		} else {
			joy_animation_set_easing(fade, joy_easing_in_cubic,
					NULL);
			down->in = TRUE;
			alpha = 1.;
		}
		joy_animation_fade_set_alpha(fade, alpha);
		joy_animation_start(fade);
	} else if (JOY_BUTTON_RIGHT == button) {
		JoyAnimation *move = down->move;
		joy_animation_pause(move);
		joy_animation_move_set_x(move, x);
		joy_animation_move_set_y(move, y);
		joy_animation_start(move);
	}
}

static void
on_button_up(JoyBubble *sketch, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyButton button, gpointer data)
{
	if (JOY_BUTTON_LEFT == button) {
		JoyApplication *app = joy_bubble_get_application(sketch);
		if (app) {
			joy_application_quit(app, EXIT_SUCCESS);
		}
	}
}

struct Crossing {
	JoyAnimation *resize;
	struct JoyEasingBack back;
	gint width;
	gint height;
};

static void
on_enter(JoyBubble *window, JoyDevice *device, gulong timestamp,
		gint x, gint y, gpointer data)
{
	struct Crossing *crossing = (struct Crossing *)data;
	joy_animation_pause(crossing->resize);
	JoyBubble *widget = joy_animation_get_widget(crossing->resize);
	gint width = joy_bubble_get_width(widget);
	gint height = joy_bubble_get_height(widget);
	if (crossing->width == width && crossing->height == height) {
		joy_animation_set_easing(crossing->resize, joy_easing_in_back,
				&crossing->back);
	} else {
		crossing->width = width;
		crossing->height = height;
	}
	joy_animation_resize_set_width(crossing->resize, 220);
	joy_animation_resize_set_height(crossing->resize, 110);
	joy_animation_start(crossing->resize);
}

static void
on_leave(JoyBubble *window, JoyDevice *device, gulong timestamp,
		gint x, gint y, gpointer data)
{
	struct Crossing *crossing = (struct Crossing *)data;
	joy_animation_pause(crossing->resize);
	JoyBubble *widget = joy_animation_get_widget(crossing->resize);
	gint width = joy_bubble_get_width(widget);
	gint height = joy_bubble_get_height(widget);
	if (G_UNLIKELY(width == crossing->width
				&& height == crossing->height)) {
		return;
	}
	if (220 == width && 110 == height) {
		joy_animation_set_easing(crossing->resize,
				joy_easing_out_bounce, NULL);
	} else {
		joy_animation_set_easing(crossing->resize, NULL, NULL);
	}
	joy_animation_resize_set_width(crossing->resize, crossing->width);
	joy_animation_resize_set_height(crossing->resize, crossing->height);
	joy_animation_start(crossing->resize);
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
	struct ButtonDown down = { NULL, NULL, TRUE };
	struct Crossing crossing = { NULL, { 5. }, 200, 100 };
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
	//joy_bubble_resize(window, 400, 200);
	g_signal_connect(window, "key-down", G_CALLBACK(on_key_down), NULL);
	g_signal_connect(window, "key-up", G_CALLBACK(on_key_up), NULL);
	g_signal_connect(window, "scroll", G_CALLBACK(on_scroll), NULL);
	JoyBubble *sketch = joy_sketch_new();
	if (!sketch) {
		goto error;
	}
	joy_container_add(window, sketch);
	joy_bubble_set_expand(sketch, FALSE);
	joy_bubble_move(sketch, joy_bubble_get_width(window) / 2 - 100,
			joy_bubble_get_height(window) / 2 - 50);
	joy_bubble_resize(sketch, 200, 100);
	g_signal_connect(sketch, "draw", G_CALLBACK(on_draw), NULL);
	crossing.resize = joy_animation_resize_new(sketch, 220, 110);
	if (!crossing.resize) {
		goto error;
	}
	joy_animation_set_duration(crossing.resize, .5);
	g_signal_connect(sketch, "enter",
			G_CALLBACK(on_enter), &crossing.resize);
	g_signal_connect(sketch, "leave",
			G_CALLBACK(on_leave), &crossing.resize);
	g_signal_connect(sketch, "button-up", G_CALLBACK(on_button_up), NULL);
	down.move = joy_animation_move_new(sketch, 0, 0);
	if (!down.move) {
		goto error;
	}
	joy_animation_set_duration(down.move, .5);
	joy_animation_set_easing(down.move, joy_easing_out_elastic, NULL);
	down.fade = joy_animation_fade_new(sketch, 0.);
	if (!down.move) {
		goto error;
	}
	joy_animation_set_duration(down.fade, 1.);
	g_signal_connect(window, "button-down",
			G_CALLBACK(on_button_down), &down);
	joy_bubble_show(window);
	gint status = joy_application_run(app, screen);
exit:
	if (crossing.resize) {
		g_object_unref(crossing.resize);
	}
	if (down.move) {
		g_object_unref(down.move);
	}
	if (down.fade) {
		g_object_unref(down.fade);
	}
	return status;
error:
	if (error) {
		g_error_free(error);
	}
	if (context) {
		g_option_context_free(context);
	}
	if (app) {
		g_object_unref(app);
	}
	status = EXIT_FAILURE;
	goto exit;
}
