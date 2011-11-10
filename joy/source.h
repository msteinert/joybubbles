/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Pollable input sources
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_SOURCE_H
#define JOY_SOURCE_H

#include <glib-object.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_SOURCE \
	(joy_source_get_type())

#define JOY_SOURCE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_SOURCE, \
		JoySource))

#define JOY_IS_SOURCE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_SOURCE))

#define JOY_SOURCE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_SOURCE, \
		JoySourceClass))

#define JOY_IS_SOURCE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_SOURCE))

#define JOY_SOURCE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_SOURCE, \
		JoySourceClass))

typedef struct JoySourceClass_ JoySourceClass;

struct JoySource_ {
	/*< private >*/
	GInitiallyUnowned parent_instance;
	gpointer priv;
};

typedef gboolean
(*JoySourcePrepare)(JoySource *self);

typedef void
(*JoySourceDispatch)(JoySource *self, gushort revents);

typedef void
(*JoySourceInput)(JoySource *self);

typedef void
(*JoySourceOutput)(JoySource *self);

typedef void
(*JoySourceHangup)(JoySource *self);

typedef void
(*JoySourceError)(JoySource *self);

struct JoySourceClass_ {
	/*< private >*/
	GInitiallyUnownedClass parent_class;
	/*< public >*/
	JoySourcePrepare prepare;
	JoySourceDispatch dispatch;
	JoySourceInput input;
	JoySourceOutput output;
	JoySourceHangup hangup;
	JoySourceError error;
};

G_GNUC_NO_INSTRUMENT
GType
joy_source_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new source object.
 *
 * \param descriptor [in] A pollable file descriptor.
 *
 * \return A new source object.
 */
G_GNUC_WARN_UNUSED_RESULT
JoySource *
joy_source_new(gint descriptor);

/**
 * \brief This function is called before polling the file descriptor.
 *
 * If the source can determine that event is waiting without calling poll(2)
 * or select(2) then it should return TRUE.
 *
 * \param self [in] A source object.
 *
 * \return TRUE if an event is waiting, FALSE otherwise.
 */
gboolean
joy_source_prepare(JoySource *self);

/**
 * \brief Set the file descriptor for this source.
 *
 * \param self [in] A source object.
 * \param descriptor [in] A pollable file descriptor.
 *
 * \return The file descriptor for this source.
 */
void
joy_source_set_descriptor(JoySource *self, gint descriptor);

/**
 * \brief Get the file descriptor for this source.
 *
 * \param self [in] A source object.
 *
 * \return The file descriptor for this source.
 */
gint
joy_source_get_descriptor(JoySource *self);

/**
 * \brief Set one or more condition flags for a source.
 *
 * \param self [in] A source object.
 * \param flags [in] Flags to set for \e self.
 */
void
joy_source_set_condition(JoySource *self, GIOCondition flags);

/**
 * \brief Clear one or more condition flags for a source.
 *
 * \param self [in] A source object.
 * \param flags [in] Flags to clear for \e self.
 */
void
joy_source_clear_condition(JoySource *self, GIOCondition flags);

/**
 * \brief Get condition flags set for a source.
 *
 * \param self [in] A source object.
 *
 * \return The condition flags set for \e self.
 */
GIOCondition
joy_source_get_condition(JoySource *self);

/**
 * \brief Dispatch a poll event for a source.
 *
 * \param self [in] A source object.
 * \param revents [in] Events returned from g_poll().
 */
void
joy_source_dispatch(JoySource *self, gushort revents);

/**
 * \brief Dispatch an input event (G_IO_IN) for a source.
 *
 * \param self [in] A source object.
 */
void
joy_source_input(JoySource *self);

/**
 * \brief Dispatch an output event (G_IO_OUT) for a source.
 *
 * \param self [in] A source object.
 */
void
joy_source_output(JoySource *self);

/**
 * \brief Dispatch a hangup event (G_IO_HUP) for a source.
 *
 * \param self [in] A source object.
 */
void
joy_source_hangup(JoySource *self);

/**
 * \brief Dispatch an error event (G_IO_ERR) for a source.
 *
 * \param self [in] A source object.
 */
void
joy_source_error(JoySource *self);

G_END_DECLS

#endif // JOY_SOURCE_H

