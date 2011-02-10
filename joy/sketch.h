/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief A custom drawing
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_SKETCH_H
#define JOY_SKETCH_H

#include <joy/bubble.h>

G_BEGIN_DECLS

#define JOY_TYPE_SKETCH \
	(joy_sketch_get_type())

#define JOY_SKETCH(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_SKETCH, JoySketch))

#define JOY_IS_SKETCH(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_SKETCH))

#define JOY_SKETCH_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_SKETCH, \
		JoySketchClass))

#define JOY_IS_SKETCH_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_SKETCH))

#define JOY_SKETCH_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_SKETCH, \
		JoySketchClass))

typedef struct JoySketchClass_ JoySketchClass;

struct JoySketch_ {
	/*< private >*/
	JoyBubble parent_instance;
};

struct JoySketchClass_ {
	/*< private >*/
	JoyBubbleClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_sketch_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new sketch.
 *
 * \return A new sketch object.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyBubble *
joy_sketch_new(void);

G_END_DECLS

#endif // JOY_SKETCH_H
