//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef VariantH
#define VariantH

//------------------------------------------------------------------------------
// Header file: "Variant.h"
//
// Contains the declaration of class Variant.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class Variant.
//
// Responsible for identifying which variant of the implosion problem is
// under consideration and determining the behavior that depends on this.
// Implemented in Misc.C.
//------------------------------------------------------------------------------

class WitVariant
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitVariant (WitProblem * theProblem);
         //
         // theProblem is the Problem to which this Variant belongs.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitVariant ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitProblem *, myProblem)
      accessFunc (WitMsgFragID, myFragID)
      accessFunc (bool,         internalSoln)
      accessFunc (bool,         compsSecScheds)
      accessFunc (bool,         respectsLotSizes)
      accessFunc (bool,         respectsOptCons)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void configure ();
         //
         // Sets the data members according to which of myProblem's Variants
         // this one is.

      void becomesCurrent ();
         //
         // Sets myProblem ()->currentVariant () to this Variant.

      bool isCurrent ();
         //
         // Returns true, iff this Variant is myProblem ()->currentVariant ().

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitVariant);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitProblem * const myProblem_;
         //
         // The Problem to which this Variant belongs.

      WitMsgFragID myFragID_;
         //
         // The ID for the Msg Frag that identifies this Variant.

      bool internalSoln_;
         //
         // true, iff the implosion solution for this Variant is computed by WIT

      bool compsSecScheds_;
         //
         // true, iff the implosion solution for this Variant is computed by a
         // method that computes secondary schedules: scrapVol, stockVol,
         // cumShipVol.

      bool respectsLotSizes_;
         //
         // true, iff this Variant considers lot sizes.

      bool respectsOptCons_;
         //
         // true, iff this Variant respects all of the contraints for optimizing
         // implosion.
   };

#endif
