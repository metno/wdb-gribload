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

#include "MockGribHandleReader.h"
#include <stdexcept>


long MockGribHandleReader::getLong(const char * name)
{
	return (long) getDouble(name);
}

double MockGribHandleReader::getDouble(const char * name)
{
	Expected::const_iterator it = expected.find(name);
	if ( it != expected.end() )
		return it->second;
	std::string errorMsg = "Can't find key: ";
	throw std::logic_error(errorMsg + name);
}

size_t MockGribHandleReader::getValuesSize( )
{
	return grid_.size();
}

double * MockGribHandleReader::getValues( )
{
	if ( grid_.empty() )
		return 0;

	double * ret = new double[grid_.size()];
	std::copy(grid_.begin(), grid_.end(), ret);

	return ret;
}

void MockGribHandleReader::fillGridWith(double value)
{
	long size = getLong("numberOfPointsAlongAParallel") * getLong("numberOfPointsAlongAMeridian");
	grid_ = Grid(size, value);
}
