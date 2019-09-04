//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "StVars.C"
//
// Implementation the following classes:
//
//    StScrapVar
//    StNonSubVar
//    StSubVar
//    StShipVar
//    StBddVar
//    StExecVar
//    StStockVar
//    StCumShipVar
//    StShipVar
//    StSlbvVar
//------------------------------------------------------------------------------

#include <StVars.h>
#include <StCons.h>
#include <StochImpOP.h>
#include <StochModeMgr.h>
#include <Scenario.h>
#include <ScenAttMgr.h>
#include <ScenInputAtt.h>
#include <ScenSolnAtt.h>
#include <OptComp.h>
#include <Material.h>
#include <Operation.h>
#include <Demand.h>
#include <SubEntry.h>

#include <float.h>

//------------------------------------------------------------------------------
// Implementation of class StScrapVar.
//------------------------------------------------------------------------------

WitStScrapVar::WitStScrapVar (WitPart * thePart, WitStochLoc * theStochLoc):

      WitStochVar (theStochLoc),
      myPart_     (thePart)
   {
   }

//------------------------------------------------------------------------------

WitStScrapVar::~WitStScrapVar ()
   {
   }

//------------------------------------------------------------------------------

double WitStScrapVar::compObjCoeff ()
   {
   double scrapCostVal;

   scrapCostVal =
      myScenAttMgr ()->
         scrapCost ()->
            myValueFor (myPart_, firstScenario ())[myPer ()];

   return - scrapCostVal * totalProb ();
   }

//------------------------------------------------------------------------------

void WitStScrapVar::uploadSolnTo (WitScenario * theScenario)
   {
   myScenAttMgr ()->
      scrapVol ()->
         storeValueFor (myPart_, theScenario, myPer (), primalValue ());
   }

//------------------------------------------------------------------------------

double WitStScrapVar::upperBoundVal ()
   {
   return DBL_MAX;
   }

//------------------------------------------------------------------------------

double WitStScrapVar::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitStScrapVar::classText ()
   {
   return "Scrap";
   }

//------------------------------------------------------------------------------

void WitStScrapVar::printItem ()
   {
   myStochImpOP ()->printItem (myPart_);
   }

//------------------------------------------------------------------------------
// Implementation of class StNonSubVar.
//------------------------------------------------------------------------------

WitStNonSubVar::WitStNonSubVar (
         WitBomEntry * theBomEnt,
         WitStochLoc * theStochLoc):

      WitStochVar (theStochLoc),
      myBomEnt_   (theBomEnt)
   {
   }

//------------------------------------------------------------------------------

WitStNonSubVar::~WitStNonSubVar ()
   {
   }

//------------------------------------------------------------------------------

double WitStNonSubVar::compObjCoeff ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

void WitStNonSubVar::uploadSolnTo (WitScenario *)
   {
   }

//------------------------------------------------------------------------------

double WitStNonSubVar::upperBoundVal ()
   {
   return DBL_MAX;
   }

//------------------------------------------------------------------------------

double WitStNonSubVar::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitStNonSubVar::classText ()
   {
   return "Non-Sub";
   }

//------------------------------------------------------------------------------

void WitStNonSubVar::printItem ()
   {
   myStochImpOP ()->printItem (myBomEnt_);
   }

//------------------------------------------------------------------------------
// Implementation of class StSubVar.
//------------------------------------------------------------------------------

WitStSubVar::WitStSubVar (WitSubEntry * theSub, WitStochLoc * theStochLoc):

      WitStochVar (theStochLoc),
      mySub_      (theSub)
   {
   }

//------------------------------------------------------------------------------

WitStSubVar::~WitStSubVar ()
   {
   }

//------------------------------------------------------------------------------

double WitStSubVar::compObjCoeff ()
   {
   double subCostVal;

   subCostVal =
      myScenAttMgr ()->
         subCost ()->
            myValueFor (mySub_, firstScenario ())[myPer ()];

   return - subCostVal * totalProb ();
   }

//------------------------------------------------------------------------------

void WitStSubVar::uploadSolnTo (WitScenario * theScenario)
   {
   myScenAttMgr ()->
      subVol ()->
         storeValueFor (mySub_, theScenario, myPer (), primalValue ());
   }

//------------------------------------------------------------------------------

double WitStSubVar::upperBoundVal ()
   {
   return DBL_MAX;
   }

//------------------------------------------------------------------------------

double WitStSubVar::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitStSubVar::classText ()
   {
   return "Substitute";
   }

//------------------------------------------------------------------------------

void WitStSubVar::printItem ()
   {
   myStochImpOP ()->printItem (mySub_);
   }

//------------------------------------------------------------------------------
// Implementation of class StShipVar.
//------------------------------------------------------------------------------

WitStShipVar::WitStShipVar (WitDemand * theDemand, WitStochLoc * theStochLoc):

      WitStochVar (theStochLoc),
      myDemand_   (theDemand)
   {
   }

//------------------------------------------------------------------------------

WitStShipVar::~WitStShipVar ()
   {
   }

//------------------------------------------------------------------------------

double WitStShipVar::compObjCoeff ()
   {
   double shipRewardVal;

   shipRewardVal =
      myScenAttMgr ()->
         shipReward ()->
            myValueFor (myDemand_, firstScenario ())[myPer ()];

   return shipRewardVal * totalProb ();
   }

//------------------------------------------------------------------------------

void WitStShipVar::uploadSolnTo (WitScenario * theScenario)
   {
   myScenAttMgr ()->
      shipVol ()->
         storeValueFor (myDemand_, theScenario, myPer (), primalValue ());
   }

//------------------------------------------------------------------------------

double WitStShipVar::upperBoundVal ()
   {
   if (myDemand_->shipLateAllowed ())
      return DBL_MAX;
   else
      return
         myScenAttMgr ()->
            demandVol ()->
               myValueFor (myDemand_, firstScenario ())[myPer ()];
   }

//------------------------------------------------------------------------------

double WitStShipVar::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitStShipVar::classText ()
   {
   return "Shipment";
   }

//------------------------------------------------------------------------------

void WitStShipVar::printItem ()
   {
   myStochImpOP ()->printItem (myDemand_);
   }

//------------------------------------------------------------------------------
// Implementation of class StBddVar.
//------------------------------------------------------------------------------

void WitStBddVar::genStSlbvVarIfNeeded ()
   {
   double softLBVal;

   softLBVal =
      myScenAttMgr ()->
         softLB ()->
            myValueFor (myBoundSet (), firstScenario ())[myPer ()];

   if (softLBVal > hardLB () + FLOAT_EPSILON)
      new WitStSlbvVar (this);
   }

//------------------------------------------------------------------------------

WitStBddVar::WitStBddVar (WitStochLoc * theStochLoc):

      WitStochVar (theStochLoc)
   {
   }

//------------------------------------------------------------------------------

WitStBddVar::~WitStBddVar ()
   {
   }

//------------------------------------------------------------------------------

double WitStBddVar::hardLB ()
   {
   return
      myScenAttMgr ()->
         hardLB ()->
            myValueFor (myBoundSet (), firstScenario ())[myPer ()];
   }

//------------------------------------------------------------------------------

double WitStBddVar::hardUB ()
   {
   double hardUBVal;

   hardUBVal =
      myScenAttMgr ()->
         hardUB ()->
            myValueFor (myBoundSet (), firstScenario ())[myPer ()];

   if (hardUBVal >= 0.0)
      return hardUBVal;
   else
      return DBL_MAX;
   }

//------------------------------------------------------------------------------
// Implementation of class StExecVar.
//------------------------------------------------------------------------------

WitStExecVar::WitStExecVar (WitOperation * theOpn, WitStochLoc * theStochLoc):

      WitStBddVar (theStochLoc),
      myOpn_      (theOpn)
   {
   }

//------------------------------------------------------------------------------

WitStExecVar::~WitStExecVar ()
   {
   }

//------------------------------------------------------------------------------

double WitStExecVar::compObjCoeff ()
   {
   double execCostVal;

   execCostVal =
      myScenAttMgr ()->
         execCost ()->
            myValueFor (myOpn_, firstScenario ())[myPer ()];

   return - execCostVal * totalProb ();
   }

//------------------------------------------------------------------------------

void WitStExecVar::uploadSolnTo (WitScenario * theScenario)
   {
   myScenAttMgr ()->
      execVol ()->
         storeValueFor (myOpn_, theScenario, myPer (), primalValue ());
   }

//------------------------------------------------------------------------------

WitBoundSet * WitStExecVar::myBoundSet ()
   {
   return myOpn_->execBounds ();
   }

//------------------------------------------------------------------------------

double WitStExecVar::upperBoundVal ()
   {
   return hardUB ();
   }

//------------------------------------------------------------------------------

double WitStExecVar::lowerBoundVal ()
   {
   return hardLB ();
   }

//------------------------------------------------------------------------------

const char * WitStExecVar::classText ()
   {
   return "Execution";
   }

//------------------------------------------------------------------------------

void WitStExecVar::printItem ()
   {
   myStochImpOP ()->printItem (myOpn_);
   }

//------------------------------------------------------------------------------
// Implementation of class StStockVar.
//------------------------------------------------------------------------------

WitStStockVar::WitStStockVar (WitMaterial * theMat, WitStochLoc * theStochLoc):

      WitStBddVar (theStochLoc),
      myMat_      (theMat)
   {
   }

//------------------------------------------------------------------------------

WitStStockVar::~WitStStockVar ()
   {
   }

//------------------------------------------------------------------------------

double WitStStockVar::compObjCoeff ()
   {
   double stockCostVal;

   stockCostVal =
      myScenAttMgr ()->
         stockCost ()->
            myValueFor (myMat_, firstScenario ())[myPer ()];

   return - stockCostVal * totalProb ();
   }

//------------------------------------------------------------------------------

void WitStStockVar::uploadSolnTo (WitScenario * theScenario)
   {
   myScenAttMgr ()->
      stockVol ()->
         storeValueFor (myMat_, theScenario, myPer (), primalValue ());
   }

//------------------------------------------------------------------------------

WitBoundSet * WitStStockVar::myBoundSet ()
   {
   return myMat_->stockBounds ();
   }

//------------------------------------------------------------------------------

double WitStStockVar::upperBoundVal ()
   {
   return hardUB ();
   }

//------------------------------------------------------------------------------

double WitStStockVar::lowerBoundVal ()
   {
   return hardLB ();
   }

//------------------------------------------------------------------------------

const char * WitStStockVar::classText ()
   {
   return "Stock";
   }

//------------------------------------------------------------------------------

void WitStStockVar::printItem ()
   {
   myStochImpOP ()->printItem (myMat_);
   }

//------------------------------------------------------------------------------
// Implementation of class StCumShipVar.
//------------------------------------------------------------------------------

WitStCumShipVar::WitStCumShipVar (
         WitDemand *   theDemand,
         WitStochLoc * theStochLoc):

      WitStBddVar (theStochLoc),
      myDemand_   (theDemand)
   {
   }

//------------------------------------------------------------------------------

WitStCumShipVar::~WitStCumShipVar ()
   {
   }

//------------------------------------------------------------------------------

double WitStCumShipVar::compObjCoeff ()
   {
   double cumShipRewardVal;

   cumShipRewardVal =
      myScenAttMgr ()->
         cumShipReward ()->
            myValueFor (myDemand_, firstScenario ())[myPer ()];

   return cumShipRewardVal * totalProb ();
   }

//------------------------------------------------------------------------------

void WitStCumShipVar::uploadSolnTo (WitScenario * theScenario)
   {
   myScenAttMgr ()->
      cumShipVol ()->
         storeValueFor (myDemand_, theScenario, myPer (), primalValue ());
   }

//------------------------------------------------------------------------------

WitBoundSet * WitStCumShipVar::myBoundSet ()
   {
   return myDemand_->cumShipBounds ();
   }

//------------------------------------------------------------------------------

double WitStCumShipVar::upperBoundVal ()
   {
   return
      min (
         myScenGroup ()->cumDemandVol () (myDemand_)[myPer ()],
         hardUB ());
   }

//------------------------------------------------------------------------------

double WitStCumShipVar::lowerBoundVal ()
   {
   return hardLB ();
   }

//------------------------------------------------------------------------------

const char * WitStCumShipVar::classText ()
   {
   return "Cum. Ship";
   }

//------------------------------------------------------------------------------

void WitStCumShipVar::printItem ()
   {
   myStochImpOP ()->printItem (myDemand_);
   }

//------------------------------------------------------------------------------
// Implementation of class StShipVar.
//------------------------------------------------------------------------------

WitStSlbvVar::WitStSlbvVar (WitStBddVar * theBddVar):

      WitStochVar (theBddVar->myStochLoc ()),
      myBddVar_   (theBddVar)
   {
   }

//------------------------------------------------------------------------------

WitStSlbvVar::~WitStSlbvVar ()
   {
   }

//------------------------------------------------------------------------------

double WitStSlbvVar::compObjCoeff ()
   {
   return - myStochLoc ()->myOptComp ()->wbounds () * totalProb ();
   }

//------------------------------------------------------------------------------

void WitStSlbvVar::genStSlbConIfNeeded ()
   {
   new WitStSlbCon (this);
   }

//------------------------------------------------------------------------------

double WitStSlbvVar::stBoundsCoeffVal ()
   {
   return totalProb ();
   }

//------------------------------------------------------------------------------

void WitStSlbvVar::uploadSolnTo (WitScenario *)
   {
   }

//------------------------------------------------------------------------------

bool WitStSlbvVar::isStSlbvVar ()
   {
   return true;
   }

//------------------------------------------------------------------------------

double WitStSlbvVar::upperBoundVal ()
   {
   return DBL_MAX;
   }

//------------------------------------------------------------------------------

double WitStSlbvVar::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitStSlbvVar::classText ()
   {
   return "S. L. B. V.";
   }

//------------------------------------------------------------------------------

void WitStSlbvVar::printItem ()
   {
   myStochImpOP ()->printItem (myBddVar_);
   }
