/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/sketch.h"

G_DEFINE_TYPE(JoySketch, joy_sketch, JOY_TYPE_BUBBLE)

static void
joy_sketch_init(G_GNUC_UNUSED JoySketch *self)
{
}

static void
joy_sketch_class_init(G_GNUC_UNUSED JoySketchClass *klass)
{
}

JoyBubble *
joy_sketch_new(void)
{
	return g_object_new(JOY_TYPE_SKETCH,
			"horizontal-expand", TRUE,
			"vertical-expand", TRUE,
			NULL);
}
