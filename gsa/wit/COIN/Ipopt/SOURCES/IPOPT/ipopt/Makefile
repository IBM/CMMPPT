# Copyright (C) 2002, Carnegie Mellon University and others.
# All Rights Reserved.
# This code is published under the Common Public License.

# $Id: Makefile,v 1.3 2002/11/24 21:34:08 andreasw Exp $
THIS_DIST_DIR   := IPOPT/ipopt
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

LIBOBJS = \
	armijo.o \
	aug_lag.o \
	calc_bar.o \
	calc_nrm.o \
	check_basis.o \
	compute_y.o \
	cutalpha.o \
	dasv2f.o \
	error.o \
	filter.o \
	filter_add.o \
	filter_check.o \
	find_param.o \
	ffinite.o \
	get_bb.o \
	get_c.o \
	get_d.o \
	get_exactw.o \
	get_f.o \
	get_g.o \
	get_h.o \
	get_hv.o \
	get_ifix.o \
	get_lambda.o \
	get_pz.o \
	get_pz_cg.o \
	get_rg.o \
	get_rgb.o \
	get_rv.o \
	get_sigma.o \
	get_step_full.o \
	get_step_red.o \
	get_wcorr.o \
	get_ypy.o \
	get_zpz.o \
	get_zwzv.o \
	init_bnd.o \
	init_mem.o \
	initparams.o \
	initpoint.o \
	ipopt.o \
	ipopt_check.o \
	linesearch.o \
	ma27_call.o \
	ma47_call.o \
	ma57_call.o \
	mc19_call.o \
	mainloop.o \
	mod_chol.o \
	new_basis.o \
	partition.o \
	read_entry.o \
	reorder_ib.o \
	reorder_x.o \
	res_hv.o \
	res_ma27.o \
	resto_kkt.o \
	resto_tron.o \
	slen.o \
	solve_barrier.o \
	timer.o \
	update_b.o \
	update_b_lm.o \
	update_mu.o \
	update_nu.o

LIB = libipopt.a

ifdef SOLIBDIR
  SOLIB := $(patsubst %.a,%.so,$(LIB))
endif

CLEANOBJS = $(LIBOBJS)
CLEANLIBS = $(LIB) $(patsubst %.a, %.so, $(LIB))


all: $(LIB) $(SOLIB)

#
#  Dependencies
#

filter.o \
get_bb.o \
get_exactw.o \
get_hv.o \
get_pz.o \
get_pz_cg.o \
get_step_full.o \
get_ypy.o \
get_zwzv.o \
init_mem.o \
initparams.o \
ipopt.o \
res_main.o \
resto_tron.o \
solve_barrier.o : TIMER.INC

$(LIBOBJS): IPOPT.INC

include $(MAKEFILETRGS)
