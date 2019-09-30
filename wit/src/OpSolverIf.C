//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "OpSolverIf.C"
//------------------------------------------------------------------------------

#include <OpSolverIf.h>
#include <OptProblem.h>

//------------------------------------------------------------------------------
// Implementation of class OpSolverIf.
//------------------------------------------------------------------------------

WitOpSolverIf::~WitOpSolverIf ()
   {
   }

//------------------------------------------------------------------------------

WitOpSolverIf::WitOpSolverIf (WitOptProblem * theOptProblem):

      WitProbAssoc  (theOptProblem->myProblem ()),
      myOptProblem_ (theOptProblem)
   {
   }
