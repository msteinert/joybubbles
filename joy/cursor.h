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
 * \brief Cursor image
 */

#ifndef JOY_CURSOR_H
#define JOY_CURSOR_H

#include <cairo.h>
#include <glib-object.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_CURSOR \
	(joy_cursor_get_type())

#define JOY_CURSOR(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_CURSOR, JoyCursor))

#define JOY_IS_CURSOR(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_CURSOR))

#define JOY_CURSOR_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_CURSOR, JoyCursorClass))

#define JOY_IS_CURSOR_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_CURSOR))

#define JOY_CURSOR_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_CURSOR, \
		JoyCursorClass))

typedef struct JoyCursorClass_ JoyCursorClass;

struct JoyCursor_ {
	/*< private >*/
	GObject parent_instance;
	gpointer priv;
};

struct JoyCursorClass_ {
	/*< private >*/
	GObjectClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_cursor_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new cursor object.
 *
 * \param image [in] The cursor image.
 * \param x [in] The X-axis hotspot.
 * \param y [in] The Y-axis hotspot.
 *
 * \return A new cursor object.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyCursor *
joy_cursor_new(cairo_surface_t *image, gint x, gint y);

/**
 * \brief Set the image for this cursor.
 *
 * This function may be used by platform-specific code to update the cursor
 * image surface to something compatible with the target hardware.
 *
 * \param self [in] A cursor object.
 * \param image [in] The cursor image.
 */
void
joy_cursor_set_image(JoyCursor *self, cairo_surface_t *image);

/**
 * \brief Get the image for this cursor.
 *
 * \param self [in] A cursor object.
 *
 * \return The image surface for \e self.
 */
cairo_surface_t *
joy_cursor_get_image(JoyCursor *self);

/**
 * \brief Get the X-axis hotspot for this cursor.
 *
 * \param self [in] A cursor object.
 *
 * \return The X-axis hotspot for \e self.
 */
gint
joy_cursor_get_hotspot_x(JoyCursor *self);

/**
 * \brief Get the Y-axis hotspot for this cursor.
 *
 * \param self [in] A cursor object.
 *
 * \return The Y-axis hotspot for \e self.
 */
gint
joy_cursor_get_hotspot_y(JoyCursor *self);

G_END_DECLS

#endif // JOY_CURSOR_H
