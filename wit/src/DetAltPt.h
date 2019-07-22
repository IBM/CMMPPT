//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Watson Implosion Technology
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
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
