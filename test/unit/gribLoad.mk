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
	                   	src/GribHandleReader.h \
	                   	src/GribField.cpp \
	                   	src/GribField.h

gribLoadUnitTest_SOURCES = \
						test/unit/main.cpp \
						test/unit/GribGridDefinitionTest.cpp \
						test/unit/GribGridDefinitionTest.h \
                        test/unit/GribFieldTest.h \
                        test/unit/GribFieldTest.cpp \
                        test/unit/MockGribHandleReader.h \
                        test/unit/MockGribHandleReader.cpp \
                        $(GRIBLOAD_LINK_IN_SOURCES)

gribLoadUnitTest_LDFLAGS = \
						$(cppunit_LIBS) -lwdbProj -lwdbTest
# HAS_CPPUNIT
endif

EXTRA_DIST +=                           test/unit/gribLoad.mk
