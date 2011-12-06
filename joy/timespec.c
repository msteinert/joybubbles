/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/timespec.h"

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
	joy_timespec_add(x, &y);
	return x;
}

struct timespec *
joy_timespec_subtract_microseconds(struct timespec *x, gulong usec)
{
	struct timespec y;
	joy_timespec_from_microseconds(&y, usec);
	joy_timespec_add(x, &y);
	return x;
}

struct timespec *
joy_timespec_subtract_nanoseconds(struct timespec *x, gulong nsec)
{
	struct timespec y;
	joy_timespec_from_nanoseconds(&y, nsec);
	joy_timespec_add(x, &y);
	return x;
}

gint
joy_timespec_compare(const struct timespec *x, const struct timespec *y)
{
	if (x->tv_sec == y->tv_sec) {
		if (x->tv_nsec == y->tv_nsec) {
			return 0;
		} else if (x->tv_nsec < y->tv_nsec) {
			return -1;
		} else {
			return 1;
		}
	} else if (x->tv_sec < y->tv_sec) {
		return -1;
	} else {
		return 1;
	}
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
