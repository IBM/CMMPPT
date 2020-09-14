//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef SplitPtH
#define SplitPtH

//------------------------------------------------------------------------------
// Header file: "SplitPt.h"
//
// Contains the declaration of class SplitPt.
//------------------------------------------------------------------------------

#include <PairStack.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// Class SplitPt
//
// "Split Point"
// Represents a point in the problem at which a selection is currently split.
// 
// Implemented in SelSplit.C.
//------------------------------------------------------------------------------

class WitSplitPt: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSplitPt (WitSplitter *, WitSelPt *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitSplitPt ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      bool isLowerThan (WitSplitPt * theSplitPt);
         //
         // Returns true, iff this SplitPt is considered to be lower than
         // theSplitPt.

      void setNetSearchVol (double theVal);
         //
         // Sets netSearchVol_ to theVal.
         // netSearchVol_ must be in a cleared state.

      void clearNetSearchVol ();
         //
         // Clears netSearchVol_.

      void initNetSplitList ();
         //
         // Sets the net split list for this SplitPt to its split list.

      void splitSelAsNeeded ();
         //
         // Splits the selection for this SplitPt, if appropriate.

      void defineSplit (
            double               totFlowVol,
            WitSelCandDblStack & theSplitPairs,
            double &             unbddVol);
         //
         // Sets theSplitPairs to be the split pairs for a split explosion of
         // totFlowVol units at this SplitPt. The pairs are stored in reverse
         // order. Sets unbddVol to the splitVol of the unbounded split
         // explosion, if any; otherwise 0.

      void activateTotFlowVol ();
         //
         // Activates totFlowVol_.

      void incTotFlowVol (double incFlowVol);
         //
         // If totFlowVol_ is active, it is incremented by incFlowVol.

      void deactivateTotFlowVol ();
         //
         // Deactivates totFlowVol_.

      bool hasSplits ();
         //
         // Returns true, iff this SplitPt has any splits

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitSelPt *, mySelPt)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void prtSplitting (WitSelCand * theSelCand, double splitBound);
         //
         // Prints the splitting of the selection at this SplitPt.

      void addNextPair (
            double &             netFlowVol,
            WitSelCandDblStack & theSplitPairs);
         //
         // In the context of defineSplit, this function adds the next split
         // pair to the front of theSplitPairs, netting from netFlowVol.

      double compUnbddVol (double netFlowVol);
         //
         // In the context of defineSplit, computes and returns the final
         // unbounded splitVol, if any; otherwise returns 0.

      void prtUnbddSplitExp (double unbddVol);
         //
         // Prints data for an unbounded split explosion.

      void prtSplitExp (
            WitSelCand * theSelCand, 
            double       splitVol, 
            double       splitBound,
            bool         boundedSplit);
         //
         // Prints data for a split explosion.

      noCopyCtorAssign (WitSplitPt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitSplitter * const mySplitter_;
         //
         // The Splitter that owns this SplitPt.

      WitSelPt * const mySelPt_;
         //
         // The SelPt associated with this SplitPt.

      double netSearchVol_;
         //
         // During a call to tempCommForSearch, this is the searchVol that was
         // passed, netted against any flow; otherwise -1.

      WitPairStack <WitSelCand, double> revSplitList_;
         //
         // The list of (SelCand, splitBound) pairs that define the split
         // selection at mySelPt_. Stored in reverse order.

      WitPairStack <WitSelCand, double> netSplitList_;
         //
         // The list of (SelCand, splitBound) pairs that define the portion of
         // the split selection at mySelPt_ that has not yet been used during
         // the current commit. Stored in forward order.

      double totFlowVol_;
         //
         // The total flowVol thru this SplitPt, when that is being measured;
         // otherwise -1.0.
   };

#endif
