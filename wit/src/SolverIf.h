//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
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
// with the embedded solver of optimization problems.
//
// Class Hierarchy:
//
// ProbAssoc
//    SolverIf
//       CoinIf
//          CoinLpIf
//          CoinMipIf
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
      // Virtual public member functions.
      //------------------------------------------------------------------------

      virtual void issueVersionMsg ();
         //
         // Issues a msg indicating the version # of the solver, if possible.
         // The default implementation does nothing.

      virtual bool lexOptNeedsReload ();
         //
         // Returns true, iff lexicographic optimization needs to be done by the
         // "reload" algorithm, when this SolverIf is being used.
         // The default implementation returns false.

      virtual void loadLp () = 0;
         //
         // Loads the optimization problem into CPLEX as an LP.

      virtual void loadIntData ();
         //
         // Loads the integrality data of the opt problem into the solver.
         // The default implementation issues a fatal error.

      virtual void reviseLp ();
         //
         // Revises the LP problem that was previously loaded into the solver.
         // The default implementation issues a fatal error.

      virtual void solverWriteMps () = 0;
         //
         // Calls the solver to write an MPS file

      virtual void loadInitSoln (const WitVector <double> & initSoln);
         //
         // Loads the initial primal solution (initSoln) into the solver.
         // The default implementation issues a fatal error.

      virtual void solveLp (bool reqPrimalSimplex);
         //
         // Makes appropriate calls to the solver to solve the optimization
         // problem as an LP.
         // If reqPrimalSimplex is true, the Primal Simplex Method will be used.
         // Otherwise, the solver selects the method (Primal or Dual Simplex).
         // The default implementation issues a fatal error.

      virtual void reSolveLp ();
         //
         // Makes appropriate calls to the solver to re-solve the optimization
         // problem as an LP.
         // The default implementation issues a fatal error.

      virtual void reSolveLexLp ();
         //
         // Makes appropriate calls to the solver to solve the optimization
         // problem as an LP, assuming it's for the second or subsequent
         // iteration of a lexicographic optimization.
         // The default implementation issues a fatal error.

      virtual void solveMip ();
         //
         // Makes appropriate calls to the solver to solve the optimization
         // problem as a MIP.
         // The default implementation issues a fatal error.

      virtual void setVarLB (WitOptVar * theOptVar, double theLB) = 0;
         //
         // Sets the lower bound on theOptVar to theLB.

      virtual void setObjCoeff (WitOptVar * theOptVar, double theVal) = 0;
         //
         // Sets the objective coefficient on theOptVar to theVal.

      virtual double primalVarVal (WitOptVar * theOptVar) = 0;
         //
         // Returns the primal variable value of theOptVar.

      virtual void getPrimalSoln (WitVector <double> & primalSoln) = 0;
         //
         // Sets primalSoln to the vector of primal solution values.
         // primalSoln must already be of the appropriate size.

      virtual void getDualSoln (WitVector <double> & dualSoln);
         //
         // Sets dualSoln to the vector of dual solution values.
         // dualSoln must already be of the appropriate size.
         // The default implementation issues a fatal error.

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

      WitSolverIf (WitOptProblem *);

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
         // The OptProblem for this SolverIf.
   };

#endif
