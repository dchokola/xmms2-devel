#ifndef __XMMS_CONFIG_H__
#define __XMMS_CONFIG_H__

#include "xmms/object.h"

#define XMMS_MAX_CONFIGFILE_LEN 255
#define XMMS_CONFIG_SYSTEMWIDE SYSCONFDIR "/xmms2.conf"

typedef struct xmms_config_St xmms_config_t;
typedef struct xmms_config_value_St xmms_config_value_t;

gboolean xmms_config_init (const gchar *filename);
void xmms_config_free (void);
xmms_config_value_t *xmms_config_lookup (const gchar *path);
gboolean xmms_config_save (const gchar *file);
GList *xmms_config_plugins_get (void);

xmms_config_value_t *xmms_config_value_new (const gchar *name);
void xmms_config_value_free (xmms_config_value_t *value);
void xmms_config_value_data_set (xmms_config_value_t *val, gchar *data);
const gchar *xmms_config_value_string_get (const xmms_config_value_t *val);
gint xmms_config_value_int_get (const xmms_config_value_t *val);
gfloat xmms_config_value_float_get (const xmms_config_value_t *val);
const gchar *xmms_config_value_name_get (const xmms_config_value_t *value);

xmms_config_value_t *xmms_config_value_register (const gchar *path, const gchar *default_value, xmms_object_handler_t cb, gpointer userdata);

void xmms_config_value_callback_set (xmms_config_value_t *val, xmms_object_handler_t cb, gpointer userdata);

#endif
