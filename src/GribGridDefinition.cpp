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
 * @addtogroup loadingprogram
 * @{
 * @addtogroup gribload
 * @{
 */
/**
 * @file
 * Implementation of GribGridDefinition class
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// CLASS
#include "GribGridDefinition.h"
// PROJECT INCLUDES
#include "GribHandleReader.h"
#include <wdbLogHandler.h>
#include <WdbProjection.h>
#include <GridGeometry.h>
// SYSTEM INCLUDES
#include <boost/assign/list_of.hpp>
#include <sstream>
#include <iostream>
#include <string>
#include <proj_api.h>

using namespace std;
using namespace wmo::codeTable;
using namespace wdb;
using namespace wdb::grib;

namespace
{

// Data Representations - WMO Code Table 6
enum {
    REGULAR_LAT_LON_GRID  = 0,
    GAUSSIAN_LAT_LON_GRID = 4,
    ROTATED_LAT_LON_GRID  = 10
};

typedef std::map<wmo::codeTable::ScanMode, GridGeometry::Orientation> Wmo2InternalScanMode;

const Wmo2InternalScanMode orientation =
	boost::assign::map_list_of
	(wmo::codeTable::LeftUpperHorizontal, GridGeometry::LeftUpperHorizontal)
	(wmo::codeTable::LeftLowerHorizontal, GridGeometry::LeftLowerHorizontal);

}

//---------------------------------------------------------------------------
// Public Methods
//---------------------------------------------------------------------------

// Lifecycle
//---------------------------------------------------------------------------

void GribGridDefinition::setup()
{
    std::string sridProj = getProjDefinition();

    wmo::codeTable::ScanMode scanMode = (wmo::codeTable::ScanMode) gribHandleReader_->getLong("scanningMode");
    Wmo2InternalScanMode::const_iterator f = orientation.find(scanMode);
    if ( f == orientation.end() )
    	throw std::runtime_error("Unrecognized scan mode");
    GridGeometry::Orientation o = f->second;
	long iNumber = gribHandleReader_->getLong("numberOfPointsAlongAParallel");
	long jNumber = gribHandleReader_->getLong("numberOfPointsAlongAMeridian");
	double iIncrement = gribHandleReader_->getDouble("iDirectionIncrementInDegrees");// * DEG_TO_RAD;
	if ( gribHandleReader_->getLong("iScansNegatively") )
		iIncrement *= -1;

	double jIncrement = gribHandleReader_->getDouble("jDirectionIncrementInDegrees");// * DEG_TO_RAD;
	if ( ! gribHandleReader_->getLong("jScansPositively") )
		jIncrement *= -1;

	double startI = gribHandleReader_->getDouble("longitudeOfFirstGridPointInDegrees");// * DEG_TO_RAD;
	double startJ = gribHandleReader_->getDouble("latitudeOfFirstGridPointInDegrees");// * DEG_TO_RAD;

    WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.gribField" );
    log.debugStream() << "Creating geometry with ("
					  << iNumber << ", "
					  << jNumber << ", "
					  << iIncrement << ", "
					  << jIncrement << ", "
					  << startI << ", "
					  << startJ << ")";
    geometry_ = new GridGeometry(sridProj, o, iNumber, jNumber, iIncrement, jIncrement, startI, startJ );
}

GribGridDefinition::GribGridDefinition( grib_handle * gribHandle )
	: geometry_(0)
{
	gribHandleReader_ = new GribHandleReader(gribHandle);
	setup();
}

GribGridDefinition::GribGridDefinition( GribHandleReaderInterface * gribHandleR )
	: geometry_(0)
{
	gribHandleReader_ = gribHandleR;
	setup();
}

GribGridDefinition::~GribGridDefinition()
{
	delete geometry_;
	delete gribHandleReader_;
}

// Operations
//---------------------------------------------------------------------------


// Access
//---------------------------------------------------------------------------

int
GribGridDefinition::numberX() const
{
    return geometry_->xNumber_;
};

int
GribGridDefinition::numberY() const
{
    return geometry_->yNumber_;
};

float
GribGridDefinition::incrementX() const
{
	return geometry_->xIncrement_;
};

float
GribGridDefinition::incrementY() const
{
	return geometry_->yIncrement_;
};

float
GribGridDefinition::startX() const
{
	return geometry_->startX_;
};

float
GribGridDefinition::startY() const
{
	return geometry_->startY_;
};

std::string GribGridDefinition::getGeometry() const
{
	return geometry_->wktRepresentation();
}

void
GribGridDefinition::setScanMode( wmo::codeTable::ScanMode mode )
{
	Wmo2InternalScanMode::const_iterator find = orientation.find(mode);
	if ( find == orientation.end() )
	{
        throw std::runtime_error( "Unsupported field conversion in GribGridDefinition" );
	}
	geometry_->setOrientation(find->second);
}

wmo::codeTable::ScanMode GribGridDefinition::getScanMode() const
{
	for ( Wmo2InternalScanMode::const_iterator it = orientation.begin(); it != orientation.end(); ++ it )
		if ( it->second == geometry_->orientation() )
			return it->first;

	// If we get here, we could not understand the scan mode:
	ostringstream errMsg;
	errMsg << "Scan mode " << geometry_->orientation() << " is unknown to the grid loader";
	throw std::runtime_error( errMsg.str() );
}

std::string GribGridDefinition::getProjDefinition()
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.gribLoad.griddefinition" );

	switch (gribHandleReader_->getLong("dataRepresentationType"))
    {
    case REGULAR_LAT_LON_GRID:
        log.debug( "Field has a regular lat/lon grid" );
        return regularLatLonProjDefinition();
    case ROTATED_LAT_LON_GRID:
        log.debug( "Field has a rotated lat/lon grid" );
        return rotatedLatLonProjDefinition();
    default:
        throw std::runtime_error("Unrecognizable data representation type in GRIB file");
    }
}


std::string
GribGridDefinition::regularLatLonProjDefinition()
{
    // Define the PROJ definitions of the calculations
    std::ostringstream srcProjDef;
    srcProjDef << "+proj=longlat";
   	long int earthIsOblate;
	earthIsOblate = gribHandleReader_->getLong("earthIsOblate");;
    if (earthIsOblate)
    {
        srcProjDef << " +a=6378160.0 +b=6356775.0";
    }
    else
    {
        srcProjDef << " +a=6367470.0";
    }
    srcProjDef << " +towgs84=0,0,0 +no_defs";

	// Set the PROJ string for SRID
    return srcProjDef.str();
    // poleLongitude_ = -90.0;

}

std::string
GribGridDefinition::rotatedLatLonProjDefinition()
{
    // Define the PROJ definitions used for the computation of the
    // Rotated projection
    std::ostringstream srcProjDef;
    srcProjDef << "+proj=ob_tran +o_proj=longlat";
    srcProjDef << " +lon_0=";
    srcProjDef << gribHandleReader_->getDouble("longitudeOfSouthernPoleInDegrees");
    srcProjDef << " +o_lat_p=";
    srcProjDef << - gribHandleReader_->getDouble("latitudeOfSouthernPoleInDegrees");
    // Earth Shape
   	long int earthIsOblate;
	earthIsOblate = gribHandleReader_->getLong("earthIsOblate");;
    if (earthIsOblate)
    {
        srcProjDef << " +a=6378160.0 +b=6356775.0";
    }
    else {
        srcProjDef << " +a=6367470.0";
    }
    srcProjDef << " +no_defs";

    // Set the PROJ string for SRID
    return srcProjDef.str();
}


/**
 * @}
 *
 * @}
 */
