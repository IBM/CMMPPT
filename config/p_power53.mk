#-------------------------------------------------------------------------------
# Platform-Specific Configuration Make File
#
# Platform ID: "power53"
#
# Platform:    AIX 5 or 6, 32-bit
#-------------------------------------------------------------------------------

big_data               = -bmaxdata:0x80000000

include $(config_dir)powerGen.mk

ds_comp_arch_flags     = -DRS6K

ds_comp_std_flags      = -qflag=i:i           \
                         -qhalt=w             \
                         -qsuppress=1540-0308 \
                         -qsuppress=1540-0309 \
                         -qsuppress=1540-0306 \
                         -qsuppress=1540-0152 \
                         -qsuppress=1540-0080 \
                         -qlanglvl=noansifor  \

ds_link_std_flags      =

ds_comp_released_flags = -O -Q

ds_make_depend_flags   = -I/usr/vacpp/include -I/
make_lib               = $(DS_DIR)bin/dsMakeLibAix5 -s

CC                     = /usr/vacpp/bin/xlC
CXX                    = /usr/vacpp/bin/xlC
reloc_ld               = /usr/vacpp/bin/xlC

#------------------------------------------------------------------------------
# ds_posix_threads_flag
# See the comment in p_linux.mk for the meaning of this macro.
#------------------------------------------------------------------------------

ds_posix_threads_flag = -lpthreads
