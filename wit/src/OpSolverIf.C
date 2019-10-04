//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "OpSolverIf.C"
//------------------------------------------------------------------------------

#include <OpSolverIf.h>
#include <OptComp.h>
#include <OptProblem.h>
#include <GlobalComp.h>

//------------------------------------------------------------------------------
// Implementation of class OpSolverIf.
//------------------------------------------------------------------------------

WitOpSolverIf * WitOpSolverIf::newInstance (WitOptProblem * theOptProblem)
   {
   if (theOptProblem->myGlobalComp ()->tempPar (1) == "coin")
      if (coinEmbedded ())
         return newInstanceForCoin  (theOptProblem);

   if (cplexEmbedded ())
      return newInstanceForCplex (theOptProblem);

   if (coinEmbedded ())
      return newInstanceForCoin  (theOptProblem);

   stronglyAssert (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitOpSolverIf::~WitOpSolverIf ()
   {
   }

//------------------------------------------------------------------------------

void WitOpSolverIf::solveOptProb ()
   {
   if      (myOptComp ()->multiObjMode ())
      {
      solveOptProbAsLexOpt ();
      }
   else if (myOptProblem ()->reSolveMode ())
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

WitOpSolverIf::WitOpSolverIf (WitOptProblem * theOptProblem):

      WitProbAssoc  (theOptProblem->myProblem ()),
      myOptProblem_ (theOptProblem)
   {
   }
