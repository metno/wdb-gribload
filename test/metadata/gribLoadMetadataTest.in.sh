## - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## 
## wdb - weather and water data storage
##
## Copyright (C) 2011 met.no
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
#!/bin/sh
# Setup environment
source __WDB_BUILDDIR__/test/install/testEnvironment.sh

# Parse command line
while test -n "$1"; do
	case $1 in
    --help) 
    	echo "$SCRIPT_USAGE"; exit 0;;
    --version) 
    	echo "$0 $SCRIPT_VERSION"; exit 0;;
    --xml)
		XML_OPT="-x" 
		shift
		continue;;
    -x)
		XML_OPT="-x"
		shift
		continue;;
    --strict-mode)
		STRICT_OPT="strict" 
		shift
		continue;;
	*)
		echo -n "Did not recognize the option "; echo $1
		echo " "
    	echo "$SCRIPT_USAGE"; exit 0;;
	esac
done

EXECUTE="${GRIBLOAD_PATH}/gribLoad --namespace=test"

TOTAL_TESTS=0
RUN_TESTS=0
OK_TESTS=0
TOTALFAIL_TESTS=0
FAILED_TESTS=0
ERROR_TESTS=0

# Metadata Files
files="/opdata/ec/data_yr_atmo/yr_ec_atmo00_000.grib 
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_003.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_006.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_009.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_012.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_015.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_018.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_021.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_024.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_027.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_030.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_033.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_036.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_039.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_042.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_045.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_048.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_051.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_054.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_057.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_060.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_063.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_066.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_069.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_072.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_078.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_084.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_090.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_096.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_102.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_108.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_114.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_120.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_126.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_132.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_138.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_144.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_150.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_156.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_162.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_168.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_174.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_180.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_186.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_192.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_198.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_204.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_210.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_216.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_222.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_228.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_234.grib
	/opdata/ec/data_yr_atmo/yr_ec_atmo00_240.grib"

# Timing
startTime=`date +%s%N`

# Count Tests
for fil in $files; do
	TOTAL_TESTS=`expr $TOTAL_TESTS + 1`;
done 

if test "$XML_OPT" != "-x"; then
	echo "1..$TOTAL_TESTS"
	echo "#"
	echo "# gribLoad Metadata Tests"
	echo "#"
fi

XML_OUT=""
# Run Tests
for fil in $files; do
	# Clean Logs
	TEST_NAME="`expr $RUN_TESTS + 1`"
	TEST_DESCRIPTION="Loading $fil"
	LOGFILE=$LOG_PATH/gribLoadMetadataTest-$RUN_TESTS.log
	rm -f $LOGFILE
	RUN_TESTS=`expr $RUN_TESTS + 1`;
	if test -z $fil; then
		fileStartTime=`date +%s%N`
		$EXECUTE --logfile $LOGFILE $fil
		fileEndTime=`date +%s%N`
		fileTime=$(echo "scale = 3; ($fileEndTime-$fileStartTime)/1000000000" | bc -l)
		if test "$XML_OPT" != "-x"; then
   			echo -e "done"
		fi
		if grep -q -E "ERROR" $LOGFILE; then
			ERROR_TESTS=`expr $ERROR_TESTS + 1`
			TOTALFAIL_TESTS=`expr $TOTALFAIL_TESTS + 1`
    		# The log contains error messages
    		if test "$XML_OPT" = "-x"; then
	    		XML_OUT="$XML_OUT\n<testcase name=\"$TEST_NAME $TEST_DESCRIPTION\" time=\"$fileTime\">\n\t<failure message=\"Error messages or warnings in log.\">\n\t</failure>\n</testcase>"
			else
	    		echo -e "not ok $TEST_NAME - $TEST_DESCRIPTION [$fileTime seconds]"
			fi
		else
			OK_TESTS=`expr $OK_TESTS + 1`;
    		# Print out
    		if test "$XML_OPT" = "-x"; then
				XML_OUT="$XML_OUT\n<testcase name=\"$TEST_NAME $TEST_DESCRIPTION\" time=\"$fileTime\">\n</testcase>"
			else
	    		echo -e "ok $TEST_NAME - $TEST_DESCRIPTION [$fileTime seconds]"
			fi
		fi
	else
		FAILED_TESTS=`expr $FAILED_TESTS + 1`
   		if test "$STRICT_OPT" = "strict"; then
			TOTALFAIL_TESTS=`expr $TOTALFAIL_TESTS + 1`
   		fi
		if test "$XML_OPT" = "-x"; then
	    		XML_OUT="$XML_OUT\n<testcase name=\"$TEST_NAME $TEST_DESCRIPTION\" time=\"0\">\n\t<failure message=\"Failed to find file.\">\n\t</failure>\n</testcase>"
		else
			echo -e "ok $TEST_NAME - Skipped $TEST_DESCRIPTION [0 seconds]"
		fi
	fi		
done

# Print Test Result
if test "$XML_OPT" != "-x"; then    
	echo "#"
	echo "# gribLoad Metadata Tests done"
	if test "$TOTALFAIL_TESTS" -eq 0; then
		echo "# OK ( $RUN_TESTS )"
		echo "#"
	    exit 0
	else
		echo "# Run: $RUN_TESTS		Failure total: $TOTALFAIL_TESTS		Failures: $FAILED_TESTS		Errors: $ERROR_TESTS"
		echo "#"
	    exit 1;
	fi 
else
	# Timing
	endTime=`date +%s%N`
	totalTime=$(echo "scale = 3; ($endTime-$startTime)/1000000000" | bc -l)
	# Create Header
	echo -e -n "<testsuite name=\"GribLoad Install Tests\" tests=\"$RUN_TESTS\" failures=\"$TOTALFAIL_TESTS\" errors=\"$ERROR_TESTS\" time=\"$totalTime\">"
	# Test Reports	
	echo -e $XML_OUT	
	# Closer
	echo -e "</testsuite>"
fi
