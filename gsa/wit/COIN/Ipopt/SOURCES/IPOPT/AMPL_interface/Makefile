# Copyright (C) 2002, Carnegie Mellon University and others.
# All Rights Reserved.
# This code is published under the Common Public License.

# $Id: Makefile,v 1.1 2002/05/02 18:52:15 andreasw Exp $
THIS_DIST_DIR   := IPOPT/AMPL_interface
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

EXE      = ipopt
MAINOBJS = ipoptAMPL.o get_amplmu.o ipopt_getdata.o

auxilobjfiles = c_out.o \
	constr.o \
	iter_out.o \
	ma28_call.o \
	ma48_call.o

AUXILOBJS = $(addprefix  $(OBJROOTDIR)/$(DIST_IPOPT_AUXIL)/, \
	 $(auxilobjfiles))

DATA    = PARAMS.DAT.example
DATADIR = $(SRCROOTDIR)/$(DIST_IPOPT_DOC)

CLEANOBJS = $(MAINOBJS)
CLEANEXES = $(EXE)
CLEANDATA = $(DATA) IPOPT.output

OBJS    = $(MAINOBJS) $(AUXILOBJS)
LIBS    = $(IPOPTLIB) $(TRONLIB) $(IPOPTLIB) $(LINALGLIBS) $(ASLLIB) $(DLLIB)

all: $(EXE) $(DATA)

$(EXE): $(OBJS) $(DEPLIBS)
	$(CC) $(LNFLAGS) -o $@ $(OBJS) $(LNKLIBS) $(CCLNLIBS)

$(DATA):
	cp $(DATADIR)/$@ .
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
#  Make sure we can run stuff in parallel
#
break := $(firstword, $(AUXILOBJS))
break : $(filter-out $break, $(AUXILOBJS))

#
#  Dependencies
#
incdir    = $(SRCROOTDIR)/$(DIST_IPOPT_IPOPT)
FINCFLAGS = $(patsubst %,-I%, $(incdir))

cincdirs  = $(ASLINCDIR) $(OBJROOTDIR)/$(DIST_OTHERS_ASL)
CINCFLAGS = $(patsubst %,-I%, $(cincdirs))


$(MAINOBJS): $(incdir)/IPOPT.INC

$(OBJS): $(ASLLIB)

include $(MAKEFILETRGS)
