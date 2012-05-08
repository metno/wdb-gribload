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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "GribLoader.h"
#include "GribFile.h"
#include "GribField.h"
#include <wdbException.h>
#include <wdbLogHandler.h>
#include <wdb/LoaderDatabaseConnection.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace wdb;
using namespace wdb::load;
using namespace boost::posix_time;
using namespace boost::filesystem;

namespace {

path getConfigFile( const path & sysPath, const path & fileName )
{
	path confPath;
	if (sysPath.empty())
		confPath = SYSCONFDIR/fileName;
	else
		confPath = sysPath/fileName;
	return confPath;
}

}

namespace wdb
{

namespace grib
{

GribLoader::GribLoader(	LoaderDatabaseConnection & connection,
						const LoaderConfiguration & loadingOptions,
						WdbLogHandler & logHandler )
	: connection_(connection),
	  loadingConfiguration_(loadingOptions),
	  logHandler_(logHandler)
{
	Grib2DataProviderName_.open( getConfigFile(loadingOptions.metadata().path, "dataprovider.conf").string() );
	Grib2ValueParameter1_.open( getConfigFile(loadingOptions.metadata().path, "valueparameter1.conf").string() );
	Grib2LevelParameter1_.open( getConfigFile(loadingOptions.metadata().path, "levelparameter1.conf").string() );
	Grib2LevelAdditions1_.open( getConfigFile(loadingOptions.metadata().path, "leveladditions1.conf").string() );
	Grib2ValueParameter2_.open( getConfigFile(loadingOptions.metadata().path, "valueparameter2.conf").string() );
	Grib2LevelParameter2_.open( getConfigFile(loadingOptions.metadata().path, "levelparameter2.conf").string() );
	Grib2LevelAdditions2_.open( getConfigFile(loadingOptions.metadata().path, "leveladditions2.conf").string() );
}

GribLoader::~GribLoader()
{
	// NOOP
}

int GribLoader::load( GribFile & gribFile )
{
	{
	    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.load" );

	    // Get first field, and check if it exists
	    wdb::grib::GribFile::Field gribField = gribFile.next();
	    if ( ! gribField )
	    {
			// If the file is empty, we need to throw an error (Test Case 2)
	        std::string errorMessage = "End of file was hit before a product was read into file ";
	        errorMessage += gribFile.fileName();
	        throw std::runtime_error( errorMessage );
		}

		// Field Number Count, for logging
	    int fieldNumber = -1;

	    int failCount = 0;

	    // The actual loading
	    for ( ; gribField; gribField = gribFile.next() )
	    {
	        try
	        {
	            load( * gribField, ++ fieldNumber );
	        }
	        catch (wdb::wdb_exception & e)
	        {
	        	++ failCount;

	        	// We don't stop loading on errors. Merely log the error, and go to the next field
	            WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.readgrib" );
	            std::ostringstream errorMsg;
	            errorMsg << e.what() << ". File: " << gribFile.fileName() << " - Field no #" << fieldNumber;
	            log.warnStream() << errorMsg.str();
	        }
		}

	    log.debugStream() << "End of Grib File";

	    // Return the count of fields that we were unable to load into wdb
	    return failCount;
	}
}


void GribLoader::load( const GribField & field, int fieldNumber )
{
    logHandler_.setObjectNumber( fieldNumber );

	try
	{
		editionNumber_ = editionNumber( field );
	    std::string dataProvider = dataProviderName( field );
	    std::string place = placeName( field );
	    std::string valueParameter = valueParameterName( field );
	    std::vector<wdb::load::Level> levels;
	    levelValues( levels, field );
	    const double * data = field.getValues( );
	    if ( loadingConfiguration_.output().list ) {
	    	cout << field.getValuesSize() << "floating points, "
				 << dataProvider << ", "
				 << place << ", "
				 << field.getReferenceTime( ) << ", "
				 << field.getValidTimeFrom( ) << ", "
				 << field.getValidTimeTo( ) << ", "
				 << valueParameter << ", "
				 << levels[0].levelParameter_ << ", "
				 << levels[0].levelFrom_ << ", "
				 << levels[0].levelTo_ << ", "
				 << dataVersion( field ) << ", "
				 << confidenceCode( field ) << std::endl;

	    }
	    else {
	    	std::vector<double> grid(data, data + field.getValuesSize());
	    	convertValues( field, grid );
			for ( unsigned int i = 0; i<levels.size(); i++ ) {
				connection_.write ( &grid[0],
									grid.size(),
									dataProvider,
									place,
									field.getReferenceTime( ),
									field.getValidTimeFrom( ),
									field.getValidTimeTo( ),
									valueParameter,
									levels[i].levelParameter_,
									levels[i].levelFrom_,
									levels[i].levelTo_,
									dataVersion( field ),
									confidenceCode( field ) );
			}
	    }
	}
	catch ( wdb::ignore_value &e )
	{
	    WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
		log.infoStream() << e.what() << " Data field not loaded.";
	}
	catch ( wdb::missing_metadata &e )
	{
	    WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
		log.warnStream() << e.what() << " Data field not loaded.";
	}
	/* Note supported < pqxx 3.0.0
	catch (pqxx::unique_violation &e) {
		// Duplicate key violations - downgraded to warning level
		log.warnStream() << e.what() << " Data field not loaded.";
	}
	*/
	catch ( std::exception & e )
	{
	    WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
		log.errorStream() << e.what() << " Data field not loaded.";
	}
}

std::string GribLoader::dataProviderName(const GribField & field) const
{
	stringstream keyStr;
	keyStr << field.getGeneratingCenter() << ", "
		   << field.getGeneratingProcess();
	try {
		std::string ret = Grib2DataProviderName_[ keyStr.str() ];
		return ret;
	}
	catch ( std::out_of_range &e ) {
	    WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
		log.errorStream() << "Could not identify the data provider.";
		throw;
	}
}

std::string GribLoader::placeName(const GribField & field) const
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
	std::string placeName;
	try {
		placeName = connection_.getPlaceName( field.numberX(), field.numberY(),
											  field.incrementX(), field.incrementY(),
											  field.startX(), field.startY(),
											  field.getProjDefinition() );
		log.debugStream() << "Identified placeName: " << placeName;
	}
	catch ( wdb::empty_result & e ) {
		log.debugStream() << "Could not identify placeName";
		if ( loadingConfiguration_.loading().loadPlaceDefinition ) {
			log.debugStream() << "Attempting to insert new placeName";
			placeName = "GribLoad AutoInsert ";
			ptime now = microsec_clock::local_time();
			placeName += to_iso_extended_string( now );
			placeName = connection_.addPlaceDefinition( placeName,
														field.numberX(), field.numberY(),
														field.incrementX(), field.incrementY(),
														field.startX(), field.startY(),
														field.getProjDefinition() );
			log.debugStream() << "Insertion of placeName '" << placeName << "' succeeded";
		}
		else {
			throw;
		}
	}
	return placeName;
}

std::string GribLoader::valueParameterName(const GribField & field) const
{
	stringstream keyStr;
	std::string ret;
	if (editionNumber_ == 1) {
		keyStr << field.getGeneratingCenter() << ", "
			   << field.getCodeTableVersionNumber() << ", "
			   << field.getParameter1() << ", "
			   << field.getTimeRange() << ", "
			   << "0, 0, 0, 0"; // Default values for thresholds
		try {
			ret = Grib2ValueParameter1_[keyStr.str()];
		}
		catch ( std::out_of_range &e ) {
			WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
			log.errorStream() << "Could not identify the value parameter.";
			throw;
		}
	}
	else {
		keyStr << field.getParameter2();
		try {
			ret = Grib2ValueParameter2_[keyStr.str()];
		}
		catch ( std::out_of_range &e ) {
			WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
			log.errorStream() << "Could not identify the value parameter.";
			throw;
		}
	}
	ret = ret.substr( 0, ret.find(',') );
	boost::trim( ret );
	return ret;
}

std::string GribLoader::valueParameterUnit(const GribField & field) const
{
	stringstream keyStr;
	std::string ret;
	if (editionNumber_ == 1) {
		keyStr << field.getGeneratingCenter() << ", "
			   << field.getCodeTableVersionNumber() << ", "
			   << field.getParameter1() << ", "
			   << field.getTimeRange() << ", "
			   << "0, 0, 0, 0"; // Default values for thresholds
		try {
			ret = Grib2ValueParameter1_[keyStr.str()];
		}
		catch ( std::out_of_range &e ) {
			WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
			log.errorStream() << "Could not identify the value parameter identified by " << keyStr.str();
			throw;
		}
	}
	else {
		keyStr << field.getParameter2();
		try {
			ret = Grib2ValueParameter2_[keyStr.str()];
		}
		catch ( std::out_of_range &e ) {
			WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
			log.errorStream() << "Could not identify the value parameter identified by " << keyStr.str();
			throw;
		}
	}
	ret = ret.substr( ret.find(',') + 1 );
	boost::trim( ret );
	return ret;
}

void GribLoader::levelValues( std::vector<wdb::load::Level> & levels, const GribField & field ) const
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
	bool ignored = false;
	stringstream keyStr;
	std::string ret;
	try {
		if (editionNumber_ == 1) {
			keyStr << field.getLevelParameter1();
			ret = Grib2LevelParameter1_[keyStr.str()];
		}
		else {
			keyStr << field.getLevelParameter2();
			ret = Grib2LevelParameter2_[keyStr.str()];
		}
		std::string levelParameter = ret.substr( 0, ret.find(',') );
		boost::trim( levelParameter );
		std::string levelUnit = ret.substr( ret.find(',') + 1 );
		boost::trim( levelUnit );
		float coeff = 1.0;
		float term = 0.0;
		connection_.readUnit( levelUnit, &coeff, &term );
		float lev1 = field.getLevelFrom();
		float lev2 = field.getLevelTo();
	    if ( ( coeff != 1.0 )&&( term != 0.0) ) {
   			lev1 =   ( ( lev1 * coeff ) + term );
   			lev2 =   ( ( lev2 * coeff ) + term );
	    }
	    wdb::load::Level baseLevel( levelParameter, lev1, lev2 );
	    levels.push_back( baseLevel );
	}
	catch ( wdb::ignore_value &e )
	{
		log.infoStream() << e.what();
		ignored = true;
	}
	catch ( std::out_of_range &e ) {
		log.errorStream() << "Could not identify the level parameter identified by " << keyStr.str();
	}
	// Find additional level
	try {
		stringstream keyStr;
		std::string ret;
		if (editionNumber_ == 1) {
			keyStr << field.getGeneratingCenter() << ", "
				   << field.getCodeTableVersionNumber() << ", "
				   << field.getParameter1() << ", "
				   << field.getTimeRange() << ", "
				   << "0, 0, 0, 0, "
				   << field.getLevelParameter1(); // Default values for thresholds
			ret = Grib2LevelAdditions1_[keyStr.str()];
		}
		else {
			keyStr << field.getParameter2();
			ret = Grib2LevelAdditions2_[keyStr.str()];
		}
		if ( ret.length() != 0 ) {
			std::string levelParameter = ret.substr( 0, ret.find(',') );
			boost::trim( levelParameter );
			string levFrom = ret.substr( ret.find_first_of(',') + 1, ret.find_last_of(',') - (ret.find_first_of(',') + 1) );
			boost::trim( levFrom );
			string levTo = ret.substr( ret.find_last_of(',') + 1 );
			boost::trim( levTo );
			float levelFrom = boost::lexical_cast<float>( levFrom );
			float levelTo = boost::lexical_cast<float>( levTo );
			wdb::load::Level level( levelParameter, levelFrom, levelTo );
			levels.push_back( level );
		}
	}
	catch ( wdb::ignore_value &e )
	{
		log.infoStream() << e.what();
	}
	catch ( std::out_of_range &e )
	{
		// NOOP
	}
	if ( levels.size() == 0 ) {
		if ( ignored )
			throw wdb::ignore_value( "Level key is ignored" );
		else
			throw std::out_of_range( "No valid level key values found." );
	}
}

int GribLoader::dataVersion( const GribField & field ) const
{
	if (editionNumber_ == 1) {
		return field.getDataVersion();
	}
	else {
		// TODO: Decoding EPS data in GRIB2 is not yet implemented.
		return 0;
	}
}

int GribLoader::confidenceCode( const GribField & field ) const
{
		return 0; // Default
}


int GribLoader::editionNumber( const GribField & field ) const
{
		return field.getEditionNumber();
}

namespace
{
struct value_convert : std::unary_function<double, double>
{
	double coeff_;
	double term_;

	value_convert(double coeff, double term) :
		coeff_(coeff),
		term_(term)
	{}

	double operator () (double val) const
	{
		return ((val * coeff_) + term_);
	}
};
}


void
GribLoader::convertValues( const GribField & field, std::vector<double> & valuesInOut ) const
{
    std::string valueUnit = valueParameterUnit( field );
	float coeff = 1.0;
	float term = 0.0;
	connection_.readUnit( valueUnit, &coeff, &term );

	try
	{
		double gribMissing = field.getMissingValue();
		double wdbMissing = connection_.getUndefinedValue();
		std::replace(valuesInOut.begin(), valuesInOut.end(), gribMissing, wdbMissing);
	}
	catch ( std::exception & )
	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
		log.warn("Unable to find key for missing GRIB value");
	}

	std::transform(valuesInOut.begin(), valuesInOut.end(), valuesInOut.begin(), value_convert(coeff, term));
}

}

}
