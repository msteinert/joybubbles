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
 * \brief Draw back-buffers
 */

#ifndef JOY_BUFFER_H
#define JOY_BUFFER_H

#include <cairo.h>
#include <glib.h>
#include <joy/types.h>

G_BEGIN_DECLS

/**
 * \brief Create a new back-buffer.
 *
 * \return A new buffer object.
 */
G_GNUC_INTERNAL
G_GNUC_WARN_UNUSED_RESULT
JoyBuffer *
joy_buffer_new(void);

/**
 * \brief Destroy a back-buffer.
 *
 * \param self [in] A back-buffer object.
 */
G_GNUC_INTERNAL
void
joy_buffer_destroy(JoyBuffer *self);

/**
 * \brief Mark a damaged region of a back-buffer.
 *
 * Regions marked as damaged will be redrawn then next time joy_buffer_draw()
 * is called.
 *
 * \param self [in] A back-buffer object.
 * \param damage [in] The region to mark as damaged.
 */
G_GNUC_INTERNAL
void
joy_buffer_damage(JoyBuffer *self, const cairo_region_t *damage);

/**
 * \brief Set the alpha value for the entire buffer.
 *
 * If the alpha value is set to some value between 0.0 and 1.0 then the
 * entire buffer will be painted with that alpha value.
 *
 * \param self [in] A back-buffer object.
 * \param alpha [in] The new alpha value.
 */
G_GNUC_INTERNAL
void
joy_buffer_set_alpha(JoyBuffer *self, gdouble alpha);

/**
 * \brief Get the alpha value for the entire buffer.
 *
 * \param self [in] A back buffer object.
 *
 * \return The alpha value for \e self.
 */
G_GNUC_INTERNAL
gdouble
joy_buffer_get_alpha(JoyBuffer *self);

/**
 * \brief Draw using a buffer.
 *
 * The "draw" signal will only be emitted if the back-buffer is damaged or
 * \e widget been resized since the last draw.
 *
 * \param self [in] A back buffer object.
 * \param widget [in] The widget to draw.
 * \param id [in] The "draw" signal ID for \e widget.
 * \param cr [in] The cairo handle to draw to.
 * \param status [out] Storage for the return value.
 */
G_GNUC_INTERNAL
void
joy_buffer_draw(JoyBuffer *self, JoyBubble *widget, guint id, cairo_t *cr,
		gboolean *status);

G_END_DECLS

#endif // JOY_BUFFER_H
