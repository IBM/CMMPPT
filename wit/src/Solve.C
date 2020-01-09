//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Solve.C"
//
// Contains the implementation of the following classes:
//    SolveGate
//    SolveMgr
//    SolverIf
//------------------------------------------------------------------------------

#include <SolveGate.h>
#include <SolveMgr.h>
#include <CoinIf.h>
#include <CplexIf.h>
#include <OptProblem.h>
#include <OptVar.h>
#include <OptCon.h>
#include <OptComp.h>
#include <OptStarter.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class SolveGate
//------------------------------------------------------------------------------

bool WitSolveGate::coinEmbedded ()
   {
   return WitCoinIf::coinEmbedded ();
   }

//------------------------------------------------------------------------------

bool WitSolveGate::cplexEmbedded ()
   {
   return WitCplexIf::cplexEmbedded ();
   }

//------------------------------------------------------------------------------

WitSolveGate::WitSolveGate (WitOptProblem * theOptProblem):

      mySolveMgr_ (new WitSolveMgr (theOptProblem))
   {
   }

//------------------------------------------------------------------------------

WitSolveGate::~WitSolveGate ()
   {
   delete mySolveMgr_;
   }

//------------------------------------------------------------------------------

void WitSolveGate::solveOptProb ()
   {
   mySolveMgr_->solveOptProb ();
   }

//------------------------------------------------------------------------------
// Implementation of class SolveMgr
//------------------------------------------------------------------------------

void WitSolveMgr::solveOptProb ()
   {
   if      (myOptComp ()->multiObjMode ())
      {
      mySolverIf_->solveOptProbAsLexOpt (); // V
      }
   else if (myOptComp ()->accOptStarter ()->isChosen ())
      {
      mySolverIf_->reSolveOptProbAsLp (); // V
      }
   else if (myOptComp ()->mipMode ())
      {
      mySolverIf_->solveOptProbAsMip (); // V
      }
   else
      {
      solveOptProbAsLp ();
      }
   }

//------------------------------------------------------------------------------

void WitSolveMgr::getColumnData (
      WitVector <double> & lb,
      WitVector <double> & ub,
      WitVector <double> & obj)
   {
   int         ncols;
   WitOptVar * theOptVar;
   int         theIdx;

   ncols = myOptProblem_->nOptVars ();

   lb .resize (ncols);
   ub .resize (ncols);
   obj.resize (ncols);

   forEachEl (theOptVar, myOptProblem_->myOptVars ())
      {
      theIdx      = theOptVar->index ();

      lb [theIdx] = theOptVar->bounds ().lower ();
      ub [theIdx] = theOptVar->bounds ().upper ();

      obj[theIdx] = theOptVar->objCoeff ();
      }
   }

//------------------------------------------------------------------------------

void WitSolveMgr::writeMps ()
   {
   if (not myOptComp ()->printMps ())
      return;

   myMsgFac () ("mpsFileMsg");

   mySolverIf_->solverWriteMps (); // V
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

   mySolverIf_->loadInitSoln (initSoln); // V
   }

//------------------------------------------------------------------------------

void WitSolveMgr::storePrimalSoln ()
   {
   WitVector <double> primalSoln;
   WitOptVar *        theVar;
   int                theIdx;

   primalSoln.resize (myOptProblem_->nOptVars ());

   mySolverIf_->getPrimalSoln (primalSoln); // V

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

//------------------------------------------------------------------------------

void WitSolveMgr::setUseDualSimplex (bool theValue)
   {
   useDualSimplex_ = theValue;
   }

//------------------------------------------------------------------------------

WitSolveMgr::WitSolveMgr (WitOptProblem * theOptProblem):

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

WitSolveMgr::~WitSolveMgr ()
   {
   delete mySolverIf_;
   }

//------------------------------------------------------------------------------

void WitSolveMgr::solveOptProbAsLp ()
   {
   mySolverIf_->issueSolveMsg (); // V

   mySolverIf_->loadLp (); // V

   writeMps ();

   useDualSimplex_ = myOptComp ()->crashOptStarter ()->isChosen ();

   loadInitSoln ();

   mySolverIf_->solveLp (myOptProblem_->needDual ()); // V

   storePrimalSoln ();

   if (myOptProblem_->needDual ())
      storeDualSoln ();
   }

//------------------------------------------------------------------------------
// Implementation of class SolverIf
//------------------------------------------------------------------------------

WitSolverIf::~WitSolverIf ()
   {
   }

//------------------------------------------------------------------------------

WitSolverIf::WitSolverIf (WitSolveMgr * theSolveMgr):

      WitProbAssoc  (theSolveMgr),
      mySolveMgr_   (theSolveMgr),
      myOptProblem_ (theSolveMgr->myOptProblem ())
   {
   }
