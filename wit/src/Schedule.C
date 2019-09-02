//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Schedule.C"
//
// Contains:
//    The implementation         of class template Schedule <DomEl, RangeEl>.
//    The explicit instantiation of class template Schedule <DomEl, RangeEl>.
//------------------------------------------------------------------------------

#include <Schedule.h>
#include <Material.h>
#include <Demand.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <RtSite.h>
#include <RtCand.h>

//------------------------------------------------------------------------------
// Implementation of class template Schedule <DomEl, RangeEl>.
//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      WitSchedule <DomEl, RangeEl>::WitSchedule ():
         myTVecs_ ()
   {
   if (false)
      {
      DomEl * theDomEl;

      DomEl::verifyMappingIndex (theDomEl);
      }
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      WitSchedule <DomEl, RangeEl>::WitSchedule (
            WitProblem * theProblem, RangeEl initValue):
         myTVecs_ ()
   {
   allocate (theProblem, initValue);
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      WitSchedule <DomEl, RangeEl>::~WitSchedule ()
   {
   clear ();
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      void WitSchedule <DomEl, RangeEl>::operator = (
         const WitSchedule & theSchedule)
   {
   int theIdx;

   for (theIdx = 0; theIdx < domainSize (); theIdx ++)
      myTVecs_.myElemAt (theIdx) = theSchedule.myTVecs_.myElemAt (theIdx);
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      void WitSchedule <DomEl, RangeEl>::clear ()
   {
   int theIdx;

   for (theIdx = 0; theIdx < domainSize (); theIdx ++)
      delete myTVecs_[theIdx];

   myTVecs_.clear ();
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      void WitSchedule <DomEl, RangeEl>::allocate (
         WitProblem * theProblem,
         RangeEl      theRangeEl)
   {
   int theIdx;

   allocate1D (theProblem);

   for (theIdx = 0; theIdx < domainSize (); theIdx ++)
      myTVecs_.myElemAt (theIdx).allocate (theProblem, theRangeEl);
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      void WitSchedule <DomEl, RangeEl>::allocate1D (WitProblem * theProblem)
   {
   int theIdx;

   clear ();

   myTVecs_.resize (DomEl::nInstances (theProblem));

   for (theIdx = 0; theIdx < domainSize (); theIdx ++)
      myTVecs_[theIdx] = new WitTVec <RangeEl>;
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template Schedule <DomEl, RangeEl>.
//------------------------------------------------------------------------------

template class WitSchedule <WitPart,      bool>;
template class WitSchedule <WitMaterial,  bool>;
template class WitSchedule <WitOperation, bool>;
template class WitSchedule <WitBomEntry,  bool>;
template class WitSchedule <WitBopEntry,  bool>;
template class WitSchedule <WitDemand,    bool>;
template class WitSchedule <WitRtSite,    bool>;
template class WitSchedule <WitRtCand,    bool>;

template class WitSchedule <WitPart,      int>;
template class WitSchedule <WitDemand,    int>;
template class WitSchedule <WitBopEntry,  int>;
template class WitSchedule <WitBillEntry, int>;

template class WitSchedule <WitNode,      double>;
template class WitSchedule <WitPart,      double>;
template class WitSchedule <WitMaterial,  double>;
template class WitSchedule <WitDemand,    double>;
template class WitSchedule <WitOperation, double>;
template class WitSchedule <WitBillEntry, double>;
template class WitSchedule <WitSubEntry,  double>;
template class WitSchedule <WitBopEntry,  double>;
