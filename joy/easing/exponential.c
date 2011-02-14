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
#include "joy/easing/exponential.h"
#include <math.h>

gdouble
joy_easing_in_exp(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (0. == time || 1. == time) {
		return time;
	} else {
		return pow(2., 10. * (time - 1.) - .001);
	}
#else
	return time;
#endif
}

gdouble
joy_easing_out_exp(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (1. == time) {
		return 1.;
	} else {
		return 1.001 * (-pow(2., -10. * time) + 1.);
	}
#else
	return time;
#endif
}

gdouble
joy_easing_in_out_exp(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (0. == time || 1. == time) {
		return time;
	}
	time *= 2.;
	if (time < 1.) {
		return .5 * pow(2., 10. * (time - 1)) - 0.0005;
	} else {
		return .5 * 1.0005 * (-pow(2., -10. * (time - 1.)) + 2.);
	}
#else
	return time;
#endif
}

gdouble
joy_easing_out_in_exp(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (.5 < time) {
		return joy_easing_out_exp(self, time * 2.) / 2.;
	} else {
		return joy_easing_in_exp(self, time * 2. - 1.) / 2. + .5;
	}
#else
	return time;
#endif
}
