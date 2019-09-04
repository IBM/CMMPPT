//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef PrCandH
#define PrCandH

//------------------------------------------------------------------------------
// Header file: "PrCand.h"
//
// Contains the declaration of class PrCand.
//------------------------------------------------------------------------------

#include <SelCand.h>
#include <ObjStack.h>

//------------------------------------------------------------------------------
// Class PrCand
//
// "Proportionate Routing Candidate"
// Represents a candidate for selection by proportionate routing.
// In this case, the selection candidate specifies the set of RtCands that are
// to be active for proportionate routing at some RtSite in some period.
//
// Class Hierarchy:
//
// SelCand
//    PrCand
//
// Implemented in PropRtg.C.
//------------------------------------------------------------------------------

class WitPrCand: public WitSelCand
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPrCand (
            WitPrCoord * thePrCoord,
            WitRtSite *  theRtSite,
            WitPeriod    thePer);
         //
         // Constructs a PrCand specifying the set of RtCands that are active
         // for proportionate routing at theRtSite in thePer, according to
         // thePrCoord.

   private:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitPrCand ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      virtual void prtID     ();
      virtual void getPrData (WitRtCandStack &);
         //
         // Overrides from class SelCand.

      noCopyCtorAssign (WitPrCand);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitObjStack <WitRtCand> actRtCands_;
         //
         // The set of RtCands that are to be active for proportionate routing
         // at the RtSite in the period at which this PrCand is selected.
   };

#endif
