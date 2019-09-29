//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef CplexIfNCH
#define CplexIfNCH

//------------------------------------------------------------------------------
// Header file: "CplexIfNC.h"
//
// Contains the declaration of NonClass functions for working with class
// CplexIf.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// NonClass functions for working with class CplexIf.
//
// Implemented in CplexIf.C.
//------------------------------------------------------------------------------

namespace WitNonClass
   {
   bool cplexEmbedded ();
      //
      // Returns true, iff CPLEX was embedded into the current build of WIT.

   WitCplexIf * newCplexIf (WitOptProblem * theOptProblem);
      //
      // If CPLEX is embedded,
      //    creates and returns a new CplexIf for theOptProblem
      // If CPLEX is not embedded,
      //    issues a fatal error.

   void deleteCplexIf (WitCplexIf * theCplexIf);
      //
      // If CPLEX is embedded,
      //    deletes theCplexIf.
      // If CPLEX is not embedded,
      //    issues a fatal error.

   void solveOptProb (WitCplexIf * theCplexIf);
      //
      // If CPLEX is embedded,
      //    solves the optimization problem for theCplexIf by invoking CPLEX.
      // If CPLEX is not embedded,
      //    issues a fatal error.
   };

#endif
