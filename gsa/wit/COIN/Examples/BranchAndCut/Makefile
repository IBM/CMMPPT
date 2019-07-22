# Set where the COIN root directory is.
# In this example it's two levels above, but it could be anywhere
export CoinDir = $(shell cd ../..; pwd)

# The location of the customized Makefiles
export MakefileDir := $(CoinDir)/Makefiles

###############################################################################

.DELETE_ON_ERROR:

.PHONY: default bc clean doc libdepend

default: bc

bc : libdepend
	${MAKE} -f Makefile.bc bcps

libdepend:
	(cd ${CoinDir}/Osi/OsiClp && ${MAKE} install)
	(cd ${CoinDir}/Cgl && ${MAKE} install)

clean :
	@rm -rf Junk
	${MAKE} -f Makefile.bc clean
