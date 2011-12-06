/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief High-resolution timer
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_TIMER_H
#define JOY_TIMER_H

#include <glib.h>

G_BEGIN_DECLS

typedef struct JoyTimer_ JoyTimer;

G_GNUC_INTERNAL
JoyTimer *
joy_timer_new(void);

G_GNUC_INTERNAL
void
joy_timer_destroy(JoyTimer *self);

G_GNUC_INTERNAL
const struct timespec *
joy_timer_get_start(JoyTimer *self);

G_GNUC_INTERNAL
void
joy_timer_start(JoyTimer *self);

G_GNUC_INTERNAL
gint
joy_timer_elapsed(JoyTimer *self, struct timespec *elapsed);

G_END_DECLS

#endif // JOY_TIMER_H
