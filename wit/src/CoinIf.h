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

      static WitCoinIf * newInstanceIfAllowed (WitOptProblem * theOptProblem);
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

      virtual void reSolveOptProbAsLp     ();
      virtual void solveOptProbAsMip      ();
      virtual void solveOptProbAsLexOpt   ();
      virtual void finishSolveOptProbAsLp ();
      virtual void issueSolveMsg          ();
      virtual void loadLp                 ();
      virtual void writeMpsSS             ();
      virtual void loadInitSolnSS         (const double *);
      virtual void solveLp                (bool);
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

      void checkLpSolnStatus ();
         //
         // Checks the status of the LP solution.

      static void enteringCoin ();
         //
         // Indicates, for timing purposes, that a COIN function is about to
         // be called.

      static void leftCoin ();
         //
         // Indicates, for timing purposes, that a COIN function was just
         // called.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      ClpSimplex * myClpSimplex_;
         //
         // The ClpSimplex owmned by this CoinIf.
   };

#endif
