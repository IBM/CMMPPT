//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef CplexMgrNCH
#define CplexMgrNCH

//------------------------------------------------------------------------------
// Header file: "CplexMgrNC.h"
//
// Contains the declaration of NonClass functions for working with class
// CplexMgr.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// NonClass functions for working with class CplexMgr.
//
// Implemented in CplexMgr.C.
//------------------------------------------------------------------------------

namespace WitNonClass
   {
   bool cplexEmbedded ();
      //
      // Returns true, iff CPLEX was embedded into the current build of WIT.

   WitCplexMgr * newCplexMgr (WitOptProblem * theOptProblem);
      //
      // If CPLEX is embedded,
      //    creates and returns a new CplexMgr for theOptProblem
      // If CPLEX is not embedded,
      //    issues a fatal error.

   void deleteCplexMgr (WitCplexMgr * theCplexMgr);
      //
      // If CPLEX is embedded,
      //    deletes theCplexMgr.
      // If CPLEX is not embedded,
      //    issues a fatal error.

   void solveOptProb (WitCplexMgr * theCplexMgr);
      //
      // If CPLEX is embedded,
      //    solves the optimization problem for theCplexMgr by invoking CPLEX.
      // If CPLEX is not embedded,
      //    issues a fatal error.
   };

#endif
