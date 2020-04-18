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

class OsiSolverInterface;
class CoinMessageHandler;

//------------------------------------------------------------------------------
// Class CoinIf
//
// "COIN Interface"
// Responsible for all interactions with COIN that occur in BOTH LP mode and
// MIP mode.
//
// Class Hierarchy:
//
// ProbAssoc
//    SolverIf
//       CoinIf
//          CoinLpIf
//          CoinMipIf
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
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCoinIf ();

      //------------------------------------------------------------------------
      // Overrides from class SolverIf.
      //------------------------------------------------------------------------

      virtual void   loadLp         ();
      virtual void   solverWriteMps ();
      virtual void   setVarLB       (WitOptVar *, double);
      virtual void   setObjCoeff    (WitOptVar *, double);
      virtual double primalVarVal   (WitOptVar *);
      virtual void   getPrimalSoln  (WitVector <double> &);

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitCoinIf (WitOptProblem *);

      //------------------------------------------------------------------------
      // Other protected constructor functions.
      //------------------------------------------------------------------------

      virtual OsiSolverInterface * myOsiSI () = 0;
         //
         // Returns the OsiSolverInterface for this CoinIf.

      accessFunc (CoinMessageHandler *, myMsgHandler)

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

      noCopyCtorAssign (WitCoinIf);

      void setUpMessageHandler ();
         //
         // Sets up the COIN Message Handler.

      void shutDownMessageHandler ();
         //
         // Shuts down the COIN Message Handler.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      CoinMessageHandler * myMsgHandler_;
         //
         // The CoinMessageHandler owned by this CoinLpIf.
   };

#endif
