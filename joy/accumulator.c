/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/accumulator.h"

gboolean
joy_boolean_accumulator(GSignalInvocationHint *hint, GValue *accu,
		const GValue *value, gpointer data)
{
	gboolean ready = g_value_get_boolean(value);
	g_value_set_boolean(accu, ready);
	return ready ? FALSE : TRUE;
}

