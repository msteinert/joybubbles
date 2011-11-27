/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief X11 window implementation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_X11_WINDOW_H
#define JOY_X11_WINDOW_H

#include "joy/window.h"
#include "joy/platform/x11/types.h"
#include <X11/Xlib.h>

G_BEGIN_DECLS

#define JOY_TYPE_X11_WINDOW \
	(joy_x11_window_get_type())

#define JOY_X11_WINDOW(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_X11_WINDOW, \
		JoyX11Window))

#define JOY_IS_X11_WINDOW(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_X11_WINDOW))

#define JOY_X11_WINDOW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_X11_WINDOW, \
		JoyX11WindowClass))

#define JOY_IS_X11_WINDOW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_X11_WINDOW))

#define JOY_X11_WINDOW_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_X11_WINDOW, \
		JoyX11WindowClass))

/** \brief The maximum width & height (in pixels) of an X11 window */
#define JOY_X11_WINDOW_MAX (65535)

typedef struct JoyX11WindowClass_ JoyX11WindowClass;

struct JoyX11Window_ {
	/*< private >*/
	JoyWindow parent_instance;
	gpointer priv;
};

struct JoyX11WindowClass_ {
	/*< private >*/
	JoyWindowClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_x11_window_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new window on the given screen.
 *
 * \param screen [in] A screen object.
 *
 * \return A new window object.
 */
G_GNUC_INTERNAL
JoyBubble *
joy_x11_window_new(JoyScreen *screen);

/**
 * \brief Get the X11 window.
 *
 * \param self [in] An X11 window object.
 *
 * \return The window XID for \e self.
 */
G_GNUC_INTERNAL
Window
joy_x11_window_get_window(JoyBubble *self);

/**
 * \brief Get the X11 screen.
 *
 * \param self [in] An X11 window object.
 *
 * \return The X11 Screen for \e self.
 */
G_GNUC_INTERNAL
Screen *
joy_x11_window_get_screen(JoyBubble *self);

/**
 * \brief Get the X11 display.
 *
 * \param self [in] An X11 window object.
 *
 * \return The X11 Display for \e self.
 */
G_GNUC_INTERNAL
Display *
joy_x11_window_get_display(JoyBubble *self);

/**
 * \brief Handle a "destroy notify" event.
 *
 * The application should never receive a "destroy notify" event, however if
 * it does this function will set the window XID to OUL and destroy the
 * cached cairo surface.
 *
 * \param self [in] An X11 window object.
 */
G_GNUC_INTERNAL
void
joy_x11_window_destroy(JoyBubble *self);

/**
 * \brief Set the current X11 event.
 *
 * The window will use this information to determine if the next event was
 * propagated by the server or the user.
 *
 * \param self [in] An X11 window object.
 * \param event [in] An X11 event structure.
 */
G_GNUC_INTERNAL
void
joy_x11_window_set_event(JoyBubble *self, XEvent *event);

/**
 * \brief Submit a window for updates.
 *
 * \param self [in] An X11 window object.
 * \param display [in] The X11 display.
 */
G_GNUC_INTERNAL
void
joy_x11_window_submit(JoyBubble *self, Display *display);

G_END_DECLS

#endif // JOY_X11_WINDOW_H
