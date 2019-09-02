//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "ScenAttMgr.C"
//
// Contains the implementation of class ScenAttMgr.
//------------------------------------------------------------------------------

#include <ScenAttMgr.h>
#include <ScenInputAtt.h>
#include <ScenSolnAtt.h>
#include <ScenMgr.h>
#include <StochAttMgr.h>
#include <DataWrit.h>
#include <CompMgr.h>
#include <Material.h>
#include <Demand.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BoundSet.h>

//------------------------------------------------------------------------------
// Implementation of class ScenAttMgr.
//------------------------------------------------------------------------------

WitScenAttMgr::WitScenAttMgr (WitStochModeMgr * theStochModeMgr):

      WitStochAssoc       (theStochModeMgr),
      myAbsScenInputAtts_ (),
      myAbsScenSolnAtts_  (),
      buildingInputAtts_  (false),
      buildingSolnAtts_   (false),
      supplyVol_          (),
      scrapCost_          (),
      stockCost_          (),
      demandVol_          (),
      shipReward_         (),
      cumShipReward_      (),
      execCost_           (),
      subCost_            (),
      hardLB_             (),
      softLB_             (),
      hardUB_             (),
      scrapVol_           (),
      stockVol_           (),
      shipVol_            (),
      cumShipVol_         (),
      execVol_            (),
      subVol_             (),
      allMats_            (),
      allDemands_         (),
      allSubs_            (),
      allBoundSets_       ()
   {
   listOwners         ();
   buildScenInputAtts ();
   buildScenSolnAtts  ();
   }

//------------------------------------------------------------------------------

WitScenAttMgr::~WitScenAttMgr ()
   {
   while (not myAbsScenSolnAtts_.isEmpty ())
      delete myAbsScenSolnAtts_.get ();

   while (not myAbsScenInputAtts_.isEmpty ())
      delete myAbsScenInputAtts_.get ();
   }

//------------------------------------------------------------------------------

void WitScenAttMgr::downloadInput ()
   {
   WitAbsScenInputAtt * theInputAtt;

   forEachEl (theInputAtt, myAbsScenInputAtts_)
      theInputAtt->downloadData (myScenMgr ()->curScenario ());
   }

//------------------------------------------------------------------------------

void WitScenAttMgr::uploadInput ()
   {
   WitAbsScenInputAtt * theInputAtt;

   forEachEl (theInputAtt, myAbsScenInputAtts_)
      theInputAtt->uploadData (myScenMgr ()->curScenario ());
   }

//------------------------------------------------------------------------------

void WitScenAttMgr::copyInputFrom (WitScenAttMgr * srcScenAttMgr)
   {
   WitAbsScenInputAtt * theInputAtt;

   forEachEl (theInputAtt, myAbsScenInputAtts_)
      theInputAtt->copyDataFrom (srcScenAttMgr);
   }

//------------------------------------------------------------------------------

void WitScenAttMgr::writeCompData (WitScenario * theScenario, bool diffedSec)
   {
   WitComponent *       theComp;
   WitDelComp *         theDelComp;
   bool                 attWritten;
   WitAbsScenInputAtt * theInputAtt;

   forEachEl (theComp, myCompMgr ()->allComponents ())
      {
      theDelComp = theComp->thisDelComp ();

      if (theDelComp == NULL)
         continue;

      attWritten = false;

      forEachEl (theInputAtt, myAbsScenInputAtts_)
         if (not theInputAtt->ownedByBoundSet ())
            theInputAtt->
               writeData (theComp, theScenario, diffedSec, attWritten);

      if (attWritten)
         myDataWriter ()->writeEndCommand ();
      }
   }

//------------------------------------------------------------------------------

void WitScenAttMgr::writeBoundSetData (
      WitScenario * theScenario,
      bool          diffedSec)
   {
   WitBoundSet *        theBoundSet;
   bool                 attWritten;
   WitAbsScenInputAtt * theInputAtt;

   forEachEl (theBoundSet, allBoundSets_)
      {
      attWritten = false;

      forEachEl (theInputAtt, myAbsScenInputAtts_)
         if (theInputAtt->ownedByBoundSet ())
            theInputAtt->
               writeData (theBoundSet, theScenario, diffedSec, attWritten);

      if (attWritten)
         {
         myDataWriter ()->writeFirstKeyWord ("endBounds");
         myDataWriter ()->writeEndCommand ();
         }
      }
   }

//------------------------------------------------------------------------------

void WitScenAttMgr::validateData ()
   {
   WitAbsScenInputAtt * theInputAtt;

   forEachEl (theInputAtt, myAbsScenInputAtts_)
      theInputAtt->validateData ();
   }

//------------------------------------------------------------------------------

void WitScenAttMgr::uploadSoln ()
   {
   WitAbsScenSolnAtt * theSolnAtt;

   forEachEl (theSolnAtt, myAbsScenSolnAtts_)
      theSolnAtt->uploadData (myScenMgr ()->curScenario ());
   }

//------------------------------------------------------------------------------

void WitScenAttMgr::clearSoln ()
   {
   WitAbsScenSolnAtt * theSolnAtt;

   forEachEl (theSolnAtt, myAbsScenSolnAtts_)
      theSolnAtt->clearData ();
   }

//------------------------------------------------------------------------------

void WitScenAttMgr::store (WitAbsScenInputAtt * theInputAtt)
   {
   stronglyAssert (buildingInputAtts_);

   myAbsScenInputAtts_.append (theInputAtt);
   }

//------------------------------------------------------------------------------

void WitScenAttMgr::store (WitAbsScenSolnAtt * theSolnAtt)
   {
   stronglyAssert (buildingSolnAtts_);

   myAbsScenSolnAtts_.append (theSolnAtt);
   }

//------------------------------------------------------------------------------

void WitScenAttMgr::listOwners ()
   {
   WitMaterial *  theMat;
   WitDemand *    theDemand;
   WitSubEntry *  theSub;
   WitComponent * theComp;
   WitBoundSet *  theBoundSet;

   forEachMaterial (theMat, myProblem ())
      allMats_.append (theMat);

   forEachDemand (theDemand, myProblem ())
      allDemands_.append (theDemand);

   forEachSubEntry (theSub, myProblem ())
      allSubs_.append (theSub);

   forEachEl (theComp, myCompMgr ()->allComponents ())
      {
      theBoundSet = theComp->myBoundSet ();

      if (theBoundSet != NULL)
         allBoundSets_.append (theBoundSet);
      }
   }

//------------------------------------------------------------------------------

void WitScenAttMgr::buildScenInputAtts ()
   {
   buildingInputAtts_ = true;

   supplyVol_ =
      new WitScenInputAtt      <WitPart> (this,
                                         "supplyVol",
                                 WitPart::defSupplyVol (),
                               & WitPart::supplyVol,
                               & WitPart::storeStochSupplyVol,
                         & WitScenAttMgr::supplyVol);

   scrapCost_ =
      new WitScenInputAtt      <WitPart> (this,
                                         "scrapCost",
                                 WitPart::defScrapCost (),
                               & WitPart::scrapCost,
                               & WitPart::storeStochScrapCost,
                         & WitScenAttMgr::scrapCost);

   stockCost_ =
      new WitScenInputAtt  <WitMaterial> (this,
                                         "stockCost",
                             WitMaterial::defStockCost (),
                           & WitMaterial::stockCost,
                           & WitMaterial::storeStochStockCost,
                         & WitScenAttMgr::stockCost);

   demandVol_ =
      new WitScenInputAtt    <WitDemand> (this,
                                         "demandVol",
                               WitDemand::defDemandVol (),
                             & WitDemand::demandVol,
                             & WitDemand::storeStochDemandVol,
                         & WitScenAttMgr::demandVol);

   shipReward_ =
      new WitScenInputAtt    <WitDemand> (this,
                                         "shipReward",
                               WitDemand::defShipReward (),
                             & WitDemand::shipReward,
                             & WitDemand::storeStochShipReward,
                         & WitScenAttMgr::shipReward);

   cumShipReward_ =
      new WitScenInputAtt    <WitDemand> (this,
                                         "cumShipReward",
                               WitDemand::defCumShipReward (),
                             & WitDemand::cumShipReward,
                             & WitDemand::storeStochCumShipReward,
                         & WitScenAttMgr::cumShipReward);

   execCost_ =
      new WitScenInputAtt <WitOperation> (this,
                                         "execCost",
                            WitOperation::defExecCost (),
                          & WitOperation::execCost,
                          & WitOperation::storeStochExecCost,
                         & WitScenAttMgr::execCost);

   subCost_ =
      new WitScenInputAtt  <WitSubEntry> (this,
                                         "subCost",
                             WitSubEntry::defSubCost (),
                           & WitSubEntry::subCost,
                           & WitSubEntry::storeStochSubCost,
                         & WitScenAttMgr::subCost);

   hardLB_ =
      new WitScenInputAtt <WitBoundSet> (this,
                                        "hardLB",
                            WitBoundSet::defHardLB (),
                          & WitBoundSet::hardLB,
                          & WitBoundSet::storeStochHardLB,
                        & WitScenAttMgr::hardLB);

   softLB_ =
      new WitScenInputAtt <WitBoundSet> (this,
                                        "softLB",
                            WitBoundSet::defSoftLB (),
                          & WitBoundSet::softLB,
                          & WitBoundSet::storeStochSoftLB,
                        & WitScenAttMgr::softLB);

   hardUB_ =
      new WitScenInputAtt <WitBoundSet> (this,
                                        "hardUB",
                            WitBoundSet::defHardUB (),
                          & WitBoundSet::hardUB,
                          & WitBoundSet::storeStochHardUB,
                        & WitScenAttMgr::hardUB);

   buildingInputAtts_ = false;
   }

//------------------------------------------------------------------------------

void WitScenAttMgr::buildScenSolnAtts ()
   {
   buildingSolnAtts_ = true;

   scrapVol_   =
      new WitScenSolnAtt <WitPart>      (this,
                                        "scrapVol",
                        & WitPart::      storeStochScrapVol);

   stockVol_   =
      new WitScenSolnAtt <WitMaterial>  (this,
                                        "stockVol",
                        & WitMaterial::  storeStochStockVol);

   shipVol_    =
      new WitScenSolnAtt <WitDemand>    (this,
                                        "shipVol",
                        & WitDemand::    storeStochShipVol);

   cumShipVol_ =
      new WitScenSolnAtt <WitDemand>    (this,
                                        "cumShipVol",
                        & WitDemand::    storeStochCumShipVol);

   execVol_    =
      new WitScenSolnAtt <WitOperation> (this,
                                        "execVol",
                        & WitOperation:: storeStochExecVol);

   subVol_     =
      new WitScenSolnAtt <WitSubEntry>  (this,
                                        "subVol",
                        & WitSubEntry::  storeStochSubVol);

   buildingSolnAtts_ = false;
   }
