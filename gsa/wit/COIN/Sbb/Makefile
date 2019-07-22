# Look at and if necessary edit the following files:
# - ../Makefiles/Makefile.location
# - Makefile.Sbb

###############################################################################

export CoinDir := $(shell cd ..; pwd)
export MakefileDir := $(CoinDir)/Makefiles
include ${MakefileDir}/Makefile.coin
include ${MakefileDir}/Makefile.location

###############################################################################

.DELETE_ON_ERROR:

.PHONY: default install clean library unitTest libdepend libSbb doc

default: install
libSbb: library

libSbb: library

install library: libdepend
	${MAKE} -f Makefile.Sbb $@

libdepend:
	(cd $(CoinDir)/Coin && $(MAKE) install)
	(cd $(CoinDir)/Clp && $(MAKE) install)
	(cd $(CoinDir)/Osi/OsiClp && $(MAKE) install)
	(cd $(CoinDir)/Cgl && $(MAKE) install)

unitTest: 
	(cd Test && ${MAKE} unitTest)

clean: 
	@rm -rf Junk
	@rm -rf $(UNAME)*
	@rm -rf dep
	@rm -rf Test/Junk
	@rm -rf Test/$(UNAME)*
	@rm -rf Test/dep
	@rm -f sbb

doc:
	doxygen $(MakefileDir)/doxygen.conf
