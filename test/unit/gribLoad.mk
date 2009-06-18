#-----------------------------------------------------------------------------
# GribLoad Unit Test Framework
#
# See: <top>/test/unit
#-----------------------------------------------------------------------------

if HAS_CPPUNIT

TESTS = gribLoadUnitTest

check_PROGRAMS = gribLoadUnitTest

GRIBLOAD_LINK_IN_SOURCES = \
	                   	src/GribGridDefinition.cpp \
	                   	src/GribGridDefinition.h \
	                   	src/GribHandleReader.cpp \
	                   	src/GribHandleReader.h 

gribLoadUnitTest_SOURCES = \
						test/unit/main.cpp \
						test/unit/GribGridDefinitionTest.cpp \
                        test/unit/GribGridDefinitionTest.h \
                        $(GRIBLOAD_LINK_IN_SOURCES)

gribLoadUnitTest_LDFLAGS = \
						$(cppunit_LIBS) -lwdbProjection -ltestConfiguration
# HAS_CPPUNIT
endif

EXTRA_DIST +=                           test/unit/gribLoad.mk