//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "CoinLpIf.C"
//
// Contains the implementation of class CoinLpIf.
// The implementation is compiled only if COIN_EMBEDDED is defined.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// All #include statements for WIT header files must be located in the
// unconditional part of the source file, so that make depend will work
// correctly.
//------------------------------------------------------------------------------

#include <CoinLpIf.h>
#include <OptComp.h>
#include <OptProblem.h>
#include <OptVar.h>
#include <OptCon.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// COIN-embedded Implementation of class CoinLpIf.
//------------------------------------------------------------------------------

#ifdef COIN_EMBEDDED

#include <OsiClpSolverInterface.hpp>

//------------------------------------------------------------------------------

WitCoinLpIf::WitCoinLpIf (WitOptProblem * theOptProblem):

      WitCoinIf (theOptProblem),
      myOsiSI_  (NULL)
   {
   OsiClpSolverInterface * theOsiClpSI;

   enterCoin ();

   myOsiSI_ = new OsiClpSolverInterface;

   myOsiSI_->passInMessageHandler (myMsgHandler ());

   leaveCoin ();
   }

//------------------------------------------------------------------------------

WitCoinLpIf::~WitCoinLpIf ()
   {
   enterCoin ();

   delete myOsiSI_;

   leaveCoin ();
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::reviseLp ()
   {
   reviseVarBounds ();
   reviseConBounds ();
   reviseObjCoeffs ();
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::loadInitSoln (const WitVector <double> & initSoln)
   {
   enterCoin ();

   myOsiSI_->setColSolution (initSoln.myCVec ());

   leaveCoin ();
   }

//------------------------------------------------------------------------------
// solveLp
//
// Note that when the primal simplex method is used, the input primal solution
// is used as the starting solution. (A values pass is performed.)
//------------------------------------------------------------------------------

void WitCoinLpIf::solveLp (bool reqPrimalSimplex)
   {
   int nIters;

   enterCoin ();

   myOsiSI ()->setHintParam (OsiDoPresolveInInitial, true, OsiHintDo);
   myOsiSI ()->setHintParam (OsiDoScale,             true, OsiHintDo);

   if (reqPrimalSimplex)
      myOsiSI ()->setHintParam (OsiDoDualInInitial, false, OsiHintDo);
   else
      myOsiSI ()->setHintParam (OsiDoDualInInitial, false, OsiHintIgnore);

   myOsiSI ()->initialSolve ();

   nIters = myOsiSI ()->getIterationCount ();

   leaveCoin ();

   checkLpSolnStatup ();

   myMsgFac () ("nSimplexItersMsg", nIters);
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::reSolveLp ()
   {
   int nIters;

   enterCoin ();

   myOsiSI ()->setHintParam (OsiDoPresolveInResolve, true, OsiHintDo);
   myOsiSI ()->setHintParam (OsiDoScale,             true, OsiHintDo);

   myOsiSI ()->resolve ();

   nIters = myOsiSI ()->getIterationCount ();

   leaveCoin ();

   checkLpSolnStatup ();

   myMsgFac () ("nSimplexItersMsg", nIters);
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::reSolveLexLp ()
   {
   myOsiSI ()->setHintParam (OsiDoDualInResolve, false, OsiHintDo);

   reSolveLp ();
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::getDualSoln (WitVector <double> & dualSoln)
   {
   dualSoln = myOsiSI_->getRowPrice ();
   }

//------------------------------------------------------------------------------

const char * WitCoinLpIf::solverName ()
   {
   return "CLP";
   }

//------------------------------------------------------------------------------

OsiSolverInterface * WitCoinLpIf::myOsiSI ()
   {
   return myOsiSI_;
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::reviseVarBounds ()
   {
   WitOptVar * theOptVar;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      {
      myOsiSI_->
         setColLower (
            theOptVar->index (),
            theOptVar->bounds ().lower ());

      myOsiSI_->
         setColUpper (
            theOptVar->index (),
            theOptVar->bounds ().upper ());
      }
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::reviseConBounds ()
   {
   WitOptCon * theOptCon;

   forEachEl (theOptCon, myOptProblem ()->myOptCons ())
      {
      myOsiSI_->
         setRowLower (
            theOptCon->index (),
            theOptCon->bounds ().lower ());

      myOsiSI_->
         setRowUpper (
            theOptCon->index (),
            theOptCon->bounds ().upper ());
      }
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::reviseObjCoeffs ()
   {
   WitOptVar * theOptVar;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      {
      myOsiSI_->
         setObjCoeff (
            theOptVar->index (),
            theOptVar->objCoeff ());
      }
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::checkLpSolnStatup ()
   {
   enterCoin ();

   if      (myOsiSI ()->isProvenOptimal ())
      myMsgFac () ("optSolnFoundMsg");

   else if (myOsiSI ()->isProvenPrimalInfeasible ())
      myMsgFac () ("infeasSmsg");

   else if (myOsiSI ()->isProvenDualInfeasible ())
      myMsgFac () ("unboundedOrInfeasSmsg");

   else if (myOsiSI ()->isIterationLimitReached ())
      myMsgFac () ("iterLimitSmsg");

   else
      myMsgFac () ("termUnknownReasonSmsg");

   leaveCoin ();
   }

#endif // COIN_EMBEDDED
