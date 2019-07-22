# $Id: Makefile,v 1.2 2002/11/24 21:54:53 andreasw Exp $
# CHANGEME:  If you copied the context of this source directory in order to
#            develop your own project, change the definition of the following
#            variable, where 'MYPROJECT' should be the name of your source
#            directory:
THIS_DIST_DIR   := IPOPT/Fortran_example
#THIS_DIST_DIR   := IPOPT/MYPROJECT

#
#  The next lines automatically determine the root dir for the objects
#
slash := /
empty :=
space := $(empty) $(empty)
dirs  := $(subst /, ,$(THIS_DIST_DIR))
dots  := $(foreach dir,$(dirs), .. )
OBJROOTDIR := $(subst $(space),$(slash),$(strip $(dots)))

makefiledefs = $(OBJROOTDIR)/Makefile.defs
makefiletrgs = $(OBJROOTDIR)/Makefile.trgs

include $(makefiledefs)

# CHANGEME: This should be the name of your executable
exe      = example

# CHANGEME: Here is the name of all object files corresponding to the source
#           code that you wrote in order to define the problem statement
mainobjs = $(exe).o

auxilobjfiles = c_out.o \
	constr.o \
	iter_out.o \
	ma28_call.o \
	ma48_call.o

# CHANGEME: List here all the dummy routines from SRCROOTDIR/IPOPT/auxil that
#           you want to include
auxilobjfiles += eval_hesscon_v_dummy.o \
	eval_hesslag_v_dummy.o \
	eval_hessobj_v_dummy.o \

# The following line automatically adds the correct directory to the name of
# the auxiliary object files
AUXILOBJS = $(addprefix  $(OBJROOTDIR)/$(DIST_IPOPT_AUXIL)/, \
	 $(auxilobjfiles))

# This is responsible for copying the example of the PARAMS.DAT file
DATA    = PARAMS.DAT.example
DATADIR = $(SRCROOTDIR)/$(DIST_IPOPT_DOC)

# This tells 'Makefile.trgs', what should be deleted for 'make clean' etc
CLEANOBJS = $(mainobjs)
CLEANEXES = $(exe)
CLEANDATA = $(DATA) IPOPT.output

OBJS    = $(mainobjs) $(AUXILOBJS)
LIBS    = $(IPOPTLIB) $(TRONLIB) $(IPOPTLIB) $(LINALGLIBS)

# CHANGEME: If you need to link with additional libraries, you can specify
#           them here:
#LIBS   += -lmylib

all: $(exe) $(DATA)

$(exe): $(OBJS) $(DEPLIBS)
	$(F77) $(LNFLAGS) -o $@ $(OBJS) $(LNKLIBS) $(LNLIBS)

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
#  Make sure we can run stuff in parallel ('make -j')
#
break := $(firstword, $(AUXILOBJS))
break : $(filter-out $break, $(AUXILOBJS))

##
##  Dependencies
##
#incdir    = $(SRCROOTDIR)/$(DIST_IPOPT_IPOPT)
#FINCFLAGS = $(patsubst %,-I%, $(incdir))
#$(mainobjs): $(incdir)/IPOPT.INC


include $(makefiletrgs)
