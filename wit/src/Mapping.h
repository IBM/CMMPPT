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

#ifndef MappingH
#define MappingH

//------------------------------------------------------------------------------
// Header file: "Mapping.h"
//
// Contains the declaration of class template Mapping <DomEl, RangeEl>.
//------------------------------------------------------------------------------

#include <CompMgr.h>
#include <Vector.h>

//------------------------------------------------------------------------------
// Class template Mapping <DomEl, RangeEl>
//
// An instance of class template Mapping <DomEl, RangeEl>
// represents a mapping whose domain is the set of all the instances of DomEl
// owned by a Problem into the set of all instances of RangeEl.
// For example, the declaration
//
//    WitMapping <WitPart, double> cost (myProblem);
//
// declares "cost" to be a mapping from the parts of myProblem into the set of
// all doubles. It is used by invoking the () operator, e.g.,
//
//   cost (thePart) = 17.9;
//
// This class is implemented using vectors, which are indexed by using the
// function: int DomEl::mappingIndex () const.
//
// This is a value-based collection class; its methods make heavy use of the
// copy constructor for RangeEl. However, instances of DomEl are accessed
// through pointers.
//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      class WitMapping
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMapping ();
         //
         // Constructs an empty Mapping (one that does not map any instances of
         // DomEl).

      WitMapping (WitProblem * theProblem, RangeEl initValue);
         //
         // Constructs a Mapping for those instances of DomEl that are owned
         // by theProblem.
         // Sets every element of this Mapping to initValue.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitMapping ();

      //------------------------------------------------------------------------
      // Public member operators.
      //------------------------------------------------------------------------

      inline RangeEl operator () (DomEl * theDomEl) const;
         //
         // Returns the value of this Mapping for theDomEl.
         // The returned value is not an lvalue.

      inline RangeEl & operator () (DomEl * theDomEl);
         //
         // Returns the value of this Mapping for theDomEl.
         // The returned value is an lvalue.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline int domainSize () const;
         //
         // Returns # DomEl instances mapped.

      void clear ();
         //
         // Resizes this Mapping to map 0 instances.

      void allocate (WitProblem * theProblem, RangeEl theRangeEl);
         //
         // Resizes this Mapping to cover the instances of DomEl
         // owned by theProblem. The value of this mapping for each instance
         // will be set to theRangeEl.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitMapping);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitVector <RangeEl> myVector_;
         //
         // Stores the member value for each instance of DomEl.
   };

//------------------------------------------------------------------------------
// Implementation of class template Mapping <DomEl, RangeEl>
//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      inline RangeEl WitMapping <DomEl, RangeEl>::operator () (
         DomEl * theDomEl) const
   {
   witAssert (theDomEl != NULL);

   return myVector_[theDomEl->mappingIndex ()];
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      inline RangeEl & WitMapping <DomEl, RangeEl>::operator () (
         DomEl * theDomEl)
   {
   witAssert (theDomEl != NULL);

   return myVector_[theDomEl->mappingIndex ()];
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      inline int WitMapping <DomEl, RangeEl>::domainSize () const
   {
   return myVector_.length ();
   }

#endif
