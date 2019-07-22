// Copyright (C) 2001, International Business Machines
// Corporation and others.  All Rights Reserved.

#include <CoinHelperFunctions.hpp>
#include <OsiClpSolverInterface.hpp>

#include "BB_tm.hpp"
#include "BB_cut.hpp"

//#############################################################################

void
BB_tm::readInput(const char* filename)
{
   int i;
   
   tm_par.read_from_file(filename);
   lp_par.read_from_file(filename);

   // read in the problem
   OsiClpSolverInterface solver;
   solver.readMps(tm_par.entry(BB_tm_par::InputFile).c_str());
   const int rownum = solver.getNumRows();
   const int colnum = solver.getNumCols();
   desc.rownum = rownum;
   desc.colnum = colnum;

   // extract integrality information
   desc.integer = new bool[colnum];
   for (i = 0; i < colnum; ++i) {
      desc.integer[i] = solver.isInteger(i);
   }

   // extract variable bounds and objective
   desc.clb = new double[colnum];
   CoinDisjointCopyN(solver.getColLower(), colnum, desc.clb);
   desc.cub = new double[colnum];
   CoinDisjointCopyN(solver.getColUpper(), colnum, desc.cub);
   desc.obj = new double[colnum];
   CoinDisjointCopyN(solver.getObjCoefficients(), colnum, desc.obj);

   // solve the lp relaxation
   solver.initialSolve();

   // extract the matrix (by row) and the dual vector
   const CoinPackedMatrix* byRow = solver.getMatrixByRow();
   const double* pi = solver.getRowPrice();

   // decide which constraints will be core constraints
   std::vector<int> core;
   std::vector<int> indexed;
   if (tm_par.entry(BB_tm_par::AllCore)) {
      for (i = 0; i < rownum; ++i) {
	 core.push_back(i);
      }
   } else {
      for (i = 0; i < rownum; ++i) {
	 if (CoinAbs(pi[i]) < 1e-8) {
	    indexed.push_back(i);
	 } else {
	    core.push_back(i);
	 }
      }
   }
   printf("BB: core size: %i, indexed size: %i\n",
	  core.size(), indexed.size());

   // make a copy of rlb/rub in the appropriate order
   const double* solver_rlb = solver.getRowLower();
   const double* solver_rub = solver.getRowUpper();
   desc.rlb_core = new double[core.size()];
   desc.rub_core = new double[core.size()];
   for (i = core.size() - 1; i >= 0; --i) {
      desc.rlb_core[i] = solver_rlb[core[i]];
      desc.rub_core[i] = solver_rub[core[i]];
   }
   desc.rlb_indexed = new double[indexed.size()];
   desc.rub_indexed = new double[indexed.size()];
   for (i = indexed.size() - 1; i >= 0; --i) {
      desc.rlb_indexed[i] = solver_rlb[indexed[i]];
      desc.rub_indexed[i] = solver_rub[indexed[i]];
   }

   // split the byRow matrix into core and indexed constraints
   // these two variables are part of the BB_tm class
   desc.core = new CoinPackedMatrix(false, 0.0, 0.0);
   desc.core->submatrixOf(*byRow, core.size(), &core[0]);

   desc.indexed = new CoinPackedMatrix(false, 0.0, 0.0);
   if (!indexed.empty())
      desc.indexed->submatrixOf(*byRow, indexed.size(), &indexed[0]);
}

//#############################################################################

void
BB_tm::pack_module_data(BCP_buffer& buf, BCP_process_t ptype)
{
   // possible process types looked up in BCP_enum_process_t.hpp
   switch (ptype) {
   case BCP_ProcessType_LP:
      {
	 lp_par.pack(buf);
	 // pack the indexed matrix and the corresponding row bounds
	 const CoinPackedMatrix& m = *desc.indexed;
	 const int indrows = m.getNumRows();
	 buf.pack(indrows);
	 if (indrows > 0) {
	    buf.pack(desc.rlb_indexed, indrows)
	       .pack(desc.rub_indexed, indrows);
	    // The matrix we'll pack here has extraGap and extraMajor 0.0,
	    // thus there won't be any gaps between the major vectors, so we
	    // can pack the whole index/element array in one chunk without
	    // having to worry about uninitialized memory reads
	    buf.pack(m.isColOrdered())
	       .pack(m.getMinorDim())
	       .pack(m.getVectorStarts(), m.getSizeVectorStarts())
	       .pack(m.getVectorLengths(), m.getSizeVectorLengths())
	       .pack(m.getIndices(), m.getNumElements())
	       .pack(m.getElements(), m.getNumElements());
	 }
	 // if we don't do final solution testing then the indexed stuff is not
	 // needed anymore
	 if (! tm_par.entry(BB_tm_par::TestSolutions)) {
	    delete[] desc.rlb_indexed;
	    delete[] desc.rub_indexed;
	    delete desc.indexed;
	    desc.rlb_indexed = 0;
	    desc.rub_indexed = 0;
	    desc.indexed = 0;
	 }
      }
      break;
   default:
      abort();
   }
}

//#############################################################################

void
BB_tm::pack_cut_algo(const BCP_cut_algo* cut, BCP_buffer& buf)
{
   const BB_cut* bb_cut = dynamic_cast<const BB_cut*>(cut);
   if (!bb_cut)
      throw BCP_fatal_error("BB_lp::pack_cut_algo() : unknown cut type!\n");
   
   bb_cut->pack(buf);
   return;
}

BCP_cut_algo*
BB_tm::unpack_cut_algo(BCP_buffer& buf)
{
   return new BB_cut(buf);
}

//#############################################################################

void
BB_tm::initialize_core(BCP_vec<BCP_var_core*>& vars,
		       BCP_vec<BCP_cut_core*>& cuts,
		       BCP_lp_relax*& matrix)
{
   int i;

   vars.reserve(desc.colnum);
   for (i = 0; i < desc.colnum; ++i) {
      if (desc.integer[i]) {
	 if (desc.clb[i] == 0.0 && desc.cub[i] == 1.0)
	    vars.push_back(new BCP_var_core(BCP_BinaryVar, desc.obj[i], 0, 1));
	 else
	    vars.push_back(new BCP_var_core(BCP_IntegerVar, desc.obj[i],
					    desc.clb[i], desc.cub[i]));
      } else {
	 vars.push_back(new BCP_var_core(BCP_ContinuousVar, desc.obj[i],
					 desc.clb[i], desc.cub[i]));
      }
   }

   const int corerows = desc.core->getNumRows();
   cuts.reserve(corerows);
   for (i = 0; i < corerows; ++i) {
      cuts.push_back(new BCP_cut_core(desc.rlb_core[i], desc.rub_core[i]));
   }

   matrix = new BCP_lp_relax;

   // depending on whether we do final solution testing or not we create the
   // LP relaxation by assigning or by copying
   if (! tm_par.entry(BB_tm_par::TestSolutions)) {
      matrix->assign(*desc.core, desc.obj, desc.clb, desc.cub,
		    desc.rlb_core, desc.rub_core);
      delete[] desc.integer;
      desc.integer = 0;
   } else {
      matrix->copyOf(*desc.core, desc.obj, desc.clb, desc.cub,
		    desc.rlb_core, desc.rub_core);
   }
}

//#############################################################################

void
BB_tm::display_feasible_solution(const BCP_solution* soln)
{
   if (tm_par.entry(BB_tm_par::TestSolutions)) {
      int i;
      const BCP_solution_generic* gsol =
	 dynamic_cast<const BCP_solution_generic*>(soln);

      // put together the solution vector
      const int colnum = desc.colnum;
      double* sol = new double[colnum];
      CoinFillN(sol, colnum, 0.0);
      const int size = gsol->_vars.size();
      for (i = 0; i < size; ++i) {
	 sol[gsol->_vars[i]->bcpind()] = gsol->_values[i];
      }

      // check that conditions on vars are satisfied
      for (i = 0; i < colnum; ++i) {
	 assert(sol[i] >= desc.clb[i] - 1e-6 && sol[i] <= desc.cub[i] + 1e-6);
	 const double frac = sol[i] - CoinAbs(sol[i]);
	 if (desc.integer[i])
	    assert(frac < 1e-5 || frac > 1-1e-5);
      }

      // check that the constraints are not violated
      const int rownum = desc.rownum;
      double* lhs = new double[rownum];
      desc.core->times(sol, lhs);
      for (i = desc.core->getNumRows() - 1; i >= 0; --i) {
	 assert(lhs[i] >= desc.rlb_core[i] - 1e-6 &&
		lhs[i] <= desc.rub_core[i] + 1e-6);
      }
      desc.indexed->times(sol, lhs);
      for (i = desc.indexed->getNumRows() - 1; i >= 0; --i) {
	 assert(lhs[i] >= desc.rlb_indexed[i] - 1e-6 &&
		lhs[i] <= desc.rub_indexed[i] + 1e-6);
      }
   }
   BCP_tm_user::display_feasible_solution(soln);
}
