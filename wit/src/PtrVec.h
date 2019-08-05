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

#ifndef PtrVecH
#define PtrVecH

//------------------------------------------------------------------------------
// Header file: "PtrVec.h"
//
// Contains the declaration of class template PtrVec         <Elem>.
// Contains the declaration of class template PtrVecItr      <Elem>.
// Contains the declaration of class template PtrVecCompFtor <Elem>.
//------------------------------------------------------------------------------

#include <Vector.h>

#include <algorithm>

template <typename Elem> class WitPtrVecCompFtor;

//------------------------------------------------------------------------------
// Class template PtrVec <Elem>
//
// A vector of pointers to Elem.
// This template is implemented as a typed wrapper for class Vector <void *>.
// See class Vector <void *> for comments on the member functions.
//
// All implementation is inline.
//------------------------------------------------------------------------------

template <typename Elem> 
      class WitPtrVec
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      inline WitPtrVec ():
            myVector_ ()
         {
         }

      inline WitPtrVec (int nElems):
            myVector_ (nElems, NULL)
         {
         }

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      inline ~WitPtrVec ()
         {
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline void sort (bool (* theCompFunc) (Elem *, Elem *))
            //
            // Sorts this PtrVec, using theCompFunc as the comparison function.
         {
         sort (theCompFunc, length ());
         }

      inline void sort (
            bool (* theCompFunc) (Elem *, Elem *),
            int     nElemsToSort);
         //
         // Sorts the first nElemsToSort of this PtrVec, using theCompFunc as
         // the comparison function.

      inline void resize (int nElems)
         {
         myVector_.resize (nElems, NULL);
         }

      inline void clear ()
         {
         myVector_.clear ();
         }

      inline void operator = (const WitPtrVec & thePtrVec)
         {
         myVector_ = thePtrVec.myVector_;
         }

      inline int length () const
         {
         return myVector_.length ();
         }

      inline Elem & myElemAt (int theIndex) const
         {
         witAssert ((* this)[theIndex] != NULL);

         return   * (* this)[theIndex];
         }

      inline Elem * operator [] (int theIndex) const
         {
         return reinterpret_cast <Elem *> (myVector_[theIndex]) ;
         }

      inline Elem * & operator [] (int theIndex)
         {
         return reinterpret_cast <Elem * &> (myVector_[theIndex]) ;
         }

      inline void attachItr (WitPtrVecItr <Elem> & theItr) const;
         //
         // Attaches theItr to this PtrVec.

      inline WitVector <void *> & myVector ()
         {
         return myVector_;
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtor (WitPtrVec);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitVector <void *> myVector_;
         //
         // The Vector that stores the data for this PtrVec.
   };

//------------------------------------------------------------------------------
// Class template PtrVecItr <Elem>
//
// Responsible for iterating through the elements of a PtrVec <Elem>.
//
// All implementation is inline.
//------------------------------------------------------------------------------

template <typename Elem> 
      class WitPtrVecItr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      inline WitPtrVecItr ():

            myPtrVecPtr_ (NULL),
            myIdx_       (-1)
         {
         }

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      inline ~WitPtrVecItr ()
         {
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline void attachTo (const WitPtrVec <Elem> & thePtrVec);
         //
         // Attaches this PtrVecItr to thePtrVec.

      inline bool advance (Elem * & theElem);
         //
         // Advances this PtrVecItr.
         // If, after the advance, this PtrVecItr still refers to a valid Elem
         // in myPtrVecPtr_, theElem is set to it and the function returns true.
         // Otherwise, this PtrVecItr is detached from myPtrVecPtr_ and the
         // function returns false.

      accessFunc (int, myIdx);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPtrVecItr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const WitPtrVec <Elem> * myPtrVecPtr_;
         //
         // Pointer to the PtrVec through which this PtrVecItr iterates.

      int myIdx_;
         //
         // The current index into * myPtrVecPtr_.
   };

//------------------------------------------------------------------------------
// Class template PtrVecCompFtor <Elem>
//
// "Pointer Vector Comparison Functor"
//
// A comparison functor to be used for sorting instances of class template
// PtrVec <Elem>, using a comparison function specified as a global function.
//
// All implementation is inline.
//------------------------------------------------------------------------------

template <typename Elem>
      class WitPtrVecCompFtor
   {
   public:

      //------------------------------------------------------------------------
      // Initial constructor function.
      //------------------------------------------------------------------------

      inline WitPtrVecCompFtor (bool (* theCompFunc) (Elem *, Elem *)):

            myCompFunc_ (theCompFunc)
         {
         }

      //------------------------------------------------------------------------
      // Copy constructor function.
      //------------------------------------------------------------------------

      inline WitPtrVecCompFtor (const WitPtrVecCompFtor & theFtor):

            myCompFunc_ (theFtor.myCompFunc_)
         {
         }

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPtrVecCompFtor ()
         {
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline bool operator () (void * theVoid1, void * theVoid2);
         //
         // The comparison operator.

   private:

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      bool (* const myCompFunc_) (Elem *, Elem *);
         //
         // The sorting comparison function for this PtrVecCompFtor.
   };

//------------------------------------------------------------------------------
// Inline implementations.
//------------------------------------------------------------------------------

template <typename Elem> 
      inline void WitPtrVec <Elem>::sort (
         bool (* theCompFunc) (Elem *, Elem *),
         int     nElemsToSort)
   {
   WitPtrVecCompFtor <Elem> theFtor (theCompFunc);
   void * *                 theCVec;

   theCVec = myVector_.myCVecForUpdate ();

   std::sort (theCVec, theCVec + nElemsToSort, theFtor);
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      inline void WitPtrVec <Elem>::attachItr (
         WitPtrVecItr <Elem> & theItr)
         const
   {
   theItr.attachTo (* this);
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      inline void WitPtrVecItr <Elem>::attachTo (
         const WitPtrVec <Elem> & thePtrVec)
   {
   myPtrVecPtr_ = & thePtrVec;

   myIdx_       = -1;
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      inline bool WitPtrVecItr <Elem>::advance (Elem * & theElem)
   {
   witAssert (myPtrVecPtr_ != NULL);

   myIdx_ ++;

   if (myIdx_ < myPtrVecPtr_->length ())
      {
      theElem = (* myPtrVecPtr_)[myIdx_];

      return true;
      }
   else
      {
      myPtrVecPtr_ = NULL;
      myIdx_       = -1;

      return false;
      }
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      inline bool WitPtrVecCompFtor <Elem>::operator () (
            void * theVoid1,
            void * theVoid2)
         {
         Elem * theElem1;
         Elem * theElem2;

         theElem1 = reinterpret_cast <Elem *> (theVoid1);
         theElem2 = reinterpret_cast <Elem *> (theVoid2);

         return myCompFunc_ (theElem1, theElem2);
         }

#endif
