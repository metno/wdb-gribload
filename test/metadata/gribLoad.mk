
#-----------------------------------------------------------------------------
# WDB GribLoad Metadata Tests
#-----------------------------------------------------------------------------

GRIBLOADMTEST_SUPPORT =		test/metadata/tearDown.in.sh \
							test/metadata/testEnvironment.in.sh

INSTALLTESTS +=				gribLoadMetadataTest

noinst_SCRIPTS +=			gribLoadMetadataTest
							$(GRIBLOADMTEST_SUPPORT:.in.sh=.sh) 

CLEANFILES +=				gribLoadMetadataTest \
							$(GRIBLOADMTEST_SUPPORT:.in.sh=.sh) 

EXTRA_DIST +=				test/metadata/gribLoadMetadataTest.in.sh \
							$(GRIBLOADMTEST_SUPPORT) \
							test/metadata/gribLoad.mk 

gribLoadMetadataTest:		test/metadata/gribLoadMetadataTest.in.sh
							mkdir -p test/metadata
							$(SED_SUBSTITUTION);\
							chmod 754 $@
