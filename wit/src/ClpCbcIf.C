//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "ClpCbcIf.C"
//
// Contains the implementation of class ClpCbcIf.
//------------------------------------------------------------------------------

#include <ClpCbcIf.h>

//------------------------------------------------------------------------------

WitClpCbcIf::WitClpCbcIf (WitOptProblem * theOptProblem):

      WitOpSolverIf (theOptProblem)
   {
   }

//------------------------------------------------------------------------------

WitClpCbcIf::~WitClpCbcIf ()
   {
   }

//------------------------------------------------------------------------------

void WitClpCbcIf::solveOptProb ()
   {
   stronglyAssert (false);
   }
