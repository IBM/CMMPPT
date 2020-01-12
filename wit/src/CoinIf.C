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

//------------------------------------------------------------------------------
// All #include statements for WIT header files must be located in the
// unconditional part of the source file, so that make depend will work
// correctly.
//------------------------------------------------------------------------------

#include <CoinIf.h>
#include <OptSolveMgr.h>
#include <OptComp.h>
#include <OptProblem.h>
#include <OptCon.h>
#include <OptVar.h>
#include <OptStarter.h>
#include <MsgFrag.h>
#include <MsgFac.h>
#include <Timing.h>

//------------------------------------------------------------------------------
// COIN-embedded Implementation of class CoinIf.
//------------------------------------------------------------------------------

#ifdef COIN_EMBEDDED

#include <ClpSimplex.hpp>
#include <ClpPresolve.hpp>

//------------------------------------------------------------------------------

bool WitCoinIf::coinEmbedded ()
   {
   return true;
   }

//------------------------------------------------------------------------------

WitCoinIf * WitCoinIf::newInstance (WitOptSolveMgr * theOptSolveMgr)
   {
   return new WitCoinIf (theOptSolveMgr);
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

void WitCoinIf::issueVersionMsg ()
   {
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

void WitCoinIf::reviseLp ()
   {
   reviseVarBounds ();
   reviseConBounds ();
   reviseObjCoeffs ();
   }

//------------------------------------------------------------------------------

void WitCoinIf::solverWriteMps ()
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

void WitCoinIf::loadInitSoln (const WitVector <double> & initSoln)
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

void WitCoinIf::reSolveLp ()
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

void WitCoinIf::solveLp (bool)
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

const char * WitCoinIf::solverName ()
   {
   return myOptComp ()->mipMode ()? "CBC": "CLP";
   }

//------------------------------------------------------------------------------

WitCoinIf::WitCoinIf (WitOptSolveMgr * theOptSolveMgr):

      WitSolverIf   (theOptSolveMgr),
      myClpSimplex_ (NULL)
   {
   enterCoin ();

   myClpSimplex_ = new ClpSimplex;

   leaveCoin ();

   setUpMessageHandler ();
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

void WitCoinIf::reviseVarBounds ()
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

void WitCoinIf::reviseConBounds ()
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

void WitCoinIf::reviseObjCoeffs ()
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

WitCoinIf * WitCoinIf::newInstance (WitOptSolveMgr *)
   {
   stronglyAssert (false);

   return NULL;
   }

#endif // Not COIN_EMBEDDED
