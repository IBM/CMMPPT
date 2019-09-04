//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef PerSglStackH
#define PerSglStackH

//------------------------------------------------------------------------------
// Header file: "PerSglStack.h"
//
// Contains the declaration of class template PerSglStack <ObjByPtr>.
//------------------------------------------------------------------------------

#include <PairStack.h>
#include <Schedule.h>

//------------------------------------------------------------------------------
// Class template PerSglStack <ObjByPtr>
//
// A stack of pairs of the form: (ObjByPtr *, Period), without duplication.
//
// Implemented in Stack.C.
//------------------------------------------------------------------------------

template <typename ObjByPtr> 
      class WitPerSglStack
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPerSglStack (WitProblem * theProblem);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPerSglStack ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void push (ObjByPtr * theObjByPtr, WitPeriod thePer);
         //
         // Pushes (theObjByPtr, thePer) onto the top of this PerSglStack.
         // No-op, if theObjByPtr, thePer) is already in his PerSglStack.

      bool pop (ObjByPtr * & theObjByPtr, WitPeriod & thePer);
         //
         // If this PerSglStack is non-Empty, the first pair it contains is
         // removed from it and theObjByPtr and thePer are set to the removed
         // pair.
         // Returns true, iff this PerSglStack is non-empty.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPerSglStack);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPairStack <ObjByPtr, WitPeriod> myPairs_;
         //
         // The pairs held by this PerSglStack.

      WitSchedule <ObjByPtr, bool> inStack_;
         //
         // inStack_ (theObjByPtr)[thePer] is true, iff (theObjByPtr, thePer) is
         // in this PerSglStack.
         // Allocated as needed.
   };

#endif
