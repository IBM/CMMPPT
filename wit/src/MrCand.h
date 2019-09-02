//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef MrCandH
#define MrCandH

//------------------------------------------------------------------------------
// Header file: "MrCand.h"
//
// Contains the declaration of class MrCand.
//------------------------------------------------------------------------------

#include <SelCand.h>

//------------------------------------------------------------------------------
// Class MrCand.
//
// "Multi-Route Candidate"
// Represents a candidate for selection by multi-route.
//
// Class Hierarchy:
//
// SelCand
//    MrCand
//
// Implemented in MultiRoute.C.
//------------------------------------------------------------------------------

class WitMrCand: public WitSelCand
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMrCand (WitRtCand *);

   private:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitMrCand ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      virtual void prtID     ();
      virtual void getMrData (WitRtCand * &);
         //
         // Overrides from class SelCand.

      noCopyCtorAssign (WitMrCand);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitRtCand * const myRtCand_;
         //
         // The RtCand for this MrCand.
   };

#endif
