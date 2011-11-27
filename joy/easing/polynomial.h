/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Polynomial easing functions for animation tweening
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_EASING_POLYNOMIAL_H
#define JOY_EASING_POLYNOMIAL_H

#include <glib.h>

G_BEGIN_DECLS

/**
 * \brief Second order acceleration from zero velocity.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_in_quad(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Second order deceleration from zero velocity.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_out_quad(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Accelerate until halfway, then decelerate (second order).
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_in_out_quad(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Decelerate until halfway, then accelerate (second order).
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_out_in_quad(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Third order acceleration from zero velocity.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_in_cubic(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Third order deceleration from zero velocity.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_out_cubic(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Accelerate until halfway, then decelerate (third order).
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_in_out_cubic(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Decelerate until halfway, then accelerate (third order).
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_out_in_cubic(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Fourth order acceleration from zero velocity.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_in_quart(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Fourth order deceleration from zero velocity.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_out_quart(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Accelerate until halfway, then decelerate (fourth order).
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_in_out_quart(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Decelerate until halfway, then accelerate (fourth order).
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_out_in_quart(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Fifth order acceleration from zero velocity.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_in_quint(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Fifth order deceleration from zero velocity.
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_out_quint(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Accelerate until halfway, then decelerate (fifth order).
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_in_out_quint(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

/**
 * \brief Decelerate until halfway, then accelerate (fifth order).
 *
 * \param self [in] Algorithm-specific data (may be NULL).
 * \param time [in] The current elapsed time or frame.
 *
 * \return The transformed elapsed time or frame.
 */
gdouble
joy_easing_out_in_quint(gpointer self G_GNUC_UNUSED, gdouble time)
	G_GNUC_CONST;

G_END_DECLS

#endif // JOY_EASING_POLYNOMIAL_H
