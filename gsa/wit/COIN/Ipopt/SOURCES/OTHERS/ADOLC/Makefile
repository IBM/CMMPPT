# Copyright (C) 2002, Carnegie Mellon University and others.
# All Rights Reserved.
# This code is published under the Common Public License.

# $Id: Makefile,v 1.2 2002/07/24 02:48:01 andreasw Exp $
THIS_DIST_DIR   := OTHERS/ADOLC
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

libad   = libad.a
libadso = libad.so

lndirs     = $(filter-out Makefile README, $(notdir $(wildcard $(SRCDIR)/*)))
alllndirs  = $(shell cd $(SRCDIR); $(FIND) $(lndirs) -type d -print )
alllnfiles = $(shell cd $(SRCDIR); $(FIND) $(lndirs) -type f -print )

CLEANOBJS = $(compdir)

CLEANLIBS = $(libad) $(libadso)

ifdef SOLIBDIR
  soarall = $(SOAR) $(SOARFLAGS) $(libadso) \
	`$(FIND) $(compdir) -name '*.o' ` ;\
	cp $(libadso) $(SOLIBDIR)
endif

$(libad) $(libadso): compileadolc

ifneq ($(UNAME),CYGWIN)
compileadolc:
	set -x; \
	rm -rf $(compdir) ;\
	$(MKINSTALLDIRS) $(compdir); \
	cd $(compdir); \
	for file in $(alllndirs); \
	do \
	  $(MKINSTALLDIRS) $$file;\
	done; \
	for file in $(alllnfiles); \
	do \
	  $(LN) $(SRCDIR)/$$file $$file ;\
	done; \
	cd INS ;\
	$(MAKE) $(ADOLCINSTARGET) ;\
	cd ../SRC ;\
	$(MAKE) -j 1;\
	mv $(libad) ../..
#
	$(soarall)
	rm -rf $(compdir)
else
compileadolc:
	set -x; \
	rm -rf $(compdir) ;\
	$(MKINSTALLDIRS) $(compdir); \
	cd $(compdir); \
	for file in $(alllndirs); \
	do \
	  $(MKINSTALLDIRS) $$file;\
	done; \
	for file in $(alllnfiles); \
	do \
	  $(LN) $(SRCDIR)/$$file $$file ;\
	done; \
	cd INS ;\
	sed 's/CFLAG =/CFLAG = -mno-cygwin /' gnu_comp > gnu_comp0 ;\
	rm gnu_comp ;\
	mv gnu_comp0 gnu_comp ;\
	$(MAKE) $(ADOLCINSTARGET) ;\
	cd ../SRC ;\
	$(MAKE) -j 1;\
	mv $(libad) ../..
#
	$(soarall)
	rm -rf $(compdir)
endif

.PHONY: compileadolc

all: ; # Nothing to do for all ?

include $(MAKEFILETRGS)
