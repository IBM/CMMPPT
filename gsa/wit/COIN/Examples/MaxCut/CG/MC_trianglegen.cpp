// Copyright (C) 2000, International Business Machines
// Corporation and others.  All Rights Reserved.
#include "BCP_vector.hpp"
#include "BCP_matrix.hpp"
#include "MC_cut.hpp"
#include "MC.hpp"
#include "MC_lp_param.hpp"

//#############################################################################

static inline void
MC_get_triplet(const double* x,
	       const int i, const int j, const int k,
	       int * mcol, double * xcol)
{
   if (i < j) {
      if (j < k) {    // i < j < k
	 xcol[0] = x[mcol[0] = MC_edge_index(i, j)];
	 xcol[1] = x[mcol[1] = MC_edge_index(i, k)];
	 xcol[2] = x[mcol[2] = MC_edge_index(j, k)];
      } else {
	 if (i < k) { // i < k < j
	    xcol[0] = x[mcol[0] = MC_edge_index(i, k)];
	    xcol[1] = x[mcol[1] = MC_edge_index(i, j)];
	    xcol[2] = x[mcol[2] = MC_edge_index(k, j)];
	 } else {     // k < i < j
	    xcol[0] = x[mcol[0] = MC_edge_index(k, i)];
	    xcol[1] = x[mcol[1] = MC_edge_index(k, j)];
	    xcol[2] = x[mcol[2] = MC_edge_index(i, j)];
	 }
      }
   } else {
      if (i < k) { // j < i < k
	 xcol[0] = x[mcol[0] = MC_edge_index(j, i)];
	 xcol[1] = x[mcol[1] = MC_edge_index(j, k)];
	 xcol[2] = x[mcol[2] = MC_edge_index(i, k)];
      } else {
	 if (j < k) { // j < k < i
	    xcol[0] = x[mcol[0] = MC_edge_index(j, k)];
	    xcol[1] = x[mcol[1] = MC_edge_index(j, i)];
	    xcol[2] = x[mcol[2] = MC_edge_index(k, i)];
	 } else {     // k < j < i
	    xcol[0] = x[mcol[0] = MC_edge_index(k, j)];
	    xcol[1] = x[mcol[1] = MC_edge_index(k, i)];
	    xcol[2] = x[mcol[2] = MC_edge_index(j, i)];
	 }
      }
   }
}
   
//#############################################################################

static inline int
MC_test_triplet(const int * mcol, const double * xcol, const double eps,
		BCP_vec<BCP_cut*>& new_cuts, BCP_vec<BCP_row*>& new_rows)
{
   int num = 0;
   if ( xcol[0] + xcol[1] + xcol[2] > 2+eps){
      const double coefs[3] = {1, 1, 1};
      new_cuts.push_back(new MC_cycle_cut(mcol, 3, 3));
      new_rows.push_back(new BCP_row(mcol,mcol+3, coefs,coefs+3, -1e40, 2.0));
      ++num;
   }
   if ( xcol[0] - xcol[1] - xcol[2] > eps){
      const double coefs[3] = {1, -1, -1};
      new_cuts.push_back(new MC_cycle_cut(mcol, 1, 3));
      new_rows.push_back(new BCP_row(mcol,mcol+3, coefs,coefs+3, -1e40, 0.0));
      ++num;
   }
   if (-xcol[0] + xcol[1] - xcol[2] > eps){
      const double coefs[3] = {1, -1, -1};
      const int mcol1[3] = {mcol[1], mcol[0], mcol[2]};
      new_cuts.push_back(new MC_cycle_cut(mcol1, 1, 3));
      new_rows.push_back(new BCP_row(mcol1, mcol1+3,
				     coefs, coefs+3, -1e40, 0.0));
      ++num;
   }
   if (-xcol[0] - xcol[1] + xcol[2] > eps){
      const double coefs[3] = {1, -1, -1};
      const int mcol2[3] = {mcol[2], mcol[0], mcol[1]};
      new_cuts.push_back(new MC_cycle_cut(mcol2, 1, 3));
      new_rows.push_back(new BCP_row(mcol2, mcol2+3,
				     coefs, coefs+3, -1e40, 0.0));
      ++num;
   }

   return num;
}

//#############################################################################

static inline int
MC_mod_nodenum(const int i, const int n, const int shift)
{
   const int i1 = i + shift;
   return i1 >= n ? i1 - n : i1;
}

//#############################################################################

void
MC_generate_triangles(const MC_problem& mc, const double* x,
		      const int strategy,
		      const int maxcutnum, const double minviol,
		      BCP_vec<BCP_cut*>& new_cuts, BCP_vec<BCP_row*>& new_rows)
{
   /* triangles are i,j,k with i<j<k. the elements are
     + + + <= 2, + - - <= 0, - + - <= 0, - - + <= 0.
     everything is ordered lexicographically */

   static int nextnode = 0;

   const int n = mc.num_nodes;
   int i1, j1, k1;
   int new_cuts_size = 0;

   int mcol[3];
   double xcol[3];

   switch (strategy) {
    case MC_TriangleGen_AroundBranchingNodeFirst:
      {
	 // first generate cuts adjacent to the branching_node
	 const int branching_node = mc.branching_node;
	 for (i1 = 0; i1 < n; ++i1) {
	    if (branching_node == i1)
	       continue;
	    for (j1 = i1+1; j1 < n; ++j1) {
	       if (branching_node == j1)
		  continue;
	       MC_get_triplet(x, branching_node, i1, j1, mcol, xcol);
	       MC_test_triplet(mcol, xcol, minviol, new_cuts, new_rows);
	    }
	 }

	 // now check the rest of the triangle inequalities
	 new_cuts_size = new_cuts.size();

	 const int idesp = static_cast<int>(floor(n * drand48()));

	 for (i1 = 0; i1 < n; ++i1){
	    const int i = MC_mod_nodenum(i1, n, idesp);
	    if (i == branching_node)
	       continue;
	    for (j1 = i1+1; j1 < n; ++j1){
	       const int j = MC_mod_nodenum(j1, n, idesp);
	       if (j == branching_node)
		  continue;
	       for (k1 = j1+1; k1 < n; ++k1){
		  const int k = MC_mod_nodenum(k1, n, idesp);
		  if (k == branching_node)
		     continue;
		  MC_get_triplet(x, i, j, k, mcol, xcol);
		  new_cuts_size +=
		     MC_test_triplet(mcol, xcol, minviol, new_cuts, new_rows);
		  if (new_cuts_size >= maxcutnum)
		     break;
	       }
	    }
	 }
      }
      break;

    case MC_TriangleGen_RoundRobin:
      for (i1 = 0; i1 < n; ++i1) {
	 const int i = MC_mod_nodenum(i1, n, nextnode);
	 for (j1 = i1+1; j1 < n; ++j1){
	    const int j = MC_mod_nodenum(j1, n, nextnode);
	    for (k1 = j1+1; k1 < n; ++k1){
	       const int k = MC_mod_nodenum(k1, n, nextnode);
	       MC_get_triplet(x, i, j, k, mcol, xcol);
	       new_cuts_size +=
		  MC_test_triplet(mcol, xcol, minviol, new_cuts, new_rows);
	       if (new_cuts_size >= maxcutnum)
		  break;
	    }
	 }
      }
      ++nextnode;
      if (nextnode == n)
	 nextnode = 0;
      break;
   }
}
