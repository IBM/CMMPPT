//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "globApi.C"
//
// Inner API functions relating to global attributes, other than for
// optimizing implosion.
//------------------------------------------------------------------------------

#include <WitRun.h>
#include <ApiMgr.h>
#include <ApiCall.h>
#include <HeurAllMgr.h>
#include <Pre.h>
#include <Post.h>
#include <Part.h>
#include <GlobalComp.h>
#include <Demand.h>
#include <PclEl.h>
#include <TripStack.h>
#include <PipMgr.h>
#include <ISRealArg.h>
#include <OSRealArg.h>
#include <OVRealArg.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------

void WitRun::witClearPipSeq ()
   {
   WitGlobalCompApiCall theCall (this);

   myProblem ()->myPipMgr ()->clearShipSeq ();
   }

//------------------------------------------------------------------------------

void WitRun::witGetAppData (void * * appData)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (appData, "appData", theCall.myGlobalComp ()->appData ());

   issueGetAttrMsg ("appData");
   }

//------------------------------------------------------------------------------

void WitRun::witGetAutoPriority (WitBoolean * autoPriority)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    autoPriority, 
                              "autoPriority", 
      theCall.myGlobalComp ()->autoPriority ());


   issueGetAttrMsg ("autoPriority");
   }

//------------------------------------------------------------------------------

void WitRun::witGetBuildWhenEmpty (WitBoolean * buildWhenEmpty)
   {
   WitGlobalCompApiCall theCall (this);

   if (wit34Compatible ())
      {
      myMsgFac () ("getting34AttributeWithCompatibilityMode",
         apiFuncName (),
         "Global",
         "buildWhenEmpty",
         "Global",
         "execEmptyBom");

      witGetExecEmptyBom (buildWhenEmpty);
      }
   else
      myMsgFac () ("getting34AttributeWithoutCompatibilityMode",
         apiFuncName (),
         "Global",
         "buildWhenEmpty",
         "Global",
         "execEmptyBom");
   }

//------------------------------------------------------------------------------

void WitRun::witGetComputeCriticalList (
      WitBoolean * computeCriticalList)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (
                               computeCriticalList,
                              "computeCriticalList",
      theCall.myGlobalComp ()->computeCriticalList ());

   issueGetAttrMsg ("computeCriticalList");
   }

//------------------------------------------------------------------------------

void WitRun::witGetCriticalList (
      int  *     lenCritList,
      char * * * partList,
      int  * *   perList)
   {
   int       theIdx;
   WitPart * thePart;
   WitPeriod thePer;

   WitPairStItr <WitPart, WitPeriod> theItr;
   WitGlobalCompApiCall              theCall (this);

   forbidNullArg (lenCritList, "lenCritList");
   forbidNullArg (partList,    "critPartList");
   forbidNullArg (perList,     "critPeriod");

   * lenCritList = theCall.myGlobalComp ()->revCritList ().nElements ();

   apiAlloc (partList, * lenCritList);
   apiAlloc (perList,  * lenCritList);

   theIdx = * lenCritList;

   theCall.myGlobalComp ()->revCritList ().attachItr (theItr);

   while (theItr.advance (thePart, thePer))
      {
      theIdx --;

      (* partList)[theIdx] = thePart->partName ().apiDupCstring ();
      (* perList) [theIdx] = thePer;
      }
   }

//------------------------------------------------------------------------------

void WitRun::witGetEquitability (int * equitability)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    equitability,
                              "equitability",
      theCall.myGlobalComp ()->equitability ());

   issueGetAttrMsg ("equitability");
   }

//------------------------------------------------------------------------------

void WitRun::witGetExecEmptyBom (WitBoolean * execEmptyBom)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    execEmptyBom, 
                              "execEmptyBom", 
      theCall.myGlobalComp ()->execEmptyBom ());

   issueGetAttrMsg ("execEmptyBom");
   }

//------------------------------------------------------------------------------

void WitRun::witGetExpCutoff (const WitOSRealArg & expCutoff)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (expCutoff, "expCutoff", theCall.myGlobalComp ()->expCutoff ());

   issueGetAttrMsg ("expCutoff");
   }

//------------------------------------------------------------------------------

void WitRun::witGetFeasible (WitBoolean * feasible)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (feasible, "feasible", theCall.myGlobalComp ()->feasible ());

   issueGetAttrMsg ("feasible");
   }

//------------------------------------------------------------------------------

void WitRun::witGetForcedMultiEq (WitBoolean * forcedMultiEq)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    forcedMultiEq, 
                              "forcedMultiEq", 
      theCall.myGlobalComp ()->forcedMultiEq ());

   issueGetAttrMsg ("forcedMultiEq");
   }

//------------------------------------------------------------------------------

void WitRun::witGetHashTableSize (int * hashTableSize)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    hashTableSize, 
                              "hashTableSize", 
      theCall.myGlobalComp ()->hashTableSize ());

   issueGetAttrMsg ("hashTableSize");
   }

//------------------------------------------------------------------------------

void WitRun::witGetHeurAllocActive (WitBoolean * heurAllocActive)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (
       heurAllocActive,
      "heurAllocActive",
      myProblem ()->myHeurAllMgr ()->active ());

   issueGetAttrMsg ("heurAllocActive");
   }

//------------------------------------------------------------------------------

void WitRun::witGetHighPrecisionWD (WitBoolean * highPrecisionWD)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (highPrecisionWD, 
      "highPrecisionWD", 
      theCall.myGlobalComp ()->highPrecisionWD ());

   issueGetAttrMsg ("highPrecisionWD");
   }

//------------------------------------------------------------------------------

void WitRun::witGetIndependentOffsets (WitBoolean * independentOffsets)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    independentOffsets, 
                              "independentOffsets", 
      theCall.myGlobalComp ()->independentOffsets ());

   issueGetAttrMsg ("independentOffsets");
   }

//------------------------------------------------------------------------------

void WitRun::witGetLocalBuildAhead (WitBoolean * localBuildAhead)
   {
   WitGlobalCompApiCall theCall (this);

   myMsgFac () ("lbaWmsg", "witGetLocalBuildAhead");

   apiGet (                    localBuildAhead, 
                              "localBuildAhead",
      theCall.myGlobalComp ()->localBuildAhead ());

   issueGetAttrMsg ("localBuildAhead");
   }

//------------------------------------------------------------------------------

void WitRun::witGetLocalMultiExec (WitBoolean * localMultiExec)
   {
   WitGlobalCompApiCall theCall (this);

   myMsgFac () ("lmeWmsg", "witGetLocalMultiExec");

   apiGet (                    localMultiExec,
                              "localMultiExec",
      theCall.myGlobalComp ()->localMultiExec ());

   issueGetAttrMsg ("localMultiExec");
   }

//------------------------------------------------------------------------------

void WitRun::witGetLotSizeTol (const WitOSRealArg & lotSizeTol)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (lotSizeTol, "lotSizeTol", theCall.myGlobalComp ()->lotSizeTol ());

   issueGetAttrMsg ("lotSizeTol");
   }

//------------------------------------------------------------------------------

void WitRun::witGetMinimalExcess  (WitBoolean * minimalExcess)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    minimalExcess,
                              "minimalExcess", 
      theCall.myGlobalComp ()->minimalExcess ());

   issueGetAttrMsg ("minimalExcess");
   }

//------------------------------------------------------------------------------

void WitRun::witGetModHeurAlloc (WitBoolean * modHeurAlloc)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (modHeurAlloc, 
      "modHeurAlloc", 
      theCall.myGlobalComp ()->modHeurAlloc ());

   issueGetAttrMsg ("modHeurAlloc");
   }

//------------------------------------------------------------------------------

void WitRun::witGetMultiExec (WitBoolean * multiExec)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (multiExec, "multiExec", theCall.myGlobalComp ()->multiExec ());

   issueGetAttrMsg ("multiExec");
   }

//------------------------------------------------------------------------------

void WitRun::witGetMultiRoute  (WitBoolean * multiRoute)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (multiRoute, "multiRoute", theCall.myGlobalComp ()->multiRoute ());

   issueGetAttrMsg ("multiRoute");
   }

//------------------------------------------------------------------------------

void WitRun::witGetNPeriods (int * nPeriodsArg)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (nPeriodsArg, "nPeriods", nPeriods ());

   issueGetAttrMsg ("nPeriods");
   }

//------------------------------------------------------------------------------

void WitRun::witGetNewPipPggOrder (WitBoolean * newPipPggOrder)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    newPipPggOrder, 
                              "newPipPggOrder", 
      theCall.myGlobalComp ()->newPipPggOrder ());

   issueGetAttrMsg ("newPipPggOrder");
   }

//------------------------------------------------------------------------------

void WitRun::witGetNonMrSelSplit (WitBoolean * nonMrSelSplit)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (nonMrSelSplit, 
      "nonMrSelSplit", 
      theCall.myGlobalComp ()->nonMrSelSplit ());

   issueGetAttrMsg ("nonMrSelSplit");
   }

//------------------------------------------------------------------------------

void WitRun::witGetNstnResidual (WitBoolean * nstnResidual)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (nstnResidual, 
      "nstnResidual", 
      theCall.myGlobalComp ()->nstnResidual ());

   issueGetAttrMsg ("nstnResidual");
   }

//------------------------------------------------------------------------------

void WitRun::witGetOutputPrecision (int * outputPrecision)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    outputPrecision, 
                              "outputPrecision",
      theCall.myGlobalComp ()->outputPrecision ());

   issueGetAttrMsg ("outputPrecision");
   }

//------------------------------------------------------------------------------

void WitRun::witGetPenExec (WitBoolean * penExec)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (penExec, "penExec", theCall.myGlobalComp ()->penExec ());

   issueGetAttrMsg ("penExec");
   }

//------------------------------------------------------------------------------

void WitRun::witGetPerfPegging (WitBoolean * perfPegging)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (perfPegging, 
      "perfPegging", 
      theCall.myGlobalComp ()->perfPegging ());

   issueGetAttrMsg ("perfPegging");
   }

//------------------------------------------------------------------------------

void WitRun::witGetPgdCritList (
      int *      lenLists,
      char * * * critPartNameList,
      int * *    critPerList,
      char * * * demPartNameList,
      char * * * demandNameList,
      int * *    shipPerList)
   {
   int        theIdx;
   WitPclEl * thePclEl;

   WitGlobalCompApiCall theCall (this);

   forbidNullArg (lenLists,         "lenLists");
   forbidNullArg (critPartNameList, "critPartNameList");
   forbidNullArg (critPerList,      "critPerList");
   forbidNullArg (demPartNameList,  "demPartNameList");
   forbidNullArg (demandNameList,   "demandNameList");
   forbidNullArg (shipPerList,      "shipPerList");

   * lenLists = theCall.myGlobalComp ()->pgdCritList ().nElements ();

   apiAlloc (critPartNameList, * lenLists);
   apiAlloc (critPerList,      * lenLists);
   apiAlloc (demPartNameList,  * lenLists);
   apiAlloc (demandNameList,   * lenLists);
   apiAlloc (shipPerList,      * lenLists);

   theIdx = 0;

   forEachEl (thePclEl, theCall.myGlobalComp ()->pgdCritList ())
      {
      (* critPartNameList)[theIdx] =
         thePclEl->critPart ()->partName ().apiDupCstring ();

      (* critPerList)[theIdx] = thePclEl->critPer ();

      (* demPartNameList)[theIdx] =
         thePclEl->myDemand ()->demandedPartName ().apiDupCstring ();

      (* demandNameList)[theIdx] =
         thePclEl->myDemand ()->demandName ().apiDupCstring ();

      (* shipPerList)[theIdx] = thePclEl->myShipPer ();

      theIdx ++;
      }
   }

//------------------------------------------------------------------------------

void WitRun::witGetPgdCritListMode (WitBoolean * pgdCritListMode)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (
                               pgdCritListMode,
                              "pgdCritListMode",
      theCall.myGlobalComp ()->pgdCritListMode ());

   issueGetAttrMsg ("pgdCritListMode");
   }

//------------------------------------------------------------------------------

void WitRun::witGetPipExists (WitBoolean * pipExists)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (pipExists, "pipExists", myProblem ()->myPipMgr ()->peggingExists ());

   issueGetAttrMsg ("pipExists");
   }

//------------------------------------------------------------------------------

void WitRun::witGetPipSeq (
      int *                lenLists,
      char * * *           partNameList,
      char * * *           demandNameList,
      int * *              shipPerList,
      const WitOVRealArg & incShipVolList)
   {
   WitDemTripStack theShipSeq (myProblem ());
   int             nTrips;
   int             theIdx;
   WitDemand *     theDemand;
   WitPeriod       shipPer;
   double          incShipVol;

   WitGlobalCompApiCall theCall (this);

   myProblem ()->myPipMgr ()->getShipSeq (theShipSeq);

   nTrips = theShipSeq.nElements ();

   apiGet (lenLists, "lenLists", nTrips);

   forbidNullArg (partNameList,   "partNameList");
   forbidNullArg (demandNameList, "demandNameList");
   forbidNullArg (shipPerList,    "shipPerList");
   forbidNullArg (incShipVolList, "incShipVolList");

   apiAlloc (partNameList,   nTrips);
   apiAlloc (demandNameList, nTrips);
   apiAlloc (shipPerList,    nTrips);
   apiAlloc (incShipVolList, nTrips);

   theIdx = -1;

   while (theShipSeq.pop (theDemand, shipPer, incShipVol))
      {
      theIdx ++;

      (* partNameList)  [theIdx] =
         theDemand->demandedPartName ().apiDupCstring ();

      (* demandNameList)[theIdx] = theDemand->demandName ().apiDupCstring ();
      (* shipPerList)   [theIdx] = shipPer;
      incShipVolList    [theIdx] = incShipVol;
      }
   }

//------------------------------------------------------------------------------

void WitRun::witGetPipSeqFromHeur (WitBoolean * pipSeqFromHeur)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (pipSeqFromHeur, 
      "pipSeqFromHeur", 
      theCall.myGlobalComp ()->pipSeqFromHeur ());

   issueGetAttrMsg ("pipSeqFromHeur");
   }

//------------------------------------------------------------------------------

void WitRun::witGetPostprocessed (WitBoolean * postprocessed)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (
       postprocessed,
      "postprocessed",
      myProblem ()->myPostprocessor ()->postprocessed ());

   issueGetAttrMsg ("postprocessed");
   }

//------------------------------------------------------------------------------

void WitRun::witGetPrefHighStockSLBs (WitBoolean * prefHighStockSLBs)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (prefHighStockSLBs, 
      "prefHighStockSLBs", 
      theCall.myGlobalComp ()->prefHighStockSLBs ());

   issueGetAttrMsg ("prefHighStockSLBs");
   }

//------------------------------------------------------------------------------

void WitRun::witGetPreprocessed (WitBoolean * preprocessed)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (
       preprocessed,
      "preprocessed",
      myProblem ()->myPreprocessor ()->preprocessed ());

   issueGetAttrMsg ("preprocessed");
   }

//------------------------------------------------------------------------------

void WitRun::witGetRespectStockSLBs (WitBoolean * respectStockSLBs)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (respectStockSLBs, 
      "respectStockSLBs", 
      theCall.myGlobalComp ()->respectStockSLBs ());

   issueGetAttrMsg ("respectStockSLBs");
   }

//------------------------------------------------------------------------------

void WitRun::witGetRoundReqVols (WitBoolean * roundReqVols)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    roundReqVols, 
                              "roundReqVols", 
      theCall.myGlobalComp ()->roundReqVols ());

   issueGetAttrMsg ("roundReqVols");
   }

//------------------------------------------------------------------------------

void WitRun::witGetSelSplit (WitBoolean * selSplit)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (selSplit, 
      "selSplit", 
      theCall.myGlobalComp ()->selSplit ());

   issueGetAttrMsg ("selSplit");
   }

//------------------------------------------------------------------------------

void WitRun::witGetSelectionRecovery  (WitBoolean * selectionRecovery)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    selectionRecovery,
                              "selectionRecovery",
      theCall.myGlobalComp ()->selectionRecovery ());

   issueGetAttrMsg ("selectionRecovery");
   }

//------------------------------------------------------------------------------

void WitRun::witGetSkipFailures (WitBoolean * skipFailures)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    skipFailures, 
                              "skipFailures", 
      theCall.myGlobalComp ()->skipFailures ());

   issueGetAttrMsg ("skipFailures");
   }

//------------------------------------------------------------------------------

void WitRun::witGetStockRealloc (WitBoolean * stockRealloc)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    stockRealloc, 
                              "stockRealloc", 
      theCall.myGlobalComp ()->stockRealloc ());

   issueGetAttrMsg ("stockRealloc");
   }

//------------------------------------------------------------------------------

void WitRun::witGetStockReallocation (WitBoolean * stockReallocation)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    stockReallocation, 
                              "stockReallocation", 
      theCall.myGlobalComp ()->stockReallocation ());

   issueGetAttrMsg ("stockReallocation");
   }

//------------------------------------------------------------------------------

void WitRun::witGetSrSelSplit (WitBoolean * srSelSplit)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    srSelSplit, 
                              "srSelSplit", 
      theCall.myGlobalComp ()->srSelSplit ());

   issueGetAttrMsg ("srSelSplit");
   }

//------------------------------------------------------------------------------

void WitRun::witGetTieBreakPropRt (WitBoolean * tieBreakPropRt)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    tieBreakPropRt,
                              "tieBreakPropRt",
      theCall.myGlobalComp ()->tieBreakPropRt ());

   issueGetAttrMsg ("tieBreakPropRt");
   }

//------------------------------------------------------------------------------

void WitRun::witGetTitle (char * * title)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (title, "title", theCall.myGlobalComp ()->title ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetTruncOffsets  (WitBoolean * truncOffsets)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    truncOffsets, 
                              "truncOffsets", 
      theCall.myGlobalComp ()->truncOffsets ());

   issueGetAttrMsg ("truncOffsets");
   }

//------------------------------------------------------------------------------

void WitRun::witGetTwoWayMultiExec (WitBoolean * twoWayMultiExec)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    twoWayMultiExec, 
                              "twoWayMultiExec", 
      theCall.myGlobalComp ()->twoWayMultiExec ());

   issueGetAttrMsg ("twoWayMultiExec");
   }

//------------------------------------------------------------------------------

void WitRun::witGetUseFocusHorizons (WitBoolean * useFocusHorizons)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (                    useFocusHorizons,
                              "useFocusHorizons",
      theCall.myGlobalComp ()->useFocusHorizons ());

   issueGetAttrMsg ("useFocusHorizons");
   }

//------------------------------------------------------------------------------

void WitRun::witGetUserHeurStart (WitBoolean * userHeurStart)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (userHeurStart, 
      "userHeurStart", 
      theCall.myGlobalComp ()->userHeurStart ());

   issueGetAttrMsg ("userHeurStart");
   }

//------------------------------------------------------------------------------

void WitRun::witGetWit34Compatible (WitBoolean * retVal)
   {
   WitGlobalCompApiCall theCall (this);

   apiGet (retVal, "wit34Compatible", wit34Compatible ());

   issueGetAttrMsg ("wit34Compatible");
   }

//------------------------------------------------------------------------------

void WitRun::witSetAppData (void * appData)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.setAppData (appData);
   }

//------------------------------------------------------------------------------

void WitRun::witSetAutoPriority (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "autoPriority", 
      theCall.myGlobalComp ()->autoPriority (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setAutoPriority (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetBuildWhenEmpty (WitBoolean boolval)
   {
   myApiMgr_->startInnerFunc ();

   if (wit34Compatible ())
      {
      myMsgFac () ("setting34AttributeWithCompatibilityMode",
         apiFuncName (),
         "Global",
         "buildWhenEmpty",
         "Global",
         "execEmptyBom");

      witSetExecEmptyBom (asaBool (boolval));
      }
   else
      myMsgFac () ("setting34AttributeWithoutCompatibilityMode",
         apiFuncName (),
         "Global",
         "buildWhenEmpty",
         "Global",
         "execEmptyBom");
   }

//------------------------------------------------------------------------------

void WitRun::witSetComputeCriticalList (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "computeCriticalList", 
      theCall.myGlobalComp ()->computeCriticalList (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setComputeCriticalList (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetEquitability (int equit)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepRangedInt (
      "equitability",
      theCall.myGlobalComp ()->equitability (),
      equit,
      1,
      100);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setEquitability (equit);
   }

//------------------------------------------------------------------------------

void WitRun::witSetExecEmptyBom (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "execEmptyBom", 
      theCall.myGlobalComp ()->execEmptyBom (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setExecEmptyBom (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetExpCutoff (const WitISRealArg & theVal)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepLBDouble (
      "expCutoff",
      theCall.myGlobalComp ()->expCutoff (),
      theVal,
      0.999E-6,
      1.0E-6);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setExpCutoff (theVal);
   }

//------------------------------------------------------------------------------

void WitRun::witSetForcedMultiEq (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "forcedMultiEq", 
      theCall.myGlobalComp ()->forcedMultiEq (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setForcedMultiEq (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetHashTableSize (int size)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepNonNegInt (
      "hashTableSize", 
      theCall.myGlobalComp ()->hashTableSize (), 
      size);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setHashTableSize (size);
   }

//------------------------------------------------------------------------------

void WitRun::witSetHighPrecisionWD (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "highPrecisionWD", 
      theCall.myGlobalComp ()->highPrecisionWD (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setHighPrecisionWD (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetIndependentOffsets (WitBoolean theValue)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.reqNoPartsOrOpns ("independentOffsets");

   theCall.prepBool (
      "independentOffsets", 
      theCall.myGlobalComp ()->independentOffsets (), 
      theValue);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setIndependentOffsets (asaBool (theValue));
   }

//------------------------------------------------------------------------------

void WitRun::witSetLocalBuildAhead (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "localBuildAhead", 
      theCall.myGlobalComp ()->localBuildAhead (), 
      boolval);

   myMsgFac () ("lbaWmsg", "witSetLocalBuildAhead");

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setLocalBuildAhead (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetLocalMultiExec (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "localMultiExec", 
      theCall.myGlobalComp ()->localMultiExec (), 
      boolval);

   myMsgFac () ("lmeWmsg", "witSetLocalMultiExec");

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setLocalMultiExec (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetLotSizeTol (const WitISRealArg & theVal)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepNonNegDouble (
      "lotSizeTol", 
      theCall.myGlobalComp ()->lotSizeTol (),
      theVal);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setLotSizeTol (theVal);
   }

//------------------------------------------------------------------------------

void WitRun::witSetMinimalExcess (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "minimalExcess", 
      theCall.myGlobalComp ()->minimalExcess (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setMinimalExcess (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetModHeurAlloc (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "modHeurAlloc", 
      theCall.myGlobalComp ()->modHeurAlloc (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setModHeurAlloc (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetMultiExec (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   if (not boolval)
      if (myGlobalComp ()->twoWayMultiExec ())
         theCall.prepBool ("twoWayMultiExec", true, WitFALSE);

   theCall.prepBool (
      "multiExec", 
      theCall.myGlobalComp ()->multiExec (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setMultiExec (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetMultiRoute (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "multiRoute", 
      theCall.myGlobalComp ()->multiRoute (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setMultiRoute (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetNPeriods (int nper)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.reqNoPartsOrOpns ("nPeriods");

   theCall.prepImmPosInt (
      "nPeriods", 
      theCall.myGlobalComp ()->nPeriods (), 
      nper);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setNPeriods (nper);
   }

//------------------------------------------------------------------------------

void WitRun::witSetNewPipPggOrder (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "newPipPggOrder", 
      theCall.myGlobalComp ()->newPipPggOrder (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setNewPipPggOrder (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetNonMrSelSplit (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   if (boolval)
      if (myGlobalComp ()->selSplit ())
         theCall.prepBool ("selSplit", true, WitFALSE);

   theCall.prepBool (
      "nonMrSelSplit", 
      theCall.myGlobalComp ()->nonMrSelSplit (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setNonMrSelSplit (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetNstnResidual (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "nstnResidual", 
      theCall.myGlobalComp ()->nstnResidual (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setNstnResidual (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetOutputPrecision (int outputPrecisionParm)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepNonNegInt (
      "outputPrecision", 
      theCall.myGlobalComp ()->outputPrecision (), 
      outputPrecisionParm);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setOutputPrecision (outputPrecisionParm);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPenExec (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool ("penExec", theCall.myGlobalComp ()->penExec (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setPenExec (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetPerfPegging (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "perfPegging", 
      theCall.myGlobalComp ()->perfPegging (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setPerfPegging (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetPgdCritListMode (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "pgdCritListMode", 
      theCall.myGlobalComp ()->pgdCritListMode (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setPgdCritListMode (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetPipSeqFromHeur (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "pipSeqFromHeur", 
      theCall.myGlobalComp ()->pipSeqFromHeur (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setPipSeqFromHeur (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetPrefHighStockSLBs (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "prefHighStockSLBs", 
      theCall.myGlobalComp ()->prefHighStockSLBs (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setPrefHighStockSLBs (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetRespectStockSLBs (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "respectStockSLBs", 
      theCall.myGlobalComp ()->respectStockSLBs (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setRespectStockSLBs (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetRoundReqVols (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "roundReqVols", 
      theCall.myGlobalComp ()->roundReqVols (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setRoundReqVols (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetSelSplit (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   if (boolval)
      if (myGlobalComp ()->nonMrSelSplit ())
         theCall.prepBool ("nonMrSelSplit", true, WitFALSE);

   theCall.prepBool (
      "selSplit", 
      theCall.myGlobalComp ()->selSplit (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setSelSplit (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetSelectionRecovery (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "selectionRecovery", 
      theCall.myGlobalComp ()->selectionRecovery (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setSelectionRecovery (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetSkipFailures (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "skipFailures", 
      theCall.myGlobalComp ()->skipFailures (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setSkipFailures (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetStockRealloc (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   if (boolval)
      if (theCall.myGlobalComp ()->stockReallocation ())
         myMsgFac () ("bothTrueSmsg", "stockRealloc", "stockReallocation");

   theCall.prepBool (
      "stockRealloc", 
      theCall.myGlobalComp ()->stockRealloc (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setStockRealloc (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetStockReallocation (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   if (boolval)
      if (theCall.myGlobalComp ()->stockRealloc ())
         myMsgFac () ("bothTrueSmsg", "stockReallocation", "stockRealloc");

   theCall.prepBool (
      "stockReallocation", 
      theCall.myGlobalComp ()->stockReallocation (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setStockReallocation (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetSrSelSplit (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   if (boolval)
      if (theCall.myGlobalComp ()->stockRealloc ())
         myMsgFac () ("bothTrueSmsg", "srSelSplit", "stockRealloc");

   theCall.prepBool (
      "srSelSplit", 
      theCall.myGlobalComp ()->srSelSplit (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setSrSelSplit (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetTieBreakPropRt (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
                              "tieBreakPropRt",
      theCall.myGlobalComp ()->tieBreakPropRt (),
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setTieBreakPropRt (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetTitle (const char * newTitle)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepString ("title", theCall.myGlobalComp ()->title (), newTitle);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setTitle (newTitle);
   }

//------------------------------------------------------------------------------

void WitRun::witSetTruncOffsets  (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "truncOffsets", 
      theCall.myGlobalComp ()->truncOffsets (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setTruncOffsets (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetTwoWayMultiExec (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   if (boolval)
      if (not myGlobalComp ()->multiExec ())
         theCall.prepBool ("multiExec", false, WitTRUE);

   theCall.prepBool (
      "twoWayMultiExec", 
      theCall.myGlobalComp ()->twoWayMultiExec (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setTwoWayMultiExec (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetUseFocusHorizons (WitBoolean newValue)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "useFocusHorizons", 
      theCall.myGlobalComp ()->useFocusHorizons (), 
      newValue);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setUseFocusHorizons (asaBool (newValue));
   }

//------------------------------------------------------------------------------

void WitRun::witSetUserHeurStart (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "userHeurStart", 
      theCall.myGlobalComp ()->userHeurStart (), boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setUserHeurStart (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetWit34Compatible (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   if (not theCall.myGlobalComp ()->wit34Allowed ())
      myMsgFac () ("setWit34CompatibleNotAllowedSmsg");

   theCall.prepBool (
      "wit34Compatible", 
      theCall.myGlobalComp ()->wit34Compatible (), 
      boolval);

   if (boolval)
      myMsgFac () ("compatible34WarningMsg");

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setWit34Compatible (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetWit34Allowed (WitBoolean boolval)
   {
   WitGlobalCompApiCall theCall (this);

   theCall.prepBool (
      "wit34Allowed", 
      theCall.myGlobalComp ()->wit34Allowed (), 
      boolval);

   if (theCall.argsOK ())
      theCall.myGlobalComp ()->setWit34Allowed (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::issueGetAttrMsg (const char * theAttName) const
   {
   if (myMsgFac ().mayIssue ("getAttrMsg"))
      myMsgFac () ("getAttrMsg", theAttName);
   }

//------------------------------------------------------------------------------

bool WitRun::alwaysTrue ()
   {
   return true;
   }
