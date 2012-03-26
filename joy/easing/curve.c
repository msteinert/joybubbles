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
#include "joy/easing/curve.h"
#include <math.h>

#ifdef JOY_ENABLE_EASING
static gdouble
sin_progress(gdouble time)
{
	return sin((time * G_PI) - G_PI_2) / 2 + .5;
}
G_GNUC_CONST
#endif

#ifdef JOY_ENABLE_EASING
static gdouble
mix_factor(gdouble time)
{
	return MIN(MAX(1. - time * 2. + .3, 0.), 1.);
}
G_GNUC_CONST
#endif

gdouble
joy_easing_in_curve(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	const gdouble progress = sin_progress(time);
	const gdouble mix = mix_factor(time);
	return progress * mix + time * (1. - mix);
#else
	return time;
#endif
}

gdouble
joy_easing_out_curve(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	const gdouble progress = sin_progress(time);
	const gdouble mix = mix_factor(1. - time);
	return progress * mix + time * (1. - mix);
#else
	return time;
#endif
}

gdouble
joy_easing_sine_curve(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return (sin(((time * G_PI * 2.)) - G_PI_2) + 1.) / 2.;
#else
	return time;
#endif
}

gdouble
joy_easing_cosine_curve(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return (cos(((time * G_PI * 2.)) - G_PI_2) + 1.) / 2.;
#else
	return time;
#endif
}
