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

#ifndef VectorH
#define VectorH

//------------------------------------------------------------------------------
// Header file: "Vector.h"
//
// Contains the declaration of class template Vector <Elem>.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class template Vector <Elem>
//
// A Vector whose elements are instances of Elem.
// The elements are stored and retrieved by value.
//
// Implemented in Vecs.C.
//------------------------------------------------------------------------------

template <typename Elem> 
      class WitVector
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitVector ();
         //
         // Constructs an instance with 0 elements.

      WitVector (int nElems, Elem initValue);

         // Constructs an instance with nElems elements, each of which is
         // initialized to initValue.

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      ~WitVector ();

      //-----------------------------------------------------------------------
      // Other public member functions.
      //-----------------------------------------------------------------------

      void operator = (const WitVector &);
      void operator = (Elem);
      void operator = (const Elem *);
         //
         // Assignment operators.

      void copyInto (Elem * theCVec) const;
         //
         // Copies the elements of this Vector into the corresponding
         // elements of theCVec.
         // The length of theCVec is assumed match that of this Vector, but
         // this assumption cannot be verified.

      void clear ();
         //
         // Resizes this Vector to 0 elements.

      void resize (int nElems, Elem theScalar);
         //
         // Resizes this Vector to nElems elements.
         // The elements are all set to theScalar.

      void resize (int nElems);
         //
         // Resizes this WitVector to nElems elements.
         // The elements have undefined value.

      bool operator == (Elem theScalar) const;
         //
         // Returns true iff every element of this Vector == theScalar.

      inline bool isAllocated () const;

      inline Elem operator [] (int theIdx) const;
         //
         // Returns the indexed element of this Vector.
         // Bounds checking is performed (by WIT), iff the compile is in
         // DEBUG  mode.
         // Returned value is not an lvalue.

      inline Elem & operator [] (int theIdx);
         //
         // Same as the preceeding operator [], except that this Vector is
         // non-const and the returned value is an lvalue.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const Elem *, myCVec)
      accessFunc (int,          length)

      inline Elem * myCVecForUpdate ();
         //
         // Same as myCVec (), but returns non-const.
         // Use with discretion.

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      void checkBounds (int theIdx) const;
         //
         // Verifies that theIdx is within the bounds of this Vector.

      noCopyCtor (WitVector);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      Elem * myCVec_;
         //
         // The C style vector which stores the data for this Vector.

      int length_;
         //
         // The # of elements stored by this Vector.
   };

//------------------------------------------------------------------------------
// Implementation of inline functions of class template Vector <Elem>
//------------------------------------------------------------------------------

template <typename Elem> 
      inline bool WitVector <Elem>::isAllocated () const
   {
   return (length_ > 0);
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      inline Elem WitVector <Elem>::operator [] (int theIdx) const
   {
   ifDebug (checkBounds (theIdx));

   return myCVec_[theIdx];
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      inline Elem & WitVector <Elem>::operator [] (int theIdx)
   {
   ifDebug (checkBounds (theIdx));

   return myCVec_[theIdx];
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      Elem * WitVector <Elem>::myCVecForUpdate ()
   {
   return myCVec_;
   }

//------------------------------------------------------------------------------
// NonClass functions with arguments belonging to specific Vector classes.
//------------------------------------------------------------------------------

namespace WitNonClass
   {
   bool operator >= (const WitVector <double> & theVector, double theScalar);
      //
      // Returns true, iff each element of theVector >= theScalar.

   template <typename Elem> 
         void copy (Elem * dstCVec, const Elem * srcCVec, int nElems);
      //
      // Copies the elements of srcCVec into dstCVec.
      // nElems is the # of elements in each vector.

   template <typename Elem> 
         bool hasSingleValue (const Elem * theCVec, int nElems);
      //
      // Returns true, iff all the elements of theCVec have the same value.
      // theCVec is assumed to have length == nElems.
   };

#endif
