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
 * \brief X11 application implementation
 */

#ifndef JOY_X11_APPLICATION_H
#define JOY_X11_APPLICATION_H

#include "joy/application.h"
#include "joy/platform/x11/types.h"
#include <X11/Xlib.h>

G_BEGIN_DECLS

#define JOY_TYPE_X11_APPLICATION \
	(joy_x11_application_get_type())

#define JOY_X11_APPLICATION(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_X11_APPLICATION, \
		JoyX11Application))

#define JOY_IS_X11_APPLICATION(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_X11_APPLICATION))

#define JOY_X11_APPLICATION_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_X11_APPLICATION, \
		JoyX11ApplicationClass))

#define JOY_IS_X11_APPLICATION_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_X11_APPLICATION))

#define JOY_X11_APPLICATION_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_X11_APPLICATION, \
		JoyX11ApplicationClass))

typedef struct JoyX11ApplicationClass_ JoyX11ApplicationClass;

struct JoyX11Application_ {
	/*< private >*/
	JoyApplication parent_instance;
	gpointer priv;
};

struct JoyX11ApplicationClass_ {
	/*< private >*/
	JoyApplicationClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_x11_application_get_type(void) G_GNUC_CONST;

/**
 * \brief Get the X11 display.
 *
 * \param self [in] An X11 application object.
 *
 * \return The X11 Display for \e self.
 */
G_GNUC_INTERNAL
Display *
joy_x11_application_get_display(JoyApplication *self);

/**
 * \brief Insert an XID into the object map.
 *
 * \param self [in] An X11 application object.
 * \param id [in] The XID to map to \e data.
 * \param data [in] The object to map to \e id.
 */
G_GNUC_INTERNAL
void
joy_x11_application_insert_xid(JoyApplication *self, XID id, gpointer data);

/**
 * \brief Remove an XID from the object map.
 *
 * \param self [in] An X11 application object.
 * \param id [in] The XID mapping to remove.
 */
G_GNUC_INTERNAL
void
joy_x11_application_remove_xid(JoyApplication *self, XID id);

/**
 * \brief Lookup an XID in the object map.
 *
 * \param self [in] An X11 application object.
 * \param id [in] The XID to look up.
 *
 * \return The object mapped to \e id.
 */
G_GNUC_INTERNAL
gpointer
joy_x11_application_lookup_xid(JoyApplication *self, XID id);

/**
 * \brief Get the application mouse device.
 *
 * This implementation currently supports a single mouse.
 *
 * \param self [in] An X11 application object.
 *
 * \return The mouse device object for \e self.
 */
G_GNUC_INTERNAL
JoyDevice *
joy_x11_application_get_mouse(JoyApplication *self);

/**
 * \brief Get the application keyboard device.
 *
 * This implementation currently supports a single keyboard.
 *
 * \param self [in] An X11 application object.
 *
 * \return The keyboard device object for \e self.
 */
G_GNUC_INTERNAL
JoyDevice *
joy_x11_application_get_keyboard(JoyApplication *self);

G_END_DECLS

#endif // JOY_X11_APPLICATION_H
