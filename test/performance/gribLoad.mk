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


PERFORMANCETESTS +=		gribLoadPerformanceTest

noinst_SCRIPTS +=		gribLoadPerformanceTest \
						$(GRIBLOADPERFTEST_SOURCES:.in.test=.test) \
						$(GRIBLOADPERFTEST_SUPPORT:.in.sh=.sh)

CLEANFILES +=			gribLoadPerformanceTest \
						$(GRIBLOADPERFTEST_SOURCES:.in.test=.test) \
						$(GRIBLOADPERFTEST_SUPPORT:.in.sh=.sh)

EXTRA_DIST +=			test/performance/gribLoadPerformanceTest.in.sh \
						$(GRIBLOADPERFTEST_SOURCES) \
						$(GRIBLOADPERFTEST_SUPPORT) \
						test/performance/gribLoad.mk 

gribLoadPerformanceTest:	test/performance/gribLoadPerformanceTest.in.sh
							mkdir -p test/performance
							$(SED_SUBSTITUTION);
							chmod 754 $@
