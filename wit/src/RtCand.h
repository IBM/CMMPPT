//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef RtCandH
#define RtCandH

//------------------------------------------------------------------------------
// Header file: "RtCand.h"
//
// Contains the declaration of class RtCand.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class RtCand
//
// "Routing Candidate"
//
// Represents a BopEntry or ConsEntry in its role as a candidate for routing.
//
// In comments, the BopEntry ot ConsEntry represented by this RtCand will be
// referred to as "the represented candidate".
//
// Class Hierarchy:
//
// ProbAssoc
//    RtCand
//       ProdRtCand
//       ConsRtCand
//
// Implemented in Routing.C.
//------------------------------------------------------------------------------

class WitRtCand: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitRtCand ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual bool usedByMr () = 0;
         //
         // Returns true, iff this RtCand would be used by Multi-Route.

      virtual void printID (int labelCol, int dataCol) = 0;
         //
         // Prints info identifying the represented candidate.
         // The label will be printed in column labelCol.
         // If dataCol > 0, the data will be printed in that column, if
         // possible.

      virtual bool isEligible (WitPeriod thePer) = 0;
         //
         // Returns true, iff the represented candidate is eligible to be
         // selected in thePer.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitRtSite *,    myRtSite)
      accessFunc (WitBillEntry *, myBillEnt)

      WitBopEntry *  myBopEnt  ();
      WitConsEntry * myConsEnt ();

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      decMappingIndex (WitRtCand)

      static int nInstances (WitProblem *);

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitRtCand (WitBillEntry *, WitRtSite *, WitRtMgr *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitRtCand);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const int mappingIndex_;
         //
         // Mapping index.

      WitBillEntry * const myBillEnt_;
         //
         // The BopEntry or ConsEntry represented by this RtCand.

      WitRtSite * const myRtSite_;
         //
         // The RtSite for this RtCand.
   };

#endif
