// Copyright (C) 2000, International Business Machines
// Corporation and others.  All Rights Reserved.
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <algorithm>

#include "CoinSort.hpp"
#include "CoinTime.hpp"

#include "BCP_vector.hpp"
#include "MKC_knapsack.hpp"
#include "MKC_var.hpp"
#include "MKC_vargen.hpp"
#include "MKC_optim.hpp"

//#############################################################################

double
MKC_generate_variables(const MKC_knapsack_set& kss,
		       const MKC_knapsack_fixing* ks_fixings,
		       BCP_vec<MKC_var*>* enumerated_ks,
		       const double* dual,
		       const double rc_bound,
		       const bool all_encountered_var,
		       const bool print_best_dj,
		       BCP_vec<MKC_var*>& generated_vars)
{
   const int ks_num = kss.ks_num;

   // First compute the bound for the enumerated knapsacks
   double t = CoinCpuTime();
   MKC_check_enumerated_variables(kss, enumerated_ks, dual, -rc_bound,
				  generated_vars, print_best_dj);
   const int enumerated_size = generated_vars.size();
   double bound = 0.0;
   for (int i = 0; i < enumerated_size; ++i) {
      bound += generated_vars[i]->dj(dual);
   }
   printf("MKC: check_enumerated_time: %.3f\n", CoinCpuTime() - t);

   t = CoinCpuTime();
   BCP_vec<double> lprelax(ks_num, 0.0);
   BCP_vec<int>    newvarnum(ks_num, 0);
   BCP_vec<double> bestdj(ks_num, 0.0);

   int enum_cnt = 0;
   int lp_cnt = 0;
   int ks_cnt = 0;

   // then compute the bound for the rest, first trying heuristics
   for (int i = 0; i < ks_num; ++i) {
      if (kss.ks_list[i].enumerated) {
	 enum_cnt++;
	 continue;
      }

      lprelax[i] =
	 MKC_generate_vars_one_ks(ks_num, i, kss.ks_list[i], ks_fixings[i],
				  dual, generated_vars, rc_bound, 2);

      if (lprelax[i] >= rc_bound) {
	 lp_cnt++;
	 continue;
      }

      ks_cnt++;
	 
      size_t oldsize = generated_vars.size();
      MKC_generate_vars_one_ks(ks_num, i,
			       kss.ks_list[i], ks_fixings[i],
			       dual, generated_vars, rc_bound, 0);
      newvarnum[i] = generated_vars.size() - oldsize;
      if (newvarnum[i] > 0) {
	 bestdj[i] = generated_vars.back()->dj(dual);
	 if (!all_encountered_var) {
	    // keep only the last, as that is the best
	    purge_ptr_vector(generated_vars, generated_vars.entry(oldsize),
			     generated_vars.end()-1);
	 }
      }
   }
   printf("MKC: heur_colgen_time: %.3f\n", CoinCpuTime() - t);

   // run exact only if did not find any enumerated and no heuristics worked.
   // Note that in this case 'bound' is still 0
   const bool run_exact = generated_vars.size() == 0;
   if (run_exact) {
      t = CoinCpuTime();
      printf("\
MKC: no improving enumerated/heur variable. Running exact algorithm.\n");
      for (int i = 0; i < ks_num; ++i) {
	 if (kss.ks_list[i].enumerated)
	    continue;
	 if (lprelax[i] >= rc_bound)
	    continue;

	 size_t oldsize = generated_vars.size();
	 MKC_generate_vars_one_ks(ks_num, i,
				  kss.ks_list[i], ks_fixings[i],
				  dual, generated_vars, rc_bound, 1);
	 newvarnum[i] = generated_vars.size() - oldsize;
	 if (newvarnum[i] > 0) {
	    bestdj[i] = generated_vars.back()->dj(dual);
	    if (!all_encountered_var) {
	       // keep only the last, as that is the best
	       purge_ptr_vector(generated_vars,	generated_vars.entry(oldsize),
				generated_vars.end()-1);
	    }
	 }
      }
      printf("MKC: exact_colgen_time: %.3f\n", CoinCpuTime() - t);
      for (int i = 0; i < ks_num; ++i) {
	 bound += bestdj[i];
	 if (print_best_dj && bestdj[i] != 0.0) {
	    printf("MKC: exact for ks %i: varnum %i, bestdj %f, lprelax %f\n",
		   i, newvarnum[i], bestdj[i], lprelax[i]);
	 }
      }
   } else {
      for (int i = 0; i < ks_num; ++i) {
	 bound += lprelax[i];
	 if (print_best_dj && bestdj[i] != 0.0) {
	    printf("MKC: heur for ks %i: varnum %i, bestdj %f, lprelax %f\n",
		   i, newvarnum[i], bestdj[i], lprelax[i]);
	 }
      }
      printf("\
MKC: found improving enumerated/heur variables. Using lprelax values for tlb.\n");
   }

   printf("MKC: enum_lp_ks : %i %i %i\n", enum_cnt, lp_cnt, ks_cnt);

   return bound;
}

//#############################################################################
// rc_bound is a positive number indicating how negative redcost variables we
// are looking for

//#############################################################################
// possible values for what_to_do:
// 0 - do heuristic
// 1 - do exact
// 2 - do lp relaxation
//
// Return value:
// if what_to_do is 0/1 : rc_bound  or the best red cost found
// if what_to_do is 2 : 0  or the best red cost found
//
// Note: if what_to_do is 0/1 then we are looking for vars with rc better than
// rc_bound (i.e., red cost < -rc_bound).

double
MKC_generate_vars_one_ks(const int ks_num, const int ks_ind,
			 MKC_knapsack& ks_orig,
			 const MKC_knapsack_fixing& ksf,
			 const double* dual,
			 BCP_vec<MKC_var*>& new_vars,
			 const double rc_bound,
			 const int what_to_do)
{
   MKC_knapsack_entry* entries_orig = ks_orig.entries;

   int j, k;

   // First compute the cost and ratio for every entry in the ks
   const double* order_dual = dual + ks_num;
   for (j = ks_orig.size - 1; j >= 0; --j) {
      MKC_knapsack_entry &current = entries_orig[j];
      current.cost = order_dual[current.index] - current.orig_cost;
      current.ratio = current.cost > 0 ? current.cost / current.weight : 0;
   }

   // Preprocess the KS
   double rc_adjust = 0;

   int color0_cnt = -1;
   int color1_cnt = -1;

   int clr[2] = {-1, -1};

   MKC_knapsack_entry * color0_list = 0;
   MKC_knapsack_entry * color1_list = 0;

   const int fixed_num = ksf.fixed_entry_num;
   if (fixed_num > 0) {
      for (j = 0; j < fixed_num; ++j) {
	 const int ind = ksf.fixed_entries[j];
	 rc_adjust += entries_orig[ind].cost;
	 k = entries_orig[ind].color;
	 if (clr[0] == -1) {
	    clr[0] = k;
	    continue;
	 }
	 if (clr[0] == k)
	    continue;

	 if (clr[1] == -1) {
	    clr[1] = k;
	    continue;
	 }
	 if (clr[1] == k)
	    continue;

	 abort();
      }

      if (clr[0] == -1) {
	 abort();
      } else {
	 const int first = ks_orig.color_beg[clr[0]];
	 const int last = ks_orig.color_beg[clr[0]+1];
	 color0_list = new MKC_knapsack_entry[last - first];
	 int k = 0;
	 for (j = first; j < last; ++j) {
	    if (ksf.fixing[entries_orig[j].index] == 1)
	       color0_list[k++] = entries_orig[j];
	 }
	 color0_cnt = k;
	 // order the entries in the color by their ratios (decreasing)
	 if (k > 0)
	    std::sort(color0_list, color0_list+k, MKC_ks_entry_ratio_descend);
      }

      if (clr[1] != -1) {
	 const int first = ks_orig.color_beg[clr[1]];
	 const int last = ks_orig.color_beg[clr[1]+1];
	 color1_list = new MKC_knapsack_entry[last - first];
	 int k = 0;
	 for (j = first; j < last; ++j) {
	    if (ksf.fixing[entries_orig[j].index] == 1)
	       color1_list[k++] = entries_orig[j];
	 }
	 color1_cnt = k;
	 // order the entries in the color by their ratios (decreasing)
	 if (k > 1)
	    std::sort(color1_list, color1_list+k, MKC_ks_entry_ratio_descend);
      }
   }

   // Create a new knapsack by throwing out the entries that cannot be in the
   // KS. Also throw out those that are listed in the color[01]_list.
   MKC_knapsack ks;
   ks.cost = ks_orig.cost;
   ks.capacity = ks_orig.capacity - ksf.fixed_weight;
   ks.entries = new MKC_knapsack_entry[ks_orig.size];
   MKC_knapsack_entry * entries = ks.entries;
   ks.size = 0;

   for (j = 0; j < ks_orig.size; ++j) {
      const int index = entries_orig[j].index;
      if (ksf.fixing[index] == 0 ||
	  entries_orig[j].ratio <= 0.000001 ||
	  entries_orig[j].weight > ks.capacity)
	 continue;

      if (clr[0] != -1)
	 if (entries_orig[j].color == clr[0])
	    continue;
      if (clr[1] != -1)
	 if (entries_orig[j].color == clr[1])
	    continue;
      entries[ks.size++] = entries_orig[j];
   }

   if (ks.size > 0) {
      ks.color_beg = new int[ks_orig.color_num + 1];
      ks.color_beg[0] = 0;
      for (k = 0, j = 1; j < ks.size; ++j) {
	 if (entries[j - 1].color != entries[j].color)
	    ks.color_beg[++k] = j;
      }
      ks.color_beg[++k] = ks.size;
      ks.color_num = k;
      // order the entries in each color by their ratios (decreasing)
      for (j = 0; j < ks.color_num; ++j) {
	 std::sort(entries + ks.color_beg[j], entries + ks.color_beg[j+1],
		   MKC_ks_entry_ratio_descend);
      }
   } else {
      ks.color_beg = 0;
      ks.color_num = 0;
   }
   const int color_num = ks.color_num;
   const int *color_beg = ks.color_beg;
   

   // throw out the heavy ones from the two color_list's
   if (clr[0] != -1) {
      for (k = 0, j = 0; j < color0_cnt; ++j) {
	 if (color0_list[j].weight <= ks.capacity)
	    color0_list[k++] = color0_list[j];
      }
      color0_cnt = k;
   }
   if (clr[1] != -1) {
      for (k = 0, j = 0; j < color1_cnt; ++j) {
	 if (color1_list[j].weight <= ks.capacity)
	    color1_list[k++] = color1_list[j];
      }
      color1_cnt = k;
   }

   // Oh boy... Now the nonfixed entries with colors different than those of
   // the fixed entries are listed in the new knapsack, and the entries with
   // colors same as the fixed entries are lised in color[01]_list

   MKC_knapsack_entry* sublist = new MKC_knapsack_entry[ks_orig.size + 1];
   int* tmp_chosen = new int[ks_orig.size];

   // somthing to store the colors and their best ratio in.
   BCP_vec<int> color_order;
   BCP_vec<double> color_ratio;
   color_order.reserve(ks_orig.size); // an upper bound on the num of colors
   color_ratio.reserve(ks_orig.size);

   // Now do a heuristic for the KS and see if we find anything
   double cutoff =
      ks.cost - rc_adjust - dual[ks_ind] + (what_to_do < 2 ? rc_bound : 0);

   // Depending on how many fixed colors we have for the knapsack we
   // handle it differently:
   if (clr[1] != -1) {
      // We got both colors fixed!
      const int sublist_size = color0_cnt + color1_cnt;
      // this'll work even if sublist_size is 0.
      std::merge(color0_list, color0_list + color0_cnt,
		 color1_list, color1_list + color1_cnt,
		 sublist, MKC_ks_entry_ratio_descend);
      MKC_do_the_knapsack(clr, sublist, sublist_size, ks, ksf, ks_ind,
			  ks_num, cutoff,
			  new_vars, tmp_chosen, what_to_do);
   } else {
      
      if (color_num > 1) {
	 color_order.clear();
	 color_ratio.clear();
	 // order the colors in increasing order of their best ratios
	 for (j = 0; j < color_num; ++j) {
	    color_order.unchecked_push_back(j);
	    color_ratio.unchecked_push_back(entries[color_beg[j]].ratio);
	 }
	 CoinSort_2(color_ratio.begin(), color_ratio.end(),
		    color_order.begin());
      }

      if (clr[0] != -1) {
	 // We've got one color fixed

	 if (color_num == 0) {
	    // First handle the case when that color is the ONLY color
	    MKC_do_the_knapsack(clr, color0_list, color0_cnt, ks, ksf, ks_ind,
				ks_num, cutoff,
				new_vars, tmp_chosen, what_to_do);
	 } else {
	    // There is another color
	    int j = 0;
	    if (color0_list[0].ratio * ks.capacity < cutoff) {
	       for ( ; j < color_num; ++j)
		  if (color_ratio[j] * ks.capacity >= cutoff)
		     break;
	    }
	    for (; j < color_num; ++j) {
	       const int cbj = color_beg[color_order[j]];
	       const int cbj1 = color_beg[color_order[j]+1];
	       const int sublist_size = color0_cnt + cbj1 - cbj;
	       std::merge(color0_list, color0_list + color0_cnt,
			  entries + cbj, entries + cbj1,
			  sublist, MKC_ks_entry_ratio_descend);
	       MKC_do_the_knapsack(clr, sublist, sublist_size, ks, ksf, ks_ind,
				   ks_num, cutoff,
				   new_vars, tmp_chosen, what_to_do);
	    }
	 }

      } else {
	 // We don't have any colors fixed.

	 if (color_num == 1) {
	    // Again, first check if there's only ONE color
	    MKC_do_the_knapsack(clr, entries, ks.size, ks, ksf, ks_ind,
				ks_num, cutoff,
				new_vars, tmp_chosen, what_to_do);
	 } else {
	    // We've got at least two colors
	    // Now for every pair of colors that has a chance do a greedy
	    // heuristic
	    for (j = 0; j < color_num; ++j)
	       if (color_ratio[j] * ks.capacity >= cutoff)
		  break;
	    for (; j < color_num; ++j) {
	       for (k = 0; k < j; ++k) {
		  const int cbj = color_beg[color_order[j]];
		  const int cbj1 = color_beg[color_order[j]+1];
		  const int cbk = color_beg[color_order[k]];
		  const int cbk1 = color_beg[color_order[k]+1];
		  const int sublist_size = cbj1 - cbj + cbk1 - cbk;
		  std::merge(entries + cbj, entries + cbj1,
			     entries + cbk, entries + cbk1,
			     sublist, MKC_ks_entry_ratio_descend);
		  MKC_do_the_knapsack(clr,
				      sublist, sublist_size, ks, ksf, ks_ind,
				      ks_num, cutoff,
				      new_vars, tmp_chosen, what_to_do);
	       }
	    }
	 }
      }
   }

   // clean up
   delete[] tmp_chosen;
   delete[] sublist;
   delete[] color0_list;
   delete[] color1_list;

   return ks.cost - rc_adjust - dual[ks_ind] - cutoff;
}

//#############################################################################

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
		    const int what_to_do)
{
   switch (what_to_do) {
    case 2:
      {
	 double sum = ks.capacity;
	 double bestposs = 0.0;
	 for (int l = 0; l < sublist_size; ++l) {
	    if (sublist[l].weight < sum) {
	       sum -= sublist[l].weight;
      		bestposs += sublist[l].cost;
	    } else {
	       bestposs += sublist[l].ratio * sum;
	       break;
	    }
	 }
	 if (cutoff < bestposs) {
	    cutoff = bestposs;
	 }
      }
      break;
    case 1:
      MKC_exact_knapsack(clr, sublist, sublist_size,
			 ksf, ks.capacity, ks.cost, ks_ind, ks_num,
			 cutoff, new_vars, tmp_chosen);
      break;
    case 0:
      MKC_greedy_knapsack(clr, sublist, sublist_size,
			  ksf, ks.capacity, ks.cost, ks_ind, ks_num,
			  cutoff, new_vars, tmp_chosen);
      break;
   }
}

//#############################################################################

void
MKC_check_enumerated_variables(const MKC_knapsack_set& kss,
			       BCP_vec<MKC_var*>* enumerated_ks,
			       const double* dual, const double rc_bound,
			       BCP_vec<MKC_var*>& new_vars,
			       const bool print_best_dj)
{
   if (! enumerated_ks)
      return;

   const int ks_num = kss.ks_num;
   const int old_nv_size = new_vars.size();
   double global_best_dj = 0.0;

   int i, j;
   for (i = 0; i < ks_num; ++i) {
      MKC_knapsack& ks = kss.ks_list[i];
      if (! ks.enumerated)
	 continue;

      BCP_vec<MKC_var*>& eks = enumerated_ks[i];
      const int size = eks.size();
      double best_dj = rc_bound;
      int best_ind = -1;
      for (j = 0; j < size; ++j) {
	 const double dj = eks[j]->dj(dual);
	 if (dj < best_dj) {
	    best_dj = dj;
	    best_ind = j;
	 }
      }
      if (print_best_dj) {
	 if (best_ind != -1)
	    printf("MKC: enumeration for ks %i: best dj %f\n", i, best_dj);
      }
      if (best_ind >= 0) {
	 // make a copy of eks[j]
	 new_vars.push_back(new MKC_var(*eks[best_ind]));
	 if (global_best_dj > best_dj) {
	    global_best_dj = best_dj;
	 }
      }
   }

   const int new_var_num = new_vars.size() - old_nv_size;
   if (new_var_num > 0) {
      printf("MKC: enumeration found %i variables, best dj: %f.\n",
	     new_var_num, global_best_dj);
   }
}
