//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef SolveGateH
#define SolveGateH

//------------------------------------------------------------------------------
// Header file: "SolveGate.h"
//
// Contains the declaration of class SolveGate
//------------------------------------------------------------------------------

#include <typedefs.h>

//------------------------------------------------------------------------------
// Class SolveGate.
//
// Gateway (i.e., Facade) to the Solver Subsystem.
// The Solver Subsystem is responsible for solving the optimization problem
// using an embedded solver, either COIN or CPLEX.
// It consists of the following classes:
//    SolveGate
//    SolveMgr
//    SolverIf
//    CoinIf
//    CplexIf
//
// Implemented in Solve.C.
//------------------------------------------------------------------------------

class WitSolveGate
   {
   public:

      //------------------------------------------------------------------------
      // Static public member functions.
      //------------------------------------------------------------------------

      static bool coinEmbedded ();
         //
         // Returns true, iff COIN embedded into the current build of WIT.

      static bool cplexEmbedded ();
         //
         // Returns true, iff CPLEX embedded into the current build of WIT.

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSolveGate (WitOptProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitSolveGate ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void solveOptProb ();
         //
         // Solves the optimization problem.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitSolveGate);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitSolveMgr * const mySolveMgr_;
         //
         // The SolveMgr owned by this SolverIf.
   };


#endif
