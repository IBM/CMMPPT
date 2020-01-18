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
#include <MsgFac.h>
#include <Timing.h>

//------------------------------------------------------------------------------
// COIN-embedded Implementation of class CoinIf.
//------------------------------------------------------------------------------

#ifdef COIN_EMBEDDED

#include <ClpSimplex.hpp>
#include <ClpPresolve.hpp>
#include <OsiClpSolverInterface.hpp>
#include <CbcModel.hpp>

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

   delete myCbcModel_;
   delete myClpSimplex_;

   leaveCoin ();
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

   myClpSimplex ()->setOptimizationDirection (-1.0);

   myClpSimplex ()->
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

void WitCoinIf::loadIntData ()
   {
   WitOptVar * theOptVar;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      {
      if (theOptVar->isAnIntVar ())
         {
         myClpSimplex ()->setInteger (theOptVar->index ());
         }
      }
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

      errCode = myClpSimplex ()->writeMps ("opt-prob.mps", 0, 1, -1.0);

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

   myClpSimplex ()->setColSolution (initSoln.myCVec ());

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

   myClpSimplex ()->setWhatsChanged (MATRIX_SAME + BASIS_SAME);

   myClpSimplex ()->dual (ifValuespass, startFinishOptions);

   statusCode = myClpSimplex ()->problemStatus    ();
   nIters     = myClpSimplex ()->numberIterations ();

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

   psClpSimplex = theClpPresolve->presolvedModel (* myClpSimplex ());

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

   if (not myClpSimplex ()->isProvenOptimal ())
      {
      myClpSimplex ()->primal (1, startFinishOptions);

      nIters += myClpSimplex ()->numberIterations ();
      }

   leaveCoin ();

   myMsgFac () ("nSimplexItersMsg", nIters);
   }

//------------------------------------------------------------------------------

void WitCoinIf::solveMip (bool)
   {
   myCbcModel_->branchAndBound ();

   myMsgFac () ("coinNYISmsg", "Optimizing Implosion in MIP mode (3)");
   }

//------------------------------------------------------------------------------

void WitCoinIf::getPrimalSoln (WitVector <double> & primalSoln)
   {
   primalSoln = myClpSimplex ()->getColSolution ();
   }

//------------------------------------------------------------------------------

void WitCoinIf::getDualSoln (WitVector <double> & dualSoln)
   {
   dualSoln = myClpSimplex ()->getRowPrice ();
   }

//------------------------------------------------------------------------------

const char * WitCoinIf::solverName ()
   {
   return myOptComp ()->mipMode ()? "CBC": "CLP";
   }

//------------------------------------------------------------------------------

WitCoinIf::WitCoinIf (WitOptSolveMgr * theOptSolveMgr):

      WitSolverIf   (theOptSolveMgr),
      myClpSimplex_ (NULL),
      myCbcModel_   (NULL)
   {
   enterCoin ();

   if (myOptComp ()->mipMode ())
      {
      myCbcModel_   = newClpBasedCbcModel ();
      }
   else
      {
      myClpSimplex_ = new ClpSimplex;
      }

   leaveCoin ();

   setUpMessageHandler ();
   }

//------------------------------------------------------------------------------

CbcModel * WitCoinIf::newClpBasedCbcModel ()
   {
   OsiClpSolverInterface * theOsiClpSI;
   CbcModel *              theCbcModel;

   theOsiClpSI = new OsiClpSolverInterface;

   theCbcModel = new CbcModel (* theOsiClpSI);

   delete theOsiClpSI;

   return theCbcModel;
   }

//------------------------------------------------------------------------------

void WitCoinIf::setUpMessageHandler ()
   {
   FILE *               theFile;
   CoinMessageHandler * theHandler;

   theFile = openFile (myOptComp ()->solverLogFileName ().myCstring (), "w");

   enterCoin ();

   theHandler = new CoinMessageHandler (theFile);

   myClpSimplex ()->passInMessageHandler (theHandler);

   leaveCoin ();
   }

//------------------------------------------------------------------------------

void WitCoinIf::shutDownMessageHandler ()
   {
   CoinMessageHandler * theHandler;
   FILE *               theFile;

   enterCoin ();

   theHandler = myClpSimplex ()->messageHandler ();

   myClpSimplex ()->setDefaultMessageHandler ();

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

   lowerBound = myClpSimplex ()->columnLower ();
   upperBound = myClpSimplex ()->columnUpper ();

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

   lowerBound = myClpSimplex ()->rowLower ();
   upperBound = myClpSimplex ()->rowUpper ();

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
      myClpSimplex ()->
         setObjectiveCoefficient (
            theOptVar->index (),
            theOptVar->objCoeff ());
      }
   }

//------------------------------------------------------------------------------

void WitCoinIf::checkMipSolnStatus (bool optNeeded)
   {
   myMsgFac () ("coinNYISmsg", "Optimizing Implosion in MIP mode (4)");
   }

//------------------------------------------------------------------------------

ClpSimplex * WitCoinIf::myClpSimplex ()
   {
   return
      myOptComp ()->mipMode ()?
         getClpSimplexFromCbcModel ():
         myClpSimplex_;
   }

//------------------------------------------------------------------------------

ClpSimplex * WitCoinIf::getClpSimplexFromCbcModel ()
   {
   OsiClpSolverInterface * theOsiClpSI;

   theOsiClpSI =
      dynamic_cast <OsiClpSolverInterface * > (myCbcModel_->solver ());

   return theOsiClpSI->getModelPtr ();
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
