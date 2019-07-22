// Copyright (C) 2001, International Business Machines
// Corporation and others.  All Rights Reserved.

#include <vector>

#include "BB_lp.hpp"
#include "BB_cut.hpp"
#include "OsiClpSolverInterface.hpp"

#include "CglOddHole.hpp"
#include "CglKnapsackCover.hpp"
#include "CglSimpleRounding.hpp"
#include "CglLiftAndProject.hpp"

#include "BCP_lp.hpp"

//#############################################################################

void
BB_lp::unpack_module_data(BCP_buffer& buf)
{
   par.unpack(buf);
   buf.unpack(indrows);
   if (indrows > 0) {
      buf.unpack(rlb, indrows, true)
	 .unpack(rub, indrows, true); // true: allocate space
      bool colordered;
      int minorDim;
      int majorDim;
      int size;
      int* start;
      int* length;
      int* indices;
      double* elements;
      buf.unpack(colordered)
	 .unpack(minorDim)
	 .unpack(start, majorDim)
	 .unpack(length, majorDim)
	 .unpack(indices, size)
	 .unpack(elements, size);
      indexed.assignMatrix(colordered, minorDim, majorDim, size,
			   elements, indices, start, length);
      lhs = new double[indrows];
   }
}

//#############################################################################

void
BB_lp::pack_cut_algo(const BCP_cut_algo* cut, BCP_buffer& buf)
{
  const BB_cut* bb_cut = dynamic_cast<const BB_cut*>(cut);
  if (!bb_cut)
    throw BCP_fatal_error("BB_lp::pack_cut_algo() : unknown cut type!\n");
    
  bb_cut->pack(buf);
  return;
}
    
//#############################################################################

BCP_cut_algo*
BB_lp::unpack_cut_algo(BCP_buffer& buf)
{
  return new BB_cut(buf);
}

//#############################################################################
OsiSolverInterface *
BB_lp::initialize_solver_interface()
{
   return new OsiClpSolverInterface;
}

//#############################################################################
void
BB_lp::initialize_new_search_tree_node(const BCP_vec<BCP_var*>& vars,
				       const BCP_vec<BCP_cut*>& cuts,
				       const BCP_vec<BCP_obj_status>& vstatus,
				       const BCP_vec<BCP_obj_status>& cstatus,
				       BCP_vec<int>& var_changed_pos,
				       BCP_vec<double>& var_new_bd,
				       BCP_vec<int>& cut_changed_pos,
				       BCP_vec<double>& cut_new_bd)
{
}

//#############################################################################
void
BB_lp::modify_lp_parameters(OsiSolverInterface* lp, bool in_strong_branching)
{
   if (in_strong_branching) {
      lp->setIntParam(OsiMaxNumIterationHotStart, 50);
   }
}

//#############################################################################
BCP_solution*
BB_lp::test_feasibility(const BCP_lp_result& lp_result,
			const BCP_vec<BCP_var*>& vars,
			const BCP_vec<BCP_cut*>& cuts)
{
   violated_cuts.clear();
   indexed.times(lp_result.x(), lhs);
   for (int i = 0; i < indrows; ++i) {
      if (lhs[i] < rlb[i] - 1e-6 || lhs[i] > rub[i] + 1e-6)
	 violated_cuts.push_back(i);
   }

   return violated_cuts.empty() ?
      BCP_lp_user::test_feasibility(lp_result, vars, cuts) : 0;
}

//#############################################################################
void
BB_lp::cuts_to_rows(const BCP_vec<BCP_var*>& vars, // on what to expand
		    BCP_vec<BCP_cut*>& cuts,       // what to expand
		    BCP_vec<BCP_row*>& rows,       // the expanded rows
		    // things that the user can use for lifting cuts if allowed
		    const BCP_lp_result& lpres,
		    BCP_object_origin origin, bool allow_multiple)
{
   const int cutnum = cuts.size();
   for (int i = 0; i < cutnum; ++i) {
      const BCP_cut_indexed* icut =
	 dynamic_cast<const BCP_cut_indexed*>(cuts[i]);
      if (icut) {
	 const int ind = icut->index();
	 rows.push_back(new BCP_row(indexed.getVector(ind),
				    rlb[ind], rub[ind]));
	 continue;
      }
      const OsiRowCut* bcut = dynamic_cast<const BB_cut*>(cuts[i]);
      if (bcut) {
	 rows.push_back(new BCP_row(bcut->row(), bcut->lb(), bcut->ub()));
	 continue;
      }
      throw BCP_fatal_error("Unknown cut type in cuts_to_rows.\n");
   }
}

//#############################################################################

void
BB_lp::generate_cuts_in_lp(const BCP_lp_result& lpres,
			   const BCP_vec<BCP_var*>& vars,
			   const BCP_vec<BCP_cut*>& cuts,
			   BCP_vec<BCP_cut*>& new_cuts,
			   BCP_vec<BCP_row*>& new_rows)
{
   int i;
   
   new_cuts.reserve(violated_cuts.size());
   for (i = violated_cuts.size() - 1; i >= 0; --i) {
      const int ind = violated_cuts[i];
      new_cuts.push_back(new BCP_cut_indexed(ind, rlb[ind], rub[ind]));
   }
   printf("BB: found %i indexed cuts.\n", new_cuts.size());

   std::vector<CglCutGenerator*> cgs;
   if (par.entry(BB_lp_par::KnapsackCover))
      cgs.push_back(new CglKnapsackCover);
   if (par.entry(BB_lp_par::LiftAndProject))
      cgs.push_back(new CglLiftAndProject);
   if (par.entry(BB_lp_par::SimpleRounding))
      cgs.push_back(new CglSimpleRounding);
   if (par.entry(BB_lp_par::OddHole))
      cgs.push_back(new CglOddHole);

   OsiSolverInterface* si = getLpProblemPointer()->lp_solver;
   for (i = vars.size() - 1; i >= 0; --i) {
      if (vars[i]->var_type() != BCP_ContinuousVar) { // integer or binary
	 si->setInteger(i);
      }
   }
   OsiCuts cutlist;
   for (i = cgs.size() - 1; i >= 0; --i) {
      cgs[i]->generateCuts(*si, cutlist);
      delete cgs[i];
   }
   printf("BB: found %i algo cuts.\n", cutlist.sizeRowCuts());

   for (i = cutlist.sizeRowCuts() - 1; i >= 0; --i) {
      new_cuts.push_back(new BB_cut(cutlist.rowCut(i)));
   }
}
