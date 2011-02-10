/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief X11 application implementation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_X11_APPLICATION_H
#define JOY_X11_APPLICATION_H

#include "joy/application.h"
#include "joy/platform/x11/types.h"
#include <X11/Xlib.h>

G_BEGIN_DECLS

#define JOY_TYPE_X11_APPLICATION \
	(joy_x11_application_get_type())

#define JOY_X11_APPLICATION(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_X11_APPLICATION, \
		JoyX11Application))

#define JOY_IS_X11_APPLICATION(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_X11_APPLICATION))

#define JOY_X11_APPLICATION_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_X11_APPLICATION, \
		JoyX11ApplicationClass))

#define JOY_IS_X11_APPLICATION_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_X11_APPLICATION))

#define JOY_X11_APPLICATION_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_X11_APPLICATION, \
		JoyX11ApplicationClass))

typedef struct JoyX11ApplicationClass_ JoyX11ApplicationClass;

struct JoyX11Application_ {
	/*< private >*/
	JoyApplication parent_instance;
	gpointer priv;
};

struct JoyX11ApplicationClass_ {
	/*< private >*/
	JoyApplicationClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_x11_application_get_type(void) G_GNUC_CONST;

/**
 * \brief Get the X11 display.
 *
 * \param self [in] An X11 application object.
 *
 * \return The X11 Display for \e self.
 */
G_GNUC_INTERNAL
Display *
joy_x11_application_get_display(JoyApplication *self);

/**
 * \brief Insert an XID into the object map.
 *
 * \param self [in] An X11 application object.
 * \param id [in] The XID to map to \e data.
 * \param data [in] The object to map to \e id.
 */
G_GNUC_INTERNAL
void
joy_x11_application_insert_xid(JoyApplication *self, XID id, gpointer data);

/**
 * \brief Remove an XID from the object map.
 *
 * \param self [in] An X11 application object.
 * \param id [in] The XID mapping to remove.
 */
G_GNUC_INTERNAL
void
joy_x11_application_remove_xid(JoyApplication *self, XID id);

/**
 * \brief Lookup an XID in the object map.
 *
 * \param self [in] An X11 application object.
 * \param id [in] The XID to look up.
 *
 * \return The object mapped to \e id.
 */
G_GNUC_INTERNAL
gpointer
joy_x11_application_lookup_xid(JoyApplication *self, XID id);

G_GNUC_INTERNAL
JoyDevice *
joy_x11_application_get_mouse(JoyApplication *self);

G_GNUC_INTERNAL
JoyDevice *
joy_x11_application_get_keyboard(JoyApplication *self);

G_END_DECLS

#endif // JOY_X11_APPLICATION_H
