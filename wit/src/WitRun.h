//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef WitRunH
#define WitRunH

//------------------------------------------------------------------------------
// Header file: "WitRun.h"
//
// Contains the declaration of class WitRun.
//------------------------------------------------------------------------------

#include <Problem.h>
#include <Session.h>

class WitInputID;

//------------------------------------------------------------------------------
// class WitRun
//
// Ultimately responsible for all of WIT.
// Non-delegated responsibility: the inner layer of WIT's API.
// Other API responsibilities are delegated to other classes.
// All non-API responsibilities are delegated to class Session.
//
// Note: For historical reasons, and due to the large number of
// member functions, the implementation of class WitRun is
// distributed throughout several source files of WIT.
// Currently, the files implementing class WitRun are:
//
//    WitRun.C
//    globApi.C
//    optApi.C
//    stochApi.C
//    partApi.C
//    demApi.C
//    opnApi.C
//    bomApi.C
//    subApi.C
//    bopApi.C
//    msgApi.C
//------------------------------------------------------------------------------

class WitRun
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitRun ();

      //------------------------------------------------------------------------
      // API functions implemented in WitRun.C.
      //------------------------------------------------------------------------

      void witAdvanceObjItr ();

      void witBuildPip ();

      void witClearPegging ();

      void witCopyData (
         WitRun * origWitRun);

      void witDeleteRun ();

      void witGetDevMode (WitBoolean * devMode);

      void witDisplayData (
         const char * fileName);

      void witEvalObjectives ();

      void witFinishExtOpt ();

      void witFinishHeurAlloc ();

      void witGeneratePriorities ();

      void witGetExpCycle (
         int *      lenLists,
         char * * * partNameList,
         char * * * operationNameList);

      void witGetExtOptIntVarIndices (int * * indices, int * len);

      void witGetExtOptLpProb (
         int *                numcols,
         int *                numrows,
         int * *              start,
         int * *              index,
         const WitOVRealArg & value,
         const WitOVRealArg & collb,
         const WitOVRealArg & colub,
         const WitOVRealArg & obj,
         const WitOVRealArg & rowlb,
         const WitOVRealArg & rowub);

      void witGetObjItrState (WitAttr * objItrState);

      void witHeurImplode ();

      void witInitialize ();

      void witIssueFatalMesg ();

      void witMrp ();

      void witNewRun ();

      void witOptImplode ();

      void witOptPreprocess ();

      void witPostprocess ();

      void witPreprocess ();

      void witPurgeData ();

      void witReadData (
         const char * fileName);

      void witResetObjItr ();

      void witSetExtOptSoln (const WitIVRealArg & colsol);

      void witShutDownExtOpt ();

      void witShutDownHeurAlloc ();

      void witStartExtOpt ();

      void witStartHeurAlloc ();

      void witWriteCriticalList (
         const char *  fileName,
         WitFileFormat fileFormat,
         int           maxListLen);

      void witWriteData (
         const char * fileName);

      void witWriteExecSched (
         const char *  fileName,
         WitFileFormat fileFormat);

      void witWriteProdSched (
         const char *  fileName,
         WitFileFormat fileFormat);

      void witWriteReqSched (
         const char *  fileName,
         WitFileFormat fileFormat);

      void witWriteShipSched (
         const char *  fileName,
         WitFileFormat fileFormat);

      void witNoDependantWitRunVeneersIsOne (
         int * isOne);

      void witNoDependantWitRunVeneersDecr  ();

      void witNoDependantWitRunVeneersIncr  ();

      void witExhaustMemory ();

      void witExecStandAlone (const char * theFileName);
         //
         // Performs main actions of the WIT stand-alone executable.
         // theFileName is the parameter file name, if any.

      void witInterpretDevCmd (const char * theDevCmd);
         //
         // Interprets the development command indicated by theDevCmd.
         // Severe error, if theDevCmd is not recognized.

      //------------------------------------------------------------------------
      // API functions implemented in globApi.C.
      //------------------------------------------------------------------------

      void witClearPipSeq ();

      void witGetAppData (
         void * * value);

      void witGetAutoPriority (
         WitBoolean * value);

      void witGetBuildWhenEmpty (
         WitBoolean * value);

      void witGetComputeCriticalList (
         WitBoolean * computeCriticalList);

      void witGetCriticalList (
         int *      lenCritList,
         char * * * partList,
         int * *    perList);

      void witGetEquitability (
         int * equitability);

      void witGetExecEmptyBom (
         WitBoolean * execEmptyBom);

      void witGetExpCutoff (
         const WitOSRealArg & expCutoff);

      void witGetFeasible (
         WitBoolean * feasible);

      void witGetForcedMultiEq (
         WitBoolean * value);

      void witGetHashTableSize (
         int * hashTableSize);

      void witGetHeurAllocActive (
         WitBoolean * heurAllocActive);

      void witGetHighPrecisionWD (
         WitBoolean * highPrecisionWD);

      void witGetIndependentOffsets (
         WitBoolean * independentOffsets);

      void witGetLocalBuildAhead (
         WitBoolean * localBuildAhead);

      void witGetLocalMultiExec (
         WitBoolean * localMultiExec);

      void witGetLotSizeTol (
         const WitOSRealArg & lotSizeTol);

      void witGetMinimalExcess (
         WitBoolean * minimalExcess);

      void witGetModHeurAlloc (
         WitBoolean * modHeurAlloc);

      void witGetMultiExec (
         WitBoolean * multiExec);

      void witGetMultiRoute (
         WitBoolean * value);

      void witGetNPeriods (
         int * nPeriods);

      void witGetNewPipPggOrder (
         WitBoolean * newPipPggOrder);

      void witGetNonMrSelSplit (
         WitBoolean * nonMrSelSplit);

      void witGetNstnResidual (
         WitBoolean * nstnResidual);

      void witGetOutputPrecision (
         int * outputPrecision);

      void witGetPenExec (
         WitBoolean * penExec);

      void witGetPerfPegging (
         WitBoolean * perfPegging);

      void witGetPgdCritList (
         int *      lenLists,
         char * * * critPartNameList,
         int * *    critPerList,
         char * * * demPartNameList,
         char * * * demandNameList,
         int * *    shipPerList);

      void witGetPgdCritListMode (
         WitBoolean * pgdCritListMode);

      void witGetPipExists (
         WitBoolean * pipExists);

      void witGetPipSeq (
         int *                lenLists,
         char * * *           partNameList,
         char * * *           demandNameList,
         int * *              shipPerList,
         const WitOVRealArg & incShipVolList);

      void witGetPipSeqFromHeur (
         WitBoolean * pipSeqFromHeur);

      void witGetPostprocessed (
         WitBoolean * postprocessed);

      void witGetPrefHighStockSLBs (
         WitBoolean * prefHighStockSLBs);

      void witGetPreprocessed (
         WitBoolean * preprocessed);

      void witGetRespectStockSLBs (
         WitBoolean * respectStockSLBs);

      void witGetRoundReqVols (
         WitBoolean * roundReqVols);

      void witGetSelSplit (
         WitBoolean * selSplit);

      void witGetSelectionRecovery (
         WitBoolean * value);

      void witGetSkipFailures (
         WitBoolean * value);

      void witGetStockRealloc (
         WitBoolean * stockRealloc);

      void witGetStockReallocation (
         WitBoolean * stockReallocation);

      void witGetSrSelSplit (
         WitBoolean * srSelSplit);

      void witGetTieBreakPropRt (
         WitBoolean * tieBreakPropRt);

      void witGetTitle (
         char * * title);

      void witGetTruncOffsets (
         WitBoolean * value);

      void witGetTwoWayMultiExec (
         WitBoolean * twoWayMultiExec);

      void witGetUseFocusHorizons (
         WitBoolean * useFocusHorizons);

      void witGetUserHeurStart (
         WitBoolean * userHeurStart);

      void witGetWit34Compatible (
         WitBoolean * value);

      void witSetAppData (
         void * appData);

      void witSetAutoPriority (
         WitBoolean autoPriority);

      void witSetBuildWhenEmpty (
         WitBoolean buildWhenEmpty);

      void witSetComputeCriticalList (
         WitBoolean computeCriticalList);

      void witSetEquitability (
         int equitability);

      void witSetExecEmptyBom (
         WitBoolean execEmptyBom);

      void witSetExpCutoff (
         const WitISRealArg & expCutoff);

      void witSetForcedMultiEq (
         WitBoolean forcedMultiEq);

      void witSetHashTableSize (
         int hashTableSize);

      void witSetHighPrecisionWD (
         WitBoolean highPrecisionWD);

      void witSetIndependentOffsets (
         WitBoolean independentOffsets);

      void witSetLocalBuildAhead (
         WitBoolean localBuildAhead);

      void witSetLocalMultiExec (
         WitBoolean localMultiExec);

      void witSetLotSizeTol (
         const WitISRealArg & lotSizeTol);

      void witSetMinimalExcess (
         WitBoolean minimalExcess);

      void witSetModHeurAlloc (
         WitBoolean modHeurAlloc);

      void witSetMultiExec (
         WitBoolean multiExec);

      void witSetMultiRoute (
         WitBoolean multiRoute);

      void witSetNPeriods (
         int nPeriods);

      void witSetNewPipPggOrder (
         WitBoolean newPipPggOrder);

      void witSetNonMrSelSplit (
         WitBoolean nonMrSelSplit);

      void witSetNstnResidual (
         WitBoolean nstnResidual);

      void witSetOutputPrecision (
         int outputPrecision);

      void witSetPenExec (
         WitBoolean penExec);

      void witSetPerfPegging (
         WitBoolean perfPegging);

      void witSetPgdCritListMode (
         WitBoolean pgdCritListMode);

      void witSetPipSeqFromHeur (
         WitBoolean pipSeqFromHeur);

      void witSetPrefHighStockSLBs (
         WitBoolean prefHighStockSLBs);

      void witSetRespectStockSLBs (
         WitBoolean respectStockSLBs);

      void witSetRoundReqVols (
         WitBoolean roundReqVols);

      void witSetSelSplit (
         WitBoolean selSplit);

      void witSetSelectionRecovery (
         WitBoolean selectionRecovery);

      void witSetSkipFailures (
         WitBoolean skipFailures);

      void witSetStockRealloc (
         WitBoolean stockRealloc);

      void witSetStockReallocation (
         WitBoolean stockReallocation);

      void witSetSrSelSplit (
         WitBoolean srSelSplit);

      void witSetTieBreakPropRt (
         WitBoolean tieBreakPropRt);

      void witSetTitle (
         const char * title);

      void witSetTruncOffsets (
         WitBoolean truncOffsets);

      void witSetTwoWayMultiExec (
         WitBoolean twoWayMultiExec);

      void witSetUseFocusHorizons (
         WitBoolean useFocusHorizons);

      void witSetUserHeurStart (
         WitBoolean userHeurStart);

      void witSetWit34Compatible (
         WitBoolean boolval);

      void witSetWit34Allowed (
         WitBoolean boolval);

      //------------------------------------------------------------------------
      // API functions implemented in optApi.C.
      //------------------------------------------------------------------------

      void witAddDblCplexParSpec (
         const char *         theName,
         const WitISRealArg & theValue);

      void witAddIntCplexParSpec (
         const char * theName,
         int          theValue);

      void witClearCplexParSpecs ();

      void witGetAccAfterOptImp (
         WitBoolean * value);

      void witGetAccAfterSoftLB (
         WitBoolean * value);

      void witGetAccelerated (
         WitBoolean * value);

      void witGetBoundsValue (
         const WitOSRealArg & boundsValue);

      void witGetCoinEmbedded (WitBoolean * coinEmbedded);

      void witGetCompPrices (
         WitBoolean * value);

      void witGetCplexEmbedded (WitBoolean * cplexEmbedded);

      void witGetCplexMipBound (
         const WitOSRealArg & cplexMipBound);

      void witGetCplexMipRelGap (
         const WitOSRealArg & cplexMipRelGap);

      void witGetCplexParSpecDblVal (
         const WitOSRealArg & cplexParSpecDblVal);

      void witGetCplexParSpecIntVal (
         int * cplexParSpecIntVal);

      void witGetCplexParSpecName (
         char * * cplexParSpecName);

      void witGetCplexStatusCode (
         int * cplexStatusCode);

      void witGetCplexStatusText (
         char * * cplexStatusText);

      void witGetCurrentObjective (
         char * * theObjName);

      void witGetDblCplexParSpec (
         const char *         theName,
         WitBoolean *         dblSpecExists,
         const WitOSRealArg & theValue);

      void witGetExtOptActive (
         WitBoolean * extOptActive);

      void witGetIntCplexParSpec (
         const char * theName,
         WitBoolean * intSpecExists,
         int *        theValue);

      void witGetMipMode (
         WitBoolean * value);

      void witGetMultiObjMode (
         WitBoolean * value);

      void witGetMultiObjTol (
         const WitOSRealArg & multiObjTol);

      void witGetObjChoice (
         int * objChoice);

      void witGetObjValue (
         const WitOSRealArg & objValue);

      void witGetObjValues (
         const WitOSRealArg & objValue,
         const WitOSRealArg & boundsValue);

      void witGetObjectiveList (
         int  *     lenList,
         char * * * objectiveList);

      void witGetObjectiveListSpec (
         char * * objectiveListSpec);

      void witGetObjectiveSeqNo (
         int * objectiveSeqNo);

      void witGetOptInitMethod (
         WitAttr * optInitMethod);

      void witGetSolverLogFileName (
         char * * solverLogFileName);

      void witGetWbounds (
         const WitOSRealArg & wbounds);

      void witSetAccAfterOptImp (
         WitBoolean accAfterOptImp);

      void witSetAccAfterSoftLB (
         WitBoolean accAfterSoftLB);

      void witSetCompPrices (
         WitBoolean compPrices);

      void witSetCplexParSpecDblVal (
         const WitISRealArg & cplexParSpecDblVal);

      void witSetCplexParSpecIntVal (
         int cplexParSpecIntVal);

      void witSetCplexParSpecName (
         const char * cplexParSpecName);

      void witSetCurrentObjective (
         const char * theObjName);

      void witSetMipMode (
         WitBoolean mipMode);

      void witSetMultiObjMode (
         WitBoolean multiObjMode);

      void witSetMultiObjTol (
         const WitISRealArg & multiObjTol);

      void witSetObjChoice (
         int objChoice);

      void witSetObjectiveList (
         int                  lenList,
         const char * const * objectiveList);

      void witSetObjectiveListSpec (
         const char * objectiveListSpec);

      void witSetObjectiveSeqNo (
         int objectiveSeqNo);

      void witSetOptInitMethod (
         WitAttr optInitMethod);

      void witSetSolverLogFileName (
         const char * solverLogFileName);

      void witSetWbounds (
         const WitISRealArg & wbounds);

      //------------------------------------------------------------------------
      // API functions implemented in stochApi.C.
      //------------------------------------------------------------------------

      void witClearStochSoln ();

      void witGetCurrentScenario (
         int * currentScenario);

      void witGetNScenarios (
         int * nScanerios);

      void witGetPeriodStage (
         int * * periodStage);

      void witGetProbability (
         const WitOSRealArg & probability);

      void witGetStageByObject (
         WitBoolean * stageByObject);

      void witGetStochMode (
         WitBoolean * stochMode);

      void witGetStochSolnMode (
         WitBoolean * stochSolnMode);

      void witSetCurrentScenario (
         int currentScenario);

      void witSetNScenarios (
         int nScenarios);

      void witSetPeriodStage (
         const int * periodStage);

      void witSetProbability (
         const WitISRealArg & probability);

      void witSetStageByObject (
         WitBoolean stageByObject);

      void witSetStochMode (
         WitBoolean stochMode);

      void witStochImplode ();

      //------------------------------------------------------------------------
      // API functions implemented in partApi.C.
      //------------------------------------------------------------------------

      WitPart * setGetPartInit (
         const char * partName);

      WitMaterial * setGetMaterialInit (
         const char * partName,
         const char * theAttName);

      void witAddPart (
         const char * partName, WitAttr partCategory);

      void witAddPartWithOperation (
         const char * partAndOperationName);

      void witCopyPartData (
         const char * dupPartName,
         WitRun *     origWitRun,
         const char * origPartName);

      void witGetFocusShortageVol (
         int   *                lenList,
         char  * * *            partList,
         const WitOTDARealArg & shortageVolList);

      void witGetObjItrPart (
         char * * partName);

      void witGetPartAppData (
         const char * partName,
         void * *     appData);

      void witGetPartAsapPipOrder (
         const char * partName,
         WitBoolean * asapPipOrder);

      void witGetPartBaSelSplit (
         const char * partName,
         WitBoolean * baSelSplit);

      void witGetPartBelowList (
         const char * partName,
         int *        lenList,
         char * * *   partNameList);

      void witGetPartBoundedLeadTimes (
         const char * partName,
         WitBoolean * boundedLeadTimes);

      void witGetPartBuildAheadUB (
         const char * partName,
         int * *      buildAheadUB);

      void witGetPartBuildAheadLimit (
         const char * partName,
         int *        buildAheadLimit);

      void witGetPartBuildAsap (
         const char * partName,
         WitBoolean * buildAsap);

      void witGetPartBuildNstn (
         const char * partName,
         WitBoolean * buildNstn);

      void witGetPartCategory (
         const char * partName,
         WitAttr *    category);

      void witGetPartConsVol (
         const char *         partName,
         const WitOVRealArg & consVol);

      void witGetPartConsumingBomEntry (
         const char * partName,
         int          consIndex,
         char * *     consumingOperationName,
         int *        bomEntryIndex);

      void witGetPartConsumingSubsBomEntry (
         const char * partName,
         int          consIndex,
         char * *     consumingOperationName,
         int *        bomEntryIndex,
         int *        subsBomEntryIndex);

      void witGetPartCycleTime (
         const char *         partName,
         const WitOVRealArg & cycleTime);

      void witGetPartDemands (
         const char * partName,
         int *        lenDemandList,
         char * * *   demandList);

      void witGetPartExcessVol (
         const char *         partName,
         const WitOVRealArg & excessVol);

      void witGetPartExists (
         const char * partNameParm,
         WitBoolean * exists);

      void witGetPartFocusShortageVol (
         const char *         partName,
         const WitOVRealArg & shortageVol);

      void witGetPartIncLotSize (
         const char *         partName,
         const WitOVRealArg & incLotSize);

      void witGetPartMinLotSize (
         const char *         partName,
         const WitOVRealArg & minLotSize);

      void witGetPartMrpConsVol (
         const char *         partName,
         const WitOVRealArg & mrpConsVol);

      void witGetPartMrpExcessVol (
         const char *         partName,
         const WitOVRealArg & mrpExcessVol);

      void witGetPartMrpProdVol (
         const char *         partName,
         const WitOVRealArg & mrpProdVol);

      void witGetPartMrpResidualVol (
         const char *         partName,
         const WitOVRealArg & mrpResidualVol);

      void witGetPartNBomEntries (
         const char * partName,
         int *        nBomEntries);

      void witGetPartNConsumingBomEntries (
         const char * partName,
         int *        nConsumingBomEntries);

      void witGetPartNConsumingSubsBomEntries (
         const char * partName,
         int *        nConsumingSubsBomEntries);

      void witGetPartNProducingBopEntries (
         const char * partName,
         int *        nProducingBopEntries);

      void witGetPartObj1ProdCost (
         const char *         partName,
         const WitOVRealArg & obj1ProdCost);

      void witGetPartObj1ScrapCost (
         const char *         partName,
         const WitOVRealArg & obj1ScrapCost);

      void witGetPartObj1StockCost (
         const char *         partName,
         const WitOVRealArg & obj1StockCost);

      void witGetPartObjectStage (
         const char * partName,
         int *        objectStage);

      void witGetPartProdBounds (
         const char *         partName,
         const WitOVRealArg & hardLower,
         const WitOVRealArg & softLower,
         const WitOVRealArg & hardUpper);

      void witGetPartProdVol (
         const char *         partName,
         const WitOVRealArg & prodVol);

      void witGetPartProducingBopEntry (
         const char * partName,
         int          prodIndex,
         char * *     producingOperationName,
         int *        bopEntryIndex);

      void witGetPartPropRouting (
         const char * partName,
         WitBoolean * propRouting);

      void witGetPartPropRtg (
         const char *   partName,
         WitBoolean * * propRtg);

      void witGetPartReqVol (
         const char *         partName,
         const WitOVRealArg & reqVol);

      void witGetPartResidualVol (
         const char *         partName,
         const WitOVRealArg & residualVol);

      void witGetPartResourceConIndex (
         const char * partName,
         const int    thePer,
         int *        resourceConIndex);

      void witGetPartScrapVarIndex (
         const char * partName,
         const int    thePer,
         int *        scrapVarIndex);

      void witGetParts (
         int *      lenPartList,
         char * * * partList);

      void witGetPartScrapAllowed (
         const char * partName,
         WitBoolean * scrapAllowed);

      void witGetPartScrapCost (
         const char *         partName,
         const WitOVRealArg & scrapCost);

      void witGetPartScrapVol  (
         const char *         partName,
         const WitOVRealArg & scrapVol);

      void witGetPartSelForDel (
         const char * partName,
         WitBoolean * selForDel);

      void witGetPartShadowPrice  (
         const char *         partName,
         const WitOVRealArg & shadowPrice);

      void witGetPartSingleSource (
         const char * partName,
         WitBoolean * singleSource);

      void witGetPartStockBounds (
         const char *         partName,
         const WitOVRealArg & hardLower,
         const WitOVRealArg & softLower,
         const WitOVRealArg & hardUpper);

      void witGetPartStockCost (
         const char *         partName,
         const WitOVRealArg & stockCost);

      void witGetPartStockSlbConIndex (
         const char * partName,
         const int    thePer,
         int *        stockSlbConIndex);

      void witGetPartStockSlbvVarIndex (
         const char * partName,
         const int    thePer,
         int *        stockSlbvVarIndex);

      void witGetPartStockVarIndex (
         const char * partName,
         const int    thePer,
         int *        stockVarIndex);

      void witGetPartStockVol  (
         const char *         partName,
         const WitOVRealArg & stockVol);

      void witGetPartSupplyVol (
         const char *         partName,
         const WitOVRealArg & supplyVol);

      void witGetPartYield (
         const char * partName,
         int * *      yield);

      void witSetPartAppData (
         const char * partName,
         void *       appData);

      void witSetPartAsapPipOrder (
         const char * partName,
         WitBoolean   asapPipOrder);

      void witSetPartBaSelSplit (
         const char * partName,
         WitBoolean   baSelSplit);

      void witSetPartBoundedLeadTimes (
         const char * partName,
         WitBoolean   boundedLeadTimes);

      void witSetPartBuildAheadUB (
         const char * partName,
         const int *  buildAheadUB);

      void witSetPartBuildAheadLimit (
         const char * partName,
         int          buildAheadLimit);

      void witSetPartBuildAsap (
         const char * partName,
         WitBoolean   buildAsap);

      void witSetPartBuildNstn (
         const char * partName,
         WitBoolean   buildNstn);

      void witSetPartCycleTime (
         const char *         partName,
         const WitIVRealArg & cycleTime);

      void witSetPartIncLotSize (
         const char *         partName,
         const WitIVRealArg & incLotSize);

      void witSetPartMinLotSize (
         const char *         partName,
         const WitIVRealArg & minLotSize);

      void witSetPartObj1ProdCost (
         const char *         partName,
         const WitIVRealArg & obj1ProdCost);

      void witSetPartObj1ScrapCost (
         const char *         partName,
         const WitIVRealArg & obj1ScrapCost);

      void witSetPartObj1StockCost (
         const char *         partName,
         const WitIVRealArg & obj1StockCost);

      void witSetPartObjectStage (
         const char * partName,
         int          objectStage);

      void witSetPartPartName (
         const char * partName,
         const char * newName);

      void witSetPartProdBounds (
         const char *         partName,
         const WitIVRealArg & hardLower,
         const WitIVRealArg & softLower,
         const WitIVRealArg & hardUpper);

      void witSetPartProdVol (
         const char *         partName,
         const WitIVRealArg & prodVol);

      void witSetPartPropRouting (
         const char * partName,
         WitBoolean   propRouting);

      void witSetPartPropRtg (
         const char *       partName,
         const WitBoolean * propRtg);

      void witSetPartScrapAllowed (
         const char * partName,
         WitBoolean   scrapAllowed);

      void witSetPartScrapCost (
         const char *         partName,
         const WitIVRealArg & scrapCost);

      void witSetPartSelForDel (
         const char * partName,
         WitBoolean   selForDel);

      void witSetPartSingleSource (
         const char * partName,
         WitBoolean   singleSource);

      void witSetPartStockBounds (
         const char *         partName,
         const WitIVRealArg & hardLower,
         const WitIVRealArg & softLower,
         const WitIVRealArg & hardUpper);

      void witSetPartStockCost (
         const char *         partName,
         const WitIVRealArg & stockCost);

      void witSetPartSupplyVol (
         const char *         partName,
         const WitIVRealArg & supplyVol);

      void witSetPartYield (
         const char * partName,
         const int *  yield);

      //------------------------------------------------------------------------
      // API functions implemented in demApi.C.
      //------------------------------------------------------------------------

      WitDemand * setGetDemandInit (
         const char * demandedPartName,
         const char * demandName);

      void witAddDemand (
         const char * partName,
         const char * demandName);

      void witAppendToPipSeq (
         const char *         partName,
         const char *         demandName,
         int                  shipPeriod,
         const WitISRealArg & incShipVol);

      void witCopyDemandData (
         const char * dupPartName,
         const char * dupDemandName,
         WitRun *     origWitRun,
         const char * origPartName,
         const char * origDemandName);

      void witEqHeurAlloc (
         int                  lenLists,
         const char * const * demandedPartNameList,
         const char * const * demandNameList,
         const int *          shipPeriodList,
         const WitIVRealArg & desIncVolList,
         const WitOVRealArg & incVolList);

      void witEqHeurAllocTwme (
         int                  lenLists,
         const char * const * demandedPartNameList,
         const char * const * demandNameList,
         const int *          shipPeriodList,
         const WitIVRealArg & desIncVolList,
         const WitOVRealArg & incVolList,
         const WitBoolean *   asapMultiExecList);

      void witGetObjItrDemand (
         char * * partName,
         char * * demandName);

      void witGetDemandAppData (
         const char * partName,
         const char * demandName,
         void * *     appData);

      void witGetDemandBuildAheadUB (
         const char * partName,
         const char * demandName,
         int * *      buildAheadUB);

      void witGetDemandBuildAheadLimit (
         const char * partName,
         const char * demandName,
         int *        buildAheadLimit);

      void witGetDemandCoExecVolPip (
         const char *         partName,
         const char *         demandName,
         int                  shipPeriod,
         int *                lenLists,
         char * * *           operationNameList,
         int * *              bopEntryIndexList,
         int * *              execPeriodList,
         const WitOVRealArg & peggedVolList);

      void witGetDemandConsVolPip (
         const char *         partName,
         const char *         demandName,
         int                  shipPeriod,
         int *                lenLists,
         char * * *           partNameList,
         int * *              periodList,
         const WitOVRealArg & peggedVolList);

      void witGetDemandCumShipBounds (
         const char *         partNameParm,
         const char *         demandNameParm,
         const WitOVRealArg & hardlbP,
         const WitOVRealArg & softlbP,
         const WitOVRealArg & hardubP);

      void witGetDemandCumShipReward (
         const char *         partNameParm,
         const char *         demandNameParm,
         const WitOVRealArg & cumShipReward);

      void witGetDemandCumShipSlbConIndex (
         const char * partName,
         const char * demandName,
         const int    thePer,
         int *        cumShipSlbConIndex);

      void witGetDemandCumShipSlbvVarIndex (
         const char * partName,
         const char * demandName,
         const int    thePer,
         int *        cumShipSlbvVarIndex);

      void witGetDemandCumShipVarIndex (
         const char * partName,
         const char * demandName,
         const int    thePer,
         int *        cumShipVarIndex);

      void witGetDemandDemandVol (
         const char *         partName,
         const char *         demandName,
         const WitOVRealArg & demandVol);

      void witGetDemandExecVolPegging (
         const char *         partName,
         const char *         demandName,
         int                  shipPeriod,
         int *                lenLists,
         char * * *           operationNameList,
         int * *              execPeriodList,
         const WitOVRealArg & peggedExecVolList);

      void witGetDemandExecVolPip (
         const char *         partName,
         const char *         demandName,
         int                  shipPeriod,
         int *                lenLists,
         char * * *           operationNameList,
         int * *              execPeriodList,
         const WitOVRealArg & peggedVolList);

      void witGetDemandExists (
         const char * partName,
         const char * demandName,
         WitBoolean * exists);

      void witGetDemandFocusHorizon (
         const char * partNameParm,
         const char * demandNameParm,
         int *        i);

      void witGetDemandFssShipVol (
         const char *         partName,
         const char *         demandName,
         const WitOVRealArg & fssShipVol);

      void witGetDemandIntShipVols (
         const char * partName,
         const char * demandName,
         WitBoolean * intShipVols);

      void witGetDemandLeadTimeUB (
         const char * partName,
         const char * demandName,
         int * *      leadTimeUB);

      void witGetDemandObj1CumShipReward (
         const char *         partNameParm,
         const char *         demandNameParm,
         const WitOVRealArg & obj1CumShipReward);

      void witGetDemandObj1ShipReward (
         const char *         partNameParm,
         const char *         demandNameParm,
         const WitOVRealArg & obj1ShipReward);

      void witGetDemandPrefBuildAhead (
         const char * partName,
         const char * demandName,
         WitBoolean * prefBuildAhead);

      void witGetDemandPriority (
         const char * partName,
         const char * demandName,
         int * *      priority);

      void witGetDemandProdVolPip (
         const char *         partName,
         const char *         demandName,
         int                  shipPeriod,
         int *                lenLists,
         char * * *           partNameList,
         int * *              periodList,
         const WitOVRealArg & peggedVolList);

      void witGetDemandSearchInc (
         const char *         partName,
         const char *         demandName,
         const WitOSRealArg & searchInc);

      void witGetDemandSelForDel (
         const char * partName,
         const char * demandName,
         WitBoolean * selForDel);

      void witGetDemandShipConIndex (
         const char * partName,
         const char * demandName,
         const int    thePer,
         int *        shipConIndex);

      void witGetDemandShipLateAllowed (
         const char * partName,
         const char * demandName,
         WitBoolean * shipLateAllowed);

      void witGetDemandShipLateLimit (
         const char * partName,
         const char * demandName,
         int *        shipLateLimit);

      void witGetDemandShipLateUB (
         const char * partName,
         const char * demandName,
         int * *      shipLateUB);

      void witGetDemandShipReward (
         const char *         partNameParm,
         const char *         demandNameParm,
         const WitOVRealArg & shipReward);

      void witGetDemandShipVarIndex (
         const char * partName,
         const char * demandName,
         const int    thePer,
         int *        shipVarIndex);

      void witGetDemandShipVol  (
         const char *         partName,
         const char *         demandName,
         const WitOVRealArg & shipVol);

      void witGetDemandSideVolPip (
         const char *         partName,
         const char *         demandName,
         int                  shipPeriod,
         int *                lenLists,
         char * * *           partNameList,
         int * *              periodList,
         const WitOVRealArg & peggedVolList);

      void witGetDemandSubVolPegging (
         const char *         partName,
         const char *         demandName,
         int                  shipPeriod,
         int *                lenLists,
         char * * *           operationNameList,
         int * *              bomEntryIndexList,
         int * *              subsBomEntryIndexList,
         int * *              execPeriodList,
         const WitOVRealArg & peggedSubVolList);

      void witGetDemandSubVolPip (
         const char *         partName,
         const char *         demandName,
         int                  shipPeriod,
         int *                lenLists,
         char * * *           operationNameList,
         int * *              bomEntryIndexList,
         int * *              subsBomEntryIndexList,
         int * *              execPeriodList,
         const WitOVRealArg & peggedVolList);

      void witGetDemandSupplyVolPip (
         const char *         partName,
         const char *         demandName,
         int                  shipPeriod,
         int *                lenLists,
         char * * *           partNameList,
         int * *              periodList,
         const WitOVRealArg & peggedVolList);

      void witIncHeurAlloc (
         const char *         partName,
         const char *         demandName,
         int                  shipPeriod,
         const WitISRealArg & desIncVol,
         const WitOSRealArg & incVol);

      void witIncHeurAllocTwme (
         const char *         partName,
         const char *         demandName,
         int                  shipPeriod,
         const WitISRealArg & desIncVol,
         const WitOSRealArg & incVol,
         WitBoolean           asapMultiExec);

      void witSetDemandAppData (
         const char * partName,
         const char * demandName,
         void *       appData);

      void witSetDemandBuildAheadUB (
         const char * partName,
         const char * demandName,
         const int *  buildAheadUB);

      void witSetDemandBuildAheadLimit (
         const char * partName,
         const char * demandName,
         int          buildAheadLimit);

      void witSetDemandCumShipBounds (
         const char *         partName,
         const char *         demandName,
         const WitIVRealArg & hardLower,
         const WitIVRealArg & softLower,
         const WitIVRealArg & hardUpper);

      void witSetDemandCumShipReward (
         const char *         partName,
         const char *         demandName,
         const WitIVRealArg & cumShipReward);

      void witSetDemandDemandName (
         const char * partName,
         const char * demandName,
         const char * newName);

      void witSetDemandDemandVol (
         const char *         partName,
         const char *         demandName,
         const WitIVRealArg & demandVol);

      void witSetDemandFocusHorizon (
         const char * partNameParm,
         const char * demandNameParm,
         int          i);

      void witSetDemandFssShipVol (
         const char *         partName,
         const char *         demandName,
         const WitIVRealArg & fssShipVol);

      void witSetDemandIntShipVols (
         const char * partName,
         const char * demandName,
         WitBoolean   intShipVols);

      void witSetDemandLeadTimeUB (
         const char * partName,
         const char * demandName,
         const int *  leadTimeUB);

      void witSetDemandObj1CumShipReward (
         const char *         partName,
         const char *         demandName,
         const WitIVRealArg & obj1CumShipReward);

      void witSetDemandObj1ShipReward (
         const char *         partName,
         const char *         demandName,
         const WitIVRealArg & obj1ShipReward);

      void witSetDemandPrefBuildAhead (
         const char * partName,
         const char * demandName,
         WitBoolean   prefBuildAhead);

      void witSetDemandPriority (
         const char * partName,
         const char * demandName,
         const int *  priority);

      void witSetDemandSearchInc (
         const char *         partName,
         const char *         demandName,
         const WitISRealArg & searchInc);

      void witSetDemandSelForDel (
         const char * partName,
         const char * demandName,
         WitBoolean   selForDel);

      void witSetDemandShipLateAllowed (
         const char * partName,
         const char * demandName,
         WitBoolean   shipLateAllowed);

      void witSetDemandShipLateLimit (
         const char * partName,
         const char * demandName,
         int          shipLateLimit);

      void witSetDemandShipLateUB (
         const char * partName,
         const char * demandName,
         const int *  shipLateUB);

      void witSetDemandShipReward (
         const char *         partName,
         const char *         demandName,
         const WitIVRealArg & shipReward);

      void witSetDemandShipVol (
         const char *         partName,
         const char *         demandName,
         const WitIVRealArg & shipVol);

      //------------------------------------------------------------------------
      // API functions implemented in opnApi.C.
      //------------------------------------------------------------------------

      WitOperation * setGetOperationInit (
         const char * operationName);

      void witAddOperation (
         const char * operationName);

      void witCopyOperationData (
         const char * dupOperationName,
         WitRun *     origWitRun,
         const char * origOperationName);

      void witGetObjItrOperation (
         char * * operationName);

      void witGetOperationAppData (
         const char * operationName,
         void * *     appData);

      void witGetOperationCoExecVolPip (
         const char *         operationName,
         int                  execPeriod,
         int *                lenLists,
         char * * *           operationNameList,
         int * *              bopEntryIndexList,
         int * *              execPeriodList,
         const WitOVRealArg & peggedVolList);

      void witGetOperationConsVolPip (
         const char *         operationName,
         int                  execPeriod,
         int *                lenLists,
         char * * *           partNameList,
         int * *              periodList,
         const WitOVRealArg & peggedVolList);

      void witGetOperationCycleTime34 (
         const char *         operationName,
         const WitOVRealArg & cycleTime34);

      void witGetOperationExecBounds (
         const char *         operationName,
         const WitOVRealArg & hardLower,
         const WitOVRealArg & softLower,
         const WitOVRealArg & hardUpper);

      void witGetOperationExecCost (
         const char *         operationName,
         const WitOVRealArg & execCost);

      void witGetOperationExecPenalty (
         const char *         operationName,
         const WitOSRealArg & execPenalty);

      void witGetOperationExecSlbConIndex (
         const char * operationName,
         const int    thePer,
         int *        execSlbConIndex);

      void witGetOperationExecSlbvVarIndex (
         const char * operationName,
         const int    thePer,
         int *        execSlbvVarIndex);

      void witGetOperationExecVarIndex (
         const char * operationName,
         const int    thePer,
         int *        execVarIndex);

      void witGetOperationExecVol (
         const char *         operationName,
         const WitOVRealArg & execVol);

      void witGetOperationExecVolPip (
         const char *         operationName,
         int                  execPeriod,
         int *                lenLists,
         char * * *           operationNameList,
         int * *              execPeriodList,
         const WitOVRealArg & peggedVolList);

      void witGetOperationExecutable (
         const char *   operationName,
         WitBoolean * * executable);

      void witGetOperationExists (
         const char * operationName,
         WitBoolean * exists);

      void witGetOperationFssExecVol (
         const char *         operationName,
         const WitOVRealArg & fssExecVol);

      void witGetOperationIncLotSize (
         const char *         operationName,
         const WitOVRealArg & incLotSize);

      void witGetOperationIncLotSize2 (
         const char *         operationName,
         const WitOVRealArg & incLotSize2);

      void witGetOperationIntExecVols (
         const char * operationName,
         WitBoolean * intExecVols);

      void witGetOperationLotSize2Thresh (
         const char *         operationName,
         const WitOVRealArg & lotSize2Thresh);

      void witGetOperationMinLotSize (
         const char *         operationName,
         const WitOVRealArg & minLotSize);

      void witGetOperationMinLotSize2 (
         const char *         operationName,
         const WitOVRealArg & minLotSize2);

      void witGetOperationMrpExecVol (
         const char *         operationName,
         const WitOVRealArg & mrpExecVol);

      void witGetOperationNBomEntries (
         const char * operationName,
         int *        nBomEntries);

      void witGetOperationNBopEntries (
         const char * operationName,
         int *        nBopEntries);

      void witGetOperationObj1ExecCost (
         const char *         operationName,
         const WitOVRealArg & obj1ExecCost);

      void witGetOperationObjectStage (
         const char * operationName,
         int *        objectStage);

      void witGetOperationPipEnabled (
         const char * operationName,
         WitBoolean * pipEnabled);

      void witGetOperationPipRank (
         const char * operationName,
         int *        pipRank);

      void witGetOperationProdVolPip (
         const char *         operationName,
         int                  execPeriod,
         int *                lenLists,
         char * * *           partNameList,
         int * *              periodList,
         const WitOVRealArg & peggedVolList);

      void witGetOperations (
         int *      lenOperationList,
         char * * * operationList);

      void witGetOperationSelForDel (
         const char * operationName,
         WitBoolean * selForDel);

      void witGetOperationSideVolPip (
         const char *         operationName,
         int                  execPeriod,
         int *                lenLists,
         char * * *           partNameList,
         int * *              periodList,
         const WitOVRealArg & peggedVolList);

      void witGetOperationSubVolPip (
         const char *         operationName,
         int                  execPeriod,
         int *                lenLists,
         char * * *           operationNameList,
         int * *              bomEntryIndexList,
         int * *              subsBomEntryIndexList,
         int * *              execPeriodList,
         const WitOVRealArg & peggedVolList);

      void witGetOperationSupplyVolPip (
         const char *         operationName,
         int                  execPeriod,
         int *                lenLists,
         char * * *           partNameList,
         int * *              periodList,
         const WitOVRealArg & peggedVolList);

      void witGetOperationTwoLevelLotSizes (
         const char * operationName,
         WitBoolean * twoLevelLotSizes);

      void witGetOperationYield (
         const char * operationName,
         int * *      yield);

      void witGetOperationYieldRate (
         const char *         operationName,
         const WitOVRealArg & yield);

      void witSetOperationAppData (
         const char * operationName,
         void *       appData);

      void witSetOperationCycleTime34 (
         const char *         operationName,
         const WitIVRealArg & cycleTime34);

      void witSetOperationExecBounds (
         const char *         operationName,
         const WitIVRealArg & hardLower,
         const WitIVRealArg & softLower,
         const WitIVRealArg & hardUpper);

      void witSetOperationExecCost (
         const char *         operationName,
         const WitIVRealArg & execCost);

      void witSetOperationExecPenalty (
         const char *         operationName,
         const WitISRealArg & execPenalty);

      void witSetOperationExecVol (
         const char *         operationName,
         const WitIVRealArg & execVol);

      void witSetOperationIncLotSize (
         const char *         operationName,
         const WitIVRealArg & incLotSize);

      void witSetOperationIncLotSize2 (
         const char *         operationName,
         const WitIVRealArg & incLotSize2);

      void witSetOperationIntExecVols (
         const char * operationName,
         WitBoolean   intExecVols);

      void witSetOperationLotSize2Thresh (
         const char *         operationName,
         const WitIVRealArg & lotSize2Thresh);

      void witSetOperationMinLotSize (
         const char *         operationName,
         const WitIVRealArg & minLotSize);

      void witSetOperationMinLotSize2 (
         const char *         operationName,
         const WitIVRealArg & minLotSize2);

      void witSetOperationObj1ExecCost (
         const char *         operationName,
         const WitIVRealArg & obj1ExecCost);

      void witSetOperationObjectStage (
         const char * operationName,
         int          objectStage);

      void witSetOperationOperationName (
         const char * operationName,
         const char * newName);

      void witSetOperationPipEnabled (
         const char * operationName,
         WitBoolean   pipEnabled);

      void witSetOperationPipRank (
         const char * operationName,
         int          pipRank);

      void witSetOperationSelForDel (
         const char * operationName,
         WitBoolean   selForDel);

      void witSetOperationTwoLevelLotSizes (
         const char * operationName,
         WitBoolean   twoLevelLotSizes);

      void witSetOperationYield (
         const char * operationName,
         const int *  yield);

      void witSetOperationYieldRate (
         const char *         operationName,
         const WitIVRealArg & yieldRate);

      //------------------------------------------------------------------------
      // API functions implemented in bomApi.C.
      //------------------------------------------------------------------------

      WitBomEntry * setGetBomEntryInit (
         const char *    consumingOperationName,
         int             bomEntryIndex);

      void witAddBomEntry (
         const char * consumingOperationName,
         const char * consumedPartName);

      void witCopyBomEntryData (
         const char * dupOperationName,
         int          dupBomEntryIndex,
         WitRun *     origWitRun,
         const char * origOperationName,
         int          origBomEntryIndex);

      void witGetObjItrBomEntry (
         char * * consumingOperationName,
         int *    bomEntryIndex);

      void witGetBomEntryAppData (
         const char * consumingOperationName,
         int          bomEntryIndex,
         void * *     appData);

      void witGetBomEntryConsRate (
         const char *         partName,
         int                  bomEntryIndex,
         const WitOVRealArg & consRate);

      void witGetBomEntryConsumedPart (
         const char * consumingOperationName,
         int          bomEntryIndex,
         char * *     consumedPartName);

      void witGetBomEntryEarliestPeriod (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int *        earliestPeriod);

      void witGetBomEntryExecPenalty (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         const WitOSRealArg & execPenalty);

      void witGetBomEntryFallout (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int *        fallout);

      void witGetBomEntryFalloutRate (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         const WitOSRealArg & falloutRate);

      void witGetBomEntryImpactPeriod (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int * *      impactPeriod);

      void witGetBomEntryLatestPeriod (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int *        latestPeriod);

      void witGetBomEntryMandEC (
         const char * consumingOperationName,
         int          bomEntryIndex,
         WitBoolean * mandEC);

      void witGetBomEntryNSubsBomEntries (
         const char * partName,
         int          bomEntryIndex,
         int *        nSubsBomEntries);

      void witGetBomEntryNonSubVarIndex (
         const char * consumingOperationName,
         int          bomEntryIndex,
         const int    thePer,
         int *        nonSubVarIndex);

      void witGetBomEntryOffset (
         const char *         partName,
         int                  bomEntryIndex,
         const WitOVRealArg & offset);

      void witGetBomEntryPropRouting (
         const char * consumingOperationName,
         int          bomEntryIndex,
         WitBoolean * propRouting);

      void witGetBomEntryPropRtg (
         const char *   consumingOperationName,
         int            bomEntryIndex,
         WitBoolean * * propRtg);

      void witGetBomEntryRoutingShare (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         const WitOVRealArg & routingShare);

      void witGetBomEntrySelForDel (
         const char * consumingOperationName,
         int          bomEntryIndex,
         WitBoolean * selForDel);

      void witGetBomEntrySingleSource (
         const char * consumingOperationName,
         int          bomEntryIndex,
         WitBoolean * singleSource);

      void witGetBomEntrySubConIndex (
         const char * consumingOperationName,
         int          bomEntryIndex,
         const int    thePer,
         int *        subConIndex);

      void witGetBomEntryUsageRate (
         const char *         partName,
         int                  bomEntryIndex,
         const WitOSRealArg & usageRate);

      void witGetBomEntryUsageTime (
         const char *         partName,
         int                  bomEntryIndex,
         const WitOSRealArg & usageTime);

      void witSetBomEntryAppData (
         const char * operationName,
         int          bomEntryIndex,
         void *       appData);

      void witSetBomEntryConsRate (
         const char *         operationName,
         int                  bomEntryIndex,
         const WitIVRealArg & consRate);

      void witSetBomEntryEarliestPeriod (
         const char * operationName,
         int          bomEntryIndex,
         int          earliestPeriod);

      void witSetBomEntryExecPenalty (
         const char *         operationName,
         int                  bomEntryIndex,
         const WitISRealArg & execPenalty);

      void witSetBomEntryFallout (
         const char * operationName,
         int          bomEntryIndex,
         int          fallout);

      void witSetBomEntryFalloutRate (
         const char *         operationName,
         int                  bomEntryIndex,
         const WitISRealArg & falloutRate);

      void witSetBomEntryLatestPeriod (
         const char * operationName,
         int          bomEntryIndex,
         int          latestPeriod);

      void witSetBomEntryMandEC (
         const char * operationName,
         int          bomEntryIndex,
         WitBoolean   mandEC);

      void witSetBomEntryOffset (
         const char *         operationName,
         int                  bomEntryIndex,
         const WitIVRealArg & offset);

      void witSetBomEntryPropRouting (
         const char * operationName,
         int          bomEntryIndex,
         WitBoolean   propRouting);

      void witSetBomEntryPropRtg (
         const char *       operationName,
         int                bomEntryIndex,
         const WitBoolean * propRtg);

      void witSetBomEntryRoutingShare (
         const char *         operationName,
         int                  bomEntryIndex,
         const WitIVRealArg & routingShare);

      void witSetBomEntrySelForDel (
         const char * operationName,
         int          bomEntryIndex,
         WitBoolean   selForDel);

      void witSetBomEntrySingleSource (
         const char * operationName,
         int          bomEntryIndex,
         WitBoolean   singleSource);

      void witSetBomEntryUsageRate (
         const char *         operationName,
         int                  bomEntryIndex,
         const WitISRealArg & usageRate);

      void witSetBomEntryUsageTime (
         const char *         operationName,
         int                  bomEntryIndex,
         const WitISRealArg & time);

      //------------------------------------------------------------------------
      // API functions implemented in subApi.C.
      //------------------------------------------------------------------------

      WitSubEntry * setGetSubBomEntryInit (
         const char *     consumingOperationName,
         int              bomEntryIndex,
         int              subsBomEntryIndex);

      void witAddSubsBomEntry (
         const char * consumingOperationName,
         int          bomEntryIndex,
         const char * consumedPartName);

      void witCopySubsBomEntryData (
         const char * dupOperationName,
         int          dupBomEntryIndex,
         int          dupSubsBomEntryIndex,
         WitRun *     origWitRun,
         const char * origOperationName,
         int          origBomEntryIndex,
         int          origSubsBomEntryIndex);

      void witGetObjItrSubsBomEntry (
         char * * consumingOperationName,
         int *    bomEntryIndex,
         int *    subsBomEntryIndex);

      void witGetSubsBomEntryAppData (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         void * *     appData);

      void witGetSubsBomEntryConsRate (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitOVRealArg & consRate);

      void witGetSubsBomEntryConsumedPart (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         char * *     consumedPart);

      void witGetSubsBomEntryEarliestPeriod (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         int *        earliestPeriod);

      void witGetSubsBomEntryExecPenalty (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitOSRealArg & execPenalty);

      void witGetSubsBomEntryExpAllowed (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         WitBoolean * expAllowed);

      void witGetSubsBomEntryExpNetAversion (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitOSRealArg & expNetAversion);

      void witGetSubsBomEntryFallout (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         int *        fallout);

      void witGetSubsBomEntryFalloutRate (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitOSRealArg & falloutRate);

      void witGetSubsBomEntryFssSubVol (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitOVRealArg & fssSubVol);

      void witGetSubsBomEntryImpactPeriod (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         int * *      impactPeriod);

      void witGetSubsBomEntryIntSubVols (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         WitBoolean * intSubVols);

      void witGetSubsBomEntryLatestPeriod (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         int *        latestPeriod);

      void witGetSubsBomEntryMrpNetAllowed (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         WitBoolean * mrpNetAllowed);

      void witGetSubsBomEntryMrpSubVol (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitOVRealArg & mrpSubVol);

      void witGetSubsBomEntryNetAllowed (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         WitBoolean * netAllowed);

      void witGetSubsBomEntryObj1SubCost (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitOVRealArg & obj1SubCost);

      void witGetSubsBomEntryOffset (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitOVRealArg & offset);

      void witGetSubsBomEntryProdVol (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitOVRealArg & prodVol);

      void witGetSubsBomEntryRoutingShare (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitOVRealArg & routingShare);

      void witGetSubsBomEntrySelForDel (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         WitBoolean * selForDel);

      void witGetSubsBomEntrySubCost (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitOVRealArg & subCost);

      void witGetSubsBomEntrySubVarIndex (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         const int    thePer,
         int *        subVarIndex);

      void witGetSubsBomEntrySubVol (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitOVRealArg & subVol);

      void witGetSubsBomEntryUsageRate (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitOSRealArg & usageRate);

      void witSetSubsBomEntryAppData (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         void *       appData);

      void witSetSubsBomEntryConsRate (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitIVRealArg & consRate);

      void witSetSubsBomEntryEarliestPeriod (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         int          earliestPeriod);

      void witSetSubsBomEntryExecPenalty (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitISRealArg & execPenalty);

      void witSetSubsBomEntryExpAllowed (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         WitBoolean   expAllowed);

      void witSetSubsBomEntryExpNetAversion (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitISRealArg & expNetAversion);

      void witSetSubsBomEntryFallout (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         int          fallout);

      void witSetSubsBomEntryFalloutRate (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitISRealArg & falloutRate);

      void witSetSubsBomEntryIntSubVols (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         WitBoolean   intSubVols);

      void witSetSubsBomEntryLatestPeriod (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         int          latestPeriod);

      void witSetSubsBomEntryMrpNetAllowed (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         WitBoolean   mrpNetAllowed);

      void witSetSubsBomEntryNetAllowed (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         WitBoolean   netAllowed);

      void witSetSubsBomEntryObj1SubCost (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitIVRealArg & obj1SubCost);

      void witSetSubsBomEntryOffset (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitIVRealArg & offset);

      void witSetSubsBomEntryProdVol (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitIVRealArg & prodVol);

      void witSetSubsBomEntryRoutingShare (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitIVRealArg & routingShare);

      void witSetSubsBomEntrySelForDel (
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex,
         WitBoolean   selForDel);

      void witSetSubsBomEntrySubCost (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitIVRealArg & subCost);

      void witSetSubsBomEntrySubVol (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitIVRealArg & subVol);

      void witSetSubsBomEntryUsageRate (
         const char *         consumingOperationName,
         int                  bomEntryIndex,
         int                  subsBomEntryIndex,
         const WitISRealArg & usageRate);

      //------------------------------------------------------------------------
      // API functions implemented in bopApi.C.
      //------------------------------------------------------------------------

      WitBopEntry * setGetBopEntryInit (
         const char *    producingOperationName,
         int             bopEntryIndex);

      void witAddBopEntry (
         const char * producingOperationName,
         const char * producedPartName);

      void witCopyBopEntryData (
         const char * dupOperationName,
         int          dupBopEntryIndex,
         WitRun *     origWitRun,
         const char * origOperationName,
         int          origBopEntryIndex);

      void witGetObjItrBopEntry (
         char * * producingOperationName,
         int *    bopEntryIndex);

      void witGetBopEntryAppData (
         const char * producingOperationName,
         int          bomEntryIndex,
         void * *     appData);

      void witGetBopEntryByProduct (
         const char * producingOperationName,
         int          bomEntryIndex,
         WitBoolean * byProduct);

      void witGetBopEntryEarliestPeriod (
         const char * producingOperationName,
         int          bomEntryIndex,
         int *        earliestPeriod);

      void witGetBopEntryExpAllowed (
         const char * producingOperationName,
         int          bomEntryIndex,
         WitBoolean * expAllowed);

      void witGetBopEntryExpAversion (
         const char *         producingOperationName,
         int                  bomEntryIndex,
         const WitOSRealArg & expAversion);

      void witGetBopEntryImpactPeriod (
         const char * producingOperationName,
         int          bopEntryIndex,
         int * *      impactPeriod);

      void witGetBopEntryLatestPeriod (
         const char * producingOperationName,
         int          bomEntryIndex,
         int *        latestPeriod);

      void witGetBopEntryMeSelSplit (
         const char * producingOperationName,
         int          bomEntryIndex,
         WitBoolean * meSelSplit);

      void witGetBopEntryOffset (
         const char *         producingOperationName,
         int                  bomEntryIndex,
         const WitOVRealArg & offset);

      void witGetBopEntryProdRate (
         const char *         producingOperationName,
         int                  bomEntryIndex,
         const WitOSRealArg & prodRate);

      void witGetBopEntryProductRate (
         const char *         producingOperationName,
         int                  bomEntryIndex,
         const WitOVRealArg & productRate);

      void witGetBopEntryProducedPart (
         const char * producingOperationName,
         int          bopEntryIndex,
         char * *     producedPartName);

      void witGetBopEntryRoutingShare (
         const char *         producingOperationName,
         int                  bomEntryIndex,
         const WitOVRealArg & routingShare);

      void witGetBopEntrySelForDel (
         const char * producingOperationName,
         int          bomEntryIndex,
         WitBoolean * selForDel);

      void witSetBopEntryAppData (
         const char * producingOperationName,
         int          bopEntryIndex,
         void *       appData);

      void witSetBopEntryByProduct (
         const char * producingOperationName,
         int          bopEntryIndex,
         WitBoolean   byProduct);

      void witSetBopEntryEarliestPeriod (
         const char * producingOperationName,
         int          bopEntryIndex,
         int          earliestPeriod);

      void witSetBopEntryExpAllowed (
         const char * producingOperationName,
         int          bopEntryIndex,
         WitBoolean   expAllowed);

      void witSetBopEntryExpAversion (
         const char *         producingOperationName,
         int                  bopEntryIndex,
         const WitISRealArg & expAversion);

      void witSetBopEntryLatestPeriod (
         const char * producingOperationName,
         int          bopEntryIndex,
         int          latestPeriod);

      void witSetBopEntryMeSelSplit (
         const char * producingOperationName,
         int          bopEntryIndex,
         WitBoolean   meSelSplit);

      void witSetBopEntryOffset (
         const char *         producingOperationName,
         int                  bopEntryIndex,
         const WitIVRealArg & offset);

      void witSetBopEntryProdRate (
         const char *         producingOperationName,
         int                  bopEntryIndex,
         const WitISRealArg & prodRate);

      void witSetBopEntryProductRate (
         const char *         producingOperationName,
         int                  bopEntryIndex,
         const WitIVRealArg & productRate);

      void witSetBopEntryRoutingShare (
         const char *         producingOperationName,
         int                  bopEntryIndex,
         const WitIVRealArg & routingShare);

      void witSetBopEntrySelForDel (
         const char * producingOperationName,
         int          bopEntryIndex,
         WitBoolean   selForDel);

      //------------------------------------------------------------------------
      // API functions implemented in msgApi.C.
      //------------------------------------------------------------------------

      void witGetMesgFile (
         FILE * * mesgFile);

      void witSetMesgFileAccessMode (
         WitBoolean   quiet,
         const char * mesgFileAccessMode);

      void witGetMesgFileAccessMode (
         char * * mesgFileAccessMode);

      void witSetMesgFileName (
         WitBoolean   quiet,
         const char * mesgFileName);

      void witGetMesgFileName (
         char * * mesgFileName);

      void witSetMesgPrintNumber (
         WitBoolean quiet,
         int        messageNumber,
         WitBoolean mesgPrintMunber);

      void witGetMesgPrintNumber (
         int          messageNumber,
         WitBoolean * mesgPrintMunber);

      void witSetMesgStopRunning (
         WitBoolean quiet,
         int        messageNumber,
         WitBoolean mesgStopRunning);

      void witGetMesgStopRunning (
         int          messageNumber,
         WitBoolean * mesgStopRunning);

      void witSetMesgThrowErrorExc (
         WitBoolean quiet,
         int        messageNumber,
         WitBoolean mesgThrowErrorExc);

      void witGetMesgThrowErrorExc (
         int          messageNumber,
         WitBoolean * mesgThrowErrorExc);

      void witSetMesgTimesPrint (
         WitBoolean quiet,
         int        messageNumber,
         int        mesgTimesPrint);

      void witGetMesgTimesPrint (
         int        messageNumber,
         int *      mesgTimesPrint);

      void witListUnissuedMsgs ();

      //------------------------------------------------------------------------
      // Non-API public member functions implemented in WitRun.C.
      //------------------------------------------------------------------------

      static WitRun * newInstance ();
         //
         // Constructs (on the heap) and returns a new WitRun.
         // To be used instead of the WitRun ctor, which is private.

      static void validateWitRun (WitRun * theWitRun, const char * theFuncName);
         //
         // Verifies that theWitRun is a valid WitRun.
         //
         // theFuncName: Name of the outer API function that was called.

      void forbidNullArg (const void * theArg, const char * argName);
         //
         // theArg:  A pointer that was passed to WIT as an argument to an
         //          API function.
         // argName: The name of the argument
         // If theArg is NULL, this function issues a severe error
         // identifying argName.

      static void apiVoidAlloc (void * *, int, size_t);
         //
         // Called by NonClass::apiAlloc.

      //------------------------------------------------------------------------
      // apiGet functions.
      //
      // These functions are designed to be called by the witGet<Object><Attr>
      // API functions. A typical apiGet function has three arguments:
      // theValue is the value to be returned by the calling API function.
      // theVarPtr is a pointer to a variable in which the value is to be
      // stored.
      // argName is the name of the argument for error reporting.
      //
      // At its simplest, an apiGet function is implemented as a simple
      // assignment. But it is also responsible for translating from the WIT
      // internal representation of an attribute (the type of theValue) into
      // the API representation of the attribute (the type of * theVarPtr). It
      // is also responsible for allocating memory, if needed.
      // Also, if theVarPtr is NULL, a severe error is issued.
      //------------------------------------------------------------------------

      void apiGet (
         int *        theVarPtr,
         const char * argName,
         int          theValue);

      void apiGet (
         WitBoolean * theVarPtr,
         const char * argName,
         bool         theValue);

      void apiGet (
         const WitOSRealArg & theArg,
         const char *         argName,
         double               theValue);

      void apiGet (
         WitAttr *    theVarPtr,
         const char * argName,
         WitAttr      theValue);

      void apiGet (
         FILE * *     theVarPtr,
         const char * argName,
         FILE *       theValue);

      void apiGet (
         void * *     theVarPtr,
         const char * argName,
         void *       theValue);

      void apiGet (
         char * *          theVarPtr,
         const char *      argName,
         const WitString & theValue);

      void apiGet (
         const WitOVRealArg &       theArg,
         const char *               argName,
         const WitVector <double> & theValue);

      void apiGet (
         int * *                 theVarPtr,
         const char *            argName,
         const WitVector <int> & theValue);

      void apiGet (
         WitBoolean * *           theVarPtr,
         const char *             argName,
         const WitVector <bool> & theValue);

      void apiGet (
         const WitOVRealArg &  theArg,
         const char *          argName,
         const WitDblFlexVec & theValue);

      void apiGet (
         WitBoolean * *            theVarPtr,
         const char *              argName,
         const WitFlexVec <bool> & theValue);

      void apiGet (
         int * *                  theVarPtr,
         const char *             argName,
         const WitFlexVec <int> & theValue);

      void apiGet (
         const WitOVRealArg & hardLBVec,
         const WitOVRealArg & softLBVec,
         const WitOVRealArg & hardUBVec,
         WitBoundSet *        theValue);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc    (WitApiMgr *,       myApiMgr)
      accessFunc    (WitSession *,      mySession)

      accessWrapper (WitMsgFacility &,  myMsgFac,        mySession ())
      accessWrapper (WitProblem *,      myProblem,       mySession ())
      accessWrapper (WitCompMgr *,      myCompMgr,       myProblem ())
      accessWrapper (WitGlobalComp *,   myGlobalComp,    myProblem ())
      accessWrapper (WitOptComp *,      myOptComp,       myProblem ())
      accessWrapper (WitDetOptImpMgr *, myDetOptImpMgr,  myProblem ())
      accessWrapper (WitStochImpMgr *,  myStochImpMgr,   myProblem ())
      accessWrapper (int,               nPeriods,        myProblem ())
      accessWrapper (bool,              wit34Compatible, myProblem ())

      WitInputID * myInputID () const;

   private:

      //------------------------------------------------------------------------
      // Private member functions implemented in WitRun.C.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitRun ();
         //
         // Declared private so that newInstance must be used.

      //------------------------------------------------------------------------
      // Other private member functions implemented in WitRun.C.
      //------------------------------------------------------------------------

      WitDelComp * curItrDelCompForGet (
            WitAttr      objItrStateVal,
            WitMsgFragID theObjTypeFragID);
         //
         // Does error checking for witGetObjItr<Object> functions.
         // objItrStateVal and theObjTypeFragID must match the class of DelComp
         // iteration's current DelComp.
         // If no errors occur, returns DelComp iteration's current DelComp.

      static void handleBadAllocForCtor ();
         //
         // Handles an std::bad_alloc exception during the ctor for this class.

      void startCopyObjData (WitRun * origWitRun);
         //
         // Performs the tasks of a witCopy<Object>Data function that are not
         // specific to the function.

      void checkGlobAttsForCopy (WitRun * origWitRun);
         //
         // Verifies that the necessary global attributes of this WitRun and
         // origWitRun match for a call to a witCopy<Object>Data function.

      void prepSet34Attr (
            const char * the34Object,
            const char * the34Attribute,
            const char * the40Object,
            const char * the40Attribute);
         //
         // Does the preparatory tasks for an API "set" function on a WIT 3.4
         // attribute. The tasks are:
         //    Verify 3.4 compatibility mode.
         //    Issue the appropriate message.
         // the34Object    is the name of the object type for the 3.4 attribute.
         // the34Attribute is the name of the 3.4 attribute
         // the40Object    is the name of the object type for the 4.0 attribute.
         // the40Attribute is the name of the 4.0 attribute

      void prepGet34Attr (
            const char * the34Object,
            const char * the34Attribute,
            const char * the40Object,
            const char * the40Attribute);
         //
         // Does the preparatory work for an API "get" function on a WIT 3.4
         // attribute. The tasks are:
         //    Verify 3.4 compatibility mode.
         //    Issue the appropriate message.
         // the34Object    is the name of the object type for the 3.4 attribute.
         // the34Attribute is the name of the 3.4 attribute
         // the40Object    is the name of the object type for the 4.0 attribute.
         // the40Attribute is the name of the 4.0 attribute

      void prepGetIndex (int thePer);
         //
         // Prepares a witGet...VarIndex or ConIndex API function call.

      void checkNPeriodsWasSet ();
         //
         // Issues a warning if nPeriods has not been set and no Nodes have yet
         // been created.

      void addingDelComp (WitMsgFragID theObjTypeFragID);
         //
         // Takes action appropriate when a DelComp is being added.
         // theObjTypeFragID identifies the type of the DelComp.

      void forbidStochMode ();
         //
         // Issues a severe error, if this WitRun is in stochastic mode.

      const char * apiFuncName ();
         //
         // Returns myApiMgr ()->apiFuncName ();

      noCopyCtorAssign (WitRun);

      //------------------------------------------------------------------------
      // Private member functions implemented in globApi.C.
      //------------------------------------------------------------------------

      void issueGetAttrMsg (const char * theAttName) const;
         //
         // Issues a message for getting the attribute whose name is given by
         // theAttName, if appropriate.

      bool alwaysTrue ();
         //
         // Always returns true. This is used by witExhaustMemory () to prevent
         // the compiler from issuing a warning message about the function's
         // apparent infinite loop. Also, this function must be implemented in
         // a file other than WitRun.C, in order to prevent the compiler from
         // analyzing its content, when it's compiling witExhaustMemory ().

      //------------------------------------------------------------------------
      // Private member functions implemented in optApi.C.
      //------------------------------------------------------------------------

      void parseObjectiveListSpec (
            const char *     objectiveListSpec,
            WitList <char> & theObjNameList);
         //
         // Parses objectiveListSpec and stores the Objective names that it
         // specifies in theObjNameList
         // The c-strings stored by this function will need to be deleted.

      void checkForDupObjNames (const WitVector <const char *> & theObjNames);
         //
         // Checks for duplicate names in theObjNames.

      //------------------------------------------------------------------------
      // Private member functions implemented in partApi.C.
      //------------------------------------------------------------------------

      WitPart * addPart34 (
            const char * partName, WitAttr partCategory);
         //
         // Adds a part in WIT 3.4 mode.

      WitPart * addPartPost34 (
            const char * partName, WitAttr partCategory);
         //
         // Adds a part in Post WIT 3.4 mode.

      WitPart * getPartP (const char * partName);

      //------------------------------------------------------------------------
      // Private member functions implemented in demApi.C.
      //------------------------------------------------------------------------

      WitDemand * getDemandP (
         const char * demandedPartName,
         const char * demandName);

      void eqHeurAlloc (
         int                  lenLists,
         const char * const * demandedPartNameList,
         const char * const * demandNameList,
         const int *          shipPeriodList,
         const WitIVRealArg & desIncVolList,
         const WitOVRealArg & incVolList,
         const WitBoolean *   asapMultiExecList);

      void incHeurAlloc (
         const char *         demandedPartName,
         const char *         demandName,
         int                  shipPeriod,
         const WitISRealArg & desIncVol,
         const WitOSRealArg & incVol,
         WitBoolean           asapMultiExec);

      void defineAllocTarget (
         const char *     demandedPartName,
         const char *     demandName,
         int              shipPeriod,
         double           desIncVol,
         bool             asapMultiExec,
         WitDemPerStack & theDemPers);

      void prepGetPegging (int shipPeriod);
         //
         // Prepares a concurrent pegging API function call.

      void getPartPip (
         WitPartTripStack &   thePegTrips,
         int *                lenLists,
         char * * *           partNameList,
         int * *              periodList,
         const WitOVRealArg & peggedVolList);

      //------------------------------------------------------------------------
      // Private member functions implemented in opnApi.C.
      //------------------------------------------------------------------------

      WitOperation * getOperationP (const char * operationName);

      //------------------------------------------------------------------------
      // Private member functions implemented in bomApi.C.
      //------------------------------------------------------------------------

      WitBomEntry * getBomEntryP (
         const char * consumingOperationName,
         int          bomEntryIndex);

      //------------------------------------------------------------------------
      // Private member functions implemented in subApi.C.
      //------------------------------------------------------------------------

      WitSubEntry * getSubBomEntryP (
         const char * operationName,
         int          bomEntryIndex,
         int          subsBomEntryIndex);

      //------------------------------------------------------------------------
      // Private member functions implemented in bopApi.C.
      //------------------------------------------------------------------------

      WitBopEntry * getBopEntryP (
         const char * producingOperationName,
         int          bopEntryIndex);

      //------------------------------------------------------------------------
      // Private member functions implemented in msgApi.C.
      //------------------------------------------------------------------------

      const char * levelExtNumFragID (int theExtNum);
         //
         // Returns the message fragment ID for a severity level indicated by
         // theExtNum.

      bool checkMsgExtNum (int theExtNum);
         //
         // Returns true, iff theExtNum is the externalNumber of a Msg.
         // If not, issues a warning Msg.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      int certificate_;
         //
         // This WitRun is a valid WitRun only if
         // certificate_ == VALID_CERTIFICATE.

      WitApiMgr * myApiMgr_;
         //
         // The ApiMgr owned by this WitRun.

      WitSession * mySession_;
         //
         // The Session owned by this WitRun.

      int nDepVeneers_;
         //
         // The number of LgFrWitRunVeneers that point to this WitRun
   };

//------------------------------------------------------------------------------

namespace WitNonClass
   {
   char * dupCString (const char * theCString);
      //
      // Creates and returns a copy of theCString.
      // Uses malloc.

   template <typename Elem> 
         void apiAlloc (Elem * * theCVecPtr, int nElems);
      //
      // Allocates * theCVecPtr to nElems elements.
      // Uses malloc, so the * theCVecPtr can be passed back to the application
      // program.

   void apiAlloc (const WitOVRealArg & theOVRealArg, int nElems);
      //
      // Same as the above.
   };

#endif
