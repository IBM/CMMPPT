#-------------------------------------------------------------------------------
# Platform-Specific Configuration Make File
#
# Platform ID: "linux"
#
# Platform:    Linux
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

LOADLIBES              += -lstdc++

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
AR                      = ar
RANLIB                  = ranlib

ds_comp_arch_flags      = 
ds_comp_std_flags	= 
ds_link_std_flags       =


#-------------------------------------------------------------------------------
# Macro ds_posix_threads_flag
#
# This is defined in the p_{platform}.mk files.
#
# On platforms on which CPLEX requires POSIX threads support, this is the
# flag to link-in POSIX threads support.
#
# On other platforms, this is the null string.
# 
# This macro should be used in the link command for building any WIT application
# program.
#
# For an example of how to use this macro correctly, see the "wit" rule in
# wit/src/appl_unix.mk, which specifies how to link the WIT stand-alone
# executable.
#-------------------------------------------------------------------------------

ds_posix_threads_flag   = -pthread -ldl
