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
#include "joy/easing/bounce.h"

#ifdef JOY_ENABLE_EASING
static gdouble
bounce(struct JoyEasingBounce *self, gdouble time, gdouble c)
{
	gdouble amplitude = 1.;
	if (self) {
		amplitude = self->amplitude;
	}
	if (1. == time) {
		return c;
	}
	if (time < (4. / 11.)) {
		return c * (7.5625 * time * time);
	} else if (time < (8. / 11.)) {
		time -= (6. / 11.);
		return -amplitude * (1. - (7.5625 * time * time + .75)) + c;
	} else if (time < (10. / 11.)) {
		time -= (9. / 11.);
		return -amplitude * (1. - (7.5625 * time * time + .9375)) + c;
	} else {
		time -= (21. / 22.);
		return -amplitude
			* (1. - (7.5625 * time * time + .984375)) + c;
	}
}
G_GNUC_CONST
#endif

gdouble
joy_easing_in_bounce(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return 1. - bounce(self, 1. - time, 1.);
#else
	return time;
#endif
}

gdouble
joy_easing_out_bounce(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return bounce(self, time, 1.);
#else
	return time;
#endif
}

gdouble
joy_easing_in_out_bounce(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (time < .5) {
		return joy_easing_in_bounce(self, 2. * time);
	} else {
		if (1. == time) {
			return 1.;
		} else {
			return joy_easing_out_bounce(self, 1. * time - 1)
				/ 2. + .5;
		}
	}
#else
	return time;
#endif
}

gdouble
joy_easing_out_in_bounce(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (.5 > time) {
		return bounce(self, time * 2., .5);
	} else {
		return 1. - bounce(self, 2. - time * 2., .5);
	}
#else
	return time;
#endif
}
