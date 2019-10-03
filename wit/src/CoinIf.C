//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "CoinIf.C"
//
// Contains the implementations of member functions of class OpSolverIf whose
//    implementation depends on whether or not COIN_EMBEDDED is defined.
//
// Contains the implementation of class CoinIf.
//------------------------------------------------------------------------------

#include <CoinIf.h>

//------------------------------------------------------------------------------
// COIN-embedded Implementation of OpSolverIf functions.
//------------------------------------------------------------------------------

#ifdef COIN_EMBEDDED

bool WitOpSolverIf::coinEmbedded ()
   {
   return true;
   }

#endif // COIN_EMBEDDED

//------------------------------------------------------------------------------
// COIN-embedded Implementation of OpSolverIf functions.
//------------------------------------------------------------------------------

#ifndef COIN_EMBEDDED

bool WitOpSolverIf::coinEmbedded ()
   {
   return false;
   }

#endif // not COIN_EMBEDDED

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
