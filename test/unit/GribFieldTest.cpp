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

#include "GribFieldTest.h"
#include <GribField.h>

CPPUNIT_TEST_SUITE_REGISTRATION( GribFieldTest );

using wdb::grib::GribField;

GribFieldTest::GribFieldTest()
{
}

GribFieldTest::~GribFieldTest()
{
}

void GribFieldTest::setUp()
{
	reader = new MockGribHandleReader;
	// Set a reasonable default
	reader->expected["dataRepresentationType"] = 10;
	reader->expected["scanningMode"] = 64; // LeftLowerHorizontal
	reader->expected["Ni"] = 6;
	reader->expected["Nj"] = 4;
	reader->expected["iDirectionIncrementInDegrees"] = 0.1;
	reader->expected["jDirectionIncrementInDegrees"] = 0.1;
	reader->expected["iScansNegatively"] = 0;
	reader->expected["jScansPositively"] = 1;
	reader->expected["latitudeOfFirstGridPointInDegrees"] = -13.25;
	reader->expected["longitudeOfFirstGridPointInDegrees"] = 5.75;
	reader->expected["latitudeOfLastGridPointInDegrees"] = 26.638; // wrong, but it works for these tests
	reader->expected["longitudeOfLastGridPointInDegrees"] = 30.446; // wrong, but it works for these tests
	reader->expected["latitudeOfSouthernPoleInDegrees"] = -22;
	reader->expected["longitudeOfSouthernPoleInDegrees"] = -40;
	reader->expected["earthIsOblate"] = 0;

	reader->expectedStr["gridType"] = "regular_ll";

	reader->fillGridWith(0);

	MockGribHandleReader::Grid & grid = reader->getGrid();
	int gridSize = 24;
	for ( int i = 0; i < gridSize; ++ i )
		grid[i] = i;
}

void GribFieldTest::tearDown()
{
}

void GribFieldTest::test()
{
	// Thre grid should now look like this (uncahanged):
	// 00 01 02 03 04 05
	// 06 07 08 09 10 11
	// 12 13 14 15 16 17
	// 18 19 20 21 22 23

	GribField f(reader);
	CPPUNIT_ASSERT_EQUAL(size_t(24), f.getValuesSize());

    const double * values = f.getValues();


	for ( int i = 0; i < 24; ++ i )
	{
		std::ostringstream msg;
		msg << "Error on index " << i;
		CPPUNIT_ASSERT_EQUAL_MESSAGE(msg.str(), double(i), values[i]);
	}
}

void GribFieldTest::testReorganizeGrid()
{
	// Thre grid should now look like this:
	// 18 19 20 21 22 23
	// 12 13 14 15 16 17
	// 06 07 08 09 10 11
	// 00 01 02 03 04 05

	reader->expected["scanningMode"] = 0; // LeftUpperHorizontal

	GribField f(reader);
	CPPUNIT_ASSERT_EQUAL(size_t(24), f.getValuesSize());

    const double * values = f.getValues();

    int idx = 0;
	for ( int y = 3; y >= 0; -- y )
	{
		for ( int x = 0; x < 6; ++ x )
		{
			std::ostringstream msg;
			msg << "Error on index " << idx;
			CPPUNIT_ASSERT_EQUAL_MESSAGE(msg.str(), double((6*y) + x), values[idx ++] );
		}
	}
}
