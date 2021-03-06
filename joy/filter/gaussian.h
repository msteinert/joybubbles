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
 * \brief Gaussian blur filter
 */

#ifndef JOY_FILTER_GAUSSIAN_H
#define JOY_FILTER_GAUSSIAN_H

#include <joy/filter/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_FILTER_GAUSSIAN \
	(joy_filter_gaussian_get_type())

#define JOY_FILTER_GAUSSIAN(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_FILTER_GAUSSIAN, \
		JoyFilterGaussian))

#define JOY_IS_FILTER_GAUSSIAN(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_FILTER_GAUSSIAN))

#define JOY_FILTER_GAUSSIAN_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_FILTER_GAUSSIAN, \
		JoyFilterGaussianClass))

#define JOY_IS_FILTER_GAUSSIAN_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_FILTER_GAUSSIAN))

#define JOY_FILTER_GAUSSIAN_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_FILTER_GAUSSIAN, \
		JoyFilterGaussianClass))

typedef struct JoyFilterGaussianClass_ JoyFilterGaussianClass;

struct JoyFilterGaussian_ {
	/*< private >*/
	JoyFilter parent_instance;
	gpointer priv;
};

struct JoyFilterGaussianClass_ {
	/*< private >*/
	JoyFilterClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_filter_gaussian_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new gaussian blur filter.
 *
 * \param radius [in] The blur radius.
 *
 * \return A new filter object or NULL if an error occurred.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyFilter *
joy_filter_gaussian_new(gdouble radius);

/**
 * \brief Set a new blur radius.
 *
 * \param self [in] A gaussian blur filter object.
 * \param radius [in] The new blur radius for \e self.
 */
void
joy_filter_gaussian_set_radius(JoyFilter *self, gdouble radius);

/**
 * \brief Get the blur radius.
 *
 * \param self [in] A gaussian blur filter object.
 *
 * \return The blur radius for \e self.
 */
gdouble
joy_filter_gaussian_get_radius(JoyFilter *self);

G_END_DECLS

#endif // JOY_FILTER_GAUSSIAN_H
