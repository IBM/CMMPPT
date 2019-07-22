# Copyright (C) 2002, Carnegie Mellon University and others.
# All Rights Reserved.
# This code is published under the Common Public License.

# $Id: Makefile,v 1.1 2002/06/07 20:55:05 andreasw Exp $
THIS_DIST_DIR   := IPOPT/CUTEr_interface
#
#  The next lines automatically determine the root dir for the objects
#
slash := /
empty :=
space := $(empty) $(empty)
dirs  := $(subst /, ,$(THIS_DIST_DIR))
dots  := $(foreach dir,$(dirs), .. )
OBJROOTDIR := $(subst $(space),$(slash),$(strip $(dots)))

# In this case we need to have the absolute path names because the CUTE
# link script will refer to the those path names
OBJROOTDIR := $(shell cd $(OBJROOTDIR); pwd)

MAKEFILEDEFS = $(OBJROOTDIR)/Makefile.defs
MAKEFILETRGS = $(OBJROOTDIR)/Makefile.trgs

include $(MAKEFILEDEFS)

auxilobjfiles = c_out.o \
        constr.o \
        iter_out.o \
        ma28_call.o \
        ma48_call.o

AUXILOBJS = $(addprefix  $(OBJROOTDIR)/$(DIST_IPOPT_AUXIL)/, \
         $(auxilobjfiles))

ipoptma_files = CUTEr_interface_main.f \
	eval_a.f \
	eval_c.f \
	eval_f.f \
	eval_g.f \
	eval_h.f \
	eval_hesslag_v.f \
	cute_init.f \
	eval_hesscon_v.f \
	eval_hessobj_v.f

LIBOBJS = $(AUXILOBJS)

LIB = libipoptcuter.a

ifdef SOLIBDIR
  SOLIB := $(patsubst %.a,%.so,$(LIB))
endif

CLEANLIBS = $(ipoptma_files) $(LIB) $(patsubst %.a,%.so,$(LIB)) ipoptma.f 

LIBS    = $(IPOPTLIB) $(TRONLIB) $(IPOPTLIB) $(CUTERLIB) $(LINALGLIBS)

ifndef MYCUTER
  dummy := $(error The environment variable MYCUTER has to be defined.)
endif

all: $(LIB) $(SOLIB) $(DEPLIBS) write-lib-list

ipoptma.f: $(ipoptma_files)
	cat $(ipoptma_files) >$@

write-lib-list:
	echo "$(LNFLAGS) $(LNKLIBS)" >$(MYCUTER)/double/lib/ipopt.liblist

#
#  Mainly for development:
#  Force check of libraries and object files in other directories
#
ifdef SOLIBDIR
$(patsubst %.a,%.so,$(IPOPTLIB)) $(AUXILOBJS): FORCE
else
$(IPOPTLIB) $(AUXILOBJS): FORCE
endif

#
#  Dependencies
#
incdir    = $(SRCROOTDIR)/$(DIST_IPOPT_IPOPT)
FINCFLAGS = $(patsubst %,-I%, $(incdir))

cincdirs  = $(ASLINCDIR) $(OBJROOTDIR)/$(DIST_OTHERS_ASL)
CINCFLAGS = $(patsubst %,-I%, $(cincdirs))

include $(MAKEFILETRGS)

$(ipoptma_files) : CUTE.INC
