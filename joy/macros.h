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
 * \brief Internal macros
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
