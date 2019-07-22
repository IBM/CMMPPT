# Copyright (C) 2002, Carnegie Mellon University and others.
# All Rights Reserved.
# This code is published under the Common Public License.

# $Id: Makefile,v 1.1 2002/05/02 18:52:15 andreasw Exp $
THIS_DIST_DIR   := IPOPT/auxil
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

OBJS = \
	c_out.o \
	constr.o \
	eval_h_dummy.o \
	eval_hesslag_v_dummy.o \
	iter_out.o \
	ma28_call.o \
	ma48_call.o

CLEANOBJS = $(OBJS)

all: ;    # don't do anything - create object files only when needed

#
#  Dependencies
#
incdir    = $(SRCROOTDIR)/$(DIST_IPOPT_IPOPT)
FINCFLAGS = $(patsubst %,-I%, $(incdir))
$(OBJS): $(incdir)/IPOPT.INC

include $(MAKEFILETRGS)
