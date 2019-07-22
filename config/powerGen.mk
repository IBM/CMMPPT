#-------------------------------------------------------------------------------
# powerGen.mk
#
# Contains some of the Makefile content generic to all "Power" platforms.
#-------------------------------------------------------------------------------

ds_make_depend_flags = -I/usr/vacpp/include -I/

make_lib             = $(DS_DIR)bin/dsMakeLibAix5 -s


# sep is the directory-name seperator character (Unix: /, DOS: \)
#
sep			= /

# obj_suffix is the suffix for object files (Unix: o, DOS: obj)
#
obj_suffix		= o

# cxx_suffix is the suffix for C Plus Plus files (Unix: C, DOS: ?)
#
cxx_suffix		= C

# lib_suffix is the suffix for libraries/archives

lib_suffix		= a

# exe_ending is the ending string for executables (Unix: nothing, DOS: .exe)
#
exe_ending		= 


ds_comp_std_flags	= -qflag=i:i -qhalt=w

ds_comp_debug_flags 	= -g -DDEBUG
ds_comp_profile_flags   = -O -Q -qmaxmem=8192 -g -pg
ds_comp_fast_flags 	= -O -Q -qmaxmem=8192
ds_comp_released_flags  = -O -Q -qmaxmem=8192

# Big listings, if you want them, use
#
ds_comp_listing_flags = -qsource -qxref=full -qattr -qlist -qlistopt

# Flags for ar
#
ds_ar_update_flags	= vru

# Flags for linker
#
ds_link_debug_flags	= $(big_data) -g   
ds_link_profile_flags	= $(big_data) -lg -pg  
ds_link_fast_flags	= $(big_data)  
ds_link_released_flags	= $(big_data)  

ds_link_reloc_flags	= -g -r

ds_math_lib		= -lm

# Executables for generic commands
# Using GNU-make defaults for
# RM - the command to remove a file
# AR - the command for maintaining libraries and archives
#
CXX			= xlC
CC			= xlC
reloc_ld		= xlC
AR			= ar
RANLIB			= ranlib
