//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef HeurImpPH
#define HeurImpPH

//------------------------------------------------------------------------------
// Header file: "HeurImpP.h"
//
// Contains the declaration of the class HeurImpPerf
//------------------------------------------------------------------------------

#include <Schedule.h>
#include <PairStack.h>
#include <PtrVec.h>

//------------------------------------------------------------------------------
// class HeurImpPerf
//
// This class is responsible for actually performing a heuristic implosion.
// An instance of this class only exists during heuristic implosion.
//------------------------------------------------------------------------------

class WitHeurImpPerf: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitHeurImpPerf (WitProblem * theProblem, bool forOptVal);
         //
         // ProbAssoc <<< theProblem.
         // forOptVal == true indicates heur implosion for opt implosion.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitHeurImpPerf ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void implode ();
         //
         // Actually performs a heuristic implosion.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitHeurImpPerf);

      accessWrapper (WitEqAllocator *, myEqAllocator, myProblem ())

      void genPriorities ();
         //
         // Generates priorities either temporarily, or perminantly, or not at
         // all, as appropriate.

      void init ();
         //
         // Initializes this HeurImpPerf for an implosion.

      int origPriority (WitDemand * theDemand, WitPeriod thePer) const;
         //
         // Returns the priority (either temp or perm) for theDemand in thePer.

      void allocShipVols ();
         //
         // In userHeurStart mode, allocates shipVols to demandVols.

      void mainLoop ();
         //
         // The triple nested loop that defines the greedy ordering in which
         // the implosion heuristic performs its allocation.

      bool prioritiesAreDifferent (WitPeriod demPer);
         //
         // Returns true, iff there is at least one Demand whose priority
         // in demPer is different from in demPer - 1 or demPer == 0.

      static bool comparePriorities (
            WitDemand * theDemand1,
            WitDemand * theDemand2);
         //
         // Compare function used to compare the priority of two demands.
         // The rules are:
         //    Low positive numbers have higher priority.
         //    Priorities <= 0 are all equally the lowest priority.
         //    Ties are broken in favor of lower index.
         //    theContext is a HeurImpPerf cast to void *, which will used as
         //    the context for the comparison.

      void allocSortedDemands (WitPeriod demPer, WitPeriod shipPer);
         //
         // Allocates to the Demands, as sorted, and within the given
         // demPer and shipPer.

      void selectForEqAlloc (
            WitDemand * theDemand,
            WitPeriod   demPer,
            WitPeriod   shipPer);
         //
         // Selects theDemand, demPer, and shipPer for equitable allocation,
         // performing equitable allocation as needed.

      void eqAlloc (WitPeriod demPer);
         //
         // Performs on equitable allocation on selDemPers_
         // for the given demPer. Clears selDemPers_.

      void incAlloc (
            WitDemand * theDemand,
            WitPeriod   demPer,
            WitPeriod   shipPer);
         //
         // Increments heuristic allocation (non-equitable) for theDemand,
         // demPer, and shipPer);

      //------------------------------------------------------------------------
      // Other private member data.
      //------------------------------------------------------------------------

      const bool forOpt_;
         //
         // true iff the heuristic implosion so for optimizing implosion.

      WitSchedule <WitDemand, int> tempPriority_;
         //
         // Schedule of temporary priorities.

      WitPtrVec <WitDemand> sortedDemVec_;
         //
         // Vector of all demands in priority order.

      WitSchedule <WitDemand, int> modPriority_;
         //
         // Original priority, modified so that the lower numerical value
         // always means higher priority.
         // Specifically:
         //    All positive priorities are reduced by 1.
         //    All non-postive priorities are set to a value larger than 
         //    the largest positive priority.

      int curDemPer_;
         //
         // Demand period to be used when sorting demands.

      WitSchedule <WitDemand, double> unmetDemandVol_;
         //
         // Unmet demand volume.

      WitDemPerStack selDemPers_;
         //
         // A list of (Demand, Period)s selected for equitable allocation.
         // Stored in reverse order, until eqAlloc.
   };

#endif
