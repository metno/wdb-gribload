
#-----------------------------------------------------------------------------
# WDB GribLoad Install Tests
#-----------------------------------------------------------------------------

GRIBLOADTEST_SOURCES =		test/install/caseF001_01.in.test \
							test/install/caseF001_02.in.test \
							test/install/caseF001_03.in.test \
							test/install/caseF002_02.in.test \
							test/install/caseF004_01.in.test \
							test/install/caseF004_03.in.test \
							test/install/caseF004_04.in.test \
							test/install/caseF005_01.in.test \
							test/install/caseF005_02.in.test \
							test/install/caseF006_01.in.test \
							test/install/caseF006_02.in.test \
							test/install/caseF006_03.in.test \
							test/install/caseF007_01.in.test \
							test/install/caseF008_01.in.test \
							test/install/caseF009_01.in.test \
							test/install/caseF009_02.in.test \
							test/install/caseF009_03.in.test 

# 003_01 removed due to the grib file being corrupt/unreadable
#							test/install/caseF003_01.in.test 

GRIBLOADTEST_SUPPORT =		test/install/tearDown.in.sh \
							test/install/testEnvironment.in.sh

INSTALLTESTS +=				gribLoadInstallTest

noinst_SCRIPTS +=			gribLoadInstallTest \
							$(GRIBLOADTEST_SOURCES:.in.test=.test) \
							$(GRIBLOADTEST_SUPPORT:.in.sh=.sh) 

CLEANFILES +=				gribLoadInstallTest \
							$(GRIBLOADTEST_SOURCES:.in.test=.test) \
							$(GRIBLOADTEST_SUPPORT:.in.sh=.sh) 

EXTRA_DIST +=				test/install/gribLoadInstallTest.in.sh \
							$(GRIBLOADTEST_SOURCES) \
							$(GRIBLOADTEST_SUPPORT) \
							test/install/caseF003_01.grib \
							test/install/gribLoad.mk 

gribLoadInstallTest:		test/install/gribLoadInstallTest.in.sh
							mkdir -p test/install
							$(SED_SUBSTITUTION);\
							chmod 754 $@
