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
joy_easing_in_curve(gpointer self, gdouble time)
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
joy_easing_out_curve(gpointer self, gdouble time)
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
joy_easing_sine_curve(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return (sin(((time * G_PI * 2.)) - G_PI_2) + 1.) / 2.;
#else
	return time;
#endif
}

gdouble
joy_easing_cosine_curve(gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return (cos(((time * G_PI * 2.)) - G_PI_2) + 1.) / 2.;
#else
	return time;
#endif
}
