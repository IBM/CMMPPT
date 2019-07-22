# Copyright (C) 2002, Carnegie Mellon University and others.
# All Rights Reserved.
# This code is published under the Common Public License.

# $Id: Makefile,v 1.1 2002/05/02 18:52:20 andreasw Exp $
THIS_DIST_DIR   := OTHERS/DASPK
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

LIBOBJS = ddaspk.o dlinpk.o daux.o

LIB = libdaspk.a

ifdef SOLIBDIR
  SOLIB := $(patsubst %.a,%.so,$(LIB))
endif

CLEANOBJS = $(LIBOBJS)
CLEANLIBS = $(LIB) $(patsubst %.a, %.so, $(LIB))

all: $(LIB) $(SOLIB)

include $(MAKEFILETRGS)
