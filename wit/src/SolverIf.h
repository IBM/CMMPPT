//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef SolverIfH
#define SolverIfH

//------------------------------------------------------------------------------
// Header file: "SolverIf.h"
//
// Contains the declaration of class SolverIf
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// class SolverIf.
// "Solver Interface"
// Responsible for all interactions with WIT's solver of optimization problems.
// The interactions are implemented polymorphically.
//
// Class Hierarchy:
//
// ProbAssoc
//    SolverIf
//       CoinIf
//       CplexIf
//------------------------------------------------------------------------------

class WitSolverIf: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitSolverIf ();

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

      WitSolverIf (WitOptProblem *);

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

      noCopyCtorAssign (WitSolverIf);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitOptProblem * const myOptProblem_;
         //
         // The OptProblem that owns this SolverIf.
   };

#endif
