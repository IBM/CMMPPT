//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef BelowH
#define BelowH

//------------------------------------------------------------------------------
// Header file: "Below.h"
//
// Contains the declaration of class BelowLister.
//------------------------------------------------------------------------------

#include <Mapping.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// Class BelowLister
//
// Responsible for building the below list for each Part, as part of
// preprocessing.
//
// Implemented in PreHelp.C.
//------------------------------------------------------------------------------

class WitBelowLister: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitBelowLister (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitBelowLister ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void buildBelow ();
         //
         // Builds the below list for each Part in myProblem ().

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitBelowLister);

      void buildBelow (WitPart * currentPart);
         //
         // Builds the below list for currentPart.

      void mergeBelow (
            WitConsEntry *          theConsEnt,
            WitObjStack <WitPart> & theParts);
         //
         // Merges the below list for theConsEnt's consumed Part into theParts.

      static bool compareParts (WitPart *, WitPart *);
         //
         // Compare function for sorting the below list of a Part.
         // The sort is by # Parts before the Part.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMapping <WitPart, bool> isBelowCurrent_;
         //
         // isBelowCurrent_ (thePart) is true iff thePart is in the below list
         // of the current Part.
   };

#endif
