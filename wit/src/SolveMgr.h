//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef SolveMgrH
#define SolveMgrH

//------------------------------------------------------------------------------
// Header file: "SolveMgr.h"
//
// Contains the declaration of class SolveMgr
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class SolveMgr.
//
// "Solve Manager"
// Oversees the Solve Process, the process of solving the optimization problem.
// Directly responsible for the aspects of solving the optimization problem
// that do not actually involve invoking the embedded solver.
//
// Implemented in Solve.C
//------------------------------------------------------------------------------

class WitSolveMgr: public WitProbAssoc
   {
      //------------------------------------------------------------------------
      // Friends.
      //------------------------------------------------------------------------

      friend class WitSolveGate;

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

      void getColumnData (
            WitVector <double> & lb,
            WitVector <double> & ub,
            WitVector <double> & obj);
         //
         // Retrieves the column portion of the LP aspect of the problem in a
         // standard format.

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

      void storeDualSoln ();
         //
         // Stores the dual solution in myOptProblem_.

      void setUseDualSimplex (bool);
         //
         // Setter

   private:

      //------------------------------------------------------------------------
      // Constructor function.
      // To be called by friend class SolveGate.
      //------------------------------------------------------------------------

      WitSolveMgr (WitOptProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      // To be called by friend class SolveGate.
      //------------------------------------------------------------------------

      ~WitSolveMgr ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitSolveMgr);

      void solveOptProbAsLp ();
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as an LP for a first solve.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitOptProblem * const myOptProblem_;
         //
         // The optimization probllem to be solved.

      WitSolverIf * mySolverIf_;
         //
         // The SolverIf owned by this SolveMgr.

      bool useDualSimplex_;
         //
         // If true,  dual   simplex is to be used.
         // If false, primal simplex is to be used.
   };

#endif
