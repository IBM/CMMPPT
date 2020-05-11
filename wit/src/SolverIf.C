//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "SolverIf.C"
//
// Contains the implementation of class SolverIf
//------------------------------------------------------------------------------

#include <SolverIf.h>
#include <OptProblem.h>

//------------------------------------------------------------------------------

WitSolverIf::~WitSolverIf ()
   {
   }

//------------------------------------------------------------------------------

WitSolverIf::WitSolverIf (WitOptProblem * theOptProblem):

      WitProbAssoc  (theOptProblem),
      myOptProblem_ (theOptProblem)
   {
   }

//------------------------------------------------------------------------------

void WitSolverIf::issueVersionMsg ()
   {
   }

//------------------------------------------------------------------------------

bool WitSolverIf::lexOptNeedsReload ()
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

void WitSolverIf::solveLp (bool)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSolverIf::reSolveLp ()
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSolverIf::reSolveLexLp ()
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSolverIf::solveMip ()
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSolverIf::getDualSoln (WitVector <double> &)
   {
   stronglyAssert (false);
   }
