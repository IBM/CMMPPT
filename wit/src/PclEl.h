//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef PclElH
#define PclElH

//------------------------------------------------------------------------------
// Header file: "PclEl.h"
//
// Contains the declaration of class PclEl.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class PclEl
//
// "Pegged Critical List Element".
//------------------------------------------------------------------------------

class WitPclEl
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPclEl (WitPart *, WitPeriod, WitDemand *, WitPeriod);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPclEl ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitPart *,   critPart)
      accessFunc (WitPeriod,   critPer)
      accessFunc (WitDemand *, myDemand)
      accessFunc (WitPeriod,   myShipPer)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPclEl);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPart * const critPart_;
         //
         // The critical Part for this PclEl.

      const WitPeriod critPer_;
         //
         // The critical period for this PclEl.

      WitDemand * const myDemand_;
         //
         // The Demand for this PclEl.

      const WitPeriod myShipPer_;
         //
         // The shipment period for this PclEl.
   };

#endif
