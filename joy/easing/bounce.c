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
