# Static or shared libraries should be built (STATIC or SHARED)?
LibType := SHARED

# Select optimization (-O or -g). -O will be automatically bumped up to the 
# highest level of optimization the compiler supports. If want something in
# between then specify the exact level you want, e.g., -O1 or -O2
OptLevel := -g

# The path to the root of the install directory structure.
# include files will be installed in ${InstallDir}/include, libraries in 
# ${InstallDir}/lib, executables in ${InstallDir}/bin.
InstallDir := ..

# Specify where the BLAS/LaPack packages are. 
# List all the libraries in the package to be linked against.

BlasIncDir := /usr/include
BlasLibDir := /usr/lib
BlasLibName := libblas.so

LapackIncDir := /usr/include
LapackLibDir := /usr/lib
LapackLibName := liblapack.so

###############################################################################
# You should not need to edit below this line.
###############################################################################

DETECTLIBDIRS = ${LapackLibDir} ${BlasLibDir}
DETECTINCDIRS = ${LapackIncDir} ${BlasIncDir}
DETECTLIBNAMES = ${LapackLibName} ${BlasLibName}

###############################################################################
# The location of the customized Makefiles
MakefileDir := ../Makefiles

include ${MakefileDir}/Makefile.coin

###############################################################################

space:= $(empty) $(empty)
OptVersion := $(subst $(space),_,$(OptLevel))

TARGETDIR := $(UNAME)$(OptVersion)
DEPDIR := dep

VPATH := . : include : Junk : ${TARGETDIR} : ${DEPDIR}

###############################################################################

CXXFLAGS += $(OPTFLAG)
CXXFLAGS += -Iinclude -I../Common/include

FFFLAGS += -Iinclude -I../Common/include
FFFLAGS += $(OPTFLAG)

###############################################################################

LIBDIRS := $(DETECTLIBDIRS) $(InstallDir)/lib
LIBS    := libdfo.so $(DETECTLIBNAMES)

LDFLAGS := $(addprefix -L,$(LIBDIRS))
LDFLAGS += $(call ifprefix,$(SHLINKPREFIX),$(LIBDIRS))
LDFLAGS += $(patsubst lib%,-l%,$(basename $(LIBS)))

###############################################################################

LIBSRC = 
LIBSRC += dfo.f
LIBSRC += dfosolv.f
LIBSRC += ptinit.f 
LIBSRC += nbuild.f 
LIBSRC += evalnp.f 
LIBSRC += mbuild.f
LIBSRC += fd.f  
LIBSRC += gterms.f 
LIBSRC += zero.f 
LIBSRC += funobj.f  
LIBSRC += mvalue.f
LIBSRC += impmod.f  
LIBSRC += xgnew.f 
LIBSRC += getnp.f 
LIBSRC += ptrepl.f 
LIBSRC += ptexch.f 
LIBSRC += nextnp.f
LIBSRC += ranlux.f

LIBOBJ=$(addprefix $(TARGETDIR)/, $(LIBSRC:.f=.o))
LIBDEP=$(addprefix $(DEPDIR)/, $(LIBSRC:.f=.d))

###############################################################################

TESTSRC = dfotest.f
TESTOBJ = $(addprefix $(TARGETDIR)/, $(TESTSRC:.f=.o))
TESTDEP = $(addprefix $(DEPDIR)/, $(TESTSRC:.f=.d))

###############################################################################

ifeq (1,0)
    MINTRSRC = mintr_npsol.f
    MINTROBJ = $(addprefix $(TARGETDIR)/, $(MINTRSRC:.f=.o))
    MINTRDEP = $(addprefix $(DEPDIR)/, $(MINTRSRC:.f=.d))
    DETECTLIBDIRS += ../npsol
    DETECTLIBNAMES += libnpsol.so
else
    MINTRSRC = mintr_cfsqp.c cfsqp.c qld.c
    MINTROBJ = $(addprefix $(TARGETDIR)/, $(MINTRSRC:.c=.o))
    MINTRDEP = $(addprefix $(DEPDIR)/, $(MINTRSRC:.c=.d))
    DETECTLIBDIRS += ../appl
    DETECTLIBNAMES += libappl.so
endif

###############################################################################

CXXFLAGS += $(addprefix -I,$(DETECTINCDIRS))
CXXFLAGS += $(addprefix -D,$(DETECTDEFINES))
FFFLAGS += $(addprefix -I,$(DETECTINCDIRS))
FFFLAGS += $(addprefix -D,$(DETECTDEFINES))

###############################################################################
# This one must be right before the rules

include ${MakefileDir}/Makefile.rules

###############################################################################

.DELETE_ON_ERROR:

.PHONY: default all install libdfo unitTest

default: install
all:	 unitTest

install: libdfo
	@echo "Installing libraries..."
	@mkdir -p ${InstallDir}/lib
	@${CP} $(TARGETDIR)/libdfo$(OptVersion)$(LIBEXT) ${InstallDir}/lib
	@rm -f ${InstallDir}/lib/libdfo$(LIBEXT)
	@cd ${InstallDir}/lib; \
		ln -s libdfo$(OptVersion)$(LIBEXT) libdfo$(LIBEXT)

###############################################################################

libdfo:	$(TARGETDIR)/libdfo$(OptVersion)$(LIBEXT)

$(TARGETDIR)/libdfo$(OptVersion)$(LIBEXT): $(LIBOBJ)
	@rm -rf Junk
	@echo ""
	@echo Creating library $(notdir $@)
	@mkdir -p $(TARGETDIR)
	@rm -f $@
	$(LD) $@ $(LIBLDFLAGS) $(LIBOBJ)

###############################################################################

unitTest: $(TARGETDIR)/unitTest

$(TARGETDIR)/unitTest : install $(TESTOBJ) $(MINTROBJ)
	@rm -rf Junk
	@echo ""
	@echo Creating unitTest
	@mkdir -p $(TARGETDIR)
	@rm -f $@
	$(FF) $(FFFLAGS) -o $@ $(TESTOBJ) $(MINTROBJ) $(LDFLAGS) $(SYSLD) -lm

###############################################################################

clean :
	rm -rf Junk
	@rm -rf $(DEPDIR)
	@rm -rf $(TARGETDIR)

###############################################################################

%::
	@mkdir -p Junk
	touch Junk/$(notdir $@)

###############################################################################

-include $(LIBDEP)
-include $(TESTDEP)
-include $(MINTRDEP)
