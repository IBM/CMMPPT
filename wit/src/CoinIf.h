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

#include <OpSolverIf.h>

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
//    OpSolverIf
//       CoinIf
//------------------------------------------------------------------------------

class WitCoinIf: public WitOpSolverIf
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
         //
         // Overrides from class OpSolverIf.

      void getRowData (
            WitVector <double> & rowlb,
            WitVector <double> & rowub);
         //
         // Retrieves the row portion of the LP aspect of the problem in the
         // representation required for loadProblem.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      ClpSimplex * myClpSimplex_;
         //
         // The ClpSimplex owmned by this CoinIf.
   };

#endif
