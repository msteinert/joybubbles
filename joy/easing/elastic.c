/* Copyright 2001 Robert Penner
 * Copyright 2011, 2012 Michael Steinert
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *
 *     2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *
 *     3. Neither the name of the Joybubbles project nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
out_elastic(struct JoyEasingElastic *self,
	    gdouble time,
	    G_GNUC_UNUSED gdouble b,
	    gdouble c,
	    G_GNUC_UNUSED gdouble d)
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
