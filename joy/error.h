/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \brief Error definitions
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_ERROR_H
#define JOY_ERROR_H

#include <glib.h>

G_BEGIN_DECLS

#define JOY_ERROR joy_error_quark()

typedef enum {
	JOY_ERROR_FAILURE,
	JOY_ERROR_MAX
} JoyError;

GQuark
joy_error_quark(void);

G_END_DECLS

#endif // JOY_ERROR_H
