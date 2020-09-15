//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef PipAttPggH
#define PipAttPggH

//------------------------------------------------------------------------------
// Header file: "PipAttPgg.h"
//
// Contains the declaration of template class PipAttPgg <SrcComp>.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <PtrSched.h>
#include <Str.h>

//------------------------------------------------------------------------------
// Class template PipAttPgg <SrcComp>
//
// "PIP Attribute Pegging"
// Stores the peggings of an attribute belonging to the SrcComp class for the
// Post-Implosion Pegging Subsystem.
//
// Implemented in PipPgg.C.
//------------------------------------------------------------------------------

template <typename SrcComp> 
      class WitPipAttPgg: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipAttPgg (const char * theAttName, WitPipPgg *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPipAttPgg ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void appendIncPgg (SrcComp * theSrcComp, WitPeriod srcPer);
         //
         // Calls appendIncPgg (...) for the two PipRcpPggs.

      void getPgg (
            WitDemand *              theDemand,
            WitPeriod                shipPer,
            WitTripStack <SrcComp> & thePegTrips);
         //
         // Sets thePegTrips to the pegging list to theDemand in shipPer in
         // forward order.

      void getPgg (
            WitOperation *           theOpn,
            WitPeriod                execPer,
            WitTripStack <SrcComp> & thePegTrips);
         //
         // Sets thePegTrips to the pegging list to theOpn in execPer in forward
         // order.

      void print ();
         //
         // Prints the attribute pegging stored by this PipAttPgg.

      void consolidate ();
         //
         // Consolidates the pegging of the attribute to both recipient cases.

      int totTrips ();
         //
         // Counts and returns the total # of pegging triples owned by the
         // PipRcpPggs of this PipAttPgg.

      void getAllSrcComps (WitList <SrcComp> & theSrcCompList);
         //
         // On return, theSrcCompList will be a List of all SrcComps in
         // myProblem ().

      void prtSrcComp (SrcComp * theSrcComp);
         //
         // Prints theSrcComp, or blank, if it's NULL.

      static const char * getSrcCompLabel ();
         //
         // Returns a c-string identifying the SrcComp class.

      static const char * getSrcCompHyphens ();
         //
         // Returns a c-string of hyphens the length of getSrcCompLabel ().

      accessFunc  (const char *,                        myAttName)
      accessFunc2 (WitPipRcpPgg <SrcComp, WitDemand> *, myPggToDems)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPipAttPgg);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const char * const myAttName_;
         //
         // The name of the pegged attribute.

      WitPipPgg * const myPipPgg_;
         //
         // The PipPgg that owns this PipAttPgg.

      WitPipRcpPgg <SrcComp, WitDemand>    * myPggToDems_;
      WitPipRcpPgg <SrcComp, WitOperation> * myPggToOpns_;
         //
         // Stores the pegging of the attribute to Demands.
         // Stores the pegging of the attribute to Operations.
   };

#endif
