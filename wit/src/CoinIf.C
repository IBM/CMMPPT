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
//    The implementation of class CoinIf is compiled only if COIN_EMBEDDED is
//    defined.
//------------------------------------------------------------------------------

#include <CoinIf.h>
#include <OptProblem.h>
#include <MsgFac.h>
#include <Timing.h>

//------------------------------------------------------------------------------
// COIN-embedded Implementation of OpSolverIf functions.
//------------------------------------------------------------------------------

#ifdef COIN_EMBEDDED

bool WitOpSolverIf::coinEmbedded ()
   {
   return true;
   }

//------------------------------------------------------------------------------

WitOpSolverIf * WitOpSolverIf::newInstanceForCoin (
      WitOptProblem * theOptProblem)
   {
   return new WitCoinIf (theOptProblem);
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

//------------------------------------------------------------------------------

WitOpSolverIf * WitOpSolverIf::newInstanceForCoin (WitOptProblem *)
   {
   stronglyAssert (false);

   return NULL;
   }

#endif // not COIN_EMBEDDED

//------------------------------------------------------------------------------

#ifdef COIN_EMBEDDED

WitCoinIf::WitCoinIf (WitOptProblem * theOptProblem):

      WitOpSolverIf (theOptProblem)
   {
   }

//------------------------------------------------------------------------------

WitCoinIf::~WitCoinIf ()
   {
   }

//------------------------------------------------------------------------------

void WitCoinIf::solveOptProbAsLp ()
   {
   WitTimer::enterSection ("coin");

   myMsgFac () ("coinNYISmsg", "Optimizing Implosion and Stochastic Implosion");

   WitTimer::leaveSection ("coin");
   }

//------------------------------------------------------------------------------

void WitCoinIf::reSolveOptProbAsLp ()
   {
   WitTimer::enterSection ("coin");

   myMsgFac () ("coinNYISmsg", "Accelarted Optimizing Implosion");

   WitTimer::leaveSection ("coin");
   }

//------------------------------------------------------------------------------

void WitCoinIf::solveOptProbAsMip ()
   {
   WitTimer::enterSection ("coin");

   myMsgFac () ("coinNYISmsg", "Optimizing Implosion in MIP mode");

   WitTimer::leaveSection ("coin");
   }

//------------------------------------------------------------------------------

void WitCoinIf::solveOptProbAsLexOpt ()
   {
   WitTimer::enterSection ("coin");

   myMsgFac () ("coinNYISmsg", "Optimizing Implosion with Multiple Objectives");

   WitTimer::leaveSection ("coin");
   }

#endif // COIN_EMBEDDED
