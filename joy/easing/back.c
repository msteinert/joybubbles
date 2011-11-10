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

#define OVERSHOOT 1.70158

gdouble
joy_easing_in_back(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	gdouble overshoot = OVERSHOOT;
	if (self) {
		overshoot = ((struct JoyEasingBack *)self)->overshoot;
	}
	return time * time * ((overshoot + 1.) * time - overshoot);
#else
	return time;
#endif
}

gdouble
joy_easing_out_back(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	gdouble overshoot = OVERSHOOT;
	if (self) {
		overshoot = ((struct JoyEasingBack *)self)->overshoot;
	}
	time -= 1.;
	return time * time * ((overshoot + 1.) * time + overshoot) + 1.;
#else
	return time;
#endif
}

gdouble
joy_easing_in_out_back(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	gdouble overshoot = OVERSHOOT;
	if (self) {
		overshoot = ((struct JoyEasingBack *)self)->overshoot;
	}
	time *= 2.;
	if (time < 1.) {
		overshoot *= 1.525;
		return .5 * (time * time * ((overshoot + 1.)
					* time - overshoot));
	} else {
		time -= 2.;
		overshoot *= 1.525;
		return .5 * (time * time * ((overshoot + 1.)
					* time + overshoot) + 2.);
	}
#else
	return time;
#endif
}

gdouble
joy_easing_out_in_back(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (.5 > time) {
		return joy_easing_out_back(self, time * 2.) / 2.;
	} else {
		return joy_easing_in_back(self, time * 2. - 1.) / 2. + .5;
	}
#else
	return time;
#endif
}

