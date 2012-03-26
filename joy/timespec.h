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

/**
 * \file
 * \brief Struct timespec manipulation
 */

#ifndef JOY_TIMESPEC_H
#define JOY_TIMESPEC_H

#include <glib.h>
#include <time.h>

G_BEGIN_DECLS

G_GNUC_INTERNAL
struct timespec *
joy_timespec_gettime(struct timespec *x, clockid_t id);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_add(struct timespec *x, const struct timespec *y);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_add_seconds(struct timespec *x, gulong sec);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_add_milliseconds(struct timespec *x, gulong msec);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_add_microseconds(struct timespec *x, gulong usec);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_add_nanoseconds(struct timespec *x, gulong nsec);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_subtract(struct timespec *x, const struct timespec *y);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_subtract_seconds(struct timespec *x, gulong sec);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_subtract_milliseconds(struct timespec *x, gulong msec);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_subtract_microseconds(struct timespec *x, gulong usec);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_subtract_nanoseconds(struct timespec *x, gulong nsec);

G_GNUC_INTERNAL
gint
joy_timespec_compare(const struct timespec *x, const struct timespec *y);

G_GNUC_INTERNAL
gdouble
joy_timespec_seconds(const struct timespec *x);

G_GNUC_INTERNAL
gulong
joy_timespec_milliseconds(const struct timespec *x);

G_GNUC_INTERNAL
gulong
joy_timespec_microseconds(const struct timespec *x);

G_GNUC_INTERNAL
gulong
joy_timespec_nanoseconds(const struct timespec *x);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_from_seconds(struct timespec *x, gulong sec);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_from_milliseconds(struct timespec *x, gulong msec);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_from_microseconds(struct timespec *x, gulong usec);

G_GNUC_INTERNAL
struct timespec *
joy_timespec_from_nanoseconds(struct timespec *x, gulong nsec);

G_END_DECLS

#endif // JOY_TIMESPEC_H
