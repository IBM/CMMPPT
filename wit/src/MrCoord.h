//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef MrCoordH
#define MrCoordH

//------------------------------------------------------------------------------
// Header file: "MrCoord.h"
//
// Contains the declaration of class MrCoord.
//------------------------------------------------------------------------------

#include <RtSite.h>
#include <RtCand.h>
#include <PtrMap.h>
#include <Schedule.h>
#include <PerSglStack.h>

//------------------------------------------------------------------------------
// Class MrCoord
//
// "Multi-Route Coordinator"
//
// Internal Coordinator of the Multi-Route Subsystem.
//
// Class Hierarchy:
//
// ProbAssoc
//    MrCoord
//
// Implemented in MultiRoute.C.
//------------------------------------------------------------------------------

class WitMrCoord: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMrCoord (WitSelector *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitMrCoord ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      WitRtCand * selRtCand (WitRtSite * theRtSite, WitPeriod thePer);
         //
         // Returns the currently selected RtCand to be used when exploding thru
         // theRtSite in thePer.

      WitAltPt * myAltPtFor (WitRtCand * theRtCand, WitPeriod thePer);
         //
         // myAltPtFor for explosion thru theRtCand in thePer.

      bool selIsSplit (WitRtSite * theRtSite, WitPeriod thePer);
         //
         // Returns true, iff there is a split routing selection for exploding
         // thru theRtSite in thePer. Valid only in sel-split mode.

      void defineSplit (
            WitRtSite *         theRtSite, 
            WitPeriod           thePer,
            double              expVol,
            WitRtCandDblStack & theSplitPairs,
            double &            unbddVol);
         //
         // Sets theSplitPairs to be the split pairs for a split explosion of
         // expVol units at theRtSite in thePer. Sets unbddVol to the splitVol
         // of the unbounded split explosion, if any; otherwise 0.

      void setPrevSel (WitRtCand * theRtCand, WitPeriod thePer, bool theVal);
         //
         // Sets prevSel_ (theRtCand)[thePer] to theVal.

      void saveRtCand (WitRtCand * theRtCand);
         //
         // Saves the value of theRtCand to be retrieved later.

      void retrieveRtCand (WitRtCand * & theRtCand);
         //
         // Sets theRtCand to the value saved by saveRtCand.

      void storeRecoveryPair (WitRtSite * theRtSite, WitPeriod thePer);
         //
         // Stores (theRtSite, thePer) as a pair at which the multi-route
         // configuration is to be restored to its initial state.

      void recoverInitState ();
         //
         // Restores the multi-route selection configuration to its initial
         // state.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitSelector *, mySelector)
      accessFunc (WitMrSelMgr *, myMrSelMgrForProd)
      accessFunc (WitMrSelMgr *, myMrSelMgrForCons)

      inline WitMrSite * myMrSiteFor (WitRtSite * theRtSite)
         {
         return myMrSiteFor_ (theRtSite);
         }

      inline bool prevSel (WitRtCand * theRtCand, WitPeriod thePer)
         {
         return
            prevSel_ (theRtCand).isAllocated ()?
               prevSel_ (theRtCand)[thePer]:
               false;
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void prtRecover (WitRtCand * theRtCand, WitPeriod thePer);
         //
         // Prints for the recovery of theRtCand in thePer.
         // (Used in selection recovery.)

      noCopyCtorAssign (WitMrCoord);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMrSelMgr * myMrSelMgrForProd_;
      WitMrSelMgr * myMrSelMgrForCons_;
         //
         // The MrSelMgrs owned by this MrCoord.

      WitPtrMap <WitRtSite, WitMrSite> myMrSiteFor_;
         //
         // myMrSiteFor_ (theRtSite) is this MrCoord's MrSite for theRtSite.

      WitSelector * const mySelector_;
         //
         // The Selector for this MrCoord.

      WitSchedule <WitRtCand, bool> prevSel_;
         //
         // If prevSel_ (theRtCand) is allocated:
         //    prevSel_ (theRtCand)[thePer] is true, iff theRtCand has been
         //    previously selected in thePer.
         //
         // Otherwise:
         //    theRtCand has not been previously selected in any period.

      WitRtCand * savedRtCand_;
         //
         // The RtCand saved and retrieved by saveRtCand () and
         // retrieveRtCand ().

      WitPerSglStack <WitRtSite> myRtSiteRecPairs_;
      WitPerSglStack <WitRtCand> myRtCandRecPairs_;
         //
         // In selection recovery mode:
         //    myRtSiteRecPairs_ is the set of all pairs (RtSite, Period) at
         //    which the multi-route configuration is to be restored to its
         //    initial state.
         //    myRtCandRecPairs_ is the set of all pairs (RtCand, Period) at
         //    which the multi-route configuration is to be restored to its
         //    initial state.
   };

#endif
