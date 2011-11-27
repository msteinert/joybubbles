/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/error.h"

GQuark
joy_error_quark(void)
{
	return g_quark_from_static_string("joy-error-quark");
}
