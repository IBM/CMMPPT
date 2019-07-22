################################################################################ Warning - what are you doing here?
# Example cgl2 is work-in-progress. Not functional.
###############################################################################

# The location of the customized Makefiles
export CoinDir = $(shell cd ../..; pwd)
export MakefileDir := $(CoinDir)/Makefiles
include ${MakefileDir}/Makefile.coin
include ${MakefileDir}/Makefile.location

###############################################################################

.DELETE_ON_ERROR:

.PHONY: default cgl2 clean doc libdepend

default: cgl2

cgl2 : libdepend
	${MAKE} -f Makefile.cgl2 cgl2

libdepend:
	(cd ../../Cgl && ${MAKE} install)
	(cd ../../Osi/OsiClp && ${MAKE} install)

clean :
	rm -rf Junk
	rm -rf dep
	rm -rf $(UNAME)*
