# The location of the COIN source files
export CoinDir = $(shell cd ../..; pwd)

# Select optimization (-O or -g). -O will be automatically bumped up to the 
# highest level of optimization the compiler supports. If want something in
# between then specify the exact level you want, e.g., -O1 or -O2
OptLevel := -O6

##############################################################################
# You should not need to edit below this line.
##############################################################################

export MakefileDir := $(CoinDir)/Makefiles
include ${MakefileDir}/Makefile.coin

##############################################################################

CXXFLAGS += $(OPTFLAG)

EXSRC := VolVolume.cpp lp.cpp lpc.cpp reader.cpp

###############################################################################

space:= $(empty) $(empty)
OptVersion := $(subst $(space),_,$(OptLevel))

TARGETDIR := $(UNAME)$(OptVersion)
DEPDIR := dep

VPATH := . : include : $(CoinDir)/Vol : $(CoinDir)/Vol/include
VPATH += Junk : ${TARGETDIR} : ${DEPDIR}

CXXFLAGS += -I. -I$(CoinDir)/Vol/include

###############################################################################

EXOBJ := $(addprefix $(TARGETDIR)/, $(EXSRC:.cpp=.o))
EXDEP := $(addprefix $(DEPDIR)/, $(EXSRC:.cpp=.d))

###############################################################################
# This one must be right before the rules

include ${MakefileDir}/Makefile.rules

.DELETE_ON_ERROR:

.PHONY: default volume-lp clean update

default: volume-lp

volume-lp: $(TARGETDIR)/volume-lp

$(TARGETDIR)/volume-lp : $(EXOBJ)
	@echo ""
	@echo Creating example volume-lp
	@mkdir -p $(TARGETDIR)
	@rm -f $@
	@$(CXX) $(CXXFLAGS) -o $@ $(EXOBJ) $(LDFLAGS) $(SYSLD) -lm
	@ln -s $@ volume-lp

###############################################################################

%::
	@mkdir -p Junk
	touch Junk/$(notdir $@)

clean :
	rm -rf Junk
	rm -rf dep
	rm -rf $(UNAME)*
	rm -f volume-lp

###############################################################################

-include $(EXDEP)
