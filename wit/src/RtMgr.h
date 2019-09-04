//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef RtMgrH
#define RtMgrH

//------------------------------------------------------------------------------
// Header file: "RtMgr.h"
//
// Contains the declaration of class RtMgr.
//------------------------------------------------------------------------------

#include <List.h>
#include <PtrMap.h>
#include <Part.h>
#include <BomEntry.h>

//------------------------------------------------------------------------------
// Class RtMgr
//
// "Routing Element Manager"
// Manager of the Routing Element Subsystem.
//
// Class Hierarchy:
//
// ProbAssoc
//    RtMgr
//
// Implemented in Routing.C.
//------------------------------------------------------------------------------

class WitRtMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitRtMgr (WitSelector *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitRtMgr ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitList <WitRtSite> &, myRtSites)
      accessFunc (const WitList <WitRtCand> &, myRtCands)
      accessFunc (WitSelector *,               mySelector)

      inline WitRtSite * myRtSiteFor (WitPart * thePart)
         {
         return myRtSiteForPart_ (thePart);
         }

      inline WitRtSite * myRtSiteFor (WitBomEntry * theBomEnt)
         {
         return myRtSiteForBomEnt_ (theBomEnt);
         }

      inline WitRtCand * myRtCandFor (WitBillEntry * theBillEnt)
         {
         return myRtCandFor_ (theBillEnt);
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void buildRtSites ();
         //
         // Builds the RtSites for this RtMgr.

      void buildRtCands ();
         //
         // Builds the RtCands for this RtMgr.

      void print ();
         //
         // Prints the Routing Element Subsystem.

      noCopyCtorAssign (WitRtMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitList <WitRtSite> myRtSites_;
      WitList <WitRtCand> myRtCands_;
         //
         // The RtSites and RtCands owned by this RtMgr.

      WitPtrMap <WitPart,     WitRtSite> myRtSiteForPart_;
      WitPtrMap <WitBomEntry, WitRtSite> myRtSiteForBomEnt_;
         //
         // Mappings from Parts and BomEnts to the RtSites that represent them.

      WitPtrMap <WitBillEntry, WitRtCand> myRtCandFor_;
         //
         // Mappings from BillEnts to the RtCands that represent them.

      WitSelector * const mySelector_;
         //
         // The Selector that owns this RtMgr.
   };

#endif
