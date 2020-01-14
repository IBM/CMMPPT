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
#include <CoinIf.h>
#include <CplexIf.h>
#include <OptProblem.h>
#include <OptVar.h>
#include <OptCon.h>
#include <OptComp.h>
#include <OptStarter.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class OptSolveGate
//------------------------------------------------------------------------------

bool WitOptSolveGate::coinEmbedded ()
   {
   return WitCoinIf::coinEmbedded ();
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
      mySolverIf_->solveOptProbAsLexOpt ();
      }
   else if (myOptComp ()->accOptStarter ()->isChosen ())
      {
      reSolveOptProbAsLp ();
      }
   else if (myOptComp ()->mipMode ())
      {
      mySolverIf_->solveOptProbAsMip ();
      }
   else
      {
      solveOptProbAsLp ();
      }
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

   if (WitCoinIf::coinEmbedded () and WitCplexIf::cplexEmbedded ())
      {
      useCoin = myOptComp ()->preferCoin ();
      }
   else if (WitCoinIf::coinEmbedded ())
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
      mySolverIf_ = WitCoinIf ::newInstance (this);
   else
      mySolverIf_ = WitCplexIf::newInstance (this);
   }

//------------------------------------------------------------------------------

WitOptSolveMgr::~WitOptSolveMgr ()
   {
   delete mySolverIf_;
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
