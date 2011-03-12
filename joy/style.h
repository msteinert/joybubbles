/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Widget styles
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_STYLE_H
#define JOY_STYLE_H

#include <cairo.h>
#include <glib-object.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_STYLE \
	(joy_style_get_type())

#define JOY_STYLE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_STYLE, JoyStyle))

#define JOY_IS_STYLE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_STYLE))

#define JOY_STYLE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_STYLE, JoyStyleClass))

#define JOY_IS_STYLE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_STYLE))

#define JOY_STYLE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_STYLE, \
		JoyStyleClass))

typedef struct JoyStyleClass_ JoyStyleClass;

struct JoyStyle_ {
	/*< private >*/
	GObject parent_instance;
};

typedef gboolean
(*JoyStyleDraw)(JoyStyle *self, JoyBubble *widget, cairo_t *cr);

struct JoyStyleClass_ {
	/*< private >*/
	GObjectClass parent_class;
	/*< public >*/
	JoyStyleDraw draw;
};

G_GNUC_NO_INSTRUMENT
GType
joy_style_get_type(void) G_GNUC_CONST;

/**
 * \brief Draw a widget using the specified style.
 *
 * \param self [in] A style object.
 * \param widget [in] The widget to draw.
 * \param cr [in] The cairo context to draw to.
 *
 * \return TRUE if \e widget was drawn, FALSE otherwise.
 */
gboolean
joy_style_draw(JoyStyle *self, JoyBubble *widget, cairo_t *cr);

G_END_DECLS

#endif // JOY_STYLE_H
