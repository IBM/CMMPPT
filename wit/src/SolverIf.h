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
// Class SolverIf.
// "Solver Interface"
// Abstract base class
// The classes derived from this class are responsible for all interactions
// with the embedded solver of the optimization problem.
//
// Class Hierarchy:
//
// ProbAssoc
//    SolverIf
//       CoinComIf
//          CoinLpIf
//          CoinMipIf
//       CplexIf
//
// Implemented in OptSolve.C
//------------------------------------------------------------------------------

class WitSolverIf: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitSolverIf ();

      //------------------------------------------------------------------------
      // Pure virtual public member functions.
      //------------------------------------------------------------------------

      virtual void issueVersionMsg () = 0;
         //
         // Issues a msg indicating the version # of the solver, if possible.

      virtual void loadLp () = 0;
         //
         // Loads the optimization problem into CPLEX as an LP.

      virtual void loadIntData () = 0;
         //
         // Loads the integrality data of the opt problem into the solver.

      virtual void reviseLp () = 0;
         //
         // Revises the LP problem that was previously loaded into the solver.

      virtual void solverWriteMps () = 0;
         //
         // Calls the solver to write an MPS file

      virtual void loadInitSoln (const WitVector <double> & initSoln) = 0;
         //
         // Loads the initial primal solution (initSoln) into the solver.

      virtual void reSolveLp () = 0;
         //
         // Makes appropriate calls to the solver to re-solve the optimization
         // problem as an LP.

      virtual void solveLp (bool optNeeded) = 0;
         //
         // Makes appropriate calls to the solver to solve the optimization
         // problem as an LP.
         // optNeeded is to be true, iff an optimal solution is required.

      virtual void solveMip (bool optNeeded) = 0;
         //
         // Makes appropriate calls to the solver to solve the optimization
         // problem as a MIP.
         // optNeeded is to be true, iff an optimal solution is required.

      virtual void setVarLB (int theColIdx, double theLB) = 0;
         //
         // Sets the lower bound on the variable whose index is theColIdx to
         // theLB.

      virtual void setObjCoeff (int theColIdx, double theVal) = 0;
         //
         // Sets the objective coefficient on the variable whose index is
         // theColIdx to theVal.

      virtual double primalVarVal (int theColIdx) = 0;
         //
         // Returns the value of the primal variable whose index is theColIdx.

      virtual void getPrimalSoln (WitVector <double> & primalSoln) = 0;
         //
         // Sets primalSoln to the vector of primal solution values.
         // primalSoln must already be of the appropriate size.

      virtual void getDualSoln (WitVector <double> & dualSoln) = 0;
         //
         // Sets dualSoln to the vector of dual solution values.
         // dualSoln must already be of the appropriate size.

      virtual const char * solverName () = 0;
         //
         // Returns the name of the solver.

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSolverIf (WitOptSolveMgr *);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitOptSolveMgr *, myOptSolveMgr)
      accessFunc (WitOptProblem *,  myOptProblem)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitSolverIf);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitOptSolveMgr * const myOptSolveMgr_;
         //
         // The OptSolveMgr that owns this SolverIf.

      WitOptProblem * const myOptProblem_;
         //
         // The OptProblem for this SolverIf.
   };

#endif
