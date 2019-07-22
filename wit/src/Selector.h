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

#ifndef SelectorH
#define SelectorH

//------------------------------------------------------------------------------
// Header file: "Selector.h"
//
// Contains the declaration of class Selector.
//------------------------------------------------------------------------------

#include <ObjStack.h>
#include <PtrTVec.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// Class Selector
//
// Owner and governing class of the "multiple selections" subsystem.
// The multiple selections subsystem is responsible for implementing the
// multiple selections technique of heuristic allocation. This is a general
// technique that includes the following techniques as special cases:
//
//    multiple routes
//    build-ahead
//    multiple execution periods
//    selective stock reallocation
//    proportionate routing
//
// Thus the multiple selections subsystem is responsible for selecting each of
// the following:
//
//    Which  BopEntry to use when exploding through a Part     (for multiRoute).
//    Which ConsEntry to use when exploding through a BomEntry (for multiRoute).
//    Which explosion period to use when filling demand at a buildNstn or
//       buildAsap Material (for build-ahead).
//    Which execution period to use when explosing through a BopEntry in a
//       particular period (for multi-exec).
//    Whether or not to use stock reallocation for a particular Material in a
//       particular period (for selective stock reallocation).
//    What set of BopEntries to use when exploding through a Part (for 
//       prop-routing).
//    What set of ConsEntries to use when exploding through a BomEntry (for 
//       prop-routing).
//
// These selections are called the "selection configuration" or just the
// "configuration".
//
// In addition, the multiple selections subsystem is responsible for the
// following extensions to the multiple selections technique:
//
//    penalized execution
//    selection splitting
//    single source
//
// Each of the particular cases of the multiple selections technique and its
// extensions is handled by its own subsystem.
//
// The Selector's specific responsibilities are as follows:
//
//    Owning the main objects of the multiple selections subsystem.
//    Co-ordinating abstract aspects of the multiple selections technique.
//------------------------------------------------------------------------------

class WitSelector: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSelector (WitHeurAtor *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitSelector ();

      //------------------------------------------------------------------------
      // Modeless selection functions.
      //------------------------------------------------------------------------

      WitPeriod modelessExpPer (WitPart * thePart, WitPeriod fillPer);
         //
         // Same as selExpPer, except that it's valid independent of build-ahead
         // mode.

      WitPeriod modelessExecPer (WitBopEntry * theBopEnt, WitPeriod expPer);
         //
         // Same as selExecPer, except that it's valid independent of 
         // multi-exec mode.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static bool instNeededFor (WitHeurAtor * theHeurAtor);
         //
         // Returns TRUE, iff a Selector is needed for theHeurAtor.

      void commitMaxQty (double & netQty);
         //
         // Commits the heuristically maximum possible quantity of topPart_ 
         // in topPer_.

      void printCommitMaxQtySel ();
         //
         // Prints a heading for commitMaxQtySel.

      void preCommitSS ();
         //
         // Performs the actions to be taken for selection splitting at the
         // beginning of a commit.

      void postCommit ();
         //
         // Performs the actions to be taken for selection, just after a commit.

      void postTempCommit (bool success);
         //
         // Performs the actions to be taken for selection, just after a 
         // temp-commit.

      void alterSelections ();
         //
         // Alters the selections, as appropriate.
         // Valid only in non-sel-split mode.

      void alterSelSS ();
         //
         // Alters the selection, as appropriate.
         // Valid only in sel-split mode.

      void alterSelection (WitAltPt * theAltPt);
         //
         // Alters the selection at theAltPt, printing as needed.

      void recFillByCons (WitPart * thePart, WitPeriod fillPer);
         //
         // Records that fact that supply was consumed to fill reqs for thePart
         // in fillPer, if appropriate.

      void recFillNetRec (WitPart * thePart, WitPeriod fillPer);
         //
         // Records the fact that net reqs for thePart are being filled in
         // fillPer.

      void rollReq (
            WitMaterial * theMat,
            WitPeriod     fromPer,
            WitPeriod     toPer,
            double        rollVol);
         //
         // Records the fact rollVol units of requirements for theMat is being
         // rolled over from fromPer to toPer.
         // Valid only in stock reallocation mode.

      void placeReqs (
            WitBopEntry *  theBopEnt,
            WitConsEntry * theConsEnt,
            WitPeriod      execPer,
            WitPeriod      fillPer,
            bool           forNetting);
         //
         // Records the fact that an explosion thru theBopEnt and theConsEnt
         // in execution period execPer to fill requirements in fillPer caused
         // requirements to be placed. 
         // If forNetting is true, then ConsEntry netting is being recorded
         // instead of actual requirement placement.

      void recExecBoundShortage (
            WitBopEntry * theBopEnt,
            WitPeriod     execPer,
            WitPeriod     fillPer);
         //
         // Records the fact that tempCommit failed due to a hard UB on an
         // execVol when exploding thru theBopEnt with execution period 
         // execPer to fill demand in fillPer.

      void recLTBoundShortage (WitPart * thePart, WitPeriod fillPer);
         //
         // Records the fact that tempCommit failed due to a lead time UB when
         // exploding thru thePart to fill demand in fillPer.

      void recFundShortage (WitPart * thePart, WitPeriod fillPer);
         //
         // Records the fact that tempCommit failed, due to insufficient
         // availability of thePart in fillPer. (A fundamemtal shortage)

      void recShortage ();
         //
         // Records the fact that tempCommit failed, for any reason.

      void recCommitVol (double commitVol);
         //
         // See RtAnalyzer::recCommitVol.

      void prepCmqSel ();
         //
         // Prepares for a call to HeurAtor::commitMaxQtySel.

      void recordFlowSS (WitSelPt * theSelPt, double incFlowVol);
         //
         // See WitFlowMonitor::recordFlow.

      bool splitCommActive ();
         //
         // See Splitter::splitCommActive.

      void recoverInitState ();
         //
         // Restores the selection configuration to its initial state.
         // Valid only in selection recovery mode.
         //
         // Implementation in progress.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc    (WitHeurAtor *,  myHeurAtor)
      accessFunc    (bool,           failResAlt)
      accessFunc    (bool,           multiRoute)
      accessFunc    (bool,           penExec)
      accessFunc    (bool,           tbprMode)
      accessFunc    (int,            nAlts)

      accessNonNull (WitRtMgr *,     myRtMgr)
      accessNonNull (WitMrMgr *,     myMrMgr)
      accessNonNull (WitPrMgr *,     myPrMgr)
      accessNonNull (WitBaMgr *,     myBaMgr)
      accessNonNull (WitMeMgr *,     myMeMgr)
      accessNonNull (WitSsrMgr *,    mySsrMgr)
      accessNonNull (WitPenExMgr *,  myPenExMgr)
      accessNonNull (WitSglSrcMgr *, mySglSrcMgr)

      accessFunc    (const WitObjStack <WitAltPt> &,   pendingAlts)
      accessFunc    (const WitList <WitSelMgr> &,      mySelMgrs)
      accessFunc    (const WitPtrTVec <WitFixedPer> &, myFixedPer)

      inline bool propRtg ()
         {
         return (myPrMgr_ != NULL);
         }

      inline bool buildAhead ()
         {
         return (myBaMgr_ != NULL);
         }

      inline bool multiExec ()
         {
         return (myMeMgr_ != NULL);
         }

      inline bool selectiveSR ()
         {
         return (mySsrMgr_ != NULL);
         }

      inline bool selSplit ()
         {
         return (mySplitter_ != NULL);
         }

      inline bool sglSrc ()
         {
         return (mySglSrcMgr_ != NULL);
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitSelector);

      void buildSelMgrs ();
         //
         // Builds the SelMgrs to be owned by this Selector.

      void collectSelMgrs ();
         //
         // Collects each of the SelMgrs owned by this Selector, in downward
         // order.

      void collect (WitSelMgr * theSelMgr);
         //
         // If theSelMgr is not NULL, it is appended to mySelMgrs_.

      bool selSplitNeeded ();
         //
         // Returns true, iff selection splitting is to be performed.

      void innerPlaceReqs (
            WitBopEntry *  theBopEnt,
            WitConsEntry * theConsEnt,
            WitPeriod      execPer,
            WitPeriod      fillPer,
            WitReqPt *     theReqPt,
            bool           forNetting);
         //
         // Records the fact that an explosion thru theBopEnt and theConsEnt
         // in execution period execPer to fill requirements in fillPer caused
         // requirements to be placed at a location represented by theReqPt.
         // If forNetting is true, then ConsEntry netting is being recorded
         // instead of actual requirement placement.

      void getResAltFromNet (
            WitBomEntry * theBomEnt,
            WitPeriod     execPer,
            WitReqPt *    theReqPt);
         //
         // If any of the ConsEnts for theBomEnt was eligible for netting in
         // execPer and corresponding ReqPt has a resolving alt, the state of
         // that ReqPt is copied into theReqPt.

      bool copyResAlt (
            WitConsEntry * theConsEnt,
            WitPeriod      execPer,
            WitReqPt *     theReqPt);
         //
         // If theConsEnt was eligible for netting in execPer and corresponding
         // ReqPt has a resolving alt, the state of that ReqPt is copied into
         // theReqPt.

      bool getPendingAlt (WitAltPt * & theAltPt);
         //
         // If pendingAlts_ is non-empty, this function pops the top element off
         // of it, sets the corresponding SelPt to non-pending, and returns
         // true; otherwise this function returns false.

      void printPreAlt ();
         //
         // Does printing appropriate just prior to altering the selection
         // configuration.

      void printInit ();
         //
         // Prints this Selector in its initial state, as appropriate.

      void countAlts ();
         //
         // Print statistics regards the # of alterations.

      inline WitReqPt * myReqPt (WitPart * thePart, WitPeriod thePer);
         //
         // Returns the ReqPt associated with thePart in thePer.

      accessNonNull (WitSplitter *, mySplitter)

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Associations.
      //------------------------------------------------------------------------

      WitHeurAtor * const myHeurAtor_;
         //
         // The HeurAtor that owns this Selector.

      WitRtMgr * myRtMgr_;
         //
         // The RtMgr owned by this Selector.

      WitMrMgr * myMrMgr_;
         //
         // The MrMgr owned by this Selector in multi-route mode;
         // otherwise NULL.

      WitPrMgr * myPrMgr_;
         //
         // The PrMgr owned by this Selector in new prop-routing mode;
         // otherwise NULL.

      WitPenExMgr * myPenExMgr_;
         //
         // The PenExMgr owned by this Selector in pen-exec mode;
         // otherwise NULL.

      WitSglSrcMgr * mySglSrcMgr_;
         //
         // The SglSrcMgr owned by this Selector in new single-source mode;
         // otherwise NULL.

      WitReqPtMgr * myReqPtMgr_;
         //
         // The ReqPtMgr owned by this Selector.

      WitSplitter * mySplitter_;
         //
         // The Splitter owned by this Selector, in sel-split for pen-exec
         // mode; otherwise NULL.

      //------------------------------------------------------------------------
      // SelMgrs.
      //------------------------------------------------------------------------

      WitBaMgr * myBaMgr_;
         //
         // The BaMgr owned by this Selector, if any; otherwise NULL.

      WitMeMgr * myMeMgr_;
         //
         // The MeMgr owned by this Selector, if any; otherwise NULL.

      WitSsrMgr * mySsrMgr_;
         //
         // The SsrMgr owned by this Selector, if any; otherwise NULL.

      WitList <WitSelMgr> mySelMgrs_;
         //
         // The SelMgrs owned by this Selector.
         // They are listed in "downward BOM" order, which is the order in which
         // they need to be traversed, when placing requirements (placeReqs ()).

      //------------------------------------------------------------------------
      // Other private member data.
      //------------------------------------------------------------------------

      WitObjStack <WitAltPt> pendingAlts_;
         //
         // The AltPts at which the selection configuration is to be altered.

      WitList <int> altCounts_;
         //
         // Only used if selPrintLevel () >= 1.
         // Each element of altCounts corresponds to a batch of alterations.
         // Its value is the # of alterations in the batch.

      bool curResAlt_;
         //
         // true, iff the current temp-commit failed and a resolving alteration
         // to the configuration was found.

      bool failResAlt_;
         //
         // true, iff a resolving alteration to the configuration was found for
         // the most recent failing temp-commit.

      WitMapping <WitMaterial, WitPeriod> & lastConsResAltPer_;
         //
         // lastConsResAltPer_ (theMat) is the last Period in which:
         //    * reqs for theMat were filled by consuming supply, and
         //    * the corresponding ReqPt has a resolving alteration.

      WitObjStack <WitMaterial> nonNegLCRAPMats_;
         //
         // The set of Materials, theMat, for which lastConsResAltPer_ (theMat)
         // is non-negative.

      bool multiRoute_;
         //
         // true, iff the multiple routes technique is to be used.

      bool penExec_;
         //
         // true, iff the penalized execution technique is to be used.

      bool tbprMode_;
         //
         // true, iff the tie breaking prop-rt technique is to be used.

      int nAlts_;
         //
         // # Alterations performed (debugging output).

      WitPtrTVec <WitFixedPer> myFixedPer_;
         //
         // myFixedPer_[thePer]->myPer () == thePer.
   };

#endif
