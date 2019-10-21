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

      virtual void writeMpsSS () = 0;
         //
         // Does the solver-specific aspects of writeMps ().

      virtual void setLpMethodByOptStarter () = 0;
         //
         // Sets the LP method to be used by the solver based on the OptStarter.

      virtual void loadInitSolnSS (const double * initSoln) = 0;
         //
         // Does the solver-specific aspects of loadInitSoln ().
         // initSoln is the initial solution to be loaded.

      virtual void setParams ();
         //
         // Sets the solver's parameters, if appropraite.
         // Default behavior: No-op.

      virtual void finishSolveLp (bool optNeeded) = 0;
         //
         // Does all aspects of solveLp (optNeeded) that have not been moved
         // to more specific functions.

      void writeMps ();
         //
         // Writes an MPS file of the opt problem entered into CPLEX, if
         // appropriate.
         // This function should probably be private eventually.

      void loadInitSoln ();
         //
         // Loads the initial primal solution from myOptProblem () into the
         // solver, as needed.
         // This function should probably be private eventually.

      void getColumnData (
            WitVector <double> & lb,
            WitVector <double> & ub,
            WitVector <double> & obj);
         //
         // Retrieves the column portion of the LP aspect of the problem in a
         // standard format.

      void solveLp (bool optNeeded);
         //
         // Makes appropriate calls to the solver to solve the optimization
         // problem as an LP.
         // optNeeded is to be true, iff an optimal solution is required.
         // This function should probably be private eventually.

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

      void solveOptProbAsLp ();
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as an LP for a first solve.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitOptProblem * const myOptProblem_;
         //
         // The OptProblem that owns this SolverIf.
   };

#endif
