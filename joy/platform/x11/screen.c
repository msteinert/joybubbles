/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <cairo-xlib.h>
#include "joy/iterator/ptr-array.h"
#include "joy/macros.h"
#include "joy/platform/x11/application.h"
#include "joy/platform/x11/screen.h"
#include "joy/platform/x11/window.h"
#include "joy/timer.h"
#include "joy/timespec.h"
#ifdef HAVE_XCOMPOSITE
#include <X11/extensions/Xcomposite.h>
#endif
#include <X11/Xutil.h>

G_DEFINE_TYPE(JoyX11Screen, joy_x11_screen, JOY_TYPE_SCREEN)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_X11_SCREEN, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyX11Screen *)instance)->priv)

struct Private {
	Screen *screen;
	GPtrArray *windows;
	Visual *visual;
	Colormap colormap;
	gint depth;
	gboolean composite;
	JoyTimer *timer;
	struct timespec submit;
	gulong frame;
};

static void
destroy(gpointer object)
{
	g_object_run_dispose(G_OBJECT(object));
	g_object_unref(object);
}

static void
joy_x11_screen_init(JoyX11Screen *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->windows = g_ptr_array_sized_new(1);
	if (G_LIKELY(priv->windows)) {
		g_ptr_array_set_free_func(priv->windows, destroy);
	}
	priv->timer = joy_timer_new();
}

static void
constructed(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	JoyScreen *self = JOY_SCREEN(base);
	Display *display = joy_x11_screen_get_display(self);
	if (G_UNLIKELY(!display)) {
		goto exit;
	}
#ifdef HAVE_XCOMPOSITE
	gint ignore;
	if (XCompositeQueryExtension(display, &ignore, &ignore)) {
		priv->composite = TRUE;
	} else
#endif
		priv->composite = FALSE;
	// search for the best Visual
	XVisualInfo template;
	template.screen = joy_screen_get_id(self);
	gint nitems;
	XVisualInfo *info = XGetVisualInfo(display, VisualScreenMask,
			&template, &nitems);
	if (G_UNLIKELY(!info)) {
		goto exit;
	}
	for (gint i = 0; i < nitems; ++i) {
		if (info[i].class == TrueColor) {
			if (info[i].depth > priv->depth) {
				priv->visual = info[i].visual;
				priv->depth = info[i].depth;
			}
		}
	}
	XFree(info);
	if (!priv->visual) {
		priv->visual = DefaultVisual(display, template.screen);
		priv->depth = DefaultDepth(display, template.screen);
	}
	priv->colormap = XCreateColormap(display,
			RootWindow(display, template.screen),
			priv->visual, AllocNone);
exit:
	if (G_OBJECT_CLASS(joy_x11_screen_parent_class)->constructed) {
		G_OBJECT_CLASS(joy_x11_screen_parent_class)->
			constructed(base);
	}
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->windows) {
		g_ptr_array_free(priv->windows, TRUE);
		priv->windows = NULL;
	}
	if (priv->colormap) {
		Display *display =
			joy_x11_screen_get_display(JOY_SCREEN(base));
		XFreeColormap(display, priv->colormap);
		priv->colormap = None;

	}
	G_OBJECT_CLASS(joy_x11_screen_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->timer) {
		joy_timer_destroy(priv->timer);
	}
	G_OBJECT_CLASS(joy_x11_screen_parent_class)->finalize(base);
}

static JoyIterator *
begin(JoyScreen *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->windows)) {
		return joy_iterator_ptr_array_begin(priv->windows);
	}
	return NULL;
}

static JoyIterator *
end(JoyScreen *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->windows)) {
		return joy_iterator_ptr_array_end(priv->windows);
	}
	return NULL;
}

static JoyBubble *
window_create(JoyScreen *self)
{
	struct Private *priv = GET_PRIVATE(self);
	JoyBubble *window = joy_x11_window_new(self);
	if (G_UNLIKELY(!window)) {
		return NULL;
	}
	g_ptr_array_add(priv->windows, window);
	return window;
}

static cairo_surface_type_t
cairo_surface_type(JoyScreen *self)
{
	return CAIRO_SURFACE_TYPE_XLIB;
}

static cairo_surface_t *
cairo_surface_create(JoyScreen *self, gint width, gint height)
{
	Pixmap pixmap = None;
	cairo_surface_t *surface = NULL;
	Display *display = joy_x11_screen_get_display(self);
	if (G_UNLIKELY(!display)) {
		goto error;
	}
	Window window = RootWindow(display, joy_screen_get_id(self));
	if (G_UNLIKELY(!window)) {
		goto error;
	}
	pixmap = XCreatePixmap(display, window, width, height,
			GET_PRIVATE(self)->depth);
	if (G_UNLIKELY(!pixmap)) {
		goto error;
	}
	surface = joy_x11_screen_cairo_surface_create(self, display, pixmap,
			width, height);
	if (G_UNLIKELY(!surface)) {
		goto error;
	}
	return surface;
error:
	if (pixmap) {
		XFreePixmap(display, pixmap);
	}
	if (surface) {
		cairo_surface_destroy(surface);
		surface = NULL;
	}
	return NULL;
}

static gulong
eta(JoyScreen *self)
{
	struct Private *priv = GET_PRIVATE(self);
	return priv->frame - joy_timespec_microseconds(&priv->submit);
}

JOY_GNUC_HOT
static void
submit(JoyScreen *self)
{
	struct Private *priv = GET_PRIVATE(self);
	Display *display = joy_x11_screen_get_display(self);
	joy_timer_start(priv->timer);
	for (gint i = 0; i < priv->windows->len; ++i) {
		JoyBubble *window = priv->windows->pdata[i];
		joy_x11_window_submit(window, display);
	}
	joy_timer_elapsed(priv->timer, &priv->submit);
}

static gboolean
enable_mirroring(JoyScreen *self, JoyScreen *mirror, GError **error)
{
	JOY_UNIMPLEMENTED;
	g_set_error_literal(error, JOY_ERROR, JOY_ERROR_UNIMPLEMENTED,
			Q_("x11: mirroring is not implemented"));
	return FALSE;
}

static gboolean
disable_mirroring(JoyScreen *self, JoyScreen *mirror, GError **error)
{
	JOY_UNIMPLEMENTED;
	g_set_error_literal(error, JOY_ERROR, JOY_ERROR_UNIMPLEMENTED,
			Q_("x11: mirroring is not implemented"));
	return FALSE;
}

static void
joy_x11_screen_class_init(JoyX11ScreenClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructed = constructed;
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	JoyScreenClass *screen_class = JOY_SCREEN_CLASS(klass);
	screen_class->begin = begin;
	screen_class->end = end;
	screen_class->window_create = window_create;
	screen_class->cairo_surface_type = cairo_surface_type;
	screen_class->cairo_surface_create = cairo_surface_create;
	screen_class->eta = eta;
	screen_class->submit = submit;
	screen_class->enable_mirroring = enable_mirroring;
	screen_class->disable_mirroring = disable_mirroring;
	g_type_class_add_private(klass, sizeof(struct Private));
}

JoyScreen *
joy_x11_screen_new(JoyApplication *app, guint id)
{
	g_return_val_if_fail(JOY_IS_X11_APPLICATION(app), NULL);
	g_return_val_if_fail(0 <= id, NULL);
	Display *display = joy_x11_application_get_display(app);
	if (G_UNLIKELY(!display)) {
		return NULL;
	}
	Screen *screen = ScreenOfDisplay(display, id);
	if (G_UNLIKELY(!screen)) {
		return NULL;
	}
	return g_object_new(JOY_TYPE_X11_SCREEN,
			"application", app,
			"width", WidthOfScreen(screen),
			"height", HeightOfScreen(screen),
			"screen-id", id,
			NULL);
}

void
joy_x11_screen_set_refresh(JoyScreen *self, gdouble refresh)
{
	g_return_if_fail(JOY_IS_X11_SCREEN(self));
	GET_PRIVATE(self)->frame = (1. / refresh) * 1e6;
}

Screen *
joy_x11_screen_get_screen(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_X11_SCREEN(self), NULL);
	return GET_PRIVATE(self)->screen;
}

Display *
joy_x11_screen_get_display(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_X11_SCREEN(self), NULL);
	JoyApplication *app = joy_screen_get_application(self);
	if (G_UNLIKELY(!app)) {
		return NULL;
	}
	return joy_x11_application_get_display(app);
}

Visual *
joy_x11_screen_get_visual(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_X11_SCREEN(self), NULL);
	return GET_PRIVATE(self)->visual;
}

Colormap
joy_x11_screen_get_colormap(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_X11_SCREEN(self), None);
	return GET_PRIVATE(self)->colormap;
}

gint
joy_x11_screen_get_depth(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_X11_SCREEN(self), 0);
	return GET_PRIVATE(self)->depth;
}

gboolean
joy_x11_screen_has_composite(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_X11_SCREEN(self), 0);
	return GET_PRIVATE(self)->composite;
}

struct Data {
	Display *display;
	Pixmap pixmap;
};

static void
data_destroy(struct Data *data)
{
	if (data) {
		XFreePixmap(data->display, data->pixmap);
		g_slice_free(struct Data, data);
	}
}

cairo_surface_t *
joy_x11_screen_cairo_surface_create(JoyScreen *self, Display *display,
		Pixmap pixmap, gint width, gint height)
{
	static const cairo_user_data_key_t key;
	cairo_surface_t *surface = NULL;
	struct Data *data = NULL;
	// create cairo surface
	surface = cairo_xlib_surface_create(display, pixmap,
			GET_PRIVATE(self)->visual, width, height);
	cairo_status_t status = cairo_surface_status(surface);
	if (G_UNLIKELY(CAIRO_STATUS_SUCCESS != status)) {
		goto error;
	}
	// attach destroy data
	data = g_slice_new0(struct Data);
	if (G_UNLIKELY(!data)) {
		goto error;
	}
	data->display = display;
	data->pixmap = pixmap;
	status = cairo_surface_set_user_data(surface, &key, data,
			(cairo_destroy_func_t)data_destroy);
	if (G_UNLIKELY(CAIRO_STATUS_SUCCESS != status)) {
		goto error;
	}
	return surface;
error:
	if (data) {
		g_slice_free(struct Data, data);
	}
	if (surface) {
		cairo_surface_destroy(surface);
	}
	return NULL;
}
