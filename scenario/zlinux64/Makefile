
# Makefile for the program for this architecture

# $Header: /gsa/yktgsa/projects/w/wit/devbase/scenario/zlinux64/Makefile,v 1.1 2008/07/25 04:52:31 fasano Exp $

platform = zlinux64

sep = /
config_dir	= ..$(sep)..$(sep)config$(sep)

include $(config_dir)p_$(platform).mk

# exe_type should be debug, profile, slow, fast, beta, or released.  
# To override it without
# changing this makefile, use "exe_type=debug" (for example) as an 
# argument to make on the command line.  This default was chosen so that
# we don't accidentally ship product code with symbol tables and other
# info for reverse-engineering enclosed.
exe_type = debug

include ..$(sep)src$(sep)appl.mk


all:	$(objects) unitTest libscen.a 

# Following is for doing compiles locally.  To use it, execute "make dotohs".
# The is useful because doing manipulating libraries and linking
# runs much faster when it is run on the machine that has the files, but
# it does not make much difference when compiling
dotohs:	$(objects)

../src/TAGS:	$(sources)
		cd ..$(sep)src; etags -C $(sources)

parTestRun:	
	$(MAKE) parComp
	$(MAKE) unitTestRun

unitTestRun:	unitTest
	$<

cmrpTestRun:	cmrpTest
	$<

atpTestRun:	atpTest
	$<

atpTest2Run:	atpTest2
	$<

sentAtpTest:	atpTest.o $(typical_scenario_libraries)
		SE_HOME=/u/dcwong/SE_HOME; export SE_HOME; \
		/u/dcwong/SE_HOME/sentinel $(CC) \
		$(LDFLAGS) $^ $(LOADLIBES) $(ds_static_fortran_flags)  \
		-o sentAtpTest

sentAtpTestRun:	sentAtpTest
	SE_HOME=/u/dcwong/SE_HOME; export SE_HOME; sentAtpTest

sentinelUnitTest:	unitTest.o $(lib_objects) \
			../../wit/$(platform)/libwit.a \
			/lib/libm.a /lib/libxlf.a
	SE_HOME=/u/dcwong/SE_HOME; export SE_HOME; \
		/u/dcwong/SE_HOME/sentinel $(CC)  $(CPPFLAGS) $(CXXFLAGS) \
		$(LDFLAGS) $^ $(LOADLIBES) \
		$(ds_static_fortran_flags) \
		-L../../witutil/$(platform) -lwutl \
		-bI:/usr/lpp/xlf/lib/lowsys.exp -o $@

sentinelUnitTestRun:	sentinelUnitTest
	SE_HOME=/u/dcwong/SE_HOME; export SE_HOME; sentinelUnitTest
	

# DO NOT DELETE THIS LINE -- make depend depends on it.

