# Copyright (C) 2002, Carnegie Mellon University and others.
# All Rights Reserved.
# This code is published under the Common Public License.

# $Id: Makefile,v 1.2 2002/07/24 02:48:40 andreasw Exp $
THIS_DIST_DIR   := OTHERS/ASL
#
#  The next lines automatically determine the root dir for the objects
#
slash := /
empty :=
space := $(empty) $(empty)
dirs  := $(subst /, ,$(THIS_DIST_DIR))
dots  := $(foreach dir,$(dirs), .. )
OBJROOTDIR := $(subst $(space),$(slash),$(strip $(dots)))

MAKEFILEDEFS = $(OBJROOTDIR)/Makefile.defs
MAKEFILETRGS = $(OBJROOTDIR)/Makefile.trgs

compdir = compdir

include $(MAKEFILEDEFS)

libampl   = amplsolver.a

alllns    = $(shell cd $(SRCDIR)/solvers; ls -d *)

CLEANOBJS = $(compdir) stdio1.h arith.h funcadd0.o

CLEANLIBS = $(libampl) 

$(libampl): compilesolver


ifeq ($(UNAME),CYGWIN)
compilesolver:
	set -x; \
	rm -rf $(compdir) ; \
	$(MKINSTALLDIRS) $(compdir); \
	cd $(compdir); \
	for file in $(alllns); \
	do \
	  $(LN) $(SRCDIR)/solvers/$$file $$file ;\
	done; \
	sed 's/a.out/a.exe/' makefile.u | sed 's#s/System_details/`uname -sr`/#s|System_details|`uname -sr`|#' >makefile ;\
	$(MAKE) -j 1 -f makefile $(ASLMAKEFLAGS) CC="$(CC)" ;\
	mv $(libampl) ..
#
	-cd $(compdir); mv stdio1.h arith.h funcadd0.o ..
#
	rm -rf $(compdir)
else
compilesolver:
	set -x; \
	rm -rf $(compdir) ; \
	$(MKINSTALLDIRS) $(compdir); \
	cd $(compdir); \
	for file in $(alllns); \
	do \
	  $(LN) $(SRCDIR)/solvers/$$file $$file ;\
	done; \
	$(MAKE) -j 1 -f makefile.u $(ASLMAKEFLAGS) CC="$(CC)" ;\
	mv $(libampl) ..
#
	-cd $(compdir); mv stdio1.h arith.h funcadd0.o ..
#
	rm -rf $(compdir)
endif

.PHONY: compilesolver

all: ; # Nothing to do for all ?

include $(MAKEFILETRGS)
