/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \brief Input sink
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_SINK_H
#define JOY_SINK_H

#include <glib-object.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_SINK \
	(joy_sink_get_type())

#define JOY_SINK(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_SINK, JoySink))

#define JOY_IS_SINK(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_SINK))

#define JOY_SINK_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_SINK, JoySinkClass))

#define JOY_IS_SINK_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_SINK))

#define JOY_SINK_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_SINK, JoySinkClass))

typedef struct JoySink_ JoySink;

typedef struct JoySinkClass_ JoySinkClass;

struct JoySink_ {
	/*< private >*/
	GObject parent_instance;
	gpointer priv;
};

struct JoySinkClass_ {
	/*< private >*/
	GObjectClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_sink_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new input sink.
 *
 * \return A new input sink object.
 */
G_GNUC_INTERNAL
G_GNUC_WARN_UNUSED_RESULT
JoySink *
joy_sink_new(void);

/**
 * \brief Add an input source to an input sink.
 *
 * \param self [in] An input sink object.
 * \param source [in] An input source object.
 */
G_GNUC_INTERNAL
void
joy_sink_add(JoySink *self, JoySource *source);

/**
 * \brief Remove an input source from an input sink.
 *
 * \param self [in] An input sink object.
 * \param source [in] An input source object.
 */
G_GNUC_INTERNAL
void
joy_sink_remove(JoySink *self, JoySource *source);

/**
 * \brief Poll an input sink for events.
 *
 * This function processes events for the specified maximum number of seconds.
 *
 * \param self [in] An input sink object.
 * \param timeout [in] The upper limit on the amount of time to block.
 */
G_GNUC_INTERNAL
void
joy_sink_poll(JoySink *self, const struct timespec *timeout);

G_END_DECLS

#endif // JOY_SINK_H
