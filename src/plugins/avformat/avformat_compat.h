/** @file avformat.h
 *  Decoder plugin for ffmpeg avcodec formats
 *
 *  Copyright (C) 2006-2017 XMMS2 Team
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

#undef ABS
#ifdef HAVE_LIBAVFORMAT_AVFORMAT_H
# include <libavformat/avformat.h>
#else
# include <avformat.h>
#endif

#ifdef HAVE_AVFORMAT_CLOSE_INPUT
# define av_close_input_file(a) ( avformat_close_input(&a) )
#endif
