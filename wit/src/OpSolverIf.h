//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef OpSolverIfH
#define OpSolverIfH

//------------------------------------------------------------------------------
// Header file: "OpSolverIf.h"
//
// Contains the declaration of class OpSolverIf
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// class OpSolverIf.
// "Optimzation Problem Solver Interface"
// Responsible for all interactions with WIT's optimization problem solver.
// The interactions are implemented polymorphically.
//
// Class Hierarchy:
//
// ProbAssoc
//    OpSolverIf
//       CoinIf
//       CplexIf
//------------------------------------------------------------------------------

class WitOpSolverIf: public WitProbAssoc
   {
   public:

      static bool coinEmbedded ();
         //
         // Returns true, iff COIN embedded into the current build of WIT.
         // Implemented in CoinIf.C.

      static bool cplexEmbedded ();
         //
         // Returns true, iff CPLEX was embedded into the current build of WIT.
         // Implemented in CplexIf.C.

      static WitOpSolverIf * newInstance (WitOptProblem * theOptProblem);
         //
         // Creates and returns a new OpSolverIf for theOptProblem.
         // Issues a fatal error, if CPLEX is not embedded.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitOpSolverIf ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void solveOptProb ();
         //
         // Solves the optimization problem.

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOpSolverIf (WitOptProblem *);

      //------------------------------------------------------------------------
      // Other protected member functions.
      //------------------------------------------------------------------------

      void solveOptProbAsLp ();
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as an LP for a first solve.

      virtual void reSolveOptProbAsLp () = 0;
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as an LP for a re-solve.

      virtual void solveOptProbAsMip () = 0;
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as a MIP.

      virtual void solveOptProbAsLexOpt () = 0;
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as a lexicographic optimization.

      virtual void finishSolveOptProbAsLp () = 0;
         //
         // Does all aspects of solveOptProbAsLp () that have not been moved
         // to more specific functions.

      virtual void issueSolveMsg () = 0;
         //
         // Issues a msg for the solve.

      virtual void loadLp () = 0;
         //
         // Loads the optimization problem into CPLEX as an LP.

      void getColumnData (
            WitVector <double> & lb,
            WitVector <double> & ub,
            WitVector <double> & obj);
         //
         // Retrieves the column portion of the LP aspect of the problem in a
         // standard format.

      bool mipMode ();
         //
         // Returns true, iff myOptProblem is a MIP.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitOptProblem *, myOptProblem)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitOpSolverIf);

      static WitOpSolverIf * newInstanceForCoin (WitOptProblem * theOptProblem);
         //
         // If COIN is embedded,
         //    creates and returns a new OpSolverIf for COIN and theOptProblem.
         // If COIN is not embedded,
         //    issues a fatal error.
         // Implemented in CoinIf.C.

      static WitOpSolverIf * newInstanceForCplex (WitOptProblem * theOptProblem);
         //
         // If CPLEX is embedded,
         //    creates and returns a new OpSolverIf for CPLEX and theOptProblem.
         // If CPLEX is not embedded,
         //    issues a fatal error.
         // Implemented in CplexIf.C.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitOptProblem * const myOptProblem_;
         //
         // The OptProblem that owns this OpSolverIf.
   };

#endif
