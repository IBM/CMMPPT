//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef PipSeqMgrH
#define PipSeqMgrH

//------------------------------------------------------------------------------
// Header file: "PipSeqMgr.h"
//
// Contains the declaration of class PipSeqMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <TripStack.h>
 
//------------------------------------------------------------------------------
// Class PipSeqMgr
//
// "Post-Implosion Pegging Sequence Manager"
// Owns and manages the shipment sequence for the Post-Implosion Pegging
// Subsystem.
//
// Implemented in PipMgr.C.
//------------------------------------------------------------------------------

class WitPipSeqMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipSeqMgr (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPipSeqMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void clear ();
         //
         // Clears the shipment sequence.

      void append (
            WitDemand * theDemand,
            WitPeriod   shipPer,
            double      incShipVol);
         //
         // Appends the args to the end of the shipment sequence.

      void get (WitDemTripStack & theShipSeq);
         //
         // Sets theShipSeq to the (forward) shipment sequence.

      void reduce ();
         //
         // Makes the shipment sequence feasible for the shipment schedule, by
         // reducing the incShipVols, as needed.

      void display ();
         //
         // Displays the shipment sequence.

      void copy (WitPipSeqMgr * theSeqMgr);
         //
         // Copies the contents of theSeqMgr into this SeqMgr.

      void writeData ();
         //
         // Writes the shipment sequence in the input data file format.

      void setShipPerForRead (WitPeriod);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitPeriod, shipPerForRead)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPipSeqMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitDemTripStack revShipSeq_;
         //       
         // The shipment sequence for PIP, stored in reverse order.
         // This is the ordered list of triples
         // (theDemand, shipPer, incShipVol) such that the PIP algorithm is to
         // increase theDemand->shipVol ()[shipPer] by incShipVol, and peg the
         // resulting resource utilization to theDemand in shipPer.

      WitPeriod shipPerForRead_;
         //
         // The shipment period to be used by the DataReader when appending to
         // the shipment sequence.
   };

#endif
