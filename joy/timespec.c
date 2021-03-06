/* Copyright 2011, 2012 Michael Steinert
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
#include <errno.h>
#include "joy/timespec.h"

struct timespec *
joy_timespec_gettime(struct timespec *x, clockid_t id)
{
	if (clock_gettime(id, x)) {
		g_message("clock_gettime: %s", g_strerror(errno));
	}
	return x;
}

struct timespec *
joy_timespec_add(struct timespec *x, const struct timespec *y)
{
	x->tv_sec = x->tv_sec + y->tv_sec;
	x->tv_nsec = x->tv_nsec + y->tv_nsec;
	if (x->tv_nsec >= 1e9) {
		++x->tv_sec;
		x->tv_nsec -= 1e9;
	}
	return x;
}

struct timespec *
joy_timespec_add_seconds(struct timespec *x, gulong sec)
{
	x->tv_sec += sec;
	return x;
}

struct timespec *
joy_timespec_add_milliseconds(struct timespec *x, gulong msec)
{
	x->tv_nsec += msec * 1e6;
	if (x->tv_nsec >= 1e9) {
		++x->tv_sec;
		x->tv_nsec -= 1e9;
	}
	return x;
}

struct timespec *
joy_timespec_add_microseconds(struct timespec *x, gulong usec)
{
	x->tv_nsec += usec * 1e3;
	if (x->tv_nsec >= 1e9) {
		++x->tv_sec;
		x->tv_nsec -= 1e9;
	}
	return x;
}

struct timespec *
joy_timespec_add_nanoseconds(struct timespec *x, gulong nsec)
{
	x->tv_nsec += nsec;
	if (x->tv_nsec >= 1e9) {
		++x->tv_sec;
		x->tv_nsec -= 1e9;
	}
	return x;
}

struct timespec *
joy_timespec_subtract(struct timespec *x, const struct timespec *y)
{
	struct timespec z = *y;
	if (x->tv_nsec < z.tv_nsec) {
		int nsec = (z.tv_nsec - x->tv_nsec) / 1e9 + 1;
		z.tv_nsec -= 1e9 * nsec;
		z.tv_sec += nsec;
	}
	if (x->tv_nsec - z.tv_nsec > 1e9) {
		int nsec = (x->tv_nsec - z.tv_nsec) / 1e9;
		z.tv_nsec += 1e9 * nsec;
		z.tv_sec -= nsec;
	}
	x->tv_sec -= z.tv_sec;
	x->tv_nsec -= z.tv_nsec;
	return x;
}

struct timespec *
joy_timespec_subtract_seconds(struct timespec *x, gulong sec)
{
	x->tv_sec -= sec;
	return x;
}

struct timespec *
joy_timespec_subtract_milliseconds(struct timespec *x, gulong msec)
{
	struct timespec y;
	joy_timespec_from_milliseconds(&y, msec);
	joy_timespec_subtract(x, &y);
	return x;
}

struct timespec *
joy_timespec_subtract_microseconds(struct timespec *x, gulong usec)
{
	struct timespec y;
	joy_timespec_from_microseconds(&y, usec);
	joy_timespec_subtract(x, &y);
	return x;
}

struct timespec *
joy_timespec_subtract_nanoseconds(struct timespec *x, gulong nsec)
{
	struct timespec y;
	joy_timespec_from_nanoseconds(&y, nsec);
	joy_timespec_subtract(x, &y);
	return x;
}

gint
joy_timespec_compare(const struct timespec *x, const struct timespec *y)
{
	if (x->tv_sec < y->tv_sec) {
		return -1;
	} else if (x->tv_sec == y->tv_sec) {
		if (x->tv_nsec < y->tv_nsec) {
			return -1;
		} else if (x->tv_nsec == y->tv_nsec) {
			return 0;
		}
		return 1;
	}
	return 1;
}

gdouble
joy_timespec_seconds(const struct timespec *x)
{
	return 1.e-9 * x->tv_nsec + x->tv_sec;
}

gulong
joy_timespec_milliseconds(const struct timespec *x)
{
	return 1e3 * x->tv_sec + 1.e-6 * x->tv_nsec;
}

gulong
joy_timespec_microseconds(const struct timespec *x)
{
	return 1e6 * x->tv_sec + 1.e-3 * x->tv_nsec;
}

gulong
joy_timespec_nanoseconds(const struct timespec *x)
{
	return 1e9 * x->tv_sec + x->tv_nsec;
}

struct timespec *
joy_timespec_from_seconds(struct timespec *x, gulong sec)
{
	x->tv_sec = sec;
	x->tv_nsec = 0;
	return x;
}

struct timespec *
joy_timespec_from_milliseconds(struct timespec *x, gulong msec)
{
	x->tv_sec = 0;
	x->tv_nsec = 0;
	joy_timespec_add_milliseconds(x, msec);
	return x;
}

struct timespec *
joy_timespec_from_microseconds(struct timespec *x, gulong usec)
{
	x->tv_sec = 0;
	x->tv_nsec = 0;
	joy_timespec_add_microseconds(x, usec);
	return x;
}

struct timespec *
joy_timespec_from_nanoseconds(struct timespec *x, gulong nsec)
{
	x->tv_sec = 0;
	x->tv_nsec = 0;
	joy_timespec_add_nanoseconds(x, nsec);
	return x;
}
