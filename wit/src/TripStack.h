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

#ifndef TripStackH
#define TripStackH

//------------------------------------------------------------------------------
// Header file: "TripStack.h"
//
// Contains the declaration of the following classes and templates:
//
//    AbsTripStack
//    TripStack <ObjByPtr>
//------------------------------------------------------------------------------

#include <Stack.h>
#include <Link.h>

//------------------------------------------------------------------------------
// Class AbsTripStack
//
// A stack of triples, (ObjByVP, Period, Double), where ObjByVP is an object of
// undefined type, accessed by pointer.
//
// Class Hierarchy:
//
// AbsStack
//    AbsTripStack
//       TripStack <ObjByPtr>
//
// Implemented in Stack.C
//------------------------------------------------------------------------------

class WitAbsTripStack: public WitAbsStack
   {
   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAbsTripStack (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitAbsTripStack ();

      //------------------------------------------------------------------------
      // Other protected member functions.
      //------------------------------------------------------------------------

      void pushAbsTrip (void *, WitPeriod, double);
         //
         // Pushes a triple formed from the args onto the top of this
         // AbsObjStack.

      bool popAbsTrip (void * &, WitPeriod &, double &);
         //
         // If this AbsTripStack is non-Empty, the first triple it contains is
         // removed from it and the args are set to the elements of the removed
         // triple. Returns true, iff this AbsTripStack is non-empty.

      void revCopyFromAbs (const WitAbsTripStack & theAbsTripStack);
         //
         // Copies the contents of theAbsTripStack into this AbsTripStack in 
         // reverse order.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      WitTripLink * firstTripLink () const;

      noCopyCtorAssign (WitAbsTripStack);
   };

//------------------------------------------------------------------------------
// Class template TripStack <ObjByPtr>
//
// A stack of triples, (ObjByPtr, Period, Double).
//
// Class Hierarchy:
//
// AbsStack
//    AbsTripStack
//       TripStack <ObjByPtr>
//
// All implementation is inline.
//------------------------------------------------------------------------------

template <typename ObjByPtr> 
      class WitTripStack: public WitAbsTripStack
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      inline WitTripStack (WitProblem * theProblem):
            WitAbsTripStack (theProblem)
         {
         }

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      inline ~WitTripStack ()
         {
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline void push (
            ObjByPtr * theObject,
            WitPeriod  thePer,
            double     theDbl)
         {
         pushAbsTrip (theObject, thePer, theDbl);
         }

      inline bool pop (
            ObjByPtr * & theObject,
            WitPeriod &  thePer,
            double &     theDbl)
         {
         return
            popAbsTrip (
               reinterpret_cast <void * &> (theObject),
               thePer,
               theDbl);
         }

      inline void takeContentsFrom (WitTripStack & thatTripStack)
         {
         takeContentsFromAbs (thatTripStack);
         }

      inline void revCopyFrom (const WitTripStack & theTripStack)
         {
         revCopyFromAbs (theTripStack);
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitTripStack);
   };

#endif
