/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Visual filtering
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_FILTER_H
#define JOY_FILTER_H

#include <cairo.h>
#include <glib-object.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_FILTER \
	(joy_filter_get_type())

#define JOY_FILTER(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_FILTER, \
		JoyFilter))

#define JOY_IS_FILTER(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_FILTER))

#define JOY_FILTER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_FILTER, \
		JoyFilterClass))

#define JOY_IS_FILTER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_FILTER))

#define JOY_FILTER_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_FILTER, \
		JoyFilterClass))

typedef struct JoyFilterClass_ JoyFilterClass;

struct JoyFilter_ {
	/*< private >*/
	GObject parent_instance;
};

typedef void
(*JoyFilterApply)(JoyFilter *self, JoyBubble *widget, cairo_t *cr);

struct JoyFilterClass_ {
	/*< private >*/
	GObjectClass parent_class;
	/*< public >*/
	JoyFilterApply apply;
};

G_GNUC_NO_INSTRUMENT
GType
joy_filter_get_type(void) G_GNUC_CONST;

/**
 * \brief Apply a filter to a widget.
 *
 * \note Filters must applied after the widget has drawn itself.
 *
 * \param self [in] A filter object.
 * \param widget [in] The widget \e self is being applied to.
 * \param cr [in] The cairo context to apply the filter to.
 */
void
joy_filter_apply(JoyFilter *self, JoyBubble *widget, cairo_t *cr);

G_END_DECLS

#endif // JOY_FILTER_H
