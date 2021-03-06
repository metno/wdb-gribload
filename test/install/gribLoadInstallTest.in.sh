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
	--database=*)
	    WDB_DB=`echo $1 | sed 's/--database=//'`
	    shift
	    continue;;
	-d)
	    shift
	    WDB_DB=$1
	    shift
	    continue;;
	--user=*)
	    WDB_USER=`echo $1 | sed 's/--user=//'`
	    shift
	    continue;;
	-u)
	    shift
	    WDB_USER=$1
	    shift
	    continue;;
	--port=*)
	    WDB_PORT=`echo $1 | sed 's/--port=//'`
	    shift
	    continue;;
	-p)
	    shift
	    WDB_PORT=$1
	    shift
	    continue;;
    --xml)
		XML_OPT="-x" 
		shift
		continue;;
    -x)
		XML_OPT="-x"
		shift
		continue;;
	*)
		echo -n "Did not recognize the option "; echo $1
		echo " "
    	echo "$SCRIPT_USAGE"; exit 0;;
	esac
done

# DATABASE_NAME
if test -z "$WDB_DB"; then
    WDB_DB=$DEF_DB
fi
# DATABASE_USER
if test -z "$WDB_USER"; then
	WDB_USER=$DEF_USER
fi
# DATABASE_PORT
if test -z "$WDB_PORT"; then
	WDB_PORT=$DEF_PORT
fi
# DATABASE CONFIGURATION
DB_CONF="-d $WDB_DB -u $WDB_USER -p $WDB_PORT"

TOTAL_TESTS=0
RUN_TESTS=0
OK_TESTS=0
TOTALFAIL_TESTS=0
FAILED_TESTS=0
ERROR_TESTS=0

# Timing
startTime=`date +%s%N`

list=`ls __WDB_BUILDDIR__/test/install/*.test  | grep -v ".in.test"`;

# Count Tests
for tst in $list; do
	TOTAL_TESTS=`expr $TOTAL_TESTS + 1`;
done 

if test "$XML_OPT" != "-x"; then
	echo "1..$TOTAL_TESTS"
	echo "#"
	echo "# gribLoad Installation Tests"
	echo "#"
fi

XML_OUT=""
# Run Tests
if test -n "$list"
    then
    for tst in $list; do
		RUN_TESTS=`expr $RUN_TESTS + 1`;
		if test "$XML_OPT" != "-x"; then
	    	$tst $DB_CONF $RUN_TESTS
		else
	    	XML_OUT="$XML_OUT\n`${tst} -x ${DB_CONF}`"
	    fi
	    TEST_RESULT=$?
	    if  test $TEST_RESULT -eq 0; then \
			OK_TESTS=`expr $OK_TESTS + 1`;
	    elif test $TEST_RESULT -eq 77; then \
		  	TOTALFAIL_TESTS=`expr $TOTALFAIL_TESTS + 1`; FAILED_TESTS=`expr $FAILED_TESTS + 1`;
	    else \
		  	TOTALFAIL_TESTS=`expr $TOTALFAIL_TESTS + 1`; ERROR_TESTS=`expr $ERROR_TESTS + 1`;
	    fi;
    done 
fi

# Print Test Result
if test "$XML_OPT" != "-x"; then    
	echo "#"
	echo "# gribLoad Installation Tests done"
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
