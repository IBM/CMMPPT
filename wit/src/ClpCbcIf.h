//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef ClpCbcIfH
#define ClpCbcIfH

//------------------------------------------------------------------------------
// Header file: "ClpCbcIf.h"
//
// Contains the declaration of class ClpCbcIf.
//------------------------------------------------------------------------------

#include <OpSolverIf.h>

//------------------------------------------------------------------------------
// class ClpCbcIf
//
// "CLP/CBC Interface"
// Responsible for all interactions with COIN CLP & CBC.
//
// Class Hierarchy:
//
// ProbAssoc
//    OpSolverIf
//       ClpCbcIf
//------------------------------------------------------------------------------

class WitClpCbcIf: public WitOpSolverIf
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitClpCbcIf (WitOptProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitClpCbcIf ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void solveOptProb ();
         //
         // Solves the optimization problem by invoking CPLEX.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitClpCbcIf);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------
   };

#endif
