/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/iterator/ptr-array.h"
#include "joy/platform/gfx3d/application.h"
#include "joy/platform/gfx3d/screen.h"
#include "joy/platform/gfx3d/source.h"

G_DEFINE_TYPE(JoyGfx3dApplication, joy_gfx3d_application, \
		JOY_TYPE_APPLICATION)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_GFX3D_APPLICATION, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyGfx3dApplication *)instance)->priv)

struct Private {
	GPtrArray *screens;
};

static void
joy_gfx3d_application_init(JoyGfx3dApplication *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
destroy(gpointer object)
{
	g_object_run_dispose(G_OBJECT(object));
	g_object_unref(object);
}

static void
constructed(GObject *base)
{
	JoyApplication *self = JOY_APPLICATION(base);
	struct Private *priv = GET_PRIVATE(base);
	// initialize screens
	priv->screens = g_ptr_array_sized_new(2);
	if (priv->screens) {
		g_ptr_array_set_free_func(priv->screens, destroy);
		JoyBubble *screen;
		// add HD screen
		screen = joy_gfx3d_screen_new(self, 0, 1280, 720);
		if (screen) {
			g_ptr_array_add(priv->screens, screen);
		}
		// add SD screen
		screen = joy_gfx3d_screen_new(self, 1, 640, 480);
		if (screen) {
			g_ptr_array_add(priv->screens, screen);
		}
	}
	joy_application_add_source(self, joy_gfx3d_source_new(self));
	if (G_OBJECT_CLASS(joy_gfx3d_application_parent_class)->constructed) {
		G_OBJECT_CLASS(joy_gfx3d_application_parent_class)->
			constructed(base);
	}
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->screens) {
		g_ptr_array_free(priv->screens, TRUE);
		priv->screens = NULL;
	}
	G_OBJECT_CLASS(joy_gfx3d_application_parent_class)->dispose(base);
}

static JoyScreen *
get_screen(JoyApplication *self, guint n)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->screens && (n < priv->screens->len))) {
		return g_ptr_array_index(priv->screens, n);
	}
	return NULL;
}

static JoyIterator *
begin(JoyApplication *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->screens)) {
		return joy_iterator_ptr_array_begin(priv->screens);
	}
	return NULL;
}

static void
joy_gfx3d_application_class_init(JoyGfx3dApplicationClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructed = constructed;
	object_class->dispose = dispose;
	JoyApplicationClass *application_class = JOY_APPLICATION_CLASS(klass);
	application_class->get_screen = get_screen;
	application_class->begin = begin;
	g_type_class_add_private(klass, sizeof(struct Private));
}

JoyApplication * 
joy_platform_application_new(void) 
{ 
	return g_object_new(JOY_TYPE_GFX3D_APPLICATION, NULL); 
} 
