/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/bubble.h"
#include "joy/platform/x11/application.h"
#include "joy/platform/x11/screen.h"
#include "joy/platform/x11/source.h"
#include "joy/platform/x11/window.h"
#include <X11/Xutil.h>

G_DEFINE_TYPE(JoyX11Source, joy_x11_source, JOY_TYPE_SOURCE)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_X11_SOURCE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyX11Source *)instance)->priv)

struct Private {
	JoyApplication *app;
	Display *display;
	JoyBubble *window;
	XID xid;
};

static void
joy_x11_source_init(JoyX11Source *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->app) {
		g_object_unref(priv->app);
		priv->app = NULL;
	}
	G_OBJECT_CLASS(joy_x11_source_parent_class)->dispose(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_APPLICATION,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_APPLICATION:
		priv->app = g_value_dup_object(value);
		priv->display = joy_x11_application_get_display(priv->app);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static gboolean
prepare(JoySource *self)
{
	if (XPending(GET_PRIVATE(self)->display)) {
		return TRUE;
	}
	return FALSE;
}

static inline JoyMouseButton
get_button(gint button)
{
	switch (button) {
	case Button1:
		return JOY_MOUSE_BUTTON_LEFT;
	case Button2:
		return JOY_MOUSE_BUTTON_MIDDLE;
	case Button3:
		return JOY_MOUSE_BUTTON_RIGHT;
	case 8:
		return JOY_MOUSE_BUTTON_X1;
	case 9:
		return JOY_MOUSE_BUTTON_X2;
	default:
		return JOY_MOUSE_BUTTON_NONE;
	}
}

static inline JoyKeySym
get_key(XKeyEvent *event)
{
	gulong sym = XLookupKeysym(event, 0);
	if (event->state & ShiftMask) {
		gulong lower;
		XConvertCase(sym, &lower, &sym);
	}
	return (JoyKeySym)sym;
}

#ifdef JOY_DEBUG
static const gchar *const events[] = {
	[KeyPress] = "KeyPress",
	[KeyRelease] = "KeyRelease",
	[ButtonPress] = "ButtonPress",
	[ButtonRelease] = "ButtonRelease",
	[MotionNotify] = "MotionNotify",
	[EnterNotify] = "EnterNotify",
	[LeaveNotify] = "LeaveNotify",
	[FocusIn] = "FocusIn",
	[FocusOut] = "FocusOut",
	[KeymapNotify] = "KeymapNotify",
	[Expose] = "Expose",
	[GraphicsExpose] = "GraphicsExpose",
	[NoExpose] = "NoExpose",
	[VisibilityNotify] = "VisibilityNotify",
	[CreateNotify] = "CreateNotify",
	[DestroyNotify] = "DestroyNotify",
	[UnmapNotify] = "UnmapNotify",
	[MapNotify] = "MapNotify",
	[MapRequest] = "MapRequest",
	[ReparentNotify] = "ReparentNotify",
	[ConfigureNotify] = "ConfigureNotify",
	[ConfigureRequest] = "ConfigureRequest",
	[GravityNotify] = "GravityNotify",
	[ResizeRequest] = "ResizeRequest",
	[CirculateNotify] = "CirculateNotify",
	[CirculateRequest] = "CirculateRequest",
	[PropertyNotify] = "PropertyNotify",
	[SelectionClear] = "SelectionClear",
	[SelectionRequest] = "SelectionRequest",
	[SelectionNotify] = "SelectionNotify",
	[ColormapNotify] = "ColormapNotify",
	[ClientMessage] = "ClientMessage",
	[MappingNotify] = "MappingNotify",
	[LASTEvent] = "LASTEvent"
};
#endif

static inline void
handle_event(JoySource *self, JoyBubble *window, XEvent *event)
{
	struct Private *priv = GET_PRIVATE(self);
	switch (event->type) {
	case ButtonPress:
		switch (event->xbutton.button) {
		case 4:
			joy_bubble_scroll(window,
					joy_x11_application_get_mouse(
						priv->app),
					event->xbutton.time,
					event->xbutton.x, event->xbutton.y,
					JOY_MOUSE_SCROLL_UP);
			break;
		case 5:
			joy_bubble_scroll(window,
					joy_x11_application_get_mouse(
						priv->app),
					event->xbutton.time,
					event->xbutton.x, event->xbutton.y,
					JOY_MOUSE_SCROLL_DOWN);
			break;
		case 6:
			joy_bubble_scroll(window,
					joy_x11_application_get_mouse(
						priv->app),
					event->xbutton.time,
					event->xbutton.x, event->xbutton.y,
					JOY_MOUSE_SCROLL_LEFT);
			break;
		case 7:
			joy_bubble_scroll(window,
					joy_x11_application_get_mouse(
						priv->app),
					event->xbutton.time,
					event->xbutton.x, event->xbutton.y,
					JOY_MOUSE_SCROLL_RIGHT);
			break;
		default:
			joy_bubble_button_down(window,
					joy_x11_application_get_mouse(
						priv->app),
					event->xbutton.time,
					event->xbutton.x, event->xbutton.y,
					get_button(event->xbutton.button));
			break;
		}
		break;
        case ButtonRelease:
		switch (event->xbutton.button) {
		case 4:
		case 5:
		case 6:
		case 7:
			break;
		default:
			joy_bubble_button_up(window,
					joy_x11_application_get_mouse(
						priv->app),
					event->xbutton.time,
					event->xbutton.x, event->xbutton.y,
					get_button(event->xbutton.button));
			break;
		}
		break;
	case ConfigureNotify:
		joy_bubble_move(window, event->xconfigure.x,
				event->xconfigure.y);
		joy_bubble_resize(window, event->xconfigure.width,
				event->xconfigure.height);
		break;
	case EnterNotify:
		if (NotifyNormal != event->xcrossing.mode) {
			break;
		}
		joy_bubble_enter(joy_bubble_at(window, event->xcrossing.x,
					event->xcrossing.y),
				joy_x11_application_get_mouse(priv->app),
				event->xcrossing.time, event->xcrossing.x,
				event->xcrossing.y);
		break;
	case LeaveNotify:
		if (NotifyNormal != event->xcrossing.mode) {
			break;
		}
		JoyDevice *mouse = joy_x11_application_get_mouse(priv->app);
		joy_bubble_leave(joy_bubble_at_device(window, mouse),
				joy_x11_application_get_mouse(priv->app),
				event->xcrossing.time,
				event->xcrossing.x, event->xcrossing.y);
		JoyScreen *screen = joy_bubble_get_screen(window);
		if (G_LIKELY(screen)) {
			joy_screen_set_at_device(screen, mouse, NULL);
		}
		break;
	case Expose:
		{
			cairo_rectangle_int_t rect = {
				event->xexpose.x,
				event->xexpose.y,
				event->xexpose.width,
				event->xexpose.height
			};
			joy_bubble_expose(window, &rect);
		}
		break;
	case MapNotify:
		joy_bubble_show(window);
		break;
	case MotionNotify:
		joy_bubble_motion(window,
				joy_x11_application_get_mouse(priv->app),
				event->xmotion.time,
				event->xmotion.x, event->xmotion.y);
		break;
	case UnmapNotify:
		joy_bubble_hide(window);
		break;
	case KeyPress:
		joy_bubble_key_down(window,
				joy_x11_application_get_keyboard(priv->app),
				event->xkey.time, event->xkey.x, event->xkey.y,
				get_key(&event->xkey));
		break;
	case KeyRelease:
		if (XPending(priv->display)) {
			XEvent next;
			XPeekEvent(priv->display, &next);
			if (next.type == KeyPress
				&& next.xkey.keycode == event->xkey.keycode
				&& next.xkey.time == event->xkey.time) {
				break;
			}
		}
		joy_bubble_key_up(window,
				joy_x11_application_get_keyboard(priv->app),
				event->xkey.time,
				event->xkey.x, event->xkey.y,
				get_key(&event->xkey));
		break;
	case DestroyNotify:
		joy_x11_window_destroy(window);
		break;
	case FocusOut:
	case FocusIn:
        case CirculateNotify:
	case GravityNotify:
	case ReparentNotify:
	case NoExpose:
	case ClientMessage:
		// intentionally unhandled events
		break;
	default:
#ifdef JOY_DEBUG
		g_message(Q_("x11: unhandled event %s [%d]"),
				events[event->type], event->type);
#else // JOY_DEBUG
		g_message(Q_("x11: unhandled event [%d]"), event->type);
		break;
#endif // JOY_DEBUG
	}
}

static void
input(JoySource *self)
{
	struct Private *priv = GET_PRIVATE(self);
	while (XPending(priv->display)) {
		XEvent event;
		XNextEvent(priv->display, &event);
		if (G_UNLIKELY(priv->xid != event.xany.window)) {
			if (priv->window) {
				joy_x11_window_set_event(priv->window, NULL);
			}
			priv->xid = event.xany.window;
			priv->window = joy_x11_application_lookup_xid(
					priv->app, event.xany.window);
			if (G_UNLIKELY(!priv->window)) {
				g_message(Q_("x11: unknown XID [%lx]"), priv->xid);
				continue;
			}
		}
		joy_x11_window_set_event(priv->window, &event);
		handle_event(self, priv->window, &event);
	}
	if (G_LIKELY(priv->window)) {
		joy_x11_window_set_event(priv->window, NULL);
	}
}

static void
joy_x11_source_class_init(JoyX11SourceClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->set_property = set_property;
	JoySourceClass *source_class = JOY_SOURCE_CLASS(klass);
	source_class->prepare = prepare;
	source_class->input = input;
	g_type_class_add_private(klass, sizeof(struct Private));
	// properties
	g_object_class_install_property(object_class, PROP_APPLICATION,
		g_param_spec_object("application", Q_("X11 Application"),
			Q_("The X11 application"), JOY_TYPE_X11_APPLICATION,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoySource *
joy_x11_source_new(JoyApplication *app)
{
	g_return_val_if_fail(JOY_IS_X11_APPLICATION(app), NULL);
	Display *display = joy_x11_application_get_display(app);
	if (G_UNLIKELY(!display)) {
		return NULL;
	}
	return g_object_new(JOY_TYPE_X11_SOURCE,
			"descriptor", ConnectionNumber(display),
			"condition", G_IO_IN,
			"application", app,
			NULL);
}
