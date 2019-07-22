# Copyright (C) 2002, Carnegie Mellon University and others.
# All Rights Reserved.
# This code is published under the Common Public License.

# $Id: Makefile,v 1.1 2002/05/02 18:52:20 andreasw Exp $
THIS_DIST_DIR   := OTHERS/TRON
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

include $(MAKEFILEDEFS)

#
#  Path to source code
#
VPATH = $(TRONSRCDIR)/tron : $(TRONSRCDIR)/icf

LIBOBJS = dtron_p.o \
	dspcg_p.o \
	dcauchy_p.o \
	dtrpcg_p.o \
	dbreakpt.o \
	dgpstep_p.o \
	dprsrch_p.o \
	dtrqsol.o \
	dmid.o \
	dicfs.o \
	dssyax.o \
	dstrsol.o \
	dicf.o \
	ihsort.o \
	insort.o \
	dsel2.o \
	dgpnrm2.o

LIB = libtron.a

ifdef SOLIBDIR
  SOLIB := $(patsubst %.a,%.so,$(LIB))
endif

PATCHSRC = dtron_p.f dspcg_p.f dcauchy_p.f dtrpcg_p.f dgpstep_p.f dprsrch_p.f

CLEANOBJS = $(LIBOBJS)
CLEANLIBS = $(LIB) $(patsubst %.a, %.so, $(LIB))
CLEANDATA = $(PATCHSRC)

all: $(LIB) $(SOLIB)

#
#  The following targets copy the source code, and apply possibly some
#  patches
#
$(PATCHSRC):
	cp $(TRONSRCDIR)/tron/$(patsubst %_p.f,%.f,$@) $@
	$(PATCH) $(SRCROOTDIR)/$(THIS_DIST_DIR)/$*.patch

#
#  Dependencies
#
incdir    = $(SRCROOTDIR)/$(DIST_IPOPT_IPOPT)
FINCFLAGS = $(patsubst %,-I%, $(incdir))

include $(MAKEFILETRGS)
