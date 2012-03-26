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
