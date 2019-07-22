#-------------------------------------------------------------------------------
# Platform-Specific Configuration Make File
#
# Platform ID: "zlinux64"
#
# Platform:    z/Linux, 64-bit
#-------------------------------------------------------------------------------

ds_make_depend_flags    = 

make_lib                =  ar

# sep is the directory-name seperator character
#
sep                     = /

# obj_suffix is the suffix for object files
#
obj_suffix             = o

# cxx_suffix is the suffix for C++ files
#
cxx_suffix             = C

# lib_suffix is the suffix for libraries/archives
#
lib_suffix             = a

# exe_ending is the ending string for executables (Unix: nothing, Windows: .exe)
#
exe_ending             = 

ds_comp_debug_flags    = -g -DDEBUG
ds_comp_profile_flags  = -O -g -pg
ds_comp_fast_flags     = -O
ds_comp_released_flags = -O

# Big listings, if you want them, use
#
ds_comp_listing_flags = -qsource -qxref=full -qattr -qlist -qlistopt

# Flags for ar
#
ds_ar_update_flags	= vru

# Flags for linker
#
ds_link_debug_flags	= -g 
ds_link_profile_flags	= -lg -pg 
ds_link_fast_flags	=
ds_link_released_flags	=

LOADLIBES += -lstdc++

ds_link_reloc_flags	= -g -r

ds_math_lib		= -lm

# Executables for generic commands
# Using GNU-make defaults for
# RM - the command to remove a file
# AR - the command for maintaining libraries and archives
#
CC                      = /usr/bin/gcc
CXX                     = /usr/bin/g++
reloc_ld                = ld
AR			= ar
RANLIB			= ranlib

ds_comp_arch_flags	= -m64
ds_comp_std_flags	= 
ds_link_std_flags	=

#------------------------------------------------------------------------------
# ds_posix_threads_flag
# See the comment in p_linux.mk for the meaning of this macro.
#------------------------------------------------------------------------------

ds_posix_threads_flag   = -pthread
