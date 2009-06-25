
#-----------------------------------------------------------------------------
# WDB GribLoad Component
#-----------------------------------------------------------------------------

noinst_PROGRAMS +=		gribWrite

gribWrite_CPPFLAGS =  	-I$(top_srcdir)/test/util/gribWrite \
						-I$(top_srcdir)/test/util/gribWrite/test
												
gribWrite_LDADD =		-lwdbMath 
#-lwdbConfig -lwdbProj -lwdbExcept -lwdbLog

gribWrite_SOURCES =		test/util/gribWrite/main.cpp \
						test/util/gribWrite/commandLineParser.cpp \
						test/util/gribWrite/commandLineParser.h \
						test/util/gribWrite/gribWriter.cpp \
						test/util/gribWrite/gribWriter.h \
						test/util/gribWrite/gribPoint.cpp \
						test/util/gribWrite/gribPoint.h 

EXTRA_DIST +=			test/util/gribWrite/gribLoad.mk

include test/util/gribWrite/test/gribLoad.mk

