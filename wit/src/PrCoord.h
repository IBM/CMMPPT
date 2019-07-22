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

#ifndef PrCoordH
#define PrCoordH

//------------------------------------------------------------------------------
// Header file: "PrCoord.h"
//
// Contains the declaration of class PrCoord.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <PtrSched.h>
#include <PerSglStack.h>

//------------------------------------------------------------------------------
// Class PrCoord
//
// "Proportionate Routing Coordinator"
//
// Internal Coordinator of the Proportionate Routing Subsystem.
//
// Class Hierarchy:
//
// ProbAssoc
//    PrCoord
//
// Implemented in PropRtg.C.
//------------------------------------------------------------------------------

class WitPrCoord: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPrCoord (WitSelector *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPrCoord ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

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
         // that define a proportionate explosion of expVol units at theRtSite
         // in thePer.

      bool active (WitRtCand * theRtCand, WitPeriod thePer);
         //
         // Returns true, iff theRtCand is currently active for proportionate
         // explosions in thePer.

      void setActive (WitRtCand * theRtCand, WitPeriod thePer, bool theValue);
         //
         // Sets the "active for prop-rt status" of theRtCand in thePer to
         // theValue.

      bool allowed (WitRtCand * theRtCand, WitPeriod thePer);
         //
         // Each returns true, iff proportionate routing is currently allowed
         // for theRtCand in thePer.

      void storeRecoveryPair (WitRtCand * theRtCand, WitPeriod thePer);
         //
         // Stores (theRtCand, thePer) as a pair at which the prop-rtg
         // configuration is to be restored to its initial state.

      void recoverInitState ();
         //
         // Restores the prop-rtg selection configuration to its initial state.

      WitSelPt * mySelPtFor (WitRtSite * theRtSite, WitPeriod thePer);
         //
         // Returns the prop-rt SelPt for theRtSite and thePer, if any;
         // otherwise NULL.

      WitAltPt * myAltPtFor (WitRtCand * theRtCand, WitPeriod thePer); 
         //
         // Returns the AltPt for explosion thru theRtCand in thePer.

      WitPrAltPt * myPrAltPtFor (WitRtCand * theRtCand, WitPeriod thePer); 
         //
         // Returns the PrAltPt owned by this PrCoord for theRtCand in thePer.

      //------------------------------------------------------------------------
      // Selection splitting functions.
      //------------------------------------------------------------------------

      bool selIsSplit (WitRtSite * theRtSite, WitPeriod thePer);
         //
         // Returns true, iff the proportionate routing thru theRtSite in thePer
         // is split. Valid only in sel-split mode.

      void defineSplit (
            WitRtSite *         theRtSite, 
            WitPeriod           thePer,
            double              expVol,
            WitRtCandDblStack & theExpPairs,
            double &            unbddVol);
         //
         // Defines a split proportionate explosion of expVol units at theRtSite
         // in thePer, by setting the following arguments:
         //
         // theExpPairs: The explosion pairs for the bounded portion of the
         //              split.
         // unbddVol:    The splitVol for the unbounded portion of the split.

      void recordFlowSS (
            WitRtSite * theRtSite,
            WitPeriod   thePer,
            double      incFlowVol);
         //
         // Records the fact that incFlowVol additional units are being flowed
         // through theRtSite in thePer, for sel-split purposes.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void printPropExp (
            WitRtSite *               theRtSite, 
            WitPeriod                 thePer,
            double                    expVol,
            const WitRtCandDblStack & theExpPairs,
            bool                      bddSplit);
         //
         // Prints the data for a proportionate explosion of
         // expVol units at theRtSite in thePer, as specified by theExpPairs.
         // bdd is true, iff the proportionate explosion is for a bounded split.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc  (WitSelector *, mySelector)
      accessFunc  (WitPrSelMgr *, myPrSelMgrForProd)
      accessFunc  (WitPrSelMgr *, myPrSelMgrForCons)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void buildPrSelPts ();
         //
         // Creates the PrSelPts owned by this PrCoord.

      void buildPrAltPts ();
         //
         // Creates the PrAltPts owned by this PrCoord.

      void findActiveRtCands (
            WitRtSite *      theRtSite,
            WitPeriod        thePer,
            WitRtCandStack & theRtCands);
         //
         // Sets theRtCands to be the set of all active RtCands for theRtSite 
         // in thePer.

      void defineExpPairs (
            const WitRtCandStack & theRtCands,
            WitPeriod              thePer,
            double                 expVol,
            WitRtCandDblStack &    theExpPairs);
         //
         // Sets theExpPairs to be the list of pairs (theRtCand, propExpVol),
         // that define a proportionate explosion of expVol units at theRtCands
         // in thePer.

      WitPrSelPt * myPrSelPtFor (WitRtSite * theRtSite, WitPeriod thePer);
         //
         // Returns the PrSelPt for theRtSite and thePer, if any;
         // otherwise NULL.

      noCopyCtorAssign (WitPrCoord);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitSelector * const mySelector_;
         //
         // The Selector for this PrCoord.

      WitRtMgr * const myRtMgr_;
         //
         // The RtMgr for this PrCoord.

      WitPrSelMgr * myPrSelMgrForProd_;
      WitPrSelMgr * myPrSelMgrForCons_;
         //
         // The PrSelMgrs for the production and consumption cases of
         // proportionate routing owned by this PrCoord.

      WitPtrSched <WitRtSite, WitPrSelPt> myPrSelPts_;
         //
         // myPrSelPts_.myPtrAt (theRtSite, thePer) is the PrSelPt owned by this
         // PrCoord that represents theRtSite and thePer, if any;
         // otherwise NULL.

      WitPtrSched <WitRtCand, WitPrAltPt> myPrAltPts_;
         //
         // myPrAltPts_.myPtrAt (theRtCand, thePer) is the PrAltPt owned by this
         // PrCoord that represents deactivation of theRtCand and thePer, if
         // any; otherwise NULL.

      WitPerSglStack <WitRtCand> myRecoveryPairs_;
         //
         // In selection recovery mode, myRecoveryPairs_ is the set of all pairs
         // (RtCand, Period) at which the prop-rtg configuration is to be
         // restored to its initial state.
   };

#endif
