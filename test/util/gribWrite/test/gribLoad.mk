#-----------------------------------------------------------------------------
# gribWrite unit tests
#-----------------------------------------------------------------------------

if HAS_CPPUNIT

gribLoadUnitTest_SOURCES +=  	test/util/gribWrite/test/gribPointTest.h \
                                test/util/gribWrite/test/gribPointTest.cpp \
                                test/util/gribWrite/gribPoint.cpp \
                                test/util/gribWrite/gribPoint.h

gribLoadUnitTest_LDFLAGS += 	$(gribWrite_LDADD)

# HAS_CPPUNIT
endif

EXTRA_DIST +=           		test/util/gribWrite/test/gribLoad.mk

