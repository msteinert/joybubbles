/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief A push-button
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_BUTTON_H
#define JOY_BUTTON_H

#include <joy/bubble.h>

G_BEGIN_DECLS

#define JOY_TYPE_BUTTON \
	(joy_button_get_type())

#define JOY_BUTTON(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_BUTTON, JoyButton))

#define JOY_IS_BUTTON(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_BUTTON))

#define JOY_BUTTON_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_BUTTON, JoyButtonClass))

#define JOY_IS_BUTTON_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_BUTTON))

#define JOY_BUTTON_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_BUTTON, \
		JoyButtonClass))

typedef struct JoyButtonClass_ JoyButtonClass;

struct JoyButton_ {
	/*< private >*/
	JoyBubble parent_instance;
	gpointer priv;
};

struct JoyButtonClass_ {
	/*< private >*/
	JoyBubbleClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_button_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new button.
 *
 * \return A new button object.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyBubble *
joy_button_new(const gchar *text);

void
joy_button_set_text(JoyBubble *self, const gchar *text);

G_END_DECLS

#endif // JOY_BUTTON_H
