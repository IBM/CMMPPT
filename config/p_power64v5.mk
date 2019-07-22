#-------------------------------------------------------------------------------
# Platform-Specific Configuration Make File
#
# Platform ID: "power64v5"
#
# Platform:    AIX 5 or 6, 64-bit
#-------------------------------------------------------------------------------

export OBJECT_MODE=64

big_data = -q64 -bmaxdata:0x06FFFFFFFFFFFFF8

include $(config_dir)powerGen.mk

ds_comp_arch_flags     = -DRS6K -q64

# The 1500-029 warning occurs when compiling wit/src/List.C with -O.
# It warns of the inability to inline some code that's in a standard library
# header file.

ds_comp_std_flags      = -qhalt=w -qsuppress=1500-029

ds_link_std_flags      =

ds_comp_released_flags = -O -Q

ds_make_depend_flags   = -I/usr/vacpp/include -I/

make_lib               = $(DS_DIR)bin/dsMakeLibAix5 -s  

CXX                    = xlC128
CC                     = xlC128
reloc_ld               = xlC128

#------------------------------------------------------------------------------
# ds_posix_threads_flag
# See the comment in p_linux.mk for the meaning of this macro.
#------------------------------------------------------------------------------

ds_posix_threads_flag  = -lpthreads
