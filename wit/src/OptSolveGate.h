//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef OptSolveGateH
#define OptSolveGateH

//------------------------------------------------------------------------------
// Header file: "OptSolveGate.h"
//
// Contains the declaration of class OptSolveGate
//------------------------------------------------------------------------------

#include <typedefs.h>

//------------------------------------------------------------------------------
// Class OptSolveGate.
//
// Gateway (i.e., Facade) to the Opt-Solve Subsystem.
// The Opt-Solve Subsystem is responsible for solving the optimization problem
// using an embedded solver, either COIN or CPLEX.
// It consists of the following classes:
//    OptSolveGate
//    OptSolveMgr
//    SolverIf
//    CoinIf
//    CplexIf
//
// Implemented in OptSolve.C.
//------------------------------------------------------------------------------

class WitOptSolveGate
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

      WitOptSolveGate (WitOptProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitOptSolveGate ();

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

      noCopyCtorAssign (WitOptSolveGate);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitOptSolveMgr * const myOptSolveMgr_;
         //
         // The OptSolveMgr owned by this SolverIf.
   };


#endif
