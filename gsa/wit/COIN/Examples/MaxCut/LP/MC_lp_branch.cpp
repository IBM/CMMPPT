// Copyright (C) 2000, International Business Machines
// Corporation and others.  All Rights Reserved.

#include <cstdio>
#include <algorithm>

#include <OsiVolSolverInterface.hpp>
#include <OsiClpSolverInterface.hpp>
#include <ClpDualRowSteepest.hpp>
#include <ClpPrimalColumnSteepest.hpp>
#include <ClpSimplex.hpp>
#ifdef MC_COMPILE_WITH_OSL
#include <OsiOslSolverInterface.hpp>
#endif

#include "MC_lp.hpp"

#include "BCP_lp.hpp"
#include "BCP_timeout.hpp"
#include "BCP_lp_functions.hpp"

//#############################################################################

bool
MC_lp::is_gap_tailoff_rel(const int k, const double minimp,
			  const double objval) const
{
  if (k <= 0)
    return false;

  const int iteration_count = current_iteration();
  if (iteration_count <= k)
    return false;

  const int hist_size = CoinMin(iteration_count, hist_len);
  const double* obj_k_hist = objhist + (hist_size - k);

  // There is tailing off if in the last k steps we haven't closed a certain
  // fraction of the gap
  const double ub = upper_bound();
  if ((ub - objval) > (ub - obj_k_hist[0]) * (1-minimp))
    return true;

  // Otherwise there's no tailing off
  return false;
}

//#############################################################################

bool
MC_lp::is_lb_tailoff_rel(const int k, const double minimp,
			 const double objval) const
{
  if (k <= 0)
    return false;

  const int iteration_count = current_iteration();
  if (iteration_count <= k)
    return false;

  const int hist_size = CoinMin(iteration_count, hist_len);
  const double* obj_k_hist = objhist + (hist_size - k);

  // There is tailing off if in the last k steps we haven't increased the lb
  if (objval - obj_k_hist[0] < minimp * CoinAbs(obj_k_hist[0]))
    return true;

  // Otherwise there's no tailing off
  return false;
}

//#############################################################################

bool
MC_lp::is_lb_tailoff_abs(const int k, const double minimp,
			 const double objval) const
{
  if (k <= 0)
    return false;

  const int iteration_count = current_iteration();
  if (iteration_count <= k)
    return false;

  const int hist_size = CoinMin(iteration_count, hist_len);
  const double* obj_k_hist = objhist + (hist_size - k);

  // There is tailing off if in the last k steps we haven't increased the lb
  if (objval - obj_k_hist[0] < minimp)
    return true;

  // Otherwise there's no tailing off
  return false;
}

//#############################################################################

void
MC_lp::tailoff_test(bool& tailoff_gap_rel, bool& tailoff_lb_abs,
		    bool& tailoff_lb_rel, const double objval) const
{
   tailoff_gap_rel =
      is_gap_tailoff_rel(par.entry(MC_lp_par::TailoffGapRelMinItcount),
			 par.entry(MC_lp_par::TailoffGapRelMinImprovement),
			 objval);
   tailoff_lb_abs =
      is_lb_tailoff_abs(par.entry(MC_lp_par::TailoffLbAbsMinItcount),
			par.entry(MC_lp_par::TailoffLbAbsMinImprovement),
			objval);
   tailoff_lb_rel =
      is_lb_tailoff_rel(par.entry(MC_lp_par::TailoffLbRelMinItcount),
			par.entry(MC_lp_par::TailoffLbRelMinImprovement),
			objval);
   const int iteration_count = current_iteration();
   const int hist_size = CoinMin(iteration_count, hist_len);
   printf("MC: Tailoff check: objval: %.3f, UB: %.0f\n",
	  objval, upper_bound());
   for (int i = 0; i < hist_size; ++i) {
      printf("                   LB[%3i]: %.3f\n", i-hist_size, objhist[i]);
   }
   printf("    gap_rel: %i   lb_abs: %i   lb_rel: %i\n",
	  tailoff_gap_rel ? 1 : 0,
	  tailoff_lb_abs ? 1 : 0,
	  tailoff_lb_rel ? 1 : 0);
}

//#############################################################################

OsiSolverInterface* 
MC_lp::solveToOpt(OsiVolSolverInterface* vollp,
		  const BCP_lp_result& lpres,
		  const BCP_vec<BCP_var*>& vars,
		  const BCP_vec<BCP_cut*>& cuts,
		  double& exact_obj)
{
  const int numrows = vollp->getNumRows();
  const int numcols = vollp->getNumCols();
  const int numnz = vollp->getNumElements();

  const double* volrc = vollp->getReducedCost();
  const double* volpi = vollp->getRowPrice();
  const double* volrub = vollp->getRowUpper();
  const double* volrlb = vollp->getRowLower();
  double t;

  OsiSolverInterface* solver = 0;
  OsiClpSolverInterface* clp = 0;
#ifdef MC_COMPILE_WITH_OSL
  OsiOslSolverInterface* osl = 0;
  EKKModel* model = 0;
#endif
  if ((par.entry(MC_lp_par::LpSolver) & MC_UseClp) != 0) {
     clp = new OsiClpSolverInterface;
     solver = clp;
#ifdef MC_COMPILE_WITH_OSL
  } else if ((par.entry(MC_lp_par::LpSolver) & MC_UseOsl) != 0) {
     osl = new OsiOslSolverInterface;
     model = osl->getModelPtr();
     ekk_messagePrintOff(model, 317);
     solver = osl;
#endif
  } else {
     throw BCP_fatal_error("\
MC_lp::solveToOpt: got here but no simplex based solver is enabled in the\n\
                   MC_LpSolver parameter.\n");
  }
  const double inf = solver->getInfinity();

  // put together a matrix with the slacks explicitly added to the problem
  int i, j;
  double *clb = new double[numcols + numrows];
  double *cub = new double[numcols + numrows];
  double *obj = new double[numcols + numrows];
  double *rlb = new double[numrows];
  double *rub = new double[numrows];

  const CoinPackedMatrix& volm = *vollp->getMatrixByCol();

  CoinDisjointCopyN(vollp->getColLower(), numcols, clb);
  CoinFillN(clb + numcols, numrows, 0.0);

  CoinDisjointCopyN(vollp->getColUpper(), numcols, cub);
  CoinFillN(cub + numcols, numrows, inf);
       
  CoinDisjointCopyN(volrc, numcols, obj);

  CoinPackedMatrix* m = new CoinPackedMatrix;
  m->copyOf(true, numrows, numcols, numnz,
	    volm.getElements(), volm.getIndices(),
	    volm.getVectorStarts(), volm.getVectorLengths());
  m->setExtraGap(0.0);
  m->setExtraMajor(0.0);
  m->reserve(numcols + numrows, numnz + numrows);

  const char* sense = vollp->getRowSense();
  const double plusone = 1.0;
  const double minusone = - 1.0;

  obj_shift = 0.0;
  for (i = 0, j = numcols; i < numrows; ++i) {
    switch (sense[i]) {
    case 'L':
      m->appendCol(1, &i, &plusone);
      obj[j++] = - volpi[i];
      rub[i] = volrub[i];
      rlb[i] = volrub[i];
      obj_shift += volrub[i] * volpi[i];
      break;
    case 'G':
      m->appendCol(1, &i, &minusone);
      obj[j++] = volpi[i];
      rub[i] = volrlb[i];
      rlb[i] = volrlb[i];
      obj_shift += volrlb[i] * volpi[i];
      break;
    case 'E':
      //	    m->appendCol(1, &i, &plusone);
      //	    cub[j] = 0.0;
      //	    obj[j++] = - volpi[i];
      rub[i] = volrlb[i];
      rlb[i] = volrlb[i];
      obj_shift += volrub[i] * volpi[i];
      break;
    default:
      abort();
    }
  }

  // save the rhs
  double* bd = new double[numrows];
  CoinDisjointCopyN(rlb, numrows, bd);

  // load in the formulation
  solver->assignProblem(m, clb, cub, obj, rlb, rub);
  delete m;
  delete[] clb;
  delete[] cub;
  delete[] obj;
  delete[] rlb;
  delete[] rub;

  const double* volsol = vollp->getColSolution();
  double* colsol = new double[numcols + numrows];
  std::copy(volsol, volsol + numcols, colsol);
  std::fill(colsol + numcols, colsol + (numcols + numrows), 0.0);
  // the rest of clb is filled with 0's, so it's OK
  for (int ii = 0; ii < numcols + numrows; ++ii) {
     if (isnan(colsol[ii])) {
	printf("colsol[%i] is isNaN\n", ii);
     }
  }
  solver->setColSolution(colsol);
  delete[] colsol;

  const double* voldual = vollp->getRowPrice();
  for (int ii = 0; ii < numrows; ++ii) {
     if (isnan(voldual[ii])) {
	printf("voldual[%i] is isNaN\n", ii);
     }
  }
  solver->setRowPrice(voldual);

  const double lax_detol = 1e-3;
  const double strict_detol = 1e-7;
  double petol = 0;
  solver->getDblParam(OsiPrimalTolerance, petol);
  // unconstrain rows with 0 dual value (0 wrt. the lax dual etol)
  for (i = 0, j = numcols; i < numrows; ++i) {
     switch (sense[i]) {
      case 'L':
      case 'G':
	if (std::abs(volpi[i]) < lax_detol) {
	   solver->setRowBounds(i, -inf, inf);
	   solver->setColUpper(j, 0.0);
	}
	++j;
	break;
      case 'E':
	if (std::abs(volpi[i]) < lax_detol) {
	   solver->setRowBounds(i, -inf, inf);
	}
	break;
      default:
	abort();
     }
  }

  t = BCP_time_since_epoch();
#ifdef MC_COMPILE_WITH_OSL
  if (model) {
     //  ekk_setRdweight(model, 1e-10);
     //  int iparam[8]={-1,-1,0,0,0,0,0,1};
     //  ekk_exoticParameters(model,8,iparam,0,NULL,0,NULL);
     ekk_setImaxfactor(model,1000);
     ekk_setRtoldinf(model, lax_detol);
     ekk_dualSimplex(model);
  }
#endif
  if (clp) {
     ClpSimplex * clpModelPtr = clp->getModelPtr();
     ClpSimplex clp_simplex;
     clp_simplex.borrowModel(*clpModelPtr);
     // Set message handler to have same levels etc
     clp_simplex.passInMessageHandler(clp->messageHandler());
     // set reasonable defaults
     clp_simplex.scaling(0); // this is 1 in OsiClpSolverInterface
     clp_simplex.setDualBound(1.0e6);
     clp_simplex.setDualTolerance(lax_detol);
     ClpDualRowSteepest steep;
     clp_simplex.setDualRowPivotAlgorithm(steep);
     clp_simplex.setPrimalTolerance(1.0e-8);
     ClpPrimalColumnSteepest steepP;
     clp_simplex.setPrimalColumnPivotAlgorithm(steepP);
#if 1
     clp_simplex.dual();
#else
     clp_simplex.primal();
#endif
     clp_simplex.returnModel(*clpModelPtr);
     CoinWarmStart* ws = clp->getWarmStart();
     solver->setWarmStart(ws);
     delete ws;
  }
  t = BCP_time_since_epoch() - t;
  printf("LP: exact optimization took %.3f seconds\n", t);

  while (true) {
     // repeat the loop as long as there is any violated constraint
     const double* lhs = solver->getRowActivity();
     bool violated = false;
     for (i = 0, j = numcols; i < numrows; ++i) {
	switch (sense[i]) {
	 case 'L':
	   if (lhs[i] > volrub[i] + petol) {
	      solver->setRowBounds(i, bd[i], bd[i]);
	      solver->setColUpper(j, inf);
	      violated = true;
	   }
	   ++j;
	   break;
	 case 'G':
	   if (lhs[i] < volrlb[i] - petol) {
	      solver->setRowBounds(i, bd[i], bd[i]);
	      solver->setColUpper(j, inf);
	      violated = true;
	   }
	   ++j;
	   break;
	 case 'E':
	   if (lhs[i] < volrlb[i] - petol || lhs[i] > volrub[i] + petol) {
	      solver->setRowBounds(i, bd[i], bd[i]);
	      violated = true;
	   }
	   break;
	 default:
	   abort();
	}
     }
     if (! violated)
	break;
     t = BCP_time_since_epoch();
     solver->resolve();
     t = BCP_time_since_epoch() - t;
     printf("LP: exact optimization took %.3f seconds\n", t);
  }

     t = BCP_time_since_epoch();
  // reset the dual tolerance and repeat the loop
#ifdef MC_COMPILE_WITH_OSL
  if (model) {
     //  ekk_setRdweight(model, 1e-10);
     //  int iparam[8]={-1,-1,0,0,0,0,0,1};
     //  ekk_exoticParameters(model,8,iparam,0,NULL,0,NULL);
     ekk_setRtoldinf(model, strict_detol);
#if 0
     ekk_dualSimplex(model);
#else
     ekk_primalSimplex(model, 3);
#endif
  }
#endif
  if (clp) {
     ClpSimplex * clpModelPtr = clp->getModelPtr();
     ClpSimplex clp_simplex;
     clp_simplex.borrowModel(*clpModelPtr);
     // Set message handler to have same levels etc
     clp_simplex.passInMessageHandler(clp->messageHandler());
     // set reasonable defaults
     clp_simplex.scaling(0); // this is 1 in OsiClpSolverInterface
     clp_simplex.setDualBound(1.0e6);
     clp_simplex.setDualTolerance(strict_detol);
     ClpDualRowSteepest steep;
     clp_simplex.setDualRowPivotAlgorithm(steep);
     clp_simplex.setPrimalTolerance(1.0e-8);
     ClpPrimalColumnSteepest steepP;
     clp_simplex.setPrimalColumnPivotAlgorithm(steepP);
     clp_simplex.returnModel(*clpModelPtr);
     CoinWarmStart* ws = solver->getWarmStart();
     clp->setWarmStart(ws);
     delete ws;
     clp_simplex.borrowModel(*clpModelPtr);
#if 0
     clp_simplex.dual();
#else
     clp_simplex.primal();
#endif
     clp_simplex.returnModel(*clpModelPtr);
     ws = clp->getWarmStart();
     solver->setWarmStart(ws);
     delete ws;
  }
  t = BCP_time_since_epoch() - t;
  printf("LP: exact optimization took %.3f seconds\n", t);

  while (true) {
     // repeat the loop as long as there is any violated constraint
     const double* lhs = solver->getRowActivity();
     bool violated = false;
     for (i = 0, j = numcols; i < numrows; ++i) {
	switch (sense[i]) {
	 case 'L':
	   if (lhs[i] > volrub[i] + petol) {
	      solver->setRowBounds(i, bd[i], bd[i]);
	      solver->setColUpper(j, inf);
	      violated = true;
	   }
	   ++j;
	   break;
	 case 'G':
	   if (lhs[i] < volrlb[i] - petol) {
	      solver->setRowBounds(i, bd[i], bd[i]);
	      solver->setColUpper(j, inf);
	      violated = true;
	   }
	   ++j;
	   break;
	 case 'E':
	   if (lhs[i] < volrlb[i] - petol || lhs[i] > volrub[i] + petol) {
	      solver->setRowBounds(i, bd[i], bd[i]);
	      violated = true;
	   }
	   break;
	 default:
	   abort();
	}
     }
     if (! violated)
	break;
     t = BCP_time_since_epoch();
     solver->resolve();
     t = BCP_time_since_epoch() - t;
     printf("LP: exact optimization took %.3f seconds\n", t);
  }
  delete[] bd;

  exact_obj = solver->getObjValue() + obj_shift;
  // FB
  double* newvolpi = new double[numrows];
  const double* duv = solver->getRowPrice();
  for (i = 0; i < numrows; ++i) {
    newvolpi[i] = volpi[i] + duv[i];
  }
  vollp->setRowPrice(newvolpi);
  // end FB
  if (solver->isProvenPrimalInfeasible()) {
    printf("MC: Solving LP to optimality: infeas.\n");
    printf("MC: Forcing BCP to prune the node.\n");
    delete solver;
    delete[] newvolpi; 
    return NULL;
  }

  soln = mc_generate_heuristic_solution(solver->getColSolution(), vars, cuts);
  if (soln && soln->objective_value() < upper_bound()) {
    send_feasible_solution(soln);
  }

  printf("MC: Solving LP to optimality before branching: %.3f",
	 exact_obj);
  const double gap =
    upper_bound() - exact_obj - get_param(BCP_lp_par::Granularity);
  printf("  Gap: %.3f\n",gap);
  if (gap < 0) {
    printf("MC: exact LP solving proved fathomability.\n");
    delete solver;
    delete[] newvolpi; 
    return NULL;
  }

  // Make sure that no cut with nonzero dual value will be deleted
  const int ineff_to_delete = get_param(BCP_lp_par::IneffectiveBeforeDelete);
  const double detol = lpres.dualTolerance();
  for (int i = 0; i < numrows; ++i) {
    BCP_cut *cut = cuts[i];
    if (cut->is_to_be_removed() || cut->is_non_removable())
      continue;
    if (cut->effective_count() <= -ineff_to_delete) {
      if (CoinAbs(newvolpi[i]) >= detol) {
	cut->set_effective_count(-ineff_to_delete + 1);
      }
    }
  }

  delete[] newvolpi; 

  // In any case, do a reduced cost fixing here.
  //int fix = 0;
  //reduced_cost_fixing(solver->getReducedCost(), gap,
		      // need to do this since rc_fixing expects non-const
  //	      getLpProblemPointer()->node->vars, fix);
  //printf("MC: rc fixing with exact opt and dj of exact fixed %i vars.\n",
  //	 fix);
  //reduced_cost_fixing(lpres.dj(), gap,
		      // need to do this since rc_fixing expects non-const
  //		      getLpProblemPointer()->node->vars, fix);
  //printf("MC: rc fixing with exact opt and dj of volume fixed %i vars.\n",
  //	 fix);
  return solver;
}

//#############################################################################

void
MC_lp::choose_branching_vars(const BCP_vec<BCP_var*>& vars, const double * x,
			     const int cand_num,
			     BCP_vec<BCP_lp_branching_object*>& cands)
{
  // Try to branch on the variables when they are ordered by abs((distance
  // from integrality) * cost)
  const int m = mc.num_edges;
  const MC_graph_edge* edges = mc.edges;

  BCP_vec<int> perm;
  perm.reserve(m);

  BCP_vec<double> dist;
  dist.reserve(m);

  const double etol = par.entry(MC_lp_par::IntegerTolerance);

  int i;

  // If it's an Ising problem AND there's an external field then check only
  // the edges coming out of the external node
  if (mc.ising_triangles) {
     const int grid = static_cast<int>(sqrt(mc.num_nodes + 1.0));
     i = 2 * grid * grid; // number of grid edges
  } else {
     i = 0;
  }
  for ( ; i < m; ++i) {
     const double xi = x[i];
     if (xi > etol && xi < 1-etol) {
	perm.unchecked_push_back(i);
	const double w = (0.5 - CoinAbs(xi-0.5)) * edges[i].cost;
	dist.unchecked_push_back(-CoinAbs(w)); 
     }
  }

  const int distsize = dist.size();
  if (distsize > cand_num) {
    CoinSort_2(dist.begin(), dist.end(), perm.begin());
    perm.erase(perm.entry(cand_num), perm.end());
  }

  if (perm.size() > 0) {
     append_branching_vars(x, vars, perm, cands);
  }
}

//#############################################################################

BCP_branching_decision
MC_lp::select_branching_candidates(const BCP_lp_result& lpres,
				   const BCP_vec<BCP_var*>& vars,
				   const BCP_vec<BCP_cut*>& cuts,
				   const BCP_lp_var_pool& local_var_pool,
				   const BCP_lp_cut_pool& local_cut_pool,
				   BCP_vec<BCP_lp_branching_object*>& cands)
{
  OsiSolverInterface* lp = getLpProblemPointer()->lp_solver;
  OsiVolSolverInterface* vollp = dynamic_cast<OsiVolSolverInterface*>(lp);
  OsiSolverInterface* exact_solver = NULL;
  double objval = lpres.objval();
  bool tailoff_gap_rel = false;
  bool tailoff_lb_abs = false;
  bool tailoff_lb_rel = false;

  const int solver_t = par.entry(MC_lp_par::LpSolver);
  const bool has_vol = (solver_t & MC_UseVol);
  const bool has_simplex = (solver_t & MC_UseClp)
#ifdef MC_COMPILE_WITH_OSL
     || (solver_t & MC_UseOsl)
#endif
     ;

  if (current_index() != 0 && has_vol && has_simplex) {
     started_exact = true;
  }

  const int iteration_count = current_iteration();

  if (started_exact) {
     // must be has_vol && has_simplex
     exact_solver = solveToOpt(vollp, lpres, vars, cuts, objval);
     if (exact_solver == NULL)
	return BCP_DoNotBranch_Fathomed;
  } else {
     tailoff_test(tailoff_gap_rel, tailoff_lb_abs, tailoff_lb_rel, objval);
     if (has_vol && has_simplex &&
	 (tailoff_gap_rel && (tailoff_lb_abs || tailoff_lb_rel))) {
	started_exact = true;
	exact_solver = solveToOpt(vollp, lpres, vars, cuts, objval);
	if (exact_solver == NULL)
	  return BCP_DoNotBranch_Fathomed;
     }
  }
	
  tailoff_test(tailoff_gap_rel, tailoff_lb_abs, tailoff_lb_rel, objval);

  // Update the history
  if (iteration_count > hist_len) {
     std::rotate(objhist, objhist+1, objhist+hist_len);
     objhist[hist_len-1] = objval;
  } else {
     objhist[iteration_count-1] = objval;
  }

  // Generate cuts from the exact soln (if there's any)
  int pool_size = local_cut_pool.size();
  if (exact_solver) {
     BCP_lp_prob* p = getLpProblemPointer();
     BCP_lp_cut_pool& cp = *p->local_cut_pool;
     BCP_vec<BCP_cut*> new_cuts;
     BCP_vec<BCP_row*> new_rows;
     const CoinPackedMatrix* m = exact_solver->getMatrixByCol();
     double* lhs = new double[m->getNumRows()];
     const double* x = exact_solver->getColSolution();
     double* xx = new double[exact_solver->getNumCols()];
     for (int ii = exact_solver->getNumCols() - 1; ii >= 0; --ii) {
	if (x[ii] < 0.0)
	   xx[ii] = 0.0;
	else if (x[ii] > 1.0)
	   xx[ii] = 1.0;
	else
	   xx[ii] = x[ii];
     }
     m->times(xx, lhs);
     generate_cuts_in_lp(xx, lhs, exact_solver->getObjValue(),
			 vars, cuts, new_cuts, new_rows);
     delete[] xx;
     const int new_size = new_cuts.size();
     if (new_rows.size() != 0) {
	cp.reserve(cp.size() + new_size);
	if (new_rows.size() == 0) {
	   new_rows.reserve(new_size);
	   cuts_to_rows(vars, new_cuts, new_rows,
			lpres, BCP_Object_FromGenerator, false);
	}
	for (int i = 0; i < new_size; ++i) {
	   new_cuts[i]->set_bcpind(-BCP_lp_next_cut_index(*p));
	   cp.unchecked_push_back(new BCP_lp_waiting_row(new_cuts[i],
							 new_rows[i]));
	}
     }
     printf("MC:   Generated %i cuts from exact solution.\n", new_size);
     pool_size = cp.size();
  }

  if (pool_size != 0 &&
      !(tailoff_gap_rel && (tailoff_lb_abs || tailoff_lb_rel))) {
     delete exact_solver;
     return BCP_DoNotBranch;
  }

  // As a last ditch effort, try to solve the problem to optimality to see if
  // we can fathom the node
  if (vollp && !exact_solver) {
     exact_solver = solveToOpt(vollp,lpres, vars, cuts, objval);
     if (exact_solver == NULL) {
	return BCP_DoNotBranch_Fathomed;
     }
  }

  // OK, branch

  choose_branching_vars(vars, lpres.x(),
			par.entry(MC_lp_par::SB_CandidateNum), cands);

  if (cands.size() == 0) {
    // Now THIS is impossible. Well, no... However unlikely, it is possible
    // that we had tailing off, had an integer solution and had violated
    // cycle inequalities. Absurd, isn't it? But it did happen to me...
    // In this case just continue...
    if (local_cut_pool.size() == 0) {
      throw BCP_fatal_error("What the heck?! No cuts, integer & tailoff!\n");
    }
    delete exact_solver;
    return BCP_DoNotBranch;
  }

  // If we have an exact solver (i.e., we used volume) then perform the strong
  // branching ourselves. It's way faster to do it with the simplex. And it
  // may be better, too.
  // BTW, this routine just throws out all but one candidate from cands.
  if (exact_solver) {
     perform_strong_branching(lpres, exact_solver, cands);
     delete exact_solver;
  }

  return BCP_DoBranch;
}

//#############################################################################

BCP_branching_object_relation
MC_lp::compare_branching_candidates(BCP_presolved_lp_brobj* newobj,
				    BCP_presolved_lp_brobj* oldobj)
{
   // *FIXME*: For now just use the default, later we might want to choose the
   // one that makes things more integral.
   return BCP_lp_user::compare_branching_candidates(newobj, oldobj);
}

//#############################################################################

void
MC_lp::set_actions_for_children(BCP_presolved_lp_brobj* best)
{
   // We have stored our own presolved brobj. Just initialize the action
   // vector then swap best with best_presolved, and finally let BCP adjust
   // the actions for the children
   best_presolved->initialize_action();
   best->swap(*best_presolved);
   delete best_presolved;
   best_presolved = 0;
   BCP_lp_user::set_actions_for_children(best);
}

//#############################################################################


void
MC_lp::perform_strong_branching(const BCP_lp_result& lpres,
				OsiSolverInterface* exact_solver,
				BCP_vec<BCP_lp_branching_object*>& cands)
{
   int i;
   
   // prepare for strong branching
   exact_solver->markHotStart();
#ifdef MC_COMPILE_WITH_OSL
   EKKModel* osl =
      dynamic_cast<OsiOslSolverInterface*>(exact_solver)->getModelPtr();
   ekk_messagePrintOff(osl, 1);   // LP opt
   ekk_messagePrintOff(osl, 317); // genral info
   ekk_messagePrintOff(osl, 318); // zeroing it count
   ekk_messagePrintOff(osl, 197); // OSL runtime
   ekk_messagePrintOff(osl, 57);  // iteration info
   ekk_messagePrintOff(osl, 38);  // iteration header
#endif

   // Look at the candidates one-by-one and presolve them.
   BCP_vec<BCP_lp_branching_object*>::iterator cani;

   printf("\n\
MC: Starting strong branching (the objective is transformed!):\
    The objective shift is %.4f\n\n", obj_shift);

   assert(best_presolved == 0);
   int best_fathom_child = 0;
   double best_objsum = DBL_MAX;
   BCP_vec<double> tmpobj;

   for (cani = cands.begin(); cani != cands.end(); ++cani){
      // Create a temporary branching object to hold the current results
      BCP_presolved_lp_brobj* tmp_presolved= new BCP_presolved_lp_brobj(*cani);
      const BCP_lp_branching_object* can = *cani;
      for (i = 0; i < can->child_num; ++i){
	 can->apply_child_bd(exact_solver, i);
	 exact_solver->solveFromHotStart();
	 tmp_presolved->get_results(*exact_solver, i);
      }
      // add the shift to the objective values
      tmp_presolved->get_lower_bounds(tmpobj);
      for (i = 0; i < can->child_num; ++i) {
	 tmpobj[i] += obj_shift;
      }
      tmp_presolved->set_lower_bounds(tmpobj);
      // reset the bounds of the affected var
      const int bvar_ind = (*can->forced_var_pos)[0];
      exact_solver->setColBounds(bvar_ind, 0.0, 1.0);
      if (get_param(BCP_lp_par::LpVerb_PresolveResult)) {
	 printf("MC strong branch:   Presolving:");
	 if (get_param(BCP_lp_par::LpVerb_PresolvePositions)) {
	    can->print_branching_info(exact_solver->getNumCols(), lpres.x(),
				      exact_solver->getObjCoefficients());
	 }
	 for (i = 0; i < can->child_num; ++i) {
	    const BCP_lp_result& res = tmp_presolved->lpres(i);
	    const double lb = res.objval();
	    printf((lb > DBL_MAX / 4 ? " [%e,%i,%i]" : " [%.4f,%i,%i]"),
		   lb, res.termcode(), res.iternum());
	 }
	 printf("\n");
      }
      
      // Compare the current one with the best so far
      // First see how many children would be fathomed
      int fathom_child = 0;
      double objsum = 0.0;
      for (i = 0; i < can->child_num; ++i) {
	 const BCP_lp_result& res = tmp_presolved->lpres(i);
	 if (res.termcode() == BCP_ProvenPrimalInf) {
	    ++fathom_child;
	    continue;
	 }
	 if (res.termcode() == BCP_ProvenOptimal &&
	     res.objval() + obj_shift + get_param(BCP_lp_par::Granularity) >
	     upper_bound()) {
	    ++fathom_child;
	    continue;
	 }
	 objsum += res.objval();
      }
      if (best_fathom_child < fathom_child) {
	 std::swap(tmp_presolved, best_presolved);
	 best_fathom_child = fathom_child;
	 delete tmp_presolved;
	 if (best_fathom_child == can->child_num) {
	    purge_ptr_vector(cands, cani + 1, cands.end());
	 }
	 best_objsum = objsum;
	 continue;
      }

      if (objsum < best_objsum) {
	 best_objsum = objsum;
	 std::swap(tmp_presolved, best_presolved);
      }
      delete tmp_presolved;
   }

   // indicate to the lp solver that strong branching is done
   exact_solver->unmarkHotStart();
   
   // delete all the candidates but the selected one
   BCP_lp_branching_object* can = best_presolved->candidate();
   for (cani = cands.begin(); cani != cands.end(); ++cani) {
      if (*cani != can)
	 delete *cani;
   }

   cands.clear();
   cands.push_back(can);
}
