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

#ifndef MrMgrH
#define MrMgrH

//------------------------------------------------------------------------------
// Header file: "MrMgr.h"
//
// Contains the declaration of class MrMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class MrMgr
//
// "Multi-Route Manager"
//
// Main Interface to the Multi-Route Subsystem.
//
// Class Hierarchy:
//
// ProbAssoc
//    MrMgr
//
// Implemented in MultiRoute.C.
//------------------------------------------------------------------------------

class WitMrMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMrMgr (WitSelector *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitMrMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static bool instNeededFor (WitProblem * theProblem);
         //
         // Returns TRUE, iff a MrMgr is needed for theProblem.

      WitBopEntry * selBopEnt (WitPart * thePart, WitPeriod expPer);
         //
         // Returns the currently selected BopEntry to be used when exploding
         // through thePart in expPer.

      WitConsEntry * selConsEnt (WitBomEntry * theBomEnt, WitPeriod execPer);
         //
         // Similar to selConsEnt (WitPart *, ...).

      WitRtCand * selRtCand (WitRtSite * theRtSite, WitPeriod thePer);
         //
         // Returns the currently selected RtCand to be used when exploding thru
         // theRtSite in thePer.

      bool activeForPE (WitRtCand * theRtCand, WitPeriod thePer);
         //
         // Returns true, iff theRtCand is currently active in thePer for
         // pen-exec purposes .

      bool multiChoice (WitRtSite * theRtSite);
         //
         // Returns true, iff there are at least two RtCands eligible for
         // explosion from theRtSite.

      bool valid (WitRtCand * theRtCand, WitPeriod thePer);
         //
         // Returns true, iff theRtCand is currently valid for selection in
         // thePer.

      void select (WitRtCand * theRtCand, WitPeriod thePer);
         //
         // Causes theRtCand to be selected by its RtSite in thePer.

      WitSelPt * mySelPtFor (WitRtSite * theRtSite, WitPeriod thePer);
         //
         // Returns the multi-route SelPt for theRtSite and thePer.

      void print ();
         //
         // Prints the multi-route subsystem.

      void storeRecoveryPair (WitRtSite * theRtSite, WitPeriod thePer);
         //
         // Stores (theRtSite, thePer) as a pair at which the multi-route
         // configuration is to be restored to its initial state.

      void recoverInitState ();
         //
         // Restores the multi-route configuration to its initial state.

      //------------------------------------------------------------------------
      // Selection splitting functions.
      //------------------------------------------------------------------------

      bool selIsSplit (WitPart * thePart, WitPeriod expPer);
         //
         // Returns true, iff there is a split routing selection for exploding
         // thru thePart in expPer. Valid only in sel-split mode.

      bool selIsSplit (WitBomEntry * theBomEnt, WitPeriod execPer);
         //
         // Similar to selIsSplit (WitPart *, ...).

      void defineSplit (
            WitPart *           thePart,
            WitPeriod           expPer,
            double              expVol,
            WitRtCandDblStack & theSplitPairs,
            double &            unbddVol);
         //
         // Sets theSplitPairs to be the split pairs for a split explosion of
         // expVol units at thePart in expPer. Sets unbddVol to the splitVol
         // of the unbounded split explosion, if any; otherwise 0.

      void defineSplit (
            WitBomEntry *       theBomEnt,
            WitPeriod           execPer,
            double              expVol,
            WitRtCandDblStack & theSplitPairs,
            double &            unbddVol);
         //
         // Similar to defineSplit (WitPart *, ...).

      void recordFlowSS (
            WitBillEntry * theBillEnt,
            WitPeriod      thePer,
            double         incFlowVol);
         //
         // Records the fact that incFlowVol additional units are being flowed
         // through theBillEnt in thePer, for sel-split purposes.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      WitSelMgr * mySelMgrForProd ();
      WitSelMgr * mySelMgrForCons ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitMrMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMrCoord * myMrCoord_;
         //
         // The MrCoord owned by this MrMgr.

      WitRtMgr * myRtMgr_;
         //
         // The RtMgr for this MrMgr.
   };

#endif
