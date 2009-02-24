#-----------------------------------------------------------------------------
# Project Makefile for wdb's gribLoad
#
# This file is processed by automake to generate the non-recursive
# project Makefile
#-----------------------------------------------------------------------------

# Scanned by Autoreconf; must be static (no expansion)
ACLOCAL_AMFLAGS = 		-I config
				
# Share directory organization
wdbdatadir = 			$(datadir)/wdb
sqldir = 				$(wdbdatadir)/sql

# Setting Targets
pkglib_PROGRAMS = 		gribLoad

# This is to be replaced by a "find wdb" macro:
AM_CXXFLAGS = 			$(wdb_CFLAGS)
AM_LDFLAGS = 			$(wdb_LIBS)
	
gribLoad_CPPFLAGS =  	-I$(top_srcdir)/src

SED_SUBSTITUTION = 		sed s/__WDB_VERSION__/$(WDB_VERSION)/g $<\
						| sed s¤__WDB_LIB_PATH__¤"$(LD_LIBRARY_PATH)"¤g \
						| sed s:__WDB_LOGDIR__:@abs_builddir@/var/logs:g \
						| sed s:__WDB_BUILDDIR__:@abs_builddir@:g \
						| sed s:__WDB_SRCDIR__:@srcdir@:g \
			 			| sed s:__WDB_BINDIR__:@bindir@:g \
			 			| sed s:__WDB_LIBDIR__:@libdir@:g \
			 			| sed s:__WDB_PKGLIBDIR__:$(libdir)/gribload:g \
			 			| sed s:__WDB_PKGDATADIR__:$(pkgdatadir):g \
			 			| sed s:__WDB_DATADIR__:$(wdbdatadir):g \
			 			| sed s:__WDB_SYSCONFDIR__:@sysconfdir@:g \
			 			| sed s:__WDB_LOCALSTATEDIR__:@localstatedir@:g \
						| sed s:__POSTGIS_DIR__:@postgis_SQL@:g \
						| sed s:__WDB_DBNAME__:$(DATABASE_NAME):g \
						| sed s:__WDB_SCHEMA__:wdb_$(DATABASE_SCHEMA):g \
						| sed s:__WCI_SCHEMA__:wci_$(DATABASE_SCHEMA):g \
						| sed s:__WCI_LIB__:wci-$(WDB_VERSION).so:g \
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

include src/gribLoad.mk
include test/install/gribLoad.mk
include test/util/gribWrite/gribLoad.mk



#-----------------------------------------------------------------------------
# Cleaning
#-----------------------------------------------------------------------------

clean-local:
	rm -rf @builddir@/var/



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

###
# Debian package related stuff
###

PKG_DIR = $(PACKAGE)-$(VERSION)
DEBIAN_DIR = $(PKG_DIR)/debian
DEBIAN_PACKAGE = $(PACKAGE)_$(VERSION)
DEBIAN_VERSION = 1
ARCH = `dpkg-architecture -qDEB_HOST_ARCH_CPU`
DEBIAN_PACKAGE_NAME = $(DEBIAN_PACKAGE)-$(DEBIAN_VERSION)_$(ARCH).deb

debian:	dist clean-debian 
	tar xvzf $(PKG_DIR).tar.gz
	mkdir -p $(DEBIAN_DIR)
	rm -rf $(DEBIAN_DIR)/*
	cp -r $(top_srcdir)/debian_files/* $(DEBIAN_DIR)
	cp $(PKG_DIR).tar.gz $(DEBIAN_PACKAGE).orig.tar.gz
	cd $(PKG_DIR) && dpkg-buildpackage -rfakeroot -us -uc
	lintian $(DEBIAN_PACKAGE_NAME)
	linda $(DEBIAN_PACKAGE_NAME)

update-debian:
	rm -rf $(DEBIAN_DIR)/*
	cp -r $(top_srcdir)/debian_files/* $(DEBIAN_DIR)
	cd $(PKG_DIR) && dpkg-buildpackage -rfakeroot -us -uc -nc
	lintian $(DEBIAN_PACKAGE_NAME)
	linda $(DEBIAN_PACKAGE_NAME)


clean-debian:
	debclean
	rm -rf $(PKG_DIR) $(DEBIAN_PACKAGE)*
