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

#ifndef PipReqMgrH
#define PipReqMgrH

//------------------------------------------------------------------------------
// Header file: "PipReqMgr.h"
//
// Contains the declaration of class PipReqMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <PtrSched.h>
#include <ObjStack.h>
#include <PairStack.h>

//------------------------------------------------------------------------------
// Class PipReqMgr
//
// "Post-Implosion Pegging Requirement Manager"
//
// Responsible for managing the requirements in PIP.
//------------------------------------------------------------------------------

class WitPipReqMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipReqMgr (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPipReqMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      bool hasReqsFor (WitOperation * theOpn, WitPeriod execPer);
         //
         // Returns true, iff there is at least one requirement for theOpn in
         // execPer.

      WitPipReqBasis * newBasisForSeqNo (int theSeqNo);
         //
         // Constructs and returns a new ReqBasis for theSeqNo.

      void addReq (
            WitPart *        thePart,
            WitPeriod        fillPer,
            WitPipReqBasis * theBasis,
            double           incReqVol);
         //
         // Adds (theBasis, incReqVol) to the list of requirements for thePart
         // in fillPer.

      void addReq (
            WitBopEntry *    theBopEnt,
            WitPeriod        execPer,
            WitPipReqBasis * theBasis,
            double           incExecVol);
         //
         // Adds (theBasis, incExecVol) to the list of requirements for
         // theBopEnt in execPer.

      void getCurReqList (WitPipReqList & theReqList);
         //
         // Transfers the requirements in the current requirement list to
         // theReqList.

      void loadCurReqList (WitPart * thePart, WitPeriod fillPer);
         //
         // Transfers the requirements for thePart in fillPer to the current
         // requirement list. Sorts and consolidates the list, as well.

      void loadCurReqList (WitOperation * theOpn, WitPeriod execPer);
         //
         // Determines the requirements for theOpn in execPer and loads them
         // into the current requirement list.

      void clearCurReqList ();
         //
         // Clears curReqList_.

      void initReqListItr ();
         //
         // Initializes requirement list iteration.

      void advanceReqListItr (
            double &           netReqVol,
            WitPipReqBasis * & theBasis,
            double &           incReqVol);
         //
         // Advances requirement list iteration by no more than netReqVol units,
         // sets theBasis and incReqVol to the current PipReqBasis and
         // incremental requirement volume, and decrements netReqVol by
         // incReqVol.

      void advanceReqListItr (
            WitPipReqBasis * & theBasis,
            double &           incReqVol);
         //
         // Advances requirement list iteration and sets theBasis and
         // incReqVol to the current PipReqBasis and incremental requirement
         // volume.

      bool finishedReqListItr ();
         //
         // Returns true, iff requirement list iteration is finished.

      void addOpnClientToCurReqs (WitOperation * theOpn, WitPeriod execPer);
         //
         // Adds the ReqClient for (theOpn, execPer) to each of the requirements
         // in the current requirement list.

      void prtSizes ();
         //
         // Prints the maximum size and capacity of myReqSpecVec_.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void buildDemClients ();
      void buildOpnClients ();
         //
         // Builds the DemReqClients owned by this PipReqMgr.
         // Builds the OpnReqClients owned by this PipReqMgr.

      static bool compareOpnsForReqClient (
            WitOperation * theOpn1,
            WitOperation * theOpn2);
         //
         // Returns true, iff theOpn1 belongs ahead of theOpn2 when building
         // OpnReqClients.

      void loadReqsFromOne (WitOperation * theOpn, WitPeriod execPer);
         //
         // Assuming theOpn has exactly one BopEntry eligible for explosion at
         // execPer, this function loads the requirements from that BopEntry at
         // execPer into the current requirement list.

      void mergeReqs (WitOperation * theOpn, WitPeriod execPer);
         //
         // Merges the requirements in execPer for each of the BopEntries for
         // theOpn and loads them into the current requirement list.

      double minIncReqVol (WitOperation * theOpn, WitPeriod execPer);
         //
         // Returns the minimum incReqVol for the first PipReqSpec for each
         // BopEntry for theOpn in execPer.
         // Returns -1, if there are no more PipReqSpecs for the BopEntries for
         // theOpn in execPer.
 
      void extractReqBases ( 
            WitOperation *        theOpn,
            WitPeriod             execPer,
            double                deltaIncReqVol,
            WitPipReqBasisStack & theBases);
         //
         // Sets theBases to consist of the PipReqBasis for the first
         // PipReqSpec for each BopEntry for theOpn in execPer.
         // Removes deltaIncReqVol units of incReqVol from the beginning of the
         // ReqList for each BopEntry for theOpn in execPer.

      void sortReqList ();
         //
         // Sorts and consolidates curReqList_.

      void loadReqSpecVec ();
         //
         // Converts the ReqPairs in curReqList_ into PipReqSpecs and stores
         // them in myReqSpecVec_.

      void expandReqSpecVec ();
         //
         // Expands myReqSpecVec_ as needed.

      static bool compareReqSpecs (
            WitPipReqSpec * theReqSpec1,
            WitPipReqSpec * theReqSpec2);
         //
         // Returns true, iff theReqSpec1 belongs ahead of theReqSpec2 in
         // myReqSpecVec_.

      void unloadReqSpecVec ();
         //
         // Converts the PipReqSpecs in myReqSpecVec_ to ReqPairs,
         // consolidates them, and stores them in curReqList_.

      bool hasMultiExp (WitOperation * theOpn, WitPeriod execPer);
         //
         // Returns true, iff theOpn has more than one BopEntry eligible for
         // explosion at execution period execPer.

      bool expEligible (WitBopEntry * theBopEnt, WitPeriod execPer);
         //
         // Returns true, iff theBopEnt is eligible for explosion at execution
         // period execPer.

      void prtCurReqListBeforeSort ();
         //
         // Prints curReqList_ before sorting.

      void prtCurReqListAfterSort ();
         //
         // Prints curReqList_ after sorting.

      void prtCurReqList (WitOperation * theOpn, WitPeriod execPer);
         //
         // Prints curReqList_ after loading from theOpn in execPer.

      void prtCurReqListAfterAddOpn ();
         //
         // Prints curReqList_ after adding an Operation pair.

      void prtBopEntReqs (WitOperation * theOpn, WitPeriod execPer);
         //
         // Prints the requirements in execPer at each BopEntry for theOpn.

      void prtReqList (const WitPipReqList & theReqList);
         //
         // Prints theReqList.

      noCopyCtorAssign (WitPipReqMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPtrVec <WitPipReqClient <WitDemand> > myDemClients_;
         //
         // myDemClients_[theSeqNo] is the DemReqClient owned by this PipReqMgr
         // whose sequence number is theSeqNo.

      WitPtrSched <WitOperation, WitPipReqClient <WitOperation> > myOpnClients_;
         //
         // myOpnClients_.myPtrAt (theOpn, execPer) is the OpnReqClient owned
         // by this PipReqMgr for theOpn and execPer.
         // myOpnClients_.myPtrTVecAt (theOpn) is allocated, iff
         // theOpn->pipEnabled () is true.

      WitPipReqBasisStack myBases_;
         //
         // The PipReqBases owned by this PipReqMgr.

      WitPtrSched <WitPart, WitPipReqList> myPartReqs_;
         //
         // myPartReqs_.myElemAt (thePart, fillPer) is the list of requirement
         // pairs for thePart in fillPer.

      WitPtrSched <WitBopEntry, WitPipReqList> myBopEntReqs_;
         //
         // myBopEntReqs_.myElemAt (theBopEnt, execPer) is the list of
         // requirement pairs for theBopEnt in execPer.

      WitPtrVec <WitPipReqSpec> myReqSpecVec_;
         //
         // The set of active PipReqSpecs owned by the PipReqMgr.
         // They are stored in this vector, so that they can be sorted.

      int nActReqSpecs_;
         //
         // The number of PipReqSpecs currently active.

      int maxActReqSpecs_;
         //
         // Maximim number of PipReqSpecs ever active.

      WitPipReqList curReqList_;
         //
         // The current requirement list for this PipReqMgr.
         // The list of pairs, (theBasis, incReqVol), indicating that
         // incReqVol units of requirement for either the current Part and
         // Period or the current Operation and Period are pegged to theBasis.

      WitPipReqListItr myReqListItr_;
         //
         // The iterator that implements requirement list iteration.

      WitPipReqBasis * curBasis_;
         //
         // The current PipReqBasis for requirement list iteration.

      double incReqVolCur_;
         //
         // The current incremental requirement volume for requirement list
         // iteration.
   };

#endif
