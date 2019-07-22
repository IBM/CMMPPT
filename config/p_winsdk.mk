#-------------------------------------------------------------------------------
# Platform-Specific Configuration Make File
#
# Platform ID: "winsdk"
#
# Platform:    Windows SDK, accessed through MSYS
#-------------------------------------------------------------------------------

# Additional flags for make depend:
#
ds_make_depend_flags = 

# sep is the directory-name seperator character
#
sep                  = /

# obj_suffix is the suffix for object files (Unix: o, Windows: obj)
#
obj_suffix           = obj

# cxx_suffix is the suffix for C++
# This is always C, because our source files are stored in RTC and therefore
# have the same names all platforms.
#
cxx_suffix           = C

# lib_suffix is the suffix for libraries/archives
#
lib_suffix           = lib

# exe_ending is the ending string for executables (Unix: nothing, Windows: .exe)
#
exe_ending           = .exe


#-------------------------------------------------------------------------------
# Compiler flag to specify the output file.
# We just let cl use its default output file name.
#-------------------------------------------------------------------------------

OUTPUT_OPTION        =

#-------------------------------------------------------------------------------
# Implicit rule to build a .obj file from a .C file:
#-------------------------------------------------------------------------------

%.obj: %.C
	$(COMPILE.C) $<

#-------------------------------------------------------------------------------
# Implicit rule to terminate with an error message when trying to build a .o
# file:
#-------------------------------------------------------------------------------

%.o:
	$(error WIT PROJECTS MAKE FILE ERROR:                  \
                "$@" cannot be built on the "winsdk" platform. \
                Consider building "$@bj" instead)              \

#-------------------------------------------------------------------------------
# Implicit rule to terminate with an error message when trying to build a .a
# file:
#-------------------------------------------------------------------------------

%.a:
	$(error WIT PROJECTS MAKE FILE ERROR:                  \
                "$@" cannot be built on the "winsdk" platform. \
                Consider building a ".lib" file instead)       \

#-------------------------------------------------------------------------------
# Implicit rule to terminate with an error message when trying to build an
# undefined target:
#-------------------------------------------------------------------------------

%:
	$(error WIT PROJECTS MAKE FILE ERROR:                \
                We have not defined a rule for building "$@" \
                on the "winsdk" platform)                    \

#-------------------------------------------------------------------------------

# Compilation flags:
#
ds_comp_debug_flags    = -Z7 -DDEBUG
ds_comp_profile_flags  = -O2
ds_comp_fast_flags     = -O2
ds_comp_released_flags = -O2


# Flags for linker:
#
ds_link_debug_flags    = -debug
ds_link_profile_flags  =
ds_link_fast_flags     =
ds_link_released_flags =

# Executables for generic commands
#
CC                     = cl
CXX                    = cl

ds_comp_arch_flags     = -TP -Za -EHs -nologo
ds_comp_std_flags      = -WX
ds_link_std_flags      = -nologo

#------------------------------------------------------------------------------
# ds_posix_threads_flag
# See the comment in p_linux.mk for the meaning of this macro.
#------------------------------------------------------------------------------

ds_posix_threads_flag  =
