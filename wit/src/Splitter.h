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

#ifndef SplitterH
#define SplitterH

//------------------------------------------------------------------------------
// Header file: "Splitter.h"
//
// Contains the declaration of class Splitter.
//------------------------------------------------------------------------------

#include <ObjStack.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// Class Splitter
//
// Performs selection splitting.
// 
// Implemented in SelSplit.C.
//------------------------------------------------------------------------------

class WitSplitter: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSplitter (WitSelector *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitSplitter ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void commitMaxQty (double & netQty);
         //
         // Commits the heuristically maximum possible quantity of topPart_ 
         // in topPer_.

      void preCommit ();
         //
         // Performs the actions to be taken for selection splitting at the
         // beginning of a commit.

      void postTempCommit (bool success);
         //
         // Performs the actions to be taken for selection splitting, just
         // after a temp-commit.

      WitSelMgr * lowestBddSelMgr ();
         //
         // Finds and returns the bounded SplitMgr owned by mySelector_ that's
         // lowest in the explosion logic, if any; otherwise returns NULL.

      void recordFlow (WitSelPt * theSelPt, double incFlowVol);
         //
         // See FlowMonitor.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitSelector *,  mySelector)
      accessFunc (WitSplitCntr *, mySplitCntr)
      accessFunc (bool,           splitCommActive)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      bool splitCommitNeeded ();
         //
         // Returns true, iff a split-commit needs to be performed.

      void splitCommit (bool & unresFound);
         //
         // Does a split-commit.
         // On return:
         //    unresFound is true, iff an unresolveable shortage was found.

      void buildZeroPts (bool & unresFound, bool & splitFound);
         //
         // Builds up the zeroPts_ set.
         // On return:
         //    unresFound is true, iff an unresolveable shortage was found.
         //    splitFound is true, iff an opportunity to split selections was
         //       found.

      void clearSplitItr ();
         //
         // Clears data associated with one iteration of the splitCommit loop.

      bool addChgPtsToZeroPts ();
         //
         // Adds the SplitPts corresponding to the current hypothetical
         // modification and alteration points to zeroPts_.
         // Returns true, iff at least one SplitPt was added.

      void splitSels ();
         //
         // Finds the lowest zero pt at which to split the selections and does
         // so there.

      bool resAltOrRest ();
         //
         // Returns true, iff the most recent failing temp-commit generated
         // either a resolving alteration or a restriction shortage.

      WitSplitPt * findLowestZeroPt ();
         //
         // Finds and returns the lowest element of zeroPts_.

      void searchForFlow (WitSplitPt * searchPt, double theFlowVol);
         //
         // Does an extended binary search on the flow thru searchPt, assuming
         // theFlowVol fails.

      bool tempCommForSearch (WitSplitPt * searchPt, double searchVol);
         //
         // Does a temp-commit if 1 unit of top level vol, discarding all flow
         // thru the zero points and discarding all flow above searchVol thru
         // theSplitPt.

      bool tempCommDiscZeroPts ();
         //
         // Does a temp-commit if 1 unit of top level vol, discarding all flow
         // thru the zero points.

      void splitChgPts ();
         //
         // Splits the selections at each current mod point/alt point based on
         // the flow resulting from doing a temp-commit.

      void findChgSplitPts (WitObjStack <WitSplitPt> & chgSplitPts);
         //
         // Sets chgSplitPts to be the set of SplitPts corresponding to the
         // modifications that would result if (hypothetically) ps-sup were
         // updated, creating them, if necessary.

      void findHypoChanges (WitObjStack <WitSelPt> & chgSelPts);
         //
         // In pen-exec mode, sets chgSelPts to be the set of modifications and
         // alterations that would result if (hypothetically) the following
         // updates performed, based on the most recent failing temp-commit:
         // * Update ps-sup.
         // * Discard the current selection at the current resolving alteration
         //   (if any).
         //
         // In non-pen-exec mode, sets chgSelPts to be the current resolving
         // alteration, if any.

      void findHypoChangesPE (WitObjStack <WitSelPt> & chgSelPts);
         //
         // Does the pen-exec case of findHypoChanges.

      //------------------------------------------------------------------------

      accessNonNull (WitPenExMgr *, myPenExMgr)

      noCopyCtorAssign (WitSplitter);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Associations.
      //------------------------------------------------------------------------

      WitSelector * const mySelector_;
         //
         // The Selector that owns this Splitter.

      WitFlowMonitor * myFlowMonitor_;
         //
         // The FlowMonitor owned by this Splitter.

      WitSplitCntr * mySplitCntr_;
         //
         // The SplitCntr owned by this Splitter.

      WitPenExMgr * myPenExMgr_;
         //
         // The PenExMgr associated with this Splitter, if any; otherwise NULL.

      //------------------------------------------------------------------------
      // Other private member data.
      //------------------------------------------------------------------------

      bool penExec_;
         //
         // true, iff the penalized execution technique is to be used.

      const double splitRes_;
         //
         // Split Resolution.

      WitObjStack <WitSplitPt> zeroPts_;
         //
         // The set of SplitPts that are to discard all flow.

      bool splitCommActive_;
         //
         // true, iff an invocation of splitCommit () is currently in progress.

      WitObjStack <WitSplitPt> mySplitPts_;
         //
         // The SplitPts owned by this Splitter.
   };

#endif
