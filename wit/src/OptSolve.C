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

      WitProbAssoc    (theOptProblem),
      myOptProblem_   (theOptProblem),
      mySolverIf_     (NULL),
      useDualSimplex_ (true)
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

   if (not myOptComp ()->mipMode ())
      loadInitSoln ();

   prevOptVar = NULL;

   myOptProblem ()->myLexOptVarSeq ().attachItr (theOptVarItr);

   while (theOptVarItr.advance (theOptVar))
      {
      myMsgFac () ("optLexObjElemMsg", theOptVar->lexObjElemName ());

      if (prevOptVar != NULL)
         {
         lockLexObjElemVal (prevOptVar);

         mySolverIf_->setObjCoeff (prevOptVar->index (), 0.0);
         }

      mySolverIf_->setObjCoeff (theOptVar->index (), 1.0);

      solveCurrentObj (prevOptVar == NULL);

      if (devMode ())
         if (WitSaeMgr::standAloneMode ())
            myMsgFac () ("lexObjElemCpuTimeMsg",
               WitTimer::getTimeAndChargeToCurrent ());

      prevOptVar = theOptVar;
      }
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

void WitOptSolveMgr::lockLexObjElemVal (WitOptVar * theOptVar)
   {
   double objVal;
   double moTol;
   double theTol;

   objVal = mySolverIf_->primalVarVal (theOptVar->index ());

   moTol  = myOptComp ()->myMultiObjMgr ()->multiObjTol ();

   theTol = WitNonClass::max (moTol * fabs (objVal), moTol);

   mySolverIf_->setVarLB (theOptVar->index (), objVal - theTol);
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
