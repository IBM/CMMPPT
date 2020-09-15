//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef DetAltPtH
#define DetAltPtH

//------------------------------------------------------------------------------
// Header file: "DetAltPt.h"
//
// Contains the declaration of class DetAltPt.
//------------------------------------------------------------------------------

#include <AltPt.h>

//------------------------------------------------------------------------------
// Class DetAltPt
//
// "Determined Alteration Point"
// A DetAltPt is an AltPt at which the selection configuration can be altered in
// a manner that is uniquely determined by the corresponding SelPt.
//
// Class Hierarchy:
//
// AltPt
//    DetAltPt
//
// Implemented in Selection.C.
//------------------------------------------------------------------------------

class WitDetAltPt: public WitAltPt
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitDetAltPt (WitDetSelPt *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitDetAltPt ();

   private:

      //------------------------------------------------------------------------
      // Other private member functions.
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

      noCopyCtorAssign (WitDetAltPt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitDetSelPt * const myDetSelPt_;
         //
         // The DetSelPt that owns this DetAltPt.
   };

#endif
