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
 * \brief GFX3D mouse implementation
 */

#ifndef JOY_GFX3D_MOUSE_H
#define JOY_GFX3D_MOUSE_H

#include "joy/device/mouse.h"
#include "joy/platform/gfx3d/types.h"

G_BEGIN_DECLS

#define JOY_TYPE_GFX3D_MOUSE \
	(joy_gfx3d_mouse_get_type())

#define JOY_GFX3D_MOUSE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_GFX3D_MOUSE, \
		JoyGfx3dMouse))

#define JOY_IS_GFX3D_MOUSE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_GFX3D_MOUSE))

#define JOY_GFX3D_MOUSE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_GFX3D_MOUSE, \
		JoyGfx3dMouseClass))

#define JOY_IS_GFX3D_MOUSE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_GFX3D_MOUSE))

#define JOY_GFX3D_MOUSE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_GFX3D_MOUSE, \
		JoyGfx3dMouseClass))

typedef struct JoyGfx3dMouseClass_ JoyGfx3dMouseClass;

struct JoyGfx3dMouse_ {
	/*< private >*/
	JoyDeviceMouse parent_instance;
};

struct JoyGfx3dMouseClass_ {
	/*< private >*/
	JoyDeviceMouseClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_gfx3d_mouse_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a mouse device.
 *
 * \param screen [in] The screen that this mouse is on.
 *
 * \return A new mouse device object.
 */
G_GNUC_INTERNAL
G_GNUC_WARN_UNUSED_RESULT
JoyDevice *
joy_gfx3d_mouse_new(JoyScreen *screen);

G_END_DECLS

#endif // JOY_GFX3D_MOUSE_H
