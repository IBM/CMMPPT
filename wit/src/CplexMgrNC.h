//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Watson Implosion Technology
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
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
