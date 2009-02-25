#-----------------------------------------------------------------------------
# WCI Types Unit Test Framework
#
# See: <top>/test/unit
#-----------------------------------------------------------------------------

if HAS_CPPUNIT

TESTS = wdbUnitTest

check_PROGRAMS = wdbUnitTest

GRIBLOAD_LINK_IN_SOURCES = \
                   src/gribGridDefinition.cpp \
                   src/gribGridDefinition.h \
                   src/GribHandleReader.cpp \
                   src/GribHandleReader.h 
#                   src/wdbSetup.cpp \
#                   src/wdbSetup.h

wdbUnitTest_SOURCES =           test/wdbUnitTest.cpp \
				test/GribGridDefinitionTest.cpp \
                                test/GribGridDefinitionTest.h \
                                test/FakeGribDatabaseInterface.h \
                                $(GRIBLOAD_LINK_IN_SOURCES)


wdbUnitTest_LDFLAGS =           $(cppunit_LIBS) -lwdbProjection -ltestConfiguration
# HAS_CPPUNIT
endif

EXTRA_DIST +=                           test/gribLoad.mk

include test/install/gribLoad.mk
include test/performance/wdb.mk
include test/util/gribLoad.mk

