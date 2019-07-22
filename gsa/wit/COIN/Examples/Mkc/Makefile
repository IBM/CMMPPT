# The location of the customized Makefiles
export CoinDir = $(shell cd ../..; pwd)
export MakefileDir := $(CoinDir)/Makefiles

# export USE_OSL := MKC_COMPILE_WITH_OSL

COIN_OPT=-O

###############################################################################

.DELETE_ON_ERROR:

.PHONY: default mkc emkc esmkc clean doc libdepend

default: mkc

mkc : libdepend
	${MAKE} -f Makefile.mkc bcps

smkc : libdepend
	${MAKE} -f Makefile.mkc sbcps

emkc : libdepend
	${MAKE} -f Makefile.mkc ebcps

esmkc : libdepend
	${MAKE} -f Makefile.mkc esbcps

libdepend:
ifeq (${USE_OSL},MKC_COMPILE_WITH_OSL)
	(cd ../../Osi/OsiOsl && ${MAKE} OptLevel=${COIN_OPT} install)
endif
	(cd ../../Osi/OsiClp && ${MAKE} OptLevel=${COIN_OPT} install)
	(cd ../../Osi/OsiVol && ${MAKE} OptLevel=${COIN_OPT} install)

clean :
	@rm -rf Junk
	${MAKE} -f Makefile.mkc clean
