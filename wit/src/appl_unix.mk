#------------------------------------------------------------------------------
# Make file for aspects of building WIT that follow a Unix-like convention,
# but are otherwise platform-independent
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# List of "executables" for WIT.
# Used by make clean
#-------------------------------------------------------------------------------

exes =                      \
           wit$(exe_ending) \
     MclModule$(exe_ending) \
   CplexModule$(exe_ending) \

#-------------------------------------------------------------------------------
# Rule to build the MclModule library object file.
#
# This file contains:
#    The MCL client object files
#    The relevent contents of the MCL library
#-------------------------------------------------------------------------------

mcl_lib_dir   = ..$(sep)..$(sep)mcl$(sep)$(platform)

mcl_lib       = $(mcl_lib_dir)$(sep)libmcl.a

mcl_mod_input = $(mcl_client_objects) -L$(mcl_lib_dir) -lmcl

MclModule: $(mcl_client_objects) $(mcl_lib)
	$(reloc_ld) -o $@ $(ds_link_reloc_flags) $(mcl_mod_input)

#-------------------------------------------------------------------------------
# cplex_lib_flags:
#    When WIT is to embed CPLEX, this is the set of flags for linking CPLEX into
#       a static library.
#    Otherwise this is the null string.
#
# Prereqisite macros:
#
#    WIT_CPLEX_HOME
#    cplex_lib_dir
#
#    For the meaning of these macros, see the comment in appl.mk.
#-------------------------------------------------------------------------------

ifneq ($(WIT_CPLEX_HOME),)

   cplex_lib_flags = -L$(cplex_lib_dir) -lcplex

else

   cplex_lib_flags =

endif

#-------------------------------------------------------------------------------
# Rule to build the CplexModule library object file.
#
# In CPLEX-embedded mode, this file contains:
#    CplexMgr.o
#    The relevent contents of the CPLEX library
#
# In CPLEX-not-embedded mode, this file contains:
#    CplexMgr.o
#-------------------------------------------------------------------------------

CplexModule: CplexMgr.o
	$(reloc_ld) -o $@ $(ds_link_reloc_flags) $^ $(cplex_lib_flags)

#-------------------------------------------------------------------------------
# BuildDate.C contains the implementation of a function that returns the date
# on which the WIT library was built. Consequently, it must be recompiled
# whenever any aspect of WIT has been modified, and so its object file is
# considered to be dependent on all other aspects of WIT:
#-------------------------------------------------------------------------------

BuildDate.o:	$(lib_objects) MclModule CplexModule wit.o

#-------------------------------------------------------------------------------
# Rule to build the WIT static library:
#-------------------------------------------------------------------------------

libwit.a:	$(lib_objects) MclModule CplexModule BuildDate.o
	$(AR) $(ds_ar_update_flags) $@ $^
	$(RANLIB) $@


-lwit:	libwit.a

#-------------------------------------------------------------------------------
# Rule to build the WIT stand-alone executable:
#-------------------------------------------------------------------------------

wit:	wit.o libwit.a
	$(CXX) $(ds_link_flags) $^ $(ds_posix_threads_flag) -o $@
