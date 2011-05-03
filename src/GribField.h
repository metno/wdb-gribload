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


#ifndef GRIBFIELD_H_
#define GRIBFIELD_H_
/**
 * @addtogroup loader
 * @{
 * @addtogroup gribload
 * @{
 */
/**
 * @file
 * This file contains the GribField class definition.
 */

// PROJECT INCLUDES
#include <wdb/WdbLevel.h>
#include <GribGridDefinition.h>
// SYSTEM INCLUDES
#include <grib_api.h>
#include <string>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace wdb
{

namespace grib
{

/**
 * GribField encapsulates a single grib field. Once constructed, this class will take
 * ownership of the provided grib field, deleting it when it is no longer needed.
 *
 * The GribField class handles the transformation and storage of a
 * single field from a GRIB file in the ROAD datamodel.
 *
 * @see GribGridDefinition
 *
 */
class GribField
{
public:
// LIFECYCLE
    /**
     * A holder for a grib field.
     *
     * @param	gribHandle		The handle to the GRIB file used by grib_api.
     *                          GribField will take ownership of the provided
     *                          handle, and delete it when it is no longer needed.
     */
     GribField( grib_handle * gribHandle );

     GribField( GribHandleReaderInterface * gribHandleReader );

    /** Destructor
     */
    ~GribField();

// OPERATIONS
    // NOOP

// ACCESS
    /** Get grid values
     *  @return		A double array containing the data grid
     */
    const double * getValues( ) const;

    /** Get size of the data grid
     *  @return		The size of the data grid
     */
    size_t getValuesSize() const;

    /**
     * Get the special code for a "missing" value
     */
    double getMissingValue() const;

    /** Generating center of the field
     * @return		The GRIB generating center
     */
    long int getGeneratingCenter() const;
    /** Generating process number of the field
     * @return		The GRIB generating process
     */
    long int getGeneratingProcess() const;
    /**
     * Return the Reference Time of the field
     *
     * @warning Do not call getValidTimeFrom() or getValidTimeTo() before you
     *          have called getReferenceTime() at least once. That will
     *          trigger a caching bug
     *
     * @return ReferenceTime
     */
    std::string getReferenceTime() const;
    /**
     * Return the Valid Time From of the field
     *
     * @warning Do not call getValidTimeFrom() or getValidTimeTo() before you
     *          have called getReferenceTime() at least once. That will
     *          trigger a caching bug
     *
     * @return ValidTimeFrom
     */
    std::string getValidTimeFrom() const;
    /**
     * Return the Valid Time To of the field
     *
     * @warning Do not call getValidTimeFrom() or getValidTimeTo() before you
     *          have called getReferenceTime() at least once. That will
     *          trigger a caching bug
     *
     * @return ValidTimeTo
     */
    std::string getValidTimeTo() const;
    /** GRIB Code Table 2 version used
     * @return The WMO Code Table 2number
     */
    long int getCodeTableVersionNumber() const;
    /** GRIB parameter
     * @return	The GRIB parameter
     */
    long int getParameter1() const;
    /** GRIB parameter
     * @return	The GRIB parameter
     */
    long int getParameter2() const;
    /** GRIB time range of the field data
     * @return	The GRIB time range
     */
    long int getTimeRange() const;
    /** GRIB level parameter
     * @return	The GRIB level parameter
     */
    long int getLevelParameter1() const;
    /** GRIB level parameter
     * @return	The GRIB level parameter
     */
    std::string getLevelParameter2() const;
    /**
     * Return the Level From of the field
     * @return LevelFrom
     */
    double getLevelFrom() const;
    /**
     * Return the Level To of the field
     * @return LevelTo
     */
    double getLevelTo() const;
    /**
     * Return the Data Version of the field
     * @return DataVersion
     */
    int getDataVersion() const;

    // Projection Information
    /** Return the number of points on the X axis
     * @return	numberX
     */
    virtual int numberX() const;
    /** Return the number of points on the Y axis
     * @return	numberY
     */
    virtual int numberY() const;
    /** Return the increment distance between points on the X axis
     * @return	incrementX
     */
    virtual float incrementX() const;
    /** Return the increment distance between points on the Y axis
     * @return	incrementY
     */
    virtual float incrementY() const;
    /** Return the starting point on the X axis
     * @return	startX
     */
    virtual float startX() const;
    /** Return the starting point on the Y axis
     * @return	startY
     */
    virtual float startY() const;
    /** Return the PROJ definition of the Grid Definition
     * @return	PROJ.4 string
     */
	std::string getProjDefinition() const;

    /**
     * Return the Edition Number of the field
     * @return Edition Number
     */
    int getEditionNumber() const;
// INQUIRY
    // NOOP

protected:

private:
// PRIVATE VARIABLES
    /// A pointer to the array of values of the GRIB field
    double * values_;
    /// The number of value elements (size of values_ array)
    size_t sizeOfValues_;
    /// The GRID definition of the GRIB field
    GribGridDefinition * grid_;

    /// Used for calculations, just a cached value of the referenceTime() method
    mutable boost::posix_time::ptime referenceTime_;
    /// Used for calculations, just a cached value of the validityTime() method
    mutable boost::posix_time::ptime validityTime_;


// PRIVATE METHODS
    /**
     * Return the Validity Time of the field
     *
     * @warning Do not call getValidTimeFrom() or getValidTimeTo() before you
     *          have called getValidityTime() at least once. That will
     *          trigger a caching bug
     *
     * @return ReferenceTime
     */
    std::string getValidityTime() const;
	/**
	 * Retrieve the values from the file into an array pointed to by values_
	 */
    void retrieveValues();
    /**
     * Convert values grid to LeftUpperHorizontal Scan mode
     */
	void gridToLeftUpperHorizontal( );
	/**
	 * Convert values grid to LeftLowerHorizontal Scan mode
	 */
	void gridToLeftLowerHorizontal( );
    /**
     * Initialize the data in the GRIB Field to prepare it for storage
     * in the ROAD database
     *
     * @param	defaultMode	The scan mode that the data should be in for storage
     */
    void initializeData( wmo::codeTable::ScanMode defaultMode );

    /// Wraps reading of grib_handle
    GribHandleReaderInterface * gribHandleReader_;
};

}	// namespace grib

} 	// namespace wdb


/**
 * @}
 *
 * @}
 */

#endif  // GRIBFIELD_H_
