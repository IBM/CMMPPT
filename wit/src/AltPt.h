//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef AltPtH
#define AltPtH

//------------------------------------------------------------------------------
// Header file: "AltPt.h"
//
// Contains the declaration of class AltPt.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class AltPt
//
// "Selection Alteration Point"
// Represents a point in the problem at which selection configuration can be
// altered.
//
// Class Hierarchy:
//
// AltPt
//    DetAltPt
//    PrAltPt
//
// Implemented in Selection.C.
//------------------------------------------------------------------------------

class WitAltPt
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitAltPt ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual bool hasResAlt () = 0;
         //
         // Returns true, iff the selection configuration can be altered at this
         // AltPt in shortage-resolving manner.

      virtual void alterSelection () = 0;
         //
         // Alters the selection configuration at this AltPt.

      virtual void storeRecoveryPt () = 0;
         //
         // Records the fact that the selection configuration is being altered
         // at this AltPt.
         // Used for selection recovery.

      virtual void tempAlterSel () = 0;
         //
         // Temporarily alters the selection configuration at this AltPt.

      virtual void cancelTempAlt () = 0;
         //
         // Undoes tempAlterSel ().

      virtual WitSelPt * mySelPt () = 0;
         //
         // Returns the SelPt whose selection would be altered at the point in
         // the problem represented by this AltPt.

      virtual void printAlteration () = 0;
         //
         // Prints the alteration of the selection configuration at this AltPt.

      void setPending (bool);
         //
         // "Set" function for pending_.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, pending)

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAltPt ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitAltPt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      bool pending_;
         //
         // true, iff the configuration is to be altered at this AltPt.
   };

#endif
