/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Window style
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_STYLE_WINDOW_H
#define JOY_STYLE_WINDOW_H

#include <joy/style/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_STYLE_WINDOW \
	(joy_style_window_get_type())

#define JOY_STYLE_WINDOW(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_STYLE_WINDOW, \
		JoyStyleWindow))

#define JOY_IS_STYLE_WINDOW(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_STYLE_WINDOW))

#define JOY_STYLE_WINDOW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_STYLE_WINDOW, \
		JoyStyleWindowClass))

#define JOY_IS_STYLE_WINDOW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_STYLE_WINDOW))

#define JOY_STYLE_WINDOW_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_STYLE_WINDOW, \
		JoyStyleWindowClass))

typedef struct JoyStyleWindowClass_ JoyStyleWindowClass;

struct JoyStyleWindow_ {
	/*< private >*/
	JoyStyle parent_instance;
};

struct JoyStyleWindowClass_ {
	/*< private >*/
	JoyStyleClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_style_window_get_type(void) G_GNUC_CONST;

G_END_DECLS

#endif // JOY_STYLE_WINDOW_H
