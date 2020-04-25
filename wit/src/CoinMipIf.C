//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "CoinMipIf.C"
//
// Contains the implementation of class CoinMipIf.
// The implementation is compiled only if COIN_EMBEDDED is defined.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// All #include statements for WIT header files must be located in the
// unconditional part of the source file, so that make depend will work
// correctly.
//------------------------------------------------------------------------------

#include <CoinMipIf.h>
#include <OptProblem.h>
#include <OptVar.h>
#include <OptComp.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// COIN-embedded Implementation of class CoinMipIf.
//------------------------------------------------------------------------------

#ifdef COIN_EMBEDDED

#include <OsiClpSolverInterface.hpp>
#include <CbcModel.hpp>

//------------------------------------------------------------------------------

WitCoinMipIf::WitCoinMipIf (WitOptProblem * theOptProblem):

      WitCoinIf   (theOptProblem),
      myCbcModel_ (NULL)
   {
   OsiClpSolverInterface * theOsiClpSI;

   enterCoin ();

   theOsiClpSI = new OsiClpSolverInterface;

   myCbcModel_ = new CbcModel (* theOsiClpSI);

   delete theOsiClpSI;

   myCbcModel_->passInMessageHandler (myMsgHandler ());

   leaveCoin ();
   }

//------------------------------------------------------------------------------

WitCoinMipIf::~WitCoinMipIf ()
   {
   enterCoin ();

   delete myCbcModel_;

   leaveCoin ();
   }

//------------------------------------------------------------------------------

bool WitCoinMipIf::lexOptNeedsReload ()
   {
   return true;
   }

//------------------------------------------------------------------------------

void WitCoinMipIf::loadIntData ()
   {
   WitOptVar * theOptVar;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      if (theOptVar->isAnIntVar ())
         {
         myCbcModel_->solver ()->setInteger (theOptVar->index ());
         }
   }

//------------------------------------------------------------------------------

void WitCoinMipIf::solveMip (bool)
   {
   enterCoin ();

   myCbcModel_->branchAndBound ();

   leaveCoin ();

   checkMipSolnStatus ();

   printMipSolveInfo ();
   }

//------------------------------------------------------------------------------

const char * WitCoinMipIf::solverName ()
   {
   return "CBC";
   }

//------------------------------------------------------------------------------

void WitCoinMipIf::checkMipSolnStatus ()
   {
   if (myCbcModel_->isProvenOptimal ())
      myMsgFac () ("optSolnFoundMsg");

   else if (myCbcModel_->isAbandoned ())
      myMsgFac () ("optProbAbandonedSmsg");

   else if (myCbcModel_->isProvenInfeasible ())
      myMsgFac () ("infeasSmsg");

   else if (myCbcModel_->isContinuousUnbounded ())
      myMsgFac () ("unboundedSmsg");

   else if (myCbcModel_->isNodeLimitReached ())
      myMsgFac () ("nodeLimitSmsg");

   else if (myCbcModel_->isSecondsLimitReached ())
      myMsgFac () ("iterOrTimeLimitSmsg");

   else if (myCbcModel_->isSolutionLimitReached ())
      myMsgFac () ("solutionLimitSmsg");

   else
      myMsgFac () ("noOptReasonUnknownSmsg");
   }

//------------------------------------------------------------------------------

void WitCoinMipIf::printMipSolveInfo ()
   {
   int nItns;
   int nNodes;

   nItns  = myCbcModel_->getIterationCount ();
   nNodes = myCbcModel_->getNodeCount ();

   myMsgFac () ("CbcSolveInfoMsg", nItns, nNodes);
   }

//------------------------------------------------------------------------------

OsiSolverInterface * WitCoinMipIf::myOsiSI ()
   {
   return myCbcModel_->solver ();
   }

#endif // COIN_EMBEDDED
