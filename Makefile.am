#-----------------------------------------------------------------------------
# Project Makefile for wdb's gribLoad
#
# This file is processed by automake to generate the non-recursive
# project Makefile
#-----------------------------------------------------------------------------

# Scanned by Autoreconf; must be static (no expansion)
ACLOCAL_AMFLAGS = 		-I config/m4
				
# Share directory organization
wdbdatadir = 			$(datadir)/wdb
sqldir = 				$(wdbdatadir)/sql

wdblibdir =				$(libdir)/wdb/
pkgsysconfdir = 		@sysconfdir@/gribLoad

# Setting Targets
wdblib_PROGRAMS = 		gribLoad

# This is to be replaced by a "find wdb" macro:
AM_CXXFLAGS =			-DSYSCONFDIR=\"$(pkgsysconfdir)\" $(wdb_CFLAGS)
AM_LDFLAGS = 			$(wdb_LIBS) \
						-lwdbLoad -lwdbConfig -lwdbProj -lwdbExcept -lwdbLog -lwdbMath
	
AM_CPPFLAGS =  			-I$(top_srcdir)/src

SED_SUBSTITUTION = 		sed s%__WDB_VERSION__%$(VERSION)%g $< \
						| sed s%__WDB_LIB_PATH__%"$(LD_LIBRARY_PATH)"%g \
						| sed s%__WDB_LIB_PATH__%"$(LD_LIBRARY_PATH)"%g \
						| sed s%__WDB_LOGDIR__%@abs_builddir@/var/logs%g \
						| sed s%__WDB_BUILDDIR__%@abs_builddir@%g \
						| sed s%__WDB_SRCDIR__%@srcdir@%g \
			 			| sed s%__WDB_BINDIR__%@bindir@%g \
			 			| sed s%__WDB_LIBDIR__%@libdir@%g \
			 			| sed s%__WDB_PKGLIBDIR__%$(libdir)/wdb%g \
			 			| sed s%__WDB_PKGDATADIR__%$(pkgdatadir)%g \
			 			| sed s%__WDB_DATADIR__%$(wdbdatadir)%g \
			 			| sed s%__WDB_SYSCONFDIR__%@sysconfdir@%g \
			 			| sed s%__WDB_LOCALSTATEDIR__%@localstatedir@%g \
			 			| sed s%__WDB_BINDIR__%@bindir@%g \
						| sed s%__POSTGIS_DIR__%@postgis_SQL@%g \
						| sed s%__WDB_DBNAME__%$(DATABASE_NAME)%g \
						| sed s%__WDB_LIB__%wdb.so%g \
						| sed s%__WDB_SCHEMA__%wdb_int%g \
						| sed s%__WCI_SCHEMA__%wci_int%g \
						| sed s%__WCI_LIB__%wci-$(WDB_VERSION).so%g \
						| sed s%__WDB_CONFIG__%$(wdb_CONFIG)%g \
						| sed s%__WDB_ADMIN__%$(wdb_ADMIN)%g \
			 			> $@

CRON_COMPILE =			$(SED_SUBSTITUTION);\
						chmod 754 $@

CSV_COMPILE =			$(SED_SUBSTITUTION)

ENT_COMPILE =			$(SED_SUBSTITUTION)

SH_COMPILE =			$(SED_SUBSTITUTION);\
						chmod 754 $@

SQL_COMPILE =			$(SED_SUBSTITUTION)

TEST_COMPILE =			$(SED_SUBSTITUTION);\
						chmod 754 $@

SUFFIXES = 				.in.cron .cron \
						.in.csv .csv \
						.in.ent .ent \
						.in.sh .sh \
						.in.sql .sql \
						.in.test .test

# Compile Rules
.in.cron.cron:
						$(CRON_COMPILE)

.in.csv.csv:
						$(CSV_COMPILE)

.in.ent.ent:
						$(ENT_COMPILE)

.in.sh.sh:
						$(SH_COMPILE)

.in.sql.sql:
						$(SQL_COMPILE)

.in.test.test:
						$(TEST_COMPILE)

noinst_PROGRAMS =
noinst_SCRIPTS =
EXTRA_DIST =
INSTALLTESTS =
CLEANFILES =
DISTCLEANFILES =
PERFORMANCETESTS =

include src/gribLoad.mk
include etc/gribLoad.mk
include doc/gribLoad.mk
include test/unit/gribLoad.mk
include test/install/gribLoad.mk
include test/metadata/gribLoad.mk
include test/performance/gribLoad.mk
include test/util/gribLoad.mk
include test/util/configuration/gribLoad.mk



#-----------------------------------------------------------------------------
# Cleaning
#-----------------------------------------------------------------------------

clean-local:
	rm -rf @builddir@/var/

#-----------------------------------------------------------------------------
# Dist Check
#-----------------------------------------------------------------------------

DISTCHECK_DATABASE_NAME = $(DATABASE_NAME)
DISTCHECK_CONFIGURE_FLAGS = --with-database-name=$(DISTCHECK_DATABASE_NAME)

#-----------------------------------------------------------------------------
# Install Check Framework
#-----------------------------------------------------------------------------

is_local: $(INSTALLCHECK_PROGRAMS)
	set -e
	for P in $(INSTALLCHECK_PROGRAMS); do
		. $(P)
	done


installcheck-local: $(INSTALLTESTS)
	@failed=0; \
	all=0; \
	xfail=0; \
	xpass=0; \
	skip=0; \
	list='$(INSTALLTESTS)'; \
	if test -n "$$list"; then \
		for tst in $$list; do \
			echo " "; \
			if test -f ./$$tst; then dir=./; \
	    		elif test -f $$tst; then dir=; \
	    		else dir="test/install"; \
			fi; \
	    		if $(TESTS_ENVIRONMENT) $${dir}$$tst; then \
	      			all=`expr $$all + 1`; \
	      			case " $(XFAIL_TESTS) " in \
				*" $$tst "*) \
					xpass=`expr $$xpass + 1`; \
					failed=`expr $$failed + 1`; \
					echo "XPASS: $$tst"; \
	      				;; \
	      			*) \
					echo "PASS: $$tst"; \
	      				;; \
	      			esac; \
			elif test $$? -ne 77; then \
	  			all=`expr $$all + 1`; \
	  			case " $(XFAIL_TESTS) " in \
				*" $$tst "*) \
					xfail=`expr $$xfail + 1`; \
					echo "XFAIL: $$tst"; \
	      				;; \
	      			*) \
					failed=`expr $$failed + 1`; \
					echo "FAIL: $$tst"; \
	      				;; \
	      			esac; \
	    		else \
	      			skip=`expr $$skip + 1`; \
	      			echo "SKIP: $$tst"; \
	    		fi; \
	  	done; \
	  	if test "$$failed" -eq 0; then \
	    		if test "$$xfail" -eq 0; then \
	      			banner="All $$all tests passed"; \
	    		else \
	      			banner="All $$all tests behaved as expected ($$xfail expected failures)"; \
	    		fi; \
	  	else \
	    		if test "$$xpass" -eq 0; then \
	      			banner="$$failed of $$all tests failed"; \
	    		else \
	      			banner="$$failed of $$all tests did not behave as expected ($$xpass unexpected passes)"; \
	    		fi; \
	  	fi; \
	  	dashes="$$banner"; \
	  	skipped=""; \
	  	if test "$$skip" -ne 0; then \
	    		skipped="($$skip tests were not run)"; \
	    		test `echo "$$skipped" | wc -c` -le `echo "$$banner" | wc -c` || \
	      			dashes="$$skipped"; \
	  	fi; \
	  	report=""; \
	  	if test "$$failed" -ne 0 && test -n "$(PACKAGE_BUGREPORT)"; then \
	    		report="Please report to $(PACKAGE_BUGREPORT)"; \
	    		test `echo "$$report" | wc -c` -le `echo "$$banner" | wc -c` || \
	      		dashes="$$report"; \
	  	fi; \
	  	dashes=`echo "$$dashes" | sed s/./=/g`; \
	  	echo "$$dashes"; \
	  	echo "$$banner"; \
	  	test -z "$$skipped" || echo "$$skipped"; \
	  	test -z "$$report" || echo "$$report"; \
	  	echo "$$dashes"; \
	  	test "$$failed" -eq 0; \
	else :; \
	fi



#-----------------------------------------------------------------------------
# Performance Check Framework
#-----------------------------------------------------------------------------


performancecheck:  $(PERFORMANCETESTS)
	@failed=0; \
	all=0; \
	xfail=0; \
	xpass=0; \
	skip=0; \
	list='$(PERFORMANCETESTS)'; \
	if test -n "$$list"; then \
		for tst in $$list; do \
			echo " "; \
			if test -f ./$$tst; then dir=./; \
	    		elif test -f $$tst; then dir=; \
	    		else dir="test/install"; \
			fi; \
	    		if $(TESTS_ENVIRONMENT) $${dir}$$tst; then \
	      			all=`expr $$all + 1`; \
	      			case " $(XFAIL_TESTS) " in \
				*" $$tst "*) \
					xpass=`expr $$xpass + 1`; \
					failed=`expr $$failed + 1`; \
					echo "XPASS: $$tst"; \
	      				;; \
	      			*) \
					echo "PASS: $$tst"; \
	      				;; \
	      			esac; \
			elif test $$? -ne 77; then \
	  			all=`expr $$all + 1`; \
	  			case " $(XFAIL_TESTS) " in \
				*" $$tst "*) \
					xfail=`expr $$xfail + 1`; \
					echo "XFAIL: $$tst"; \
	      				;; \
	      			*) \
					failed=`expr $$failed + 1`; \
					echo "FAIL: $$tst"; \
	      				;; \
	      			esac; \
	    		else \
	      			skip=`expr $$skip + 1`; \
	      			echo "SKIP: $$tst"; \
	    		fi; \
	  	done; \
	  	if test "$$failed" -eq 0; then \
	    		if test "$$xfail" -eq 0; then \
	      			banner="All $$all tests passed"; \
	    		else \
	      			banner="All $$all tests behaved as expected ($$xfail expected failures)"; \
	    		fi; \
	  	else \
	    		if test "$$xpass" -eq 0; then \
	      			banner="$$failed of $$all tests failed"; \
	    		else \
	      			banner="$$failed of $$all tests did not behave as expected ($$xpass unexpected passes)"; \
	    		fi; \
	  	fi; \
	  	dashes="$$banner"; \
	  	skipped=""; \
	  	if test "$$skip" -ne 0; then \
	    		skipped="($$skip tests were not run)"; \
	    		test `echo "$$skipped" | wc -c` -le `echo "$$banner" | wc -c` || \
	      			dashes="$$skipped"; \
	  	fi; \
	  	report=""; \
	  	if test "$$failed" -ne 0 && test -n "$(PACKAGE_BUGREPORT)"; then \
	    		report="Please report to $(PACKAGE_BUGREPORT)"; \
	    		test `echo "$$report" | wc -c` -le `echo "$$banner" | wc -c` || \
	      		dashes="$$report"; \
	  	fi; \
	  	dashes=`echo "$$dashes" | sed s/./=/g`; \
	  	echo "$$dashes"; \
	  	echo "$$banner"; \
	  	test -z "$$skipped" || echo "$$skipped"; \
	  	test -z "$$report" || echo "$$report"; \
	  	echo "$$dashes"; \
	  	test "$$failed" -eq 0; \
	else :; \
	fi

include config/mk/debian.mk