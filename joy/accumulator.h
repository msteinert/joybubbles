/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \brief Common accumulators for signal handlers
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_ACCUMULATOR_H
#define JOY_ACCUMULATOR_H

#include <glib-object.h>

G_BEGIN_DECLS

/**
 * \brief Accumulate a boolean return value, stop if TRUE is returned.
 */
G_GNUC_INTERNAL
gboolean
joy_boolean_accumulator(GSignalInvocationHint *hint, GValue *accu,
		const GValue *value, gpointer data);

G_END_DECLS

#endif // JOY_ACCUMULATOR_H

