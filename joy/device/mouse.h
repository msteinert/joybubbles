/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Mouse device abstraction
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_DEVICE_MOUSE_H
#define JOY_DEVICE_MOUSE_H

#include <joy/device.h>
#include <joy/device/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_DEVICE_MOUSE \
	(joy_device_mouse_get_type())

#define JOY_DEVICE_MOUSE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_DEVICE_MOUSE, \
		JoyDeviceMouse))

#define JOY_IS_DEVICE_MOUSE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_DEVICE_MOUSE))

#define JOY_DEVICE_MOUSE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_DEVICE_MOUSE, \
		JoyDeviceMouseClass))

#define JOY_IS_DEVICE_MOUSE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_DEVICE_MOUSE))

#define JOY_DEVICE_MOUSE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_DEVICE_MOUSE, \
		JoyDeviceMouseClass))

/**
 * \brief Mouse button IDs
 */
typedef enum {
	JOY_MOUSE_BUTTON_NONE = 0, /**< Sentinel value */
	JOY_MOUSE_BUTTON_LEFT = 1 << 1, /**< Left mouse button */
	JOY_MOUSE_BUTTON_MIDDLE = 1 << 2, /**< Middle mouse button */
	JOY_MOUSE_BUTTON_RIGHT = 1 << 3, /**< Right mouse button */
	JOY_MOUSE_BUTTON_X1 = 1 << 4, /**< First extra mouse button */
	JOY_MOUSE_BUTTON_X2 = 1 << 5, /**< Second extra mouse button */
	JOY_MOUSE_BUTTON_MAX = G_MAXINT /**< Sentinel value */
} JoyMouseButton;

/**
 * \brief Indicates the direction of a scroll-wheel motion.
 */
typedef enum {
	JOY_MOUSE_SCROLL_NONE = 0, /**< Sentinel value */
	JOY_MOUSE_SCROLL_UP, /**< Scroll-wheel up */
	JOY_MOUSE_SCROLL_DOWN, /**< Scroll-wheel down */
	JOY_MOUSE_SCROLL_LEFT, /**< Scroll-wheel left */
	JOY_MOUSE_SCROLL_RIGHT, /*< Scroll-wheel right */
	JOY_MOUSE_SCROLL_MAX /**< Sentinel value */
} JoyMouseScroll;

typedef struct JoyDeviceMouseClass_ JoyDeviceMouseClass;

struct JoyDeviceMouse_ {
	/*< private >*/
	JoyDevice parent_instance;
	gpointer priv;
};

typedef void
(*JoyDeviceMouseSetCursor)(JoyDevice *self, JoyCursor *cursor);

typedef void
(*JoyDeviceMouseWarp)(JoyDevice *self, gint x, gint y);

typedef void
(*JoyDeviceMouseGetLocation)(JoyDevice *self, gint *x, gint *y);

typedef void
(*JoyDeviceMouseShow)(JoyDevice *self);

typedef void
(*JoyDeviceMouseHide)(JoyDevice *self);

typedef gboolean
(*JoyDeviceMouseVisible)(JoyDevice *self);

struct JoyDeviceMouseClass_ {
	/*< private >*/
	JoyDeviceClass parent_class;
	/*< public >*/
	JoyDeviceMouseSetCursor set_cursor;
	JoyDeviceMouseWarp warp;
	JoyDeviceMouseGetLocation get_location;
	JoyDeviceMouseShow show;
	JoyDeviceMouseHide hide;
	JoyDeviceMouseVisible visible;
};

G_GNUC_NO_INSTRUMENT
GType
joy_device_mouse_get_type(void) G_GNUC_CONST;

/**
 * \brief Set the state of a mouse button.
 *
 * \param self [in] A mouse device object.
 * \param sym [in] A mouse button.
 */
void
joy_device_mouse_button_down(JoyDevice *self, JoyMouseButton button);

/**
 * \brief Unset the state of a mouse button.
 *
 * \param self [in] A mouse device object.
 * \param sym [in] A mouse button
 */
void
joy_device_mouse_button_up(JoyDevice *self, JoyMouseButton button);

/**
 * \brief Retrieve the state of the buttons for a mouse.
 *
 * \param self [in] A mouse device object.
 *
 * \param The button state for \e self.
 */
glong
joy_device_mouse_button_state(JoyDevice *self);

/**
 * \brief Set the keyboard device associated with this mouse.
 *
 * \param self [in] A mouse device object.
 * \param keyboard [in] A keyboard device object.
 */
void
joy_device_mouse_set_keyboard(JoyDevice *self, JoyDevice *keyboard);

/**
 * \brief Get the keyboard device associated with this mouse.
 *
 * \param self [in] A mouse device object.
 *
 * \return The keyboard device associated with \e self.
 */
JoyDevice *
joy_device_mouse_get_keyboard(JoyDevice *self);

/**
 * \brief Set the cursor image.
 *
 * \param self [in] A mouse device object.
 * \param cursor [in] The new cursor to set.
 */
void
joy_device_mouse_set_cursor(JoyDevice *self, JoyCursor *cursor);

/**
 * \brief Move the mouse cursor to a new location.
 *
 * This function should generate mouse motion events if the pointer was
 * successfully moved to a new location.
 *
 * \param self [in] A mouse device object.
 * \param x [in] The new X-axis coordinate (relative to the screen).
 * \param y [in] The new Y-axis coordinate (relative to the screen).
 */
void
joy_device_mouse_warp(JoyDevice *self, gint x, gint y);

/**
 * \brief Get the absolute location of the mouse cursor.
 *
 * \param self [in] A mouse device object.
 * \param x [out] The X-axis coordinate (relative to the screen, may be NULL).
 * \param y [out] The Y-axis coordinate (relative to the screen, may be NULL).
 */
void
joy_device_mouse_get_location(JoyDevice *self, gint *x, gint *y);

/**
 * \brief Show the mouse cursor.
 *
 * \param self [in] A mouse device object.
 */
void
joy_device_mouse_show(JoyDevice *self);

/**
 * \brief Hide the mouse cursor.
 *
 * \param self [in] A mouse device object.
 */
void
joy_device_mouse_hide(JoyDevice *self);

/**
 * \brief Determine if the mouse cursor is visible.
 *
 * \param self [in] A mouse device object.
 *
 * \return TRUE if the cursor is visible, FALSE otherwise.
 */
gboolean
joy_device_mouse_visible(JoyDevice *self);

G_END_DECLS

#endif // JOY_DEVICE_MOUSE_H
