/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include <input_mgr_lite.h>
#include "joy/bubble.h"
#include "joy/platform/gfx3d/application.h"
#include "joy/platform/gfx3d/screen.h"
#include "joy/platform/gfx3d/source.h"
#include "joy/platform/gfx3d/window.h"
#include "joy/timer.h"
#include "joy/timespec.h"
#include <unistd.h>

G_DEFINE_TYPE(JoyGfx3dSource, joy_gfx3d_source, JOY_TYPE_SOURCE)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_GFX3D_SOURCE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyGfx3dSource *)instance)->priv)

struct Private {
	JoyApplication *app;
	JoyScreen *screen;
	JoyTimer *timer;
	gint tv;
};

static void
joy_gfx3d_source_init(JoyGfx3dSource *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->timer = joy_timer_new();
	joy_timer_start(priv->timer);
	priv->tv = IM_TV_NONE;
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->app) {
		g_object_unref(priv->app);
		priv->app = NULL;
	}
	G_OBJECT_CLASS(joy_gfx3d_source_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	JoySource *self = JOY_SOURCE(base);
	struct Private *priv = GET_PRIVATE(base);
	close(joy_source_get_descriptor(self));
	if (priv->timer) {
		joy_timer_destroy(priv->timer);
	}
	G_OBJECT_CLASS(joy_gfx3d_source_parent_class)->finalize(base);
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
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

/**
 * \brief Set a modifier key for the specified keyboard device.
 *
 * \param event [in] An input_mgr event.
 * \param keyboard [in] The keyboard device that generated \e event.
 * \param sym [in] The translated key sym.
 */
static void
modifier(const IM_INPUT *event, JoyDevice *keyboard, JoyKeySym sym)
{
	switch (event->state) {
	case IM_KEY_DOWN:
		joy_device_keyboard_modifier_down(keyboard, sym);
		return;
	case IM_KEY_UP:
		joy_device_keyboard_modifier_up(keyboard, sym);
		return;
	default:
		return;
	}
}

/**
 * \brief Translate input_mgr key codes into Joybubbles key syms.
 *
 * \param event [in] An input_mgr event.
 * \param keyboard [in] The keyboard device that generated \e event.
 */
static inline JoyKeySym
get_key(const IM_INPUT *event, JoyDevice *keyboard)
{
	switch (event->key) {
	case INPUT_KEY_POWERTOGGLE:
	case INPUT_KEY_POWER_ON:
	case INPUT_KEY_POWER_OFF:
		return JOY_KEY_Power;
	case INPUT_KEY_SAT_MODE:
		return JOY_KEY_Sat;
	case INPUT_KEY_TV_MODE:
		return JOY_KEY_TV;
	case INPUT_KEY_DVD_MODE:
		return JOY_KEY_DVD;
	case INPUT_KEY_AUX_MODE:
		return JOY_KEY_AUX;
	case INPUT_KEY_ENTER:
		return JOY_KEY_Return;
	case INPUT_KEY_GUIDE:
		return JOY_KEY_Guide;
	case INPUT_KEY_MENU:
		return JOY_KEY_Menu;
	case INPUT_KEY_PIP_TOGGLE:
		return JOY_KEY_PiP;
	case INPUT_KEY_PIP_POSITION:
		return JOY_KEY_Position;
	case INPUT_KEY_PIP_SWAP:
		return JOY_KEY_Swap;
	case INPUT_KEY_RED:
		return JOY_KEY_Red;
	case INPUT_KEY_GREEN:
		return JOY_KEY_Green;
	case INPUT_KEY_YELLOW:
		return JOY_KEY_Yellow;
	case INPUT_KEY_BLUE:
		return JOY_KEY_Blue;
	case INPUT_KEY_DVR:
		return JOY_KEY_DVR;
	case INPUT_KEY_SEARCH:
		return JOY_KEY_Search;
	case INPUT_KEY_PAUSE:
		return JOY_KEY_Pause;
	case INPUT_KEY_PLAY:
		return JOY_KEY_Play;
	case INPUT_KEY_RECORD:
		return JOY_KEY_Record;
	case INPUT_KEY_REWIND:
		return JOY_KEY_Rewind;
	case INPUT_KEY_FASTFORWARD:
		return JOY_KEY_Forward;
	case INPUT_KEY_STOP:
		return JOY_KEY_Stop;
	case INPUT_KEY_SKIPBACK:
		return JOY_KEY_Skip_Back;
	case INPUT_KEY_SKIPAHEAD:
		return JOY_KEY_Skip_Forward;
	case INPUT_KEY_INPUT:
		return JOY_KEY_Input;
	case INPUT_KEY_JUMP:
		return JOY_KEY_Jump;
	case INPUT_KEY_INFO:
		return JOY_KEY_Info;
	case INPUT_KEY_LIVE_TV:
		return JOY_KEY_Live_TV;
	case INPUT_KEY_DISH:
		return JOY_KEY_Dish;
	case INPUT_KEY_UP:
		return JOY_KEY_Up;
	case INPUT_KEY_DOWN:
		return JOY_KEY_Down;
	case INPUT_KEY_LEFT:
		return JOY_KEY_Left;
	case INPUT_KEY_RIGHT:
		return JOY_KEY_Right;
	case INPUT_KEY_CANCEL:
		return JOY_KEY_Cancel;
	case INPUT_KEY_PAGE_UP:
		return JOY_KEY_Page_Up;
	case INPUT_KEY_PAGE_DOWN:
		return JOY_KEY_Page_Down;
	case INPUT_KEY_1:
		return JOY_KEY_1;
	case INPUT_KEY_2:
		return JOY_KEY_2;
	case INPUT_KEY_3:
		return JOY_KEY_3;
	case INPUT_KEY_4:
		return JOY_KEY_4;
	case INPUT_KEY_5:
		return JOY_KEY_5;
	case INPUT_KEY_6:
		return JOY_KEY_6;
	case INPUT_KEY_7:
		return JOY_KEY_7;
	case INPUT_KEY_8:
		return JOY_KEY_8;
	case INPUT_KEY_9:
		return JOY_KEY_9;
	case INPUT_KEY_0:
		return JOY_KEY_0;
	case INPUT_KEY_STAR:
		return JOY_KEY_asterisk;
	case INPUT_KEY_POUND:
		return JOY_KEY_numbersign;
	case INPUT_KEY_LSHIFT:
		modifier(event, keyboard, JOY_KEY_Shift_L);
		return JOY_KEY_Shift_L;
	case INPUT_KEY_RSHIFT:
		modifier(event, keyboard, JOY_KEY_Shift_R);
		return JOY_KEY_Shift_R;
	case LINUX_KEY_LEFTCTRL:
		modifier(event, keyboard, JOY_KEY_Control_L);
		return JOY_KEY_Control_L;
	case LINUX_KEY_RIGHTCTRL:
		modifier(event, keyboard, JOY_KEY_Control_R);
		return JOY_KEY_Control_R;
	case LINUX_KEY_CAPSLOCK:
		modifier(event, keyboard, JOY_KEY_Caps_Lock);
		return JOY_KEY_Caps_Lock;
	case LINUX_KEY_LEFTMETA:
		modifier(event, keyboard, JOY_KEY_Meta_L);
		return JOY_KEY_Meta_L;
	case LINUX_KEY_RIGHTMETA:
		modifier(event, keyboard, JOY_KEY_Meta_R);
		return JOY_KEY_Meta_R;
	case LINUX_KEY_LEFTALT:
		modifier(event, keyboard, JOY_KEY_Alt_L);
		return JOY_KEY_Alt_L;
	case LINUX_KEY_RIGHTALT:
		modifier(event, keyboard, JOY_KEY_Alt_R);
		return JOY_KEY_Alt_R;
	case LINUX_KEY_ESC:
		return JOY_KEY_Escape;
	case INPUT_KEY_F1:
		return JOY_KEY_F1;
	case INPUT_KEY_F2:
		return JOY_KEY_F2;
	case INPUT_KEY_F3:
		return JOY_KEY_F3;
	case INPUT_KEY_F4:
		return JOY_KEY_F4;
	case INPUT_KEY_F5:
		return JOY_KEY_F5;
	case INPUT_KEY_F6:
		return JOY_KEY_F6;
	case INPUT_KEY_F7:
		return JOY_KEY_F7;
	case INPUT_KEY_F8:
		return JOY_KEY_F8;
	case INPUT_KEY_F9:
		return JOY_KEY_F9;
	case INPUT_KEY_F10:
		return JOY_KEY_F10;
	case INPUT_KEY_F11:
		return JOY_KEY_F11;
	case INPUT_KEY_F12:
		return JOY_KEY_F12;
	default:
		g_message(Q_("gfx3d: untranslated key: %d"), event->key);
		return event->key;
	}
}

/**
 * \brief Handle key & mouse button events.
 *
 * \param self [in] A source object.
 * \param screen [in] The screen \e event occurred on.
 * \param event [in] An input_mgr event.
 * \param timestamp [in] The timestamp for \e event.
 */
static inline void
non_motion_event(G_GNUC_UNUSED JoySource *self,
		 JoyScreen *screen,
		 const IM_INPUT *event,
		 gulong timestamp)
{
	gint x, y;
	joy_gfx3d_screen_get_cursor_position(screen, &x, &y);
	JoyBubble *window = joy_screen_window_at(screen, x, y);
	if (!window) {
		return;
	}
	JoyDevice *device;
	switch (event->key) {
	case 0x8110: // left mouse button
		switch (event->state) {
		case IM_KEY_DOWN:
			device = joy_gfx3d_screen_get_mouse(screen);
			joy_device_mouse_button_down(device,
					JOY_MOUSE_BUTTON_LEFT);
			joy_bubble_button_down(window, device, timestamp,
					x, y, JOY_MOUSE_BUTTON_LEFT);
			break;
		case IM_KEY_UP:
			device = joy_gfx3d_screen_get_mouse(screen);
			joy_device_mouse_button_up(device,
					JOY_MOUSE_BUTTON_LEFT);
			joy_bubble_button_up(window, device, timestamp,
					x, y, JOY_MOUSE_BUTTON_LEFT);
			break;
		default:
			g_message(Q_("gfx3d: unknown button state: %d"),
					event->state);
			break;
		}
		break;
	case 0x88: // right mouse button
		switch (event->state) {
		case IM_KEY_DOWN:
			device = joy_gfx3d_screen_get_mouse(screen);
			joy_device_mouse_button_down(device,
					JOY_MOUSE_BUTTON_RIGHT);
			joy_bubble_button_down(window, device, timestamp,
					x, y, JOY_MOUSE_BUTTON_RIGHT);
			break;
		case IM_KEY_UP:
			device = joy_gfx3d_screen_get_mouse(screen);
			joy_device_mouse_button_up(device,
					JOY_MOUSE_BUTTON_RIGHT);
			joy_bubble_button_up(window, device, timestamp,
					x, y, JOY_MOUSE_BUTTON_RIGHT);
			break;
		default:
			g_message(Q_("gfx3d: unknown button state: %d"),
					event->state);
			break;
		}
		break;
	case 0x6005: // scroll wheel
		switch (event->data[0]) {
		case 255:
			device = joy_gfx3d_screen_get_mouse(screen);
			joy_bubble_scroll(window, device, timestamp,
					x, y, JOY_MOUSE_SCROLL_DOWN);
			break;
		case 1:
			device = joy_gfx3d_screen_get_mouse(screen);
			joy_bubble_scroll(window, device, timestamp,
					x, y, JOY_MOUSE_SCROLL_UP);
			break;
		default:
			g_message(Q_("gfx3d: unknown scroll direction: %d"),
					event->data[0]);
			break;
		}
		break;
	default: // key/button press
		switch (event->state) {
		case IM_KEY_DISCRETE:
			device = joy_gfx3d_screen_get_keyboard(screen);
			joy_bubble_key_down(window, device, timestamp,
					x, y, get_key(event, device));
			joy_bubble_key_up(window, device, timestamp,
					x, y, get_key(event, device));
			break;
		case IM_KEY_DOWN:
			device = joy_gfx3d_screen_get_keyboard(screen);
			joy_bubble_key_down(window, device, timestamp,
					x, y, get_key(event, device));
			break;
		case IM_KEY_UP:
			device = joy_gfx3d_screen_get_keyboard(screen);
			joy_bubble_key_up(window, device, timestamp,
					x, y, get_key(event, device));
			break;
		default:
			g_message(Q_("gfx3d: unknown key state: %d"),
					event->state);
			break;
		}
		break;
	}
}

/**
 * \brief Dispatch a motion event.
 *
 * \param self [in] A source object.
 * \param screen [in] The screen the event occurred on.
 * \param timestamp [in] The timestamp for the event.
 */
static inline void
motion_event(G_GNUC_UNUSED JoySource *self,
	     JoyScreen *screen,
	     gulong timestamp)
{
	gint x, y;
	joy_gfx3d_screen_get_cursor_position(screen, &x, &y);
	JoyBubble *window = joy_screen_window_at(screen, x, y);
	if (window) {
		JoyDevice *mouse = joy_gfx3d_screen_get_mouse(screen);
		joy_bubble_motion(window, mouse, timestamp, x, y);
	}
}

/**
 * \brief Handle a relative motion event.
 *
 * \param self [in] A source object.
 * \param screen [in] The screen \e event occurred on.
 * \param event [in] An input_mgr event.
 * \param timestamp [in] The timestamp for \e event.
 */
static inline void
relative_motion_event(JoySource *self, JoyScreen *screen, IM_INPUT *event,
		gulong timestamp)
{
	gint x = (gshort)(event->data[0] | event->data[1] << 8);
	gint y = -(gshort)(event->data[2] | event->data[3] << 8);
	joy_gfx3d_screen_move_cursor(screen, x, y);
	motion_event(self, screen, timestamp);
}

/**
 * \brief Handle an absolute motion event.
 *
 * \param self [in] A source object.
 * \param screen [in] The screen \e event occurred on.
 * \param event [in] An input_mgr event.
 * \param timestamp [in] The timestamp for \e event.
 */
static inline void
absolute_motion_event(JoySource *self, JoyScreen *screen, IM_INPUT *event,
		gulong timestamp)
{
	gint x = (gshort)(event->data[0] | event->data[1] << 8);
	gint y = -(gshort)(event->data[2] | event->data[3] << 8);
	joy_gfx3d_screen_warp_cursor(screen, x, y);
	motion_event(self, screen, timestamp);
}

/**
 * \brief Get a (possibly cached) screen.
 *
 * \param self [in] A source object.
 * \param tv [in] An input manager TV ID.
 *
 * \return The screen for \e tv or NULL if there is no such screen.
 */
static inline JoyScreen *
get_screen(JoySource *self, gint tv)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(tv == priv->tv)) {
		return priv->screen;
	}
	switch (tv) {
	case IM_TV1:
		priv->screen = joy_application_get_screen(priv->app, 0);
		break;
	case IM_TV2:
		priv->screen = joy_application_get_screen(priv->app, 1);
		break;
	default:
		priv->screen = NULL;
		break;
	}
	priv->tv = priv->screen ? tv : IM_TV_NONE;
	return priv->screen;
}

static void
input(JoySource *self)
{
	struct Private *priv = GET_PRIVATE(self);
	gint fd = joy_source_get_descriptor(self);
	IM_INPUT event;
	while (TRUE) {
		ssize_t bytes = read(fd, &event, sizeof(event));
		if (G_UNLIKELY(-1 == bytes)) {
			if (G_UNLIKELY(EAGAIN != errno)) {
				g_message("gfx3d: %s", g_strerror(errno));
			}
			return;
		}
		if (G_UNLIKELY(sizeof(event) != bytes)) {
			g_message(Q_("gfx3d: incomplete event (%d)"), bytes);
			continue;
		}
		JoyScreen *screen = get_screen(self, event.tv);
		if (G_UNLIKELY(!screen)) {
			g_message(Q_("gfx3d: unrecognized TV (%d)"), event.tv);
			continue;
		}
		struct timespec elapsed;
		joy_timer_elapsed(priv->timer, &elapsed);
		gulong timestamp = joy_timespec_milliseconds(&elapsed);
		switch (event.key) {
		case INPUT_KEY_RELATIVE_POINT:
			relative_motion_event(self, screen, &event, timestamp);
			continue;
		case INPUT_KEY_ABSOLUTE_POINT:
			absolute_motion_event(self, screen, &event, timestamp);
			continue;
		default:
			non_motion_event(self, screen, &event, timestamp);
			continue;
		}
	}
}

static void
joy_gfx3d_source_class_init(JoyGfx3dSourceClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	JoySourceClass *source_class = JOY_SOURCE_CLASS(klass);
	source_class->input = input;
	g_type_class_add_private(klass, sizeof(struct Private));
	// properties
	g_object_class_install_property(object_class, PROP_APPLICATION,
		g_param_spec_object("application", Q_("GFX3D Application"),
			Q_("The application"), JOY_TYPE_GFX3D_APPLICATION,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoySource *
joy_gfx3d_source_new(JoyApplication *app, gint descriptor)
{
	g_return_val_if_fail(JOY_IS_GFX3D_APPLICATION(app), NULL);
	return g_object_new(JOY_TYPE_GFX3D_SOURCE,
			"descriptor", descriptor,
			"condition", G_IO_IN,
			"application", app,
			NULL);
}
