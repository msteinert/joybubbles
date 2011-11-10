/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 *
 * The following applies to the algorithms used in this file:
 *
 * Open source under the BSD License.
 *
 * Copyright 2001 Robert Penner
 * All rights reserved.
 *
 * For more information see the file COPYING is this directory.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/easing/circular.h"
#include <math.h>

gdouble
joy_easing_in_circ(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return -(sqrt(1. - time * time) - 1.);
#else
	return time;
#endif
}

gdouble
joy_easing_out_circ(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	time -= 1.;
	return sqrt(1. - time * time);
#else
	return time;
#endif
}

gdouble
joy_easing_in_out_circ(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	time *= 2.;
	if (time < 1.) {
		return -.5 * (sqrt(1. - time * time) - 1);
	} else {
		time -= 2.;
		return .5 * (sqrt(1. - time * time) + 1);
	}
#else
	return time;
#endif
}

gdouble
joy_easing_out_in_circ(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (.5 > time) {
		return joy_easing_out_circ(self, time * 2.) / 2.;
	} else {
		return joy_easing_in_circ(self, time * 2. - 1.) / 2. + .5;
	}
#else
	return time;
#endif
}

