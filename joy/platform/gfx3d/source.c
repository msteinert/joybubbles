/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <fcntl.h>
#include <input_mgr_lite.h>
#include "joy/bubble.h"
#include "joy/platform/gfx3d/application.h"
#include "joy/platform/gfx3d/screen.h"
#include "joy/platform/gfx3d/source.h"
#include "joy/platform/gfx3d/window.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

G_DEFINE_TYPE(JoyGfx3dSource, joy_gfx3d_source, JOY_TYPE_SOURCE)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_GFX3D_SOURCE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyGfx3dSource *)instance)->priv)

struct Private {
	JoyApplication *app;
	GTimer *timer;
};

static void
joy_gfx3d_source_init(JoyGfx3dSource *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->timer = g_timer_new();
	g_timer_start(priv->timer);
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
	int fd = joy_source_get_descriptor(self);
	if (-1 != fd) {
		close(fd);
	}
	if (priv->timer) {
		g_timer_destroy(priv->timer);
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

static inline JoyKeySym
get_key(IM_INPUT *event, JoyDevice *keyboard)
{
	gboolean modifier = FALSE;
	JoyKeySym sym = event->key;
	switch (event->key) {
	case INPUT_KEY_POWERTOGGLE:
	case INPUT_KEY_POWER_ON:
	case INPUT_KEY_POWER_OFF:
		sym = JOY_KEY_Power;
		break;
	case INPUT_KEY_SAT_MODE:
		sym = JOY_KEY_Sat;
		break;
	case INPUT_KEY_TV_MODE:
		sym = JOY_KEY_TV;
		break;
	case INPUT_KEY_DVD_MODE:
		sym = JOY_KEY_DVD;
		break;
	case INPUT_KEY_AUX_MODE:
		sym = JOY_KEY_AUX;
		break;
	case INPUT_KEY_ENTER:
		sym = JOY_KEY_Return;
		break;
	case INPUT_KEY_GUIDE:
		sym = JOY_KEY_Guide;
		break;
	case INPUT_KEY_MENU:
		sym = JOY_KEY_Menu;
		break;
	case INPUT_KEY_PIP_TOGGLE:
		sym = JOY_KEY_PiP;
		break;
	case INPUT_KEY_PIP_POSITION:
		sym = JOY_KEY_Position;
		break;
	case INPUT_KEY_PIP_SWAP:
		sym = JOY_KEY_Swap;
		break;
	case INPUT_KEY_RED:
		sym = JOY_KEY_Red;
		break;
	case INPUT_KEY_GREEN:
		sym = JOY_KEY_Green;
		break;
	case INPUT_KEY_YELLOW:
		sym = JOY_KEY_Yellow;
		break;
	case INPUT_KEY_BLUE:
		sym = JOY_KEY_Blue;
		break;
	case INPUT_KEY_DVR:
		sym = JOY_KEY_DVR;
		break;
	case INPUT_KEY_SEARCH:
		sym = JOY_KEY_Search;
		break;
	case INPUT_KEY_PAUSE:
		sym = JOY_KEY_Pause;
		break;
	case INPUT_KEY_PLAY:
		sym = JOY_KEY_Play;
		break;
	case INPUT_KEY_RECORD:
		sym = JOY_KEY_Record;
		break;
	case INPUT_KEY_REWIND:
		sym = JOY_KEY_Rewind;
		break;
	case INPUT_KEY_FASTFORWARD:
		sym = JOY_KEY_Forward;
		break;
	case INPUT_KEY_STOP:
		sym = JOY_KEY_Stop;
		break;
	case INPUT_KEY_SKIPBACK:
		sym = JOY_KEY_Skip_Back;
		break;
	case INPUT_KEY_SKIPAHEAD:
		sym = JOY_KEY_Skip_Forward;
		break;
	case INPUT_KEY_INPUT:
		sym = JOY_KEY_Input;
		break;
	case INPUT_KEY_JUMP:
		sym = JOY_KEY_Jump;
		break;
	case INPUT_KEY_INFO:
		sym = JOY_KEY_Info;
		break;
	case INPUT_KEY_LIVE_TV:
		sym = JOY_KEY_Live_TV;
		break;
	case INPUT_KEY_DISH:
		sym = JOY_KEY_Dish;
		break;
	case INPUT_KEY_UP:
		sym = JOY_KEY_Up;
		break;
	case INPUT_KEY_DOWN:
		sym = JOY_KEY_Down;
		break;
	case INPUT_KEY_LEFT:
		sym = JOY_KEY_Left;
		break;
	case INPUT_KEY_RIGHT:
		sym = JOY_KEY_Right;
		break;
	case INPUT_KEY_CANCEL:
		sym = JOY_KEY_Cancel;
		break;
	case INPUT_KEY_PAGE_UP:
		sym = JOY_KEY_Page_Up;
		break;
	case INPUT_KEY_PAGE_DOWN:
		sym = JOY_KEY_Page_Down;
		break;
	case INPUT_KEY_1:
		sym = JOY_KEY_1;
		break;
	case INPUT_KEY_2:
		sym = JOY_KEY_2;
		break;
	case INPUT_KEY_3:
		sym = JOY_KEY_3;
		break;
	case INPUT_KEY_4:
		sym = JOY_KEY_4;
		break;
	case INPUT_KEY_5:
		sym = JOY_KEY_5;
		break;
	case INPUT_KEY_6:
		sym = JOY_KEY_6;
		break;
	case INPUT_KEY_7:
		sym = JOY_KEY_7;
		break;
	case INPUT_KEY_8:
		sym = JOY_KEY_8;
		break;
	case INPUT_KEY_9:
		sym = JOY_KEY_9;
		break;
	case INPUT_KEY_0:
		sym = JOY_KEY_0;
		break;
	case INPUT_KEY_STAR:
		sym = JOY_KEY_asterisk;
		break;
	case INPUT_KEY_POUND:
		sym = JOY_KEY_numbersign;
		break;
	case INPUT_KEY_LSHIFT:
		modifier = TRUE;
		sym = JOY_KEY_Shift_L;
		break;
	case INPUT_KEY_RSHIFT:
		modifier = TRUE;
		sym = JOY_KEY_Shift_R;
		break;
	case LINUX_KEY_LEFTCTRL:
		modifier = TRUE;
		sym = JOY_KEY_Control_L;
		break;
	case LINUX_KEY_RIGHTCTRL:
		modifier = TRUE;
		sym = JOY_KEY_Control_R;
		break;
	case LINUX_KEY_CAPSLOCK:
		modifier = TRUE;
		sym = JOY_KEY_Caps_Lock;
	case LINUX_KEY_LEFTMETA:
		modifier = TRUE;
		sym = JOY_KEY_Meta_L;
		break;
	case LINUX_KEY_RIGHTMETA:
		modifier = TRUE;
		sym = JOY_KEY_Meta_R;
		break;
	case LINUX_KEY_LEFTALT:
		modifier = TRUE;
		sym = JOY_KEY_Alt_L;
		break;
	case LINUX_KEY_RIGHTALT:
		modifier = TRUE;
		sym = JOY_KEY_Alt_R;
		break;
	case LINUX_KEY_ESC:
		sym = JOY_KEY_Escape;
		break;
	case INPUT_KEY_F1:
		sym = JOY_KEY_F1;
		break;
	case INPUT_KEY_F2:
		sym = JOY_KEY_F2;
		break;
	case INPUT_KEY_F3:
		sym = JOY_KEY_F3;
		break;
	case INPUT_KEY_F4:
		sym = JOY_KEY_F4;
		break;
	case INPUT_KEY_F5:
		sym = JOY_KEY_F5;
		break;
	case INPUT_KEY_F6:
		sym = JOY_KEY_F6;
		break;
	case INPUT_KEY_F7:
		sym = JOY_KEY_F7;
		break;
	case INPUT_KEY_F8:
		sym = JOY_KEY_F8;
		break;
	case INPUT_KEY_F9:
		sym = JOY_KEY_F9;
		break;
	case INPUT_KEY_F10:
		sym = JOY_KEY_F10;
		break;
	case INPUT_KEY_F11:
		sym = JOY_KEY_F11;
		break;
	case INPUT_KEY_F12:
		sym = JOY_KEY_F12;
		break;
	default:
		g_message(Q_("gfx3d: untranslated key: %d"), event->key);
		break;
	}
	if (modifier) {
		switch (event->state) {
		case IM_KEY_DOWN:
			joy_device_keyboard_modifier_down(keyboard, sym);
			break;
		case IM_KEY_UP:
			joy_device_keyboard_modifier_up(keyboard, sym);
		default:
			break;
		}
	}
	return sym;
}

static inline void
non_motion_event(JoySource *self, JoyScreen *screen, IM_INPUT *event,
		unsigned long timestamp)
{
	gint x, y;
	joy_gfx3d_screen_get_cursor_position(screen, &x, &y);
	JoyBubble *window = joy_screen_window_at(screen, x, y);
	if (!window) {
		return;
	}
	JoyDevice *mouse = joy_gfx3d_screen_get_mouse(screen);
	JoyDevice *keyboard = joy_gfx3d_screen_get_keyboard(screen);
	switch (event->key) {
	case 0x8110: // left mouse button
		switch (event->state) {
		case IM_KEY_DOWN:
			joy_device_mouse_button_down(mouse, JOY_BUTTON_LEFT);
			joy_bubble_button_down(window, mouse, timestamp, x, y,
					JOY_BUTTON_LEFT);
			break;
		case IM_KEY_UP:
			joy_device_mouse_button_up(mouse, JOY_BUTTON_LEFT);
			joy_bubble_button_up(window, mouse, timestamp, x, y,
					JOY_BUTTON_LEFT);
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
			joy_device_mouse_button_down(mouse, JOY_BUTTON_RIGHT);
			joy_bubble_button_down(window, mouse, timestamp, x, y,
					JOY_BUTTON_RIGHT);
			break;
		case IM_KEY_UP:
			joy_device_mouse_button_up(mouse, JOY_BUTTON_RIGHT);
			joy_bubble_button_up(window, mouse, timestamp, x, y,
					JOY_BUTTON_RIGHT);
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
			joy_bubble_scroll(window, mouse, timestamp, x, y,
					JOY_SCROLL_DOWN);
			break;
		case 1:
			joy_bubble_scroll(window, mouse, timestamp, x, y,
					JOY_SCROLL_UP);
			break;
		default:
			g_message(Q_("gfx3d: unknown scroll direction: %d"),
					event->data[0]);
			break;
		}
		break;
	default: // key/button press
		{
			JoyKeySym sym = get_key(event, keyboard);
			switch (event->state) {
			case IM_KEY_DISCRETE:
				joy_bubble_key_down(window, keyboard,
						timestamp, x, y, sym);
				joy_bubble_key_up(window, keyboard,
						timestamp, x, y, sym);
				break;
			case IM_KEY_DOWN:
				joy_bubble_key_down(window, keyboard,
						timestamp, x, y, sym);
				break;
			case IM_KEY_UP:
				joy_bubble_key_up(window, keyboard,
						timestamp, x, y, sym);
				break;
			default:
				g_message(Q_("gfx3d: unknown key state: %d"),
						event->state);
				break;
			}
			break;
		}
	}
}

static void
input(JoySource *self)
{
	struct Private *priv = GET_PRIVATE(self);
	int fd = joy_source_get_descriptor(self);
	IM_INPUT event;
	ssize_t bytes = read(fd, &event, sizeof(IM_INPUT));
	if (-1 == bytes || sizeof(IM_INPUT) != bytes) {
		return;
	}
	unsigned long timestamp = g_timer_elapsed(priv->timer, NULL) * 1000;
	JoyScreen *screen = joy_application_get_screen(priv->app,
			event.tv == IM_TV1 ? 0 : 1);
	if (G_UNLIKELY(!screen)) {
		return;
	}
	gint x, y;
	JoyBubble *window;
	switch (event.key) {
	case INPUT_KEY_RELATIVE_POINT: // relative mouse motion
		joy_gfx3d_screen_move_cursor(screen,
				(short)(event.data[0] | event.data[1] << 8),
				-(short)(event.data[2] | event.data[3] << 8));
		joy_gfx3d_screen_get_cursor_position(screen, &x, &y);
		window = joy_screen_window_at(screen, x, y);
		if (window) {
			JoyDevice *mouse = joy_gfx3d_screen_get_mouse(screen);
			joy_bubble_motion(window, mouse, timestamp, x, y);
		}
		break;
	case INPUT_KEY_ABSOLUTE_POINT: // absolute mouse motion
		joy_gfx3d_screen_warp_cursor(screen,
				(short)(event.data[0] | event.data[1] << 8),
				(short)(event.data[2] | event.data[3] << 8));
		joy_gfx3d_screen_get_cursor_position(screen, &x, &y);
		window = joy_screen_window_at(screen, x, y);
		if (window) {
			JoyDevice *mouse = joy_gfx3d_screen_get_mouse(screen);
			joy_bubble_motion(window, mouse, timestamp, x, y);
		}
		break;
	default:
		non_motion_event(self, screen, &event, timestamp);
		break;
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
joy_gfx3d_source_new(JoyApplication *app)
{
	g_return_val_if_fail(JOY_IS_GFX3D_APPLICATION(app), NULL);
	int fd = open(INPUT_MANAGER_FIFO_NAME, O_RDONLY);
	if (G_UNLIKELY(-1 == fd)) {
		return NULL;
	}
	return g_object_new(JOY_TYPE_GFX3D_SOURCE,
			"descriptor", fd,
			"application", app,
			NULL);
}
