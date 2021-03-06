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
 * \brief Application interface
 */

#ifndef JOY_APPLICATION_H
#define JOY_APPLICATION_H

#include <glib-object.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_APPLICATION \
	(joy_application_get_type())

#define JOY_APPLICATION(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_APPLICATION, \
		JoyApplication))

#define JOY_IS_APPLICATION(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_APPLICATION))

#define JOY_APPLICATION_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_APPLICATION, \
		JoyApplicationClass))

#define JOY_IS_APPLICATION_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_APPLICATION))

#define JOY_APPLICATION_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_APPLICATION, \
		JoyApplicationClass))

typedef struct JoyApplicationClass_ JoyApplicationClass;

struct JoyApplication_ {
	/*< private >*/
	GObject parent_instance;
	gpointer priv;
};

typedef JoyScreen *
(*JoyApplicationGetScreen)(JoyApplication *self, guint n);

typedef JoyScreen *
(*JoyApplicationGetDefaultScreen)(JoyApplication *self);

typedef JoyIterator *
(*JoyApplicationBegin)(JoyApplication *self);

typedef void
(*JoyApplicationAddOptions)(JoyApplication *self, GOptionContext *context);

struct JoyApplicationClass_ {
	/*< private >*/
	GObjectClass parent_class;
	/*< public >*/
	JoyApplicationGetScreen get_screen;
	JoyApplicationGetDefaultScreen get_default_screen;
	JoyApplicationBegin begin;
	JoyApplicationAddOptions add_options;
};

G_GNUC_NO_INSTRUMENT
GType
joy_application_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new application instance.
 *
 * \return A new application or NULL if an error occurred. The caller owns
 *         the return value and must free memory by calling g_object_unref()
 *         or joy_application_run().
 */
G_GNUC_WARN_UNUSED_RESULT
JoyApplication *
joy_application_new(void);

/**
 * \brief Set the application name.
 *
 * \param self [in] An application object.
 * \param name [in] The new application name.
 */
void
joy_application_set_name(JoyApplication *self, const gchar *name);

/**
 * \brief Get the application name.
 *
 * \param self [in] An application object.
 *
 * \return The application name.
 */
const gchar *
joy_application_get_name(JoyApplication *self);

/**
 * \brief Set the application theme.
 *
 * \param self [in] An application object.
 * \param theme [in] A theme object.
 */
void
joy_application_set_theme(JoyApplication *self, JoyStyle *theme);

/**
 * \brief Get the application theme.
 *
 * \param self [in] An application object.
 *
 * \return The theme set for \e self (may be NULL).
 */
JoyStyle *
joy_application_get_theme(JoyApplication *self);

/**
 * \brief Initialize an application.
 *
 * For an alternative way to initialize the application, see
 * joy_application_add_options().
 *
 * \param self [in] An application object.
 * \param argc [in/out] A pointer to the number of command line options.
 * \param argv [in/out] A pointer to the command line option vector.
 *
 * \note Any options understood by \e self will be removed from \e argv.
 *       The value of \e argc will be updated to reflect any removed options.
 */
void
joy_application_initialize(JoyApplication *self, int *argc, char ***argv);

/**
 * \brief Add options to a GLib option context.
 *
 * After adding Joy options to a GLib option context and calling
 * g_option_context_parse() the application will be initialized.
 *
 * For an alternative way to initialize the application, see
 * joy_application_initialize().
 *
 * \param self [in] An application object.
 * \param context [in] A GLib option context.
 */
void
joy_application_add_options(JoyApplication *self, GOptionContext *context);

/**
 * \brief Get the specified screen.
 *
 * \param self [in] An application object.
 * \param n [in] The ID of the desired screen.
 *
 * \return The requested screen or NULL if no such screen exists. The
 *         screen ID zero is always valid and must return a non-NULL value.
 */
JoyScreen *
joy_application_get_screen(JoyApplication *self, guint n);

JoyScreen *
joy_application_get_default_screen(JoyApplication *self);

/**
 * \brief Iterate over screens for an application.
 *
 * \param self [in] An application object.
 *
 * \return An iterator starting with the first screen of \e self.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_application_begin(JoyApplication *self);

/**
 * \brief Add an input source to the application main loop.
 *
 * \param self [in] An application object.
 * \param source [in] A file descriptor.
 */
void
joy_application_add_source(JoyApplication *self, JoySource *source);

/**
 * \brief Remove an input source from the application main loop.
 *
 * \param self [in] An application object.
 * \param source [in] A file descriptor.
 */
void
joy_application_remove_source(JoyApplication *self, JoySource *source);

/**
 * \brief Force the application main loop to quit on the next iteration.
 *
 * \param self [in] An application object.
 * \param status [in] The application exit status, i.e., EXIT_SUCCESS or
 *                    EXIT_FAILURE.
 */
void
joy_application_quit(JoyApplication *self, gint status);

/**
 * \brief Run an application on the specified screen.
 *
 * After this function returns the application has been destroyed and
 * should no longer be referenced.
 *
 * \param self [in] An application object.
 * \param screen [in] The screen to run this application on.
 *
 * \return The application status, i.e., EXIT_SUCCESS or EXIT_FAILURE.
 */
gint
joy_application_run(JoyApplication *self, JoyScreen *screen);

G_END_DECLS

#endif // JOY_APPLICATION_H
