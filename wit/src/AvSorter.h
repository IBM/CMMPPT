//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef AvSorterH
#define AvSorterH

//------------------------------------------------------------------------------
// Header file: "AvSorter.h"
//
// Contains the declaration of class template AvSorter.
//------------------------------------------------------------------------------

#include <PtrVec.h>

//------------------------------------------------------------------------------
// Class template AvSorter <SortEntry>
//
// Responsible for sorting instances of the SortEntry class (either BopEntry or
// SubEntry) by their aversion.
//
// Implemented in PreHelp.C.
//------------------------------------------------------------------------------

template <typename SortEntry> 
      class WitAvSorter
   {
   public:

      //-----------------------------------------------------------------------
      // Public member functions.
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitAvSorter (int nEntries);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      ~WitAvSorter ();

      //-----------------------------------------------------------------------
      // Other public member functions.
      //-----------------------------------------------------------------------

      void sort (WitList <SortEntry> & theList);
         //
         // Sorts the elements of theList according to the aversion function.

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      bool inOrder (const WitList <SortEntry> & theList);
         //
         // Returns true, iff the elements of theList are ordered consistently
         // with the aversion function.

      static bool compareAversions (SortEntry *, SortEntry *);
         //
         // Sort compare function based on aversion.

      static double aversion (SortEntry *);
         //
         // Returns the appropriate aversion.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitPtrVec <SortEntry> workingVec_;
         //
         // Working PtrVec.
   };

#endif
