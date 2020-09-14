//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "demApi.C"
//
// Inner API functions relating to Demands.
//------------------------------------------------------------------------------

#include <WitRun.h>
#include <ApiMgr.h>
#include <InputID.h>
#include <ApiCall.h>
#include <ISRealArg.h>
#include <OSRealArg.h>
#include <IVRealArg.h>
#include <OVRealArg.h>
#include <EqAll.h>
#include <HeurAllMgr.h>
#include <PegEl.h>
#include <PipMgr.h>
#include <DetOptImpMgr.h>
#include <ExtOptMgr.h>
#include <GlobalComp.h>
#include <Part.h>
#include <Demand.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <BoundSet.h>
#include <ObjStack.h>
#include <PairStack.h>
#include <TripStack.h>
#include <MsgFrag.h>
#include <MsgFac.h>

#include <stdlib.h>

//------------------------------------------------------------------------------
// setGetDemandInit
// Initilization of wit[S|G]etDemand* functions.
//------------------------------------------------------------------------------

WitDemand * WitRun::setGetDemandInit (
      const char *  partName,
      const char *  demandName)
   {
   forbidNullArg (partName,   "demandedPartName");
   forbidNullArg (demandName, "demandName");

   if (myMsgFac ().mayIssue ("partNameMsg"))
      myMsgFac () ("partNameMsg", partName);

   if (myMsgFac ().mayIssue ("demandNameMsg"))
      myMsgFac () ("demandNameMsg", demandName);

   return getDemandP (partName, demandName);
   }

//------------------------------------------------------------------------------

void WitRun::witAddDemand (
      const char * partName,
      const char * demandName)
   {
   WitPart * thePart;

   myApiMgr_->startInnerFunc ();

   forbidNullArg (partName,   "partName");
   forbidNullArg (demandName, "demandName");

   if (myMsgFac ().mayIssue ("demandNameAdMsg"))
      myMsgFac () ("demandNameAdMsg", partName, demandName);

   thePart = myCompMgr ()->findPart (partName);

   if (thePart == NULL)
      myMsgFac () ("undefinedPartMsg", partName);

   myInputID ()->setMyPart (thePart);

   if (isNullCstring (demandName))
      myMsgFac () ("nullStringSmsg", "demandName");

   if (myCompMgr ()->findDemand (thePart, demandName) != NULL)
      myMsgFac () ("duplicateDemNameAdMsg", demandName);

   addingDelComp ("demandFrag");

   new WitDemand (demandName, thePart);

   myInputID ()->setMyPart (NULL);
   }

//------------------------------------------------------------------------------
// witAppendToPipSeq
// Append a shipment triple to the end of the PIP shipment sequence.
//------------------------------------------------------------------------------

void WitRun::witAppendToPipSeq (
      const char *         partName,
      const char *         demandName,
      int                  shipPeriod,
      const WitISRealArg & incShipVol)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   if (myMsgFac ().mayIssue ("shipPerMsg"))
      myMsgFac () ("shipPerMsg", shipPeriod);

   if (myMsgFac ().mayIssue ("incShipVolMsg"))
      myMsgFac () ("incShipVolMsg", incShipVol);

   if (not ((shipPeriod >= 0) and (shipPeriod < nPeriods ())))
      {
      myInputID ()->setArgName ("shipPeriod");

      myMsgFac () ("argRangeIntLeValLtIntSmsg", shipPeriod, 0, nPeriods ());
      }

   if (not (incShipVol.asaDbl () >= 0.0))
      {
      myInputID ()->setArgName ("incShipVol");

      myMsgFac () ("argRangeValGeDblSmsg", incShipVol, 0.0);
      }

   myProblem ()->
      myPipMgr ()->
         appendToShipSeq (theCall.myDemand (), shipPeriod, incShipVol);
   }

//------------------------------------------------------------------------------

void WitRun::witCopyDemandData (
      const char * dupPartName,
      const char * dupDemandName,
      WitRun *     origWitRun,
      const char * origPartName,
      const char * origDemandName)
   {
   startCopyObjData (origWitRun);

   myMsgFac ()  ("dupObjectMsg", myMsgFac ().myFrag ("demandFrag"));

   WitDemandApiCall  dupCall (this,        dupPartName,  dupDemandName);

   myMsgFac () ("origObjectMsg", myMsgFac ().myFrag ("demandFrag"));

   WitDemandApiCall origCall (origWitRun, origPartName, origDemandName);

   if (dupCall.myDemand () == origCall.myDemand ())
      myMsgFac () ("copyObjectToSelfSmsg",
         myMsgFac ().myFrag ("demandFrag"));

   dupCall.myDemand ()->copyAttrsFrom (origCall.myDemand ());
   }

//------------------------------------------------------------------------------
// witEqHeurAlloc
// API function to equitably increment heuristic allocation.
//------------------------------------------------------------------------------

void WitRun::witEqHeurAlloc (
      int                  lenLists,
      const char * const * partNameList,
      const char * const * demandNameList,
      const int *          shipPeriodList,
      const WitIVRealArg & desIncVolList,
      const WitOVRealArg & incVolList)
   {
   myApiMgr_->startInnerFunc ();

   if (myGlobalComp ()->twoWayMultiExec ())
      myMsgFac () ("wrongFuncForTwmeSmsg",
         "witEqHeurAlloc",
         true,
         "witEqHeurAllocTwme");

   eqHeurAlloc (
      lenLists,
      partNameList,
      demandNameList,
      shipPeriodList,
      desIncVolList,
      incVolList,
      NULL);
   }

//------------------------------------------------------------------------------

void WitRun::witEqHeurAllocTwme (
      int                  lenLists,
      const char * const * partNameList,
      const char * const * demandNameList,
      const int *          shipPeriodList,
      const WitIVRealArg & desIncVolList,
      const WitOVRealArg & incVolList,
      const WitBoolean *   asapMultiExecList)
   {
   myApiMgr_->startInnerFunc ();

   if (not myGlobalComp ()->twoWayMultiExec ())
      myMsgFac () ("wrongFuncForTwmeSmsg",
         "witEqHeurAllocTwme",
         false,
         "witEqHeurAlloc");


   eqHeurAlloc (
      lenLists,
      partNameList,
      demandNameList,
      shipPeriodList,
      desIncVolList,
      incVolList,
      asapMultiExecList);
   }

//------------------------------------------------------------------------------

void WitRun::witGetObjItrDemand (
      char * * partName,
      char * * demandName)
   {
   WitDemand * theDemand;

   myApiMgr_->startInnerFunc ();

   theDemand =
      curItrDelCompForGet (WitAT_DEMAND, "demandFrag")->
         thisDemand ();

   stronglyAssert (theDemand != NULL);

   apiGet (partName,   "partName",   theDemand->demandedPartName ());
   apiGet (demandName, "demandName", theDemand->demandName ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandAppData (
      const char * partName,
      const char * demandName,
      void * *    appData)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (appData, "appData", theCall.myDemand ()->appData ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandBuildAheadUB (
      const char * partName,
      const char * demandName,
      int * *      buildAheadUB)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (buildAheadUB, "buildAheadUB", theCall.myDemand ()->buildAheadUB ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandBuildAheadLimit (
      const char * partName,
      const char * demandName,
      int *        buildAheadLimit)
   {
   int * buildAheadUBval;

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("getVecAttrAsScalMsg"))
      myMsgFac () (
         "getVecAttrAsScalMsg",
         apiFuncName (),
         "demand",
         "buildAheadLimit",
         "buildAheadUB");

   witGetDemandBuildAheadUB (partName, demandName, & buildAheadUBval);

   forbidNullArg (buildAheadLimit, "buildAheadLimit");

   * buildAheadLimit = buildAheadUBval[0];

   free (buildAheadUBval);
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandCoExecVolPip (
      const char *         partName,
      const char *         demandName,
      int                  shipPeriod,
      int *                lenLists,
      char * * *           operationNameList,
      int * *              bopEntryIndexList,
      int * *              execPeriodList,
      const WitOVRealArg & peggedVolList)
   {
   WitBopEntTripStack coExecVolTrips (myProblem ());
   int                nTrips;
   int                theIdx;
   WitBopEntry *      theBopEnt;
   WitPeriod          execPer;
   double             pgdCoExecVol;

   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepGetPip (shipPeriod);

   myProblem ()->
      myPipMgr ()->
         getCoExecVolPgg (theCall.myDemand (), shipPeriod, coExecVolTrips);

   nTrips = coExecVolTrips.nElements ();

   apiGet (lenLists, "lenLists", nTrips);

   forbidNullArg (operationNameList, "operationNameList");
   forbidNullArg (bopEntryIndexList, "bopEntryIndexList");
   forbidNullArg (execPeriodList,    "execPeriodList");
   forbidNullArg (peggedVolList,     "peggedVolList");

   apiAlloc (operationNameList, nTrips);
   apiAlloc (bopEntryIndexList, nTrips);
   apiAlloc (execPeriodList,    nTrips);
   apiAlloc (peggedVolList,     nTrips);

   theIdx = -1;

   while (coExecVolTrips.pop (theBopEnt, execPer, pgdCoExecVol))
      {
      theIdx ++;

      (* operationNameList)[theIdx] = 
         theBopEnt->myOperationName ().apiDupCstring ();

      (* bopEntryIndexList)[theIdx] = theBopEnt->localIndex ();
      (* execPeriodList)   [theIdx] = execPer;
      peggedVolList        [theIdx] = pgdCoExecVol;
      }
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandConsVolPip (
      const char *         partName,
      const char *         demandName,
      int                  shipPeriod,
      int *                lenLists,
      char * * *           partNameList,
      int * *              periodList,
      const WitOVRealArg & peggedVolList)
   {
   WitPartTripStack consVolTrips (myProblem ());

   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepGetPip (shipPeriod);

   myProblem ()->
      myPipMgr ()->
         getConsVolPgg (theCall.myDemand (), shipPeriod, consVolTrips);

   getPartPip (
      consVolTrips,
      lenLists,
      partNameList,
      periodList,
      peggedVolList);
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandCumShipBounds (
      const char *         partName,
      const char *         demandName,
      const WitOVRealArg & hardLB,
      const WitOVRealArg & softLB,
      const WitOVRealArg & hardUB)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (hardLB, softLB, hardUB, theCall.myDemand ()->cumShipBounds ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandCumShipReward (
      const char *         partName,
      const char *         demandName,
      const WitOVRealArg & cumShipReward)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (
       cumShipReward,
      "cumShipReward",
      theCall.myDemand ()->cumShipReward ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandCumShipSlbConIndex (
      const char * partName,
      const char * demandName,
      const int    thePer,
      int *        cumShipSlbConIndex)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   prepGetIndex (thePer);

   apiGet (
       cumShipSlbConIndex,
      "cumShipSlbConIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            cumShipSlbConIndex (theCall.myDemand (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandCumShipSlbvVarIndex (
      const char * partName,
      const char * demandName,
      const int    thePer,
      int *        cumShipSlbvVarIndex)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   prepGetIndex (thePer);

   apiGet (
       cumShipSlbvVarIndex,
      "cumShipSlbvVarIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            cumShipSlbvVarIndex (theCall.myDemand (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandCumShipVarIndex (
      const char * partName,
      const char * demandName,
      const int    thePer,
      int *        cumShipVarIndex)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   prepGetIndex (thePer);

   apiGet (
       cumShipVarIndex,
      "cumShipVarIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            cumShipVarIndex (theCall.myDemand (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandDemandVol (
      const char *         partName,
      const char *         demandName,
      const WitOVRealArg & demandVol)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (demandVol, "demandVol", theCall.myDemand ()->demandVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandExecVolPegging (
      const char *         partName,
      const char *         demandName,
      int                  shipPeriod,
      int *                lenLists,
      char * * *           operationNameList,
      int * *              execPeriodList,
      const WitOVRealArg & peggedExecVolList)
   {
   WitObjStack <WitFixedPegEl> theFixedPegEls (myProblem ());
   WitObjStItr <WitFixedPegEl> theItr;
   int                         nOpnPegEls;
   int                         theIdx;
   WitFixedPegEl *             theFixedPegEl;
   WitOperation *              theOpn;

   WitDemandApiCall theCall (this, partName, demandName);

   prepGetPegging (shipPeriod);

   theCall.myDemand ()->getMyFixedPegEls (shipPeriod, theFixedPegEls);

   nOpnPegEls = 0;

   theFixedPegEls.attachItr (theItr);

   while (theItr.advance (theFixedPegEl))
      if (theFixedPegEl->myOpn () != NULL)
         nOpnPegEls ++;

   apiGet (lenLists, "lenLists", nOpnPegEls);

   forbidNullArg (operationNameList, "operationNameList");
   forbidNullArg (execPeriodList,    "execPeriodList");
   forbidNullArg (peggedExecVolList, "peggedExecVolList");

   apiAlloc (operationNameList, nOpnPegEls);
   apiAlloc (execPeriodList,    nOpnPegEls);
   apiAlloc (peggedExecVolList, nOpnPegEls);

   theIdx = 0;

   while (theFixedPegEls.pop (theFixedPegEl))
      {
      theOpn = theFixedPegEl->myOpn ();

      if (theOpn == NULL)
         continue;

      (* operationNameList)[theIdx] = theOpn->operationName ().apiDupCstring ();

      (* execPeriodList)   [theIdx] = theFixedPegEl->myExecPer ();

      peggedExecVolList    [theIdx] = theFixedPegEl->peggedVol ();

      theIdx ++;
      }
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandExecVolPip (
      const char *         partName,
      const char *         demandName,
      int                  shipPeriod,
      int *                lenLists,
      char * * *           operationNameList,
      int * *              execPeriodList,
      const WitOVRealArg & peggedVolList)
   {
   WitOpnTripStack execVolTrips (myProblem ());
   int             nTrips;
   int             theIdx;
   WitOperation *  theOpn;
   WitPeriod       execPer;
   double          pgdExecVol;

   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepGetPip (shipPeriod);

   myProblem ()->
      myPipMgr ()->
         getExecVolPgg (theCall.myDemand (), shipPeriod, execVolTrips);

   nTrips = execVolTrips.nElements ();

   apiGet (lenLists, "lenLists", nTrips);

   forbidNullArg (operationNameList, "operationNameList");
   forbidNullArg (execPeriodList,    "execPeriodList");
   forbidNullArg (peggedVolList,     "peggedVolList");

   apiAlloc (operationNameList, nTrips);
   apiAlloc (execPeriodList,    nTrips);
   apiAlloc (peggedVolList,     nTrips);

   theIdx = -1;

   while (execVolTrips.pop (theOpn, execPer, pgdExecVol))
      {
      theIdx ++;

      (* operationNameList)[theIdx] = theOpn->operationName ().apiDupCstring ();

      (* execPeriodList)   [theIdx] = execPer;

      peggedVolList        [theIdx] = pgdExecVol;
      }
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandExists (
      const char * partName,
      const char * demandName,
      WitBoolean * exists)
   {
   WitPart *   thePart;
   WitDemand * theDemand;

   myApiMgr_->startInnerFunc ();

   thePart = myCompMgr ()->findPart (partName);

   if (thePart == NULL)
      {
      * exists  = false;
      }
   else
      {
      theDemand = myCompMgr ()->findDemand (thePart, demandName);

      * exists  = (theDemand != NULL);
      }
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandFocusHorizon (
      const char * partName,
      const char * demandName,
      int *        focusHorizon)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (focusHorizon, "focusHorizon", theCall.myDemand ()->focusHorizon ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandFssShipVol (
      const char *         partName,
      const char *         demandName,
      const WitOVRealArg & fssShipVol)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (fssShipVol, "fssShipVol", theCall.myDemand ()->fssShipVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandIntShipVols (
      const char * partName,
      const char * demandName,
      WitBoolean * intShipVols)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (
      intShipVols, 
      "intShipVols", 
      theCall.myDemand ()->intShipVols ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandLeadTimeUB (
      const char * partName,
      const char * demandName,
      int * *      leadTimeUB)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (leadTimeUB, "leadTimeUB", theCall.myDemand ()->leadTimeUB ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandObj1CumShipReward (
      const char *         partName,
      const char *         demandName,
      const WitOVRealArg & obj1CumShipReward)
   {
   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("getAttrNewNameMsg"))
      myMsgFac () (
         "getAttrNewNameMsg",
         apiFuncName (),
         "demand",
         "obj1CumShipReward",
         "cumShipReward");

   witGetDemandCumShipReward (
      partName,
      demandName,
      obj1CumShipReward);
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandObj1ShipReward (
      const char *         partName,
      const char *         demandName,
      const WitOVRealArg & obj1ShipReward)
   {
   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("getAttrNewNameMsg"))
      myMsgFac () (
         "getAttrNewNameMsg",
         apiFuncName (),
         "demand",
         "obj1ShipReward",
         "shipReward");

   witGetDemandShipReward (
      partName,
      demandName,
      obj1ShipReward);
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandPrefBuildAhead (
      const char * partName,
      const char * demandName,
      WitBoolean * prefBuildAhead)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (
      prefBuildAhead, 
      "prefBuildAhead", 
      theCall.myDemand ()->prefBuildAhead ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandPriority (
      const char * partName,
      const char * demandName,
      int * *      priority)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (priority, "priority", theCall.myDemand ()->priority ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandProdVolPip (
      const char *         partName,
      const char *         demandName,
      int                  shipPeriod,
      int *                lenLists,
      char * * *           partNameList,
      int * *              periodList,
      const WitOVRealArg & peggedVolList)
   {
   WitPartTripStack prodVolTrips (myProblem ());

   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepGetPip (shipPeriod);

   myProblem ()->
      myPipMgr ()->
         getProdVolPgg (theCall.myDemand (), shipPeriod, prodVolTrips);

   getPartPip (
      prodVolTrips,
      lenLists,
      partNameList,
      periodList,
      peggedVolList);
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandSearchInc (
      const char *         partName,
      const char *         demandName,
      const WitOSRealArg & searchInc)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (searchInc, "searchInc", theCall.myDemand ()->searchInc ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandSelForDel (
      const char * partName,
      const char * demandName,
      WitBoolean * selForDel)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (selForDel, "selForDel", theCall.myDemand ()->selForDel ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandShipConIndex (
      const char * partName,
      const char * demandName,
      const int    thePer,
      int *        shipConIndex)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   prepGetIndex (thePer);

   apiGet (
       shipConIndex,
      "shipConIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            shipConIndex (theCall.myDemand (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandShipLateAllowed (
      const char * partName,
      const char * demandName,
      WitBoolean * shipLateAllowed)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (
      shipLateAllowed, 
      "shipLateAllowed", 
      theCall.myDemand ()->shipLateAllowed ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandShipLateLimit (
      const char * partName,
      const char * demandName,
      int *        shipLateLimit)
   {
   int * shipLateUBval;

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("getVecAttrAsScalMsg"))
      myMsgFac () (
         "getVecAttrAsScalMsg",
         apiFuncName (),
         "demand",
         "shipLateLimit",
         "shipLateUB");

   witGetDemandShipLateUB (partName, demandName, & shipLateUBval);

   forbidNullArg (shipLateLimit, "shipLateLimit");

   * shipLateLimit = shipLateUBval[0];

   free (shipLateUBval);
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandShipLateUB (
      const char * partName,
      const char * demandName,
      int * *      shipLateUB)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (shipLateUB, "shipLateUB", theCall.myDemand ()->shipLateUB ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandShipReward (
      const char *         partName,
      const char *         demandName,
      const WitOVRealArg & shipReward)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (
       shipReward,
      "shipReward",
      theCall.myDemand ()->shipReward ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandShipVarIndex (
      const char * partName,
      const char * demandName,
      const int    thePer,
      int *        shipVarIndex)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   prepGetIndex (thePer);

   apiGet (
       shipVarIndex,
      "shipVarIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            shipVarIndex (theCall.myDemand (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandShipVol (
      const char *         partName,
      const char *         demandName,
      const WitOVRealArg & shipVol)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   apiGet (shipVol, "shipVol", theCall.myDemand ()->shipVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandSideVolPip (
      const char *         partName,
      const char *         demandName,
      int                  shipPeriod,
      int *                lenLists,
      char * * *           partNameList,
      int * *              periodList,
      const WitOVRealArg & peggedVolList)
   {
   WitPartTripStack sideVolTrips (myProblem ());

   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepGetPip (shipPeriod);

   myProblem ()->
      myPipMgr ()->
         getSideVolPgg (theCall.myDemand (), shipPeriod, sideVolTrips);

   getPartPip (
      sideVolTrips,
      lenLists,
      partNameList,
      periodList,
      peggedVolList);
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandSubVolPegging (
      const char *         partName,
      const char *         demandName,
      int                  shipPeriod,
      int *                lenLists,
      char * * *           operationNameList,
      int * *              bomEntryIndexList,
      int * *              subsBomEntryIndexList,
      int * *              execPeriodList,
      const WitOVRealArg & peggedSubVolList)
   {
   WitObjStack <WitFixedPegEl> theFixedPegEls (myProblem ());
   WitObjStItr <WitFixedPegEl> theItr;
   int                         nSubPegEls;
   int                         theIdx;
   WitFixedPegEl *             theFixedPegEl;
   WitSubEntry *               theSub;

   WitDemandApiCall theCall (this, partName, demandName);

   prepGetPegging (shipPeriod);

   theCall.myDemand ()->getMyFixedPegEls (shipPeriod, theFixedPegEls);

   nSubPegEls = 0;

   theFixedPegEls.attachItr (theItr);

   while (theItr.advance (theFixedPegEl))
      if (theFixedPegEl->mySub () != NULL)
         nSubPegEls ++;

   apiGet (lenLists, "lenLists", nSubPegEls);

   forbidNullArg (operationNameList,     "operationNameList");
   forbidNullArg (bomEntryIndexList,     "bomEntryIndexList");
   forbidNullArg (subsBomEntryIndexList, "subsBomEntryIndexList");
   forbidNullArg (execPeriodList,        "execPeriodList");
   forbidNullArg (peggedSubVolList,      "peggedSubVolList");

   apiAlloc (operationNameList,     nSubPegEls);
   apiAlloc (bomEntryIndexList,     nSubPegEls);
   apiAlloc (subsBomEntryIndexList, nSubPegEls);
   apiAlloc (execPeriodList,        nSubPegEls);
   apiAlloc (peggedSubVolList,      nSubPegEls);
   
   theIdx = 0;

   while (theFixedPegEls.pop (theFixedPegEl))
      {
      theSub = theFixedPegEl->mySub ();

      if (theSub == NULL)
         continue;

      (* operationNameList)    [theIdx] = 
         theSub->myOperationName ().apiDupCstring ();

      (* bomEntryIndexList)    [theIdx] = theSub       ->myBomEntIndex ();
      (* subsBomEntryIndexList)[theIdx] = theSub       ->localIndex ();
      (* execPeriodList)       [theIdx] = theFixedPegEl->myExecPer ();
      peggedSubVolList         [theIdx] = theFixedPegEl->peggedVol ();

      theIdx ++;
      }
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandSubVolPip (
      const char *         partName,
      const char *         demandName,
      int                  shipPeriod,
      int *                lenLists,
      char * * *           operationNameList,
      int * *              bomEntryIndexList,
      int * *              subsBomEntryIndexList,
      int * *              execPeriodList,
      const WitOVRealArg & peggedVolList)
   {
   WitSubTripStack subVolTrips (myProblem ());
   int             nTrips;
   int             theIdx;
   WitSubEntry *   theSub;
   WitPeriod       execPer;
   double          pgdSubVol;

   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepGetPip (shipPeriod);

   myProblem ()->
      myPipMgr ()->
         getSubVolPgg (theCall.myDemand (), shipPeriod, subVolTrips);

   nTrips = subVolTrips.nElements ();

   apiGet (lenLists, "lenLists", nTrips);

   forbidNullArg (operationNameList,     "operationNameList");
   forbidNullArg (bomEntryIndexList,     "bomEntryIndexList");
   forbidNullArg (subsBomEntryIndexList, "subsBomEntryIndexList");
   forbidNullArg (execPeriodList,        "execPeriodList");
   forbidNullArg (peggedVolList,         "peggedVolList");

   apiAlloc (operationNameList,     nTrips);
   apiAlloc (bomEntryIndexList,     nTrips);
   apiAlloc (subsBomEntryIndexList, nTrips);
   apiAlloc (execPeriodList,        nTrips);
   apiAlloc (peggedVolList,         nTrips);

   theIdx = -1;

   while (subVolTrips.pop (theSub, execPer, pgdSubVol))
      {
      theIdx ++;

      (* operationNameList)    [theIdx] = 
         theSub->myOperationName ().apiDupCstring ();

      (* bomEntryIndexList)    [theIdx] = theSub->myBomEntIndex ();
      (* subsBomEntryIndexList)[theIdx] = theSub->localIndex ();
      (* execPeriodList)       [theIdx] = execPer;
      peggedVolList            [theIdx] = pgdSubVol;
      }
   }

//------------------------------------------------------------------------------

void WitRun::witGetDemandSupplyVolPip (
      const char *         partName,
      const char *         demandName,
      int                  shipPeriod,
      int *                lenLists,
      char * * *           partNameList,
      int * *              periodList,
      const WitOVRealArg & peggedVolList)
   {
   WitPartTripStack supplyVolTrips (myProblem ());

   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepGetPip (shipPeriod);

   myProblem ()->
      myPipMgr ()->
         getSupplyVolPgg (theCall.myDemand (), shipPeriod, supplyVolTrips);

   getPartPip (
      supplyVolTrips,
      lenLists,
      partNameList,
      periodList,
      peggedVolList);
   }

//------------------------------------------------------------------------------

void WitRun::witIncHeurAlloc (
      const char *         partName,
      const char *         demandName,
      int                  shipPeriod,
      const WitISRealArg & desIncVol,
      const WitOSRealArg & incVol)
   {
   myApiMgr_->startInnerFunc ();

   if (myGlobalComp ()->twoWayMultiExec ())
      myMsgFac () ("wrongFuncForTwmeSmsg",
         "witIncHeurAlloc",
         true,
         "witIncHeurAllocTwme");

   incHeurAlloc (
      partName,
      demandName,
      shipPeriod,
      desIncVol,
      incVol,
      false);
   }

//------------------------------------------------------------------------------

void WitRun::witIncHeurAllocTwme (
      const char *         partName,
      const char *         demandName,
      int                  shipPeriod,
      const WitISRealArg & desIncVol,
      const WitOSRealArg & incVol,
      WitBoolean           asapMultiExec)
   {
   myApiMgr_->startInnerFunc ();

   if (not myGlobalComp ()->twoWayMultiExec ())
      myMsgFac () ("wrongFuncForTwmeSmsg",
         "witIncHeurAllocTwme",
         false,
         "witIncHeurAlloc");

   incHeurAlloc (
      partName,
      demandName,
      shipPeriod,
      desIncVol,
      incVol,
      asapMultiExec);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandAppData (
      const char * partName,
      const char * demandName,
      void *       appData)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.setAppData (appData);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandBuildAheadUB (
      const char * partName,
      const char * demandName,
      const int *  buildAheadUB)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepBuildAheadUB (buildAheadUB);

   if (theCall.argsOK ())
      theCall.myDemand ()->setBuildAheadUB (buildAheadUB);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandBuildAheadLimit (
      const char * partName,
      const char * demandName,
      int          buildAheadLimit)
   {
   WitTVec <int> & buildAheadUBVec = myApiMgr ()->reserveTempIntVec ();

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setVecAttrToScalMsg"))
      myMsgFac () (
         "setVecAttrToScalMsg",
         apiFuncName (),
         "demand",
         "buildAheadLimit",
         "buildAheadUB");

   buildAheadUBVec = buildAheadLimit;

   witSetDemandBuildAheadUB (
      partName, 
      demandName, 
      buildAheadUBVec.myCVec ());

   myApiMgr ()->releaseTempVecs ();
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandCumShipBounds (
      const char *         partName,
      const char *         demandName,
      const WitIVRealArg & hardLB,
      const WitIVRealArg & softLB,
      const WitIVRealArg & hardUB)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.setBoundSet (
      theCall.myDemand ()->cumShipBounds (), 
      hardLB,
      softLB,
      hardUB);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandCumShipReward (
      const char *         partName,
      const char *         demandName,
      const WitIVRealArg & cumShipReward)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepDblVec ("cumShipReward", cumShipReward);

   if (theCall.argsOK ())
      theCall.myDemand ()->setCumShipReward (cumShipReward);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandDemandName (
      const char * partName,
      const char * demandName,
      const char * newName)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   forbidNullArg (newName, "newName");

   if (isNullCstring (newName))
      myMsgFac () ("nullStringSmsg", "newName");

   if (myCompMgr ()->findDemand (theCall.myDemand ()->demandedPart (), newName))
      myMsgFac () ("setDemandNameToDupSmsg", newName);

   theCall.prepString (
                          "demandName",
      theCall.myDemand ()->demandName (),
                              newName);

   if (theCall.argsOK ())
      theCall.myDemand ()->setDemandName (newName);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandDemandVol (
      const char *         partName,
      const char *         demandName,
      const WitIVRealArg & demandVol)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepNonNegDblVec ("demandVol", demandVol);

   if (theCall.argsOK ())
      theCall.myDemand ()->setDemandVol (demandVol);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandFocusHorizon (
      const char * partName,
      const char * demandName,
      int          focusHorizon)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepRangedInt (
      "focusHorizon",
      theCall.myDemand ()->focusHorizon (),
      focusHorizon,
      - 1,
      theCall.myDemand ()->lastPeriod ());

   if (theCall.argsOK ())
      theCall.myDemand ()->setFocusHorizon (focusHorizon);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandFssShipVol (
      const char *         partName,
      const char *         demandName,
      const WitIVRealArg & fssShipVol)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepNonNegDblVec ("fssShipVol", fssShipVol);

   if (theCall.argsOK ())
      theCall.myDemand ()->setFssShipVol (fssShipVol);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandIntShipVols (
      const char * partName,
      const char * demandName,
      WitBoolean   intShipVols)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepBool (
      "intShipVols",
      theCall.myDemand ()->intShipVols (),
      intShipVols);

   if (theCall.argsOK ())
      theCall.myDemand ()->setIntShipVols (asaBool (intShipVols));
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandLeadTimeUB (
      const char * partName,
      const char * demandName,
      const int *  leadTimeUB)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepLeadTimeUB (leadTimeUB);

   if (theCall.argsOK ())
      theCall.myDemand ()->setLeadTimeUB (leadTimeUB);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandObj1CumShipReward (
      const char *         partName,
      const char *         demandName,
      const WitIVRealArg & obj1CumShipReward)
   {
   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setAttrNewNameMsg"))
      myMsgFac () (
         "setAttrNewNameMsg",
         apiFuncName (),
         "demand",
         "obj1CumShipReward",
         "cumShipReward");

   witSetDemandCumShipReward (
      partName,
      demandName,
      obj1CumShipReward);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandObj1ShipReward (
      const char *         partName,
      const char *         demandName,
      const WitIVRealArg & obj1ShipReward)
   {
   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setAttrNewNameMsg"))
      myMsgFac () (
         "setAttrNewNameMsg",
         apiFuncName (),
         "demand",
         "obj1ShipReward",
         "shipReward");

   witSetDemandShipReward (
      partName,
      demandName,
      obj1ShipReward);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandPrefBuildAhead (
      const char * partName,
      const char * demandName,
      WitBoolean   prefBuildAhead)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepBool (
      "prefBuildAhead",
      theCall.myDemand ()->prefBuildAhead (),
      prefBuildAhead);

   if (theCall.argsOK ())
      theCall.myDemand ()->setPrefBuildAhead (asaBool (prefBuildAhead));
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandPriority (
      const char * partName,
      const char * demandName,
      const int *  priority)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepIntVec ("priority", priority);

   if (theCall.argsOK ())
      theCall.myDemand ()->setPriority (priority);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandSearchInc (
      const char *         partName,
      const char *         demandName,
      const WitISRealArg & searchInc)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepLBDouble (
      "searchInc",
      theCall.myDemand ()->searchInc (),
      searchInc,
      0.000999,
      0.001);

   if (theCall.argsOK ())
      theCall.myDemand ()->setSearchInc (searchInc);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandSelForDel (
      const char * partName,
      const char * demandName,
      WitBoolean   selForDel)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.setSelForDel (selForDel);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandShipLateAllowed (
      const char * partName,
      const char * demandName,
      WitBoolean   shipLateAllowed)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepBool (
      "shipLateAllowed",
      theCall.myDemand ()->shipLateAllowed (),
      shipLateAllowed);

   if (theCall.argsOK ())
      theCall.myDemand ()->setShipLateAllowed (asaBool (shipLateAllowed));
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandShipLateLimit (
      const char * partName,
      const char * demandName,
      int          shipLateLimit)
   {
   WitTVec <int> & buildAheadUBVec = myApiMgr ()->reserveTempIntVec ();

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setVecAttrToScalMsg"))
      myMsgFac () (
         "setVecAttrToScalMsg",
         apiFuncName (),
         "demand",
         "shipLateLimit",
         "shipLateUB");

   buildAheadUBVec = shipLateLimit;

   witSetDemandShipLateUB (
      partName, 
      demandName, 
      buildAheadUBVec.myCVec ());

   myApiMgr ()->releaseTempVecs ();
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandShipLateUB (
      const char * partName,
      const char * demandName,
      const int *  shipLateUB)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepShipLateUB (shipLateUB);

   if (theCall.argsOK ())
      theCall.myDemand ()->setShipLateUB (shipLateUB);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandShipReward (
      const char *         partName,
      const char *         demandName,
      const WitIVRealArg & shipReward)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepDblVec ("shipReward", shipReward);

   if (theCall.argsOK ())
      theCall.myDemand ()->setShipReward (shipReward);
   }

//------------------------------------------------------------------------------

void WitRun::witSetDemandShipVol (
      const char *         partName,
      const char *         demandName,
      const WitIVRealArg & shipVol)
   {
   WitDemandApiCall theCall (this, partName, demandName);

   theCall.prepNonNegDblVec ("shipVol", shipVol);

   if (theCall.argsOK ())
      theCall.myDemand ()->setShipVol (shipVol);
   }

//------------------------------------------------------------------------------
// getDemandP
// Get pointer to Demand.
//------------------------------------------------------------------------------

WitDemand * WitRun::getDemandP (const char * partName, const char * demandName)
   {
   WitDemand * retVal;
   WitPart   * partP;

   partP = getPartP (partName);

   myInputID ()->setMyPart (partP);

   retVal = myCompMgr ()->findDemand (partP, demandName);

   if (retVal == NULL)
      myMsgFac () ("undefinedDemandMsg", demandName, partP->partName ());

   myInputID ()->setMyDemand (retVal);

   return retVal;
   }

//------------------------------------------------------------------------------
// eqHeurAlloc
//
// Does the work common to witEqHeurAlloc and witEqHeurAllocTwme.
//------------------------------------------------------------------------------

void WitRun::eqHeurAlloc (
      int                  lenLists,
      const char * const * partNameList,
      const char * const * demandNameList,
      const int *          shipPeriodList,
      const WitIVRealArg & desIncVolList,
      const WitOVRealArg & incVolList,
      const WitBoolean *   asapMultiExecList)
   {
   int              listIndex;
   int              dupIndex1;
   int              dupIndex2;
   WitDemand *      theDemand;
   WitPeriod        shipPer;
   bool             asapMultiExec;
   WitEqAllocator * theEqAllocator;
   WitDemPerStack   theDemPers (myProblem ());

   forbidStochMode ();

   forbidNullArg (partNameList,   "demandedPartNameList");
   forbidNullArg (demandNameList, "demandNameList");
   forbidNullArg (shipPeriodList, "shipPeriodList");
   forbidNullArg (desIncVolList,  "desIncVolList");
   forbidNullArg (incVolList   ,  "incVolList");

   if (myGlobalComp ()->twoWayMultiExec ())
      forbidNullArg (asapMultiExecList, "asapMultiExecList");
   else
      witAssert (asapMultiExecList == NULL);

   if (not myProblem ()->myHeurAllMgr ()->active ())
      myMsgFac () ("inactiveHeurAllocSmsg");

   theEqAllocator = myProblem ()->myEqAllocator ();

   theEqAllocator->activate ();

   if (lenLists < 1)
      {
      myInputID ()->setArgName ("lenLists");

      myMsgFac () ("argRangeValGeIntSmsg", lenLists, 1);
      }

   for (listIndex = 0; listIndex < lenLists; ++ listIndex)
      {
      myInputID ()->setArgIndex (listIndex);

      asapMultiExec = 
         myGlobalComp ()->twoWayMultiExec ()?
            asaBool (asapMultiExecList[listIndex]):
            false;

      defineAllocTarget (
         partNameList  [listIndex],
         demandNameList[listIndex],
         shipPeriodList[listIndex],
         desIncVolList [listIndex],
         asapMultiExec,
         theDemPers);
      }

   theDemPers.reverse ();

   myInputID ()->setArgIndex (-1);

   if (theEqAllocator->containsDup (theDemPers, dupIndex1, dupIndex2))
      myMsgFac () ("dupTargetSmsg",
         dupIndex1,
         dupIndex2,
         partNameList  [dupIndex1],
         demandNameList[dupIndex1],
         shipPeriodList[dupIndex1]);

   theEqAllocator->eqAlloc (theDemPers);

   theEqAllocator->print (theDemPers);

   apiAlloc (incVolList, lenLists);

   listIndex = -1;

   while (theDemPers.pop (theDemand, shipPer))
      {
      listIndex ++;

      incVolList[listIndex] = theEqAllocator->incVol (theDemand, shipPer);
      }
   }

//------------------------------------------------------------------------------
// incHeurAlloc
//
// Does the work common to witIncHeurAlloc and witIncHeurAllocTwme.
//------------------------------------------------------------------------------

void WitRun::incHeurAlloc (
      const char *         partName,
      const char *         demandName,
      int                  shipPer,
      const WitISRealArg & desIncVol,
      const WitOSRealArg & incVol,
      WitBoolean           asapMultiExec)
   {
   WitDemandApiCall theCall (this, partName, demandName);
   double           incVolVal;

   if (myGlobalComp ()->twoWayMultiExec ())
      myProblem ()->myHeurAllMgr ()->printMeDir (asaBool (asapMultiExec));

   if (not myProblem ()->myHeurAllMgr ()->active ())
      myMsgFac () ("inactiveHeurAllocSmsg");

   if (not ((shipPer >= 0) and (shipPer < nPeriods ())))
      {
      myInputID ()->setArgName ("shipPer");

      myMsgFac () ("argRangeIntLeValLtIntSmsg", shipPer, 0, nPeriods ());
      }

   if (not (desIncVol.asaDbl () >= 0.0))
      {
      myInputID ()->setArgName ("desIncVol");

      myMsgFac () ("argRangeValGeDblSmsg", desIncVol, 0.0);
      }

   incVolVal =
      myProblem ()->myHeurAllMgr ()->incHeurAlloc (
         theCall.myDemand (), 
         shipPer, 
         desIncVol,
         asaBool (asapMultiExec));

   apiGet (incVol, "incVol", incVolVal);

   if (myMsgFac ().mayIssue ("incHeurDataMsg"))
      myMsgFac () ("incHeurDataMsg", shipPer, desIncVol, incVolVal);
   }

//------------------------------------------------------------------------------

void WitRun::defineAllocTarget (
      const char *     partName,
      const char *     demandName,
      int              shipPer,
      double           desIncVol,
      bool             asapMultiExec,
      WitDemPerStack & theDemPers)
   {
   WitPart *   thePart;
   WitDemand * theDemand;


   forbidNullArg (partName, "demandedPartNameList");

   thePart = myCompMgr ()->findPart (partName);

   if (thePart == NULL)
      {
      myInputID ()->setArgName ("demandedPartNameList");

      myMsgFac () ("undefinedPartMsg", partName);
      }

   forbidNullArg (demandName, "demandNameList");

   theDemand = myCompMgr ()->findDemand (thePart, demandName);

   if (theDemand == NULL)
      {
      myInputID ()->setArgName ("demandNameList");

      myMsgFac () ("undefinedDemandMsg", demandName, partName);
      }

   if ((shipPer < 0) or (shipPer >= nPeriods ()))
      {
      myInputID ()->setArgName ("shipPeriodList");

      myMsgFac () ("argRangeIntLeValLtIntSmsg", shipPer, 0, nPeriods ());
      }

   theDemPers.push (theDemand, shipPer);

   if (desIncVol < 0.0)
      {
      myInputID ()->setArgName ("desIncVolList");

      myMsgFac () ("argRangeValGeDblSmsg", desIncVol, 0.0);
      }

   myProblem ()->myEqAllocator ()->setDesIncVol (theDemand, shipPer, desIncVol);

   if (myGlobalComp ()->twoWayMultiExec ())
       myProblem ()->myEqAllocator ()->setAsapMultiExec (
          theDemand, 
          shipPer, 
          asapMultiExec);
   }

//------------------------------------------------------------------------------

void WitRun::prepGetPegging (int shipPeriod)
   {
   if (myMsgFac ().mayIssue ("shipPerMsg"))
      myMsgFac () ("shipPerMsg", shipPeriod);

   if (not myGlobalComp ()->perfPegging ())
      myMsgFac () ("reqPerfPeggingSmsg");

   if (not ((shipPeriod >= 0) and (shipPeriod < nPeriods ())))
      {
      myInputID ()->setArgName ("shipPeriod");

      myMsgFac () ("argRangeIntLeValLtIntSmsg", shipPeriod, 0, nPeriods ());
      }
   }

//------------------------------------------------------------------------------

void WitRun::getPartPip (
      WitPartTripStack &   thePegTrips,
      int *                lenLists,
      char * * *           partNameList,
      int * *              periodList,
      const WitOVRealArg & peggedVolList)
   {
   int       nTrips;
   int       theIdx;
   WitPart * thePart;
   WitPeriod srcPer;
   double    pegVol;

   nTrips = thePegTrips.nElements ();

   apiGet (lenLists, "lenLists", nTrips);

   forbidNullArg (partNameList,  "partNameList");
   forbidNullArg (periodList,    "periodList");
   forbidNullArg (peggedVolList, "peggedVolList");

   apiAlloc (partNameList,  nTrips);
   apiAlloc (periodList,    nTrips);
   apiAlloc (peggedVolList, nTrips);

   theIdx = -1;

   while (thePegTrips.pop (thePart, srcPer, pegVol))
      {
      theIdx ++;

      (* partNameList) [theIdx] = thePart->partName ().apiDupCstring ();
      (* periodList)   [theIdx] = srcPer;
      peggedVolList    [theIdx] = pegVol;
      }
   }
