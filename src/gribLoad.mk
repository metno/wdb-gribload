
#-----------------------------------------------------------------------------
# WDB GribLoad Component
#-----------------------------------------------------------------------------

pkglib_PROGRAMS +=		gribLoad

gribLoad_SOURCES =		src/gribLoad.cpp \
						src/GribDatabaseInterface.h \
						src/gribDatabaseConnection.cpp \
						src/gribDatabaseConnection.h \
						src/gribField.cpp \
						src/gribField.h \
						src/gribGridDefinition.cpp \
						src/gribGridDefinition.h \
						src/GribHandleReader.cpp \
						src/GribHandleReader.h \
						src/transactors/gribTransactorDataProvider.h \
						src/transactors/gribTransactorValueParameter.h \
						src/transactors/gribTransactorLevelParameter.h \
						src/wmoCodeTables.h
					
EXTRA_DIST +=			src/gribLoad.mk
