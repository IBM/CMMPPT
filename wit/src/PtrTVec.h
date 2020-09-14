//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef PtrTVecH
#define PtrTVecH

//------------------------------------------------------------------------------
// Header file: "PtrTVec.h"
//
// Contains the declaration of class template PtrTVec <Elem>.
//------------------------------------------------------------------------------

#include <TVec.h>

//------------------------------------------------------------------------------
// Class template PtrTVec <Elem>
//
// A time vector of pointers to Elem.
// This class is implemented as a typed wrapper for class TVec <void *>.
// See class TVec <void *> for comments on the member functions.
//
// All implementation is inline.
//------------------------------------------------------------------------------

template <typename Elem> 
      class WitPtrTVec
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      inline WitPtrTVec ():
            myTVec_ ()
         {
         }

      inline WitPtrTVec (WitProblem * theProblem):
            myTVec_ (theProblem, NULL)
         {
         }

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      inline ~WitPtrTVec ()
         {
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline void allocate (WitProblem * theProblem)
         {
         myTVec_.allocate (theProblem, NULL);
         }

      inline void clear ()
         {
         myTVec_.clear ();
         }

      inline bool isAllocated () const
         {
         return myTVec_.isAllocated ();
         }

      inline void setToNull ()
         {
         myTVec_ = static_cast <void *> (NULL);
         }

      inline void operator = (const WitPtrTVec & thePtrTVec)
         {
         myTVec_ = thePtrTVec.myTVec_;
         }

      inline Elem & operator () (int theIndex) const
         {
         witAssert ((* this)[theIndex] != NULL);

         return   * (* this)[theIndex];
         }

      inline Elem * operator [] (int theIndex) const
         {
         return reinterpret_cast <Elem *> (myTVec_[theIndex]);
         }

      inline Elem * & operator [] (int theIndex)
         {
         return reinterpret_cast <Elem * &> (myTVec_[theIndex]);
         }

      inline int length () const
         {
         return myTVec_.length ();
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtor (WitPtrTVec);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitTVec <void *> myTVec_;
         //
         // The TVec that stores the data for this PtrTVec.
   };

//------------------------------------------------------------------------------
// NonClass function template deleteContents.
//
// Deletes all of the objects stored by thePtrTVec and clears thePtrTVec.
//------------------------------------------------------------------------------

namespace WitNonClass
   {
   template <typename Elem> 
         void deleteContents (WitPtrTVec <Elem> & thePtrTVec)
      {
      WitPeriod theIdx;
   
      for (theIdx = 0; theIdx < thePtrTVec.length (); theIdx ++)
         delete thePtrTVec[theIdx];
   
      thePtrTVec.clear ();
      }
   }

#endif
