//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "DetCons.C"
//
// Implementation the following hierarchy of classes:
//
//    DetCon
//       ResourceCon
//       SubCon
//       ShipCon
//       SlbCon
//       MultiObjCon
//       TbvCon
//------------------------------------------------------------------------------

#include <DetCons.h>
#include <DetImpOP.h>
#include <DetOptImpMgr.h>
#include <DetVars.h>
#include <OptComp.h>
#include <Material.h>
#include <Operation.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <ExecPerSch.h>
#include <MultiObjMgr.h>

#include <float.h>

//------------------------------------------------------------------------------
// Implementation of class DetCon.
//------------------------------------------------------------------------------

WitDetCon::WitDetCon (WitDetImpOP * theDetImpOP, WitPeriod thePer):

      WitOptCon   (theDetImpOP),
      myDetImpOP_ (theDetImpOP),
      myPeriod_   (thePer)
   {
   witAssert (thePer >= 0);
   }

//------------------------------------------------------------------------------

WitDetCon::WitDetCon (WitDetImpOP * theDetImpOP):

      WitOptCon   (theDetImpOP),
      myDetImpOP_ (theDetImpOP),
      myPeriod_   (-1)
   {
   }

//------------------------------------------------------------------------------

WitDetCon::~WitDetCon ()
   {
   }

//------------------------------------------------------------------------------

WitOptProblem * WitDetCon::myOptProblem ()
   {
   return myDetImpOP_;
   }

//------------------------------------------------------------------------------

void WitDetCon::printSource ()
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
// Implementation of class ResourceCon.
//------------------------------------------------------------------------------

WitResourceCon::WitResourceCon (
         WitPart *     thePart,
         WitPeriod     t,
         WitDetImpOP * theDetImpOP):

      WitDetCon (theDetImpOP, t),

      myPart_   (thePart)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitResourceCon::~WitResourceCon ()
   {
   }

//------------------------------------------------------------------------------

double WitResourceCon::upperBoundVal ()
   {
   return myPart_->supplyVol ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

double WitResourceCon::lowerBoundVal ()
   {
   return myPart_->supplyVol ()[myPeriod ()];
   }

//------------------------------------------------------------------------------

const char * WitResourceCon::classText ()
   {
   return "Resource";
   }

//------------------------------------------------------------------------------

void WitResourceCon::printItem ()
   {
   myDetImpOP ()->printItem (myPart ());
   }

//------------------------------------------------------------------------------

void WitResourceCon::generateCoeffs ()
   {
   WitPeriod     thePer;
   WitMaterial * theMat;
   WitDemand *   theDemand;
   WitScrapVar * theScrapVar;

   thePer = myPeriod ();

   theMat = myPart ()->thisMat ();

   if (theMat != NULL)
      {
      if (theMat->canStock (thePer))
         myDetImpOP ()->myStockVar (theMat, thePer)->incCoeff (1.0);

      if (thePer > 0)
         if (theMat->canStock (thePer - 1))
            myDetImpOP ()->myStockVar (theMat, thePer - 1)->incCoeff (-1.0);
      }

   forEachEl (theDemand, myPart ()->myDemands ())
      myDetImpOP ()->myShipVar (theDemand, thePer)->incCoeff (1.0);

   theScrapVar = myDetImpOP ()->myScrapVar (myPart (), thePer);

   if (theScrapVar != NULL)
      theScrapVar->incCoeff (1.0);

   genBillEntCoeffs ();
   }

//------------------------------------------------------------------------------

void WitResourceCon::genBillEntCoeffs ()
   {
   WitBomEntry * theBomEnt;
   WitSubEntry * theSub;
   WitBopEntry * theBopEnt;
   WitPeriod     execPerF;
   WitPeriod     execPerL;
   WitPeriod     execPer;
   WitOptVar *   theOptVar;

   forEachEl (theBomEnt, myPart ()->consumingBomEntries ())
      {
      getExecPerRange (theBomEnt, execPerF, execPerL);

      for (execPer = execPerL; execPer >= execPerF; execPer --)
         if (theBomEnt->impactPeriod ()[execPer] == myPeriod ())
            {
            if (theBomEnt->hasSubsInEffect ()[execPer])
               theOptVar = myDetImpOP ()->myNonSubVar (theBomEnt, execPer);
            else
               theOptVar =
                  myDetImpOP ()->myExecVar (theBomEnt->myOperation (), execPer);

            theOptVar->incCoeff (theBomEnt->effConsRate ()[execPer]);
            }
      }

   forEachEl (theSub, myPart ()->consumingSubEntries ())
      {
      getExecPerRange (theSub, execPerF, execPerL);

      for (execPer = execPerL; execPer >= execPerF; execPer --)
         if (theSub->impactPeriod ()[execPer] == myPeriod ())
            myDetImpOP ()->mySubVar (theSub, execPer)->
               incCoeff (theSub->effConsRate ()[execPer]);
      }

   forEachEl (theBopEnt, myPart ()->producingBopEntries ())
      {
      getExecPerRange (theBopEnt, execPerF, execPerL);

      for (execPer = execPerL; execPer >= execPerF; execPer --)
         if (theBopEnt->impactPeriod ()[execPer] == myPeriod ())
            myDetImpOP ()->
               myExecVar (theBopEnt->myOperation (), execPer)->
                  incCoeff (- theBopEnt->effProdRate ()[execPer]);
      }
   }

//------------------------------------------------------------------------------

void WitResourceCon::getExecPerRange (
      WitBillEntry * theBillEnt,
      WitPeriod &    execPerF,
      WitPeriod &    execPerL)
   {
   myDetImpOP ()->
      myExecPerSched ()->
         getExecPerRange (theBillEnt, myPeriod (), execPerF, execPerL);
   }

//------------------------------------------------------------------------------
// Implementation of class SubCon.
//------------------------------------------------------------------------------

WitSubCon::WitSubCon (
         WitBomEntry * theBomEnt,
         WitPeriod     t,
         WitDetImpOP * theDetImpOP):

      WitDetCon (theDetImpOP, t),

      myBomEnt_ (theBomEnt)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitSubCon::~WitSubCon ()
   {
   }

//------------------------------------------------------------------------------

double WitSubCon::upperBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

double WitSubCon::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitSubCon::classText ()
   {
   return "Substitution";
   }

//------------------------------------------------------------------------------

void WitSubCon::printItem ()
   {
   myDetImpOP ()->printItem (myBomEnt ());
   }

//------------------------------------------------------------------------------

void WitSubCon::generateCoeffs ()
   {
   WitPeriod     thePer;
   WitSubEntry * theSub;

   thePer = myPeriod ();

   forEachEl (theSub, myBomEnt ()->mySubEntries ())
      if (theSub->inEffect (thePer))
         myDetImpOP ()->mySubVar (theSub, thePer)->incCoeff (1.0);

   myDetImpOP ()->myNonSubVar (myBomEnt (), thePer)->incCoeff (1.0);

   myDetImpOP ()->
      myExecVar (myBomEnt ()->myOperation (), thePer)->
         incCoeff (-1.0);
   }

//------------------------------------------------------------------------------
// Implementation of class ShipCon.
//------------------------------------------------------------------------------

WitShipCon::WitShipCon (
         WitDemand *   theDemand,
         WitPeriod     t,
         WitDetImpOP * theDetImpOP):

      WitDetCon (theDetImpOP, t),

      myDemand_ (theDemand)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitShipCon::~WitShipCon ()
   {
   }

//------------------------------------------------------------------------------

double WitShipCon::upperBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

double WitShipCon::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitShipCon::classText ()
   {
   return "Shipment";
   }

//------------------------------------------------------------------------------

void WitShipCon::printItem ()
   {
   myDetImpOP ()->printItem (myDemand ());
   }

//------------------------------------------------------------------------------

void WitShipCon::generateCoeffs ()
   {
   WitPeriod thePer;

   thePer = myPeriod ();

   if (thePer > 0)
      myDetImpOP ()->myCumShipVar (myDemand (), thePer - 1)->incCoeff  (1.0);

   myDetImpOP ()   ->myShipVar    (myDemand (), thePer)    ->incCoeff  (1.0);

   myDetImpOP ()   ->myCumShipVar (myDemand (), thePer)    ->incCoeff (-1.0);
   }

//------------------------------------------------------------------------------
// Implementation of class SlbCon.
//------------------------------------------------------------------------------

WitSlbCon::WitSlbCon (
         WitBoundedVar * theBoundedVar,
         WitDetImpOP *   theDetImpOP):

      WitDetCon     (theDetImpOP, theBoundedVar->myPeriod ()),

      myBoundedVar_ (theBoundedVar)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitSlbCon::~WitSlbCon ()
   {
   }

//------------------------------------------------------------------------------

WitBoundSet * WitSlbCon::myBoundSet ()
   {
   return myBoundedVar_->myBoundSet ();
   }

//------------------------------------------------------------------------------

double WitSlbCon::upperBoundVal ()
   {
   return DBL_MAX;
   }

//------------------------------------------------------------------------------

double WitSlbCon::lowerBoundVal ()
   {
   return myBoundedVar_->softLB ();
   }

//------------------------------------------------------------------------------

const char * WitSlbCon::classText ()
   {
   return "S. L. B.";
   }

//------------------------------------------------------------------------------

void WitSlbCon::printItem ()
   {
   myDetImpOP ()->printItem (myBoundedVar_);
   }

//------------------------------------------------------------------------------

void WitSlbCon::generateCoeffs ()
   {
   myBoundedVar_->incCoeff (1.0);

   myDetImpOP ()->mySlbvVar (myBoundedVar_)->incCoeff (1.0);
   }

//------------------------------------------------------------------------------
// Implementation of class MultiObjCon.
//------------------------------------------------------------------------------

WitMultiObjCon::WitMultiObjCon (
         WitObjective * theObj,
         WitDetImpOP *  theDetImpOP):

      WitDetCon (theDetImpOP),
      myObj_    (theObj)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitMultiObjCon::~WitMultiObjCon ()
   {
   }

//------------------------------------------------------------------------------

double WitMultiObjCon::upperBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

double WitMultiObjCon::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitMultiObjCon::classText ()
   {
   return "Multi-Obj";
   }

//------------------------------------------------------------------------------

void WitMultiObjCon::printItem ()
   {
   myDetImpOP ()->printItem (myObj_);
   }

//------------------------------------------------------------------------------

void WitMultiObjCon::generateCoeffs ()
   {
   WitMultiObjMgr * theMultiObjMgr;
   WitObjective *   savedObj;
   WitOptVar *      theOptVar;
   double           theObjCoeff;

   theMultiObjMgr = myDetImpOP ()->myOptComp ()->myMultiObjMgr ();

   myDetImpOP ()->myMultiObjVar (myObj_)->incCoeff (-1.0);

   savedObj       = theMultiObjMgr->currentObjective ();

   theMultiObjMgr->setCurrentObjective (myObj_);

   forEachEl (theOptVar, myDetImpOP ()->myOptVars ())
      {
      if (theOptVar->mandDetVar ()->isSlbvVar ())
         continue;

      theObjCoeff = theOptVar->compObjCoeff ();

      theOptVar->incCoeff (theObjCoeff);
      }

   theMultiObjMgr->setCurrentObjective (savedObj);
   }

//------------------------------------------------------------------------------
// Implementation of class MultiObjCon.
//------------------------------------------------------------------------------

WitTbvCon::WitTbvCon (WitDetImpOP * theDetImpOP):

      WitDetCon (theDetImpOP)
   {
   myDetImpOP ()->storeDerived (this);
   }

//------------------------------------------------------------------------------

WitTbvCon::~WitTbvCon ()
   {
   }

//------------------------------------------------------------------------------

double WitTbvCon::upperBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

double WitTbvCon::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitTbvCon::classText ()
   {
   return "T. B. V.";
   }

//------------------------------------------------------------------------------

void WitTbvCon::printItem ()
   {
   myDetImpOP ()->printItem ();
   }

//------------------------------------------------------------------------------

void WitTbvCon::generateCoeffs ()
   {
   WitSlbvVar *    theSlbvVar;
   WitBoundedVar * theBoundedVar;

   myDetImpOP ()->myNtbvVar ()->incCoeff (+1.0);

   forEachEl (theBoundedVar, myDetImpOP ()->myBoundedVars ())
      {
      theSlbvVar = myDetImpOP ()->mySlbvVar (theBoundedVar);

      if (theSlbvVar != NULL)
         {
         theSlbvVar->incCoeff (+1.0);
         }
      }
   }
