## - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## 
## wdb - weather and water data storage
##
## Copyright (C) 2007 met.no
##
##  Contact information:
##  Norwegian Meteorological Institute
##  Box 43 Blindern
##  0313 OSLO
##  NORWAY
##  E-mail: wdb@met.no
##
##  This is free software; you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation; either version 2 of the License, or
##  (at your option) any later version.
##
## - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Help Info
export SCRIPT_USAGE="Usage: $0 [OPTION]

gribLoad Metadata Test Suite
Description: The gribLoad metadata tests check the metadata configuration
files of the package by loading 
(The WDB installation variable must have been set up correctly to 
 indicate the test database prior to the gribLoad installation tests
 being run. See wdb_gribload_system-test-specification for details)

Options:
--help             display this help and exit
--version          output version information and exit
--xml,-x		   output test results in JUnit XML
--strict-mode	   treat skipped tests as errors
"

export SCRIPT_VERSION=__WDB_VERSION__

# Database Setup
WDB_CONFIG=__WDB_CONFIG__
export PSQL="psql `$WDB_CONFIG --psqlArgs`"

# Paths
export GRIBLOAD_PATH=__WDB_PKGLIBDIR__
export GRIBWRITE_PATH=__WDB_BUILDDIR__
export VAR_PATH=__WDB_BUILDDIR__/var
export TMP_PATH=__WDB_BUILDDIR__/var/tmp
export LOG_PATH=__WDB_LOGDIR__
export SRC_PATH=__WDB_SRCDIR__

# Setup testfile and log
export TESTCASE_FILE=case$TESTCASE
export TEST_FILE=$TMP_PATH/$TESTCASE_FILE.grib
export LOGFILE=$LOG_PATH/$TESTCASE_FILE-gribload.log
rm -f $LOGFILE

# Create temp space
mkdir -p $VAR_PATH
mkdir -p $TMP_PATH
mkdir -p $LOG_PATH
