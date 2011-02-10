/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Cursor image
 * \author Mike Steinert <michael.steinert@echostar.com>
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
