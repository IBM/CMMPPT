#------------------------------------------------------------------------------
# Make file for platform-independent aspects of building MCL
#------------------------------------------------------------------------------

appl_name             = mcl

# Skip the "macro" compilation flags:
#
skip_comp_macro_flags = YES

include $(config_dir)ds.mk

exes	              = unitTest$(exe_ending) 

intermediate_files    = 

# The list of files in lib_objects and headers comes from the include below:
#
include ..$(sep)src$(sep)sources.mk

cxx_objects           = unitTest.$(obj_suffix) $(lib_objects)

sources               = $(sort $(cxx_objects:.$(obj_suffix)=.$(cxx_suffix))) 

objects               = $(cxx_objects) 


ld_path_flags         = -L.

CPPFLAGS	      = -I$(wituse_includes_dir) 
CFLAGS		      = $(ds_comp_flags)
CXXFLAGS	      = $(ds_comp_flags)

ld_paths	      = $(ld_path_flags) 
LDFLAGS		      = $(ds_link_flags) $(ld_paths)

vpath %.$(cxx_suffix) $(src_dir)
vpath %.h             $(src_dir)

#------------------------------------------------------------------------------
# Rule for "make all".
# Note the required tab character on the second line of this rule.
#------------------------------------------------------------------------------

all:	unitTest$(exe_ending)
	
