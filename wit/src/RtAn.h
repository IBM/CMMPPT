//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef RtAnH
#define RtAnH

//------------------------------------------------------------------------------
// Header file: "RtAn.h"
//
// Contains the declaration of class RtAnalyzer.
//------------------------------------------------------------------------------

#include <Schedule.h>
#include <PtrSched.h>
#include <PairStack.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// Class RtAnalyzer
//
// "Routing Analyzer"
// Responsible for performing the routing analysis of penalized execution.
//
// Implemented in PenExec.C.
//------------------------------------------------------------------------------

class WitRtAnalyzer: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitRtAnalyzer (WitSelector *, WitExpRest *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitRtAnalyzer ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void modifyRouting ();
         //
         // Modifies the current routing as appropriate in order to fill a small
         // incremental requirement for topPart in topPer at a heuristically 
         // minimum exec penalty.

      void findHypoModPts (WitObjStack <WitSelPt> & theModPts);
         //
         // Sets theModPts to the set of SelPts at which the selection
         // configuration would be modified by a hypothetical call to
         // modifyRouting ().

      void recCommitVol (double commitVol);
         //
         // Records the fact that commitVol units of demand were commited based
         // on the current routing.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitRtAnalyzer);

      void checkInput ();
         //
         // Verifies that all required preconditions for pen-exec are met.

      void findRoutingMods ();

      bool inInitState ();
         //
         // Returns true, iff this RtAnalyzer is in its initial state.
         // (This function is only used for program verification.)

      void preAnalyze (WitPart * fillPart, WitPeriod fillPer);
         //
         // Pre-analyzes fillPart for filling demand in fillPer, i.e, records 
         // the fact the prerequisites for fillPart in fillPer are to be 
         // pre-analyzed and that fillPart in fillPer is to be analyzed some 
         // time after its prerequisites have been analyzed.

      void recBomEntPrereq (
            WitPart *     fillPart, 
            WitPeriod     fillPer,
            WitBomEntry * theBomEnt,
            WitPeriod     execPer);
         //
         // Records the prerequisite for fillPart in fillPer found thru 
         // theBomEnt in execPer, if any.

      void recConsEntPrereq (
            WitPart *      fillPart, 
            WitPeriod      fillPer,
            WitConsEntry * theConsEnt,
            WitPeriod      execPer);
         //
         // Records the prerequisite for fillPart in fillPer found thru 
         // theConsEnt in execPer, if any.

      void recordPrereq (
            WitPart * depPart, 
            WitPeriod depPer,
            WitPart * prereqPart, 
            WitPeriod prereqPer);
         //
         // Records the fact that prereqPart in prereqPer is a prerequisite for 
         // depPart in depPer, i.e., that depPart in depPer cannot be analyzed 
         // until prereqPart in prereqPer has been analyzed.

      bool activeForPreAn (WitRtCand * theRtCand, WitPeriod thePer);
         //
         // Returns true, iff theRtCand is currently active for explosion in
         // thePer for pre-analysis purposes.

      double analyze (WitPart * fillPart, WitPeriod fillPer);
         //
         // Analyzes fillPart for filling demand in fillPer and returns the
         // minimum penalty computed for it.

      bool needToAnalyze (
            WitPart *   fillPart,
            WitPeriod   fillPer, 
            WitPeriod & expPer);
         //
         // Returns true, iff it's necessary to do a detailed routing
         // analysis the fillPart in fillPer.
         // In the true case, expPer is set to the explosion period.

      double penThruBopEnt (WitBopEntry * theBopEnt, WitPeriod expPer);
         //
         // Returns the per unit exec penalty for exploding down thru theBopEnt
         // in expPer.

      double penThruBomEnt (WitBomEntry * theBomEnt, WitPeriod execPer);
         //
         // Returns the per unit exec penalty for exploding down thru theBomEnt
         // or one of its substitutes in expPer.

      double penThruBomEntMR (WitBomEntry * theBomEnt, WitPeriod execPer);
         //
         // Does the multi-route case of penThruBomEnt.

      double penThruRtSiteMR (WitRtSite * theRtSite, WitPeriod thePer);
         //
         // Analyzes theRtSite for a multi-route explosion in thePer and returns
         // the minimum penalty computed for it.

      double penThruRtSiteOPR (WitRtSite * theRtSite, WitPeriod thePer);
         //
         // Analyzes theRtSite for an overriding proportionate routing
         // explosion in thePer and returns the minimum penalty computed for it.

      double penThruRtSiteTBPR (WitRtSite * theRtSite, WitPeriod thePer);
         //
         // Analyzes theRtSite for a tie breaking proportionate routing
         // explosion in thePer and returns the minimum penalty computed for it.

      double penThruRtCand (WitRtCand * theRtCand, WitPeriod thePer);
         //
         // Returns the per unit exec penalty for exploding down thru theRtCand
         // in thePer.

      double penThruConsEnt (WitConsEntry * theConsEnt, WitPeriod execPer);
         //
         // Returns the per unit exec penalty for exploding down thru theConsEnt
         // in expPer.

      bool canPseudoNet (WitBomEntry * theBomEnt, WitPeriod execPer);
         //
         // Returns true, iff explosion thru theBomEnt in execPer
         // is blocked by the availability of pseudo-supply for netting thru the
         // substitutes of theBomEnt or thru theBomEnt itself.

      bool canDirPseudoNet (WitConsEntry * theConsEnt, WitPeriod execPer);
         //
         // Returns true, iff explosion thru theConsEnt in execPer is blocked by
         // the availability of pseudo-supply for netting thru theConsEnt
         // directly.

      void postAnalyze (WitPart * anaPart, WitPeriod anaPer);
         //
         // Records the fact that anaPart has been analyzed in anaPer.

      void addHypoPrModPts (WitObjStack <WitSelPt> & theModPts);
         //
         // Valid only when called by findHypoModPts.
         // Adds to theModPts the set of SelPts at which the prop-rt
         // configuration would be modified by a hypothetical call to 
         // modifyRouting ().

      //------------------------------------------------------------------------
      // Print functions.
      // Valid only if printMode_ is true.
      //------------------------------------------------------------------------

      void printAnaHead  ();
      void printPreAn    (WitPart *,   WitPeriod);
      void printPrereq   (WitPart *,   WitPeriod);
      void printGap      ();
      void printAnalysis (WitPart *,   WitPeriod);
      void printMrMods   ();
      void printLpprMods ();

      void printLpprMods (
            const WitRtCandPerStack & theRtCandPers,
            const char *              statText);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Associations.
      //------------------------------------------------------------------------

      WitSelector * const mySelector_;
         //
         // The Selector for this RtAnalyzer.

      WitExpRest * const myExpRest_;
         //
         // The ExpRest associated with this RtAnalyzer.

      WitMrMgr * const myMrMgr_;
         //
         // The MrMgr associated with this RtAnalyzer.

      WitPrMgr * myPrMgr_;
         //
         // The PrMgr associated with this RtAnalyzer, if any, otherwise NULL.

      WitRtMgr * const myRtMgr_;
         //
         // The RtMgr associated with this RtAnalyzer.

      //------------------------------------------------------------------------
      // Other private member data.
      //------------------------------------------------------------------------

      WitRtCandPerStack curMrMods_;
         //
         // Valid only during a call to modifyRouting or findHypoModPts.
         // The set of modifications to the multi-route configuration that would
         // result from a hypothetical call to modifyRouting ().

      WitRtCandPerStack curPrActMods_;
      WitRtCandPerStack curPrDeactMods_;
         //
         // Non-Empty only in tie breaking prop-rt mode.
         // Valid only during a call to modifyRouting or findHypoModPts.
         //
         // curPrActMods_ is the set of (RtCand, Period) pairs at which prop-rt
         // would be activated by a hypothetical call to modifyRouting ().
         //
         // curPrDeactMods_ is the set of (RtCand, Period) pairs at which
         // prop-rt would be deactivated by a hypothetical call to
         // modifyRouting ().

      WitPartPerStack toPreAn_;
         //
         // The set of (Part, fill period)s to be pre-analyzed.

      WitPtrSched <WitPart, WitPartPerStack> dependents_;
         //
         // dependents_.refFor (thePart, fillPer) is the set of (Part, Period)s 
         // which depend directly on (thePart, fillPer) for their analysis.

      WitSchedule <WitPart, int> nPrereqsToAna_;
         //
         // nPrereqsToAna_ (thePart, fillPer) is # direct prerequisites of
         // thePart in fillPer that have yet to be analyzed.

      WitPartPerStack toAnalyze_;
         //
         // The set of (Part, fill period)s to be analyzed.

      WitSchedule <WitPart, double> minPen_;
         //
         // minPen_ (thePart, thePer) is the minimum per unit penalty found for
         // meeting an infinitesimal incremental demand for thePart in thePer.

      WitSchedule <WitPart, bool> minPenValid_;
         //
         // minPenValid_ (thePart, thePer) is true, iff 
         // minPen_ (thePart, thePer) currently holds a valid value.
         // Allocated and used in DEBUG mode only.

      const bool printMode_;
         //
         // true, iff printing has been requested.

      double curPen_;
         //
         // The penalty of the currently selected route.

      double totCommitVol_;
         //
         // The total volume commited.

      double weightedTotPen_;
         //
         // The total penalty incurred, weighted by commitVol.

      WitSchedule <WitRtSite, bool> inTheModPts_;
         //
         // Valid only in tie breaking prop-rt mode.
         // inTheModPts_ (theRtSite)[thePer] is true, iff addHypoPrModPts () is
         // executing and the SelPt for prop-rt at theRtSite in thePer has been
         // added to theModPts.
   };

#endif
