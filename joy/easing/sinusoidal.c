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
#include "joy/easing.h"
#include <math.h>

gdouble
joy_easing_in_sine(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (1. == time) {
		return 1.;
	} else {
		return -cos(time * M_PI_2) + 1.;
	}
#else
	return time;
#endif
}

gdouble
joy_easing_out_sine(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return sin(time * M_PI_2);
#else
	return time;
#endif
}

gdouble
joy_easing_in_out_sine(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return -.5 * (cos(M_PI * time) - 1.);
#else
	return time;
#endif
}

gdouble
joy_easing_out_in_sine(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (.5 > time) {
		return joy_easing_out_sine(self, time * 2.) / 2.;
	} else {
		return joy_easing_in_sine(self, time * 2. - 1.) / 2. + .5;
	}
#else
	return time;
#endif
}
