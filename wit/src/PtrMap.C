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
// Source file: "PtrMap.C"
//
// Contains:
//    The implementation           of class template PtrMap <DomEl, RangeEl>.
//    The explicit instantiatation of class template PtrMap <DomEl, RangeEl>.
//------------------------------------------------------------------------------

#include <PtrMap.h>
#include <CompMgr.h>
#include <Scenario.h>
#include <Material.h>
#include <Demand.h>
#include <Operation.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BoundSet.h>
#include <Objective.h>
#include <RtSite.h>
#include <RtCand.h>

//------------------------------------------------------------------------------
// Implementation of class template PtrMap <DomEl>.
//------------------------------------------------------------------------------

template <PtrMapArgDecs> 
      WitPtrMap <PtrMapArgs>::WitPtrMap ():
         myPtrVec_ ()
   {
   if (false)
      {
      DomEl * theDomEl;

      DomEl::verifyMappingIndex (theDomEl);
      }
   }

//------------------------------------------------------------------------------

template <PtrMapArgDecs> 
      WitPtrMap <PtrMapArgs>::WitPtrMap (WitProblem * theProblem):

         myPtrVec_ ()
   {
   allocate (theProblem);
   }

//------------------------------------------------------------------------------

template <PtrMapArgDecs> 
      WitPtrMap <PtrMapArgs>::~WitPtrMap ()
   {
   }

//------------------------------------------------------------------------------

template <PtrMapArgDecs> 
      void WitPtrMap <PtrMapArgs>::allocate (WitProblem * theProblem)
   {
   allocate (DomEl::nInstances (theProblem));
   }

//------------------------------------------------------------------------------

template <PtrMapArgDecs> 
      void WitPtrMap <PtrMapArgs>::allocate (int nInst)
   {
   myPtrVec_.resize (nInst);
   }

//------------------------------------------------------------------------------

template <PtrMapArgDecs> 
      void WitPtrMap <PtrMapArgs>::clear ()
   {
   myPtrVec_.clear ();
   }

//------------------------------------------------------------------------------
// Explicit instantiatation of class template PtrMap <DomEl, RangeEl>.
//------------------------------------------------------------------------------

template class WitPtrMap <WitNode,      WitNode>;
template class WitPtrMap <WitMaterial,  WitBaMat>;
template class WitPtrMap <WitPart,      WitRtSite>;
template class WitPtrMap <WitBomEntry,  WitRtSite>;
template class WitPtrMap <WitBillEntry, WitRtCand>;
template class WitPtrMap <WitRtSite,    WitSglSrcSite>;
template class WitPtrMap <WitRtSite,    WitMrSite>;
template class WitPtrMap <WitRtCand,    WitMrCand>;
template class WitPtrMap <WitNode,      WitStage>;

template class WitPtrMap <WitPart,      WitStScrapVar>;
template class WitPtrMap <WitMaterial,  WitStStockVar>;
template class WitPtrMap <WitOperation, WitStExecVar>;
template class WitPtrMap <WitDemand,    WitStShipVar>;
template class WitPtrMap <WitDemand,    WitStCumShipVar>;
template class WitPtrMap <WitBomEntry,  WitStNonSubVar>;
template class WitPtrMap <WitSubEntry,  WitStSubVar>;

template class WitPtrMap <WitObjective, WitMultiObjVar>;
template class WitPtrMap <WitObjective, WitMultiObjCon>;

template class WitPtrMap <WitPart,      WitDblFlexVec>;
template class WitPtrMap <WitMaterial,  WitDblFlexVec>;
template class WitPtrMap <WitDemand,    WitDblFlexVec>;
template class WitPtrMap <WitOperation, WitDblFlexVec>;
template class WitPtrMap <WitSubEntry,  WitDblFlexVec>;
template class WitPtrMap <WitBoundSet,  WitDblFlexVec>;
template class WitPtrMap <WitScenario,  WitDblFlexVec>;
template class WitPtrMap <WitObjective, WitDblFlexVec>;

template class WitPtrMap <WitNode,      WitList   <WitNode> >;

template class WitPtrMap <WitPart,      WitPtrMap <WitScenario,
                                                   WitDblFlexVec> >;
template class WitPtrMap <WitMaterial,  WitPtrMap <WitScenario,
                                                   WitDblFlexVec> >;
template class WitPtrMap <WitDemand,    WitPtrMap <WitScenario,
                                                   WitDblFlexVec> >;
template class WitPtrMap <WitOperation, WitPtrMap <WitScenario,
                                                   WitDblFlexVec> >;
template class WitPtrMap <WitSubEntry,  WitPtrMap <WitScenario,
                                                   WitDblFlexVec> >;
template class WitPtrMap <WitBoundSet,  WitPtrMap <WitScenario,
                                                   WitDblFlexVec> >;
