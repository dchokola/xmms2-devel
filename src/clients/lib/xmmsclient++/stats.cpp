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

#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient++/client.h>
#include <xmmsclient/xmmsclient++/stats.h>
#include <xmmsclient/xmmsclient++/signal.h>
#include <xmmsclient/xmmsclient++/helpers.h>
#include <xmmsclient/xmmsclient++/mainloop.h>
#include <xmmsclient/xmmsclient++/typedefs.h>

#include <boost/bind.hpp>

#include <list>

namespace Xmms
{
	
	Stats::~Stats()
	{
	}

	const Dict Stats::mainStats() const
	{

		xmmsc_result_t* res = 
		    call( connected_, ml_,
		          boost::bind( xmmsc_main_stats, conn_ ) );

		Dict resultMap( res );

		xmmsc_result_unref( res );
		return resultMap;

	}

	const DictList Stats::pluginList(Plugins::Type type) const
	{

		xmmsc_result_t* res = 
		    call( connected_, ml_,
		          boost::bind( xmmsc_plugin_list, conn_, type ) ); 	
		
		List< Dict > resultList( res );

		xmmsc_result_unref( res );
		return resultList;

	}

	void
	Stats::mainStats( const DictSlot& slot,
	                  const ErrorSlot& error ) const
	{

		aCall<Dict>( connected_, boost::bind( xmmsc_main_stats, conn_ ), 
		             slot, error );
	}

	void
	Stats::pluginList(Plugins::Type type,
	                   const DictListSlot& slot,
	                   const ErrorSlot& error ) const
	{
		aCall<DictList>( connected_, 
		                 boost::bind( xmmsc_plugin_list, conn_, type ),
		                 slot, error );
	}

	void 
	Stats::pluginList( const DictListSlot& slot,
	                   const ErrorSlot& error ) const
	{
		pluginList( Plugins::ALL, slot, error );
	}

	void
	Stats::signalVisualisationData(const UintListSlot& slot,
	                               const ErrorSlot& error ) const
	{
		using boost::bind;
		aCall<List<unsigned int> >( connected_,
		                            bind( xmmsc_signal_visualisation_data,
		                                  conn_ ), 
		                            slot, error );
	}

	void
	Stats::broadcastMediainfoReaderStatus(const ReaderStatusSlot& slot,
	                                      const ErrorSlot& error ) const
	{
		using boost::bind;
		aCall<ReaderStatus>( connected_,
		                     bind( xmmsc_broadcast_mediainfo_reader_status,
		                           conn_),
		                     slot, error );
	}

	void
	Stats::signalMediainfoReaderUnindexed(const UintSlot& slot,
	                                      const ErrorSlot& error ) const
	{
		using boost::bind;
		aCall<unsigned int>( connected_,
		                     bind( xmmsc_signal_mediainfo_reader_unindexed,
		                           conn_),
		                     slot, error );
	}

	Stats::Stats( xmmsc_connection_t*& conn, bool& connected,
	              MainloopInterface*& ml ) :
		conn_( conn ), connected_( connected ), ml_( ml )
	{
	}

}
