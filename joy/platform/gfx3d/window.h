/* Copyright 2011, 2012 Michael Steinert
 * Copyright 2011 EchoStar Corporation
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
 * \brief GFX3D window implementation
 */

#ifndef JOY_GFX3D_WINDOW_H
#define JOY_GFX3D_WINDOW_H

#include <cairo.h>
#include "joy/window.h"
#include "joy/platform/gfx3d/types.h"

G_BEGIN_DECLS

#define JOY_TYPE_GFX3D_WINDOW \
	(joy_gfx3d_window_get_type())

#define JOY_GFX3D_WINDOW(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_GFX3D_WINDOW, \
		JoyGfx3dWindow))

#define JOY_IS_GFX3D_WINDOW(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_GFX3D_WINDOW))

#define JOY_GFX3D_WINDOW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_GFX3D_WINDOW, \
		JoyGfx3dWindowClass))

#define JOY_IS_GFX3D_WINDOW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_GFX3D_WINDOW))

#define JOY_GFX3D_WINDOW_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_GFX3D_WINDOW, \
		JoyGfx3dWindowClass))

typedef struct JoyGfx3dWindowClass_ JoyGfx3dWindowClass;

struct JoyGfx3dWindow_ {
	/*< private >*/
	JoyWindow parent_instance;
	gpointer priv;
};

struct JoyGfx3dWindowClass_ {
	/*< private >*/
	JoyWindowClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_gfx3d_window_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new window on the given screen.
 *
 * \param screen [in] A screen object.
 *
 * \return A new window object.
 */
G_GNUC_INTERNAL
JoyBubble *
joy_gfx3d_window_new(JoyScreen *screen);

/**
 * \brief Copy exposed areas of a window to the frame buffer.
 *
 * \param self [in] A window object.
 * \param display [in] A GFX3D native display handle.
 * \param fb [in] A GFX3D native frame buffer (surface) handle.
 * \param area [in] The screen area.
 */
G_GNUC_INTERNAL
void
joy_gfx3d_window_submit(JoyBubble *self, GFX3D_NATIVE_Display display,
		GFX3D_NATIVE_Surface fb, cairo_region_t *area);

G_END_DECLS

#endif // JOY_GFX3D_WINDOW_H
