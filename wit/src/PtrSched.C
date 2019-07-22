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

//------------------------------------------------------------------------------
// Source file: "PtrSched.C"
//
// Contains the implementation of class   template PtrSched <DomEl, RangeEl>.
// Contains the explicit instantiation of template PtrSched <DomEl, RangeEl>.
//------------------------------------------------------------------------------

#include <PtrSched.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <Operation.h>
#include <Material.h>
#include <Demand.h>
#include <BoundSet.h>
#include <RtSite.h>
#include <RtCand.h>

//------------------------------------------------------------------------------
// Implementation of class template PtrSched <DomEl, RangeEl>.
//------------------------------------------------------------------------------

template <PtrSchedArgDecs> 
      WitPtrSched <PtrSchedArgs>::WitPtrSched ():
         myPtrVec_ ()
   {
   if (false)
      {
      DomEl * theDomEl;

      DomEl::verifyMappingIndex (theDomEl);
      }
   }

//------------------------------------------------------------------------------

template <PtrSchedArgDecs> 
      WitPtrSched <PtrSchedArgs>::WitPtrSched (WitProblem * theProblem):

         myPtrVec_ ()
   {
   allocate (theProblem);
   }

//------------------------------------------------------------------------------

template <PtrSchedArgDecs> 
      WitPtrSched <PtrSchedArgs>::~WitPtrSched ()
   {
   clear ();
   }

//------------------------------------------------------------------------------

template <PtrSchedArgDecs> 
      void WitPtrSched <PtrSchedArgs>::allocate (WitProblem * theProblem)
   {
   int theIdx;

   allocate1D (theProblem);

   for (theIdx = 0; theIdx < myPtrVec_.length (); theIdx ++)
      myPtrVec_.myElemAt (theIdx).allocate (theProblem);
   }

//------------------------------------------------------------------------------

template <PtrSchedArgDecs> 
      void WitPtrSched <PtrSchedArgs>::allocate1D (WitProblem * theProblem)
   {
   allocate1D (DomEl::nInstances (theProblem));
   }

//------------------------------------------------------------------------------

template <PtrSchedArgDecs> 
      void WitPtrSched <PtrSchedArgs>::allocate1D (int nInst)
   {
   int theIdx;

   clear ();

   myPtrVec_.resize (nInst);

   for (theIdx = 0; theIdx < nInst; theIdx ++)
      myPtrVec_[theIdx] = new WitPtrTVec <RangeEl>;
   }

//------------------------------------------------------------------------------

template <PtrSchedArgDecs> 
      void WitPtrSched <PtrSchedArgs>::clear ()
   {
   int theIdx;

   for (theIdx = 0; theIdx < myPtrVec_.length (); theIdx ++)
      delete myPtrVec_[theIdx];

   myPtrVec_.clear ();
   }

//------------------------------------------------------------------------------

template class WitPtrSched <WitPart,      WitScrapVar>;
template class WitPtrSched <WitBomEntry,  WitNonSubVar>;
template class WitPtrSched <WitSubEntry,  WitSubVar>;
template class WitPtrSched <WitDemand,    WitShipVar>;
template class WitPtrSched <WitBoundSet,  WitSlbvVar>;
template class WitPtrSched <WitOperation, WitExecVar>;
template class WitPtrSched <WitMaterial,  WitStockVar>;
template class WitPtrSched <WitDemand,    WitCumShipVar>;
template class WitPtrSched <WitPart,      WitResourceCon>;
template class WitPtrSched <WitBomEntry,  WitSubCon>;
template class WitPtrSched <WitDemand,    WitShipCon>;
template class WitPtrSched <WitBoundSet,  WitSlbCon>;
template class WitPtrSched <WitBopEntry,  WitMeSitePt>;
template class WitPtrSched <WitDemand,    WitObjStack <WitPegEl> >;
template class WitPtrSched <WitOperation, WitPegEl>;
template class WitPtrSched <WitSubEntry,  WitPegEl>;
template class WitPtrSched <WitRtSite,    WitPrSelPt>;
template class WitPtrSched <WitRtCand,    WitPrAltPt>;
template class WitPtrSched <WitPart,      WitReqPt>;
template class WitPtrSched <WitPart,      WitPartPerStack>;
template class WitPtrSched <WitMaterial,  WitSsrPt>;
template class WitPtrSched <WitPart,      WitPipReqList>;
template class WitPtrSched <WitBopEntry,  WitPipReqList>;
template class WitPtrSched <WitOperation, WitPipReqClient <WitOperation> >;
template class WitPtrSched <WitDemand,    WitTripStack    <WitOperation> >;
template class WitPtrSched <WitDemand,    WitTripStack    <WitBopEntry>  >;
template class WitPtrSched <WitDemand,    WitTripStack    <WitSubEntry>  >;
template class WitPtrSched <WitDemand,    WitTripStack    <WitPart>      >;
template class WitPtrSched <WitOperation, WitTripStack    <WitOperation> >;
template class WitPtrSched <WitOperation, WitTripStack    <WitBopEntry>  >;
template class WitPtrSched <WitOperation, WitTripStack    <WitSubEntry>  >;
template class WitPtrSched <WitOperation, WitTripStack    <WitPart>      >;


