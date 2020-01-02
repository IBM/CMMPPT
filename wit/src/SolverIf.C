//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "SolverIf.C"
//
// Contains the implementation of class SolverIf.
//------------------------------------------------------------------------------

#include <SolverIf.h>
#include <OptVar.h>
#include <OptCon.h>
#include <OptComp.h>
#include <OptProblem.h>
#include <OptStarter.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------

WitSolverIf::~WitSolverIf ()
   {
   }

//------------------------------------------------------------------------------

void WitSolverIf::solveOptProb ()
   {
   if      (myOptComp ()->multiObjMode ())
      {
      solveOptProbAsLexOpt ();
      }
   else if (myOptComp ()->accOptStarter ()->isChosen ())
      {
      reSolveOptProbAsLp ();
      }
   else if (mipMode ())
      {
      solveOptProbAsMip ();
      }
   else
      {
      solveOptProbAsLp ();
      }
   }

//------------------------------------------------------------------------------

WitSolverIf::WitSolverIf (WitOptProblem * theOptProblem):

      WitProbAssoc    (theOptProblem->myProblem ()),
      myOptProblem_   (theOptProblem),
      useDualSimplex_ (true)
   {
   }

//------------------------------------------------------------------------------

void WitSolverIf::writeMps ()
   {
   if (not myOptComp ()->printMps ())
      return;

   myMsgFac () ("mpsFileMsg");

   writeMpsSS ();
   }

//------------------------------------------------------------------------------

void WitSolverIf::loadInitSoln ()
   {
   WitVector <double> initSoln;
   WitOptVar *        theVar;
      
   if (not myOptComp ()->optInitMethod ()->external ())
      return;

   initSoln.resize (myOptProblem ()->nOptVars (), 0.0);

   forEachEl (theVar, myOptProblem ()->myOptVars ())
      initSoln[theVar->index ()] = theVar->primalValue ();

   loadInitSolnSS (initSoln.myCVec ());
   }

//------------------------------------------------------------------------------

void WitSolverIf::getColumnData (
      WitVector <double> & lb,
      WitVector <double> & ub,
      WitVector <double> & obj)
   {
   int         ncols;
   WitOptVar * theOptVar;
   int         theIdx;

   ncols = myOptProblem ()->nOptVars ();

   lb .resize (ncols);
   ub .resize (ncols);
   obj.resize (ncols);

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      {
      theIdx      = theOptVar->index ();

      lb [theIdx] = theOptVar->bounds ().lower ();
      ub [theIdx] = theOptVar->bounds ().upper ();

      obj[theIdx] = theOptVar->objCoeff ();
      }
   }

//------------------------------------------------------------------------------

void WitSolverIf::storePrimalSoln ()
   {
   WitVector <double> primalSoln;
   WitOptVar *        theVar;
   int                theIdx;

   primalSoln.resize (myOptProblem ()->nOptVars ());

   getPrimalSoln (primalSoln);

   forEachEl (theVar, myOptProblem ()->myOptVars ())
      {
      theIdx = theVar->index ();

      theVar->setPrimalValue (primalSoln[theIdx]);
      }
   }

//------------------------------------------------------------------------------

void WitSolverIf::storeDualSoln ()
   {
   WitVector <double> dualSoln;
   WitOptCon *        theCon;
   int                theIdx;

   dualSoln.resize (myOptProblem ()->nOptCons ());

   getDualSoln (dualSoln);

   forEachEl (theCon, myOptProblem ()->myOptCons ())
      {
      theIdx = theCon->index ();

      theCon->setDualValue (dualSoln[theIdx]);
      }
   }

//------------------------------------------------------------------------------

bool WitSolverIf::mipMode ()
   {
   return myOptComp ()->mipMode ();
   }

//------------------------------------------------------------------------------

void WitSolverIf::setUseDualSimplex (bool theValue)
   {
   useDualSimplex_ = theValue;
   }

//------------------------------------------------------------------------------

void WitSolverIf::solveOptProbAsLp ()
   {
   issueSolveMsg ();

   loadLp ();

   writeMps ();

   setUseDualSimplex (myOptComp ()->crashOptStarter ()->isChosen ());

   loadInitSoln ();

   solveLp (myOptProblem ()->needDual ());

   storePrimalSoln ();

   if (myOptProblem ()->needDual ())
      storeDualSoln ();
   }
