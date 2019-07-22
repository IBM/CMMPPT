# The location of the customized Makefiles
export CoinDir = $(shell cd ../..; pwd)
export MakefileDir := $(CoinDir)/Makefiles
include ${MakefileDir}/Makefile.coin
include ${MakefileDir}/Makefile.location

###############################################################################

.DELETE_ON_ERROR:

.PHONY: default cgl1 clean doc libdepend

default: cgl1

cgl1 : libdepend
	${MAKE} -f Makefile.cgl1 cgl1

libdepend:
	(cd ../../Cgl && ${MAKE} install)
	(cd ../../Osi/OsiClp && ${MAKE} install)

clean :
	rm -rf Junk
	rm -rf dep
	rm -rf $(UNAME)*
