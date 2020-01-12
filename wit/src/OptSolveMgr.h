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

      void issueSolveMsg ();
         //
         // Issues a msg indicating that the solve has begun.

      void writeMps ();
         //
         // Writes an MPS file of the opt problem entered into the solver, if
         // appropriate.

      void loadInitSoln ();
         //
         // If an external initial primal solution is to be used, this function
         // acquires it from myOptProblem_ and then loads it into the solver.

      void storePrimalSoln ();
         //
         // Stores the primal solution in myOptProblem_.

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

      void solveOptProbAsLp ();
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as an LP for a first solve.

      void reSolveOptProbAsLp ();
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as an LP for a re-solve.

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
   };

#endif
