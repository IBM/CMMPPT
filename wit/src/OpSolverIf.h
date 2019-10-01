//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef OpSolverIfH
#define OpSolverIfH

//------------------------------------------------------------------------------
// Header file: "OpSolverIf.h"
//
// Contains the declaration of class OpSolverIf
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// class OpSolverIf.
// "Optimzation Problem Solver Interface"
// Responsible for all interactions with WIT's optimization problem solver.
// The interactions are implemented polymorphically.
//
// Class Hierarchy:
//
// ProbAssoc
//    OpSolverIf
//       CplexIf
//------------------------------------------------------------------------------

class WitOpSolverIf: public WitProbAssoc
   {
   public:

      static bool cplexEmbedded ();
         //
         // Returns true, iff CPLEX was embedded into the current build of WIT.
         // Implemented in CplexIf.C.

      static WitOpSolverIf * newInstance (WitOptProblem * theOptProblem);
         //
         // Creates and returns a new OpSolverIf for theOptProblem.
         // Issues a fatal error, if CPLEX is not embedded.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitOpSolverIf ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void solveOptProb () = 0;
         //
         // Solves the optimization problem.

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOpSolverIf (WitOptProblem *);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitOptProblem *, myOptProblem)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitOpSolverIf);

      static WitOpSolverIf * newInstanceForCplex (WitOptProblem * theOptProblem);
         //
         // If CPLEX is embedded,
         //    creates and returns a new OpSolverIf for CPLEX and theOptProblem.
         // If CPLEX is not embedded,
         //    issues a fatal error.
         // Implemented in CplexIf.C.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitOptProblem * const myOptProblem_;
         //
         // The OptProblem that owns this OpSolverIf.
   };

#endif
