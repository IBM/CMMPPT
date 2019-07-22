# Make file for compiling and linking WIT

# Notes:
# This makefile is included in makefiles in this directory and in 
# the ../$(platform) directories, so don't assume that the current
# directory is the one this makefile is in.

# $Header: /gsa/yktgsa/projects/w/wit/devbase/eso2/src/appl.mk,v 1.10 2008/07/23 08:42:52 fasano Exp $

include $(config_dir)ds.mk


lib_objects = \
	BCP_string.$(obj_suffix) \
	createModel.$(obj_suffix) \
	eso2Problem.$(obj_suffix) \
	featureAppData.$(obj_suffix) \
	featureXrefAppData.$(obj_suffix) \
	loosePartAppData.$(obj_suffix) \
	mtmAppData.$(obj_suffix) \
	mtmDemanBomAppData.$(obj_suffix) \
	pipSequenceTriple.$(obj_suffix) \
	partDemandAppData.$(obj_suffix) \
	probAppData.$(obj_suffix) \
	partInterplant.$(obj_suffix) \
	scoFile.$(obj_suffix) 

objects = \
	unitTest.$(obj_suffix) \
	eso2.$(obj_suffix) \
	$(lib_objects) 

# Get list of source files
cxx_suffix=cpp
sources = $(objects:.$(obj_suffix)=.$(cxx_suffix))


headers =        \
	BCP_string.h   \
	CoinFinite.h   \
	CoinFloatEqual.h   \
	alternatePartData.h   \
	featureData.h   \
	globalAttrFile.h   \
	loosePartData.h   \
	mtmData.h   \
	mtmDemanBomData.h   \
	mtmInterplantData.h   \
	mtmInterplantOverrideData.h   \
	pipSequenceTriple.h \
	partDemandAppData.h \
	partDemandData.h   \
	partInterplantData.h   \
	partInterplantOverrideData.h   \
	scoFile.h   \
	appData.h   \
	featureAppData.h   \
	featureXrefAppData.h   \
	loosePartApp.h   \
	mtmAppData.h   \
	mtmDemanBomAppData.h   \
	probAppData.h   \
	eso2Problem.h   \
	partInterplant.h  

ld_path_flags = -L.

# CPPFLAGS - for C Pre-Processor
# CXXFLAGS - for C++ Compiler
#CXX=/usr/vacpp/bin/xlC
#CXX=xlC
#CC=/usr/vacpp/bin/xlC
#CC=xlC
#CPPFLAGS	= -I$(src_dir). -I$(wituse_includes_dir) -I$(wit_api_includes_dir) -I../.. 
CPPFLAGS        = -I$(src_dir). \
		-I$(general_include_dir) \
		-I$(general_include_dir)$(sep)coin \
		-I$(wituse_includes_dir) \
		-I$(wit_api_includes_dir) \
		-I..$(sep).. \
		-I.

CFLAGS		= $(ds_comp_flags)
CXXFLAGS	= $(ds_comp_flags)
LDFLAGS1 	= $(ds_link_flags) $(ld_path_flags) 
#reloc_ld=/usr/vacpp/bin/xlC
ds_make_depend_flags        =

ifeq ($(platform),sun4)
LDFLAGS2 	= -L..$(sep)..$(sep)wit$(sep)$(platform) \
		  -L..$(sep)..$(sep)mcl$(sep)$(platform) 
else
LDFLAGS2 	= -L..$(sep)..$(sep)wit$(sep)$(platform) 
endif

LDFLAGS		= $(LDFLAGS1) $(LDFLAGS2)

ifeq ($(platform),sun4)
	# Use -lrwtoolgs for now ... 
#	LOADLIBES	= -lwitnl \
	LOADLIBES	= -lwit \
			  -lmcl \
			  $(ds_osl_lib) \
			  $(ds_math_lib) $(rw_lib) $(ds_fortran_lib) \
			  $(lex_yacc_lib)
else
LOADLIBES	+= $(wit_api_lib) $(ds_math_lib)  \
		  $(ds_fortran_lib) \
		  $(lex_yacc_lib) 
endif

# last two above added by wally 5/23/96 because libwit.a currently needs
# RW code that isn't in libwit.a (especially on Sun's)

vpath %.$(cxx_suffix) $(src_dir)

libeso2.a:	$(lib_objects) 
	$(AR) $(ds_ar_update_flags) $@ $^
	$(RANLIB) $@

eso2:		$(lib_objects) ../../wit/$(platform)/libwit.a 
fileConvOct03:	$(lib_objects) ../../wit/$(platform)/libwit.a 
purge:		$(lib_objects) ../../wit/$(platform)/libwit.a 
unitTest:	$(lib_objects) ../../wit/$(platform)/libwit.a 

