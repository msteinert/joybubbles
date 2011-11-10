/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Curve easing functions for animation tweening
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_EASING_CURVE_H
#define JOY_EASING_CURVE_H

#include <glib.h>

G_BEGIN_DECLS

/**
 * \brief Accelerate from zero velocity.
 *
 * Begins by growing slowly, increasing in speed. At the end of the curve
 * the speed will be constant.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_in_curve(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Decelerate from zero velocity.
 *
 * Begins by growing steadily, ending slowly. The speed at the beginning of
 * the curve will be constant.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_out_curve(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief The value grows sinusoidally.
 *
 * \note The calculated end value will be 0 rather than 1.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_sine_curve(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief The value grows cosinusoidally.
 *
 * \note The calculated start and end values will be 0.5.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_cosine_curve(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

G_END_DECLS

#endif // JOY_EASING_CURVE_H

