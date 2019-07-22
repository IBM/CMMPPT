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

#ifndef DetOptImpMgrH
#define DetOptImpMgrH

//------------------------------------------------------------------------------
// Header file: "DetOptImpMgr.h"
//
// Contains the declaration of class DetOptImpMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <Str.h>
#include <Schedule.h>
#include <PairStack.h>

//------------------------------------------------------------------------------
// Class DetOptImpMgr
// Responsible for performing a deterministic optimizing implosion.
//------------------------------------------------------------------------------

class WitDetOptImpMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitDetOptImpMgr (WitProblem * theProblem);
         //
         // ProbAssoc <<< theProblem.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitDetOptImpMgr ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, extOptActive)
      accessFunc (bool, accelerated)
      accessFunc (bool, clearingSoln)
      accessFunc (bool, uploadingSoln)

      accessNonNull (WitExtOptMgr *, myExtOptMgr)

      accessWrapper (WitOptComp *, myOptComp, myProblem ())

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void shutDown ();
         //
         // Shuts down optimizing implosion.
         // If accelerated () is true, this DetOptImpMgr is restored to an
         // unaccelerated state.

      void implode ();
         //
         // Performs an optimizing implosion.

      void evalObjectives ();
         //
         // Sets the optimization problem solution to the production and
         // shipment schedules and evaluates the objective functions for this
         // solution.

      bool boundsObjUsed () const;
         //
         // Returns true, iff the bounds objective was used.

      inline bool extOptActive ()
         {
         return (myExtOptMgr_ != NULL);
         }

      void startExtOpt ();
         //
         // Initiates external optimizing implosion.

      void finishExtOpt ();
         //
         // Concludes external optimizing implosion.

      void shutDownExtOpt ();
         //
         // Terminates external optimizing implosion without constructing the
         // implosion solution.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      accessNonNull (WitDetImpOP *, myDetImpOP)

      void startOptImp ();
         //
         // Initiates optimizing implosion.

      void finishOptImp ();
         //
         // Concludes optimizing implosion.

      void validateMipMode ();
         //
         // Verifies that the data is valid for MIP mode.

      void compInitSoln ();
         //
         // Computes the initial implosion solution, if any.

      void heurImpOnFirstObj ();
         //
         // Invokes heuristic implosion for the first objective in multi-obj
         // mode.

      void setUpOptProblem ();
         //
         // Sets up myDetImpOP_

      void compInitOptSoln ();
         //
         // Computes an initial solution to myDetImpOP_.

      void compObjectives ();
         //
         // Computes the values of the objective functions.

      void compSglObj ();
         //
         // Computes the values of the objective functions in single objective
         // mode.

      void compMultiObj ();
         //
         // Computes the values of the objective functions in multiple
         // objectives mode.

      void uploadSoln ();
         //
         // Uploads the implosion solution to the Components of myProblem ().

      double solnValue (const WitOptVar * theOptVar);
         //
         // Returns the primal solution value of theOptVar, modified as
         // appropriate for passing to myProblem ().

      void buildCritList ();
         //
         // Uses the dual solution to the optimization problem to form the
         // critical parts list.

      static bool comparePotential (
            std::pair <WitPart *, WitPeriod> *,
            std::pair <WitPart *, WitPeriod> *);
         //
         // Compare function for sorting the critical part list.

      noCopyCtorAssign (WitDetOptImpMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitDetImpOP * myDetImpOP_;
         //
         // The DetImpOP owned by this DetOptImpMgr, in any.

      bool extOptActive_;
         //
         // True, iff external optimizing implosion is currently active.

      WitExtOptMgr * myExtOptMgr_;
         //
         // The ExtOptMgr owned by this DetOptImpMgr, when external
         // optimizing implosion is active; otherwise NULL.
         // External optimizing implosion is currently active,
         // iff myExtOptMgr_ is not NULL.

      bool accelerated_;
         //
         // true, iff this DetOptImpMgr is eligible for an accelerated
         // implosion.

      bool clearingSoln_;
         //
         // True, iff the optimizing implosion subsystem is currently clearing
         // the solution.

      bool uploadingSoln_;
         //
         // True, iff the optimizing implosion subsystem is currently uploading
         // the solution.

      WitPartPerStack revCritList_;
         //
         // The critical part list for optimizing implosion.
         // Stored in reverse order.

      WitSchedule <WitPart, double> potential_;
         //
         // potential_(thePart)[thePer] is the estimated "potential for
         // improvement" that would result from increasing the supply of thePart
         // in period thePer. Specifically, it is the shadow price on the
         // material balance constraint for thePart in period thePer.
   };

#endif
