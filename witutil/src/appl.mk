# Make file for compiling and linking WIT

# Notes:
# This makefile is included in makefiles in this directory and in 
# the ../$(platform) directories, so don't assume that the current
# directory is the one this makefile is in.

# $Header: /gsa/yktgsa/projects/w/wit/devbase/witutil/src/appl.mk,v 1.4 2006/04/19 20:38:36 rjw Exp $

include $(config_dir)ds.mk


lib_objects = \
	witU5.$(obj_suffix) \
	allocMrp.$(obj_suffix) \
	fullExpl.$(obj_suffix) 

objects = \
	shrtFall.$(obj_suffix) \
	shrtFll2.$(obj_suffix) \
	unitTest.$(obj_suffix) \
	whereUse.$(obj_suffix) \
	aMrpMain.$(obj_suffix) \
	$(lib_objects) 

# Get list of source files
sources = $(objects:.$(obj_suffix)=.$(cxx_suffix))


headers =        \
	witU5.h   \
	shrtFll2.h   \
	whereUse.h   \
	allocMrp.h   \
	fullExpl.h

ld_path_flags = -L.

# CPPFLAGS - for C Pre-Processor
# CXXFLAGS - for C++ Compiler
CPPFLAGS	= -I$(src_dir). -I$(wituse_includes_dir) -I$(wit_api_includes_dir) -I../.. 
CFLAGS		= $(ds_comp_flags)
CXXFLAGS	= $(ds_comp_flags)
LDFLAGS1 	= $(ds_link_flags) $(ld_path_flags) 

ifeq ($(platform),sun4)
LDFLAGS2 	= -L..$(sep)..$(sep)wit$(sep)$(platform) \
		  -L..$(sep)..$(sep)mcl$(sep)$(platform) \
		  -L$(rw_library_dir)
else
LDFLAGS2 	= -L..$(sep)..$(sep)wit$(sep)$(platform) \
		  -L$(rw_library_dir)
endif

# last line above added by wally 5/23/96 because libwit.a currently needs
# RW code that isn't in libwit.a (especially on Sun's)
LDFLAGS 	= $(LDFLAGS1) $(LDFLAGS2) 
# LOADLIBES	= $(wit_api_lib) $(ds_math_lib)  \
# 		  $(ds_fortran_lib) \
# 		  $(lex_yacc_lib) \
# 		  $(rw_lib)

ifeq ($(platform),sun4)
	# Use -lrwtoolgs for now ... 
	LOADLIBES	= -lwitnl \
			  -lmcl \
			  $(ds_math_lib) $(rw_lib) $(ds_fortran_lib) \
			  $(lex_yacc_lib)
else
LOADLIBES	+= $(wit_api_lib) $(ds_math_lib)

endif

# last two above added by wally 5/23/96 because libwit.a currently needs
# RW code that isn't in libwit.a (especially on Sun's)

# $(ds_fortran_lib) $(rw_lib)
# PTRPATH is to be used with cfront-based compilers
PTRPATHOPTS	= -ptr./ptrepository -ptr../../wit/$(platform)/ptrepository

vpath %.$(cxx_suffix) $(src_dir)

libwutl.a:	$(lib_objects) 
	$(AR) $(ds_ar_update_flags) $@ $^
	$(RANLIB) $@

#shrtFall:	$(lib_objects) ../../wit/$(platform)/libwit.a 
shrtFall:	$(lib_objects) ../../wit/$(platform)/libwit.a 
shrtFll2:	$(lib_objects) ../../wit/$(platform)/libwit.a 
unitTest:	$(lib_objects) ../../wit/$(platform)/libwit.a 
whereUse:	$(lib_objects) ../../wit/$(platform)/libwit.a 
aMrpMain:	$(lib_objects) ../../wit/$(platform)/libwit.a 

