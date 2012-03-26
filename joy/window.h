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
 * \brief Top level window abstraction
 */

#ifndef JOY_WINDOW_H
#define JOY_WINDOW_H

#include <cairo.h>
#include <joy/container.h>
#include <joy/device/mouse.h>
#include <joy/device/keyboard.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_WINDOW \
	(joy_window_get_type())

#define JOY_WINDOW(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_WINDOW, JoyWindow))

#define JOY_IS_WINDOW(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_WINDOW))

#define JOY_WINDOW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_WINDOW, JoyWindowClass))

#define JOY_IS_WINDOW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_WINDOW))

#define JOY_WINDOW_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_WINDOW, \
		JoyWindowClass))

typedef struct JoyWindowClass_ JoyWindowClass;

struct JoyWindow_ {
	/*< private >*/
	JoyContainer parent_instance;
	gpointer priv;
};

/* virtual functions */
typedef cairo_surface_t *
(*JoyWindowCairoSurfaceCreate)(JoyBubble *self);

typedef void
(*JoyWindowRaise)(JoyBubble *self);

typedef void
(*JoyWindowLower)(JoyBubble *self);

struct JoyWindowClass_ {
	/*< private >*/
	JoyContainerClass parent_class;
	/*< public >*/
	JoyWindowCairoSurfaceCreate cairo_surface_create;
	JoyWindowRaise raise;
	JoyWindowLower lower;
};

G_GNUC_NO_INSTRUMENT
GType
joy_window_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new window.
 *
 * \param screen [in] The screen upon which to create the new window.
 *
 * \return A new window object.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyBubble *
joy_window_new(JoyScreen *screen);

/**
 * \brief Get a cairo surface for a window object.
 *
 * \param self [in] A window object.
 *
 * \return A cairo surface for \e self. The caller owns the returned value
 *         and must call cairo_surface_destroy() to free memory.
 */
G_GNUC_WARN_UNUSED_RESULT
cairo_surface_t *
joy_window_cairo_surface_create(JoyBubble *self);

/**
 * \brief Raise a window to the top of the stacking order.
 *
 * \param self [in] A window object.
 */
void
joy_window_raise(JoyBubble *self);

/**
 * \brief Lower a window to the bottom of the stacking order.
 *
 * \param self [in] A window object.
 */
void
joy_window_lower(JoyBubble *self);

G_END_DECLS

#endif // JOY_WINDOW_H
