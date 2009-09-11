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

path getConfigFile( const path & fileName )
{
	static const path sysConfDir = SYSCONFDIR;
	path confPath = sysConfDir/fileName;
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
	Grib2DataProviderName_.open( getConfigFile("dataprovider.conf").file_string() );
	Grib2ValueParameter_.open( getConfigFile("valueparameter.conf").file_string() );
	Grib2LevelParameter_.open( getConfigFile("levelparameter.conf").file_string() );
	Grib2LevelAdditions_.open( getConfigFile("leveladditions.conf").file_string() );
}

GribLoader::~GribLoader()
{
	// NOOP
}

void GribLoader::load( GribField & field )
{
	try
	{
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
				 << confidenceCode( field );

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

std::string GribLoader::dataProviderName(const GribField & field)
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

std::string GribLoader::placeName(GribField & field)
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
			ptime now = second_clock::local_time();
			placeName += to_iso_extended_string( now );
			connection_.addPlaceDefinition( placeName,
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

std::string GribLoader::valueParameterName(const GribField & field)
{
	stringstream keyStr;
	keyStr << field.getGeneratingCenter() << ", "
		   << field.getCodeTableVersionNumber() << ", "
		   << field.getParameter() << ", "
		   << field.getTimeRange() << ", "
		   << "0, 0, 0, 0, " // Default values for thresholds
		   << field.getLevelParameter();
	std::string ret;
	try {
		ret = Grib2ValueParameter_[keyStr.str()];
	}
	catch ( std::out_of_range &e ) {
	    WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
		log.errorStream() << "Could not identify the value parameter.";
		throw;
	}
	ret = ret.substr( 0, ret.find(',') );
	boost::trim( ret );
	return ret;
}

std::string GribLoader::valueParameterUnit(const GribField & field)
{
	stringstream keyStr;
	keyStr << field.getGeneratingCenter() << ", "
		   << field.getCodeTableVersionNumber() << ", "
		   << field.getParameter() << ", "
		   << field.getTimeRange() << ", "
		   << "0, 0, 0, 0, " // Default values for thresholds
		   << field.getLevelParameter();
	std::string ret;
	try {
		ret = Grib2ValueParameter_[keyStr.str()];
	}
	catch ( std::out_of_range &e ) {
	    WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
		log.errorStream() << "Could not identify the value parameter.";
		throw;
	}
	ret = ret.substr( ret.find(',') + 1 );
	boost::trim( ret );
	return ret;
}

void GribLoader::levelValues( std::vector<wdb::load::Level> & levels, const GribField & field )
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.grib.gribloader" );
	bool ignored = false;
	try {
		stringstream keyStr;
		keyStr << field.getLevelParameter();
		std::string ret = Grib2LevelParameter_[keyStr.str()];
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
		log.errorStream() << "Could not identify the level parameter.";
	}
	// Find additional level
	try {
		stringstream keyStr;
		keyStr << field.getGeneratingCenter() << ", "
			   << field.getCodeTableVersionNumber() << ", "
			   << field.getParameter() << ", "
			   << field.getTimeRange() << ", "
			   << "0, 0, 0, 0"; // Default values for thresholds
		std::string ret = Grib2LevelAdditions_[keyStr.str()];
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

int GribLoader::dataVersion( const GribField & field )
{
		return field.getDataVersion();
}

int GribLoader::confidenceCode( const GribField & field )
{
		return 0; // Default
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
GribLoader::convertValues( const GribField & field, std::vector<double> & valuesInOut )
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
