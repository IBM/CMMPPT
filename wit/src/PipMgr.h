//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef PipMgrH
#define PipMgrH

//------------------------------------------------------------------------------
// Header file: "PipMgr.h"
//
// Contains the declaration of class PipMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class PipMgr
// "Post-Implosion Pegging Subsystem Manager"
// Owner, Manager and Public Interface of the Post-Implosion Pegging Subsystem.
//------------------------------------------------------------------------------

class WitPipMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipMgr (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPipMgr ();

      //------------------------------------------------------------------------
      // Public member functions relating to the shipment sequence.
      //------------------------------------------------------------------------

      void clearShipSeq ();
         //
         // Clears the shipment sequence.

      void appendToShipSeq (
            WitDemand * theDemand,
            WitPeriod   shipPer,
            double      incShipVol);
         //
         // Appends the args to the end of the shipment sequence.

      void getShipSeq (WitDemTripStack & theShipSeq);
         //
         // Sets theShipSeq to the (forward) shipment sequence.

      void displayShipSeq ();
         //
         // Displays the shipment sequence.

      void copyShipSeq (WitPipMgr * thePipMgr);
         //
         // Copies the shipment sequence from thePipMgr into this PipMgr.

      void writeShipSeqData ();
         //
         // Writes the shipment sequence in the input data file format.

      void setShipSeqShipPer (WitPeriod);

      WitPeriod shipSeqShipPer ();
         //
         // Returns the shipment period to be used by readData when appending to
         // the shipment sequence.

      //------------------------------------------------------------------------
      // Public member functions relating to the pegging.
      //------------------------------------------------------------------------

      void buildPegging ();
         //
         // Builds the pegging.

      void clearPegging ();
         //
         // Clears the pegging, if any.

      void printPegging ();
         //
         // Prints the pegging, if any.

      inline bool peggingExists ()
         {
         return (myPgg_ != NULL);
         }

      //------------------------------------------------------------------------
      // get---Pgg functions.
      // Each of these functions sets its last arg to a pegging to theDemand and
      // shipPer, where the peggings are for:
      //
      //    execVol
      //    coExecVol
      //    subVol
      //    supplyVol
      //    prodVol
      //    consVol
      //    sideVol
      //------------------------------------------------------------------------

      void getExecVolPgg (
            WitDemand *          theDemand,
            WitPeriod            shipPer,
            WitOpnTripStack &    execVolTrips);

      void getCoExecVolPgg (
            WitDemand *          theDemand,
            WitPeriod            shipPer,
            WitBopEntTripStack & coExecVolTrips);

      void getSubVolPgg (
            WitDemand *          theDemand,
            WitPeriod            shipPer,
            WitSubTripStack &    subVolTrips);

      void getSupplyVolPgg (
            WitDemand *          theDemand,
            WitPeriod            shipPer,
            WitPartTripStack &   supplyVolTrips);

      void getProdVolPgg (
            WitDemand *          theDemand,
            WitPeriod            shipPer,
            WitPartTripStack &   prodVolTrips);

      void getConsVolPgg (
            WitDemand *          theDemand,
            WitPeriod            shipPer,
            WitPartTripStack &   consVolTrips);

      void getSideVolPgg (
            WitDemand *          theDemand,
            WitPeriod            shipPer,
            WitPartTripStack &   sideVolTrips);

      //------------------------------------------------------------------------
      // get---Pgg functions.
      // Each of these functions sets its last arg to a pegging to theOpn and
      // execPer, where the peggings are for:
      //
      //    execVol
      //    coExecVol
      //    subVol
      //    supplyVol
      //    prodVol
      //    consVol
      //    sideVol
      //------------------------------------------------------------------------

      void getExecVolPgg (
            WitOperation *       theOpn,
            WitPeriod            execPer,
            WitOpnTripStack &    execVolTrips);

      void getCoExecVolPgg (
            WitOperation *       theOpn,
            WitPeriod            execPer,
            WitBopEntTripStack & coExecVolTrips);

      void getSubVolPgg (
            WitOperation *       theOpn,
            WitPeriod            execPer,
            WitSubTripStack &    subVolTrips);

      void getSupplyVolPgg (
            WitOperation *       theOpn,
            WitPeriod            execPer,
            WitPartTripStack &   supplyVolTrips);

      void getProdVolPgg (
            WitOperation *       theOpn,
            WitPeriod            execPer,
            WitPartTripStack &   prodVolTrips);

      void getConsVolPgg (
            WitOperation *       theOpn,
            WitPeriod            execPer,
            WitPartTripStack &   consVolTrips);

      void getSideVolPgg (
            WitOperation *       theOpn,
            WitPeriod            execPer,
            WitPartTripStack &   sideVolTrips);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void checkRestrictions ();
         //
         // Verifies that the various data restrictions for PIP are met.

      accessNonNull (WitPipPgg *, myPgg)

      noCopyCtorAssign (WitPipMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPipSeqMgr * mySeqMgr_;
         //
         // The PipSeqMgr owned by this PipMgr.

      WitPipPgg * myPgg_;
         //
         // The PipPgg owned by this PipMgr when a pegging exists;
         // otherwise NULL.
   };

#endif
