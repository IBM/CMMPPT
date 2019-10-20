//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "CoinIf.C"
//
// Contains the implementation of class CoinIf.
// If COIN_EMBEDDED is not defined, then only static functions are implemented.
//------------------------------------------------------------------------------

#include <CoinIf.h>

//------------------------------------------------------------------------------
// COIN-embedded Implementation of class CoinIf.
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

bool WitCoinIf::coinEmbedded ()
   {
   return true;
   }

//------------------------------------------------------------------------------

WitCoinIf * WitCoinIf::newInstanceIfAllowed (WitOptProblem * theOptProblem)
   {
   return new WitCoinIf (theOptProblem);
   }

//------------------------------------------------------------------------------

WitCoinIf::WitCoinIf (WitOptProblem * theOptProblem):

      WitSolverIf   (theOptProblem),
      myClpSimplex_ (NULL)
   {
   WitTimer::enterSection ("coin");

   myClpSimplex_ = new ClpSimplex;

   WitTimer::leaveSection ("coin");
   }

//------------------------------------------------------------------------------

WitCoinIf::~WitCoinIf ()
   {
   WitTimer::enterSection ("coin");

   delete myClpSimplex_;

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

   myClpSimplex_->setOptimizationDirection (-1.0);

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

   WitTimer::leaveSection ("coin");
   }

//------------------------------------------------------------------------------

void WitCoinIf::writeMpsSS ()
   {
   int errCode;

   WitTimer::enterSection ("coin");

   try
      {
      errCode = myClpSimplex_->writeMps ("opt-prob.mps", 0, 1, -1.0);
      }

   catch (...)
      {
      myMsgFac () ("clpWriteMpsExcSmsg", "opt-prob.mps");
      }

   WitTimer::leaveSection ("coin");

   if (errCode != 0)
      myMsgFac () ("clpWriteMpsErrSmsg", errCode);
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

//------------------------------------------------------------------------------
// Non-COIN-embedded Implementation of class CoinIf.
//------------------------------------------------------------------------------

#ifndef COIN_EMBEDDED

bool WitCoinIf::coinEmbedded ()
   {
   return false;
   }

//------------------------------------------------------------------------------

WitCoinIf * WitCoinIf::newInstanceIfAllowed (WitOptProblem *)
   {
   stronglyAssert (false);

   return NULL;
   }

#endif // Not COIN_EMBEDDED
