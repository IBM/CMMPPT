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
   else if (myOptProblem ()->reSolveMode ())
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

      WitProbAssoc  (theOptProblem->myProblem ()),
      myOptProblem_ (theOptProblem)
   {
   }

//------------------------------------------------------------------------------

void WitSolverIf::setParams ()
   {
   }

//------------------------------------------------------------------------------

void WitSolverIf::writeMps ()
   {
   if (myOptComp ()->printMps ())
      {
      myMsgFac () ("mpsFileMsg");

      writeMpsSS ();
      }
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

void WitSolverIf::solveLp (bool optNeeded)
   {
   setParams ();

   finishSolveLp (optNeeded);
   }

//------------------------------------------------------------------------------

bool WitSolverIf::mipMode ()
   {
   return myOptComp ()->mipMode ();
   }

//------------------------------------------------------------------------------

void WitSolverIf::solveOptProbAsLp ()
   {
   issueSolveMsg ();

   loadLp ();

   writeMps ();

   setLpMethodByOptStarter ();

   loadInitSoln ();

   solveLp (myOptProblem ()->needDual ());

   finishSolveOptProbAsLp ();
   }
