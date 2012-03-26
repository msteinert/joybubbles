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
 * \brief Input sink
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
