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
#include "joy/application.h"
#include "joy/iterator.h"
#include "joy/macros.h"
#include "joy/sink.h"
#include "joy/screen.h"
#include "joy/source.h"
#include "joy/theme.h"
#include <locale.h>
#include <stdlib.h>

G_DEFINE_ABSTRACT_TYPE(JoyApplication, joy_application, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(self, JOY_TYPE_APPLICATION, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyApplication *)instance)->priv)

struct Private {
	JoySink *sink;
	JoyTheme *theme;
	gboolean initialized;
	gboolean quit;
	gchar *name;
	gint status;
	gdouble frame;
	gdouble min;
};

static void
joy_application_init(JoyApplication *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->sink = joy_sink_new();
	priv->status = EXIT_SUCCESS;
	priv->frame = 1. / 60.;
	priv->min = priv->frame / 3.;
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->sink) {
		g_object_unref(priv->sink);
		priv->sink = NULL;
	}
	if (priv->theme) {
		g_object_unref(priv->theme);
		priv->theme = NULL;
	}
	G_OBJECT_CLASS(joy_application_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->name);
	G_OBJECT_CLASS(joy_application_parent_class)->finalize(base);
}

static void
joy_application_class_init(JoyApplicationClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	g_type_class_add_private(klass, sizeof(struct Private));
}

/**
 * \brief Create a platform-specific application.
 *
 * This function creates all of the platform-specific structures. If this
 * function returns a non-NULL value then all of the platform specific
 * application APIs and screens have been initialized.
 *
 * \note Platform-specific backends must implement this function.
 *
 * \return A new application object.
 */
G_GNUC_INTERNAL
G_GNUC_WARN_UNUSED_RESULT
JoyApplication *
joy_platform_application_new(void);

JoyApplication *
joy_application_new(void)
{
	g_type_init();
	return joy_platform_application_new();
}

void
joy_application_set_name(JoyApplication *self, const gchar *name)
{
	g_return_if_fail(JOY_IS_APPLICATION(self));
	struct Private *priv = GET_PRIVATE(self);
	g_free(priv->name);
	priv->name = g_strdup(name);
}

const gchar *
joy_application_get_name(JoyApplication *self)
{
	g_return_val_if_fail(JOY_IS_APPLICATION(self), NULL);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->name) {
		return priv->name;
	} else {
		const gchar *name = g_get_application_name();
		if (name) {
			return name;
		}
		return G_OBJECT_TYPE_NAME(self);
	}
}

void
joy_application_set_theme(JoyApplication *self, JoyTheme *theme)
{
	g_return_if_fail(JOY_IS_APPLICATION(self));
	g_return_if_fail(JOY_IS_THEME(theme));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->theme) {
		g_object_unref(priv->theme);
	}
	priv->theme = g_object_ref(theme);
	if (priv->theme) {
		for (JoyIterator *iter = joy_application_begin(self); iter;
				iter = joy_iterator_next(iter)) {
			JoyScreen *screen = joy_iterator_item(iter);
			joy_screen_set_theme(screen, theme);
		}
	}
}

JoyTheme *
joy_application_get_theme(JoyApplication *self)
{
	g_return_val_if_fail(JOY_IS_APPLICATION(self), NULL);
	return GET_PRIVATE(self)->theme;
}

void
joy_application_set_refresh(JoyApplication *self, gdouble refresh)
{
	g_return_if_fail(JOY_IS_APPLICATION(self));
	struct Private *priv = GET_PRIVATE(self);
	priv->frame = 1. / refresh;
	priv->min = priv->frame / 3.;
}

static gboolean
arg_refresh_cb(const gchar *key, const gchar *value, gpointer self)
{
	errno = 0;
	double refresh = g_ascii_strtod(value, NULL);
	if (errno) {
		g_message(Q_("%s: not a valid refresh rate"), value);
		return FALSE;
	}
	joy_application_set_refresh(self, refresh);
	return TRUE;
}

static const GOptionEntry const joy_arguments[] = {
	{ "joybubbles-refresh", '\0', 0,
		G_OPTION_ARG_CALLBACK, arg_refresh_cb,
		N_("The refresh rate [60.0 Hz]"),
		N_("RATE") },
	{ NULL }
};

static gboolean
pre_hook(GOptionContext *context, GOptionGroup *group, gpointer data,
		GError **error)
{
	JoyApplication *self = JOY_APPLICATION(data);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->initialized) {
		return TRUE;
	}
	return TRUE;
}

static gboolean
post_hook(GOptionContext *context, GOptionGroup *group, gpointer data,
		GError **error)
{
	JoyApplication *self = JOY_APPLICATION(data);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->initialized) {
		return TRUE;
	}
	priv->initialized = TRUE;
	return TRUE;
}

static GOptionGroup *
get_option_group(JoyApplication *self)
{
	g_return_val_if_fail(JOY_IS_APPLICATION(self), NULL);
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->initialized) {
		setlocale(LC_ALL, "");
#ifdef ENABLE_NLS
		bindtextdomain(GETTEXT_PACKAGE, JOY_LOCALEDIR);
		bindtextdomain(GETTEXT_PACKAGE "-properties", JOY_LOCALEDIR);
#ifdef HAVE_BIND_TEXTDOMAIN_CODESET
		bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
		bind_textdomain_codeset(GETTEXT_PACKAGE
				"-properties", "UTF-8");
#endif // HAVE_BIND_TEXTDOMAIN_CODESET
#endif // ENABLE_NLS
	}
	GOptionGroup *group = g_option_group_new("joybubbles",
			Q_("Joybubbles Options"),
			Q_("Show Joybubbles Options"), self, NULL);
	if (!group) {
		return NULL;
	}
	g_option_group_set_parse_hooks(group, pre_hook, post_hook);
	g_option_group_add_entries(group, joy_arguments);
	g_option_group_set_translation_domain(group, GETTEXT_PACKAGE);
	return group;
}

void
joy_application_initialize(JoyApplication *self, int *argc, char ***argv)
{
	g_return_if_fail(JOY_IS_APPLICATION(self));
	struct Private *priv = GET_PRIVATE(self);
	GOptionContext *context = g_option_context_new(priv->name);
	if (G_UNLIKELY(!context)) {
		return;
	}
	g_option_context_set_ignore_unknown_options(context, TRUE);
	g_option_context_set_help_enabled(context, FALSE);
	GOptionGroup *group = get_option_group(self);
	if (G_LIKELY(group)) {
		g_option_context_set_main_group(context, group);
		GError *error = NULL;
		if (!g_option_context_parse(context, argc, argv, &error)) {
			g_error_free(error);
		}
	}
	g_option_context_free(context);
}

void
joy_application_add_options(JoyApplication *self,
		GOptionContext *context)
{
	g_return_if_fail(JOY_IS_APPLICATION(self));
	g_return_if_fail(context);
	GOptionGroup *group = get_option_group(self);
	if (G_LIKELY(group)) {
		g_option_context_add_group(context, group);
	}
}

JoyScreen *
joy_application_get_screen(JoyApplication *self, guint n)
{
	g_return_val_if_fail(JOY_IS_APPLICATION(self), NULL);
	return JOY_APPLICATION_GET_CLASS(self)->get_screen(self, n);
}

JoyIterator *
joy_application_begin(JoyApplication *self)
{
	g_return_val_if_fail(JOY_IS_APPLICATION(self), NULL);
	return JOY_APPLICATION_GET_CLASS(self)->begin(self);
}

void
joy_application_add_source(JoyApplication *self,
		JoySource *source)
{
	g_return_if_fail(JOY_IS_APPLICATION(self));
	joy_sink_add(GET_PRIVATE(self)->sink, source);
}

void
joy_application_remove_source(JoyApplication *self, JoySource *source)
{
	g_return_if_fail(JOY_IS_APPLICATION(self));
	joy_sink_remove(GET_PRIVATE(self)->sink, source);
}

void
joy_application_quit(JoyApplication *self, gint status)
{
	g_return_if_fail(JOY_IS_APPLICATION(self));
	struct Private *priv = GET_PRIVATE(self);
	if (EXIT_SUCCESS != status) {
		priv->status = EXIT_FAILURE;
	}
	priv->quit = TRUE;
}

JOY_GNUC_HOT
gint
joy_application_run(JoyApplication *self, JoyScreen *screen)
{
	g_return_val_if_fail(JOY_IS_APPLICATION(self), EXIT_FAILURE);
	g_return_val_if_fail(JOY_IS_SCREEN(screen), EXIT_FAILURE);
	struct Private *priv = GET_PRIVATE(self);
	GTimer *timer = g_timer_new();
	if (G_UNLIKELY(!timer)) {
		priv->status = EXIT_FAILURE;
		goto exit;
	}
	gdouble elapsed = 0.;
	gdouble ptime = priv->min;
	while (!priv->quit) {
		if (ptime + elapsed < priv->frame) {
			if (joy_screen_in_animation(screen)) {
				elapsed = joy_sink_poll(priv->sink,
						priv->frame - ptime);
			} else {
				elapsed = 0.;
				joy_sink_wait(priv->sink,
						priv->frame - priv->min);
			}
		} else {
			elapsed = 0.;
		}
		g_timer_start(timer);
		joy_screen_animate(screen);
		joy_screen_draw(screen);
		ptime = g_timer_elapsed(timer, NULL);
		if (ptime + elapsed < priv->frame) {
			joy_screen_submit(screen);
			if (ptime < priv->min) {
				ptime = priv->min;
			}
		}
	}
exit:
	if (timer) {
		g_timer_destroy(timer);
	}
	gint status = priv->status;
	g_object_run_dispose(G_OBJECT(self));
	g_object_unref(self);
	return status;
}

#ifdef JOY_TRACE
#ifdef HAVE_BFD
#include <bfd.h>
#else // HAVE_BFD
#ifdef HAVE_DLADDR
#include <dlfcn.h>
#endif // HAVE_DLADDR
#endif // HAVE_BFD
G_GNUC_NO_INSTRUMENT
void
__cyg_profile_func_enter(void *this_fn, void *call_site)
{
#ifdef HAVE_BFD
	g_print(Q_("[TRACE] enter [%p] (TODO libbfd tracing)\n"), this_fn);
#else // HAVE_BFD
#ifdef HAVE_DLADDR
	Dl_info info;
	if (dladdr(this_fn, &info) && info.dli_sname) {
		g_print(Q_("[TRACE] enter %s [%p]\n"),
				info.dli_sname, this_fn);
	} else {
		g_print(Q_("[TRACE] enter [%p]\n"), this_fn);
	}
#else // HAVE_DLADDR
	g_print(Q_("[TRACE] enter [%p]\n"), this_fn);
#endif // HAVE_DLADDR
#endif // HAVE_BFD
}

G_GNUC_NO_INSTRUMENT
void
__cyg_profile_func_exit(void *this_fn, void *call_site)
{
#ifdef HAVE_BFD
	g_print(Q_("[TRACE] exit [%p] (TODO libbfd tracing)\n"), this_fn);
#else // HAVE_BFD
#ifdef HAVE_DLADDR
	Dl_info info;
	if (dladdr(this_fn, &info) && info.dli_sname) {
		g_print(Q_("[TRACE] exit %s [%p]\n"),
				info.dli_sname, this_fn);
	} else {
		g_print(Q_("[TRACE] exit [%p]\n"), this_fn);
	}
#else // HAVE_DLADDR
	g_print(Q_("[TRACE] exit [%p]\n"), this_fn);
#endif // HAVE_DLADDR
#endif // HAVE_BFD
}
#endif // JOY_TRACE
