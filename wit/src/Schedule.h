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

#ifndef ScheduleH
#define ScheduleH

//------------------------------------------------------------------------------
// Header file: "Schedule.h"
//
// Contains the declaration of class template Schedule <DomEl, RangeEl>.
//------------------------------------------------------------------------------

#include <CompMgr.h>
#include <TVec.h>
#include <PtrVec.h>

//------------------------------------------------------------------------------
// Class template Schedule <DomEl, RangeEl>
//
// An instance of class template Schedule <DomEl, RangeEl>
// represents a mapping whose domain is the Cartesian product of
//    the set of all instances of DomEl belonging to a Problem, and
//    the set of all periods belonging to that Problem
// and whose range is the set of all instances of RangeEl.
// For example, the declaration
//
//    WitSchedule <WitPart, double> tempSupplyVol (myProblem);
//
// declares tempSupplyVol to be a mapping from
//
//    {(thePart, t) | thePart is a Part in myProblem and t is a period in
//                    myProblem}
//
// into the set of all doubles. It is used by invoking the () operator, which
// returns a TVec <double> and then indexing into that TVec <double>. Thus if
// tempSupplyVol represent a temporary supply schedule, then
//
//    tempSupplyVol (thePart)[thePer] = 43.9;
//
// sets the temporary supply volume scheduled for thePart in thePer to 43.9.
//
// This class is implemented using a PtrVec <TVec <RangeEl> >
// which is indexed by using the NonClass template function: 
// mappingIndex (DomEl *).
//
// This is a value-based collection class; its methods make heavy use
// of the copy constructor for RangeEl. However, instances of DomEl
// are accessed through pointers.
//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      class WitSchedule
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSchedule ();

      WitSchedule (WitProblem * theProblem, RangeEl theRangeEl);
         //
         // Constructs a Schedule for mapping the DomEl instances
         // and periods in theProblem into RangeEl.
         // Sets every element of this Schedule to theRangeEl.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitSchedule ();

      //------------------------------------------------------------------------
      // Public member operators.
      //------------------------------------------------------------------------

      inline const WitTVec <RangeEl> & operator () (
            const DomEl * theObject)
            const;
         //
         // Returns the TVec of values for theObject (const).

      inline WitTVec <RangeEl> & operator () (const DomEl * theObject);
         //
         // Returns the TVec of values for theObject (non-const).

      void operator = (const WitSchedule & theSchedule);

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline int domainSize () const;
         //
         // Returns # DomEl instances scheduled.

      void clear ();
         //
         // Resizes this Schedule to cover 0 instances.

      void allocate (WitProblem * theProblem, RangeEl theRangeEl);
         //
         // Resizes this Schedule for mapping the DomEl instances
         // and periods in theProblem into RangeEl. For each instance of
         // DomEl, and each period, the value of this Schedule will
         // be set to theRangeEl.

      void allocate1D (WitProblem * theProblem);
         //
         // Allocates the Vector of row Vector pointers, but not the row
         // Vectors.

      inline const WitTVec <RangeEl> & myTVecAt (int theIdx) const
         {
         return myTVecs_.myElemAt (theIdx);
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtor (WitSchedule);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPtrVec <WitTVec <RangeEl> > myTVecs_;
         //
         // For each instance of DomEl, theInstance,
         // myTVecs_ (theInstance->mappingIndex ()) is the TVec <RangeEl>
         // that stores the value of this Schedule for theInstance in each
         // period.
   };

//------------------------------------------------------------------------------
// Implementation of inlines.
//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      inline const WitTVec <RangeEl> &
         WitSchedule <DomEl, RangeEl>::operator () (const DomEl * theObject)
            const
   {
   witAssert (theObject != NULL);

   return myTVecs_.myElemAt (theObject->mappingIndex ());
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      inline WitTVec <RangeEl> &
         WitSchedule <DomEl, RangeEl>::operator () (const DomEl * theObject)
   {
   witAssert (theObject != NULL);

   return myTVecs_.myElemAt (theObject->mappingIndex ());
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      inline int WitSchedule <DomEl, RangeEl>::domainSize () const
   {
   return myTVecs_.length ();
   }

//------------------------------------------------------------------------------

#endif
