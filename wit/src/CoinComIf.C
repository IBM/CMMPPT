//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "CoinComIf.C"
//
// Contains the implementation of class CoinComIf.
// If COIN_EMBEDDED is not defined, then only static functions are implemented.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// All #include statements for WIT header files must be located in the
// unconditional part of the source file, so that make depend will work
// correctly.
//------------------------------------------------------------------------------

#include <CoinComIf.h>
#include <OptComp.h>
#include <OptProblem.h>
#include <MsgFac.h>
#include <Timing.h>

//------------------------------------------------------------------------------
// COIN-embedded Implementation of class CoinComIf.
//------------------------------------------------------------------------------

#ifdef COIN_EMBEDDED

#include <ClpSimplex.hpp>

//------------------------------------------------------------------------------

bool WitCoinComIf::coinEmbedded ()
   {
   return true;
   }

//------------------------------------------------------------------------------

WitCoinComIf::~WitCoinComIf ()
   {
   shutDownMessageHandler ();
   }

//------------------------------------------------------------------------------

void WitCoinComIf::solveOptProbAsLexOpt ()
   {
   myMsgFac () ("coinNYISmsg", "Optimizing Implosion with Multiple Objectives");
   }

//------------------------------------------------------------------------------

void WitCoinComIf::issueVersionMsg ()
   {
   }

//------------------------------------------------------------------------------

void WitCoinComIf::loadLp ()
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

   myOptProblem ()->getColumnData   (collb, colub, obj);
   myOptProblem ()->getRowData      (rowlb, rowub);

   enterCoin ();

   myClpModel ()->setOptimizationDirection (-1.0);

   myClpModel ()->
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

   leaveCoin ();
   }

//------------------------------------------------------------------------------

void WitCoinComIf::solverWriteMps ()
   {
   int errCode;

   try
      {
      enterCoin ();

      errCode = myClpModel ()->writeMps ("opt-prob.mps", 0, 1, -1.0);

      leaveCoin ();
      }

   catch (...)
      {
      myMsgFac () ("clpWriteMpsExcSmsg", "opt-prob.mps");
      }

   if (errCode != 0)
      myMsgFac () ("clpWriteMpsErrSmsg", errCode);
   }

//------------------------------------------------------------------------------

void WitCoinComIf::getPrimalSoln (WitVector <double> & primalSoln)
   {
   primalSoln = myClpModel ()->getColSolution ();
   }

//------------------------------------------------------------------------------

WitCoinComIf::WitCoinComIf (WitOptSolveMgr * theOptSolveMgr):

      WitSolverIf   (theOptSolveMgr),
      myMsgHandler_ (NULL)
   {
   setUpMessageHandler ();
   }

//------------------------------------------------------------------------------

void WitCoinComIf::enterCoin ()
   {
   WitTimer::enterSection ("coin");
   }

//------------------------------------------------------------------------------

void WitCoinComIf::leaveCoin ()
   {
   WitTimer::leaveSection ("coin");
   }

//------------------------------------------------------------------------------

void WitCoinComIf::setUpMessageHandler ()
   {
   FILE * theFile;

   theFile = openFile (myOptComp ()->solverLogFileName ().myCstring (), "w");

   enterCoin ();

   myMsgHandler_ = new CoinMessageHandler (theFile);

   leaveCoin ();
   }

//------------------------------------------------------------------------------

void WitCoinComIf::shutDownMessageHandler ()
   {
   FILE * theFile;

   enterCoin ();

   theFile = myMsgHandler_->filePointer ();

   delete myMsgHandler_;

   myMsgHandler_ = NULL;

   leaveCoin ();

   fclose (theFile);
   }

#endif // COIN_EMBEDDED

//------------------------------------------------------------------------------
// Non-COIN-embedded Implementation of class CoinComIf.
//------------------------------------------------------------------------------

#ifndef COIN_EMBEDDED

bool WitCoinComIf::coinEmbedded ()
   {
   return false;
   }

#endif // Not COIN_EMBEDDED
