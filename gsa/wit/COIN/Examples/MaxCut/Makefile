# The location of the customized Makefiles
export CoinDir = $(shell cd ../..; pwd)
export MakefileDir := $(CoinDir)/Makefiles

# Set this Make variable to TRUE if you want to compile the MaxCut example
# with OSL as well (besides Clp and Vol). Note that in this case you have to
# have COIN_libOsl and COIN_libOsiOsl uncommented in COIN/Makefile.location
# and you may have to specify the location of OSL.

MC_COMPILE_WITH_OSL = FALSE
#MC_COMPILE_WITH_OSL = TRUE
export MC_COMPILE_WITH_OSL

###############################################################################

.DELETE_ON_ERROR:

.PHONY: default maxcut clean doc libdepend

default: maxcut

maxcut : libdepend
	${MAKE} -f Makefile.maxcut bcps

libdepend:
	(cd ../../Osi/OsiClp && ${MAKE} install)
ifeq ($(MC_COMPILE_WITH_OSL),TRUE)
	(cd ../../Osi/OsiOsl && ${MAKE} install)
endif
	(cd ../../Osi/OsiVol && ${MAKE} install)

clean :
	@rm -rf Junk
	${MAKE} -f Makefile.maxcut clean

doc:
	(sed -e "s|INPUT[\t ]*=[\t ]*.|INPUT = . ${CoinDir}/Bcp|g" \
		$(MakefileDir)/doxygen.conf > doxygen.conf; \
	 doxygen doxygen.conf)
