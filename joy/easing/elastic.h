/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Elastic easing functions for animation tweening
 * \author Mike Steinert <michael.steinert@echostar.com>
 *
 * These functions implement an exponentially decaying sine wave.
 */

#ifndef JOY_EASING_ELASTIC_H
#define JOY_EASING_ELASTIC_H

#include <glib.h>

G_BEGIN_DECLS

/// \brief Optional data for elastic easing functions.
struct JoyEasingElastic {
	/// The default amplitude is 0.3.
	gdouble amplitude;
	/// The default period is 1.0.
	gdouble period;
};

/**
 * \brief Accelerate from zero velocity.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_in_elastic(gpointer self, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Decelerate from zero velocity.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_out_elastic(gpointer self, gdouble time)
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
joy_easing_in_out_elastic(gpointer self, gdouble time)
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
joy_easing_out_in_elastic(gpointer self, gdouble time)
	G_GNUC_CONST;

G_END_DECLS

#endif // JOY_EASING_ELASTIC_H

