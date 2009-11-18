/*
 wdb

 Copyright (C) 2009 met.no

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

#ifndef GRIBLOADER_H_
#define GRIBLOADER_H_

/**
 * @addtogroup loader
 * @{
 * @addtogroup gribload
 * @{
 */

/**
 * @file
 * This file contains the definition of the GribLoader class.
 */

// PROJECT INCLUDES
#include <wdb/WdbLevel.h>
#include <WdbConfigFile.h>
#include <wdb/LoaderConfiguration.h>
#include <wdb/LoaderDatabaseConnection.h>

// SYSTEM INCLUDES
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <vector>

namespace wdb
{

// FORWARD REFERENCES
class WdbLogHandler;

namespace grib
{

// FORWARD REFERENCES
class GribFile;
class GribField;

/**
 * GribLoader handles all the nitty gritty of loading a GribField
 * into the database. It extracts GRIB parameters from GribField,
 * performs the required cross-referencing to get the WCI metadata
 * and inserts the data into the database.
 *
 * @see GribField
 */
class GribLoader
{
public:
	/** Default Constructor
	 * @param	connection		The database connection
	 * @param	loadingOptions	The options to load the database
	 * @param	logHandler		Reference to the loghandler
	 */
	GribLoader( wdb::load::LoaderDatabaseConnection & connection,
				const wdb::load::LoaderConfiguration & loadingOptions,
				wdb::WdbLogHandler & logHandler );
	/// Destructor
	~GribLoader( );

	/**
	 * Load an entire GRIB file into wdb. This function will not fail if there
	 * are errors in the provided file. Instead an error count will be returned
	 *
	 * @throw exception if the given file was empty
	 * @param gribFile the file to load into a wdb database
	 * @return the number of inserts that _failed_
	 */
	int load( GribFile & gribFile );

	/** Load a given GRIB field into the database
	 * @param	field		The GRIB field to be loaded
	 */
	void load( const GribField & field, int fieldNumber = 0 );

private:

	/** Retrieve the data provider name of the GRIB field
	 * @param	field	The GRIB field to be loaded
	 * @returns			The metadata string
	 */
	std::string dataProviderName( const GribField & field ) const;
	/** Retrieve the place name of the GRIB field
	 * @param	field	The GRIB field to be loaded
	 * @returns			The metadata string
	 */
	std::string placeName( const GribField & field ) const;
	/** Retrieve the value parameter name of the GRIB field
	 * @param	field	The GRIB field to be loaded
	 * @returns			The metadata string
	 */
	std::string valueParameterName( const GribField & field ) const;
	/** Retrieve the value parameter unit of the GRIB field
	 * @param	field	The GRIB field to be loaded
	 * @returns			The metadata string
	 */
	std::string valueParameterUnit( const GribField & field ) const;
	/** Retrieve the list of levels in he GRIB field
	 *  Unlike with parameters, we can encode more than one level
	 *  into a field.
	 * @param	levels	The levels of the GRIB field
	 * @param	field	The GRIB field to be loaded
	 */
	void levelValues( std::vector<wdb::load::Level> & levels, const GribField & field ) const;
	/** Retrieve the data version of the GRIB field
	 * @param	field	The GRIB field to be loaded
	 * @returns			The metadata string
	 */
	int dataVersion(const GribField & field) const;
	/** Retrieve the confidence code of the GRIB field
	 * @param	field	The GRIB field to be loaded
	 * @returns			The metadata string
	 */
	int confidenceCode(const GribField & field) const;
	/**
	 * Convert the values_ depending on the value parameter unit
	 */
    void convertValues( const GribField & field, std::vector<double> & valuesInOut ) const;

private:
	/// The Database Connection
	wdb::load::LoaderDatabaseConnection & connection_;
	/// The GribLoad Configuration
	const wdb::load::LoaderConfiguration & loadingConfiguration_;
	/// The GribLoad Logger
	wdb::WdbLogHandler & logHandler_;

	/// Conversion Hash Map - Dataprovider Name
	wdb::WdbConfigFile Grib2DataProviderName_;
	/// Conversion Hash Map - Value Parameter
	wdb::WdbConfigFile Grib2ValueParameter_;
	/// Conversion Hash Map - Level Parameter
	wdb::WdbConfigFile Grib2LevelParameter_;
	/// Conversion Hash Map - Level Additions
	wdb::WdbConfigFile Grib2LevelAdditions_;

};

} // grib

} // wdb

#endif /*GRIBLOADER_H_*/
