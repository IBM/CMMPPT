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

      virtual void solveOptProbAsLp     ();
      virtual void reSolveOptProbAsLp   ();
      virtual void solveOptProbAsMip    ();
      virtual void solveOptProbAsLexOpt ();
         //
         // Overrides from class OpSolverIf.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------
   };

#endif
