#-----------------------------------------------------------------------------
# libwdbtest
#-----------------------------------------------------------------------------

# Library
#-----------------------------------------------------------------------------

noinst_LTLIBRARIES = 		libwdbTest.la

libwdbTest_la_SOURCES = test/util/configuration/testConfiguration.cpp

pkginclude_HEADERS = 	test/util/configuration/testConfiguration.h

libwdbTest_la_CPPFLAGS = \
						$(AM_CPPFLAGS) -fPIC

EXTRA_DIST +=			test/util/configuration/gribLoad.mk \
						test/util/configuration/Makefile.am \
						test/util/configuration/Makefile.in

DISTCLEANFILES +=		test/util/configuration/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

test/util/configuration/all: libwdbTest.la

test/util/configuration/clean: clean
