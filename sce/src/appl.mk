# Make file for compiling and linking genGUI

# Notes:
# This makefile is included in makefiles in this directory and in 
# the ../$(platform) directories, so don't assume that the current
# directory is the one this makefile is in.

# $Header: /gsa/yktgsa/projects/w/wit/devbase/sce/src/appl.mk,v 1.9 2011/04/15 01:23:32 yktdev Exp $

include $(config_dir)ds.mk
include ../src/sources.mk

objects = scemain.$(obj_suffix) $(lib_objects)
OBJS    = $(objects)

# Get list of source files: either of the following two lines should work
# sources = $(subst %.$(obj_suffix),%.$(cxx_suffix),$objects)
sources = $(objects:.$(obj_suffix)=.$(cxx_suffix))


ld_path_flags = -L.

# CPPFLAGS - for C Pre-Processor
# CXXFLAGS - for C++ Compiler
CPPFLAGS        = -I$(src_dir). -I$(wituse_includes_dir) 
CFLAGS          = $(ds_comp_flags)
CXXFLAGS        = $(ds_comp_flags)
ld_paths		= $(ld_path_flags) \
		  -L$(SCENARIO_ROOT_DIR)/$(platform) \
		  -L$(WIT_ROOT_DIR)/$(platform) \
		  -L$(SCENARIO_ROOT_DIR)/../mcl/$(platform) 

LDFLAGS 	= $(ds_link_flags) $(ld_paths) $(ds_posix_threads_flag)


ifeq ($(platform),sun4)
	# Use -lrwtoolgs for now ... 
	LOADLIBES	= -lwitnl \
			  -lscen \
			  -lwutl \
			  -lmcl \
			  $(ds_math_lib) $(rw_lib) $(ds_fortran_lib) \
			  $(lex_yacc_lib)
else
	# Explicitly use shared rw lib
	LOADLIBES	+= -lmcl \
			   $(ds_math_lib) \
			   $(ds_posix_threads_flag) \
			   $(lex_yacc_lib)
endif

# SCE ODBC

sql_library_dir         = $(DB2INSTANCEPATH)/sqllib/lib
sql_include_dir         = $(DB2INSTANCEPATH)/sqllib/include

db2_api_lib             = -ldb2 -lc

odbc_compiler_flag	= -DSCE_ODBC


# PTRPATH is to be used with cfront-based compilers
PTRPATHOPTS     = -ptr./ptrepository -ptr../../wit/$(platform)/ptrepository

vpath %.$(cxx_suffix) $(src_dir)
vpath %.h             $(src_dir)
vpath %.y             $(src_dir)
vpath %.l             $(src_dir)

libsce.a:      $(lib_objects)
	$(AR) $(ds_ar_update_flags) $@ $^
	$(RANLIB) $@


# Libraries needed to make programs that use the scenario framework and
# implosion engine
# typical_scenario_libraries = \
#	$(SCENARIO_ROOT_DIR)/$(platform)/libscen.a \
#	$(WIT_ROOT_DIR)/$(platform)/libwitnl.a 

typical_scenario_libraries = \
	$(WIT_ROOT_DIR)/../scenario/$(platform)/libscen.a \
	$(WIT_ROOT_DIR)/../wit/$(platform)/libwit.a \
	$(WIT_ROOT_DIR)/../mcl/$(platform)/libmcl.a





sclibs= -lscen -lwitnl

scemain:	$(lib_objects) $(typical_scenario_libraries)
unitTest:	$(lib_objects) $(typical_scenario_libraries)

#
# Following are making SCE-ODBC version
#

# sce engine objects that use db objects

#objs_using_db = scenSce.o

objs_using_db =  \
		scemain.$(obj_suffix) \
		scenSce.$(obj_suffix) \
		sceFssMg.$(obj_suffix)

objs_using_db_target = 	 \
			scemainTarget.$(obj_suffix) \
			scenSceTarget.$(obj_suffix) \
			sceFssMgTarget.$(obj_suffix)

lib_db2 = $(sql_library_dir)/libdb2.a
debug = -g -DDEBUG -DRWDEBUG 
compile_flag = $(CPPFLAGS) $(CXXFLAGS) $(odbc_compiler_flag)

db_objects = \
	dbObj.$(obj_suffix)   \
	demandDB.$(obj_suffix) \
	dbLink.$(obj_suffix) \
	arg.$(obj_suffix) \
	dbProf.$(obj_suffix) \
	capAttDB.$(obj_suffix) \
	fsssDB.$(obj_suffix) \
	ipOrdDB.$(obj_suffix) \
	supplyDB.$(obj_suffix) \
	ppdfDB.$(obj_suffix)

build	:
	rm -f $(objs_using_db) libsce.a

odbc_scemain	:$(db_objects) $(objs_using_db_target) $(objects) libsce.a.target
	xlC -o scemain  $(compile_flag) $(debug) $(db_objects) $(objects) \
		-lm \
		-bI:/usr/lpp/xlC/lib/libC.imp \
		-L$(rw_library_dir) \
		-lrwtoolgs            \
		$(LDFLAGS) \
		$(LOADLIBES) \
		-L$(sql_library_dir) \
		$(db2_api_lib) \
		$(sclibs) \
		-bmaxdata:0x80000000 \
		-bloadmap:map

scemainTarget.o	:	scemain.C
	xlC -c -o scemain.o $(compile_flag) $(debug)  -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/scemain.C

scenSceTarget.o	:	scenSce.C
	xlC -c -o scenSce.o $(compile_flag) $(debug)  -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/scenSce.C

sceFssMgTarget.o	:	sceFssMg.C
	xlC -c -o sceFssMg.o $(compile_flag) $(debug)  -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/sceFssMg.C

libsce.a.target:	$(lib_objects) $(db_objects)
	$(AR) $(ds_ar_update_flags) $@ $^ 
	$(RANLIB) $@
	mv libsce.a.target libsce.a

arg.o	:	arg.C
	xlC -c $(compile_flag) $(debug)  -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/arg.C

dbObj.o	:	dbObj.C
	xlC -c $(compile_flag) $(debug) -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/dbObj.C

demandDB.o	:	demandDB.C
	xlC -c $(compile_flag) $(debug) -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/demandDB.C


dbLink.o	:	dbLink.C
	xlC -c $(compile_flag) $(debug) -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/dbLink.C


dbProf.o	:	dbProf.C
	xlC -c $(compile_flag) $(debug) -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/dbProf.C

capAttDB.o	:	capAttDB.C
	xlC -c $(compile_flag) $(debug) -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/capAttDB.C

fsssDB.o	:	fsssDB.C
	xlC -c $(compile_flag) $(debug) -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/fsssDB.C

ipOrdDB.o	:	ipOrdDB.C
	xlC -c $(compile_flag) $(debug) -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/ipOrdDB.C

supplyDB.o	:	supplyDB.C
	xlC -c $(compile_flag) $(debug) -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/supplyDB.C

ppdfDB.o	:	ppdfDB.C
	xlC -c $(compile_flag) $(debug) -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/ppdfDB.C

odbc_test	:	$(db_objects) unitTest1.o
	xlC -o unitTest1  $(compile_flag) $(debug) $(db_objects) unitTest1.o \
		-lm \
		-bI:/usr/lpp/xlC/lib/libC.imp \
		-L$(rw_library_dir) \
		-lrwtoolgs            \
		$(LDFLAGS) \
		$(LOADLIBES) \
		-L$(sql_library_dir) \
		$(db2_api_lib) \
		$(typical_scenario_libraries) \
		-bmaxdata:0x80000000 \
		-bloadmap:map

unitTest1.o	:	unitTest1.C
	xlC -c $(compile_flag) $(debug) -qflag=i:i -qhalt=w \
		-I$(sql_include_dir) \
		../src/unitTest1.C

clean_db_obj	:
		rm -f $(db_objects) 


