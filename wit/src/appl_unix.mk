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
    CoinModule$(exe_ending) \
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
# coin_lib_flags:
#    When WIT is to embed COIN, this is the list of flags for linking COIN into
#       a static library.
#    Otherwise this is the null string.
#
# coin_link_flags:
#    When WIT is to embed COIN, this is the list of additional flags needed for
#       building the WIT stand-alone executable.
#    Otherwise this is the null string.
#
# Prereqisite macros:
#    WIT_COIN_HOME
#       For the meaning of this macro, see the comment in appl.mk.
#
#    WIT_COIN_LINK_FLAGS
#       These are the additional link flags needed to build a WIT application
#       program (including the stand-alone executable) when COIN is embedded.
#       The choice of link flags depends on how COIN CLP and CBC were built.
#       Suggested possible values:
#          export WIT_COIN_LINK_FLAGS="-lz -llapack"
#          export WIT_COIN_LINK_FLAGS=\
#                 "-lcoinasl -lcoinglpk -lcoinlapack -lcoinmetis -lcoinmumps"
#          export WIT_COIN_LINK_FLAGS=
#-------------------------------------------------------------------------------

ifneq ($(WIT_COIN_HOME),)

   coin_lib_flags  =         \
      -L$(WIT_COIN_HOME)/lib \
      -lClp                  \
      -lCbc                  \
      -lOsiClp               \
      -lOsi                  \
      -lCgl                  \
      -lCoinUtils            \

   coin_link_flags = $(WIT_COIN_LINK_FLAGS)

else

   coin_lib_flags  =

   coin_link_flags =

endif

#-------------------------------------------------------------------------------
# Rule to build the CoinModule library object file.
#
# In COIN-embedded mode, this file contains:
#    The COIN client objects (CoinIf.o, CoinLpIf.o, CoinMipIf.o)
#    The relevent contents of the COIN libraries
#
# In COIN-not-embedded mode, this file contains:
#    The COIN client objects
#-------------------------------------------------------------------------------

CoinModule: $(coin_client_objects)
	$(reloc_ld) -o $@ $(ds_link_reloc_flags) $^ $(coin_lib_flags)

#-------------------------------------------------------------------------------
# cplex_lib_flags:
#    When WIT is to embed CPLEX, this is the list of flags for linking CPLEX into
#       a static library.
#    Otherwise this is the null string.
#
# cplex_link_flags:
#    When WIT is to embed CPLEX, this is the list of additional flags needed for
#       building the WIT stand-alone executable.
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

   cplex_lib_flags  = -L$(cplex_lib_dir) -lcplex

   cplex_link_flags = $(ds_posix_threads_flag)

else

   cplex_lib_flags  =

   cplex_link_flags =

endif

#-------------------------------------------------------------------------------
# Rule to build the CplexModule library object file.
#
# In CPLEX-embedded mode, this file contains:
#    CplexIf.o
#    The relevent contents of the CPLEX library
#
# In CPLEX-not-embedded mode, this file contains:
#    CplexIf.o
#-------------------------------------------------------------------------------

CplexModule: CplexIf.o
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

libwit.a:	$(lib_objects) MclModule CoinModule CplexModule BuildDate.o
	$(AR) $(ds_ar_update_flags) $@ $^
	$(RANLIB) $@


-lwit:	libwit.a

#-------------------------------------------------------------------------------
# Rule to build the WIT stand-alone executable:
#-------------------------------------------------------------------------------

wit:	wit.o libwit.a
	$(CXX) $(ds_link_flags) $^ $(coin_link_flags) $(cplex_link_flags) -o $@
