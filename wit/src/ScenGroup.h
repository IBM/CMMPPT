//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef ScenGroupH
#define ScenGroupH

//------------------------------------------------------------------------------
// Header file: "ScenGroup.h"
//
// Contains the declaration of class ScenGroup.
//------------------------------------------------------------------------------

#include <StochAssoc.h>
#include <List.h>
#include <PtrTVec.h>
#include <Schedule.h>

//------------------------------------------------------------------------------
// Class ScenGroup
//
// "Scenario Group"
// Represents a group of Scenarios.
// A ScenGroup is considered to belong to one of the two stages:
//    If it belongs to stage 0, then it represents the set of all Scenarios.
//    If it belongs to stage 1, then it represents one particular Scenario.
//
// Class hierarchy:
//
//    ProbAssoc
//       StochAssoc
//          ScenGroup
//
// Implemented in StochOpt.C.
//------------------------------------------------------------------------------

class WitScenGroup: public WitStochAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitScenGroup (WitStochImpOP *, WitStage *, const WitList <WitScenario> &);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitScenGroup ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void postGenFlexible ();
         //
         // Takes actions appropriate at the conclusion of generateFlexible ().

      void print ();
         //
         // Prints this ScenGroup to the OptProblem file.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc  (WitStochImpOP *,                         myStochImpOP)
      accessFunc  (WitStage *,                              myStage)
      accessFunc  (const WitList <WitScenario> &,           myScenarios)
      accessFunc  (double,                                  totalProb)
      accessFunc  (const WitList <WitStochLoc> &,           myStochLocs)
      accessFunc2 (const WitSchedule <WitDemand, double> &, cumDemandVol)

      inline WitStochLoc * myStochLocFor (WitPeriod thePer)
         {
         witAssert (myStochLocVec_[thePer] != NULL);

         return myStochLocVec_[thePer];
         }

      inline WitScenario * firstScenario ()
         {
         return myScenarios_.first ();
         }

   private:

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      void compCumDemandVol ();
         //
         // Computes cumDemandVol_.

      noCopyCtorAssign (WitScenGroup);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitStochImpOP * const myStochImpOP_;
         //
         // The StochImpOP that owns this ScenGroup.

      WitStage * const myStage_;
         //
         // The stage to which this ScenGroup belongs.

      WitList <WitScenario> myScenarios_;
         //
         // The group of Scenarios that this ScenGroup represents.

      double totalProb_;
         //
         // The sum of the probabilities of the Scenarios represented by this
         // ScenGroup.

      WitPtrTVec <WitStochLoc> myStochLocVec_;
         //
         // myStochLocVec_[thePer] is the StochLoc owned by this ScenGroup for
         // this ScenGroup and thePer, if any; otherwise NULL.

      WitList <WitStochLoc> myStochLocs_;
         //
         // StochLocs owned by this ScenGroup.

      WitSchedule <WitDemand, double> cumDemandVol_;
         //
         // cumDemandVol_ (theDemand)[thePer] is the cumulative demand for
         // theDemand in thePer in this ScenGroup.
         // Allocated only during opt problem generation.
   };

#endif
