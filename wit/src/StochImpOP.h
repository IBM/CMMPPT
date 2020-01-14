//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef StochImpOPH
#define StochImpOPH

//------------------------------------------------------------------------------
// Header file: "StochImpOP.h"
//
// Contains the declaration of class StochImpOP.
//------------------------------------------------------------------------------

#include <OptProblem.h>
#include <List.h>

//------------------------------------------------------------------------------
// class StochImpOP
//
// "Stochastic Implosion Optimization Problem"
//
// Class hierarchy:
//
//    ProbAssoc
//       OptProblem
//          StochImpOP
//
// Implemented in StochOpt.C.
//------------------------------------------------------------------------------

class WitStochImpOP: public WitOptProblem
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStochImpOP (WitStochModeMgr *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStochImpOP ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual bool needDual ();

      //-----------------------------------------------------------------------
      // OptVC item printing functions.
      //
      // Each of these functions prints its argument to optProbFile ().
      //-----------------------------------------------------------------------

      void printItem (WitNode *);
      void printItem (WitDemand *);
      void printItem (WitBomEntry *);
      void printItem (WitSubEntry *);
      void printItem (WitStBddVar *);

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc    (WitStochModeMgr *,                myStochModeMgr)
      accessFunc    (WitScenGroup *,                   myScenGroupForStage0)
      accessNonNull (WitExecPerSched <WitBillEntry> *, myExecPerSched)

   private:

      //-----------------------------------------------------------------------
      // Overriding private virtual member functions.
      //-----------------------------------------------------------------------

      virtual void generateVariables   ();
      virtual void generateConstraints ();
      virtual void postGenFixed        ();
      virtual void postGenFlexible     ();
      virtual void printOptVCHeading   ();
      virtual void doScreening         ();

      //-----------------------------------------------------------------------
      // Other private member functions.
      //-----------------------------------------------------------------------

      void buildScenGroups ();
         //
         // Creates theScenGroups to be owned by this StochImpOP.
         // To be called during construction.

      bool positiveHardLBsExist  ();
         //
         // Returns true iff there is at least one BoundSet with a positive
         // hardLB in some Scenario.

      bool disallowedScrapExists ();
         //
         // Returns true iff there is at least one Part with
         // scrapAllowed == false.

      noCopyCtorAssign (WitStochImpOP);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitStochModeMgr * const myStochModeMgr_;
         //
         // The StochModeMgr for this StochImpOP.

      WitList <WitScenGroup> myScenGroups_;
         //
         // The ScenGroups owned by this StochImpOP.

      WitScenGroup * myScenGroupForStage0_;
         //
         // The ScenGroup owned by this StochImpOP for Stage 0.

      WitExecPerSched <WitBillEntry> * myExecPerSched_;
         //
         // The ExecPerSched owned by this StochImpOP during fixed matrix
         // generation; otherwise NULL.
   };

#endif
