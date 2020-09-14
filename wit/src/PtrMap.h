//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef PtrMapH
#define PtrMapH

//------------------------------------------------------------------------------
// Header file: "PtrMap.h"
//
// Contains the declaration of class template PtrMap <DomEl, RangeEl>.
//------------------------------------------------------------------------------

#include <PtrVec.h>

//------------------------------------------------------------------------------
// Template argument macros for class template PtrMap <DomEl, RangeEl>.
//------------------------------------------------------------------------------

#define PtrMapArgDecs typename DomEl, typename RangeEl
#define PtrMapArgs             DomEl,          RangeEl

//------------------------------------------------------------------------------
// Class template PtrMap <DomEl, RangeEl>
//
// A mapping from the instances of class DomEl (domain element) in a Problem to
// pointers to instances of class RangeEl (range element).
//------------------------------------------------------------------------------

template <PtrMapArgDecs>
      class WitPtrMap
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPtrMap ();
         //
         // Constructs an empty PtrMap (one that does not map any DomEls).

      WitPtrMap (WitProblem * theProblem);
         //
         // Constructs a PtrMap for those DomEls that are owned by theProblem.
         // Sets every element of this PtrMap to NULL.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPtrMap ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void allocate (WitProblem * theProblem);
         //
         // Resizes this PtrMap to cover the DomEls owned by theProblem. 
         // The value of this mapping for each DomEl will be set to NULL.

      void allocate (int nInst);
         //
         // Same as above, but # instances is specified explicitly.

      void clear ();
         //
         // Resizes this PtrMap to cover no DomEls.

      inline RangeEl * & operator () (DomEl * theDomEl);
         //
         // Returns a reference to the value of this PtrMap for theDomEl.

      inline RangeEl * operator () (DomEl * theDomEl) const;
         //
         // Const version.

      inline RangeEl & myElemAt (DomEl * theDomEl) const;
         //
         // Returns a ref to the RangeEl of this PtrMap for theDomEl.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPtrMap);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPtrVec <RangeEl> myPtrVec_;
         //
         // Stores the member value for each DomEl.
   };

//------------------------------------------------------------------------------
// Implementation of inlines.
//------------------------------------------------------------------------------

template <PtrMapArgDecs>
      inline RangeEl * & WitPtrMap <PtrMapArgs>::operator () (DomEl * theDomEl)
   {
   witAssert (theDomEl != NULL);

   return myPtrVec_[theDomEl->mappingIndex ()];
   }

//------------------------------------------------------------------------------

template <PtrMapArgDecs>
      inline RangeEl * WitPtrMap <PtrMapArgs>::operator () (
         DomEl * theDomEl)
         const
   {
   witAssert (theDomEl != NULL);

   return myPtrVec_[theDomEl->mappingIndex ()];
   }

//------------------------------------------------------------------------------

template <PtrMapArgDecs>
      inline RangeEl & WitPtrMap <PtrMapArgs>::myElemAt (DomEl * theDomEl) const
   {
   witAssert (theDomEl != NULL);

   witAssert (myPtrVec_[theDomEl->mappingIndex ()] != NULL);

   return   * myPtrVec_[theDomEl->mappingIndex ()];
   }

#endif
