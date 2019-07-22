# The location of the customized Makefiles
export CoinDir = $(shell cd ../..; pwd)
export MakefileDir := $(CoinDir)/Makefiles

###############################################################################

.DELETE_ON_ERROR:

.PHONY: default ufl clean libdepend

default: ufl

ufl : libdepend
	${MAKE} -f Makefile.ufl ufl

libdepend:
	(cd ../../Vol && ${MAKE} install)

clean :
	rm -rf Junk
	rm -rf dep
	rm -rf $(UNAME)*
