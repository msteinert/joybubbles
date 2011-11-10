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
#ifdef JOY_HAVE_DBUS
#include "joy/source/dbus.h"
#endif // JOY_HAVE_DBUS
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define NAME "Joybubbles Example"

static gboolean
on_draw(JoyBubble *image, cairo_t *cr, gpointer data)
{
	gint width = joy_bubble_get_width(image) - 2;
	gint height = joy_bubble_get_height(image) - 2;
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
		gint x, gint y, JoyMouseButton button,
		struct ButtonDown *down)
{
	if (JOY_MOUSE_BUTTON_RIGHT == button) {
		joy_animation_pause(down->move);
		joy_animation_move_set_x(down->move, x);
		joy_animation_move_set_y(down->move, y);
		joy_animation_start(down->move);
	}
}

static void
on_clicked(JoyBubble *button, struct ButtonDown *down)
{
	joy_animation_pause(down->fade);
	gdouble alpha;
	if (down->in) {
		joy_animation_set_easing(down->fade, joy_easing_out_cubic,
				NULL);
		down->in = FALSE;
		alpha = 0.;
	} else {
		joy_animation_set_easing(down->fade, joy_easing_in_cubic,
				NULL);
		down->in = TRUE;
		alpha = 1.;
	}
	joy_animation_fade_set_alpha(down->fade, alpha);
	joy_animation_start(down->fade);
}

static void
on_button_up(JoyBubble *image, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyMouseButton button, gpointer data)
{
	if (JOY_MOUSE_BUTTON_LEFT == button) {
		JoyApplication *app = joy_bubble_get_application(image);
		if (app) {
			joy_application_quit(app, EXIT_SUCCESS);
		}
	}
}

struct Crossing {
	JoyAnimation *resize;
	struct JoyEasingBack back;
	gint dx, dy;
	gint w1, h1;
	gint w2, h2;
};

static void
on_enter(JoyBubble *window, JoyDevice *device, gulong timestamp,
		gint x, gint y, struct Crossing *crossing)
{
	joy_animation_pause(crossing->resize);
	JoyBubble *widget = joy_animation_get_widget(crossing->resize);
	gint width = joy_bubble_get_width(widget);
	gint height = joy_bubble_get_height(widget);
	if (G_UNLIKELY(width == crossing->w2 && height == crossing->h2)) {
		return;
	}
	if (width == crossing->w1 && height == crossing->h1) {
		joy_animation_set_easing(crossing->resize,
				joy_easing_in_back, &crossing->back);
	} else {
		joy_animation_set_easing(crossing->resize, NULL, NULL);
	}
	joy_animation_resize_set_width(crossing->resize, crossing->w2);
	joy_animation_resize_set_height(crossing->resize, crossing->h2);
	joy_animation_start(crossing->resize);
}

static void
on_leave(JoyBubble *window, JoyDevice *device, gulong timestamp,
		gint x, gint y, struct Crossing *crossing)
{
	joy_animation_pause(crossing->resize);
	JoyBubble *widget = joy_animation_get_widget(crossing->resize);
	gint width = joy_bubble_get_width(widget);
	gint height = joy_bubble_get_height(widget);
	if (G_UNLIKELY(width == crossing->w1 && height == crossing->h1)) {
		return;
	}
	if (width == crossing->w2 && height == crossing->h2) {
		joy_animation_set_easing(crossing->resize,
				joy_easing_out_bounce, NULL);
	} else {
		joy_animation_set_easing(crossing->resize, NULL, NULL);
	}
	joy_animation_resize_set_width(crossing->resize, crossing->w1);
	joy_animation_resize_set_height(crossing->resize, crossing->h1);
	joy_animation_start(crossing->resize);
}

static void
on_scroll(JoyBubble *window, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyMouseScroll direction,
		struct Crossing *crossing)
{
	gint width, height;
	JoyBubble *widget = joy_animation_get_widget(crossing->resize);
	switch (direction) {
	case JOY_MOUSE_SCROLL_UP:
		width = joy_bubble_get_width(widget);
		height = joy_bubble_get_height(widget);
		joy_bubble_resize(widget, width + crossing->dx,
				height + crossing->dy);
		crossing->w1 = joy_bubble_get_width(widget);
		crossing->h1 = joy_bubble_get_height(widget);;
		crossing->w2 = crossing->w1 * 1.25;
		crossing->h2 = crossing->h1 * 1.25;
		break;
	case JOY_MOUSE_SCROLL_DOWN:
		width = joy_bubble_get_width(widget);
		height = joy_bubble_get_height(widget);
		if (width - crossing->dx < 0 || height - crossing->dy < 0) {
			break;
		}
		joy_bubble_resize(widget, width - crossing->dx,
				height - crossing->dy);
		crossing->w1 = joy_bubble_get_width(widget);
		crossing->h1 = joy_bubble_get_height(widget);;
		crossing->w2 = crossing->w1 * 1.25;
		crossing->h2 = crossing->h1 * 1.25;
		break;
	default:
		return;
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
#ifdef JOY_HAVE_DBUS
	JoyDBus *dbus = NULL;
#endif // JOY_HAVE_DBUS
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
		goto error;
	}
	g_option_context_free(context);
	context = NULL;
#ifdef JOY_HAVE_DBUS
	const gchar *address = g_getenv("DBUS_SESSION_BUS_ADDRESS");
	if (address) {
		dbus = joy_dbus_new(app, address, &error);
		if (!dbus) {
			g_message("%s", error->message);
			g_error_free(error);
			error = NULL;
		}
	}
#endif // JOY_HAVE_DBUS
	JoyScreen *screen = joy_application_get_default_screen(app);
	if (!screen) {
		goto error;
	}
	JoyBubble *window = joy_window_new(screen);
	if (!window) {
		goto error;
	}
#ifdef JOY_PLATFORM_X11
	joy_bubble_resize(window, 1280, 720);
#endif // JOY_PLATFORM_X11
	g_signal_connect(window, "key-down", G_CALLBACK(on_key_down), NULL);
	g_signal_connect(window, "key-up", G_CALLBACK(on_key_up), NULL);
	JoyBubble *image;
	if (1 < argc) {
		image = joy_image_new(argv[1]);
		if (!image) {
			goto error;
		}
		joy_container_add(window, image);
		gint width = (gdouble)joy_image_get_width(image) / 2.;
		gint height = (gdouble)joy_image_get_height(image) / 2.;
		joy_bubble_resize(image, width, height);
	} else {
		image = joy_image_new(NULL);
		if (!image) {
			goto error;
		}
		joy_container_add(window, image);
		joy_bubble_resize(image, 200, 100);
		g_signal_connect(image, "draw", G_CALLBACK(on_draw), NULL);
	}
	joy_bubble_move(image, joy_bubble_get_width(window) / 2
			- joy_bubble_get_width(image) / 2,
			joy_bubble_get_height(window) / 2
			- joy_bubble_get_height(image) / 2);
	crossing.resize = joy_animation_resize_new(image, 0, 0);
	if (!crossing.resize) {
		goto error;
	}
	joy_animation_set_duration(crossing.resize, .5);
	crossing.w1 = joy_bubble_get_width(image);
	crossing.h1 = joy_bubble_get_height(image);
	crossing.w2 = (gint)(1.25 * crossing.w1);
	crossing.h2 = (gint)(1.25 * crossing.h1);
	crossing.dx = crossing.w1 * .1;
	crossing.dy = crossing.h1 * .1;
	g_signal_connect(image, "enter", G_CALLBACK(on_enter),
			&crossing.resize);
	g_signal_connect(image, "leave", G_CALLBACK(on_leave),
			&crossing.resize);
	g_signal_connect(image, "button-up", G_CALLBACK(on_button_up), NULL);
	g_signal_connect(window, "scroll", G_CALLBACK(on_scroll), &crossing);
	down.move = joy_animation_move_new(image, 0, 0);
	if (!down.move) {
		goto error;
	}
	joy_animation_set_duration(down.move, .5);
	joy_animation_set_easing(down.move, joy_easing_out_elastic, NULL);
	down.fade = joy_animation_fade_new(image, 0.);
	if (!down.move) {
		goto error;
	}
	joy_animation_set_duration(down.fade, 1.);
	g_signal_connect(window, "button-down", G_CALLBACK(on_button_down),
			&down);
	JoyBubble *label = joy_label_new(Q_("Hello, World!"));
	joy_container_add(window, label);
	joy_bubble_move(label, 100, 100);
	JoyBubble *button = joy_button_new(Q_("Click Me"));
	joy_container_add(window, button);
	joy_bubble_resize(button, 150, 60);
	joy_bubble_move(button, 100, 400);
	g_signal_connect(button, "clicked", G_CALLBACK(on_clicked), &down);
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
#ifdef JOY_HAVE_DBUS
	if (dbus) {
		g_object_unref(dbus);
	}
#endif // JOY_HAVE_DBUS
	return status;
error:
	if (error) {
		g_message("%s", error->message);
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

