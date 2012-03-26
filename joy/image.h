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
 * \brief Load & display images
 */

#ifndef JOY_IMAGE_H
#define JOY_IMAGE_H

#include <joy/bubble.h>

G_BEGIN_DECLS

#define JOY_TYPE_IMAGE \
	(joy_image_get_type())

#define JOY_IMAGE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_IMAGE, JoyImage))

#define JOY_IS_IMAGE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_IMAGE))

#define JOY_IMAGE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_IMAGE, \
		JoyImageClass))

#define JOY_IS_IMAGE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_IMAGE))

#define JOY_IMAGE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_IMAGE, \
		JoyImageClass))

typedef struct JoyImageClass_ JoyImageClass;

struct JoyImage_ {
	/*< private >*/
	JoyBubble parent_instance;
	gpointer priv;
};

struct JoyImageClass_ {
	/*< private >*/
	JoyBubbleClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_image_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new image.
 *
 * \param filename [in] The name of the image file to load (may be NULL).
 *
 * \return A new image object.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyBubble *
joy_image_new(const gchar *filename);

/**
 * \brief Set the image surface.
 *
 * \param self [in] An image object.
 * \param surface [in] The surface to use for \e self.
 * \param width [in] The width of \e surface.
 * \param height [in] The height of \e surface.
 */
void
joy_image_set_surface(JoyBubble *self, cairo_surface_t *surface,
		gint width, gint height);

/**
 * \brief Get the image surface.
 *
 * \param self [in] An image object.
 *
 * \return The image surface for \e self.
 */
cairo_surface_t *
joy_image_get_surface(JoyBubble *self);

/**
 * \brief Get the original image width.
 *
 * \param self [in] An image object.
 *
 * \param The original width of \e self.
 */
gint
joy_image_get_width(JoyBubble *self);

/**
 * \brief Get the original image height.
 *
 * \param self [in] An image object.
 *
 * \param The original height of \e self.
 */
gint
joy_image_get_height(JoyBubble *self);

G_END_DECLS

#endif // JOY_IMAGE_H
