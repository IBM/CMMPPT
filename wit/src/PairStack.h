//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef PairStackH
#define PairStackH

//------------------------------------------------------------------------------
// Header file: "PairStack.h"
//
// Contains the declaration of the following templates:
//
//    PairStack <ObjByPtr, ObjByVal>
//    PairStItr <ObjByPtr, ObjByVal>
//------------------------------------------------------------------------------

#include <Stack.h>

//------------------------------------------------------------------------------
// Class template PairStack <ObjByPtr, ObjByVal>
//
// A.K.A. PairStack <PairStArgs>
//
// A stack of pairs of the form: (ObjByPtr *, ObjByVal).
//
// Class Hierarchy:
//
// AbsStack
//    PairStack <PairStArgs>
//
// Implemented in Stack.C.
//------------------------------------------------------------------------------

#define PairStArgDecs  typename ObjByPtr, typename ObjByVal
#define PairStArgs              ObjByPtr,          ObjByVal
   //
   // Template argument macros.

//------------------------------------------------------------------------------

template <PairStArgDecs> 
      class WitPairStack: public WitAbsStack
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPairStack (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPairStack ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void push (ObjByPtr * theObjByPtr, ObjByVal theObjByVal);
         //
         // Pushes (theObjByPtr, theObjByVal) onto the top of this PairStack.

      bool pop (ObjByPtr * & theObjByPtr, ObjByVal & theObjByVal);
         //
         // If this PairStack is non-Empty, the first pair it contains is
         // removed from it and (theObjByPtr, theObjByVal) is set to the removed
         // pair.
         // Returns true, iff this PairStack is non-empty.

      void revCopyFrom (const WitPairStack & thePairStack);
         //
         // Copies the contents of thePairStack into this PairStack in reverse
         // order.

      inline void takeContentsFrom (WitPairStack & thatPairStack)
         {
         takeContentsFromAbs (thatPairStack);
         }

      inline void attachItr (WitPairStItr <PairStArgs> & thePairStItr) const
         {
         attachItrAbs (thePairStItr);
         }

      ObjByPtr * firstObject () const;
         //
         // Asserts that this PairStack is not empty.
         // Returns the first object in this PairStack.

      ObjByVal firstValue () const;
         //
         // Asserts that this PairStack is not empty.
         // Returns the first value in this PairStack.

      static bool advance (
            WitPairStItr <PairStArgs> & theItr,
            ObjByPtr * &                theObjByPtr,
            ObjByVal &                  theObjByVal);
         //
         // See AbsStItr::advanceAbs.
         // Sets theObjByPtr and theObjByVal, if the advance is successful.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      inline WitPairLink <ObjByVal> * firstPairLink () const
         {
         return static_cast <WitPairLink <ObjByVal> *> (firstAbsLink ());
         }

      noCopyCtorAssign (WitPairStack);
   };

//------------------------------------------------------------------------------
// Class template PairStItr <ObjByPtr, ObjByVal>
//
// A.K.A. PairStItr <PairStArgs>
//
// Responsible for iterating through the elements of a PairStack <PairStArgs>.
//
// Class Hierarchy:
//
// AbsStItr
//    PairStItr <PairStArgs>
//
// All implementation is inline.
//------------------------------------------------------------------------------

template <PairStArgDecs> 
      class WitPairStItr: public WitAbsStItr
   {
      friend class WitPairStack <PairStArgs>;

   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      inline WitPairStItr ():
            WitAbsStItr ()
         {
         }

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      inline ~WitPairStItr ()
         {
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline bool advance (ObjByPtr * & theObjByPtr, ObjByVal & theObjByVal)
         {
         return
            WitPairStack <PairStArgs>::advance (
               * this,
                 theObjByPtr,
                 theObjByVal);
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPairStItr);
   };

#endif
