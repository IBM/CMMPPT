//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "CoinIf.C"
//
// Contains the implementation of class CoinIf.
// If COIN_EMBEDDED is not defined, then only static functions are implemented.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// All #include statements for WIT header files must be located in the
// unconditional part of the source file, so that make depend will work
// correctly.
//------------------------------------------------------------------------------

#include <CoinLpIf.h>
#include <CoinMipIf.h>
#include <OptComp.h>
#include <OptProblem.h>
#include <OptVar.h>
#include <MsgFac.h>
#include <Timing.h>

//------------------------------------------------------------------------------
// COIN-embedded Implementation of class CoinIf.
//------------------------------------------------------------------------------

#ifdef COIN_EMBEDDED

#include <OsiSolverInterface.hpp>

//------------------------------------------------------------------------------

bool WitCoinIf::coinEmbedded ()
   {
   return true;
   }

//------------------------------------------------------------------------------

WitCoinIf * WitCoinIf::newInstance (WitOptProblem * theOptProblem)
   {
   if (theOptProblem->myOptComp ()->mipMode ())
      return new WitCoinMipIf (theOptProblem);
   else
      return new WitCoinLpIf  (theOptProblem);
   }

//------------------------------------------------------------------------------

WitCoinIf::~WitCoinIf ()
   {
   shutDownMessageHandler ();
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

void WitCoinIf::solverWriteMps ()
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

void WitCoinIf::setVarLB (WitOptVar * theOptVar, double theLB)
   {
   myOsiSI ()->setColLower (theOptVar->index (), theLB);
   }

//------------------------------------------------------------------------------

void WitCoinIf::setObjCoeff (WitOptVar * theOptVar, double theVal)
   {
   myOsiSI ()->setObjCoeff (theOptVar->index (), theVal);
   }

//------------------------------------------------------------------------------

double WitCoinIf::primalVarVal (WitOptVar * theOptVar)
   {
   return myOsiSI ()->getColSolution()[theOptVar->index ()];
   }

//------------------------------------------------------------------------------

void WitCoinIf::getPrimalSoln (WitVector <double> & primalSoln)
   {
   primalSoln = myOsiSI ()->getColSolution ();
   }

//------------------------------------------------------------------------------

WitCoinIf::WitCoinIf (WitOptProblem * theOptProblem):

      WitSolverIf   (theOptProblem),
      myMsgHandler_ (NULL)
   {
   setUpMessageHandler ();
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

//------------------------------------------------------------------------------

void WitCoinIf::setUpMessageHandler ()
   {
   FILE * theFile;

   theFile = openFile (myOptComp ()->solverLogFileName ().myCstring (), "w");

   enterCoin ();

   myMsgHandler_ = new CoinMessageHandler (theFile);

   leaveCoin ();
   }

//------------------------------------------------------------------------------

void WitCoinIf::shutDownMessageHandler ()
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
