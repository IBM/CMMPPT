#------------------------------------------------------------------------------
# Make file for platform-independent aspects of building WIT-J's C++ native
# library.
#------------------------------------------------------------------------------

# Skip the "macro" compilation flags:
#
skip_comp_macro_flags = YES


include $(config_dir)ds.mk

#------------------------------------------------------------------------------
# The list of files in lib_objects and headers comes from the include below:
#------------------------------------------------------------------------------

include ../src/sources.mk

cxx_objects   = $(lib_objects)

#-------------------------------------------------------------------------------
# All source files
# This macro is used by make depend.
#-------------------------------------------------------------------------------
#
sources       = $(objects:.$(obj_suffix)=.$(cxx_suffix))

objects       = $(cxx_objects) 

#-------------------------------------------------------------------------------

ld_path_flags = -L.

# CPPFLAGS - for C Pre-Processor
# CXXFLAGS - for C++ Compiler
#
CPPFLAGS      = -I$(src_dir) -I$(JAVA_HOME)/include -I../../../wit/src 
CXXFLAGS      = $(ds_comp_flags)
LDFLAGS       = $(ds_link_flags) $(ld_path_flags) 


vpath %.$(cxx_suffix) $(src_dir)
vpath %.h             $(src_dir)
