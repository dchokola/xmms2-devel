/*  XMMS2 - X Music Multiplexer System
 *  Copyright (C) 2003-2007 XMMS2 Team
 *
 *  PLUGINS ARE NOT CONSIDERED TO BE DERIVED WORK !!!
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */




#ifndef __XMMS_PRIV_CONFIG_H__
#define __XMMS_PRIV_CONFIG_H__

#include "xmms/xmms_config.h"

#define XMMS_MAX_CONFIGFILE_LEN 255
#define XMMS_CONFIG_SYSTEMWIDE SYSCONFDIR "/xmms2.conf"

void xmms_config_init (const gchar *filename);
void xmms_config_shutdown (void);

gboolean xmms_config_save (const gchar *file);
void xmms_config_setvalue (xmms_config_t *conf, gchar *key, const gchar *value, xmms_error_t *err); 

#endif
