
#ifndef __joy_marshal_MARSHAL_H__
#define __joy_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* VOID:VOID (../../joy/marshal.list:1) */
#define joy_marshal_VOID__VOID	g_cclosure_marshal_VOID__VOID

/* VOID:POINTER (../../joy/marshal.list:2) */
#define joy_marshal_VOID__POINTER	g_cclosure_marshal_VOID__POINTER

/* VOID:INT (../../joy/marshal.list:3) */
#define joy_marshal_VOID__INT	g_cclosure_marshal_VOID__INT

/* VOID:OBJECT,ULONG,INT,INT,INT (../../joy/marshal.list:4) */
extern void joy_marshal_VOID__OBJECT_ULONG_INT_INT_INT (GClosure     *closure,
                                                        GValue       *return_value,
                                                        guint         n_param_values,
                                                        const GValue *param_values,
                                                        gpointer      invocation_hint,
                                                        gpointer      marshal_data);

/* VOID:OBJECT,ULONG,INT,INT (../../joy/marshal.list:5) */
extern void joy_marshal_VOID__OBJECT_ULONG_INT_INT (GClosure     *closure,
                                                    GValue       *return_value,
                                                    guint         n_param_values,
                                                    const GValue *param_values,
                                                    gpointer      invocation_hint,
                                                    gpointer      marshal_data);

/* VOID:OBJECT (../../joy/marshal.list:6) */
#define joy_marshal_VOID__OBJECT	g_cclosure_marshal_VOID__OBJECT

/* VOID:DOUBLE (../../joy/marshal.list:7) */
#define joy_marshal_VOID__DOUBLE	g_cclosure_marshal_VOID__DOUBLE

/* BOOLEAN:VOID (../../joy/marshal.list:8) */
extern void joy_marshal_BOOLEAN__VOID (GClosure     *closure,
                                       GValue       *return_value,
                                       guint         n_param_values,
                                       const GValue *param_values,
                                       gpointer      invocation_hint,
                                       gpointer      marshal_data);

/* VOID:INT,INT (../../joy/marshal.list:9) */
extern void joy_marshal_VOID__INT_INT (GClosure     *closure,
                                       GValue       *return_value,
                                       guint         n_param_values,
                                       const GValue *param_values,
                                       gpointer      invocation_hint,
                                       gpointer      marshal_data);

/* VOID:OBJECT,DOUBLE (../../joy/marshal.list:10) */
extern void joy_marshal_VOID__OBJECT_DOUBLE (GClosure     *closure,
                                             GValue       *return_value,
                                             guint         n_param_values,
                                             const GValue *param_values,
                                             gpointer      invocation_hint,
                                             gpointer      marshal_data);

/* BOOLEAN:POINTER (../../joy/marshal.list:11) */
extern void joy_marshal_BOOLEAN__POINTER (GClosure     *closure,
                                          GValue       *return_value,
                                          guint         n_param_values,
                                          const GValue *param_values,
                                          gpointer      invocation_hint,
                                          gpointer      marshal_data);

G_END_DECLS

#endif /* __joy_marshal_MARSHAL_H__ */

