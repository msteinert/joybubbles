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
#include "joy/style.h"
#include <time.h>

struct JoyTimer_ {
	struct timespec start;
};

JoyTimer *
joy_timer_new(void)
{
	JoyTimer *self = g_slice_new0(JoyTimer);
	if (clock_gettime(CLOCK_MONOTONIC, &self->start)) {
		g_message("clock_gettime: %s", g_strerror(errno));
	}
	return self;
}

void
joy_timer_destroy(JoyTimer *self)
{
	if (self) {
		g_slice_free(JoyTimer, self);
	}
}

void
joy_timer_start(JoyTimer *self)
{
	if (clock_gettime(CLOCK_MONOTONIC, &self->start)) {
		g_message("clock_gettime: %s", g_strerror(errno));
	}
}

gulong
joy_timer_elapsed(JoyTimer *self)
{
	struct timespec end, diff = self->start;
	if (clock_gettime(CLOCK_MONOTONIC, &end)) {
		g_message("clock_gettime: %s", g_strerror(errno));
	}
	if (end.tv_nsec < diff.tv_nsec) {
		gulong sec = (gulong)((diff.tv_nsec - end.tv_nsec) / (1E9 + 1));
		diff.tv_sec += sec;
		diff.tv_nsec = (gulong)(1E9 * sec);
	}
	if (end.tv_nsec - diff.tv_nsec > 1E9) {
		gulong sec = (end.tv_nsec - diff.tv_nsec) / 1E9;
		diff.tv_sec -= sec;
		diff.tv_nsec += (gulong)(1E9 * sec);
	}
	diff.tv_sec = end.tv_sec - diff.tv_sec;
	diff.tv_nsec = end.tv_nsec - diff.tv_nsec;
	return (diff.tv_sec * 1000000L) + (diff.tv_nsec * 0.001);
}
