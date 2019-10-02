//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "CoinIf.C"
//
// Contains the implementation of class CoinIf.
//------------------------------------------------------------------------------

#include <CoinIf.h>

//------------------------------------------------------------------------------

WitCoinIf::WitCoinIf (WitOptProblem * theOptProblem):

      WitOpSolverIf (theOptProblem)
   {
   }

//------------------------------------------------------------------------------

WitCoinIf::~WitCoinIf ()
   {
   }

//------------------------------------------------------------------------------

void WitCoinIf::solveOptProb ()
   {
   stronglyAssert (false);
   }
