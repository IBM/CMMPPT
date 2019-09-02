//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef FlowMonH
#define FlowMonH

//------------------------------------------------------------------------------
// Header file: "FlowMon.h"
//
// Contains the declaration of class FlowMonitor.
//------------------------------------------------------------------------------

#include <PairStack.h>

//------------------------------------------------------------------------------
// Class FlowMonitor
//
// Responsible for flow monitoring for selection splitting.
// 
// Implemented in SelSplit.C.
//------------------------------------------------------------------------------

class WitFlowMonitor: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitFlowMonitor (WitHeurAtor *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitFlowMonitor ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void activate ();
         //
         // Activates flow-monitoring.

      void deactivate ();
         //
         // Deactivates flow-monitoring.

      void recordFlow (WitSelPt * theSelPt, double incFlowVol);
         //
         // Records the fact that incFlowVol additional units are being flowed
         // thru theSelPt.
         // No-op, if this FlowMonitor is not currently active.

      void postTempCommit (bool success);
         //
         // Performs the actions to be taken for flow monitoring, just
         // after a temp-commit.

      bool splitIsNeeded (
            WitSelPt * theSelPt, 
            bool       compToSuccess,
            double &   flowGap);
         //
         // Returns true, iff the selection needs to be split at theSelPt. 
         // If compToSuccess is true, the failing flow will be compared to the
         // successful flow; otherwise it will be compared to 0.
         // On Return, flowGap will be the flow gap at theSelPt.
         // Valid only in active mode.

      void setTotFlowVols ();
         //
         // Sets the totFlowVols of the SplitPts whose flow is being measured,
         // according to the most recent successful tempCommit.
         // Valid only in active mode.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      double flowThruSelPt (
            WitSelPt *               theSelPt,
            const WitSelPtDblStack & theFlowPairs);
         //
         // Returns the total flow through theSelPt, that's recorded in
         // theFlowPairs.

      noCopyCtorAssign (WitFlowMonitor);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitHeurAtor * myHeurAtor_;
         //
         // The HeurAtor for this FlowMonitor.

      bool active_;
         //
         // true, iff this FlowMonitor Splitter is currently active.

      WitSelPtDblStack curFlowPairs_;
         //
         // For each SelPt, theSelPt the total flowVol thru theSelPt during the
         // current temp-commit is given by:
         //
         //    sum over (theSelPt, incFlowVol) in curFlowPairs_ of incFlowVol.

      WitSelPtDblStack okFlowPairs_;
         //
         // Same as curFlowPairs_, but for the most recent successful 
         // temp-commit, if any.

      WitSelPtDblStack failFlowPairs_;
         //
         // Same as curFlowPairs_, but for the most recent failing temp-commit,
         // if any.
   };

#endif
