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
#include "joy/macros.h"
#include "joy/platform/x11/application.h"
#include "joy/platform/x11/screen.h"
#include "joy/platform/x11/window.h"
#include <string.h>
#ifdef HAVE_XCOMPOSITE
#include <X11/extensions/Xcomposite.h>
#endif
#include <X11/Xutil.h>

G_DEFINE_TYPE(JoyX11Window, joy_x11_window, JOY_TYPE_WINDOW)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_X11_WINDOW, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyX11Window *)instance)->priv)

struct Private {
	Window window;
	Region region;
	GC gc;
	cairo_surface_t *surface;
	XEvent *event;
};

static void
joy_x11_window_init(JoyX11Window *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
constructed(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	JoyBubble *self = JOY_BUBBLE(base);
	// set window attributes
	JoyScreen *screen = joy_bubble_get_screen(self);
	if (G_UNLIKELY(!screen)) {
		goto exit;
	}
	JoyApplication *app = joy_screen_get_application(screen);
	if (G_UNLIKELY(!app)) {
		goto exit;
	}
	Display *display = joy_x11_application_get_display(app);
	if (G_UNLIKELY(!display)) {
		goto exit;
	}
	gulong mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;
	XSetWindowAttributes attr;
	attr.background_pixel = 0;
	attr.border_pixel = 0;
	attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask
		| ButtonPressMask | ButtonReleaseMask | PointerMotionMask
		| EnterWindowMask | LeaveWindowMask | StructureNotifyMask;
	attr.colormap = joy_x11_screen_get_colormap(screen);
	// create window
	priv->window = XCreateWindow(display,
			RootWindow(display, joy_screen_get_id(screen)),
			joy_bubble_get_x(self),
			joy_bubble_get_y(self),
			joy_bubble_get_width(self),
			joy_bubble_get_height(self),
			0, joy_x11_screen_get_depth(screen), InputOutput,
			joy_x11_screen_get_visual(screen), mask, &attr);
	if (G_UNLIKELY(!priv->window)) {
		goto exit;
	}
	// set the window name
	const gchar *name = joy_application_get_name(app);
	if (G_LIKELY(name)) {
		Atom format = XInternAtom(display, "UTF8_STRING", False);
		XChangeProperty(display, priv->window,
				XInternAtom(display, "_NET_WM_NAME", False),
				format, 8, PropModeReplace, (guchar *)name,
				strlen(name));
		XChangeProperty(display, priv->window,
				XInternAtom(display, "WM_NAME", False),
				format, 8, PropModeReplace, (guchar *)name,
				strlen(name));
		XChangeProperty(display, priv->window,
				XInternAtom(display, "WM_ICON_NAME", False),
				format, 8, PropModeReplace, (guchar *)name,
				strlen(name));
	}
	// create the graphics context & clip region
	priv->gc = XCreateGC(display, priv->window, 0, NULL);
	if (G_LIKELY(priv->gc)) {
		XSetGraphicsExposures(display, priv->gc, False);
	}
	priv->region = XCreateRegion();
	// hide the titlebar & borders
	struct {
		gulong flags;
		gulong functions;
		gulong decorations;
		glong inputMode;
		gulong status;
	} hints = {
		2, 0, 0, 0, 0
	};
	Atom atom = XInternAtom(display, "_MOTIF_WM_HINTS", False);
	XChangeProperty(display, priv->window, atom, atom, 32,
			PropModeReplace, (guchar *)&hints, 5);
	// store object mapping
	joy_x11_application_insert_xid(app, priv->window, self);
#ifdef HAVE_XCOMPOSITE
	if (joy_x11_screen_has_composite(screen)) {
		XCompositeRedirectWindow(display, priv->window,
				CompositeRedirectAutomatic);
	}
#endif
exit:
	if (G_OBJECT_CLASS(joy_x11_window_parent_class)->constructed) {
		G_OBJECT_CLASS(joy_x11_window_parent_class)->
			constructed(base);
	}
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	JoyBubble *self = JOY_BUBBLE(base);
	if (priv->surface) {
		cairo_surface_destroy(priv->surface);
		priv->surface = NULL;
	}
	if (priv->region) {
		XDestroyRegion(priv->region);
		priv->region = NULL;
	}
	if (priv->gc) {
		Display *display = joy_x11_window_get_display(self);
		if (display) {
			XFreeGC(display, priv->gc);
		}
		priv->gc = NULL;
	}
	if (priv->window) {
		JoyApplication *app = joy_bubble_get_application(self);
		if (app) {
			joy_x11_application_remove_xid(app, priv->window);
			Display *display =
				joy_x11_application_get_display(app);
			if (display) {
				XDestroyWindow(display, priv->window);
			}
		}
		priv->window = None;
	}
	G_OBJECT_CLASS(joy_x11_window_parent_class)->dispose(base);
}

static void
move(JoyBubble *self, gint x, gint y)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->event || G_UNLIKELY(!priv->window)) {
		return;
	}
	XMoveWindow(joy_x11_window_get_display(self), priv->window, x, y);
}

static void
resize(JoyBubble *self, gint width, gint height)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->surface) {
		cairo_surface_destroy(priv->surface);
		priv->surface = NULL;
	}
	if (priv->event || G_UNLIKELY(!priv->window)) {
		goto exit;
	}
	if (G_UNLIKELY(JOY_X11_WINDOW_MAX < width)) {
		width = JOY_X11_WINDOW_MAX;
	} else if (G_UNLIKELY(1 > width)) {
		width = 1;
	}
	if (G_UNLIKELY(JOY_X11_WINDOW_MAX < height)) {
		height = JOY_X11_WINDOW_MAX;
	} else if (G_UNLIKELY(1 > height)) {
		height = 1;
	}
	XResizeWindow(joy_x11_window_get_display(self), priv->window,
			width, height);
	if (priv->surface) {
		cairo_xlib_surface_set_size(priv->surface, width, height);
	}
exit:
	JOY_BUBBLE_CLASS(joy_x11_window_parent_class)->
		resize(self, width, height);
}

static void
expose(JoyBubble *self, const cairo_rectangle_int_t *rect)
{
	struct Private *priv = GET_PRIVATE(self);
	XRectangle rectangle = {
		rect->x, rect->y, rect->width, rect->height
	};
	XUnionRectWithRegion(&rectangle, priv->region, priv->region);
	JOY_BUBBLE_CLASS(joy_x11_window_parent_class)->expose(self, rect);
}

static void
show(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if  (priv->event || G_UNLIKELY(!priv->window)) {
		return;
	}
	XMapWindow(joy_x11_window_get_display(self), priv->window);
	JOY_BUBBLE_CLASS(joy_x11_window_parent_class)->show(self);
}

static void
hide(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if  (priv->event || G_UNLIKELY(!priv->window)) {
		return;
	}
	XUnmapWindow(joy_x11_window_get_display(self), priv->window);
	JOY_BUBBLE_CLASS(joy_x11_window_parent_class)->hide(self);
}

static cairo_surface_t *
cairo_surface_create(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->surface)) {
		return cairo_surface_reference(priv->surface);
	}
	if (G_UNLIKELY(!priv->window)) {
		goto error;
	}
	JoyScreen *screen = joy_bubble_get_screen(self);
	Display *display = joy_x11_screen_get_display(screen);
	gint width = joy_bubble_get_width(self);
	gint height = joy_bubble_get_height(self);
	Pixmap pixmap = XCreatePixmap(display, priv->window, width,
			height, joy_x11_screen_get_depth(screen));
	if (G_UNLIKELY(!pixmap)) {
		goto error;
	}
	priv->surface = joy_x11_screen_cairo_surface_create(screen,
			display, pixmap, width, height);
	if (G_UNLIKELY(!priv->surface)) {
		goto error;
	}
	return cairo_surface_reference(priv->surface);
error:
	if (priv->surface) {
		cairo_surface_destroy(priv->surface);
		priv->surface = NULL;
	}
	return NULL;
}

static void
raise_(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->window)) {
		XRaiseWindow(joy_x11_window_get_display(self), priv->window);
	}
}

static void
lower(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->window)) {
		XLowerWindow(joy_x11_window_get_display(self), priv->window);
	}
}

static void
joy_x11_window_class_init(JoyX11WindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructed = constructed;
	object_class->dispose = dispose;
	JoyBubbleClass *bubble_class = JOY_BUBBLE_CLASS(klass);
	bubble_class->move = move;
	bubble_class->resize = resize;
	bubble_class->expose = expose;
	bubble_class->show = show;
	bubble_class->hide = hide;
	JoyWindowClass *window_class = JOY_WINDOW_CLASS(klass);
	window_class->cairo_surface_create = cairo_surface_create;
	window_class->raise = raise_;
	window_class->lower = lower;
	g_type_class_add_private(klass, sizeof(struct Private));
}

JoyBubble *
joy_x11_window_new(JoyScreen *screen)
{
	g_return_val_if_fail(JOY_IS_X11_SCREEN(screen), NULL);
	return g_object_new(JOY_TYPE_X11_WINDOW,
			"screen", screen,
			"x-coordinate", 0,
			"y-coordinate", 0,
			"width", joy_screen_get_width(screen),
			"height", joy_screen_get_height(screen),
			"horizontal-expand", FALSE,
			"vertical-expand", FALSE,
			NULL);
}

Window
joy_x11_window_get_window(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_X11_WINDOW(self), None);
	return GET_PRIVATE(self)->window;
}

Screen *
joy_x11_window_get_screen(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_X11_WINDOW(self), NULL);
	JoyScreen *screen = joy_bubble_get_screen(self);
	if (G_UNLIKELY(!screen)) {
		return NULL;
	}
	return joy_x11_screen_get_screen(screen);
}

Display *
joy_x11_window_get_display(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_X11_WINDOW(self), NULL);
	JoyScreen *screen = joy_bubble_get_screen(self);
	if (G_UNLIKELY(!screen)) {
		return NULL;
	}
	return joy_x11_screen_get_display(screen);
}

void
joy_x11_window_destroy(JoyBubble *self)
{
	g_return_if_fail(JOY_IS_X11_WINDOW(self));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->surface) {
		cairo_surface_destroy(priv->surface);
		priv->surface = NULL;
	}
	if (priv->region) {
		XDestroyRegion(priv->region);
		priv->region = NULL;
	}
	if (priv->gc) {
		Display *display = joy_x11_window_get_display(self);
		if (display) {
			XFreeGC(display, priv->gc);
		}
		priv->gc = NULL;
	}
	priv->window = None;
}

void
joy_x11_window_set_event(JoyBubble *self, XEvent *event)
{
	g_return_if_fail(JOY_IS_X11_WINDOW(self));
	GET_PRIVATE(self)->event = event;
}

JOY_GNUC_HOT
void
joy_x11_window_submit(JoyBubble *self, Display *display)
{
	g_return_if_fail(JOY_IS_X11_WINDOW(self));
	g_return_if_fail(display);
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->window || !priv->surface || !priv->gc)) {
		return;
	}
	if (G_LIKELY(priv->region)) {
		XSetRegion(display, priv->gc, priv->region);
	}
	XCopyArea(display, cairo_xlib_surface_get_drawable(priv->surface),
			priv->window, priv->gc, 0, 0,
			joy_bubble_get_width(self),
			joy_bubble_get_height(self), 0, 0);
	if (G_LIKELY(priv->region)) {
		XEmptyRegion(priv->region);
	}
}
