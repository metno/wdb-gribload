/*
    gribLoad

    Copyright (C) 2009 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: post@met.no

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


#ifndef MOCKGRIBHANDLEREADER_H_
#define MOCKGRIBHANDLEREADER_H_

#include <GribHandleReader.h>
#include <map>
#include <vector>
#include <string>


class MockGribHandleReader : public wdb::grib::GribHandleReaderInterface
{
public:
	typedef std::map<std::string, double> Expected;
	Expected expected;
	typedef std::map<std::string, std::string> ExpectedStr;
	ExpectedStr expectedStr;

	virtual long getLong(const char * name);
	virtual double getDouble(const char * name);
	virtual std::string getString( const char * name );
	virtual size_t getValuesSize( );
	virtual double * getValues( );
	typedef std::vector<double> Grid;

	/**
	 * Fill the grid to be returned with the given value, overwriting or
	 * removing any values already there.
	 *
	 * @warning expected["numberOfPointsAlongAParallel"] and
	 * expected["numberOfPointsAlongAMeridian"] must have been set before
	 * calling this function.
	 *
	 */
	void fillGridWith(double value);
	Grid & getGrid() { return grid_; }
	const Grid & getGrid() const { return grid_; }

private:
	std::vector<double> grid_;
};


#endif /* MOCKGRIBHANDLEREADER_H_ */
