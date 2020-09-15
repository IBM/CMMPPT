//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "SolveMgr.C"
//
// Contains the implementation of classe SolveMgr
//------------------------------------------------------------------------------

#include <SolveMgr.h>
#include <CoinIf.h>
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

bool WitSolveMgr::coinEmbedded ()
   {
   return WitCoinIf::coinEmbedded ();
   }

//------------------------------------------------------------------------------

bool WitSolveMgr::cplexEmbedded ()
   {
   return WitCplexIf::cplexEmbedded ();
   }

//------------------------------------------------------------------------------

WitSolveMgr::WitSolveMgr (WitOptProblem * theOptProblem):

      WitProbAssoc  (theOptProblem),
      myOptProblem_ (theOptProblem),
      mySolverIf_   (newSolverIf (theOptProblem))
   {
   }

//------------------------------------------------------------------------------

WitSolveMgr::~WitSolveMgr ()
   {
   delete mySolverIf_;
   }

//------------------------------------------------------------------------------

void WitSolveMgr::solveOptProb ()
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

WitSolverIf * WitSolveMgr::newSolverIf (WitOptProblem * theOptProblem)
   {
   if (coinEmbedded () and cplexEmbedded ())
      {
      if (theOptProblem->myOptComp ()->preferCoin ())
         return
            WitCoinIf::newInstance (theOptProblem);
      else
         return
            WitCplexIf::newInstance (theOptProblem);
      }
   else if (WitCoinIf::coinEmbedded ())
      return
         WitCoinIf::newInstance (theOptProblem);

   else if (WitCplexIf::cplexEmbedded ())
      return
         WitCplexIf::newInstance (theOptProblem);
   else
      {
      stronglyAssert (false);

      return NULL;
      }
   }

//------------------------------------------------------------------------------

void WitSolveMgr::solveOptProbAsLexOpt ()
   {
   issueSolveMsg ();

   myMsgFac () ("lexOptMsg");

   mySolverIf_->loadLp ();

   if (myOptComp ()->mipMode ())
      mySolverIf_->loadIntData ();

   writeMps ();

   if (devMode ())
      WitTimer::getTimeAndChargeToCurrent ();

   if (not myOptComp ()->mipMode ())
      loadInitSoln ();

   if (mySolverIf_->lexOptNeedsReload ())
      solveLexOptWReload ();
   else
      solveLexOptNoReload ();

   storePrimalSoln ();
   }

//------------------------------------------------------------------------------

void WitSolveMgr::solveOptProbAsLp ()
   {
   bool reqPrimalSimplex;

   issueSolveMsg ();

   mySolverIf_->loadLp ();

   writeMps ();

   loadInitSoln ();

   reqPrimalSimplex = myOptComp ()->optInitMethod ()->external ();

   mySolverIf_->solveLp (reqPrimalSimplex);

   storePrimalSoln ();

   if (myOptProblem_->needDual ())
      storeDualSoln ();
   }

//------------------------------------------------------------------------------

void WitSolveMgr::reSolveOptProbAsLp ()
   {
   myMsgFac () ("reSolveLpMsg", mySolverIf_->solverName ());

   mySolverIf_->reviseLp ();

   writeMps ();

   mySolverIf_->reSolveLp ();

   storePrimalSoln ();

   if (myOptProblem_->needDual ())
      storeDualSoln ();
   }

//------------------------------------------------------------------------------

void WitSolveMgr::solveOptProbAsMip ()
   {
   issueSolveMsg ();

   if (not optProbHasIntVars ())
      myMsgFac () ("mipModeNoIntVarsSmsg");

   mySolverIf_->loadLp ();

   mySolverIf_->loadIntData ();

   writeMps ();

   mySolverIf_->solveMip ();

   storePrimalSoln ();
   }

//------------------------------------------------------------------------------

void WitSolveMgr::issueSolveMsg ()
   {
   myMsgFac () ("solveOptProblemMsg",
      myMsgFac ().myFrag (myOptComp ()->mipMode ()? "mipFrag": "lpFrag"),
      mySolverIf_->solverName ());

   mySolverIf_->issueVersionMsg ();
   }

//------------------------------------------------------------------------------

void WitSolveMgr::writeMps ()
   {
   if (not myOptComp ()->printMps ())
      return;

   myMsgFac () ("mpsFileMsg");

   mySolverIf_->solverWriteMps ();
   }

//------------------------------------------------------------------------------

void WitSolveMgr::loadInitSoln ()
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

void WitSolveMgr::solveLexOptNoReload ()
   {
   WitOptVar *              prevOptVar;
   WitPtrVecItr <WitOptVar> theOptVarItr;
   WitOptVar *              theOptVar;
   double                   prevObjVal;

   prevOptVar = NULL;

   myOptProblem_->myLexOptVarSeq ().attachItr (theOptVarItr);

   while (theOptVarItr.advance (theOptVar))
      {
      myMsgFac () ("optLexObjElemMsg", theOptVar->lexObjElemName ());

      if (prevOptVar != NULL)
         {
         prevObjVal = mySolverIf_->primalVarVal (prevOptVar);

         boundLexObjElemVal (prevOptVar, prevObjVal);

         mySolverIf_->setObjCoeff (prevOptVar, 0.0);
         }

      solveCurLexObjElem (theOptVar, prevOptVar == NULL);

      prevOptVar = theOptVar;
      }
   }

//------------------------------------------------------------------------------
// solveLexOptWReload
//
// For the meaning of optLexObjElemVal, see applyPrevBounds.
//------------------------------------------------------------------------------

void WitSolveMgr::solveLexOptWReload ()
   {
   WitVector <double>       optLexObjElemVal;
   bool                     firstObjElem;
   WitPtrVecItr <WitOptVar> theOptVarItr;
   WitOptVar *              theOptVar;

   optLexObjElemVal.resize (myOptProblem_->myLexOptVarSeq ().length (), 0.0);

   firstObjElem = true;

   myOptProblem_->myLexOptVarSeq ().attachItr (theOptVarItr);

   while (theOptVarItr.advance (theOptVar))
      {
      myMsgFac () ("optLexObjElemMsg", theOptVar->lexObjElemName ());

      if (not firstObjElem)
         {
         mySolverIf_->loadLp ();

         mySolverIf_->loadIntData ();

         applyPrevBounds (theOptVar, optLexObjElemVal);
         }

      solveCurLexObjElem (theOptVar, firstObjElem);

      optLexObjElemVal[theOptVarItr.myIdx ()] =
         mySolverIf_->primalVarVal (theOptVar);

      firstObjElem = false;
      }
   }

//------------------------------------------------------------------------------

void WitSolveMgr::applyPrevBounds (
      WitOptVar *                theOptVar,
      const WitVector <double> & optLexObjElemVal)
   {
   WitPtrVecItr <WitOptVar> innerOptVarItr;
   WitOptVar *              innerOptVar;
   double                   theObjVal;

   myOptProblem_->myLexOptVarSeq ().attachItr (innerOptVarItr);

   while (innerOptVarItr.advance (innerOptVar))
      {
      if (innerOptVar == theOptVar)
         break;

      theObjVal = optLexObjElemVal[innerOptVarItr.myIdx ()];

      boundLexObjElemVal (innerOptVar, theObjVal);
      }
   }

//------------------------------------------------------------------------------

void WitSolveMgr::boundLexObjElemVal (WitOptVar * theOptVar, double theVal)
   {
   double moTol;
   double theTol;

   moTol  = myOptComp ()->myMultiObjMgr ()->multiObjTol ();

   theTol = WitNonClass::max (moTol * fabs (theVal), moTol);

   mySolverIf_->setVarLB (theOptVar, theVal - theTol);
   }

//------------------------------------------------------------------------------

void WitSolveMgr::solveCurLexObjElem (WitOptVar * theOptVar, bool firstObjElem)
   {
   mySolverIf_->setObjCoeff (theOptVar, 1.0);

   if (myOptComp ()->mipMode ())
      {
      mySolverIf_->solveMip ();
      }
   else if (firstObjElem)
      {
      bool reqPrimalSimplex;

      reqPrimalSimplex = myOptComp ()->optInitMethod ()->external ();

      mySolverIf_->solveLp (reqPrimalSimplex);
      }
   else
      mySolverIf_->reSolveLexLp ();

   if (devMode ())
      if (WitSaeMgr::standAloneMode ())
         myMsgFac () ("lexObjElemCpuTimeMsg",
            WitTimer::getTimeAndChargeToCurrent ());
   }

//------------------------------------------------------------------------------

bool WitSolveMgr::optProbHasIntVars ()
   {
   WitOptVar * theOptVar;

   forEachEl (theOptVar, myOptProblem_->myOptVars ())
      {
      if (theOptVar->isAnIntVar ())
         {
         return true;
         }
      }

   return false;
   }

//------------------------------------------------------------------------------

void WitSolveMgr::storePrimalSoln ()
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

void WitSolveMgr::storeDualSoln ()
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
