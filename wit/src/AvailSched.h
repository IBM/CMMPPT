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

#ifndef AvailSchedH
#define AvailSchedH

//------------------------------------------------------------------------------
// Header file: "AvailSched.h"
//
// Contains the declaration of class AvailSched.
//------------------------------------------------------------------------------

#include <Schedule.h>
#include <Mapping.h>
#include <Material.h>

//------------------------------------------------------------------------------
// Class AvailSched
//
// The availability schedule for heuristic allocation.
//------------------------------------------------------------------------------

class WitAvailSched: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAvailSched (WitHeurAtor *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitAvailSched ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void attachSsrMgr ();
         //
         // Sets mySsrMgr_.

      void netWoSR (
            WitPart * thePart,
            WitPeriod reqPer,
            double &  theReqVol);
         //
         // Nets theReqVol in reqPer against the availability of thePart without
         // stock reallocation.

      void netWithIncAvail (
            WitPart * thePart,
            WitPeriod thePer,
            double &  theReqVol);
         //
         // Nets theReqVol against the incremental availability of thePart in
         // thePer.

      void netMatWoSR (
            WitMaterial * theMat,
            WitPeriod     reqPer,
            double &      theReqVol);
         //
         // Nets theReqVol in reqPer against the availability of theMat without
         // stock reallocation.

      WitPeriod netAndRoll (
            WitMaterial * theMat, 
            WitPeriod     reqPer,
            double &      theReqVol,
            double &      rollVol,
            bool &        bddSsrSplit);
         //
         // Nets theReqVol against the availability of theMat in reqPer,
         // ignoring preallocation in later periods. Returns the period to
         // which some of theReqVol is to be rolled over, if any; else -1. In
         // the roll-over case, sets rollVol to the amount to roll-over. If the
         // rollVol is for a bounded split of SSR, sets bddSsrSplit to true.

      void reserveAchStSLB (
            WitMaterial * theMat, 
            WitPeriod     thePer,
            double &      unachVol);
         //
         // Reserves the portion of the stock soft LB for theMat in thePer
         // that's currently achieved. On return, unachVol is the amount of 
         // stock soft LB for theMat in thePer that remains unachieved.

      void addToIncAvailVol (
            WitPart * thePart, 
            WitPeriod thePer, 
            double    deltaAvailVol);
         //
         // Adds deltaAvailVol to the incremental availability of thePart in
         // thePer.

      void print ();
         //
         // Prints this AvailSched.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      inline double incAvailVol (WitPart * thePart, WitPeriod thePer)
         {
         return incAvailVol_ (thePart)[thePer];
         }

      inline bool selectiveSR (WitMaterial * theMat)
         {
         return selectiveSR_ (theMat);
         }

      inline bool eitherSR (WitMaterial * theMat)
         {
         return uncondSR_ (theMat) or selectiveSR_ (theMat);
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void initForUHS ();
         //
         // Puts this AvailSched into its initial state for a user-specified
         // heuristic starting solution.

      void verifyUhsNNScrapCon ();
         //
         // Assumes that the current solution is the user-specified heuristic
         // starting solution and verifies scrapVol >= 0.0 and issues a
         // warning, if not.

      void initStockRealloc ();
         //
         // Does initialization for stock reallocation, as needed.

      bool unlimitedBuildAhead (WitMaterial * theMat);
         //
         // Returns true, iff:
         //    theMat has build-ahead and
         //    buildAheadUB[t] >= t for all t.

      double allocedVol (
            WitMaterial * theMat,
            WitPeriod     reqPer,
            bool          forSR);
         //
         // Computes and returns the amount of availability of theMat in reqPer
         // that has been allocated to periods later than reqPer.
         // If forSR is true, then availability allocated to periods eligible
         // for stock reallocation is ignored.

      double availToNet (
            WitMaterial * theMat, 
            WitPeriod     reqPer, 
            double        theReqVol,
            double        allVol);
         //
         // Returns the amount of theMat that's available to be netted against
         // theReqVol in reqPer.
         // allVol is the amount, if any, that has been pre-allocated.

      WitPeriod rollPeriod (WitMaterial * theMat, WitPeriod fromPer);
         //
         // Returns the period in which to roll requirements from reqPer, if
         // any, else -1.

      bool srAllowedFrom (WitMaterial * theMat, WitPeriod thePer);
         //
         // Returns true, iff stock reallocation for theMat from thePer is
         // allowed.

      noCopyCtorAssign (WitAvailSched);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitHeurAtor * const myHeurAtor_;
         //
         // The HeurAtor that owns this AvailSched.

      WitSsrMgr * mySsrMgr_;
         //
         // The SsrMgr associated with this AvailSched, in SSR mode; else NULL.

      WitSchedule <WitPart, double> incAvailVol_;
         //
         // incremental availability schedule.
         // incAvailVol[thePer] == 
         //   supplyVol[thePer] 
         //  +  prodVol[thePer] 
         //  -  consVol[thePer] 
         //  -  shipVol[thePer]
         //  - (  stockVol reserved for softLB in thePer
         //     - stockVol reserved for softLB in thePer - 1)
         //
         // Note that incAvailVol[t] can be negative.

      WitMapping <WitMaterial, WitPeriod> lastDecPer_;
         //
         // Last availability decrement period.
         // lastDecPer_ (theMat) == max {t | incAvailVol_[t] <  0}
         //                         -1, if   incAvailVol_[t] >= 0 for all t.

      WitMapping <WitMaterial, bool> uncondSR_;
         //
         // uncondSR_ (theMat) is true, iff unconditional stock reallocation is
         // to be performed on theMat.

      WitMapping <WitMaterial, bool> selectiveSR_;
         //
         // selectiveSR_ (theMat) is true, iff selective stock reallocation is
         // to be performed on theMat.
   };

#endif
