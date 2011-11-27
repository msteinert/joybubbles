/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Bounce easing functions for animation tweening
 * \author Mike Steinert <michael.steinert@echostar.com>
 *
 * These functions implement exponentially decaying parabolic bouncing.
 */

#ifndef JOY_EASING_BOUNCE_H
#define JOY_EASING_BOUNCE_H

#include <glib.h>

G_BEGIN_DECLS

/// \brief Optional data for bounce easing functions.
struct JoyEasingBounce {
	/// The default amplitude is 1.0.
	gdouble amplitude;
};

/**
 * \brief Decelerate from zero velocity.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_in_bounce(gpointer self, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Accelerate from zero velocity.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_out_bounce(gpointer self, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Accelerate until halfway, then decelerate.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_in_out_bounce(gpointer self, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Decelerate until halfway, then accelerate.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_out_in_bounce(gpointer self, gdouble time)
	G_GNUC_CONST;

G_END_DECLS

#endif // JOY_EASING_BOUNCE_H
