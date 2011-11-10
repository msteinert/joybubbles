/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/device.h"
#include "joy/screen.h"

G_DEFINE_ABSTRACT_TYPE(JoyDevice, joy_device, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_DEVICE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyDevice *)instance)->priv)

struct Private {
	gchar *name;
	JoyScreen *screen;
};

static void
joy_device_init(JoyDevice *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->screen) {
		g_object_unref(priv->screen);
		priv->screen = NULL;
	}
	G_OBJECT_CLASS(joy_device_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->name);
	G_OBJECT_CLASS(joy_device_parent_class)->finalize(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_NAME,
	PROP_SCREEN,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_NAME:
		priv->name = g_value_dup_string(value);
		break;
	case PROP_SCREEN:
		priv->screen = g_value_dup_object(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
joy_device_class_init(JoyDeviceClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	g_type_class_add_private(klass, sizeof(struct Private));
	// properties
	g_object_class_install_property(object_class, PROP_NAME,
		g_param_spec_string("name", Q_("Name"),
			Q_("The name of this device"), NULL,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_SCREEN,
		g_param_spec_object("screen", Q_("Screen"),
			Q_("The screen this device is on"), JOY_TYPE_SCREEN,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

const gchar *
joy_device_get_name(JoyDevice *self)
{
	g_return_val_if_fail(JOY_IS_DEVICE(self), NULL);
	return GET_PRIVATE(self)->name;
}

JoyScreen *
joy_device_get_screen(JoyDevice *self)
{
	g_return_val_if_fail(JOY_IS_DEVICE(self), NULL);
	return GET_PRIVATE(self)->screen;
}

