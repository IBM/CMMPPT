//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "ExtOptMgr.C"
//
// Contains the implementation of class ExtOptMgr.
//------------------------------------------------------------------------------

#include <ExtOptMgr.h>
#include <DetImpOP.h>
#include <DetVars.h>
#include <DetCons.h>
#include <IVRealArg.h>
#include <Timing.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class ExtOptMgr.
//------------------------------------------------------------------------------

WitExtOptMgr::WitExtOptMgr (WitDetImpOP * theDetImpOP):

      WitProbAssoc  (theDetImpOP),
      myDetImpOP_   (theDetImpOP),
      solnProvided_ (false)
   {
   }

//------------------------------------------------------------------------------

WitExtOptMgr::~WitExtOptMgr ()
   {
   }

//------------------------------------------------------------------------------

void WitExtOptMgr::getLpProb (
      WitVector <int> &    start,
      WitVector <int> &    index,
      WitVector <double> & value,
      WitVector <double> & collb,
      WitVector <double> & colub,
      WitVector <double> & obj,
      WitVector <double> & rowlb,
      WitVector <double> & rowub)
   {
   WitTimer::enterSection ("opt-prob");

   myDetImpOP_->getMatrixByCols (start, index, value);

   myDetImpOP_->getColumnData   (collb, colub, obj);
   myDetImpOP_->getRowData      (rowlb, rowub);

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitExtOptMgr::getIntVarIndices (WitVector <int> & indices)
   {
   WitOptVar * theOptVar;
   int         nIntVars;
   int         vecIdx;

   WitTimer::enterSection ("opt-prob");

   nIntVars = 0;

   forEachEl (theOptVar, myDetImpOP_->myOptVars ())
      if (theOptVar->isAnIntVar ())
         nIntVars ++;

   indices.resize (nIntVars);

   vecIdx = -1;

   forEachEl (theOptVar, myDetImpOP_->myOptVars ())
      if (theOptVar->isAnIntVar ())
         {
         vecIdx ++;

         indices[vecIdx] = theOptVar->index ();
         }

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitExtOptMgr::setSoln (const WitIVRealArg & theVector)
   {
   WitOptVar * theOptVar;
   int         theIdx;

   WitTimer::enterSection ("opt-prob");

   forEachEl (theOptVar, myDetImpOP_->myOptVars ())
      {
      theIdx = theOptVar->index ();

      theOptVar->setPrimalValue (theVector[theIdx]);
      }

   solnProvided_ = true;

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::scrapVarIndex (WitPart * thePart, WitPeriod thePer)
   {
   return indexOf (myDetImpOP_->myScrapVar (thePart, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::nonSubVarIndex (WitBomEntry * theBomEnt, WitPeriod thePer)
   {
   return indexOf (myDetImpOP_->myNonSubVar (theBomEnt, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::subVarIndex (WitSubEntry * theSub, WitPeriod thePer)
   {
   return indexOf (myDetImpOP_->mySubVar (theSub, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::shipVarIndex (WitDemand * theDemand, WitPeriod thePer)
   {
   return indexOf (myDetImpOP_->myShipVar (theDemand, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::execVarIndex (WitOperation * theOpn, WitPeriod thePer)
   {
   return indexOf (myDetImpOP_->myExecVar (theOpn, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::stockVarIndex (WitMaterial * theMat, WitPeriod thePer)
   {
   return indexOf (myDetImpOP_->myStockVar (theMat, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::cumShipVarIndex (WitDemand * theDemand, WitPeriod thePer)
   {
   return indexOf (myDetImpOP_->myCumShipVar (theDemand, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::execSlbvVarIndex (WitOperation * theOpn, WitPeriod thePer)
   {
   return slbvVarIndexOf (myDetImpOP_->myExecVar (theOpn, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::stockSlbvVarIndex (WitMaterial * theMat, WitPeriod thePer)
   {
   return slbvVarIndexOf (myDetImpOP_->myStockVar (theMat, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::cumShipSlbvVarIndex (
      WitDemand * theDemand,
      WitPeriod   thePer)
   {
   return slbvVarIndexOf (myDetImpOP_->myCumShipVar (theDemand, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::resourceConIndex (WitPart * thePart, WitPeriod thePer)
   {
   return indexOf (myDetImpOP_->myResourceCon (thePart, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::subConIndex (WitBomEntry * theBomEnt, WitPeriod thePer)
   {
   return indexOf (myDetImpOP_->mySubCon (theBomEnt, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::shipConIndex (WitDemand * theDemand, WitPeriod thePer)
   {
   return indexOf (myDetImpOP_->myShipCon (theDemand, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::execSlbConIndex (WitOperation * theOpn, WitPeriod thePer)
   {
   return slbConIndexOf (myDetImpOP_->myExecVar (theOpn, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::stockSlbConIndex (WitMaterial * theMat, WitPeriod thePer)
   {
   return slbConIndexOf (myDetImpOP_->myStockVar (theMat, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::cumShipSlbConIndex (
      WitDemand * theDemand,
      WitPeriod   thePer)
   {
   return slbConIndexOf (myDetImpOP_->myCumShipVar (theDemand, thePer));
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::slbvVarIndexOf (WitBoundedVar * theBoundedVar)
   {
   WitSlbvVar * theSlbvVar;

   if (theBoundedVar == NULL)
      return -1;

   theSlbvVar = myDetImpOP_->mySlbvVar (theBoundedVar);

   return indexOf (theSlbvVar);
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::slbConIndexOf (WitBoundedVar * theBoundedVar)
   {
   WitSlbCon * theSlbCon;

   if (theBoundedVar == NULL)
      return -1;

   theSlbCon = myDetImpOP_->mySlbCon (theBoundedVar);

   return indexOf (theSlbCon);
   }

//------------------------------------------------------------------------------

int WitExtOptMgr::indexOf (WitOptVC * theOptVC)
   {
   if (theOptVC == NULL)
      return -1;

   return theOptVC->index ();
   }
