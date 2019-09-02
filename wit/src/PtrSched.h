//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef PtrSchedH
#define PtrSchedH

//------------------------------------------------------------------------------
// Header file: "PtrSched.h"
//
// Contains the declaration of class template PtrSched <DomEl, RangeEl>.
//------------------------------------------------------------------------------

#include <PtrVec.h>
#include <PtrTVec.h>
#include <CompMgr.h>

//------------------------------------------------------------------------------
// Template argument macros for class template PtrSched <DomEl, RangeEl>.
//------------------------------------------------------------------------------

#define PtrSchedArgDecs typename DomEl, typename RangeEl
#define PtrSchedArgs             DomEl,          RangeEl

//------------------------------------------------------------------------------
// Class template PtrSched <DomEl, RangeEl>
//
// A mapping from the instances of class DomEl (domain element) in a Problem to
// instances of class PtrTVec <RangeEl>.
//------------------------------------------------------------------------------

template <PtrSchedArgDecs>
      class WitPtrSched
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPtrSched ();
         //
         // Constructs an empty PtrSched (one that does not map any DomEls).

      WitPtrSched (WitProblem * theProblem);
         //
         // Constructs a PtrSched for those DomEls that are owned by theProblem.
         // Sets every element of this PtrSched to NULL.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPtrSched ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline int domainSize () const
         {
         return myPtrVec_.length ();
         }

      void allocate (WitProblem * theProblem);
         //
         // Resizes this PtrSched to cover the DomEls owned by theProblem. 
         // The value of this mapping for each DomEl will be set to NULL.

      void allocate1D (WitProblem * theProblem);
         //
         // Allocates the Vector of row Vector pointers, but not the row
         // Vectors.

      void allocate1D (int nInst);
         //
         // Same as above, but # instances is specified explicitly.

      void clear ();
         //
         // Deallocates this PtrSched.

      inline RangeEl & myElemAt (
            const DomEl * theDomEl,
            WitPeriod     thePer)
            const;
         //
         // Returns a ref to the RangeEl of this PtrSched for theDomEl and
         // thePer.

      inline RangeEl * & myPtrAt (const DomEl * theDomEl, WitPeriod thePer);
         //
         // Returns a ref to a ptr to the RangeEl of this PtrSched for theDomEl
         // and thePer.

      inline RangeEl * myPtrAt (const DomEl * theDomEl, WitPeriod thePer) const;
         //
         // Const version of the above.

      inline WitPtrTVec <RangeEl> & myPtrTVecAt (const DomEl * theDomEl);
         //
         // Returns a ref to the PtrTVec of this PtrSched for theDomEl.

      inline const WitPtrTVec <RangeEl> & myPtrTVecAt (
            const DomEl * theDomEl)
            const;
         //
         // Const version of the above.

      inline WitPtrTVec <RangeEl> & myPtrTVecAt (int theIdx)
         {
         return myPtrVec_.myElemAt (theIdx);
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPtrSched);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPtrVec <WitPtrTVec <RangeEl> > myPtrVec_;
         //
         // Stores the RangeEl ptr for each DomEl and each period.
   };

//------------------------------------------------------------------------------
// Inline implementations.
//------------------------------------------------------------------------------

template <PtrSchedArgDecs>
      inline RangeEl & WitPtrSched <PtrSchedArgs>::myElemAt (
         const DomEl * theDomEl, 
         WitPeriod     thePer)
         const
   {
   witAssert (theDomEl != NULL);

   witAssert (myPtrVec_.myElemAt (theDomEl->mappingIndex ())[thePer] != NULL);

   return   * myPtrVec_.myElemAt (theDomEl->mappingIndex ())[thePer];
   }

//------------------------------------------------------------------------------

template <PtrSchedArgDecs>
      inline RangeEl * & WitPtrSched <PtrSchedArgs>::myPtrAt (
         const DomEl * theDomEl, 
         WitPeriod     thePer)
   {
   witAssert (theDomEl != NULL);

   return myPtrVec_.myElemAt (theDomEl->mappingIndex ())[thePer];
   }

//------------------------------------------------------------------------------

template <PtrSchedArgDecs>
      inline RangeEl * WitPtrSched <PtrSchedArgs>::myPtrAt (
         const DomEl * theDomEl, 
         WitPeriod     thePer)
         const
   {
   witAssert (theDomEl != NULL);

   return myPtrVec_.myElemAt (theDomEl->mappingIndex ())[thePer];
   }

//------------------------------------------------------------------------------

template <PtrSchedArgDecs>
      inline WitPtrTVec <RangeEl> &
         WitPtrSched <PtrSchedArgs>::myPtrTVecAt (const DomEl * theDomEl)
   {
   witAssert (theDomEl != NULL);

   return myPtrVec_.myElemAt (theDomEl->mappingIndex ());
   }

//------------------------------------------------------------------------------

template <PtrSchedArgDecs>
      inline const WitPtrTVec <RangeEl> &
         WitPtrSched <PtrSchedArgs>::myPtrTVecAt (const DomEl * theDomEl) const
   {
   witAssert (theDomEl != NULL);

   return myPtrVec_.myElemAt (theDomEl->mappingIndex ());
   }

#endif
