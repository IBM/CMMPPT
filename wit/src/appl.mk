#------------------------------------------------------------------------------
# Make file for platform-independent aspects of building WIT
#------------------------------------------------------------------------------

appl_name = wit

# Skip the "macro" compilation flags:
#
skip_comp_macro_flags = YES

include $(config_dir)ds.mk

#-------------------------------------------------------------------------------
# The list of files in lib_objects and headers comes from the include below:
#-------------------------------------------------------------------------------

include ../src/sources.mk

#-------------------------------------------------------------------------------
# Object files that invoke MCL.
# (Note that these are not listed in lib_objects.)
#-------------------------------------------------------------------------------

mcl_client_objects =        \
       MsgFac.$(obj_suffix) \
       MsgMgr.$(obj_suffix) \
   MsgBuilder.$(obj_suffix) \

#-------------------------------------------------------------------------------
# Object files corresponding to C++ source files:
#-------------------------------------------------------------------------------

cxx_objects =              \
   $(lib_objects)          \
   $(mcl_client_objects)   \
   CplexIf.$(obj_suffix)   \
   BuildDate.$(obj_suffix) \
   wit.$(obj_suffix)       \

#-------------------------------------------------------------------------------
# All source files
# This macro is used by make depend.
# witLexer.l and witParse.y are not included: witLexer.C and witParse.C are 
# good enough for this purpose.
#-------------------------------------------------------------------------------

sources = $(sort $(cxx_objects:.$(obj_suffix)=.$(cxx_suffix)))

objects = $(cxx_objects) 

#-------------------------------------------------------------------------------

ld_path_flags   = -L.

# CPPFLAGS - for C Pre-Processor
# CXXFLAGS - for C++ Compiler
#
CPPFLAGS        = -I$(src_dir) -I$(wituse_includes_dir)

CFLAGS          = $(ds_comp_flags)
CXXFLAGS        = $(ds_comp_flags)


ld_paths        = $(ld_path_flags) -L$(WIT_ROOT_DIR)$(sep)$(platform)

LDFLAGS         = $(ds_link_flags) $(ld_paths)


ifeq ($(findstring linux,$(platform)),linux)

   LOADLIBES    =

else

   LOADLIBES    = $(ds_math_lib)

endif

vpath %.$(cxx_suffix) $(src_dir)
vpath %.h             $(src_dir)
vpath %.y             $(src_dir)
vpath %.l             $(src_dir)

#-------------------------------------------------------------------------------
# Special rule to compile Session.C:
#
# Macro:
#    session_macro_flags:
#       Defines some C/C++ macros that are used by Session.C.
#-------------------------------------------------------------------------------

session_macro_flags = -DEXE_TYPE=$(exe_type) -DPLATFORM=$(platform)

Session.$(obj_suffix):	Session.$(cxx_suffix)
			$(COMPILE.C) $(OUTPUT_OPTION) $< $(session_macro_flags)

#-------------------------------------------------------------------------------
# Macros to facilitate building WIT, when CPLEX is to be embedded.
#
# Macro:
#    cplex_lib_dir:
#       When WIT is to embed CPLEX, this is the directory where the CPLEX
#       library can be found. Otherwise this is the null string.
#
# Prereqisite macros:
#
#    WIT_CPLEX_HOME:
#       This is an environment variable.
#       If CPLEX is to be embedded into WIT, WIT_CPLEX_HOME should be defined as
#       the path where CPLEX files are found.
#       If CPLEX is not to be embedded into WIT, WIT_CPLEX_HOME should be left
#       undefined, or defined as the null string.
#
#    cplex_lib_subdir:
#       This is defined in the p_{platform}.mk files.
#       This is the default subdirectory in which the CPLEX library can be found
#       on the current platform (when CPLEX is installed).
#       It only includes the part of the directory path after /lib/.
#
#    WIT_CPLEX_LIB_SUBDIR:
#       This is an environment variable.
#       If this is defined as a non-null string, then cplex_lib_subdir is
#       overridden and this value is used instead.
#       Thus its value should be the subdirectory in which the CPLEX library
#       can be found.
#       It only includes the part of the directory path after /lib/.
#       If this variable is undefined or defined as a null string, it has no
#       effect.
#-------------------------------------------------------------------------------

ifneq ($(WIT_CPLEX_HOME),)

      #-------------------------
      # CPLEX is to be embedded.
      #-------------------------

   ifneq ($(WIT_CPLEX_LIB_SUBDIR),)

      cplex_lib_dir = $(WIT_CPLEX_HOME)/lib/$(WIT_CPLEX_LIB_SUBDIR)

   else

      cplex_lib_dir = $(WIT_CPLEX_HOME)/lib/$(cplex_lib_subdir)

   endif


else

      #-----------------------------
      # CPLEX is not to be embedded.
      #-----------------------------

   cplex_lib_dir    =

endif

#-------------------------------------------------------------------------------
# Special rule to compile CplexIf.C:
#
# Only applies when WIT is to embed CPLEX.
#
# Macro:
#
#    comp_cplex_flags
#       The special compilation flags for compiling CplexIf.C.
#-------------------------------------------------------------------------------

ifneq ($(WIT_CPLEX_HOME),)

comp_cplex_flags = -DCPLEX_EMBEDDED -I$(WIT_CPLEX_HOME)/include/ilcplex

CplexIf.$(obj_suffix):	CplexIf.$(cxx_suffix)
			$(COMPILE.C) $(OUTPUT_OPTION) $< $(comp_cplex_flags)

endif

#------------------------------------------------------------------------------
# Rule for "make all".
# Note the required tab character on the second line of this rule.
#------------------------------------------------------------------------------

all:	wit$(exe_ending)
	
