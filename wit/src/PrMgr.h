//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef PrMgrH
#define PrMgrH

//------------------------------------------------------------------------------
// Header file: "PrMgr.h"
//
// Contains the declaration of class PrMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class PrMgr
//
// "Proportionate Routing Manager"
//
// Main Interface to the Proportionate Routing Subsystem.
//
// Class Hierarchy:
//
// ProbAssoc
//    PrMgr
//
// Implemented in PropRtg.C.
//------------------------------------------------------------------------------

class WitPrMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPrMgr (WitSelector *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPrMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static bool instNeededFor (WitProblem * theProblem);
         //
         // Returns TRUE, iff a PrMgr is needed for theProblem.

      void print ();
         //
         // Prints the proportionate routing subsystem.

      void definePropExp (
            WitRtSite *         theRtSite, 
            WitPeriod           thePer,
            double              expVol,
            WitRtCandDblStack & theExpPairs);
         //
         // Sets theExpPairs to be the list of pairs (theRtCand, propExpVol),
         // that define a proportionate explosion of expVol units at
         // theRtSite in thePer.

      void definePropExp (
            WitPart *           thePart, 
            WitPeriod           expPer,
            double              expVol,
            WitRtCandDblStack & theExpPairs);
         //
         // Sets theExpPairs to be the list of pairs (theRtCand, propExpVol),
         // that define a proportionate explosion of expVol units at
         // thePart in expPer.

      void definePropExp (
            WitBomEntry *       theBomEnt, 
            WitPeriod           execPer,
            double              expVol,
            WitRtCandDblStack & theExpPairs);
         //
         // Similar to definePropExp (WitPart *, ...).

      bool active (WitRtCand * theRtCand, WitPeriod thePer);
         //
         // Each returns true, iff proportionate routing is currently active for
         // theRtCand in thePer.

      void setActive (WitRtCand * theRtCand, WitPeriod thePer, bool theValue);
         //
         // Sets the "active for prop-rt status" of theRtCand in thePer to
         // theValue.

      bool allowed (WitRtCand * theRtCand, WitPeriod thePer);
         //
         // Each returns true, iff proportionate routing is currently allowed
         // for theRtCand in thePer.

      WitSelPt * mySelPtFor (WitRtSite * theRtSite, WitPeriod thePer);
         //
         // Returns the prop-rt SelPt for theRtSite and thePer.

      void storeRecoveryPair (WitRtCand * theRtCand, WitPeriod thePer);
         //
         // Stores (theRtCand, thePer) as a pair at which the prop-rtg
         // configuration is to be restored to its initial state.

      void recoverInitState ();
         //
         // Restores the prop-rtg configuration to its initial state.

      //------------------------------------------------------------------------
      // Selection splitting functions.
      //------------------------------------------------------------------------

      bool selIsSplit (WitPart * thePart, WitPeriod expPer);
         //
         // Returns true, iff the proportionate routing thru thePart in
         // expPer is split. Valid only in sel-split mode.

      bool selIsSplit (WitBomEntry * theBomEnt, WitPeriod execPer);
         //
         // Similar to selIsSplit (WitPart *, ...).

      void defineSplit (
            WitPart *           thePart, 
            WitPeriod           expPer,
            double              expVol,
            WitRtCandDblStack & theExpPairs,
            double &            unbddVol);
         //
         // Defines a split proportionate explosion of expVol units at
         // thePart in expPer, by setting the following arguments:
         //
         // theExpPairs: The explosion pairs for the bounded portion of the
         //              split.
         // unbddVol:    The splitVol for the unbounded portion of the split.

      void defineSplit (
            WitBomEntry *       theBomEnt, 
            WitPeriod           execPer,
            double              expVol,
            WitRtCandDblStack & theExpPairs,
            double &            unbddVol);
         //
         // Similar to defineSplit (WitPart *, ...).

      void recordFlowSS (
            WitPart * thePart,
            WitPeriod expPer,
            double    incFlowVol);
         //
         // Records the fact that incFlowVol additional units are being flowed
         // through thePart in expPer, for sel-split purposes.

      void recordFlowSS (
            WitBomEntry * theBomEnt,
            WitPeriod     execPer,
            double        incFlowVol);
         //
         // Similar to recordFlowSS (WitPart *, ...).

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      WitSelMgr * mySelMgrForProd ();
      WitSelMgr * mySelMgrForCons ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPrMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPrCoord * myPrCoord_;
         //
         // The PrCoord owned by this PrMgr.

      WitRtMgr * const myRtMgr_;
         //
         // The RtMgr for this PrMgr.
   };

#endif
