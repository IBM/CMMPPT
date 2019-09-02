//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef DetSelPtH
#define DetSelPtH

//------------------------------------------------------------------------------
// Header file: "DetSelPt.h"
//
// Contains the declaration of class DetSelPt.
//------------------------------------------------------------------------------

#include <SelPt.h>

//------------------------------------------------------------------------------
// Class DetSelPt
//
// "Determined Selection Point"
// A DetSelPt is a SelPt at which any alteration to the selection configuration
// is uniquely determined by the SelPt itself.
//
// Class Hierarchy:
//
// SelPt
//    DetSelPt
//       MrPt
//       BaPt
//       MePt
//       SsrPt
//
// Implemented in Selection.C.
//------------------------------------------------------------------------------

class WitDetSelPt: public WitSelPt
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      virtual bool hasResAlt () = 0;
         //
         // Returns true, iff this DetSelPt currently has a selection and an 
         // shortage-resolving alternative to the current selection exists.

      virtual void alterSelection () = 0;
         //
         // Alters the selection for this DetSelPt.

      virtual void storeRecoveryPt () = 0;
         //
         // Records the fact that the selection configuration is being altered
         // at this DetSelPt.
         // Used for selection recovery.

      virtual void tempAlterSel () = 0;
         //
         // Temporarily alters the selection for this DetSelPt.

      virtual void cancelTempAlt () = 0;
         //
         // Undoes tempAlterSel ().

      virtual void printAlteration () = 0;
         //
         // Prints the alteration of the selection configuration at this
         // DetAltPt.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitDetAltPt *, myDetAltPt)

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitDetSelPt ();

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitDetSelPt ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitDetSelPt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitDetAltPt * myDetAltPt_;
         //
         // The DetAltPt owned by this DetSelPt.
         // myDetAltPt_ is the DetAltPt at which the selection configuration can
         // be altered in a manner that is uniquely determined by this DetSelPt.
   };

#endif
