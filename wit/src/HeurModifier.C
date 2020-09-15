//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "HeurModifier.C"
//
// Contains the implementation of class HeurModifier
//------------------------------------------------------------------------------

#include <HeurModifier.h>
#include <HeurAllMgr.h>
#include <GlobalComp.h>
#include <Capacity.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <Demand.h>
#include <BoundSet.h>
#include <IVRealArg.h>
#include <AvailSched.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class HeurModifier.
//------------------------------------------------------------------------------

WitHeurModifier::WitHeurModifier (WitAvailSched * theAvailSched):

      WitProbAssoc      (theAvailSched),
      myAvailSched_     (theAvailSched),
      toChkCapPairs_    (myProblem ()),
      toChkOpnPairs_    (myProblem ()),
      toChkBomEntPairs_ (myProblem ()),
      toChkMats_        (myProblem ()),
      toChkDemands_     (myProblem ()),
      chkNeededMat_     (myProblem (), false),
      chkNeededDemand_  (myProblem (), false)
   {
   checkPreconditions ();
   }

//------------------------------------------------------------------------------

WitHeurModifier::~WitHeurModifier ()
   {
   }

//------------------------------------------------------------------------------

void WitHeurModifier::modifyForExecVol (
      WitOperation *       theOpn,
      const WitIVRealArg & theVec)
   {
   recNeedsForExecVol (theOpn, theVec);
   modAvailForExecVol (theOpn, theVec);
   }

//------------------------------------------------------------------------------

void WitHeurModifier::modifyForSubVol (
      WitSubEntry *        theSub,
      const WitIVRealArg & theVec)
   {
   recNeedsForSubVol (theSub, theVec);
   modAvailForSubVol (theSub, theVec);
   }

//------------------------------------------------------------------------------

void WitHeurModifier::modifyForShipVol (
      WitDemand *          theDemand,
      const WitIVRealArg & theVec)
   {
   recNeedsForShipVol (theDemand, theVec);
   modAvailForShipVol (theDemand, theVec);
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkFeasibility ()
   {
   checkCapPairs    ();
   checkMats        ();
   checkOpnPairs    ();
   checkBomEntPairs ();
   checkDemands     ();
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkPreconditions ()
   {
   if (not myGlobalComp ()->                    selectionRecovery     ())
      myMsgFac () ("boolAttInvForModHeurSmsg", "selectionRecovery",   true);

   if (    myGlobalComp ()->                    skipFailures          ())
      myMsgFac () ("boolAttInvForModHeurSmsg", "skipFailures",        false);

   if (    myGlobalComp ()->                    perfPegging           ())
      myMsgFac () ("boolAttInvForModHeurSmsg", "perfPegging",         false);

   if (    myGlobalComp ()->                    computeCriticalList   ())
      myMsgFac () ("boolAttInvForModHeurSmsg", "computeCriticalList", false);

   if (    myGlobalComp ()->                    pgdCritListMode       ())
      myMsgFac () ("boolAttInvForModHeurSmsg", "pgdCritListMode",     false);
   }

//------------------------------------------------------------------------------

void WitHeurModifier::recNeedsForExecVol (
      WitOperation *       theOpn,
      const WitIVRealArg & theVec)
   {
   WitPeriod     execPer;
   WitBomEntry * theBomEnt;
   WitBopEntry * theBopEnt;
   WitPart *     thePart;
   WitPeriod     impactPer;

   forEachPeriod (execPer, myProblem ())
      {
      if (theVec[execPer] != theOpn->execVol ()[execPer])
         {
         toChkOpnPairs_.push (theOpn, execPer);

         forEachEl (theBomEnt, theOpn->bom ())
            {
            if (not theBomEnt->mySubEntries ().isEmpty ())
               {
               toChkBomEntPairs_.push (theBomEnt, execPer);
               }

            if (theBomEnt->inEffect (execPer))
               {
               thePart   = theBomEnt->myPart       ();
               impactPer = theBomEnt->impactPeriod ()[execPer];

               recPartPairNeed (thePart, impactPer);
               }
            }

         forEachEl (theBopEnt, theOpn->bop ())
            {
            if (theBopEnt->inEffect (execPer))
               {
               thePart   = theBopEnt->myPart       ();
               impactPer = theBopEnt->impactPeriod ()[execPer];

               recPartPairNeed (thePart, impactPer);
               }
            }
         }
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::recNeedsForSubVol (
      WitSubEntry *        theSub,
      const WitIVRealArg & theVec)
   {
   WitBomEntry * theBomEnt;
   WitPeriod     execPer;
   WitPeriod     consPer;

   theBomEnt = theSub->myBomEnt ();

   forEachPeriod (execPer, myProblem ())
      {
      if (theVec[execPer] != theSub->subVol ()[execPer])
         {
         toChkBomEntPairs_.push (theBomEnt, execPer);

         if (theSub->inEffect (execPer))
            {
            consPer = theSub->impactPeriod ()[execPer];

            recPartPairNeed (theSub->myPart (), consPer);
            }

         if (theBomEnt->inEffect (execPer))
            {
            consPer = theBomEnt->impactPeriod ()[execPer];

            recPartPairNeed (theBomEnt->myPart (), consPer);
            }
         }
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::recNeedsForShipVol (
      WitDemand *          theDemand,
      const WitIVRealArg & theVec)
   {
   WitPeriod shipPer;

   forEachPeriod (shipPer, myProblem ())
      {
      if (theVec[shipPer] != theDemand->shipVol ()[shipPer])
         {
         if (not chkNeededDemand_ (theDemand))
            {
            chkNeededDemand_ (theDemand) = true;

            toChkDemands_.push (theDemand);
            }

         recPartPairNeed (theDemand->demandedPart (), shipPer);
         }
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::recPartPairNeed (WitPart * thePart, WitPeriod thePer)
   {
   WitMaterial * theMat;

   theMat = thePart->thisMat ();

   if (theMat != NULL)
      {
      if (not chkNeededMat_ (theMat))
         {
         chkNeededMat_ (theMat) = true;

         toChkMats_.push (theMat);
         }
      }
   else
      {
      toChkCapPairs_.push (thePart, thePer);
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::modAvailForExecVol (
      WitOperation *       theOpn,
      const WitIVRealArg & theVec)
   {
   WitPeriod     execPer;
   double        deltaExecVol;
   WitBomEntry * theBomEnt;
   WitBopEntry * theBopEnt;
   WitPart *     thePart;
   WitPeriod     impactPer;
   double        deltaAvailVol;

   forEachPeriod (execPer, myProblem ())
      {
      deltaExecVol =
           theVec            [execPer]
         - theOpn->execVol ()[execPer];

      if (fabs (deltaExecVol) < NET_TOL)
         continue;

      forEachEl (theBomEnt, theOpn->bom ())
         {
         if (not theBomEnt->inEffect (execPer))
            continue;

         thePart       =   theBomEnt->myPart       ();
         impactPer     =   theBomEnt->impactPeriod ()[execPer];
         deltaAvailVol = - theBomEnt->effConsRate  ()[execPer] * deltaExecVol;

         myAvailSched_->addToIncAvailVol (thePart, impactPer, deltaAvailVol);
         }

      forEachEl (theBopEnt, theOpn->bop ())
         {
         if (not theBopEnt->inEffect (execPer))
            continue;

         thePart       = theBopEnt->myPart       ();
         impactPer     = theBopEnt->impactPeriod ()[execPer];
         deltaAvailVol = theBopEnt->effProdRate  ()[execPer] * deltaExecVol;

         myAvailSched_->addToIncAvailVol (thePart, impactPer, deltaAvailVol);
         }
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::modAvailForSubVol (
      WitSubEntry *        theSub,
      const WitIVRealArg & theVec)
   {
   WitPart *     theSubPart;
   WitBomEntry * theBomEnt;
   WitPart *     theBomPart;
   WitPeriod     execPer;
   double        deltaSubVol;
   WitPeriod     consPer;
   double        deltaAvailVol;

   theSubPart = theSub   ->myPart   ();
   theBomEnt  = theSub   ->myBomEnt ();
   theBomPart = theBomEnt->myPart   ();

   forEachPeriod (execPer, myProblem ())
      {
      deltaSubVol =
           theVec           [execPer]
         - theSub->subVol ()[execPer];

      if (fabs (deltaSubVol) < NET_TOL)
         continue;

      if (theSub->inEffect (execPer))
         {
         consPer       =   theSub->impactPeriod ()[execPer];
         deltaAvailVol = - theSub->effConsRate  ()[execPer] * deltaSubVol;

         myAvailSched_->addToIncAvailVol (theSubPart, consPer, deltaAvailVol);
         }

      if (theBomEnt->inEffect (execPer))
         {
         consPer       = theBomEnt->impactPeriod ()[execPer];
         deltaAvailVol = theBomEnt->effConsRate  ()[execPer] * deltaSubVol;

         myAvailSched_->addToIncAvailVol (theBomPart, consPer, deltaAvailVol);
         }
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::modAvailForShipVol (
      WitDemand *          theDemand,
      const WitIVRealArg & theVec)
   {
   WitPart * thePart;
   WitPeriod shipPer;
   double    deltaShipVol;

   thePart = theDemand->demandedPart ();

   forEachPeriod (shipPer, myProblem ())
      {
      deltaShipVol =
           theVec               [shipPer]
         - theDemand->shipVol ()[shipPer];

      if (fabs (deltaShipVol) < NET_TOL)
         continue;

      myAvailSched_->addToIncAvailVol (thePart, shipPer, - deltaShipVol);
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkCapPairs ()
   {
   WitPart *     thePart;
   WitPeriod     thePer;
   WitCapacity * theCap;

   while (toChkCapPairs_.pop (thePart, thePer))
      {
      theCap = thePart->thisCap ();

      witAssert (theCap != NULL);

      checkScrapCon (theCap, thePer);
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkMats ()
   {
   WitMaterial * theMat;

   while (toChkMats_.pop (theMat))
      {
      chkNeededMat_ (theMat) = false;

      checkScrapCon (theMat);
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkOpnPairs ()
   {
   WitOperation * theOpn;
   WitPeriod      execPer;

   while (toChkOpnPairs_.pop (theOpn, execPer))
      {
      checkNonExecCon (theOpn, execPer);
      checkExecHUBCon (theOpn, execPer); 
      checkLotSizeCon (theOpn, execPer);
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkBomEntPairs ()
   {
   WitBomEntry * theBomEnt;
   WitPeriod     execPer;

   while (toChkBomEntPairs_.pop (theBomEnt, execPer))
      {
      checkSubCon (theBomEnt, execPer);
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkDemands ()
   {
   WitDemand * theDemand;

   while (toChkDemands_.pop (theDemand))
      {
      chkNeededDemand_ (theDemand) = false;

      checkCumShipCon (theDemand);
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkScrapCon (WitCapacity * theCap, WitPeriod thePer)
   {
   double theScrapVol;

   theScrapVol = myAvailSched_->incAvailVol (theCap, thePer);

   if (theScrapVol < - FEAS_TOL)
      {
      myMsgFac () ("modHeurScrapViolSmsg",
         theCap->partName (),
         thePer,
         theScrapVol);
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkScrapCon (WitMaterial * theMat)
   {
   double    cumAvail;
   WitPeriod thePer;

   cumAvail = 0.0;

   forEachPeriod (thePer, myProblem ())
      {
      cumAvail += myAvailSched_->incAvailVol (theMat, thePer);

      if (cumAvail < - FEAS_TOL)
         {
         myMsgFac () ("modHeurScrapViolSmsg",
            theMat->partName (),
            thePer,
            cumAvail);
         }

      if (not theMat->canStock (thePer))
         cumAvail = 0.0;
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkNonExecCon (WitOperation * theOpn, WitPeriod execPer)
   {
   if (not theOpn->executable ()[execPer])
      if (theOpn->execVol ()[execPer] > 0.0)
         {
         myMsgFac () ("modHeurNonExecViolSmsg",
            theOpn->operationName (),
            execPer,
            theOpn->execVol ()[execPer]);
         }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkExecHUBCon (WitOperation * theOpn, WitPeriod execPer)
   {
   double hardUBVal;
   double violation;

   if (theOpn->execBounds ()->hardUBIsFinite (execPer))
      {
      hardUBVal = theOpn->execBounds ()->hardUB ()[execPer];

      violation = positivePart (theOpn->execVol ()[execPer] - hardUBVal);

      if (violation > FEAS_TOL)
         {
         myMsgFac () ("modHeurExecHUBViolSmsg",
            theOpn->operationName (),
            execPer,
            theOpn->execVol ()[execPer],
            hardUBVal,
            violation);
         }
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkLotSizeCon (WitOperation * theOpn, WitPeriod execPer)
   {
   double delta;
   double violation;

   delta     = theOpn->lotSizeDelta (execPer, theOpn->execVol ()[execPer], 0.0);

   violation = positivePart (delta);

   if (violation > FEAS_TOL)
      {
      myMsgFac () ("modHeurLotSizeViolSmsg",
         theOpn->operationName (),
         execPer,
         theOpn->execVol ()[execPer],
         violation);
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkSubCon (WitBomEntry * theBomEnt, WitPeriod execPer)
   {
   double        sumSubVols;
   WitSubEntry * theSub;
   double        theExecVol;
   double        violation;

   sumSubVols = 0.0;

   forEachEl (theSub, theBomEnt->mySubEntries ())
      {
      sumSubVols += theSub->subVol ()[execPer];
      }

   theExecVol = theBomEnt->myOperation ()->execVol ()[execPer];

   violation  = positivePart (sumSubVols - theExecVol);

   if (violation > FEAS_TOL)
      {
      myMsgFac () ("modHeurSubViolSmsg",
         theBomEnt->myOperationName (),
         theBomEnt->localIndex      (),
         execPer,
         theExecVol,
         sumSubVols,
         violation);
      }
   }

//------------------------------------------------------------------------------

void WitHeurModifier::checkCumShipCon (WitDemand * theDemand)
   {
   double    cumDemandVol;
   double    cumShipVol;
   WitPeriod shipPer;
   double    violation;

   cumDemandVol = 0.0;
   cumShipVol   = 0.0;

   forEachPeriod (shipPer, myProblem ())
      {
      cumDemandVol += theDemand->demandVol ()[shipPer];
      cumShipVol   += theDemand->shipVol   ()[shipPer];

      violation     = positivePart (cumShipVol - cumDemandVol);

      if (violation > FEAS_TOL)
         {
         myMsgFac () ("modHeurCumShipViolSmsg",
            theDemand->demandedPartName (),
            theDemand->demandName       (),
            shipPer,
            cumShipVol,
            cumDemandVol,
            violation);
         }
      }
   }
