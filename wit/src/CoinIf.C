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
//
// Class CoinIf is implemented only if COIN_EMBEDDED is defined.
//------------------------------------------------------------------------------

#define CALL_COIN 0
   //
   // 1, if Calls to COIN are to be compiled.
   // 0, if Calls to COIN are to be skipped.

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

//------------------------------------------------------------------------------

WitOpSolverIf * WitOpSolverIf::newInstanceForCoin (
      WitOptProblem * theOptProblem)
   {
   return new WitCoinIf (theOptProblem);
   }

#endif // COIN_EMBEDDED

//------------------------------------------------------------------------------
// Non-COIN-embedded Implementation of OpSolverIf functions.
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
// Implementation of class CoinIf.
//------------------------------------------------------------------------------

#ifdef COIN_EMBEDDED

#include <OptProblem.h>
#include <OptCon.h>
#include <OptVar.h>
#include <MsgFrag.h>
#include <MsgFac.h>
#include <Timing.h>

#include <ClpSimplex.hpp>

//------------------------------------------------------------------------------

WitCoinIf::WitCoinIf (WitOptProblem * theOptProblem):

      WitOpSolverIf (theOptProblem),
      myClpSimplex_ (NULL)
   {
   WitTimer::enterSection ("coin");

#if CALL_COIN

   myClpSimplex_ = new ClpSimplex;

#endif // CALL_COIN

   WitTimer::leaveSection ("coin");
   }

//------------------------------------------------------------------------------

WitCoinIf::~WitCoinIf ()
   {
   WitTimer::enterSection ("coin");

#if CALL_COIN

   delete myClpSimplex_;

#endif // CALL_COIN

   WitTimer::leaveSection ("coin");
   }

//------------------------------------------------------------------------------

void WitCoinIf::reSolveOptProbAsLp ()
   {
   myMsgFac () ("coinNYISmsg", "Accelarted Optimizing Implosion");
   }

//------------------------------------------------------------------------------

void WitCoinIf::solveOptProbAsMip ()
   {
   myMsgFac () ("coinNYISmsg", "Optimizing Implosion in MIP mode");
   }

//------------------------------------------------------------------------------

void WitCoinIf::solveOptProbAsLexOpt ()
   {
   myMsgFac () ("coinNYISmsg", "Optimizing Implosion with Multiple Objectives");
   }

//------------------------------------------------------------------------------

void WitCoinIf::finishSolveOptProbAsLp ()
   {
   myMsgFac () ("coinNYISmsg", "Optimizing Implosion and Stochastic Implosion");
   }

//------------------------------------------------------------------------------

void WitCoinIf::issueSolveMsg ()
   {
   myMsgFac () ("solveOptProblemMsg",
      myMsgFac ().myFrag (mipMode ()? "mipFrag": "lpFrag"),
                          mipMode ()? "CBC":     "CLP");
   }

//------------------------------------------------------------------------------

void WitCoinIf::loadLp ()
   {
   WitVector <int>    start;
   WitVector <int>    index;
   WitVector <double> value;
   WitVector <double> collb;
   WitVector <double> colub;
   WitVector <double> obj;
   WitVector <double> rowlb;
   WitVector <double> rowub;

   myOptProblem ()->getMatrixByCols (start, index, value);

   getColumnData (collb, colub, obj);

   getRowData (rowlb, rowub);

   WitTimer::enterSection ("coin");

#if CALL_COIN

   myClpSimplex_->
      loadProblem (
         myOptProblem ()->nOptVars (),
         myOptProblem ()->nOptCons (),
         start.myCVec (),
         index.myCVec (),
         value.myCVec (),
         collb.myCVec (),
         colub.myCVec (),
         obj  .myCVec (),
         rowlb.myCVec (),
         rowub.myCVec ());

#endif // CALL_COIN

   WitTimer::leaveSection ("coin");
   }

//------------------------------------------------------------------------------

void WitCoinIf::getRowData (
      WitVector <double> & rowlb,
      WitVector <double> & rowub)
   {
   WitOptCon * theOptCon;
   int         theIdx;

   rowlb.resize (myOptProblem ()->nOptCons ());
   rowub.resize (myOptProblem ()->nOptCons ());

   forEachEl (theOptCon, myOptProblem ()->myOptCons ())
      {
      theIdx = theOptCon->index ();

      rowlb[theIdx] = theOptCon->bounds ().lower ();
      rowub[theIdx] = theOptCon->bounds ().upper ();
      }
   }

#endif // COIN_EMBEDDED
