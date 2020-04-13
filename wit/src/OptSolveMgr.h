//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef OptSolveMgrH
#define OptSolveMgrH

//------------------------------------------------------------------------------
// Header file: "OptSolveMgr.h"
//
// Contains the declaration of class OptSolveMgr
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class OptSolveMgr.
//
// "Opt-Solve Manager"
// Oversees the Opt-Solve Process, the process of solving the optimization
// problem.
// Directly responsible for the aspects of solving the optimization problem
// that do not actually involve invoking the embedded solver.
//
// Implemented in OptSolve.C
//------------------------------------------------------------------------------

class WitOptSolveMgr: public WitProbAssoc
   {
      //------------------------------------------------------------------------
      // Friends.
      //------------------------------------------------------------------------

      friend class WitOptSolveGate;

   public:

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitOptProblem *, myOptProblem)
      accessFunc (bool,            useDualSimplex)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void solveOptProb ();
         //
         // Solves the optimization problem.

      void setUseDualSimplex (bool);
         //
         // Setter

   private:

      //------------------------------------------------------------------------
      // Constructor function.
      // To be called by friend class OptSolveGate.
      //------------------------------------------------------------------------

      WitOptSolveMgr (WitOptProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      // To be called by friend class OptSolveGate.
      //------------------------------------------------------------------------

      ~WitOptSolveMgr ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitOptSolveMgr);

      void solveOptProbAsLexOpt ();
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as a lexicographic optimization.

      void solveOptProbAsLp ();
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as an LP for a first solve.

      void reSolveOptProbAsLp ();
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as an LP for a re-solve.

      void solveOptProbAsMip ();
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as a MIP.

      void issueSolveMsg ();
         //
         // Issues a msg indicating that the solve has begun.

      void writeMps ();
         //
         // Writes an MPS file of the opt problem entered into the solver, if
         // appropriate.

      void solveLexOpt ();
         //
         // Solves the lexicographic optimization problem.

      void setUpLexOptReload ();
         //
         // Sets up lex opt reload mode.

      void prepLexObjElemOpt (WitOptVar * prevOptVar, WitOptVar * theOptVar);
         //
         // Prepares to optimize theOptVar as a lex objective element.
         // prevOptVar is the previous lex obj element, if any, else NULL.

      void lexReloadAndBound (WitOptVar * theOptVar);
         //
         // For lex opt reload mode only.
         // Reloads the opt problem and applies the bounds on the lex
         // objective elements prior to theOptVar.

      void storeOptLexObjElemVal (WitPtrVecItr <WitOptVar> & theOptVarItr);
         //
         // Stores the optimal objective function value for the lex objective
         // element at theOptVarItr

      void loadInitSoln ();
         //
         // If an external initial primal solution is to be used, this function
         // acquires it from myOptProblem_ and then loads it into the solver.
         // Not valid in MIP mode.

      void boundLexObjElemVal (WitOptVar * theOptVar, double theVal);
         //
         // Assuming theOptVar represents a lexicographic objective element,
         // this function bounds the theOptVar to be at least theVal minus a
         // tolerance.

      void solveCurrentObj (bool firstObj);
         //
         // Optimizies the current objective in a lexicographic optimization.
         // firstObj is true, iff the current objective is the first objecetve.

      bool optProbHasIntVars ();
         //
         // Return true, iff the opt problem has at least one integer variable.

      void storePrimalSoln ();
         //
         // Stores the primal solution in myOptProblem_.

      void storeDualSoln ();
         //
         // Stores the dual solution in myOptProblem_.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitOptProblem * const myOptProblem_;
         //
         // The optimization probllem to be solved.

      WitSolverIf * mySolverIf_;
         //
         // The SolverIf owned by this OptSolveMgr.

      bool useDualSimplex_;
         //
         // If true,  dual   simplex is to be used.
         // If false, primal simplex is to be used.

      WitVector <double> optLexObjElemVal_;
         //
         // If mySolverIf_->lexOptReloadNeeded () is true,
         //    optLexObjElemVal_[theIdx] is the optimal objective function
         //    value for the lex objective element whose index in the lex opt
         //    sequence is theIdx.
         // Otherwise optLexObjElemVal_ is not allocated.
   };

#endif
