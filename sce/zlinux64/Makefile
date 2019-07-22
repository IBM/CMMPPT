# Makefile for the program for this architecture

# $Header: /gsa/yktgsa/projects/w/wit/devbase/sce/linux/Makefile,v 1.1 2003/08/05 13:13:17 dpc Exp $

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

all:	$(objects) scemain libsce.a unitTest


# Following is for doing compiles locally.  To use it, execute "make dotohs".
# The is useful because doing manipulating libraries and linking
# runs much faster when it is run on the machine that has the files, but
# it does not make much difference when compiling
dotohs:	$(objects)


# Following is for doing compiles locally and link on mfglog2.  It assumes
# you have an alias for setvars.[ck]sh and that your working directory is named
# after your userid or that there is a soft link to do the same thing.
remlink:	dotohs
	sleep 2		# Give NFS time to catch up
	rsh mfglog2 "setvars; cd /u/wit/$(USER)/sce/$(platform); pwd; nice $(MAKE) -k all"

# Following is for doing compiles remotely.  They assume
# that your working directory is named after your userid or that there is a soft 
# link to do the same thing.
#
# remcomp further assumes :
#    - Your default shell is csh.
#    - You have an alias for setvars.csh named setvars.
#    - /usr/gnu/bin is in your PATH
#
# remcompKsh further assumes:
#    - Your default shell is ksh.
# 
remcomp:
	rsh $(XLC_SERVER) "setvars; cd /u/wit/$(USER)/sce/$(platform); pwd; nice /usr/gnu/bin/make dotohs exe_type=$(exe_type); hostname"
	sleep 2         # Give NFS time to catch up

remcompKsh:
	rsh mfglog8 "export PATH=/usr/gnu/bin/:$(PATH); . /u/wit/bin/setvars.ksh; cd /u/wit/$(USER)/sce/$(platform); pwd; nice $(MAKE) dotohs exe_type=$(exe_type); hostname"
	sleep 2         # Give NFS time to catch up

../src/TAGS:	$(sources)
		cd ..$(sep)src; etags -C $(sources)

scemainRun:	scemain
	scemain


sentinelScemain:	scemain.o $(lib_objects) $(typical_scenario_libraries)
	SE_HOME=/u/dcwong/SE_HOME; export SE_HOME; \
		/u/dcwong/SE_HOME/sentinel $(CC) $(LDFLAGS) $^ $(LOADLIBES) \
		$(ds_static_fortran_flags) -o $@

ds_comp_std_flags =       \
   -Wno-ctor-dtor-privacy \


# DO NOT DELETE THIS LINE -- make depend depends on it.

