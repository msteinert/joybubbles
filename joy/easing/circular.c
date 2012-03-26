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
#include "joy/easing/circular.h"
#include <math.h>

gdouble
joy_easing_in_circ(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return -(sqrt(1. - time * time) - 1.);
#else
	return time;
#endif
}

gdouble
joy_easing_out_circ(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	time -= 1.;
	return sqrt(1. - time * time);
#else
	return time;
#endif
}

gdouble
joy_easing_in_out_circ(G_GNUC_UNUSED gpointer self, gdouble time)
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
