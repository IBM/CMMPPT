//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef CoinComIfH
#define CoinComIfH

//------------------------------------------------------------------------------
// Header file: "CoinComIf.h"
//
// Contains the declaration of class CoinComIf.
//------------------------------------------------------------------------------

#include <SolverIf.h>

class ClpModel;

//------------------------------------------------------------------------------
// Class CoinComIf
//
// "COIN Common Interface"
// Responsible for all interactions with COIN that occur in BOTH LP mode and
// MIP mode.
//
// Class Hierarchy:
//
// ProbAssoc
//    SolverIf
//       CoinComIf
//          CoinLpIf
//          CoinMipIf
//------------------------------------------------------------------------------

class WitCoinComIf: public WitSolverIf
   {
   public:

      //------------------------------------------------------------------------
      // Static public member functions.
      //------------------------------------------------------------------------

      static bool coinEmbedded ();
         //
         // Returns true, iff COIN embedded into the current build of WIT.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCoinComIf ();

      //------------------------------------------------------------------------
      // Overrides from class SolverIf.
      //------------------------------------------------------------------------

      virtual void solveOptProbAsLexOpt ();
      virtual void issueVersionMsg      ();
      virtual void loadLp               ();
      virtual void solverWriteMps       ();
      virtual void getPrimalSoln        (WitVector <double> &);

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitCoinComIf (WitOptSolveMgr *);

      //------------------------------------------------------------------------
      // Other protected constructor functions.
      //------------------------------------------------------------------------

      void setUpMessageHandler ();
         //
         // Sets up the COIN Message Handler.

      void shutDownMessageHandler ();
         //
         // Shuts down the COIN Message Handler.

      void checkStatusCode (int statusCode);
         //
         // Checks the status of the LP solution, given by statusCode.

      virtual ClpModel * myClpModel () = 0;
         //
         // Returns the ClpModel for this CoinComIf.

      static void enterCoin ();
         //
         // Indicates, for timing purposes, that COIN functions are about to
         // be called.

      static void leaveCoin ();
         //
         // Indicates, for timing purposes, that COIN functions are no longer
         // about to be called.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitCoinComIf);
   };

#endif
