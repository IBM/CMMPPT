# The location of the customized Makefiles
export CoinDir = $(shell cd ../..; pwd)
export MakefileDir := $(CoinDir)/Makefiles
include ${MakefileDir}/Makefile.coin
include ${MakefileDir}/Makefile.location

###############################################################################

.DELETE_ON_ERROR:

.PHONY: default vollp clean doc libdepend

default: vollp

vollp : libdepend
	${MAKE} -f Makefile.vollp vollp

libdepend:
	(cd ../../Coin && ${MAKE} install)
	(cd ../../Vol && ${MAKE} install)

clean :
	rm -rf Junk
	rm -rf dep
	rm -rf $(UNAME)*
