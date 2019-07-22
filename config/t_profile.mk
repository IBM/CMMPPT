# Flags based on whether $exe_type is debug, profile, or product

ds_comp_flags = $(ds_comp_basic_flags) $(ds_comp_profile_flags)
ds_link_flags = $(ds_link_basic_flags) $(ds_link_profile_flags)
rw_lib		= -lrwtool
# For Open Interface
ND_LIB		= lib
