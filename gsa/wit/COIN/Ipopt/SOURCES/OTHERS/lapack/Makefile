# Copyright (C) 2002, Carnegie Mellon University and others.
# All Rights Reserved.
# This code is published under the Common Public License.

# $Id: Makefile,v 1.1 2002/05/02 18:52:20 andreasw Exp $
THIS_DIST_DIR   := OTHERS/lapack
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

LIBOBJS = dgesv.o \
	dgetf2.o \
	dgetrf.o \
	dgetrs.o \
	dlabad.o \
	dlacon.o \
	dlacpy.o \
	dlae2.o \
	dlaebz.o \
	dlaev2.o \
	dlagtf.o \
	dlagts.o \
	dlamch.o \
	dlansp.o \
	dlanst.o \
	dlapy2.o \
	dlaqsp.o \
	dlarf.o \
	dlarfg.o \
	dlarnv.o \
	dlartg.o \
	dlaruv.o \
	dlascl.o \
	dlaset.o \
	dlasr.o \
	dlasrt.o \
	dlassq.o \
	dlaswp.o \
	dlatps.o \
	dopgtr.o \
	dopmtr.o \
	dorg2l.o \
	dorg2r.o \
	dppcon.o \
	dppequ.o \
	dpprfs.o \
	dppsv.o \
	dppsvx.o \
	dpptrf.o \
	dpptrs.o \
	drscl.o \
	dspev.o \
	dspevx.o \
	dsptrd.o \
	dstebz.o \
	dstein.o \
	dsteqr.o \
	dsterf.o \
	ieeeck.o \
	ilaenv.o

LIB = liblapack.a

ifdef SOLIBDIR
  SOLIB := $(patsubst %.a,%.so,$(LIB))
endif

CLEANOBJS = $(LIBOBJS)
CLEANLIBS = $(LIB) $(patsubst %.a,%.so,$(LIB))

all: $(LIB)  $(patsubst %.a, %.so, $(LIB))

include $(MAKEFILETRGS)
