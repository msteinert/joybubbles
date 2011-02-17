/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \brief Draw back-buffers
 * \author Mike Steinert <michael.steinert@echostar.com>
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
