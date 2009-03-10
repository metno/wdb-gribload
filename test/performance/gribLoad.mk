#-----------------------------------------------------------------------------
# WDB Call Interface Performance Tests
#-----------------------------------------------------------------------------

GRIBLOADPERFTEST_SOURCES =	test/performance/caseP001_01.in.test \
							test/performance/caseP002_01.in.test \
							test/performance/caseP003_01.in.test

GRIBLOADPERFTEST_SUPPORT =	test/performance/buildUp.in.sh \
							test/performance/tearDown.in.sh \
							test/performance/testEnvironment.in.sh \
							test/performance/gribLoad_time.in.sh


PERFORMANCETESTS +=		gribLoadPerformanceTest.sh

noinst_SCRIPTS +=		gribLoadPerformanceTest.sh \
						$(GRIBLOADPERFTEST_SOURCES:.in.test=.test) \
						$(GRIBLOADPERFTEST_SUPPORT:.in.sh=.sh)

CLEANFILES +=			gribLoadPerformanceTest.sh \
						$(GRIBLOADPERFTEST_SOURCES:.in.test=.test) \
						$(GRIBLOADPERFTEST_SUPPORT:.in.sh=.sh)

EXTRA_DIST +=			test/performance/gribLoadPerformanceTest.in.sh \
						$(GRIBLOADPERFTEST_SOURCES) \
						$(GRIBLOADPERFTEST_SUPPORT) \
						test/performance/gribLoad.mk 

performancetestdirectory:
	mkdir -p test/performance

gribLoadPerformanceTest.sh:	test/performance/gribLoadPerformanceTest.in.sh performancetestdirectory
							$(SED_SUBSTITUTION);\
							chmod 754 $@
