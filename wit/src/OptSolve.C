//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "OptSolve.C"
//
// Contains the implementation of the following classes:
//    OptSolveGate
//    OptSolveMgr
//    SolverIf
//------------------------------------------------------------------------------

#include <OptSolveGate.h>
#include <OptSolveMgr.h>
#include <CoinLpIf.h>
#include <CoinMipIf.h>
#include <CplexIf.h>
#include <OptProblem.h>
#include <SaeMgr.h>
#include <OptVar.h>
#include <OptCon.h>
#include <OptComp.h>
#include <OptStarter.h>
#include <MultiObjMgr.h>
#include <MsgFrag.h>
#include <MsgFac.h>
#include <Timing.h>

//------------------------------------------------------------------------------
// Implementation of class OptSolveGate
//------------------------------------------------------------------------------

bool WitOptSolveGate::coinEmbedded ()
   {
   return WitCoinComIf::coinEmbedded ();
   }

//------------------------------------------------------------------------------

bool WitOptSolveGate::cplexEmbedded ()
   {
   return WitCplexIf::cplexEmbedded ();
   }

//------------------------------------------------------------------------------

WitOptSolveGate::WitOptSolveGate (WitOptProblem * theOptProblem):

      myOptSolveMgr_ (new WitOptSolveMgr (theOptProblem))
   {
   }

//------------------------------------------------------------------------------

WitOptSolveGate::~WitOptSolveGate ()
   {
   delete myOptSolveMgr_;
   }

//------------------------------------------------------------------------------

void WitOptSolveGate::solveOptProb ()
   {
   myOptSolveMgr_->solveOptProb ();
   }

//------------------------------------------------------------------------------
// Implementation of class OptSolveMgr
//------------------------------------------------------------------------------

void WitOptSolveMgr::solveOptProb ()
   {
   if      (myOptComp ()->multiObjMode ())
      {
      solveOptProbAsLexOpt ();
      }
   else if (myOptComp ()->accOptStarter ()->isChosen ())
      {
      reSolveOptProbAsLp ();
      }
   else if (myOptComp ()->mipMode ())
      {
      solveOptProbAsMip ();
      }
   else
      {
      solveOptProbAsLp ();
      }
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::setUseDualSimplex (bool theValue)
   {
   useDualSimplex_ = theValue;
   }

//------------------------------------------------------------------------------

WitOptSolveMgr::WitOptSolveMgr (WitOptProblem * theOptProblem):

      WitProbAssoc      (theOptProblem),
      myOptProblem_     (theOptProblem),
      mySolverIf_       (NULL),
      useDualSimplex_   (true),
      optLexObjElemVal_ ()
   {
   bool useCoin;

   if (WitCoinComIf::coinEmbedded () and WitCplexIf::cplexEmbedded ())
      {
      useCoin = myOptComp ()->preferCoin ();
      }
   else if (WitCoinComIf::coinEmbedded ())
      {
      useCoin = true;
      }
   else if (WitCplexIf::cplexEmbedded ())
      {
      useCoin = false;
      }
   else
      {
      stronglyAssert (false);
      }

   if (useCoin)
      {
      if (myOptComp ()->mipMode ())
         mySolverIf_ = WitCoinMipIf::newInstance (this);
      else
         mySolverIf_ =  WitCoinLpIf::newInstance (this);
      }
   else
      mySolverIf_    =   WitCplexIf::newInstance (this);
   }

//------------------------------------------------------------------------------

WitOptSolveMgr::~WitOptSolveMgr ()
   {
   delete mySolverIf_;
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::solveOptProbAsLexOpt ()
   {
   issueSolveMsg ();

   myMsgFac () ("lexOptMsg");

   mySolverIf_->loadLp ();

   if (myOptComp ()->mipMode ())
      mySolverIf_->loadIntData ();

   writeMps ();

   solveLexOpt ();

   storePrimalSoln ();
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::solveOptProbAsLp ()
   {
   issueSolveMsg ();

   mySolverIf_->loadLp ();

   writeMps ();

   useDualSimplex_ = myOptComp ()->crashOptStarter ()->isChosen ();

   loadInitSoln ();

   mySolverIf_->solveLp (myOptProblem_->needDual ());

   storePrimalSoln ();

   if (myOptProblem_->needDual ())
      storeDualSoln ();
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::reSolveOptProbAsLp ()
   {
   myMsgFac () ("reSolveLpMsg", mySolverIf_->solverName ());

   mySolverIf_->reviseLp ();

   writeMps ();

   mySolverIf_->reSolveLp ();

   storePrimalSoln ();

   if (myOptProblem ()->needDual ())
      storeDualSoln ();
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::solveOptProbAsMip ()
   {
   issueSolveMsg ();

   if (not optProbHasIntVars ())
      myMsgFac () ("mipModeNoIntVarsSmsg");

   mySolverIf_->loadLp ();

   mySolverIf_->loadIntData ();

   writeMps ();

   mySolverIf_->solveMip (false);

   storePrimalSoln ();
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::issueSolveMsg ()
   {
   myMsgFac () ("solveOptProblemMsg",
      myMsgFac ().myFrag (myOptComp ()->mipMode ()? "mipFrag": "lpFrag"),
      mySolverIf_->solverName ());

   mySolverIf_->issueVersionMsg ();
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::writeMps ()
   {
   if (not myOptComp ()->printMps ())
      return;

   myMsgFac () ("mpsFileMsg");

   mySolverIf_->solverWriteMps ();
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::solveLexOpt ()
   {
   WitOptVar *              prevOptVar;
   WitPtrVecItr <WitOptVar> theOptVarItr;
   WitOptVar *              theOptVar;

   if (devMode ())
      WitTimer::getTimeAndChargeToCurrent ();

   if (mySolverIf_->lexOptReloadNeeded ())
      setUpLexOptReload ();

   if (not myOptComp ()->mipMode ())
      loadInitSoln ();

   prevOptVar = NULL;

   myOptProblem ()->myLexOptVarSeq ().attachItr (theOptVarItr);

   while (theOptVarItr.advance (theOptVar))
      {
      prepLexObjElemOpt (prevOptVar, theOptVar);

      solveCurrentObj (prevOptVar == NULL);

      if (mySolverIf_->lexOptReloadNeeded ())
         storeOptLexObjElemVal (theOptVarItr);

      if (devMode ())
         if (WitSaeMgr::standAloneMode ())
            myMsgFac () ("lexObjElemCpuTimeMsg",
               WitTimer::getTimeAndChargeToCurrent ());

      prevOptVar = theOptVar;
      }
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::setUpLexOptReload ()
   {
   optLexObjElemVal_.resize (myOptProblem ()->myLexOptVarSeq ().length (), 0.0);
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::prepLexObjElemOpt (
      WitOptVar * prevOptVar,
      WitOptVar * theOptVar)
   {
   double prevObjVal;

   myMsgFac () ("optLexObjElemMsg", theOptVar->lexObjElemName ());

   if (prevOptVar != NULL)
      {
      if (mySolverIf_->lexOptReloadNeeded ())
         {
         lexReloadAndBound (theOptVar);
         }
      else
         {
         prevObjVal = mySolverIf_->primalVarVal (prevOptVar);

         boundLexObjElemVal (prevOptVar, prevObjVal);

         mySolverIf_->setObjCoeff (prevOptVar, 0.0);
         }
      }

   mySolverIf_->setObjCoeff (theOptVar, 1.0);
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::lexReloadAndBound (WitOptVar * theOptVar)
   {
   WitPtrVecItr <WitOptVar> innerOptVarItr;
   WitOptVar *              innerOptVar;
   double                   theObjVal;

   mySolverIf_->loadLp ();

   mySolverIf_->loadIntData ();

   myOptProblem ()->myLexOptVarSeq ().attachItr (innerOptVarItr);

   while (innerOptVarItr.advance (innerOptVar))
      {
      if (innerOptVar == theOptVar)
         break;

      theObjVal = optLexObjElemVal_[innerOptVarItr.myIdx ()];

      boundLexObjElemVal (innerOptVar, theObjVal);
      }
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::storeOptLexObjElemVal (
      WitPtrVecItr <WitOptVar> & theOptVarItr)
   {
   int         theIdx;
   WitOptVar * theOptVar;

   theIdx                    = theOptVarItr.myIdx ();

   theOptVar                 = myOptProblem ()->myLexOptVarSeq ()[theIdx];

   optLexObjElemVal_[theIdx] = mySolverIf_->primalVarVal (theOptVar);
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::loadInitSoln ()
   {
   WitVector <double> initSoln;
   WitOptVar *        theVar;
      
   if (not myOptComp ()->optInitMethod ()->external ())
      return;

   initSoln.resize (myOptProblem_->nOptVars (), 0.0);

   forEachEl (theVar, myOptProblem_->myOptVars ())
      initSoln[theVar->index ()] = theVar->primalValue ();

   mySolverIf_->loadInitSoln (initSoln);
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::boundLexObjElemVal (WitOptVar * theOptVar, double theVal)
   {
   double moTol;
   double theTol;

   moTol  = myOptComp ()->myMultiObjMgr ()->multiObjTol ();

   theTol = WitNonClass::max (moTol * fabs (theVal), moTol);

   mySolverIf_->setVarLB (theOptVar, theVal - theTol);
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::solveCurrentObj (bool firstObj)
   {
   if (myOptComp ()->mipMode ())
      {
      mySolverIf_->solveMip (true);
      }
   else
      {
      useDualSimplex_ =
         firstObj and myOptComp ()->crashOptStarter ()->isChosen ();

      mySolverIf_->solveLp (true);
      }
   }

//------------------------------------------------------------------------------

bool WitOptSolveMgr::optProbHasIntVars ()
   {
   WitOptVar * theOptVar;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      {
      if (theOptVar->isAnIntVar ())
         {
         return true;
         }
      }

   return false;
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::storePrimalSoln ()
   {
   WitVector <double> primalSoln;
   WitOptVar *        theVar;
   int                theIdx;

   primalSoln.resize (myOptProblem_->nOptVars ());

   mySolverIf_->getPrimalSoln (primalSoln);

   forEachEl (theVar, myOptProblem_->myOptVars ())
      {
      theIdx = theVar->index ();

      theVar->setPrimalValue (primalSoln[theIdx]);
      }
   }

//------------------------------------------------------------------------------

void WitOptSolveMgr::storeDualSoln ()
   {
   WitVector <double> dualSoln;
   WitOptCon *        theCon;
   int                theIdx;

   dualSoln.resize (myOptProblem_->nOptCons ());

   mySolverIf_->getDualSoln (dualSoln);

   forEachEl (theCon, myOptProblem_->myOptCons ())
      {
      theIdx = theCon->index ();

      theCon->setDualValue (dualSoln[theIdx]);
      }
   }

//------------------------------------------------------------------------------
// Implementation of class SolverIf
//------------------------------------------------------------------------------

WitSolverIf::~WitSolverIf ()
   {
   }

//------------------------------------------------------------------------------

WitSolverIf::WitSolverIf (WitOptSolveMgr * theOptSolveMgr):

      WitProbAssoc   (theOptSolveMgr),
      myOptSolveMgr_ (theOptSolveMgr),
      myOptProblem_  (theOptSolveMgr->myOptProblem ())
   {
   }

//------------------------------------------------------------------------------

void WitSolverIf::issueVersionMsg ()
   {
   }

//------------------------------------------------------------------------------

bool WitSolverIf::lexOptReloadNeeded ()
   {
   return false;
   }

//------------------------------------------------------------------------------

void WitSolverIf::loadIntData ()
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSolverIf::reviseLp ()
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSolverIf::loadInitSoln (const WitVector <double> &)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSolverIf::reSolveLp ()
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSolverIf::solveLp (bool)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSolverIf::solveMip (bool)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSolverIf::getDualSoln (WitVector <double> &)
   {
   stronglyAssert (false);
   }
