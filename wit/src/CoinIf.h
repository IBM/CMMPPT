//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef CoinIfH
#define CoinIfH

//------------------------------------------------------------------------------
// Header file: "CoinIf.h"
//
// Contains the declaration of class CoinIf.
//------------------------------------------------------------------------------

#include <SolverIf.h>

class ClpSimplex;

//------------------------------------------------------------------------------
// class CoinIf
//
// "COIN Interface"
// Responsible for all interactions with COIN.
//
// Class Hierarchy:
//
// ProbAssoc
//    SolverIf
//       CoinIf
//------------------------------------------------------------------------------

class WitCoinIf: public WitSolverIf
   {
   public:

      //------------------------------------------------------------------------
      // Static public member functions.
      //------------------------------------------------------------------------

      static bool coinEmbedded ();
         //
         // Returns true, iff COIN embedded into the current build of WIT.

      static WitCoinIf * newInstance (WitOptProblem * theOptProblem);
         //
         // If COIN is embedded,
         //    creates and returns a new CoinIf for theOptProblem.
         // If COIN is not embedded,
         //    issues a fatal error.

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitCoinIf (WitOptProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCoinIf ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitCoinIf);

      virtual void reSolveOptProbAsLp   ();
      virtual void solveOptProbAsMip    ();
      virtual void solveOptProbAsLexOpt ();
      virtual void issueSolveMsg        ();
      virtual void loadLp               ();
      virtual void writeMpsSS           ();
      virtual void loadInitSolnSS       (const double *);
      virtual void solveLp              (bool);
      virtual void getPrimalSoln        (WitVector <double> &);
      virtual void getDualSoln          (WitVector <double> &);
         //
         // Overrides from class SolverIf.

      void setUpMessageHandler ();
         //
         // Sets up the COIN Message Handler.

      void shutDownMessageHandler ();
         //
         // Shouts down the COIN Message Handler.

      void getRowData (
            WitVector <double> & rowlb,
            WitVector <double> & rowub);
         //
         // Retrieves the row portion of the LP aspect of the problem in the
         // representation required for loadProblem.

      void checkStatusCode (int statusCode);
         //
         // Checks the status of the LP solution, given by statusCode.

      void reviseLp ();
         //
         // Revises the LP problem that was previously loaded into CLP.

      void reviseVarBounds ();
         //
         // Revises the upper and lower bounds on variables of the LP problem
         // that was previously loaded into CLP.

      void reviseConBounds ();
         //
         // Revises the upper and lower bounds on constraints of the LP problem
         // that was previously loaded into CLP.

      void reviseObjCoeffs ();
         //
         // Revises the objective function coefficients of the LP problem that
         // was previously loaded into CLP.

      void reSolveLp ();
         //
         // Makes appropriate calls to the solver to re-solve the optimization
         // problem as an LP.

      static void enterCoin ();
         //
         // Indicates, for timing purposes, that COIN functions are about to
         // be called.

      static void leaveCoin ();
         //
         // Indicates, for timing purposes, that COIN functions are no longer
         // about to be called.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      ClpSimplex * myClpSimplex_;
         //
         // The ClpSimplex owmned by this CoinIf.
   };

#endif
