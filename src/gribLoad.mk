
#-----------------------------------------------------------------------------
# WDB GribLoad Component
#-----------------------------------------------------------------------------

gribLoad_SOURCES =		src/main.cpp \
						src/GribLoader.cpp \
						src/GribLoader.h \
						src/GribFile.cpp \
						src/GribFile.h \
						src/GribField.cpp \
						src/GribField.h \
						src/GribGridDefinition.cpp \
						src/GribGridDefinition.h \
						src/GribHandleReader.cpp \
						src/GribHandleReader.h \
						src/GribHandleReaderInterface.h
					
EXTRA_DIST +=			src/gribLoad.mk
