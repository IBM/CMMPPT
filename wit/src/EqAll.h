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

#ifndef EqAllH
#define EqAllH

//------------------------------------------------------------------------------
// Header file: "EqAll.h"
//
// Contains the declaration of class EqAllocator.
//------------------------------------------------------------------------------

#include <Schedule.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// class EqAllocator
//
// This class provides the interface between equitable allocation and the rest
// of WIT. An instance of this class exists of long as the Problem that owns it
// exists.
//------------------------------------------------------------------------------

class WitEqAllocator: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitEqAllocator (WitProblem * theProblem);
         //
         // ProbAssoc <<< theProblem.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitEqAllocator ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void activate ();
         //
         // Puts this EqAllocator into an active state.
         // Heurstic allocation must be active.

      void heurAllocIsReset ();
         //
         // Responds to the fact that heuristic allocation is being reset:
         // Specifically, puts this EqAllocator into an inactive state.

      void setAsapMultiExec (
            WitDemand * theDemand, 
            WitPeriod   shipPer,
            bool        theValue);

      bool containsDup (
            const WitDemPerStack & theDemPers,
            int &                  firstDupIndex,
            int &                  secondDupIndex);
         //
         // Returns true, iff theDemPers contains the same (Demand, Period)
         // more than once. In this case, firstDupIndex and secondDupIndex are
         // set to the first two indices of the first duplicated pair.

      void eqAlloc (const WitDemPerStack & theDemPers);
         //
         // Performs equitable allocation on theDemPers.
         // Resets the desIncVol of the (Demand, Period)s in theDemPers.
         // Prerequisites:
         //    This EqAllocator must be active.
         //    theDemPers must be non-empty.
         //    theDemPers must contain no duplicates.

      void print (const WitDemPerStack & theDemPers);
         //
         // Prints theDemPers.

      void setDesIncVol (
            WitDemand * theDemand, 
            WitPeriod   shipPer, 
            double      theValue);
         //
         // Sets desIncVol_ (theDemand)[shipPer] to theValue.

      double incVol (WitDemand * theDemand, WitPeriod shipPer);
         //
         // Data access.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitEqAllocator);

      void deactivate ();
         //
         // Puts this EqAllocator into an inactive state.

      bool multiSigDDs (const WitDemPerStack & theDemPers);
         //
         // Returns true, iff theDemPers contains at least 2 "significant"
         // (Demand, Period)s, i.e., those with positive desIncVol.

      void eqAllocMulti (const WitDemPerStack & theDemPers);
         //
         // Performs equitable allocation on theDemPers, which is assumed
         // to have at least 2 elements.

      bool asapMultiExec (WitDemand * theDemand, WitPeriod shipPer);
         //
         // Returns the asapMultiExec argument value to be passed to 
         // HeurAllMgr::inc for theDemand and shipPer.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      bool active_;
         //
         // true, equitable allocation is active.
         // Equitable allocation can only be active when heuristic allocation
         // is active.

      WitSchedule <WitDemand, double> desIncVol_;
         //
         // The Schedule of desired incremental allocation volumes.

      WitSchedule <WitDemand, double> incVol_;
         //
         // The Schedule of incremental allocation volumes.

      WitSchedule <WitDemand, double> prevDesVol_;
         //
         // The Schedule of previous desired incremental allocation volumes.

      WitSchedule <WitDemand, int> firstIndex_;
         //
         // Used by containsDup.
         // firstIndex_ (theDemand)[thePer] the first index in theDemPers of the
         // (Demand, Period) that matches theDemand and thePer, or -1, if no
         // such pair exists. For this purpose, the elements of theDemPers are
         // considered to have index 0, 1, ...

      WitSchedule <WitDemand, bool> asapMultiExec_;
         //
         // The Schedule of asapMultiExec argument values to be passed to 
         // HeurAllMgr::inc.
         // Only allocated in two-way multi-exec mode.
   };

#endif
