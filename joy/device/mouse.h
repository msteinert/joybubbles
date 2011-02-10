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
	JOY_BUTTON_NONE = 0,
	JOY_BUTTON_LEFT,
	JOY_BUTTON_MIDDLE,
	JOY_BUTTON_RIGHT,
	JOY_BUTTON_X1,
	JOY_BUTTON_X2,
	JOY_BUTTON_MAX
} JoyButton;

/**
 * \brief Indicates the direction of a scroll-wheel motion.
 */
typedef enum {
	JOY_SCROLL_NONE = 0,
	JOY_SCROLL_UP,
	JOY_SCROLL_DOWN,
	JOY_SCROLL_LEFT,
	JOY_SCROLL_RIGHT
} JoyScroll;


typedef struct JoyDeviceMouseClass_ JoyDeviceMouseClass;

struct JoyDeviceMouse_ {
	/*< private >*/
	JoyDevice parent_instance;
};

typedef void
(*JoyDeviceMouseSetCursor)(JoyDevice *self,
		JoyCursor *cursor);

typedef void
(*JoyDeviceMouseWarp)(JoyDevice *self, JoyScreen *screen,
		gint x, gint y);

typedef void
(*JoyDeviceMouseGetLocation)(JoyDevice *self,
		JoyScreen **screen, gint *x, gint *y);

struct JoyDeviceMouseClass_ {
	/*< private >*/
	JoyDeviceClass parent_class;
	/*< public >*/
	JoyDeviceMouseSetCursor set_cursor;
	JoyDeviceMouseWarp warp;
	JoyDeviceMouseGetLocation get_location;
};

G_GNUC_NO_INSTRUMENT
GType
joy_device_mouse_get_type(void) G_GNUC_CONST;

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
 * \param screen [in] The screen the mouse cursor should appear on.
 * \param x [in] The new X-axis coordinate (in pixels).
 * \param y [in] The new Y-axis coordinate (in pixels).
 */
void
joy_device_mouse_warp(JoyDevice *self, JoyScreen *screen, gint x, gint y);

/**
 * \brief Get the absolute location of the mouse cursor.
 *
 * \param self [in] A mouse device object.
 * \param screen [out] The screen the mouse cursor is on (may be \e NULL).
 * \param x [out] The X-axis coordinate (in pixels, may be \e NULL).
 * \param y [out] The Y-axis coordinate (in pixels, may be \e NULL).
 */
void
joy_device_mouse_get_location(JoyDevice *self, JoyScreen **screen,
		gint *x, gint *y);

G_END_DECLS

#endif // JOY_DEVICE_MOUSE_H
