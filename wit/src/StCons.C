//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "StCons.C"
//
// Implementation the following classes:
//
//    StResCon
//    StSubCon
//    StShipCon
//    StSlbCon
//------------------------------------------------------------------------------

#include <StCons.h>
#include <Stage.h>
#include <StageMgr.h>
#include <Scenario.h>
#include <ScenAttMgr.h>
#include <ScenInputAtt.h>
#include <StochImpOP.h>
#include <StochLoc.h>
#include <StVars.h>
#include <StochImpMgr.h>
#include <StochModeMgr.h>
#include <Material.h>
#include <Demand.h>
#include <SubEntry.h>
#include <Operation.h>
#include <BopEntry.h>
#include <ExecPerSch.h>
#include <MsgFac.h>

#include <float.h>

//------------------------------------------------------------------------------
// Implementation of class StResCon.
//------------------------------------------------------------------------------

WitStResCon::WitStResCon (WitPart * thePart, WitStochLoc * theStochLoc):

      WitStochCon (theStochLoc),
      myPart_     (thePart)
   {
   }

//------------------------------------------------------------------------------

WitStResCon::~WitStResCon ()
   {
   }

//------------------------------------------------------------------------------

void WitStResCon::generateCoeffs ()
   {
   genPartCoeffs   ();
   genMatCoeffs    ();
   genDemandCoeffs ();
   genBomEntCoeffs ();
   genSubCoeffs    ();
   genBopEntCoeffs ();
   }

//------------------------------------------------------------------------------

double WitStResCon::upperBoundVal ()
   {
   return lowerBoundVal ();
   }

//------------------------------------------------------------------------------

double WitStResCon::lowerBoundVal ()
   {
   return
      myScenAttMgr ()->
         supplyVol ()->
            myValueFor (myPart_, firstScenario ())[myPer ()];
   }

//------------------------------------------------------------------------------

const char * WitStResCon::classText ()
   {
   return "Resource";
   }

//------------------------------------------------------------------------------

void WitStResCon::printItem ()
   {
   myStochImpOP ()->printItem (myPart_);
   }

//------------------------------------------------------------------------------

void WitStResCon::genPartCoeffs ()
   {
   WitStScrapVar * theScrapVar;

   theScrapVar = myStochLoc ()->myScrapVarFor (myPart_);

   if (theScrapVar != NULL)
      theScrapVar->incCoeff (1.0);
   }

//------------------------------------------------------------------------------

void WitStResCon::genMatCoeffs ()
   {
   WitMaterial *   theMat;
   WitStStockVar * theStockVar;
   WitStStockVar * prevPerStockVar;

   theMat = myPart_->thisMat ();

   if (theMat == NULL)
      return;

   theStockVar = myStochLoc ()->myStockVarFor (theMat);

   if (theStockVar != NULL)
       theStockVar->incCoeff (1.0);

   if (myPer () == 0)
      return;

   prevPerStockVar =
      findVarStochLocFor (theMat, myPer () - 1)->
         myStockVarFor   (theMat);

   if (prevPerStockVar != NULL)
       prevPerStockVar->incCoeff (-1.0);
   }

//------------------------------------------------------------------------------

void WitStResCon::genDemandCoeffs ()
   {
   WitDemand * theDemand;

   forEachEl (theDemand, myPart_->myDemands ())
      myStochLoc ()->myShipVarFor (theDemand)->incCoeff (1.0);
   }

//------------------------------------------------------------------------------

void WitStResCon::genBomEntCoeffs ()
   {
   WitBomEntry *  theBomEnt;
   WitOperation * theOpn;
   WitPeriod      execPerF;
   WitPeriod      execPerL;
   WitPeriod      execPer;
   WitStochLoc *  theStochLoc;
   WitOptVar *    theOptVar;
   double         theRate;

   forEachEl (theBomEnt, myPart_->consumingBomEntries ())
      {
      theOpn       = theBomEnt->myOperation ();

      getExecPerRange (theBomEnt, execPerF, execPerL);

      for (execPer = execPerL; execPer >= execPerF; execPer --)
         if (theBomEnt->impactPeriod ()[execPer] == myPer ())
            {
            theStochLoc = findVarStochLocFor (theOpn, execPer);

            if (theBomEnt->hasSubsInEffect ()[execPer])
               theOptVar = theStochLoc->myNonSubVarFor (theBomEnt);
            else
               theOptVar = theStochLoc->myExecVarFor   (theOpn);

            theRate = theBomEnt->effConsRate ()[execPer];

            theOptVar->incCoeff (theRate);
            }
      }
   }

//------------------------------------------------------------------------------

void WitStResCon::genSubCoeffs ()
   {
   WitSubEntry *  theSub;
   WitOperation * theOpn;
   WitPeriod      execPerF;
   WitPeriod      execPerL;
   WitPeriod      execPer;
   WitStSubVar *  theSubVar;
   double         theRate;

   forEachEl (theSub, myPart_->consumingSubEntries ())
      {
      theOpn = theSub->myOperation ();

      getExecPerRange (theSub, execPerF, execPerL);

      for (execPer = execPerL; execPer >= execPerF; execPer --)
         if (theSub->impactPeriod ()[execPer] == myPer ())
            {
            theSubVar =
               findVarStochLocFor (theOpn, execPer)->mySubVarFor (theSub);

            theRate   = theSub->effConsRate ()[execPer];

            theSubVar->incCoeff (theRate);
            }
      }
   }

//------------------------------------------------------------------------------

void WitStResCon::genBopEntCoeffs ()
   {
   WitBopEntry *  theBopEnt;
   WitOperation * theOpn;
   WitPeriod      execPerF;
   WitPeriod      execPerL;
   WitPeriod      execPer;
   WitStExecVar * theExecVar;
   double         theRate;

   forEachEl (theBopEnt, myPart_->producingBopEntries ())
      {
      theOpn = theBopEnt->myOperation ();

      getExecPerRange (theBopEnt, execPerF, execPerL);

      for (execPer = execPerL; execPer >= execPerF; execPer --)
         if (theBopEnt->impactPeriod ()[execPer] == myPer ())
            {
            theExecVar =
               findVarStochLocFor (theOpn, execPer)->myExecVarFor (theOpn);

            theRate      = theBopEnt->effProdRate ()[execPer];

            theExecVar->incCoeff (- theRate);
            }
      }
   }

//------------------------------------------------------------------------------

void WitStResCon::getExecPerRange (
      WitBillEntry * theBillEnt,
      WitPeriod &    execPerF,
      WitPeriod &    execPerL)
   {
   myStochImpOP ()->
      myExecPerSched ()->
         getExecPerRange (theBillEnt, myPer (), execPerF, execPerL);
   }

//------------------------------------------------------------------------------
// Implementation of class StSubCon.
//------------------------------------------------------------------------------

WitStSubCon::WitStSubCon (WitBomEntry * theBomEnt, WitStochLoc * theStochLoc):

      WitStochCon (theStochLoc),
      myBomEnt_   (theBomEnt)
   {
   }

//------------------------------------------------------------------------------

WitStSubCon::~WitStSubCon ()
   {
   }

//------------------------------------------------------------------------------

void WitStSubCon::generateCoeffs ()
   {
   WitSubEntry *  theSub;
   WitOperation * theOpn;

   theOpn = myBomEnt_->myOperation ();

   myStochLoc ()->      myNonSubVarFor (myBomEnt_)->incCoeff  (1.0);

   forEachEl (theSub, myBomEnt_->mySubEntries ())
      if (theSub->inEffect (myPer ()))
         myStochLoc ()->mySubVarFor    (theSub)   ->incCoeff  (1.0);

   myStochLoc ()->      myExecVarFor   (theOpn)   ->incCoeff (-1.0);
   }

//------------------------------------------------------------------------------

double WitStSubCon::upperBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

double WitStSubCon::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitStSubCon::classText ()
   {
   return "Substitution";
   }

//------------------------------------------------------------------------------

void WitStSubCon::printItem ()
   {
   myStochImpOP ()->printItem (myBomEnt_);
   }

//------------------------------------------------------------------------------
// Implementation of class StShipCon.
//------------------------------------------------------------------------------

WitStShipCon::WitStShipCon (WitDemand * theDemand, WitStochLoc * theStochLoc):

      WitStochCon (theStochLoc),
      myDemand_   (theDemand)
   {
   }

//------------------------------------------------------------------------------

WitStShipCon::~WitStShipCon ()
   {
   }

//------------------------------------------------------------------------------

void WitStShipCon::generateCoeffs ()
   {
   WitStochLoc * theStochLoc;

   if (myPer () > 0)
      {
      theStochLoc =
         findVarStochLocFor (myDemand_->demandedPart (), myPer () - 1);

      theStochLoc->myCumShipVarFor (myDemand_)->incCoeff  (1.0);
      }

   myStochLoc () ->myShipVarFor    (myDemand_)->incCoeff  (1.0);
   myStochLoc () ->myCumShipVarFor (myDemand_)->incCoeff (-1.0);
   }

//------------------------------------------------------------------------------

double WitStShipCon::upperBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

double WitStShipCon::lowerBoundVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

const char * WitStShipCon::classText ()
   {
   return "Shipment";
   }

//------------------------------------------------------------------------------

void WitStShipCon::printItem ()
   {
   myStochImpOP ()->printItem (myDemand_);
   }

//------------------------------------------------------------------------------
// Implementation of class StSlbCon.
//------------------------------------------------------------------------------

WitStSlbCon::WitStSlbCon (WitStSlbvVar * theSlbvVar):

      WitStochCon (theSlbvVar->myStochLoc ()),
      mySlbvVar_  (theSlbvVar)
   {
   }

//------------------------------------------------------------------------------

WitStSlbCon::~WitStSlbCon ()
   {
   }

//------------------------------------------------------------------------------

void WitStSlbCon::generateCoeffs ()
   {
   myBddVar ()->incCoeff (1.0);
   mySlbvVar_ ->incCoeff (1.0);
   }

//------------------------------------------------------------------------------

double WitStSlbCon::upperBoundVal ()
   {
   return DBL_MAX;
   }

//------------------------------------------------------------------------------

double WitStSlbCon::lowerBoundVal ()
   {
   WitBoundSet * theBoundSet;

   theBoundSet = myBddVar ()->myBoundSet ();

   return
      myScenAttMgr ()->
         softLB ()->
            myValueFor (theBoundSet, firstScenario ())[myPer ()];
   }

//------------------------------------------------------------------------------

const char * WitStSlbCon::classText ()
   {
   return "S. L. B.";
   }

//------------------------------------------------------------------------------

void WitStSlbCon::printItem ()
   {
   myStochImpOP ()->printItem (myBddVar ());
   }

//------------------------------------------------------------------------------

WitStBddVar * WitStSlbCon::myBddVar ()
   {
   return mySlbvVar_->myBddVar ();
   }
