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
 * \brief Display text
 */

#ifndef JOY_LABEL_H
#define JOY_LABEL_H

#include <joy/bubble.h>
#include <pango/pango.h>

G_BEGIN_DECLS

#define JOY_TYPE_LABEL \
	(joy_label_get_type())

#define JOY_LABEL(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_LABEL, JoyLabel))

#define JOY_IS_LABEL(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_LABEL))

#define JOY_LABEL_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_LABEL, JoyLabelClass))

#define JOY_IS_LABEL_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_LABEL))

#define JOY_LABEL_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_LABEL, JoyLabelClass))

typedef struct JoyLabelClass_ JoyLabelClass;

struct JoyLabel_ {
	/*< private >*/
	JoyBubble parent_instance;
	gpointer priv;
};

struct JoyLabelClass_ {
	/*< private >*/
	JoyBubbleClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_label_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new label.
 *
 * \param text [in] Initial label text (may be NULL).
 *
 * \return A new label object.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyBubble *
joy_label_new(const gchar *text);

void
joy_label_set_text(JoyBubble *self, const gchar *text);

const gchar *
joy_label_get_text(JoyBubble *self);

void
joy_label_set_markup(JoyBubble *self, const gchar *markup);

gboolean
joy_label_get_markup(JoyBubble *self);

void
joy_label_set_wrap(JoyBubble *self, PangoWrapMode wrap);

PangoWrapMode
joy_label_get_wrap(JoyBubble *self);

void
joy_label_set_ellipsize(JoyBubble *self, PangoEllipsizeMode ellipsize);

PangoEllipsizeMode
joy_label_get_ellipsize(JoyBubble *self);

void
joy_label_set_alignment(JoyBubble *self, PangoAlignment alignment);

PangoAlignment
joy_label_get_alignment(JoyBubble *self);

void
joy_label_set_justify(JoyBubble *self, gboolean justify);

gboolean
joy_label_get_justify(JoyBubble *self);

G_END_DECLS

#endif // JOY_LABEL_H
