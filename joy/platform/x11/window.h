/* Copyright 2011, 2012 Michael Steinert
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *
 *     2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *
 *     3. Neither the name of the Joybubbles project nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file
 * \brief X11 window implementation
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
