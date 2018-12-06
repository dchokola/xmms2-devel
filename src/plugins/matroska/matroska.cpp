/** @file matroska.cpp
 *  Demuxer plugin for Matroska and related audio formats
 *  Copyright (C) 2003-2018 XMMS2 Team
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

#include <xmms/xmms_xformplugin.h>
#include <xmms/xmms_bindata.h>
#include <xmms/xmms_sample.h>
#include <xmms/xmms_log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <glib.h>

/* Who names header files like this? */
#include <matroska/FileKax.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	matroska_id id;
} xmms_mka_data_t;

static const xmms_xform_metadata_basic_mapping_t basic_mappings[] = {
	{ "WM/AlbumTitle",               XMMS_MEDIALIB_ENTRY_PROPERTY_ALBUM             },
	{ "WM/AlbumArtist",              XMMS_MEDIALIB_ENTRY_PROPERTY_ALBUM_ARTIST      },
	{ "WM/OriginalArtist",           XMMS_MEDIALIB_ENTRY_PROPERTY_ORIGINAL_ARTIST   },
	{ "WM/Year",                     XMMS_MEDIALIB_ENTRY_PROPERTY_YEAR              },
	{ "WM/OriginalReleaseYear",      XMMS_MEDIALIB_ENTRY_PROPERTY_ORIGINALYEAR      },
	{ "WM/Composer",                 XMMS_MEDIALIB_ENTRY_PROPERTY_COMPOSER          },
	{ "WM/Writer",                   XMMS_MEDIALIB_ENTRY_PROPERTY_LYRICIST          },
	{ "WM/Conductor",                XMMS_MEDIALIB_ENTRY_PROPERTY_CONDUCTOR         },
	{ "WM/ModifiedBy",               XMMS_MEDIALIB_ENTRY_PROPERTY_REMIXER           },
	{ "WM/Producer",                 XMMS_MEDIALIB_ENTRY_PROPERTY_PRODUCER          },
	{ "WM/ContentGroupDescription",  XMMS_MEDIALIB_ENTRY_PROPERTY_GROUPING          },
	{ "WM/TrackNumber",              XMMS_MEDIALIB_ENTRY_PROPERTY_TRACKNR           },
	{ "WM/PartOfSet",                XMMS_MEDIALIB_ENTRY_PROPERTY_PARTOFSET         },
	{ "WM/Genre",                    XMMS_MEDIALIB_ENTRY_PROPERTY_GENRE             },
	{ "WM/BeatsPerMinute",           XMMS_MEDIALIB_ENTRY_PROPERTY_BPM               },
	{ "WM/ISRC",                     XMMS_MEDIALIB_ENTRY_PROPERTY_ISRC              },
	{ "WM/Publisher",                XMMS_MEDIALIB_ENTRY_PROPERTY_PUBLISHER         },
	{ "WM/CatalogNo",                XMMS_MEDIALIB_ENTRY_PROPERTY_CATALOGNUMBER     },
	{ "WM/Barcode",                  XMMS_MEDIALIB_ENTRY_PROPERTY_BARCODE           },
	{ "WM/AlbumSortOrder",           XMMS_MEDIALIB_ENTRY_PROPERTY_ALBUM_SORT        },
	{ "WM/AlbumArtistSortOrder",     XMMS_MEDIALIB_ENTRY_PROPERTY_ALBUM_ARTIST_SORT },
	{ "WM/ArtistSortOrder",          XMMS_MEDIALIB_ENTRY_PROPERTY_ARTIST_SORT       },
	{ "Year",                        XMMS_MEDIALIB_ENTRY_PROPERTY_YEAR              },
	{ "Copyright",                   XMMS_MEDIALIB_ENTRY_PROPERTY_COPYRIGHT         },
	{ "Station",                     XMMS_MEDIALIB_ENTRY_PROPERTY_CHANNEL           },
	{ "MusicBrainz/Album Id",        XMMS_MEDIALIB_ENTRY_PROPERTY_ALBUM_ID          },
	{ "MusicBrainz/Artist Id",       XMMS_MEDIALIB_ENTRY_PROPERTY_ARTIST_ID         },
	{ "MusicBrainz/Track Id",        XMMS_MEDIALIB_ENTRY_PROPERTY_TRACK_ID          },
	{ "MusicBrainz/Album Artist Id", XMMS_MEDIALIB_ENTRY_PROPERTY_COMPILATION       },
};

static gboolean xmms_mka_plugin_setup (xmms_xform_plugin_t *xform_plugin);
static gboolean xmms_mka_init (xmms_xform_t *xform);
static void xmms_mka_destroy (xmms_xform_t *xform);
static gint xmms_mka_read (xmms_xform_t *xform, xmms_sample_t *buf,
                           gint len, xmms_error_t *err);
static gint64 xmms_mka_seek (xmms_xform_t *xform, gint64 samples,
                             xmms_xform_seek_mode_t whence, xmms_error_t *err);

XMMS_XFORM_PLUGIN_DEFINE ("matroska",
                          "Matroska Demuxer",
                           XMMS_VERSION,
                          "Demultiplexer for Matroska/WebM containers",
                          xmms_mka_plugin_setup);

static gboolean
xmms_mka_plugin_setup (xmms_xform_plugin_t *xform_plugin)
{
	xmms_xform_methods_t methods;

	XMMS_XFORM_METHODS_INIT (methods);

	methods.init = xmms_mka_init;
	methods.destroy = xmms_mka_destroy;
	methods.read = xmms_mka_read;
	methods.seek = xmms_mka_seek;

	xmms_xform_plugin_methods_set (xform_plugin, &methods);

	xmms_xform_plugin_metadata_basic_mapper_init (xform_plugin,
	                                              basic_mappings,
	                                              G_N_ELEMENTS (basic_mappings));

	xmms_xform_plugin_indata_add (xform_plugin,
	                              XMMS_STREAM_TYPE_MIMETYPE,
	                              "video/webm",
	                              "audio/webm",
	                              "video/x-matroska",
	                              "audio/x-matroska",
	                              "video/x-matroska-3d",
	                              XMMS_STREAM_TYPE_END);

	xmms_magic_add ("matroska header", "video/x-matroska",
	                "0 belong 0x1a45dfa3", NULL);

	return TRUE;
}

static gboolean
xmms_mka_init (xmms_xform_t *xform)
{
	xmms_mka_data_t *data;

	g_return_val_if_fail (xform, FALSE);

	data = g_try_new0 (xmms_mka_data_t, 1);
	if(!data) {
		return FALSE;
	}

	/* fill data with important stuff */

	xmms_xform_private_data_set (xform, data);

	matroska_init();

/*
	ret = asf_init (data->file);
	if (ret < 0) {
		XMMS_DBG ("ASF parser failed to init with error %d", ret);
		asf_packet_destroy (data->packet);
		asf_close (data->file);

		return FALSE;
	}

	data->track = xmms_asf_get_track (xform, data->file);
	if (data->track < 0) {
		XMMS_DBG ("No audio track found");
		asf_packet_destroy (data->packet);
		asf_close (data->file);

		return FALSE;
	}

	xmms_asf_get_mediainfo (xform);
*/

	XMMS_DBG ("Matroska demuxer initialized successfully!");

	return TRUE;
}

static void
xmms_mka_destroy (xmms_xform_t *xform)
{
	xmms_mka_data_t *data;

	matroska_done();

	g_return_if_fail (xform);

	data = (xmms_mka_data_t *) xmms_xform_private_data_get (xform);
	g_return_if_fail (data);

	g_free (data);
}


static gint
xmms_mka_read (xmms_xform_t *xform, xmms_sample_t *buf, gint len,
               xmms_error_t *err)
{
	return 0;
}

static gint64
xmms_mka_seek (xmms_xform_t *xform, gint64 samples,
               xmms_xform_seek_mode_t whence, xmms_error_t *err)
{
	return 0;
}


#ifdef __cplusplus
}
#endif
