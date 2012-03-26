/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include <fcntl.h>
#include <input_mgr_lite.h>
#include "joy/error.h"
#include "joy/iterator/ptr-array.h"
#include "joy/platform/gfx3d/application.h"
#include "joy/platform/gfx3d/screen.h"
#include "joy/platform/gfx3d/source.h"
#include <sys/types.h>
#include <sys/stat.h>

G_DEFINE_TYPE(JoyGfx3dApplication, joy_gfx3d_application, \
		JOY_TYPE_APPLICATION)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_GFX3D_APPLICATION, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyGfx3dApplication *)instance)->priv)

struct Private {
	GPtrArray *screens;
	gboolean vsync;
	gchar *input;
};

static void
joy_gfx3d_application_init(JoyGfx3dApplication *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->vsync = TRUE;
}

static void
destroy(gpointer object)
{
	g_object_run_dispose(G_OBJECT(object));
	g_object_unref(object);
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

static JoyScreen *
get_default_screen(JoyApplication *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->screens && priv->screens->len)) {
		return g_ptr_array_index(priv->screens, 0);
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

static gboolean
arg_vsync_cb(G_GNUC_UNUSED const gchar *key,
	     const gchar *value,
	     gpointer self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (0 == strcasecmp("false", value)) {
		priv->vsync = FALSE;
	}
	return TRUE;
}

static gboolean
arg_input_cb(G_GNUC_UNUSED const gchar *key,
	     const gchar *value,
	     gpointer self)
{
	struct Private *priv = GET_PRIVATE(self);
	priv->input = g_strdup(value);
	return TRUE;
}

static const GOptionEntry const gfx3d_arguments[] = {
	{
		"gfx3d-vsync",
		'\0',
		0,
		G_OPTION_ARG_CALLBACK,
		arg_vsync_cb,
		N_("Synchronize with the vertical refresh [true]"),
		N_("[true|false]")
	},
	{
		"gfx3d-input",
		'\0',
		0,
		G_OPTION_ARG_CALLBACK,
		arg_input_cb,
		N_("The input FIFO [" INPUT_MANAGER_FIFO_NAME "]"),
		N_("PATH")
	},
	{ NULL, '\0', 0, G_OPTION_ARG_NONE, NULL, NULL, NULL }
};

static gboolean
post_hook(G_GNUC_UNUSED GOptionContext *context,
	  G_GNUC_UNUSED GOptionGroup *group,
	  gpointer self,
	  GError **error)
{
	struct Private *priv = GET_PRIVATE(self);
	// initialize screens
	priv->screens = g_ptr_array_sized_new(2);
	if (!priv->screens) {
		g_set_error_literal(error, JOY_ERROR, JOY_ERROR_FAILURE,
				"gfx3d: out of memory");
		return FALSE;
	}
	g_ptr_array_set_free_func(priv->screens, destroy);
	JoyScreen *screen;
	// initialize the HD display
	screen = joy_gfx3d_screen_new(self, 0, 1280, 720);
	if (!screen) {
		g_set_error_literal(error, JOY_ERROR, JOY_ERROR_FAILURE,
				"gfx3d: failed to initialize the HD display");
		return FALSE;
	}
	joy_gfx3d_screen_set_vsync(screen, priv->vsync);
	g_ptr_array_add(priv->screens, screen);
	// initialize the SD display
	screen = joy_gfx3d_screen_new(self, 1, 640, 480);
	if (!screen) {
		g_set_error_literal(error, JOY_ERROR, JOY_ERROR_FAILURE,
				"gfx3d: failed to initialize the SD display");
		return FALSE;
	}
	joy_gfx3d_screen_set_vsync(screen, priv->vsync);
	g_ptr_array_add(priv->screens, screen);
	// add input_mgr source
	const gchar *input = priv->input ? priv->input
		: INPUT_MANAGER_FIFO_NAME;
	gint descriptor = open(input, O_RDONLY | O_NONBLOCK);
	if (G_UNLIKELY(-1 == descriptor)) {
		g_set_error(error, JOY_ERROR, JOY_ERROR_FAILURE,
				"gfx3d: %s: %s", input, g_strerror(errno));
		return FALSE;
	}
	JoySource *source = joy_gfx3d_source_new(self, descriptor);
	joy_application_add_source(self, source);
	return TRUE;
}

static void
add_options(JoyApplication *self, GOptionContext *context)
{
	GOptionGroup *group = g_option_group_new("gfx3d",
			Q_("GFX3D Options"),
			Q_("Show GFX3D Options"), self, NULL);
	if (!group) {
		return;
	}
	g_option_group_set_parse_hooks(group, NULL, post_hook);
	g_option_group_add_entries(group, gfx3d_arguments);
	g_option_context_add_group(context, group);
}

static void
joy_gfx3d_application_class_init(JoyGfx3dApplicationClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	JoyApplicationClass *application_class = JOY_APPLICATION_CLASS(klass);
	application_class->get_screen = get_screen;
	application_class->get_default_screen = get_default_screen;
	application_class->begin = begin;
	application_class->add_options = add_options;
	g_type_class_add_private(klass, sizeof(struct Private));
}

JoyApplication *
joy_platform_application_new(void)
{
	return g_object_new(JOY_TYPE_GFX3D_APPLICATION,
			NULL);
}
