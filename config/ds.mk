#-------------------------------------------------------------------------------
# ds.mk
#
# This is the general make include file for WIT-related software.
#
# This include file is for things that are generally useful when making
# applications for Decision Support.  It is intended to be included in all
# makefiles that are used to make WIT, programs that use the WIT API, and 
# later, programs that are in the Logistics Framework.
#
# IMPORTANT NOTES:
# Makefiles should include p_$(platform).mk before this file
#	because $(sep) and a lot of other things are defined there
# Makefiles should define $(appl_name) before calling this.  It is the
#   name of the directory where the application's subtree is kept, such as
#   "wit", "pmrp", or other immediate descendants of "$(DSD_DIR)".
# Makefiles should define $(sources) before using the rules in this
# file. $(sources) sould contain the list of .C and .c files that
# are in $(appl_name)'s source directory.
#
# Makefiles that use this make files should define the following
# variables so that the rules for constructing various types of files
# do the right thing.  Included with the variables are definitions for a 
# simple application that uses the WIT API
# CPPFLAGS	= -I$(wit_api_include_dir).
# CFLAGS  	= $(ds_comp_flags) 
# C++FLAGS 	= $(ds_comp_flags)
# LDFLAGS 	= $(ds_link_flags) $(wit_api_link_devbase_dir_flag)
# LOADLIBES	= $(wit_api_lib) $(ds_math_lib) $(ds_fortran_lib) $(db2_api_lib)
#
# Macros with _api_ in them are for use when making programs that use the API
#
# A few words about names:
# Names with _comp_ in them have to do with compiling
# Names with _link_ in them have to do with linking
# Names with _debug_ in them have to do with generating debuggable exe's
# Names with _profile_ in them have to do with ugenerating profilable exe's
# Names with _product_ in them have to do with exe's for production (includes
#					options to compile with code optim-
#					ization, no internal symbol table, etc)
# Names with _std_ in them are for options that are used no matter what
#					kind of exe is wanted
# Names with wit_api_ in them are used when making programs that use the WIT 
# 					API
# Names that end with _lib are linker options specifying libraries to search.
#	The proper from is "-lx", as in "-lm" or "-lxlc".
#	Gnu Make know that when it sees such a name in a dependency list, it
#	should make sure the file named "libx.a" in the library search path
#	is up-to-date.
#
# Note: to see the default SUFFIXES, type make -p -f /dev/null
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# ds_comp_macro_flags
#
# This defines some C/C++ macros that are used by some of the source code.
# The macro flags are specified, unless skip_comp_macro_flags is defined as YES.
# skip_comp_macro_flags is defined in the appl.mk files, as needed.
#-------------------------------------------------------------------------------

ifeq ($(skip_comp_macro_flags),YES)

ds_comp_macro_flags  =

else

ds_comp_macro_flags  = -DEXE_TYPE=$(exe_type) -DPLATFORM=$(platform)

endif

#-------------------------------------------------------------------------------

ds_comp_basic_flags  = $(ds_comp_arch_flags)  \
                       $(ds_comp_std_flags)   \
                       $(ds_comp_macro_flags) \

ds_link_basic_flags  = $(ds_link_std_flags)

wit_api_lib          = -lwit

db2_api_lib          = -ldb2 -lc

src_dir              = ..$(sep)src$(sep)

wit_api_includes_dir = ..$(sep)..$(sep)wit$(sep)src

wituse_includes_dir  = ..$(sep)..

#-------------------------------------------------------------------------------
# Default exe_type
#
# exe_type should be debug, profile, fast, or released.
#-------------------------------------------------------------------------------

exe_type = debug

#-------------------------------------------------------------------------------
# Verify that exe_type has been set to a valid value.
#-------------------------------------------------------------------------------

empty                :=
space                := $(empty) $(empty)
space_exe_type_space := $(space)$(exe_type)$(space)
valid_exe_types      := $(space)debug$(space)fast$(space)profile$(space)released$(space)
find_result          := $(findstring $(space_exe_type_space),$(valid_exe_types))
stripped_find_result := $(strip $(find_result))

ifneq ($(exe_type),$(stripped_find_result))

   $(error WIT PROJECTS MAKE FILE ERROR:                            \
           exe_type was specified as "$(exe_type)".                 \
           It needs to be one of the following: $(valid_exe_types)) \

endif

#-------------------------------------------------------------------------------

include $(config_dir)t_$(exe_type).mk

include $(config_dir)sfDefine.mk

include $(config_dir)legacy.mk

#-------------------------------------------------------------------------------
# Rule to do "make depend":
#-------------------------------------------------------------------------------

all_make_depend_flags = -f../$(platform)/Makefile    \
                        --                           \
                        $(ds_make_depend_flags)      \
                        -I$(config_dir)dummy_headers \
                        $(CPPFLAGS)                  \
                        $(ds_comp_flags)             \
                        --                           \
                        $(sources)                   \


depend: 
	chmod u+w Makefile
	cd ../src;$(X11R5_BIN_DIR)makedepend $(all_make_depend_flags)

#-------------------------------------------------------------------------------
# Rule to do "make clean":
#-------------------------------------------------------------------------------

make_clean_files = *.$(obj_suffix)       \
                   *.$(lib_suffix)       \
                   ptrepository/*        \
                   $(exes)               \
                   *.so                  \
                   *.dll                 \
                   *.rco                 \
                   *.dat                 \
                   *.class               \
                   *.ilk                 \
                   *.pdb                 \
                   $(exe_type_file)      \
                   $(intermediate_files) \


clean:	
	-$(RM) $(make_clean_files)
