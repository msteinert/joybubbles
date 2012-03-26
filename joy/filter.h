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
 * \brief Visual filtering
 */

#ifndef JOY_FILTER_H
#define JOY_FILTER_H

#include <cairo.h>
#include <glib-object.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_FILTER \
	(joy_filter_get_type())

#define JOY_FILTER(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_FILTER, \
		JoyFilter))

#define JOY_IS_FILTER(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_FILTER))

#define JOY_FILTER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_FILTER, \
		JoyFilterClass))

#define JOY_IS_FILTER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_FILTER))

#define JOY_FILTER_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_FILTER, \
		JoyFilterClass))

typedef struct JoyFilterClass_ JoyFilterClass;

struct JoyFilter_ {
	/*< private >*/
	GObject parent_instance;
};

typedef void
(*JoyFilterApply)(JoyFilter *self, JoyBubble *widget, cairo_t *cr);

struct JoyFilterClass_ {
	/*< private >*/
	GObjectClass parent_class;
	/*< public >*/
	JoyFilterApply apply;
};

G_GNUC_NO_INSTRUMENT
GType
joy_filter_get_type(void) G_GNUC_CONST;

/**
 * \brief Apply a filter to a widget.
 *
 * \note Filters must applied after the widget has drawn itself.
 *
 * \param self [in] A filter object.
 * \param widget [in] The widget \e self is being applied to.
 * \param cr [in] The cairo context to apply the filter to.
 */
void
joy_filter_apply(JoyFilter *self, JoyBubble *widget, cairo_t *cr);

G_END_DECLS

#endif // JOY_FILTER_H
