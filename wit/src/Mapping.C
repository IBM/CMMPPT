//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Mapping.C"
//
// Contains:
//    The implementation         of class template Mapping <DomEl, RangeEl>.
//    The explicit instantiation of class template Mapping <DomEl, RangeEl>.
//------------------------------------------------------------------------------

#include <Mapping.h>
#include <Material.h>
#include <Operation.h>
#include <BomEntry.h>
#include <Demand.h>

//------------------------------------------------------------------------------
// Implementation of class template Mapping <DomEl, RangeEl>
//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      WitMapping <DomEl, RangeEl>::WitMapping ():
         myVector_ ()
   {
   if (false)
      {
      DomEl * theDomEl;

      DomEl::verifyMappingIndex (theDomEl);
      }
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      WitMapping <DomEl, RangeEl>::WitMapping (
         WitProblem * theProblem,
         RangeEl      initValue):

      myVector_ ()
   {
   allocate (theProblem, initValue);
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      WitMapping <DomEl, RangeEl>::~WitMapping ()
   {
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      void WitMapping <DomEl, RangeEl>::clear ()
   {
   myVector_.clear ();
   }

//------------------------------------------------------------------------------

template <typename DomEl, typename RangeEl>
      void WitMapping <DomEl, RangeEl>::allocate (
         WitProblem * theProblem,
         RangeEl      theRangeEl)
   {
   myVector_.resize (DomEl::nInstances (theProblem), theRangeEl);
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template Mapping <DomEl, RangeEl>.
//------------------------------------------------------------------------------

template class WitMapping <WitNode,      bool>;
template class WitMapping <WitPart,      bool>;
template class WitMapping <WitMaterial,  bool>;
template class WitMapping <WitOperation, bool>;
template class WitMapping <WitBomEntry,  bool>;
template class WitMapping <WitDemand,    bool>;

template class WitMapping <WitNode,      int>;
template class WitMapping <WitPart,      int>;
template class WitMapping <WitMaterial,  int>;
template class WitMapping <WitOperation, int>;
