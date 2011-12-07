/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Struct timespec manipulation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_TIMESPEC_H
#define JOY_TIMESPEC_H

#include <glib.h>
#include <time.h>

G_BEGIN_DECLS

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_gettime(struct timespec *x, clockid_t id);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_add(struct timespec *x, const struct timespec *y);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_add_seconds(struct timespec *x, gulong sec);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_add_milliseconds(struct timespec *x, gulong msec);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_add_microseconds(struct timespec *x, gulong usec);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_add_nanoseconds(struct timespec *x, gulong nsec);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_subtract(struct timespec *x, const struct timespec *y);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_subtract_seconds(struct timespec *x, gulong sec);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_subtract_milliseconds(struct timespec *x, gulong msec);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_subtract_microseconds(struct timespec *x, gulong usec);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_subtract_nanoseconds(struct timespec *x, gulong nsec);

G_GNUC_INTERNAL
inline gint
joy_timespec_compare(const struct timespec *x, const struct timespec *y);

G_GNUC_INTERNAL
inline gdouble
joy_timespec_seconds(const struct timespec *x);

G_GNUC_INTERNAL
inline gulong
joy_timespec_milliseconds(const struct timespec *x);

G_GNUC_INTERNAL
inline gulong
joy_timespec_microseconds(const struct timespec *x);

G_GNUC_INTERNAL
inline gulong
joy_timespec_nanoseconds(const struct timespec *x);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_from_seconds(struct timespec *x, gulong sec);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_from_milliseconds(struct timespec *x, gulong msec);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_from_microseconds(struct timespec *x, gulong usec);

G_GNUC_INTERNAL
inline struct timespec *
joy_timespec_from_nanoseconds(struct timespec *x, gulong nsec);

G_END_DECLS

#endif // JOY_TIMESPEC_H
