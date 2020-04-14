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
#include <ClpSimplex.hpp>
#include <ClpPresolve.hpp>

//------------------------------------------------------------------------------

WitCoinLpIf * WitCoinLpIf::newInstance (WitOptProblem * theOptProblem)
   {
   return new WitCoinLpIf (theOptProblem);
   }

//------------------------------------------------------------------------------

WitCoinLpIf::~WitCoinLpIf ()
   {
   enterCoin ();

   delete myOsiSI_;
   delete myClpSimplex_;

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

   nIters = myClpSimplex_->numberIterations ();

   leaveCoin ();

   checkLpSolnStatus (myClpSimplex_);

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
   int           nIters;

   ifValuesPass       = useDualSimplex ()? 0: 1;

   startFinishOptions = myOptComp ()->accAfterOptImp ()? 1: 0;

   enterCoin ();

   theClpPresolve = new ClpPresolve;

   psClpSimplex = theClpPresolve->presolvedModel (* myClpSimplex_);

   if (psClpSimplex == NULL)
      myMsgFac () ("unboundedOrInfeasSmsg");

   if (useDualSimplex ())
      psClpSimplex->dual   (ifValuesPass, startFinishOptions);
   else
      psClpSimplex->primal (ifValuesPass, startFinishOptions);

   nIters = psClpSimplex->numberIterations ();

   leaveCoin ();

   checkLpSolnStatus (psClpSimplex);

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

WitCoinLpIf::WitCoinLpIf (WitOptProblem * theOptProblem):

      WitCoinComIf  (theOptProblem),
      myClpSimplex_ (NULL),
      myOsiSI_      (NULL)
   {
   OsiClpSolverInterface * theOsiClpSI;

   enterCoin ();

   myClpSimplex_ = new ClpSimplex;

   myOsiSI_      = new OsiClpSolverInterface (myClpSimplex_);

   myClpSimplex_->passInMessageHandler (myMsgHandler ());

   leaveCoin ();
   }

//------------------------------------------------------------------------------

OsiSolverInterface * WitCoinLpIf::myOsiSI ()
   {
   return myOsiSI_;
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

//------------------------------------------------------------------------------

void WitCoinLpIf::checkLpSolnStatus (ClpSimplex * theClpSimplex)
   {
   int statusCode;

   statusCode = theClpSimplex->problemStatus ();

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

#endif // COIN_EMBEDDED

//------------------------------------------------------------------------------
// Non-COIN-embedded Implementation of class CoinLpIf.
//------------------------------------------------------------------------------

#ifndef COIN_EMBEDDED

WitCoinLpIf * WitCoinLpIf::newInstance (WitOptProblem *)
   {
   stronglyAssert (false);

   return NULL;
   }

#endif // Not COIN_EMBEDDED
