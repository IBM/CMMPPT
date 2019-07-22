// Copyright (C) 2000, International Business Machines
// Corporation and others.  All Rights Reserved.
#include "BCP_os.hpp"

#ifdef NEED_STD_TEMPLATE_FUNCTIONS
#include <algorithm>
template int *
std::unique(int *, int *);

template void
std::fill(char *, char *, bool const &);

template bool
std::lexicographical_compare(int *, int *, int *, int *);

template bool
std::equal(int *, int *, int *);

#ifdef NEED_IMPLICIT_TEMPLATE_FUNCTIONS
template double *
std::__rotate(double *, double *, double *, BCP_PtrDiff *,
	      bidirectional_iterator_tag);
#endif

template int *
std::find(int *, int *, int const &, random_access_iterator_tag);

#include "MKC_knapsack.hpp"

template MKC_knapsack_entry*
merge(MKC_knapsack_entry*, MKC_knapsack_entry*,
      MKC_knapsack_entry*, MKC_knapsack_entry*,
      MKC_knapsack_entry*,
      bool(*)(const MKC_knapsack_entry&, const MKC_knapsack_entry&));

#endif

//#############################################################################

#include "templates/BCP_vector_general.cpp"
#include "MKC_var.hpp"

#ifdef NEED_TEMPLATE_CLASSES
template BCP_vec<MKC_var*>;
#endif

#ifdef NEED_TEMPLATE_FUNCTIONS
template void
purge_ptr_vector(BCP_vec<MKC_var *> &);
#endif

//#############################################################################

#ifdef NEED_IMPLICIT_TEMPLATE_FUNCTIONS
#include "MKC_knapsack.hpp"
#include "MKC_var.hpp"

template void
__final_insertion_sort(MKC_knapsack_entry*, MKC_knapsack_entry*,
		       bool(*)(const MKC_knapsack_entry&,
			       const MKC_knapsack_entry&));
template void
__introsort_loop(MKC_knapsack_entry*, MKC_knapsack_entry*,
		 MKC_knapsack_entry*, BCP_PtrDiff,
		 bool(*)(const MKC_knapsack_entry&,
			 const MKC_knapsack_entry&));

template void
__final_insertion_sort(MKC_var**, MKC_var**,
		       bool(*)(const MKC_var*, const MKC_var*));
template void
__introsort_loop(MKC_var**, MKC_var**, MKC_var**, BCP_PtrDiff,
		 bool(*)(const MKC_var*, const MKC_var*));
#endif
