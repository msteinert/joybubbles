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
#include "joy/easing/elastic.h"
#include <math.h>

#define AMPLITUDE .3
#define PERIOD 1.

#ifdef JOY_ENABLE_EASING
static gdouble
in_elastic(struct JoyEasingElastic *self, gdouble time,
		gdouble b, gdouble c, gdouble d)
{
	gdouble amplitude = AMPLITUDE;
	gdouble period = PERIOD;
	if (self) {
		amplitude = self->amplitude;
		period = self->period;
	}
	if (0. == time) {
		return b;
	}
	gdouble t = time / d;
	if (1. == t) {
		return b + c;
	}
	gdouble s;
	if (amplitude < fabs(c)) {
		amplitude = c;
		s = period / 4.;
	} else {
		s = period / (2. * G_PI) * asin(c / amplitude);
	}
	t -= 1.;
	return -(amplitude * pow(2., 10. * t)
			* sin((t * d - s) * (2. * G_PI) / period)) + b;
}
G_GNUC_CONST
#endif

#ifdef JOY_ENABLE_EASING
static gdouble
out_elastic(struct JoyEasingElastic *self, gdouble time,
		gdouble b, gdouble c, gdouble d)
{
	gdouble amplitude = AMPLITUDE;
	gdouble period = PERIOD;
	if (self) {
		amplitude = self->amplitude;
		period = self->period;
	}
	if (0. == time) {
		return 0.;
	}
	if (1. == time) {
		return c;
	}
	gdouble s;
	if (amplitude < c) {
		amplitude = c;
		s = period / 4.;
	} else {
		s = period / (2. * G_PI) * asin(c / amplitude);
	}
	return (amplitude * pow(2., -10. * time)
			* sin((time - s) * (2. * G_PI) / period) + c);
}
G_GNUC_CONST
#endif

gdouble
joy_easing_in_elastic(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return in_elastic(self, time, 0., 1., 1.);
#else
	return time;
#endif
}

gdouble
joy_easing_out_elastic(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return out_elastic(self, time, 0., 1., 1.);
#else
	return time;
#endif
}

gdouble
joy_easing_in_out_elastic(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	gdouble amplitude = AMPLITUDE;
	gdouble period = PERIOD;
	if (self) {
		amplitude = ((struct JoyEasingElastic *)self)->amplitude;
		period = ((struct JoyEasingElastic *)self)->period;
	}
	if (0. == time) {
		return 0.;
	}
	time *= 2.;
	if (2. == time) {
		return 1.0;
	}
	gdouble s;
	if (amplitude < 1.) {
		amplitude = 1.;
		s = period / 4.;
	} else {
		s = period / (2. * G_PI) * asin(1. / amplitude);
	}
	if (time < 1) {
		return -.5 * (amplitude * pow(2., 10. * (time - 1.))
				* sin((time - 1. - s) * (2. * G_PI) / period));
	} else {
		return amplitude * pow(2., -10. * (time - 1.))
			* sin((time - 1. - s) * (2. * G_PI) / period)
			* .5 + 1.;
	}
#else
	return time;
#endif
}

gdouble
joy_easing_out_in_elastic(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (.5 > time) {
		return out_elastic(self, 2. * time, 0., .5, 1.);
	} else {
		return in_elastic(self, 2. * time - 1., .5, .5, 1.);
	}
#else
	return time;
#endif
}
