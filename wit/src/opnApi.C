//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "opnApi.C"
//
// Inner API functions relating to Operations.
//------------------------------------------------------------------------------

#include <WitRun.h>
#include <ApiMgr.h>
#include <InputID.h>
#include <ApiCall.h>
#include <ISRealArg.h>
#include <OSRealArg.h>
#include <IVRealArg.h>
#include <OVRealArg.h>
#include <Pre.h>
#include <DetOptImpMgr.h>
#include <ExtOptMgr.h>
#include <StochImpMgr.h>
#include <PipMgr.h>
#include <CompMgr.h>
#include <Operation.h>
#include <BopEntry.h>
#include <SubEntry.h>
#include <BoundSet.h>
#include <TripStack.h>
#include <MsgFrag.h>
#include <MsgFac.h>

#include <GlobalComp.h>

#include <stdlib.h>

//------------------------------------------------------------------------------
// setGetOperationInit
// Initilization of wit[GS]etOperation* functions.
//------------------------------------------------------------------------------

WitOperation * WitRun::setGetOperationInit (const char * operationName)
   {
   WitOperation * theOpn;

   forbidNullArg (operationName, "operationName");

   if (myMsgFac ().mayIssue ("operationNameMsg"))
      myMsgFac () ("operationNameMsg", operationName);

   theOpn = getOperationP (operationName);

   myInputID ()->setMyOperation (theOpn);

   return theOpn;
   }

//------------------------------------------------------------------------------

void WitRun::witAddOperation (const char * operationNameParm)
   {
   myApiMgr_->startInnerFunc ();

   forbidNullArg (operationNameParm, "operationName");

   if (myMsgFac ().mayIssue ("operationNameAdMsg"))
      myMsgFac () ("operationNameAdMsg", operationNameParm);

   if (myCompMgr ()->findOperation(operationNameParm) != NULL)
      myMsgFac () ("dupOperationApMsg", operationNameParm);

   if (isNullCstring (operationNameParm))
      myMsgFac () ("nullStringSmsg", "operationName");

   addingDelComp ("operationFrag");

   checkNPeriodsWasSet ();

   new WitOperation (operationNameParm, myCompMgr ());
   }

//------------------------------------------------------------------------------

void WitRun::witCopyOperationData (
      const char * dupOperationName,
      WitRun *     origWitRun,
      const char * origOperationName)
   {
   startCopyObjData (origWitRun);

   myMsgFac ()  ("dupObjectMsg", myMsgFac ().myFrag ("operationFrag"));

   WitOperationApiCall dupCall  (this,        dupOperationName);

   myMsgFac () ("origObjectMsg", myMsgFac ().myFrag ("operationFrag"));

   WitOperationApiCall origCall (origWitRun, origOperationName);

   if (dupCall.myOperation () == origCall.myOperation ())
      myMsgFac () ("copyObjectToSelfSmsg",
         myMsgFac ().myFrag ("operationFrag"));

   dupCall.myOperation ()->copyAttrsFrom (origCall.myOperation ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetObjItrOperation (char * * operationName)
   {
   WitOperation * theOpn;

   myApiMgr_->startInnerFunc ();

   theOpn =
      curItrDelCompForGet (WitAT_OPERATION, "operationFrag")->
         thisOpn ();

   stronglyAssert (theOpn != NULL);

   apiGet (operationName, "operationName", theOpn->operationName ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationAppData (
      const char * operationName,
      void * *     appData)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (appData, "appData", theCall.myOperation ()->appData ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationCoExecVolPip (
      const char *         operationName,
      int                  execPeriod,
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

   WitOperationApiCall theCall (this, operationName);

   theCall.prepGetPip (execPeriod);

   myProblem ()->
      myPipMgr ()->
         getCoExecVolPgg (theCall.myOperation (), execPeriod, coExecVolTrips);

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

void WitRun::witGetOperationConsVolPip (
      const char *         operationName,
      int                  execPeriod,
      int *                lenLists,
      char * * *           partNameList,
      int * *              periodList,
      const WitOVRealArg & peggedVolList)
   {
   WitPartTripStack consVolTrips (myProblem ());

   WitOperationApiCall theCall (this, operationName);

   theCall.prepGetPip (execPeriod);

   myProblem ()->
      myPipMgr ()->
         getConsVolPgg (theCall.myOperation (), execPeriod, consVolTrips);

   getPartPip (
      consVolTrips,
      lenLists,
      partNameList,
      periodList,
      peggedVolList);
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationCycleTime34 (
      const char *         operationName,
      const WitOVRealArg & cycleTime34)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (cycleTime34, "cycleTime34", theCall.myOperation ()->cycleTime34 ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationExecBounds (
      const char *         operationName,
      const WitOVRealArg & hardlbP,
      const WitOVRealArg & softlbP,
      const WitOVRealArg & hardubP)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (hardlbP, softlbP, hardubP, theCall.myOperation ()->execBounds ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationExecCost (
      const char *         operationName,
      const WitOVRealArg & execCost)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (
      execCost, 
      "execCost", 
      theCall.myOperation ()->execCost ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationExecPenalty (
      const char *         operationName,
      const WitOSRealArg & execPenalty)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (execPenalty, "execPenalty", theCall.myOperation ()->execPenalty ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationExecSlbConIndex (
      const char * operationName,
      const int    thePer,
      int *        execSlbConIndex)
   {
   WitOperationApiCall theCall (this, operationName);

   prepGetIndex (thePer);

   apiGet (
       execSlbConIndex,
      "execSlbConIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            execSlbConIndex (theCall.myOperation (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationExecSlbvVarIndex (
      const char * operationName,
      const int    thePer,
      int *        execSlbvVarIndex)
   {
   WitOperationApiCall theCall (this, operationName);

   prepGetIndex (thePer);

   apiGet (
       execSlbvVarIndex,
      "execSlbvVarIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            execSlbvVarIndex (theCall.myOperation (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationExecVarIndex (
      const char * operationName,
      const int    thePer,
      int *        execVarIndex)
   {
   WitOperationApiCall theCall (this, operationName);

   prepGetIndex (thePer);

   apiGet (
       execVarIndex,
      "execVarIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            execVarIndex (theCall.myOperation (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationExecVol (
      const char *         operationName,
      const WitOVRealArg & execVol)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (execVol, "execVol", theCall.myOperation ()->execVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationExecVolPip (
      const char *         operationName,
      int                  execPeriod,
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

   WitOperationApiCall theCall (this, operationName);

   theCall.prepGetPip (execPeriod);

   myProblem ()->
      myPipMgr ()->
         getExecVolPgg (theCall.myOperation (), execPeriod, execVolTrips);

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

void WitRun::witGetOperationExecutable (
      const char *   operationName,
      WitBoolean * * executable)
   {
   WitOperationApiCall theCall (this, operationName);

   myProblem ()->myPreprocessor ()->preprocess ();

   apiGet (
      executable,
      "executable",
      theCall.myOperation ()->executable ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationExists (
      const char * operationNameParm,
      WitBoolean * exists)
   {
   WitOperation * theOpn;

   myApiMgr_->startInnerFunc ();

   theOpn   = myCompMgr ()->findOperation (operationNameParm);

   * exists = (theOpn != NULL);
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationFssExecVol (
      const char *         operationName,
      const WitOVRealArg & fssExecVol)
   {
   WitOperationApiCall theCall (this, operationName);

   forbidStochMode ();

   myProblem ()->computeFssIfNeeded ();

   apiGet (fssExecVol, "fssExecVol", theCall.myOperation ()->fssExecVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationIncLotSize (
      const char *         operationName,
      const WitOVRealArg & incLotSize)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (incLotSize, "incLotSize", theCall.myOperation ()->incLotSize ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationIncLotSize2 (
      const char *         operationName,
      const WitOVRealArg & incLotSize2)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (incLotSize2, "incLotSize2", theCall.myOperation ()->incLotSize2 ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationIntExecVols (
      const char * operationName,
      WitBoolean * intExecVols)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (
       intExecVols,
      "intExecVols", 
      theCall.myOperation ()->intExecVols ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationLotSize2Thresh (
      const char *         operationName,
      const WitOVRealArg & lotSize2Thresh)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (
      lotSize2Thresh, 
      "lotSize2Thresh", 
      theCall.myOperation ()->lotSize2Thresh ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationMinLotSize (
      const char *         operationName,
      const WitOVRealArg & minLotSize)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (minLotSize, "minLotSize", theCall.myOperation ()->minLotSize ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationMinLotSize2 (
      const char *         operationName,
      const WitOVRealArg & minLotSize2)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (minLotSize2, "minLotSize2", theCall.myOperation ()->minLotSize2 ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationMrpExecVol (
      const char *         operationName,
      const WitOVRealArg & mrpExecVol)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (mrpExecVol, "mrpExecVol", theCall.myOperation ()->mrpExecVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationNBomEntries (
      const char * operationName,
      int *        nBomEntries)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (
      nBomEntries,
      "nBomEntries",
      theCall.myOperation ()->bom ().nElements ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationNBopEntries (
      const char * operationName,
      int *        nBopEntries)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (
      nBopEntries,
      "nBopEntries",
      theCall.myOperation ()->bop ().nElements ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationObj1ExecCost (
      const char *         operationName,
      const WitOVRealArg & obj1ExecCost)
   {
   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("getAttrNewNameMsg"))
      myMsgFac () (
         "getAttrNewNameMsg",
         apiFuncName (),
         "operation",
         "obj1ExecCost",
         "execCost");

   witGetOperationExecCost (operationName, obj1ExecCost);
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationObjectStage (
      const char * operationName,
      int *        objectStage)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.getObjectStage (objectStage);
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationPipEnabled (
      const char * operationName,
      WitBoolean * pipEnabled)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (
       pipEnabled,
      "pipEnabled", 
      theCall.myOperation ()->pipEnabled ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationPipRank (
      const char * operationName,
      int *        pipRank)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (
       pipRank,
      "pipRank", 
      theCall.myOperation ()->pipRank ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationProdVolPip (
      const char *         operationName,
      int                  execPeriod,
      int *                lenLists,
      char * * *           partNameList,
      int * *              periodList,
      const WitOVRealArg & peggedVolList)
   {
   WitPartTripStack prodVolTrips (myProblem ());

   WitOperationApiCall theCall (this, operationName);

   theCall.prepGetPip (execPeriod);

   myProblem ()->
      myPipMgr ()->
         getProdVolPgg (theCall.myOperation (), execPeriod, prodVolTrips);

   getPartPip (
      prodVolTrips,
      lenLists,
      partNameList,
      periodList,
      peggedVolList);
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperations (
      int  *     lenOperationList,
      char * * * operationList)
   {
   WitOperation * theOpn;
   int            i;

   myApiMgr_->startInnerFunc ();

   * lenOperationList = myCompMgr ()->nOperations ();

   apiAlloc (operationList, * lenOperationList);

   i = 0;

   forEachOperation (theOpn, myProblem ())
      (* operationList)[i++] = theOpn->operationName ().apiDupCstring ();
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationSelForDel (
      const char * operationName,
      WitBoolean * selForDel)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (selForDel, "selForDel", theCall.myOperation ()->selForDel ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationSideVolPip (
      const char *         operationName,
      int                  execPeriod,
      int *                lenLists,
      char * * *           partNameList,
      int * *              periodList,
      const WitOVRealArg & peggedVolList)
   {
   WitPartTripStack sideVolTrips (myProblem ());

   WitOperationApiCall theCall (this, operationName);

   theCall.prepGetPip (execPeriod);

   myProblem ()->
      myPipMgr ()->
         getSideVolPgg (theCall.myOperation (), execPeriod, sideVolTrips);

   getPartPip (
      sideVolTrips,
      lenLists,
      partNameList,
      periodList,
      peggedVolList);
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationSubVolPip (
      const char *         operationName,
      int                  execPeriod,
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

   WitOperationApiCall theCall (this, operationName);

   theCall.prepGetPip (execPeriod);

   myProblem ()->
      myPipMgr ()->
         getSubVolPgg (theCall.myOperation (), execPeriod, subVolTrips);

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

void WitRun::witGetOperationSupplyVolPip (
      const char *         operationName,
      int                  execPeriod,
      int *                lenLists,
      char * * *           partNameList,
      int * *              periodList,
      const WitOVRealArg & peggedVolList)
   {
   WitPartTripStack supplyVolTrips (myProblem ());

   WitOperationApiCall theCall (this, operationName);

   theCall.prepGetPip (execPeriod);

   myProblem ()->
      myPipMgr ()->
         getSupplyVolPgg (theCall.myOperation (), execPeriod, supplyVolTrips);

   getPartPip (
      supplyVolTrips,
      lenLists,
      partNameList,
      periodList,
      peggedVolList);
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationTwoLevelLotSizes (
      const char * operationName,
      WitBoolean * twoLevelLotSizes)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (
       twoLevelLotSizes,
      "twoLevelLotSizes", 
      theCall.myOperation ()->twoLevelLotSizes ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationYield (
      const char * operationName,
      int * *      yield)
   {
   double *  yieldRateCVec;
   WitPeriod thePer;

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("getPre50AttrMsg"))
      myMsgFac () (
         "getPre50AttrMsg",
         apiFuncName (),
         "operation",
         "yield",
         "yieldRate");

   witGetOperationYieldRate (operationName, & yieldRateCVec);

   forbidNullArg (yield, "yield");

   apiAlloc (yield, nPeriods ());

   forEachPeriod (thePer, myProblem ())
      (* yield)[thePer] = fracToPercent (yieldRateCVec[thePer]);

   free (yieldRateCVec);
   }

//------------------------------------------------------------------------------

void WitRun::witGetOperationYieldRate (
      const char *         operationName,
      const WitOVRealArg & yieldRate)
   {
   WitOperationApiCall theCall (this, operationName);

   apiGet (yieldRate, "yieldRate", theCall.myOperation ()->yieldRate ());
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationAppData (
      const char * operationName,
      void *       appData)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.setAppData (appData);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationCycleTime34 (
      const char *         operationName,
      const WitIVRealArg & cycleTime34)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.setCycleTime34 (cycleTime34);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationExecBounds (
      const char *         operationName,
      const WitIVRealArg & hardLB,
      const WitIVRealArg & softLB,
      const WitIVRealArg & hardUB)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.setBoundSet (
      theCall.myOperation ()->execBounds (), 
      hardLB, 
      softLB, 
      hardUB);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationExecCost (
      const char *         operationName,
      const WitIVRealArg & execCost)
   {
   WitOperationApiCall theCall (this, operationName);
   WitPeriod           thePer;

   theCall.prepDblVec ("execCost", execCost);

   if (theCall.argsOK ())
      {
      forEachPeriod (thePer, myProblem ())
         if (execCost[thePer] < 0.0)
            myMsgFac () (
               "negOperationCostWmsg",
               "execCost",
               operationName,
               execCost[thePer],
               thePer);

      theCall.myOperation ()->setExecCost (execCost);
      }
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationExecPenalty (
      const char *         operationName,
      const WitISRealArg & execPenalty)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.prepNonNegDouble (
      "execPenalty", 
      theCall.myOperation ()->execPenalty (),
      execPenalty);

   if (theCall.argsOK ())
      theCall.myOperation ()->setExecPenalty (execPenalty);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationExecVol (
      const char *         operationName,
      const WitIVRealArg & execVol)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.prepNonNegDblVec ("execVol", execVol);

   if (theCall.argsOK ())
      theCall.myOperation ()->setExecVol (execVol);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationIncLotSize (
      const char *         operationName,
      const WitIVRealArg & incLotSize)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.prepNonNegDblVec ("incLotSize", incLotSize);

   if (theCall.argsOK ())
      theCall.myOperation ()->setIncLotSize (incLotSize);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationIncLotSize2 (
      const char *         operationName,
      const WitIVRealArg & incLotSize2)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.prepNonNegDblVec ("incLotSize2", incLotSize2);

   if (theCall.argsOK ())
      theCall.myOperation ()->setIncLotSize2 (incLotSize2);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationIntExecVols (
      const char * operationName,
      WitBoolean   intExecVols)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.prepBool (
                             "intExecVols", 
      theCall.myOperation ()->intExecVols (), 
                              intExecVols);

   theCall.myOperation ()->setIntExecVols (asaBool (intExecVols));
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationLotSize2Thresh (
      const char *         operationName,
      const WitIVRealArg & lotSize2Thresh)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.prepNonNegDblVec ("lotSize2Thresh", lotSize2Thresh);

   if (theCall.argsOK ())
      theCall.myOperation ()->setLotSize2Thresh (lotSize2Thresh);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationMinLotSize (
      const char *         operationName,
      const WitIVRealArg & minLotSize)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.prepNonNegDblVec ("minLotSize", minLotSize);

   if (theCall.argsOK ())
      theCall.myOperation ()->setMinLotSize (minLotSize);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationMinLotSize2 (
      const char *         operationName,
      const WitIVRealArg & minLotSize2)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.prepNonNegDblVec ("minLotSize2", minLotSize2);

   if (theCall.argsOK ())
      theCall.myOperation ()->setMinLotSize2 (minLotSize2);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationObj1ExecCost (
      const char *         operationName,
      const WitIVRealArg & obj1ExecCost)
   {
   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setAttrNewNameMsg"))
      myMsgFac () (
         "setAttrNewNameMsg",
         apiFuncName (),
         "operation",
         "obj1ExecCost",
         "execCost");

   witSetOperationExecCost (operationName, obj1ExecCost);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationObjectStage (
      const char * operationName,
      int          objectStage)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.setObjectStage (objectStage);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationOperationName (
      const char * operationName,
      const char * newName)
   {
   WitOperationApiCall theCall (this, operationName);

   forbidNullArg (newName, "newName");

   if (isNullCstring (newName))
      myMsgFac () ("nullStringSmsg", "newName");

   if (myCompMgr ()->findOperation (newName))
      myMsgFac () ("setNodeNameToDupSmsg",
         "witSetOperationOperationName",
         myMsgFac ().myFrag ("operationFrag"),
         newName);

   theCall.prepString (
                             "operationName",
      theCall.myOperation ()->operationName (),
                                    newName);

   if (theCall.argsOK ())
      theCall.myOperation ()->setOperationName (newName);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationPipEnabled (
      const char * operationName,
      WitBoolean   pipEnabled)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.prepBool (
                             "pipEnabled", 
      theCall.myOperation ()->pipEnabled (), 
                              pipEnabled);

   theCall.myOperation ()->setPipEnabled (asaBool (pipEnabled));
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationPipRank (
      const char * operationName,
      int          pipRank)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.prepInt (
                             "pipRank", 
      theCall.myOperation ()->pipRank (), 
                              pipRank);

   theCall.myOperation ()->setPipRank (pipRank);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationSelForDel (
      const char * operationName,
      WitBoolean   selForDel)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.setSelForDel (selForDel);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationTwoLevelLotSizes (
      const char * operationName,
      WitBoolean   twoLevelLotSizes)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.myOperation ()->setTwoLevelLotSizes (asaBool (twoLevelLotSizes));
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationYield (
      const char * operationName,
      const int *  yield)
   {
   WitTVec <double> yieldRateTVec (myProblem (), 0.0);
   WitPeriod        thePer;

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setPre50AttrMsg"))
      myMsgFac () (
         "setPre50AttrMsg",
         apiFuncName (),
         "operation",
         "yield",
         "yieldRate");

   forbidNullArg (yield, "yield");

   forEachPeriod (thePer, myProblem ())
      yieldRateTVec[thePer] = percentToFrac (yield[thePer]);

   witSetOperationYieldRate (operationName, yieldRateTVec.myCVec ());
   }

//------------------------------------------------------------------------------

void WitRun::witSetOperationYieldRate (
      const char *         operationName,
      const WitIVRealArg & yieldRate)
   {
   WitOperationApiCall theCall (this, operationName);

   theCall.setYieldRate (yieldRate);
   }

//------------------------------------------------------------------------------
// getOperationP
// Return pointer to operation.  If not defined issue message.
//------------------------------------------------------------------------------

WitOperation * WitRun::getOperationP (const char * operationName)
   {
   WitOperation * operationP;

   operationP = myCompMgr ()->findOperation (operationName);

   if (operationP == NULL)
      myMsgFac () ("undefinedOperationMsg", operationName);

   return operationP;
   }
