# Copyright (C) 2002, Carnegie Mellon University and others.
# All Rights Reserved.
# This code is published under the Common Public License.

# $Id: Makefile,v 1.3 2002/09/11 05:01:10 andreasw Exp $
THIS_DIST_DIR   := OTHERS/HSL
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
# Determine list of objects based on Macro definitions in DEFS
#  (from Makefile.defs)
#

LIBOBJS =

ifeq (,$(findstring -DNO_MA27,$(DEFS)))
LIBOBJS += ma27ad.o
endif

ifeq (,$(findstring -DNO_MA47,$(DEFS)))
LIBOBJS += ma47ad.o
endif

ifeq (,$(findstring -DNO_MA57,$(DEFS)))
LIBOBJS += ma57ad.o \
	mc34ad.o \
	mc47ad.o \
	mc59ad.o \
	mc71ad.o
endif

ifeq (,$(findstring -DNO_MA28,$(DEFS)))
LIBOBJS += ma28ad.o \
	ma30ad.o \
	mc22ad.o \
	mc23ad.o \
	mc24ad.o
endif

ifeq (,$(findstring -DNO_MA48,$(DEFS)))
LIBOBJS += ma48ad.o \
	ma50ad.o \
	ma51ad.o \
	mc41ad.o
endif

ifneq (-DNO_MA57 -DNO_MA48, $(findstring -DNO_MA57,$(DEFS)) $(findstring -DNO_MA48,$(DEFS)))
LIBOBJS += $(FD05AD).o
endif

ifneq (-DNO_MA28 -DNO_MA48, $(findstring -DNO_MA28,$(DEFS)) $(findstring -DNO_MA48,$(DEFS)))
LIBOBJS += mc13dd.o \
	mc21ad.o
endif

ifeq (,$(findstring -DNO_MC30,$(DEFS)))
LIBOBJS += mc30ad.o
endif

ifeq (,$(findstring -DNO_MC19,$(DEFS)))
LIBOBJS += mc19ad.o
endif

ifeq (,$(findstring -DNO_MC35,$(DEFS)))
LIBOBJS += mc35ad.o
endif

ifeq (,$(findstring -DNO_MC39,$(DEFS)))
LIBOBJS += mc39ad.o
endif

ifneq (-DNO_MA28 -DNO_MC35, $(findstring -DNO_MA28,$(DEFS)) $(findstring -DNO_MC35,$(DEFS)))
LIBOBJS += mc20ad.o
endif

LIB = libhsl.a

ifdef SOLIBDIR
  SOLIB := $(patsubst %.a,%.so,$(LIB))
endif

CLEANOBJS = $(wildcard *.o)
CLEANLIBS = $(LIB) $(patsubst %.a, %.so, $(LIB))

all: $(LIB) $(SOLIB)

include $(MAKEFILETRGS)
