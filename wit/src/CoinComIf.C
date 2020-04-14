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
#include <OptVar.h>
#include <MsgFac.h>
#include <Timing.h>

//------------------------------------------------------------------------------
// COIN-embedded Implementation of class CoinComIf.
//------------------------------------------------------------------------------

#ifdef COIN_EMBEDDED

#include <OsiSolverInterface.hpp>

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

   myOsiSI ()->setObjSense (-1.0);

   myOsiSI ()->
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
   try
      {
      enterCoin ();

      myOsiSI ()->writeMps ("opt-prob", "mps", -1.0);

      leaveCoin ();
      }

   catch (...)
      {
      myMsgFac () ("osiWriteMpsExcSmsg", "opt-prob.mps");
      }
   }

//------------------------------------------------------------------------------

void WitCoinComIf::setVarLB (WitOptVar * theOptVar, double theLB)
   {
   myOsiSI ()->setColLower (theOptVar->index (), theLB);
   }

//------------------------------------------------------------------------------

void WitCoinComIf::setObjCoeff (WitOptVar * theOptVar, double theVal)
   {
   myOsiSI ()->setObjCoeff (theOptVar->index (), theVal);
   }

//------------------------------------------------------------------------------

double WitCoinComIf::primalVarVal (WitOptVar * theOptVar)
   {
   return myOsiSI ()->getColSolution()[theOptVar->index ()];
   }

//------------------------------------------------------------------------------

void WitCoinComIf::getPrimalSoln (WitVector <double> & primalSoln)
   {
   primalSoln = myOsiSI ()->getColSolution ();
   }

//------------------------------------------------------------------------------

WitCoinComIf::WitCoinComIf (WitOptProblem * theOptProblem):

      WitSolverIf   (theOptProblem),
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
