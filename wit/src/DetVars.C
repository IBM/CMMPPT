//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "DetVars.C"
//
// Implementation the following classes:
//
//    DetVar
//    ScrapVar
//    NonSubVar
//    SubVar
//    ShipVar
//    BoundedVar
//    ExecVar
//    StockVar
//    CumShipVar
//    SlbvVar
//    MultiObjVar
//    NtbvVar
//------------------------------------------------------------------------------

#include <DetVars.h>
#include <DetImpOP.h>
#include <DetOptImpMgr.h>
#include <OptComp.h>
#include <Material.h>
#include <Operation.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <Demand.h>
#include <BoundSet.h>
#include <Objective.h>
#include <MsgFac.h>

#include <float.h>

//------------------------------------------------------------------------------
// Implementation of class DetVar.
//------------------------------------------------------------------------------

WitDetVar::WitDetVar (WitDetImpOP * theDetImpOP, WitPeriod thePer):

      WitOptVar   (theDetImpOP),
      myDetImpOP_ (theDetImpOP),
      myPeriod_   (thePer)
   {
   witAssert (thePer >= 0);
   }

//------------------------------------------------------------------------------

WitDetVar::WitDetVar (WitDetImpOP * theDetImpOP):

      WitOptVar   (theDetImpOP),
      myDetImpOP_ (theDetImpOP),
      myPeriod_   (-1)
   {
   }

//------------------------------------------------------------------------------

WitDetVar::~WitDetVar ()
   {
   }

//------------------------------------------------------------------------------

WitOptProblem * WitDetVar::myOptProblem ()
   {
   return myDetImpOP_;
   }

//------------------------------------------------------------------------------

WitDetVar * WitDetVar::mandDetVar ()
   {
   return this;
   }

//------------------------------------------------------------------------------

bool WitDetVar::isSlbvVar ()
   {
   return false;
   }

//------------------------------------------------------------------------------

void WitDetVar::printSource ()
   {
   printItem ();

   if (hasPeriod ())
      {
      myDetImpOP_->printPeriod (myPeriod ());
      }
   else
      {
      myDetImpOP_->printNonPeriod ();
      }
   }

//------------------------------------------------------------------------------
// Implementation of class ScrapVar.
//------------------------------------------------------------------------------

WitScrapVar::WitScrapVar (
         WitPart *     thePart,
         WitPeriod     t,
         WitDetImpOP * theDetImpOP):

      WitDetVar (theDetImpOP, t),

      myPart_   (thePart)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitScrapVar::~WitScrapVar ()
   {
   }

//------------------------------------------------------------------------------

double WitScrapVar::compObjCoeff ()
   {
   return - myPart ()->scrapCost ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

double WitScrapVar::upperBoundVal ()
   {
   return DBL_MAX;
   }

//------------------------------------------------------------------------------

double WitScrapVar::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitScrapVar::classText ()
   {
   return "Scrap";
   }

//------------------------------------------------------------------------------

void WitScrapVar::printItem ()
   {
   myDetImpOP ()->printItem (myPart ());
   }

//------------------------------------------------------------------------------

double WitScrapVar::indepInitValue ()
   {
   return myPart ()->scrapVol ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

void WitScrapVar::uploadSoln ()
   {
   myPart ()->storeOptScrapVol (myPeriod (), primalValue ());
   }

//------------------------------------------------------------------------------
// Implementation of class NonSubVar.
//------------------------------------------------------------------------------

WitNonSubVar::WitNonSubVar (
          WitBomEntry * theBomEnt,
          WitPeriod     t,
          WitDetImpOP * theDetImpOP):

      WitDetVar (theDetImpOP, t),

      myBomEnt_ (theBomEnt)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitNonSubVar::~WitNonSubVar ()
   {
   }

//------------------------------------------------------------------------------

double WitNonSubVar::compObjCoeff ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

double WitNonSubVar::upperBoundVal ()
   {
   return DBL_MAX;
   }

//------------------------------------------------------------------------------

double WitNonSubVar::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitNonSubVar::classText ()
   {
   return "Non-Sub";
   }

//------------------------------------------------------------------------------

void WitNonSubVar::printItem ()
   {
   myDetImpOP ()->printItem (myBomEnt ());
   }

//------------------------------------------------------------------------------

double WitNonSubVar::indepInitValue ()
   {
   return myBomEnt ()->nonSubVol ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

void WitNonSubVar::uploadSoln ()
   {
   myBomEnt ()->storeOptNonSubVol (myPeriod (), primalValue ());
   }

//------------------------------------------------------------------------------
// Implementation of class SubVar.
//------------------------------------------------------------------------------

WitSubVar::WitSubVar (
          WitSubEntry * theSub,
          WitPeriod     t,
          WitDetImpOP * theDetImpOP):

      WitDetVar (theDetImpOP, t),

      mySub_    (theSub)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitSubVar::~WitSubVar ()
   {
   }

//------------------------------------------------------------------------------

double WitSubVar::compObjCoeff ()
   {
   return - mySub ()->subCost ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

double WitSubVar::upperBoundVal ()
   {
   return DBL_MAX;
   }

//------------------------------------------------------------------------------

double WitSubVar::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitSubVar::classText ()
   {
   return "Substitute";
   }

//------------------------------------------------------------------------------

void WitSubVar::printItem ()
   {
   myDetImpOP ()->printItem (mySub ());
   }

//------------------------------------------------------------------------------

double WitSubVar::indepInitValue ()
   {
   return mySub ()->subVol ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

bool WitSubVar::isAnIntVar ()
   {
   return mySub ()->intSubVols ();
   }

//------------------------------------------------------------------------------

void WitSubVar::uploadSoln ()
   {
   mySub ()->storeOptSubVol (myPeriod (), primalValue ());
   }

//------------------------------------------------------------------------------
// Implementation of class ShipVar.
//------------------------------------------------------------------------------

WitShipVar::WitShipVar (
         WitDemand *   theDemand,
         WitPeriod     t,
         WitDetImpOP * theDetImpOP):

      WitDetVar (theDetImpOP, t),

      myDemand_ (theDemand)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitShipVar::~WitShipVar ()
   {
   }

//------------------------------------------------------------------------------

double WitShipVar::compObjCoeff ()
   {
   return + myDemand ()->shipReward ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

double WitShipVar::upperBoundVal ()
   {
   return
      myDemand ()->shipLateAllowed ()?
         DBL_MAX:
         myDemand ()->demandVol ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

double WitShipVar::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitShipVar::classText ()
   {
   return "Shipment";
   }

//------------------------------------------------------------------------------

void WitShipVar::printItem ()
   {
   myDetImpOP ()->printItem (myDemand ());
   }

//------------------------------------------------------------------------------

double WitShipVar::indepInitValue ()
   {
   return myDemand ()->shipVol ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

bool WitShipVar::isAnIntVar ()
   {
   return myDemand ()->intShipVols ();
   }

//------------------------------------------------------------------------------

void WitShipVar::uploadSoln ()
   {
   myDemand ()->storeOptShipVol (myPeriod (), primalValue ());
   }

//------------------------------------------------------------------------------
// Implementation of class BoundedVar.
//------------------------------------------------------------------------------

WitBoundedVar::WitBoundedVar (WitDetImpOP * theDetImpOP, WitPeriod t):

      WitDetVar (theDetImpOP, t)
   {
   myDetImpOP ()->storeBoundedVar (this);
   }

//------------------------------------------------------------------------------

WitBoundedVar::~WitBoundedVar ()
   {
   }

//------------------------------------------------------------------------------

double WitBoundedVar::hardLB ()
   {
   return myBoundSet ()->hardLB () [myPeriod ()];
   }

//------------------------------------------------------------------------------

double WitBoundedVar::softLB ()
   {
   return myBoundSet ()->softLB () [myPeriod ()];
   }

//------------------------------------------------------------------------------

double WitBoundedVar::hardUB ()
   {
   return
      (myBoundSet ()->hardUBIsFinite (myPeriod ()))?
         myBoundSet ()->hardUB () [myPeriod ()]:
         + DBL_MAX;
   }

//------------------------------------------------------------------------------

bool WitBoundedVar::needsAnSlbvVar ()
   {
   if (myProblem ()->myOptComp ()->accAfterSoftLB ())
      return true;

   if (softLB () > hardLB () + FLOAT_EPSILON)
      return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitBoundedVar::printBoundInfo ()
   {
   fprintf      (myDetImpOP ()->optProbFile (), "%8d",            index ());
   fprintf      (myDetImpOP ()->optProbFile (), " %9.1f",         hardLB ());
   fprintf      (myDetImpOP ()->optProbFile (), " %9.1f",         softLB ());
   writeExtReal (myDetImpOP ()->optProbFile (), " %9.1f", " %9s", hardUB ());
   fprintf      (myDetImpOP ()->optProbFile (), "\n");
   }

//------------------------------------------------------------------------------

void WitBoundedVar::printSoln ()
   {
   WitSlbvVar * theSlbvVar;
   double       slbvVal;

   theSlbvVar = myDetImpOP ()->mySlbvVar (this);

   slbvVal =
      (theSlbvVar == NULL)?
         0.0:
         theSlbvVar->primalValue ();

   fprintf      (myDetImpOP ()->solnFile (), " %8d",   index ());
   fprintf      (myDetImpOP ()->solnFile (), " %9.1f", hardLB ());
   fprintf      (myDetImpOP ()->solnFile (), " %9.1f", softLB ());
   fprintf      (myDetImpOP ()->solnFile (), " %9.1f", slbvVal);
   fprintf      (myDetImpOP ()->solnFile (), " %9.1f", primalValue ());
   writeExtReal (myDetImpOP ()->solnFile (), " %9.1f", " %9s",   hardUB ());
   fprintf      (myDetImpOP ()->solnFile (), "\n");
   }

//------------------------------------------------------------------------------
// Implementation of class ExecVar.
//------------------------------------------------------------------------------

WitExecVar::WitExecVar (
         WitOperation * theOpn,
         WitPeriod      thePer,
         WitDetImpOP *  theDetImpOP):

      WitBoundedVar (theDetImpOP, thePer),

      myOperation_  (theOpn)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitExecVar::~WitExecVar ()
   {
   }

//------------------------------------------------------------------------------

double WitExecVar::compObjCoeff ()
   {
   return - myOperation ()->execCost ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

double WitExecVar::upperBoundVal ()
   {
   return hardUB ();
   }

//------------------------------------------------------------------------------

double WitExecVar::lowerBoundVal ()
   {
   return hardLB ();
   }

//------------------------------------------------------------------------------

const char * WitExecVar::classText ()
   {
   return "Execution";
   }

//------------------------------------------------------------------------------

void WitExecVar::printItem ()
   {
   myDetImpOP ()->printItem (myOperation ());
   }

//------------------------------------------------------------------------------

double WitExecVar::indepInitValue ()
   {
   return myOperation ()->execVol ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

bool WitExecVar::isAnIntVar ()
   {
   return myOperation ()->intExecVols ();
   }

//------------------------------------------------------------------------------

void WitExecVar::uploadSoln ()
   {
   myOperation ()->storeOptExecVol (myPeriod (), primalValue ());
   }

//------------------------------------------------------------------------------

WitBoundSet * WitExecVar::myBoundSet ()
   {
   return myOperation ()->execBounds ();
   }

//------------------------------------------------------------------------------
// Implementation of class StockVar.
//------------------------------------------------------------------------------

WitStockVar::WitStockVar (
         WitMaterial * theMat,
         WitPeriod     t,
         WitDetImpOP * theDetImpOP):

      WitBoundedVar (theDetImpOP, t),

      myMaterial_   (theMat)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitStockVar::~WitStockVar ()
   {
   }

//------------------------------------------------------------------------------

double WitStockVar::compObjCoeff ()
   {
   return - myMaterial ()->stockCost ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

double WitStockVar::upperBoundVal ()
   {
   return hardUB ();
   }

//------------------------------------------------------------------------------

double WitStockVar::lowerBoundVal ()
   {
   return hardLB ();
   }

//------------------------------------------------------------------------------

const char * WitStockVar::classText ()
   {
   return "Stock";
   }

//------------------------------------------------------------------------------

void WitStockVar::printItem ()
   {
   myDetImpOP ()->printItem (myMaterial ());
   }

//------------------------------------------------------------------------------

double WitStockVar::indepInitValue ()
   {
   return myMaterial ()->stockVol ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

void WitStockVar::uploadSoln ()
   {
   myMaterial ()->storeOptStockVol (myPeriod (), primalValue ());
   }

//------------------------------------------------------------------------------

WitBoundSet * WitStockVar::myBoundSet ()
   {
   return myMaterial ()->stockBounds ();
   }

//------------------------------------------------------------------------------
// Implementation of class CumShipVar.
//------------------------------------------------------------------------------

WitCumShipVar::WitCumShipVar (
         WitDemand *   theDemand,
         WitPeriod     t,
         WitDetImpOP * theDetImpOP):

      WitBoundedVar (theDetImpOP, t),

      myDemand_     (theDemand)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitCumShipVar::~WitCumShipVar ()
   {
   }

//------------------------------------------------------------------------------

double WitCumShipVar::compObjCoeff ()
   {
   return + myDemand ()->cumShipReward ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

double WitCumShipVar::upperBoundVal ()
   {
   return
      min (
         myDemand ()->cumDemandVol ()[myPeriod ()],
         hardUB ());
   }

//------------------------------------------------------------------------------

double WitCumShipVar::lowerBoundVal ()
   {
   return hardLB ();
   }

//------------------------------------------------------------------------------

const char * WitCumShipVar::classText ()
   {
   return "Cum. Ship";
   }

//------------------------------------------------------------------------------

void WitCumShipVar::printItem ()
   {
   myDetImpOP ()->printItem (myDemand ());
   }

//------------------------------------------------------------------------------

double WitCumShipVar::indepInitValue ()
   {
   return myDemand ()->cumShipVol ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

void WitCumShipVar::uploadSoln ()
   {
   myDemand ()->storeOptCumShipVol (myPeriod (), primalValue ());
   }

//------------------------------------------------------------------------------

WitBoundSet * WitCumShipVar::myBoundSet ()
   {
   return myDemand ()->cumShipBounds ();
   }

//------------------------------------------------------------------------------
// Implementation of class SlbvVar.
//------------------------------------------------------------------------------

WitSlbvVar::WitSlbvVar (
         WitBoundedVar * theBoundedVar,
         WitDetImpOP *   theDetImpOP):

      WitDetVar     (theDetImpOP, theBoundedVar->myPeriod ()),

      myBoundedVar_ (theBoundedVar)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitSlbvVar::~WitSlbvVar ()
   {
   }

//------------------------------------------------------------------------------

double WitSlbvVar::compObjCoeff ()
   {
   return - myProblem ()->myOptComp ()->wbounds ();
   }

//------------------------------------------------------------------------------

double WitSlbvVar::upperBoundVal ()
   {
   return DBL_MAX;
   }

//------------------------------------------------------------------------------

double WitSlbvVar::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitSlbvVar::classText ()
   {
   return "S. L. B. V.";
   }

//------------------------------------------------------------------------------

void WitSlbvVar::printItem ()
   {
   myDetImpOP ()->printItem (myBoundedVar_);
   }

//------------------------------------------------------------------------------

void WitSlbvVar::uploadSoln ()
   {
   }

//------------------------------------------------------------------------------

bool WitSlbvVar::isSlbvVar ()
   {
   return true;
   }

//------------------------------------------------------------------------------
// Implementation of class MultiObjVar.
//------------------------------------------------------------------------------

WitMultiObjVar::WitMultiObjVar (
         WitObjective * theObj,
         WitDetImpOP *  theDetImpOP):

      WitDetVar (theDetImpOP),
      myObj_    (theObj)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitMultiObjVar::~WitMultiObjVar ()
   {
   }

//------------------------------------------------------------------------------

double WitMultiObjVar::compObjCoeff ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

void WitMultiObjVar::uploadSoln ()
   {
   }

//------------------------------------------------------------------------------

const char * WitMultiObjVar::lexObjElemName ()
   {
   return myObj_->myObjName ().myCstring ();
   }

//------------------------------------------------------------------------------

void WitMultiObjVar::reportLexUnbounded ()
   {
   if (myObj_->hasNegativeCosts ())
      myDetImpOP ()->myMsgFac () ("lexUnboundedSmsg", myObj_->myObjName ());
   }

//------------------------------------------------------------------------------

double WitMultiObjVar::upperBoundVal ()
   {
   return DBL_MAX;
   }

//------------------------------------------------------------------------------

double WitMultiObjVar::lowerBoundVal ()
   {
   return - DBL_MAX;
   }

//------------------------------------------------------------------------------

const char * WitMultiObjVar::classText ()
   {
   return "Multi-Obj";
   }

//------------------------------------------------------------------------------

void WitMultiObjVar::printItem ()
   {
   myDetImpOP ()->printItem (myObj_);
   }

//------------------------------------------------------------------------------
// Implementation of class NtbvVar.
//------------------------------------------------------------------------------

WitNtbvVar::WitNtbvVar (WitDetImpOP * theDetImpOP):

      WitDetVar (theDetImpOP)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitNtbvVar::~WitNtbvVar ()
   {
   }

//------------------------------------------------------------------------------

double WitNtbvVar::compObjCoeff ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

void WitNtbvVar::uploadSoln ()
   {
   }

//------------------------------------------------------------------------------

const char * WitNtbvVar::lexObjElemName ()
   {
   return "Total Soft Lower Bounds Violation";
   }

//------------------------------------------------------------------------------

void WitNtbvVar::reportLexUnbounded ()
   {
   }

//------------------------------------------------------------------------------

double WitNtbvVar::upperBoundVal ()
   {
   return DBL_MAX;
   }

//------------------------------------------------------------------------------

double WitNtbvVar::lowerBoundVal ()
   {
   return - DBL_MAX;
   }

//------------------------------------------------------------------------------

const char * WitNtbvVar::classText ()
   {
   return "T. B. V.";
   }

//------------------------------------------------------------------------------

void WitNtbvVar::printItem ()
   {
   myDetImpOP ()->printItem ();
   }
