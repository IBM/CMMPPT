//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef PrAltPtH
#define PrAltPtH

//------------------------------------------------------------------------------
// Header file: "PrAltPt.h"
//
// Contains the declaration of class PrAltPt.
//------------------------------------------------------------------------------

#include <AltPt.h>

//------------------------------------------------------------------------------
// Class PrAltPt
//
// "Proportionate Routing Alteration Point"
// A PrAltPt is an AltPt at which the proportionate routing configuration can be
// altered. In this case, the alteration is the deactivation of a particular
// RtCand in a particular period for proportionate routing purposes.
//
// Class Hierarchy:
//
// AltPt
//    PrAltPt
//
// Implemented in PropRtg.C.
//------------------------------------------------------------------------------

class WitPrAltPt: public WitAltPt
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPrAltPt (WitPrSelPt *, WitRtCand *, WitPrCoord *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitPrAltPt ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void setStatus (bool activeVal, bool allowedVal);
         //
         // Sets active_ and allowed_.

      void recoverInitState ();
         //
         // Restores the Prop-Rtg configuration at this PrAltPt to its initial
         // state.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitPrSelPt *, myPrSelPt)
      accessFunc (bool,         active)
      accessFunc (bool,         allowed)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      virtual bool       hasResAlt       ();
      virtual void       alterSelection  ();
      virtual void       storeRecoveryPt ();
      virtual void       tempAlterSel    ();
      virtual void       cancelTempAlt   ();
      virtual WitSelPt * mySelPt         ();
      virtual void       printAlteration ();
         //
         // Overrides from class AltPt.

      void printRecovery ();
         //
         // Prints the restoration of the Prop-Rtg configuration at this PrAltPt
         // to its initial state.

      noCopyCtorAssign (WitPrAltPt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPrSelPt * const myPrSelPt_;
         //
         // The PrSelPt for this PrAltPt.

      WitRtCand * const myRtCand_;
         //
         // The RtCand for this PrAltPt.

      WitPrCoord * const myPrCoord_;
         //
         // The PrCoord that owns this PrAltPt.

      bool active_;
         //
         // true, iff proportionate routing is currently active for myRtCand_ in
         // the period for this PrAltPt.

      bool allowed_;
         //
         // true, iff proportionate routing is currently allowed for myRtCand_
         // in the period for this PrAltPt.
         // If active_ is true, then allowed_ must be true.
         // When not in TBPR mode, active_ is true iff allowed_ is true.
   };

#endif
