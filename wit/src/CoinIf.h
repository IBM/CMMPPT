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

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void solveOptProb ();
         //
         // Solves the optimization problem by invoking CPLEX.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitCoinIf);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------
   };

#endif
