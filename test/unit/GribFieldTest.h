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

#ifndef GRIBFIELDTEST_H_
#define GRIBFIELDTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include "MockGribHandleReader.h"

class GribFieldTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( GribFieldTest );
	CPPUNIT_TEST( test );
	CPPUNIT_TEST( testReorganizeGrid );
	CPPUNIT_TEST_SUITE_END();

public:
	GribFieldTest();
	virtual ~GribFieldTest();

	virtual void setUp();
	virtual void tearDown();

	void test();
	void testReorganizeGrid();

private:
	MockGribHandleReader * reader;
};

#endif /* GRIBFIELDTEST_H_ */
