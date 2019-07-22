# Make file for compiling and linking WIT

# Notes:
# This makefile is included in makefiles in this directory and in 
# the ../$(platform) directories, so don't assume that the current
# directory is the one this makefile is in.

# $Header: /gsa/yktgsa/projects/w/wit/devbase/scenario/src/appl.mk,v 1.7 2009/12/01 20:17:42 ervolina Exp $

include $(config_dir)ds.mk

lib_objects = \
	abFctyAp.$(obj_suffix) \
	algorthm.$(obj_suffix) \
	attTable.$(obj_suffix) \
	attrInfo.$(obj_suffix) \
	backlgSS.$(obj_suffix) \
	boolFS.$(obj_suffix) \
	calendar.$(obj_suffix) \
	collflt.$(obj_suffix) \
	consSS.$(obj_suffix) \
	critPaSS.$(obj_suffix) \
	cumSS.$(obj_suffix) \
	cycleSS.$(obj_suffix) \
	dateTime.$(obj_suffix) \
	deltaSS.$(obj_suffix) \
	demVolSS.$(obj_suffix) \
	demand.$(obj_suffix) \
	demandSS.$(obj_suffix) \
	demdNmFS.$(obj_suffix) \
	demdPerd.$(obj_suffix) \
	demdPnPC.$(obj_suffix) \
	demdSchP.$(obj_suffix) \
	demdSchd.$(obj_suffix) \
	dif.$(obj_suffix) \
	difAbstr.$(obj_suffix) \
	difEssSc.$(obj_suffix) \
	difEssen.$(obj_suffix) \
	difImpl.$(obj_suffix) \
	difPWcmr.$(obj_suffix) \
	difSFile.$(obj_suffix) \
	difSP.$(obj_suffix) \
	difWitR.$(obj_suffix) \
	error.$(obj_suffix) \
	excessSS.$(obj_suffix) \
	explode.$(obj_suffix) \
	fctyDflt.$(obj_suffix) \
	fctyTest.$(obj_suffix) \
	filtStrt.$(obj_suffix) \
	fssCtrl.$(obj_suffix) \
	fssPrdSS.$(obj_suffix) \
	fssSS.$(obj_suffix) \
	generVc.$(obj_suffix) \
	implode.$(obj_suffix) \
	inPrFile.$(obj_suffix) \
	initProb.$(obj_suffix) \
	itemTimV.$(obj_suffix) \
	lastPrbS.$(obj_suffix) \
	machdep.$(obj_suffix) \
	model.$(obj_suffix) \
	offsetSS.$(obj_suffix) \
	outInter.$(obj_suffix) \
	pairComp.$(obj_suffix) \
	part.$(obj_suffix) \
	partCtFS.$(obj_suffix) \
	partSS.$(obj_suffix) \
	partSchP.$(obj_suffix) \
	partSchd.$(obj_suffix) \
	priorSS.$(obj_suffix) \
	prodSS.$(obj_suffix) \
	refCount.$(obj_suffix) \
	reqSS.$(obj_suffix) \
	RWToSTLHelpers.$(obj_suffix) \
	scBdDflt.$(obj_suffix) \
	scBdTest.$(obj_suffix) \
	scenBld.$(obj_suffix) \
	scenDrct.$(obj_suffix) \
	scenP.$(obj_suffix) \
	scenSnFS.$(obj_suffix) \
	scenario.$(obj_suffix) \
	schedLF.$(obj_suffix) \
	schFctry.$(obj_suffix) \
	schdDesc.$(obj_suffix) \
	schdSnFS.$(obj_suffix) \
	schdSnSS.$(obj_suffix) \
	schdStrt.$(obj_suffix) \
	scrapSS.$(obj_suffix) \
	scTestP.$(obj_suffix) \
	shipSS.$(obj_suffix) \
	shpRewSS.$(obj_suffix) \
	sOfDemds.$(obj_suffix) \
	sOfParts.$(obj_suffix) \
	sortSchP.$(obj_suffix) \
	sortSchd.$(obj_suffix) \
	stockSS.$(obj_suffix) \
	supplySS.$(obj_suffix) \
	testScen.$(obj_suffix) \
	timeVecs.$(obj_suffix) \
	usageSS.$(obj_suffix) \
	usrInMod.$(obj_suffix) \
	witRunVn.$(obj_suffix) \
	wrkgProb.$(obj_suffix) \
	zeroTvFS.$(obj_suffix) 

#objects = $(lib_objects) unitTest.$(obj_suffix) cmrpTest.$(obj_suffix) \
#	atpTest.$(obj_suffix) atpTest2.$(obj_suffix) cfcTest.$(obj_suffix) \
#	cmrpTwo.$(obj_suffix)
objects = $(lib_objects) unitTest.$(obj_suffix) 

# Get list of source files: either of the following two lines should work
sources = $(objects:.$(obj_suffix)=.$(cxx_suffix))

headers =        \
	abFctyAp.h \
	algorthm.h \
	attTable.h \
	attrInfo.h \
	attrSnPC.h \
	bacSSATP.h \
	backlgSS.h \
	boolFS.h \
	calendar.h \
	collflt.h \
	consSS.h \
	constant.h \
	critPaSS.h \
	cumSS.h \
	cycleSS.h \
	deltaSS.h \
	demSSATP.h \
	demVolSS.h \
	demand.h \
	demandSS.h \
	demdBgPC.h \
	demdNmFS.h \
	demdPerd.h \
	demdPnPC.h \
	demdPrPC.h \
	demdSchP.h \
	demdSchd.h \
	demdVgPC.h \
	descSchd.h \
	dfa.h \
	dif.h \
	difAbstr.h \
	difEssSc.h \
	difEssen.h \
	difImpl.h \
	difSFile.h \
	difSP.h \
	difTest.h \
	difVWcmr.h \
	difWitD.h \
	difWitR.h \
 	error.h \
	excessSS.h \
	explode.h \
	fctyDflt.h \
	fctyDifT.h \
	fctyTest.h \
	filtStrt.h \
	fssCtrl.h \
        fssPrdSS.h \
	fssSS.h \
	generVc.h \
	hashUtil.h \
	idnumber.h \
	implode.h \
	inPrFile.h \
	initProb.h \
	ipDifT.h \
	item.h \
	itemTimV.h \
	key.h \
	lastPrbS.h \
	machdep.h \
	model.h \
	msAttrIt.h \
	msItemIt.h \
	multDSch.h \
	multPSch.h \
	multiSch.h \
	offsetSS.h \
	outInter.h \
 	pairComp.h \
	part.h \
	partCtFS.h \
	partNmFS.h \
	partSS.h \
	partSchP.h \
	partSchd.h \
	pcatPC.h \
	priorSS.h \
	prodSS.h \
	refCount.h \
	reqSS.h \
	sOfDemds.h \
	sOfPaFS.h \
	sOfParts.h \
	scATPP.h \
	scBdATP.h \
	scBdDflt.h \
	scBdDifT.h \
	scBdTest.h \
	scDifTP.h \
	scTestP.h \
	scenATP.h \
	scenBld.h \
	scenFac.h \
	scenDifT.h \
	scenDrct.h \
	scenP.h \
	scenSnFS.h \
	scenario.h \
	schFctry.h \
	schdDeSt.h \
	schdDesc.h \
	schdSnFS.h \
	schdSnPC.h \
	schdSnSS.h \
	schdStrt.h \
	sched.h \
	scrapSS.h \
	shipSS.h \
	shpBndSS.h \
	shpRewSS.h \
	shpSSATP.h \
	sopSnPC.h \
	sortSchP.h \
	sortSchd.h \
	sortVecE.h \
	stateATP.h \
	stockSS.h \
	supSSATP.h \
	supplySS.h \
	testScen.h \
	timVComp.h \
	timeVecs.h \
	tvElemPC.h \
	usageSS.h \
	usrInMod.h \
	util.h \
	witRunVn.h \
	wrkgProb.h \
	yktscen.h \
	zeroTvFS.h 

ld_path_flags = -L.

# CPPFLAGS - for C Pre-Processor
# CXXFLAGS - for C++ Compiler
CPPFLAGS	= -I$(src_dir). -I$(wituse_includes_dir) -I$(wit_api_includes_dir)
CFLAGS		= $(ds_comp_flags)
CXXFLAGS	= $(ds_comp_flags)
ld_paths		= $(ld_path_flags) \
		  -L$(SCENARIO_ROOT_DIR)/$(platform) \
		  -L$(WIT_ROOT_DIR)/$(platform) \
		  -L../../witutil/$(platform)
LDFLAGS 	= $(ds_link_flags) $(ld_paths) 

ifeq ($(platform),sun4)
	# Use -lrwtoolgs for now ... 
	LOADLIBES	= $(ds_math_lib) $(rw_lib) $(ds_fortran_lib) \
			  $(lex_yacc_lib)
else
	# Explicitly use shared rw lib
	LOADLIBES	+= $(ds_math_lib) \
			   $(lex_yacc_lib)
endif

# PTRPATH is to be used with cfront-based compilers
PTRPATHOPTS	= -ptr./ptrepository -ptr../../wit/$(platform)/ptrepository

vpath %.$(cxx_suffix) $(src_dir)

libscen.a:	$(lib_objects)
#	$(make_lib) $(ds_ar_update_flags) $(ld_paths) \
#		-lwutl $(LOADLIBES) \
#		$@ $^
	$(AR) $(ds_ar_update_flags) $@ $^
	$(RANLIB) $@

# The following rule is just here in case multiple names for the
# same directorie (e.g. soft-links) cause make to not figure out how to
# make $(SCENARIO_ROOT_DIR)/$(platform)/libscen.a
$(SCENARIO_ROOT_DIR)/$(platform)/libscen.a: libscen.a

# Libraries needed to make programs that use the scenario framework and
# implosion engine
typical_scenario_libraries = \
	$(SCENARIO_ROOT_DIR)/$(platform)/libscen.a \
	$(WIT_ROOT_DIR)/$(platform)/libwit.a \
        ../../mcl/$(platform)/libmcl.a \
        ../../witutil/$(platform)/libwutl.a

unitTest:	$(typical_scenario_libraries)
heurTest:	$(typical_scenario_libraries)
sOPTest:	$(typical_scenario_libraries)
cmrpTest:	$(typical_scenario_libraries)
cmrpTwo:	$(typical_scenario_libraries)
atpTest:	$(typical_scenario_libraries)
atpTest2:	$(typical_scenario_libraries)
cfcTest:	$(typical_scenario_libraries)
difTest:	$(typical_scenario_libraries)
difTest2:	$(typical_scenario_libraries)
difTest3:	$(typical_scenario_libraries)
