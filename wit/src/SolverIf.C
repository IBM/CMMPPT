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

void WitSolverIf::solveOptProbAsLp ()
   {
   issueSolveMsg ();

   loadLp ();

   finishSolveOptProbAsLp ();
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

bool WitSolverIf::mipMode ()
   {
   return myOptComp ()->mipMode ();
   }
