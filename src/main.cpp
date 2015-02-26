/*
    wdb - weather and water data storage

    Copyright (C) 2007 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: wdb@met.no

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA  02110-1301, USA
*/


/**
 * @addtogroup loader
 * @{
 * @addtogroup gribload
 * @{
 */
/** @file
 * GribLoad is a loading program for storing the contents of GRIB
 * fields in WDB.
 *
 * The main program opens a GRIB file, decodes each field sequentially
 * using the grib_api in the class GribField, and then stores the
 * contents of the field in the database.
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// PROJECT INCLUDES
#include "GribFile.h"
#include "GribLoader.h"

#include <wdbException.h>
#include <wdb/errors.h>
#include <wdb/LoaderDatabaseConnection.h>
#include <wdb/LoaderConfiguration.h>
#include <wdbLogHandler.h>
// SYSTEM INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/operations.hpp>

using namespace std;
using namespace wdb;
using namespace wdb::grib;


// Support Functions
namespace
{
/**
 * Write the program version to stream
 * @param	out		Stream to write to
 */
void version( ostream & out )
{
	out << "gribLoad (" << PACKAGE << ") " << VERSION << endl;
}

/**
 * Write help information to stram
 * @param	options		Description of the program options
 * @param	out			Stream to write to
 */
void help( const boost::program_options::options_description & options, ostream & out )
{
	version( out );
	out << '\n';
    out << "Usage: gribLoad [OPTIONS] GRIBFILES...\n\n";
    out << "Options:\n";
    out << options << endl;
}

} // namespace

/**
 * Main Program
 * @param	argc	Command line parameter count
 * @param	argv	Command line parameters
 * @return	Exit status
 */
int
main(int argc, char **argv)
{
	wdb::load::LoaderConfiguration conf;
	try
    {
    	conf.parse( argc, argv );
	} catch (wdb::load::LoadError & e)
	{
		std::clog << e.what() << std::endl;
		return wdb::load::exitStatus();
	}
	if ( conf.general().help )
	{
		help( conf.shownOptions(), cout );
		return 0;
	}
	if ( conf.general().version )
	{
		version( cout );
		return 0;
	}
	if ( conf.input().file.empty() )
	{
		cerr << "No inputfile." << endl;
		return 1;
	}

	WdbLogHandler logHandler( conf.logging().loglevel, conf.logging().logfile );
    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.main" );

    log.debug( "Starting gribLoad" );

    try
    {

		// File sanity check
		const std::vector<std::string> & filesToLoad = conf.input().file;
		for ( std::vector<std::string>::const_iterator it = filesToLoad.begin(); it != filesToLoad.end(); ++ it )
		{
			using namespace boost::filesystem;
			const path p(*it);
			if ( (! exists(p)) || is_directory(p) )
				throw wdb::load::LoadError(wdb::load::UnableToReadFile, * it + ": No such file - will not load");
		}

		// Database Connection
		log.debugStream() << "Attempting to connect to database " << conf.pqDatabaseConnection();
	//   	wdb::load::LoaderDatabaseConnection connection( conf.pqDatabaseConnection(), conf.database().user );
		wdb::load::LoaderDatabaseConnection connection( conf );
		log.debugStream() << "...connected";

		GribLoader loader( connection, conf, logHandler );

		for ( std::vector<std::string>::const_iterator file = filesToLoad.begin(); file != filesToLoad.end(); ++ file )
		{
			logHandler.setObjectName( * file );
			GribFile gribFile(* file);
			loader.load(gribFile);
		}

		log.debugStream() << "Done";
    }
	catch ( wdb::load::LoadError & e )
	{
		log.fatal(e.what());
		return wdb::load::exitStatus();
	}
	catch ( std::exception & e )
	{
		log.fatalStream() << e.what();
		return int(wdb::load::UnknownError);
	}

	return wdb::load::exitStatus();
}

/**
 * @}
 * @}
 */
