/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Sinusoidal easing functions for animation tweening
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_EASING_SINUSOIDAL_H
#define JOY_EASING_SINUSOIDAL_H

#include <glib.h>

G_BEGIN_DECLS

/**
 * \brief Accelerate from zero velocity.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_in_sine(gpointer self G_GNUC_UNUSED, gdouble time)
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
joy_easing_out_sine(gpointer self G_GNUC_UNUSED, gdouble time)
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
joy_easing_in_out_sine(gpointer self G_GNUC_UNUSED, gdouble time)
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
joy_easing_out_in_sine(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

G_END_DECLS

#endif // JOY_EASING_SINUSOIDAL_H

