// Copyright (C) 2000, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef _MKC_VARGEN_H
#define _MKC_VARGEN_H

#include <cstdio>

#include "BCP_vector.hpp"

class MKC_knapsack_set;
class MKC_knapsack_fixing;
class MKC_var;
class BCP_var;

double
MKC_generate_variables(const MKC_knapsack_set& kss,
		       const MKC_knapsack_fixing* ks_fixings,
		       BCP_vec<MKC_var*>* enumerated_ks,
		       const double* dual,
		       const double rc_bound,
		       const bool all_encountered_var,
		       const bool print_best_dj,
		       BCP_vec<MKC_var*>& generated_vars);

double
MKC_generate_vars_one_ks(const int ks_num, const int ks_ind,
			 MKC_knapsack& ks_orig,
			 const MKC_knapsack_fixing& ksf,
			 const double* dual,
			 BCP_vec<MKC_var*>& new_vars,
			 const double rc_bound,
			 const int what_to_do);

void
MKC_do_the_knapsack(const int clr[2],
		    const MKC_knapsack_entry * sublist,
		    const int sublist_size,
		    const MKC_knapsack& ks,
		    const MKC_knapsack_fixing& ksf,
		    const int ks_ind,
		    const int ks_num,
		    double& cutoff,
		    BCP_vec<MKC_var*>& new_vars,
		    int* tmp_chosen,
		    const int what_to_do);

void
MKC_check_enumerated_variables(const MKC_knapsack_set& kss,
			       BCP_vec<MKC_var*>* enumerated_ks,
			       const double* dual, const double rc_bound,
			       BCP_vec<MKC_var*>& new_vars,
			       const bool print_best_dj);

#endif
