/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "joy/timer.h"
#include "joy/timespec.h"
#include "joy/style.h"
#include <time.h>

struct JoyTimer_ {
	struct timespec start;
};

JoyTimer *
joy_timer_new(void)
{
	JoyTimer *self = g_slice_new0(JoyTimer);
	joy_timespec_gettime(&self->start, CLOCK_MONOTONIC);
	return self;
}

void
joy_timer_destroy(JoyTimer *self)
{
	if (self) {
		g_slice_free(JoyTimer, self);
	}
}

const struct timespec *
joy_timer_get_start(JoyTimer *self)
{
	return &self->start;
}

void
joy_timer_start(JoyTimer *self)
{
	joy_timespec_gettime(&self->start, CLOCK_MONOTONIC);
}

void
joy_timer_elapsed(JoyTimer *self, struct timespec *elapsed)
{
	joy_timespec_gettime(elapsed, CLOCK_MONOTONIC);
	joy_timespec_subtract(elapsed, &self->start);
}
