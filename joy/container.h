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
 * \brief A widget that contains others
 *
 * This widget is the base class for containers.
 */

#ifndef JOY_CONTAINER_H
#define JOY_CONTAINER_H

#include <joy/bubble.h>

G_BEGIN_DECLS

#define JOY_TYPE_CONTAINER \
	(joy_container_get_type())

#define JOY_CONTAINER(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_CONTAINER, \
		JoyContainer))

#define JOY_IS_CONTAINER(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_CONTAINER))

#define JOY_CONTAINER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_CONTAINER, \
		JoyContainerClass))

#define JOY_IS_CONTAINER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_CONTAINER))

#define JOY_CONTAINER_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_CONTAINER, \
		JoyContainerClass))

typedef struct JoyContainerClass_ JoyContainerClass;

struct JoyContainer_ {
	/*< private >*/
	JoyBubble parent_instance;
	gpointer priv;
};

typedef JoyIterator *
(*JoyContainerBegin)(JoyBubble *self);

typedef JoyIterator *
(*JoyContainerEnd)(JoyBubble *self);

typedef void
(*JoyContainerAdd)(JoyBubble *self, JoyBubble *child);

typedef void
(*JoyContainerRemove)(JoyBubble *self, JoyBubble *child);

struct JoyContainerClass_ {
	/*< private >*/
	JoyBubbleClass parent_class;
	/*< public >*/
	JoyContainerBegin begin;
	JoyContainerEnd end;
	JoyContainerAdd add;
	JoyContainerRemove remove;
};

G_GNUC_NO_INSTRUMENT
GType
joy_container_get_type(void) G_GNUC_CONST;

/**
 * \brief Get an iterator to the first child of a container.
 *
 * \param self [in] A container object.
 *
 * \return An iterator to the first child of \e self or NULL if \e self does
 *         not have any children.
 *
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_container_begin(JoyBubble *self);

/**
 * \brief Get an iterator to the last child of a container.
 *
 * \param self [in] A container object.
 *
 * \return An iterator to the last child of \e self or NULL if \e self does
 *         not have any children.
 *
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_container_end(JoyBubble *self);

/**
 * \brief Add a child to a container.
 *
 * \param self [in] A container object.
 * \param child [in] The new child of \e self.
 */
void
joy_container_add(JoyBubble *self, JoyBubble *child);

/**
 * \brief remove a child of a container.
 *
 * \param self [in] A container object.
 * \param child [in] A child of \e self.
 */
void
joy_container_remove(JoyBubble *self, JoyBubble *child);

G_END_DECLS

#endif // JOY_CONTAINER_H
