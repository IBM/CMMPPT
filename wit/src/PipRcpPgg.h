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

#ifndef PipRcpPggH
#define PipRcpPggH

//------------------------------------------------------------------------------
// Header file: "PipRcpPgg.h"
//
// Contains the declaration of class template PipRcpPgg <SrcComp, RcpComp>.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <PtrSched.h>

//------------------------------------------------------------------------------
// Class template PipRcpPgg <SrcComp, RcpComp>
//
// "PIP Recipient Pegging"
// Stores the pegging of an attribute belonging to the SrcComp class and pegged
// to the RcpComp class for the Post-Implosion Pegging Subsystem.
//
// Implemented in PipPgg.C.
//------------------------------------------------------------------------------

template <typename SrcComp, typename RcpComp>
      class WitPipRcpPgg: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipRcpPgg (WitPipAttPgg <SrcComp> *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPipRcpPgg ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void appendIncPgg (
            SrcComp *                theSrcComp,
            WitPeriod                srcPer,
            WitPipIncPgg <RcpComp> * theIncPgg);
         //
         // For each triple (theRcpComp, rcpPer, pegVol) in theIncPgg, appends 
         // (theSrcComp, srcPer, pegVol) to the pegging list for theRcpComp in
         // rcpPer, removing the triple from theIncPgg.

      void consolidate ();
         //
         // For each RcpComp and period, merges each set of PegTrips that share
         // the same SrcComp and srcPer into a single PegTrip.

      void getPegTrips (
            RcpComp *                theRcpComp,
            WitPeriod                rcpPer,
            WitTripStack <SrcComp> & thePegTrips);
         //
         // Sets thePegTrips to the pegging list for theRcpComp in rcpPer in
         // forward order.

      int totTrips ();
         //
         // Counts and returns the total # of pegging triples owned by this
         // PipRcpPgg.

      void print ();
         //
         // Prints the attribute pegging stored by this PipRcpPgg.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void prtRcpPgg (RcpComp * theRcpComp, WitPeriod rcpPer);
         //
         // Prints the attribute pegging stored by this PipRcpPgg for theRcpComp
         // in rcpPer.

      noCopyCtorAssign (WitPipRcpPgg);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPipAttPgg <SrcComp> * const myPipAttPgg_;
         //
         // The PipAttPgg that owns this PipRcpPgg.

      WitPtrSched <RcpComp, WitTripStack <SrcComp> > myPegTrips_;
         //
         // myPegTrips_.myPtrAt (theRcpComp, rcpPer) is a ptr to the list of
         // pegging triples for theRcpComp in rcpPer for the pegged attribute,
         // or NULL, if there is none.
   };

#endif
