//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "CoinLpIf.C"
//
// Contains the implementation of class CoinLpIf.
// If COIN_EMBEDDED is not defined, then only static functions are implemented.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// All #include statements for WIT header files must be located in the
// unconditional part of the source file, so that make depend will work
// correctly.
//------------------------------------------------------------------------------

#include <CoinLpIf.h>
#include <OptSolveMgr.h>
#include <OptComp.h>
#include <OptProblem.h>
#include <OptVar.h>
#include <OptCon.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// COIN-embedded Implementation of class CoinLpIf.
//------------------------------------------------------------------------------

#ifdef COIN_EMBEDDED

#include <ClpSimplex.hpp>
#include <ClpPresolve.hpp>

//------------------------------------------------------------------------------

WitCoinLpIf * WitCoinLpIf::newInstance (WitOptSolveMgr * theOptSolveMgr)
   {
   return new WitCoinLpIf (theOptSolveMgr);
   }

//------------------------------------------------------------------------------

WitCoinLpIf::~WitCoinLpIf ()
   {
   shutDownMessageHandler ();

   enterCoin ();

   delete myClpSimplex_;

   leaveCoin ();
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::loadIntData ()
   {
   stronglyAssert (false);
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

   myClpSimplex_->setColSolution (initSoln.myCVec ());

   leaveCoin ();
   }

//------------------------------------------------------------------------------
// reSolveLp
//
// Note that the existing basis may be primal feasible, dual feasible, both, or
// neither. Dual simplex is being used for this general case.
//------------------------------------------------------------------------------

void WitCoinLpIf::reSolveLp ()
   {
   int ifValuespass;
   int startFinishOptions;
   int statusCode;
   int nIters;

   ifValuespass = 0;
      //
      // 0 means don't do a values pass.

   startFinishOptions = 1 + 2 + 4;
      //
      // Interpreted as bits:
      //    1 - do not delete work areas and factorization at end
      //    2 - use old factorization if same number of rows
      //    4 - skip as much initialization of work areas as possible
      //        (based on whatsChanged in clpmodel.hpp)

   enterCoin ();

   myClpSimplex_->setWhatsChanged (MATRIX_SAME + BASIS_SAME);

   myClpSimplex_->dual (ifValuespass, startFinishOptions);

   statusCode = myClpSimplex_->problemStatus    ();
   nIters     = myClpSimplex_->numberIterations ();

   leaveCoin ();

   checkStatusCode (statusCode);

   myMsgFac () ("nSimplexItersMsg", nIters);
   }

//------------------------------------------------------------------------------
// solveLp
//
// ifValuesPass == 0, means don't do a values pass.
// ifValuesPass == 1, means       do a values pass.
//
// startFinishOptions == 1 means retain work areas and factorization at end.
// startFinishOptions == 0 means delete work areas and factorization at end.
//------------------------------------------------------------------------------

void WitCoinLpIf::solveLp (bool)
   {
   int           ifValuesPass;
   int           startFinishOptions;
   ClpPresolve * theClpPresolve;
   ClpSimplex *  psClpSimplex;
   int           statusCode;
   int           nIters;

   ifValuesPass       = myOptSolveMgr ()->useDualSimplex ()? 0: 1;

   startFinishOptions = myOptComp ()->accAfterOptImp ()? 1: 0;

   enterCoin ();

   theClpPresolve = new ClpPresolve;

   psClpSimplex = theClpPresolve->presolvedModel (* myClpSimplex_);

   if (psClpSimplex == NULL)
      myMsgFac () ("unboundedOrInfeasSmsg");

   if (myOptSolveMgr ()->useDualSimplex ())
      psClpSimplex->dual   (ifValuesPass, startFinishOptions);
   else
      psClpSimplex->primal (ifValuesPass, startFinishOptions);

   statusCode = psClpSimplex->problemStatus    ();
   nIters     = psClpSimplex->numberIterations ();

   leaveCoin ();

   checkStatusCode (statusCode);

   enterCoin ();

   theClpPresolve->postsolve (true);

   delete theClpPresolve;

   delete psClpSimplex;

   if (not myClpSimplex_->isProvenOptimal ())
      {
      myClpSimplex_->primal (1, startFinishOptions);

      nIters += myClpSimplex_->numberIterations ();
      }

   leaveCoin ();

   myMsgFac () ("nSimplexItersMsg", nIters);
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::solveMip (bool)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::getDualSoln (WitVector <double> & dualSoln)
   {
   dualSoln = myClpSimplex_->getRowPrice ();
   }

//------------------------------------------------------------------------------

const char * WitCoinLpIf::solverName ()
   {
   return "CLP";
   }

//------------------------------------------------------------------------------

WitCoinLpIf::WitCoinLpIf (WitOptSolveMgr * theOptSolveMgr):

      WitCoinComIf  (theOptSolveMgr),
      myClpSimplex_ (NULL)
   {
   enterCoin ();

   myClpSimplex_ = new ClpSimplex;

   leaveCoin ();

   setUpMessageHandler ();
   }

//------------------------------------------------------------------------------

ClpModel * WitCoinLpIf::myClpModel ()
   {
   return myClpSimplex_;
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::reviseVarBounds ()
   {
   double *    lowerBound;
   double *    upperBound;
   WitOptVar * theOptVar;

   lowerBound = myClpSimplex_->columnLower ();
   upperBound = myClpSimplex_->columnUpper ();

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      {
      lowerBound[theOptVar->index ()] = theOptVar->bounds ().lower ();
      upperBound[theOptVar->index ()] = theOptVar->bounds ().upper ();
      }
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::reviseConBounds ()
   {
   double *    lowerBound;
   double *    upperBound;
   WitOptCon * theOptCon;

   lowerBound = myClpSimplex_->rowLower ();
   upperBound = myClpSimplex_->rowUpper ();

   forEachEl (theOptCon, myOptProblem ()->myOptCons ())
      {
      lowerBound[theOptCon->index ()] = theOptCon->bounds ().lower ();
      upperBound[theOptCon->index ()] = theOptCon->bounds ().upper ();
      }
   }

//------------------------------------------------------------------------------

void WitCoinLpIf::reviseObjCoeffs ()
   {
   WitOptVar * theOptVar;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      {
      myClpSimplex_->
         setObjectiveCoefficient (
            theOptVar->index (),
            theOptVar->objCoeff ());
      }
   }

#endif // COIN_EMBEDDED

//------------------------------------------------------------------------------
// Non-COIN-embedded Implementation of class CoinLpIf.
//------------------------------------------------------------------------------

#ifndef COIN_EMBEDDED

WitCoinLpIf * WitCoinLpIf::newInstance (WitOptSolveMgr *)
   {
   stronglyAssert (false);

   return NULL;
   }

#endif // Not COIN_EMBEDDED
