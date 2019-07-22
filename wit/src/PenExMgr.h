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

#ifndef PenExMgrH
#define PenExMgrH

//------------------------------------------------------------------------------
// Header file: "PenExMgr.h"
//
// Contains the declaration of class PenExMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class PenExMgr
//
// Class Hierarchy:
//
// ProbAssoc
//    PenExMgr
//
// Implemented in PenExec.C.
//------------------------------------------------------------------------------

class WitPenExMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPenExMgr (WitSelector *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPenExMgr ();

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

      void placeReqs (
            WitPart * srcPart,
            WitPeriod srcPer,
            WitPart * reqPart,
            WitPeriod reqPer);
         //
         // To be called when requirements are being placed on reqPart in
         // reqPer, whose source is srcPart in srcPer.

      void recExpBeyondNet (
            WitPart * thePart, 
            WitPeriod thePer, 
            bool      netForConsEnt);
         //
         // Records the fact that restricted explosion needs to continue after
         // netting supply of thePart in fillPer. netForConsEnt should be true,
         // iff the netting was ConsEntry netting.

      void postExpBomEnt ();
         //
         // To be called just after HeurAtor::explodeBomEntry.

      bool restExpAllowed (WitBopEntry * theBopEnt, WitPeriod fillPer);
         //
         // Returns true, iff restricted explosion thru theBopEnt to fill reqs
         // in fillPer is currently allowed.

      void recordFundShortage ();
         //
         // To be called whenever a fundamental shortage occurs.

      void reqPsSupRep (WitPart * thePart, WitPeriod thePer);
         //
         // Requests replenishment of pseudo-supply of thePart in thePer.

      void postTempCommit (bool success);
         //
         // To be called at the end of each temp commit, with success indicating
         // its result.

      void updatePsSup ();
         //
         // Makes updates to pseudo-supply that were requested in the most
         // recent failing temp-commit.

      void replenishPsSups ();
         //
         // Replenishes pseudo-supplies where this was requested.

      void print ();
         //
         // Prints this PenExMgr.

      bool failRestShortOnly ();
         //
         // Returns true, iff a restriction shortage was found in the most
         // recent failing temp commit, but no fundamental shortage.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPenExMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitExpRest * myExpRest_;
         //
         // The ExpRest owned by this PenExMgr.

      WitRtAnalyzer * myRtAnalyzer_;
         //
         // The RtAnalyzer owned by this PenExMgr.
   };

#endif
