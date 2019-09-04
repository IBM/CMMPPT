//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef SglSrcMgrH
#define SglSrcMgrH

//------------------------------------------------------------------------------
// Header file: "SglSrcMgr.h"
//
// Contains the declaration of class SglSrcMgr.
//------------------------------------------------------------------------------

#include <PtrMap.h>
#include <ObjStack.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// Class SglSrcMgr
//
// "Single-Source Manager"
//
// Performs single-source heuristic allocation.
//
// This class is part of the implementation of the new single-source algorithm.
//------------------------------------------------------------------------------

class WitSglSrcMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSglSrcMgr (WitSelector *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitSglSrcMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static bool sglSrcReq (WitProblem * theProblem);
         //
         // Returns TRUE, iff single-source has been requested for the Problem

      void commitMaxQty (double & netQty);
         //
         // Does commitMaxQty in sgl-src mode.

      void recPermCommit ();
         //
         // Records the fact that a perm-commit is being initiated.

      bool expProhibited (WitPart * thePart, WitPeriod expPer);
         //
         // Returns true, iff the sgl-src subsystem currently prohibits
         // explosion thru thePart in expPer.

      bool expProhibited (WitOperation * theOpn, WitPeriod execPer);
         //
         // Returns true, iff the sgl-src subsystem currently prohibits
         // explosion thru theOpn in execPer.

      void recExp (WitBillEntry * theBillEnt);
         //
         // Records the fact that requirements were exploded thru theBillEnt.

      bool sglSrcMode (WitRtSite * theRtSite);
         //
         // Returns true, iff theRtSite is currently in sgl-src mode.

      void saveAllocVal (int    & theVar);
      void saveAllocVal (double & theVar);
         //
         // Saves the current value of theVar to be restored as part of the
         // allocation.

      void saveExecVol (WitOperation * theOpn, WitPeriod thePer);
         //
         // Saves the execVol for theOpn in thePer.

      void saveSubVol (WitSubEntry * theSub, WitPeriod thePer);
         //
         // Saves the subVol for theSub in thePer.

      void saveConfigVal (bool & theVar);
      void saveConfigVal (int  & theVar);
         //
         // Saves the current value of theVar to be restored as part of the
         // selection configuration.

      template <typename ObjByPtr>
            inline void saveConfigVal (ObjByPtr * & theVar)
         {
         saveConfigObjByVPVal (reinterpret_cast <void * &> (theVar));
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void saveConfigObjByVPVal (void * & theVar);
         //
         // Saves the current value of theVar to be restored as part of the
         // selection configuration.

      void verifyRestrictions ();
         //
         // Verifies that all of the problem restrictions required for sgl-src 
         // hold.

      bool expProhibited (WitSglSrcSite * theSite, WitPeriod thePer);
         //
         // Returns true, iff explosion thru theSite is prohibited in thePer.

      void initMsAlloc (double & netQty, bool & fullAlloc);
         //
         // Performs the initial multi-source allocation on netQty.
         // On return, fullAlloc is true, iff a full allocation was achieved.

      WitSglSrcSite * nextOpp ();
         //
         // Finds and returns the next SglSrcSite to consider for sgl-src,
         // if any, else NULL.

      void selectDiscovery (WitSglSrcSite * theSite);
         //
         // Has theSite select its next RtCand, having discovered that it
         // already has a sgl-src flow.

      void tryNextCand (double netQty, WitSglSrcSite * theSite);
         //
         // Attempts to use the next RtCand of theSite as its sgl-src
         // selection, and keeps it, if it's successful.

      void saveCurAsIncumb ();
         //
         // Saves the current sgl-src configuration as the incumbent.

      void preserveIncumb ();
         //
         // Preserves the incumbent allocation.

      void prtNonFull ();
         //
         // Does printing for a non-full allocation.

      void cmqFixed (double netQty, double & newNetQty);
         //
         // Does commitMaxQty for a fixed sgl-src configuration.
         // The resulting new value of netQty is stored in newNetQty.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitSelector * const mySelector_;
         //
         // The Selector that owns this SglSrcMgr.

      WitRtMgr * const myRtMgr_;
         //
         // The RtMgr for this SglSrcMgr.

      WitPtrMap <WitRtSite, WitSglSrcSite> mySglSrcSiteFor_;
         //
         // mySglSrcSiteFor_ (theRtSite) is the SglSrcSite owned by this
         // SglSrcMgr corresponding to theRtSite, if any, else NULL.

      WitObjStack <WitSglSrcSite> alteredSites_;
         //
         // The set of SglSrcSites whose selection has been altered since the
         // most resent call to reset.

      bool cmqMode_;
         //
         // True, iff commitMaxQty (...) is currently executing.

      int curPermCommIdx_;
         //
         // The index of the current perm-commit.

      WitObjStack <WitSglSrcSite> curVisitSites_;
         //
         // The set of SglSrcSites visited in perm mode during the most recent
         // call to cmqFixed.

      WitObjStack <WitSglSrcSite> incumbVisitSites_;
         //
         // The set of SglSrcSites visited during the incumbent call to
         // cmqFixed.

      bool pureMsMode_;
         //
         // True, iff no sgl-src selections have been made in the incumbent
         // sgl-src configuration.

      //------------------------------------------------------------------------
      // Data Repositories.
      //------------------------------------------------------------------------

      WitDataRepos * initAllocRepos_;
         //
         // The DataRepos for restoring the allocation to its initial state.

      WitDataRepos * initConfigRepos_;
         //
         // The DataRepos for restoring the selection configuration to its
         // initial state.

      WitDataRepos * incumbAllocRepos_;
         //
         // The DataRepos for restoring the allocation to its incumbent state.

      WitDataRepos * incumbConfigRepos_;
         //
         // The DataRepos for restoring the selection configuration to its
         // incumbent state.

      //------------------------------------------------------------------------
      // Algorithmic event counts.
      //------------------------------------------------------------------------

      int nCmq_;
      int nCmqFixed_;
      int nFullAlloc_;
      int nNonFullAlloc_;
      int nOpps_;
      int nDiscovered_;
      int nTested_;
      int nAccepted_;
      int nRejected_;
         //
         // Number of occurrences of:
         //    commitMaxQty
         //    cmqFixed
         //    Full     allocation
         //    Non-full allocation
         //    Sgl-src opportunity considered
         //    Sgl-src selection discovered
         //    Sgl-src selection tested
         //    Sgl-src selection accepted
         //    Sgl-src selection rejected
   };

#endif
