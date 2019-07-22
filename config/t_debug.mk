# Flags based on whether $exe_type is debug, profile, or product

ds_comp_flags	= $(ds_comp_basic_flags) $(ds_comp_debug_flags)
ds_link_flags	= $(ds_link_basic_flags) $(ds_link_debug_flags)
rw_lib		= -lrwtoolg
# For Open Interface
ND_LIB		= libdbg
