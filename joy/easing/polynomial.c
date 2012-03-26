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
#include "joy/easing/polynomial.h"

gdouble
joy_easing_in_quad(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return time * time;
#else
	return time;
#endif
}

gdouble
joy_easing_out_quad(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return -time * (time - 2);
#else
	return time;
#endif
}

gdouble
joy_easing_in_out_quad(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	time *= 2.;
	if (time < 1.) {
		return time * time / 2.;
	} else {
		time -= 1.;
		return -.5 * (time * (time - 2) - 1);
	}
#else
	return time;
#endif
}

gdouble
joy_easing_out_in_quad(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (.5 >  time) {
		return joy_easing_out_quad(self, time * 2.) / 2.;
	} else {
		return joy_easing_in_quad(self, time * 2. - 1.) / 2. + .5;
	}
#else
	return time;
#endif
}

gdouble
joy_easing_in_cubic(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return time * time * time;
#else
	return time;
#endif
}

gdouble
joy_easing_out_cubic(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	time -= 1.;
	return time * time * time + 1.;
#else
	return time;
#endif
}

gdouble
joy_easing_in_out_cubic(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	time *= 2.0;
	if (time < 1.) {
		return .5 * time * time * time;
	} else {
		time -= 2.;
		return 0.5 * (time * time * time + 2.);
	}
#else
	return time;
#endif
}

gdouble
joy_easing_out_in_cubic(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (.5 > time) {
		return joy_easing_out_cubic(self, time * 2.) / 2.;
	} else {
		return joy_easing_in_cubic(self, time * 2. - 1.) / 2. + .5;
	}
#else
	return time;
#endif
}

gdouble
joy_easing_in_quart(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return time * time * time * time;
#else
	return time;
#endif
}

gdouble
joy_easing_out_quart(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	time -= 1.;
	return -(time * time * time * time - 1.);
#else
	return time;
#endif
}

gdouble
joy_easing_in_out_quart(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	time *= 2.;
	if (time < 1.) {
		return .5 * time * time * time * time;
	} else {
		time -= 2.;
		return -.5 * (time * time * time * time - 2.);
	}
#else
	return time;
#endif
}

gdouble
joy_easing_out_in_quart(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (.5 > time) {
		return joy_easing_out_quart(self, time * 2.) / 2.;
	} else {
		return joy_easing_in_quart(self, time * 2. - 1.) / 2. + .5;
	}
#else
	return time;
#endif
}

gdouble
joy_easing_in_quint(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	return time * time * time * time * time;
#else
	return time;
#endif
}

gdouble
joy_easing_out_quint(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	time -= 1.;
	return time * time * time * time * time + 1.;
#else
	return time;
#endif
}

gdouble
joy_easing_in_out_quint(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	time *= 2.;
	if (time < 1) {
		return .5 * time * time * time * time * time;
	} else {
		time -= 2.;
		return .5 * (time * time * time * time * time + 2.);
	}

#else
	return time;
#endif
}

gdouble
joy_easing_out_in_quint(G_GNUC_UNUSED gpointer self, gdouble time)
{
#ifdef JOY_ENABLE_EASING
	if (.5 > time) {
		return joy_easing_out_quint(self, time * 2.) / 2.;
	} else {
		return joy_easing_in_quint(self, time * 2. - 2.) / 2. + .5;
	}
#else
	return time;
#endif
}
