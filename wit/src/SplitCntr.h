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

#ifndef SplitCntrH
#define SplitCntrH

//------------------------------------------------------------------------------
// Header file: "SplitCntr.h"
//
// Contains the declaration of class SplitCntr.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class SplitCntr
//
// "Split Counter"
// Responsible for maintaining and displaying count information for selection
// splitting.
// 
// Implemented in SelSplit.C.
//------------------------------------------------------------------------------

class WitSplitCntr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSplitCntr (WitSelector *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitSplitCntr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void initCurSplits  ();
      void countCurSplits ();
      void countSplits    (bool tempCommOK);
      void countBS        (bool bsDone);
      void countBatch     ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void print ();
         //
         // Prints splitting count info.

      noCopyCtorAssign (WitSplitCntr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitSelector * const mySelector_;
         //
         // The Selector for this SplitCntr.

      int nCurSplits_;
         //
         // # of splits defined in current call to splitCommit.

      int nSplitsDone_;
         //
         // # of splits actually committed.

      int nSplitsDiscarded_;
         //
         // # of splits defined but never used.

      int nSplitSplitComs_;
         //
         // # of calls to splitCommit in which at least one split was done.

      int nUnsplitSplitComs_;
         //
         // # of calls to splitCommit in which no split was done.

      int nBSsDone_;
      int nBSsSkipped_;
         //
         // # binary searches done and skipped, respectively.

      int nBatches_;
         //
         // # split batches.
   };

#endif
