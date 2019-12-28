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

#include <OptComp.h>
#include <OptProblem.h>
#include <OptCon.h>
#include <OptVar.h>
#include <OptStarter.h>
#include <MsgFrag.h>
#include <MsgFac.h>
#include <Timing.h>

#include <ClpSimplex.hpp>
#include <ClpPresolve.hpp>

//------------------------------------------------------------------------------

bool WitCoinIf::coinEmbedded ()
   {
   return true;
   }

//------------------------------------------------------------------------------

WitCoinIf * WitCoinIf::newInstance (WitOptProblem * theOptProblem)
   {
   return new WitCoinIf (theOptProblem);
   }

//------------------------------------------------------------------------------

WitCoinIf::WitCoinIf (WitOptProblem * theOptProblem):

      WitSolverIf   (theOptProblem),
      myClpSimplex_ (NULL)
   {
   enterCoin ();

   myClpSimplex_ = new ClpSimplex;

   leaveCoin ();

   setUpMessageHandler ();
   }

//------------------------------------------------------------------------------

WitCoinIf::~WitCoinIf ()
   {
   shutDownMessageHandler ();

   enterCoin ();

   delete myClpSimplex_;

   leaveCoin ();
   }

//------------------------------------------------------------------------------

void WitCoinIf::reSolveOptProbAsLp ()
   {
   myMsgFac () ("coinNYISmsg", "Accelerated Optimizing Implosion");
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

   enterCoin ();

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

   leaveCoin ();
   }

//------------------------------------------------------------------------------

void WitCoinIf::writeMpsSS ()
   {
   int errCode;

   try
      {
      enterCoin ();

      errCode = myClpSimplex_->writeMps ("opt-prob.mps", 0, 1, -1.0);

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

void WitCoinIf::loadInitSolnSS (const double * initSoln)
   {
   enterCoin ();

   myClpSimplex_->setColSolution (initSoln);

   leaveCoin ();
   }

//------------------------------------------------------------------------------
// solveLp
//
// dual   is called with ifValuesPass == 0, meaning don't do a values pass.
// primal is called with ifValuesPass == 1, meaning       do a values pass.
// Both  are called with startFinshOptions == 0.
//------------------------------------------------------------------------------

void WitCoinIf::solveLp (bool)
   {
   ClpPresolve * theClpPresolve;
   ClpSimplex *  psClpSimplex;
   int           statusCode;
   int           nIters;

   enterCoin ();

   theClpPresolve = new ClpPresolve;

   psClpSimplex = theClpPresolve->presolvedModel (* myClpSimplex_);

   if (psClpSimplex == NULL)
      myMsgFac () ("unboundedOrInfeasSmsg");

   if (useDualSimplex ())
      psClpSimplex->dual   (0, 0);
   else
      psClpSimplex->primal (1, 0);

   nIters     = psClpSimplex->numberIterations ();

   statusCode = psClpSimplex->problemStatus ();

   leaveCoin ();

   checkStatusCode (statusCode);

   enterCoin ();

   theClpPresolve->postsolve (true);

   delete theClpPresolve;

   delete psClpSimplex;

   if (not myClpSimplex_->isProvenOptimal ())
      {
      myClpSimplex_->primal (1, 0);

      nIters += myClpSimplex_->numberIterations ();
      }

   leaveCoin ();

   myMsgFac () ("nSimplexItersMsg", nIters);
   }

//------------------------------------------------------------------------------

void WitCoinIf::getPrimalSoln (WitVector <double> & primalSoln)
   {
   primalSoln = myClpSimplex_->getColSolution ();
   }

//------------------------------------------------------------------------------

void WitCoinIf::getDualSoln (WitVector <double> & dualSoln)
   {
   dualSoln = myClpSimplex_->getRowPrice ();
   }

//------------------------------------------------------------------------------

void WitCoinIf::setUpMessageHandler ()
   {
   FILE *               theFile;
   CoinMessageHandler * theHandler;

   theFile = openFile (myOptComp ()->solverLogFileName ().myCstring (), "w");

   enterCoin ();

   theHandler = new CoinMessageHandler (theFile);

   myClpSimplex_->passInMessageHandler (theHandler);

   leaveCoin ();
   }

//------------------------------------------------------------------------------

void WitCoinIf::shutDownMessageHandler ()
   {
   CoinMessageHandler * theHandler;
   FILE *               theFile;

   enterCoin ();

   theHandler = myClpSimplex_->messageHandler ();

   myClpSimplex_->setDefaultMessageHandler ();

   theFile = theHandler->filePointer ();

   delete theHandler;

   leaveCoin ();

   fclose (theFile);
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

//------------------------------------------------------------------------------

void WitCoinIf::checkStatusCode (int statusCode)
   {
   switch (statusCode)
      {
      case 0:
         {
         myMsgFac () ("optSolnFoundMsg");

         return;
         }

      case 1:
         myMsgFac () ("infeasSmsg");

      case 2:
         myMsgFac () ("unboundedOrInfeasSmsg");

      case 3:
         myMsgFac () ("iterOrTimeLimitSmsg");

      case 4:
         myMsgFac () ("solverStoppedErrorsSmsg");

      case 5:
         myMsgFac () ("clpStoppedEventSmsg");

      default:
         myMsgFac () ("unexpClpStatusCodeSmsg", statusCode);
      }
   }

//------------------------------------------------------------------------------

void WitCoinIf::enterCoin ()
   {
   WitTimer::enterSection ("coin");
   }

//------------------------------------------------------------------------------

void WitCoinIf::leaveCoin ()
   {
   WitTimer::leaveSection ("coin");
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

WitCoinIf * WitCoinIf::newInstance (WitOptProblem *)
   {
   stronglyAssert (false);

   return NULL;
   }

#endif // Not COIN_EMBEDDED
