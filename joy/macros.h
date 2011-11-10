/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Internal macros
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_MACROS_H
#define JOY_MACROS_H

#include <glib.h>
#include "joy/error.h"

G_BEGIN_DECLS

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3)
#define JOY_GNUC_HOT \
	__attribute__((hot))
#else
#define JOY_GNUC_HOT
#endif // __GNUC__

#ifdef G_DISABLE_CHECKS
#define joy_return_error_if_fail(expr, val, error)
#else // G_DISABLE_CHECKS
#ifdef __GNUC__
#define joy_return_error_if_fail(expr, val, error) \
G_STMT_START { \
	if G_UNLIKELY(!(expr)) { \
		g_set_error(error, JOY_ERROR, JOY_ERROR_FAILURE, \
				"%s: assertion `%s' failed", \
				G_STRLOC, #expr); \
		g_return_if_fail_warning(G_LOG_DOMAIN, G_STRLOC, #expr); \
		return val; \
	} \
} G_STMT_END
#else // __GNUC__
#define joy_return_error_if_fail(expr, val, error) \
G_STMT_START { \
	if G_UNLIKELY(!(expr)) { \
		g_set_error(error, JOY_ERROR, JOY_ERROR_FAILURE, \
				"file %s: line %s: assertion `%s' failed", \
				__FILE__, __LINE__, #expr); \
		g_log(G_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL, \
				"file %s: line %d: assertion `%s' failed", \
				__FILE__, __LINE__, #expr); \
		return val; \
	} \
} G_STMT_END
#endif // __GNUC__
#endif // G_DISABLE_CHECKS

#ifdef G_DISABLE_CHECKS
#define JOY_UNIMPLEMENTED
#else // G_DISABLE_CHECKS
#define JOY_UNIMPLEMENTED \
G_STMT_START { \
	g_log(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING, \
			"%s(%s:%d): unimplemented", \
			__func__, __FILE__, __LINE__); \
} G_STMT_END
#endif // G_DISABLE_CHECKS

G_END_DECLS

#endif // JOY_MACROS_H

