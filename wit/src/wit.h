/*============================================================================*/
/* Constrained Materials Management and Production Planning Tool              */
/*                                                                            */
/* (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved                    */
/*============================================================================*/

#ifndef wit_h
#define wit_h

#include <stdio.h>
#include <limits.h>

/*----------------------------------------------------------------------------*/
/* WIT typedefs.                                                              */
/*----------------------------------------------------------------------------*/

typedef const char * witAttr;
typedef const char * witFileFormat;
typedef int          witReturnCode;
typedef int          witBoolean;

enum { WitTRUE = 1, WitFALSE = 0 };

#ifdef __cplusplus

   class WitRun;

#else

   typedef struct WitRunStruct WitRun;

#endif

#ifdef __cplusplus

   extern "C"
      {

#endif

/*----------------------------------------------------------------------------*/
/* WIT API function return codes.                                             */
/*----------------------------------------------------------------------------*/

extern const witReturnCode WitINFORMATIONAL_RC;
extern const witReturnCode WitWARNING_RC;
extern const witReturnCode WitSEVERE_RC;
extern const witReturnCode WitFATAL_RC;

/*----------------------------------------------------------------------------*/
/* WIT API functions for the current release.                                 */
/*----------------------------------------------------------------------------*/

witReturnCode witAddBomEntry (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const char * const consumedPartName);

witReturnCode witAddBopEntry (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const char * const producedPartName);

witReturnCode witAddDblCplexParSpec (
   WitRun * const     theWitRun,
   const char * const theName,
   const float        theValue);

witReturnCode witAddDblCplexParSpecDbl (
   WitRun * const     theWitRun,
   const char * const theName,
   const double       theValue);

witReturnCode witAddDemand (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName);

witReturnCode witAddIntCplexParSpec (
   WitRun * const     theWitRun,
   const char * const theName,
   const int          theValue);

witReturnCode witAddOperation (
   WitRun * const theWitRun,
   const char * const operationName);

witReturnCode witAddPart (
   WitRun * const theWitRun,
   const char * const partName,
   const witAttr partCategory);

witReturnCode witAddPartWithOperation (
   WitRun * const theWitRun,
   const char * const partAndOperationName);

witReturnCode witAddSubsBomEntry (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const char * const consumedPartName);

witReturnCode witAdvanceObjItr (
   WitRun * const theWitRun);

witReturnCode witAppendToPipSeq (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   int                shipPeriod,
   float              incShipVol);

witReturnCode witAppendToPipSeqDbl (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   int                shipPeriod,
   double             incShipVol);

witReturnCode witBuildPip (
   WitRun * const theWitRun);

witReturnCode witClearCplexParSpecs (
   WitRun * const theWitRun);

witReturnCode witClearPegging (
   WitRun * const theWitRun);

witReturnCode witClearPipSeq (
   WitRun * const theWitRun);

witReturnCode witClearStochSoln (
   WitRun * const theWitRun);

witReturnCode witCopyBomEntryData (
   WitRun * const     dupWitRun,
   const char * const dupOperationName,
   const int          dupBomEntryIndex,
   WitRun * const     origWitRun,
   const char * const origOperationName,
   const int          origBomEntryIndex);

witReturnCode witCopyBopEntryData (
   WitRun * const     dupWitRun,
   const char * const dupOperationName,
   const int          dupBopEntryIndex,
   WitRun * const     origWitRun,
   const char * const origOperationName,
   const int          origBopEntryIndex);

witReturnCode witCopyData (
   WitRun * const dupWitRun,
   WitRun * const origWitRun);

witReturnCode witCopyDemandData (
   WitRun * const     dupWitRun,
   const char * const dupPartName,
   const char * const dupDemandName,
   WitRun * const     origWitRun,
   const char * const origPartName,
   const char * const origDemandName);

witReturnCode witCopyOperationData (
   WitRun * const     dupWitRun,
   const char * const dupOperationName,
   WitRun * const     origWitRun,
   const char * const origOperationName);

witReturnCode witCopyPartData (
   WitRun * const     dupWitRun,
   const char * const dupPartName,
   WitRun * const     origWitRun,
   const char * const origPartName);

witReturnCode witCopySubsBomEntryData (
   WitRun * const     dupWitRun,
   const char * const dupOperationName,
   const int          dupBomEntryIndex,
   const int          dupSubsBomEntryIndex,
   WitRun * const     origWitRun,
   const char * const origOperationName,
   const int          origBomEntryIndex,
   const int          origSubsBomEntryIndex);

witReturnCode witDeleteRun (
   WitRun * theWitRun);

witReturnCode witDisplayData (
   WitRun * const theWitRun,
   const char * const fileName);

witReturnCode witEqHeurAlloc (
   WitRun * const theWitRun,
   const int lenLists,
   const char * const * const demandedPartNameList,
   const char * const * const demandNameList,
   const int * const shipPeriodList,
   const float * const desIncVolList,
   float * * incVolList);

witReturnCode witEqHeurAllocDbl (
   WitRun * const theWitRun,
   const int lenLists,
   const char * const * const demandedPartNameList,
   const char * const * const demandNameList,
   const int * const shipPeriodList,
   const double * const desIncVolList,
   double * * incVolList);

witReturnCode witEqHeurAllocTwme (
   WitRun * const theWitRun,
   const int lenLists,
   const char * const * const demandedPartNameList,
   const char * const * const demandNameList,
   const int * const shipPeriodList,
   const float * const desIncVolList,
   float * * incVolList,
   const witBoolean * const asapMultiExecList);

witReturnCode witEqHeurAllocTwmeDbl (
   WitRun * const theWitRun,
   const int lenLists,
   const char * const * const demandedPartNameList,
   const char * const * const demandNameList,
   const int * const shipPeriodList,
   const double * const desIncVolList,
   double * * incVolList,
   const witBoolean * const asapMultiExecList);

witReturnCode witEvalObjectives (
   WitRun * const theWitRun);

witReturnCode witFinishExtOpt (
   WitRun * const theWitRun);

witReturnCode witFinishHeurAlloc (
   WitRun * const theWitRun);

witReturnCode witFree (
   void * mem );

witReturnCode witGeneratePriorities (
   WitRun * const theWitRun);

witReturnCode witGetAccAfterSoftLB (
   WitRun * const theWitRun,
   witBoolean * accAfterSoftLB);

witReturnCode witGetAccAfterOptImp (
   WitRun * const theWitRun,
   witBoolean * accAfterOptImp);

witReturnCode witGetAccelerated (
   WitRun * const theWitRun,
   witBoolean * accelerated);

witReturnCode witGetAppData (
   WitRun * const theWitRun,
   void * * appData);

witReturnCode witGetAutoPriority (
   WitRun * const theWitRun,
   witBoolean * autoPriority);

witReturnCode witGetBomEntryAppData (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   void * * appData);

witReturnCode witGetBomEntryConsRate (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   float * * consRate);

witReturnCode witGetBomEntryConsRateDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   double * * consRate);

witReturnCode witGetBomEntryConsumedPart (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   char * * consumedPartName);

witReturnCode witGetBomEntryEarliestPeriod (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   int * earliestPeriod);

witReturnCode witGetBomEntryExecPenalty (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   float * execPenalty);

witReturnCode witGetBomEntryExecPenaltyDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   double * execPenalty);

witReturnCode witGetBomEntryFalloutRate (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   float * falloutRate);

witReturnCode witGetBomEntryFalloutRateDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   double * falloutRate);

witReturnCode witGetBomEntryImpactPeriod (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   int * * impactPeriod);

witReturnCode witGetBomEntryLatestPeriod (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   int * latestPeriod);

witReturnCode witGetBomEntryMandEC (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   witBoolean * mandEC);

witReturnCode witGetBomEntryNSubsBomEntries (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   int * nSubsBomEntries);

witReturnCode witGetBomEntryNonSubVarIndex (
   WitRun * const     theWitRun,
   const char * const consumingOperationName,
   const int          bomEntryIndex,
   const int          thePer,
   int *              nonSubVarIndex);

witReturnCode witGetBomEntryOffset (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   float * * offset);

witReturnCode witGetBomEntryOffsetDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   double * * offset);

witReturnCode witGetBomEntryPropRtg (
   WitRun * const     theWitRun,
   const char * const consumingOperationName,
   const int          bomEntryIndex,
   witBoolean * *     propRtg);

witReturnCode witGetBomEntryRoutingShare (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   float * * routingShare);

witReturnCode witGetBomEntryRoutingShareDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   double * * routingShare);

witReturnCode witGetBomEntrySelForDel (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   witBoolean * selForDel);

witReturnCode witGetBomEntrySingleSource (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   witBoolean * singleSource);

witReturnCode witGetBomEntrySubConIndex (
   WitRun * const     theWitRun,
   const char * const consumingOperationName,
   const int          bomEntryIndex,
   const int          thePer,
   int *              subConIndex);

witReturnCode witGetBopEntryAppData (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   void * * appData);

witReturnCode witGetBopEntryEarliestPeriod (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   int * earliestPeriod);

witReturnCode witGetBopEntryExpAllowed (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   witBoolean * expAllowed);

witReturnCode witGetBopEntryExpAversion (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   float * expAversion);

witReturnCode witGetBopEntryExpAversionDbl (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   double * expAversion);

witReturnCode witGetBopEntryImpactPeriod (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   int * * impactPeriod);

witReturnCode witGetBopEntryLatestPeriod (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   int * latestPeriod);

witReturnCode witGetBopEntryOffset (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   float * * offset);

witReturnCode witGetBopEntryOffsetDbl (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   double * * offset);

witReturnCode witGetBopEntryProductRate (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   float * * productRate);

witReturnCode witGetBopEntryProductRateDbl (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   double * * productRate);

witReturnCode witGetBopEntryProducedPart (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   char * * producedPartName);

witReturnCode witGetBopEntryRoutingShare (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   float * * routingShare);

witReturnCode witGetBopEntryRoutingShareDbl (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   double * * routingShare);

witReturnCode witGetBopEntrySelForDel (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   witBoolean * selForDel);

witReturnCode witGetBoundsValue (
   WitRun * const theWitRun,
   float *        boundsValue);

witReturnCode witGetBoundsValueDbl (
   WitRun * const theWitRun,
   double *       boundsValue);

witReturnCode witGetCompPrices (
   WitRun * const theWitRun,
   witBoolean * compPrices);

witReturnCode witGetComputeCriticalList (
   WitRun * const theWitRun,
   witBoolean * computeCriticalList);

witReturnCode witGetCplexEmbedded (
   WitRun * const theWitRun,
   witBoolean *   cplexEmbedded);

witReturnCode witGetCplexMipBound (
   WitRun * const theWitRun,
   float * cplexMipBound);

witReturnCode witGetCplexMipBoundDbl (
   WitRun * const theWitRun,
   double * cplexMipBound);

witReturnCode witGetCplexMipRelGap (
   WitRun * const theWitRun,
   float * cplexMipRelGap);

witReturnCode witGetCplexMipRelGapDbl (
   WitRun * const theWitRun,
   double * cplexMipRelGap);

witReturnCode witGetCplexParSpecDblVal (
   WitRun * const theWitRun,
   float * cplexParSpecDblVal);

witReturnCode witGetCplexParSpecDblValDbl (
   WitRun * const theWitRun,
   double * cplexParSpecDblVal);

witReturnCode witGetCplexParSpecIntVal (
   WitRun * const theWitRun,
   int * cplexParSpecIntVal);

witReturnCode witGetCplexParSpecName (
   WitRun * const theWitRun,
   char * * cplexParSpecName);

witReturnCode witGetCplexStatusCode (
   WitRun * const theWitRun,
   int *          cplexStatusCode);

witReturnCode witGetCplexStatusText (
   WitRun * const theWitRun,
   char * *       cplexStatusText);

witReturnCode witGetCriticalList (
   WitRun * const theWitRun,
   int * lenCritList,
   char * * * critPartList,
   int * * critPeriod);

witReturnCode witGetCurrentObjective (
   WitRun * const theWitRun,
   char * * theObjName);

witReturnCode witGetCurrentScenario (
   WitRun * const theWitRun,
   int * currentScenario);

witReturnCode witGetDblCplexParSpec (
   WitRun * const     theWitRun,
   const char * const theName,
   witBoolean *       dblSpecExists,
   float *            theValue);

witReturnCode witGetDblCplexParSpecDbl (
   WitRun * const     theWitRun,
   const char * const theName,
   witBoolean *       dblSpecExists,
   double *           theValue);

witReturnCode witGetDemandAppData (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   void * * appData);

witReturnCode witGetDemandCoExecVolPip (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         operationNameList,
   int * *            bopEntryIndexList,
   int * *            execPeriodList,
   float * *          peggedVolList);

witReturnCode witGetDemandCoExecVolPipDbl (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         operationNameList,
   int * *            bopEntryIndexList,
   int * *            execPeriodList,
   double * *         peggedVolList);

witReturnCode witGetDemandConsVolPip (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   float * *          peggedVolList);

witReturnCode witGetDemandConsVolPipDbl (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   double * *         peggedVolList);

witReturnCode witGetDemandCumShipBounds (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   float * * hardLower,
   float * * softLower,
   float * * hardUpper);

witReturnCode witGetDemandCumShipBoundsDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   double * * hardLower,
   double * * softLower,
   double * * hardUpper);

witReturnCode witGetDemandCumShipReward (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   float * * cumShipReward);

witReturnCode witGetDemandCumShipRewardDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   double * * cumShipReward);

witReturnCode witGetDemandCumShipSlbConIndex (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          thePer,
   int *              cumShipSlbConIndex);

witReturnCode witGetDemandCumShipSlbvVarIndex (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          thePer,
   int *              cumShipSlbvVarIndex);

witReturnCode witGetDemandCumShipVarIndex (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          thePer,
   int *              cumShipVarIndex);

witReturnCode witGetDemandDemandVol (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   float * * demandVol);

witReturnCode witGetDemandDemandVolDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   double * * demandVol);

witReturnCode witGetDemandExecVolPegging (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int shipPeriod,
   int * lenLists,
   char * * * operationNameList,
   int * * execPeriodList,
   float * * peggedExecVolList);

witReturnCode witGetDemandExecVolPeggingDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int shipPeriod,
   int * lenLists,
   char * * * operationNameList,
   int * * execPeriodList,
   double * * peggedExecVolList);

witReturnCode witGetDemandExecVolPip (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         operationNameList,
   int * *            execPeriodList,
   float * *          peggedExecVolList);

witReturnCode witGetDemandExecVolPipDbl (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         operationNameList,
   int * *            execPeriodList,
   double * *         peggedExecVolList);

witReturnCode witGetDemandExists (
   WitRun * const     theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   witBoolean *       exists);

witReturnCode witGetDemandFocusHorizon (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   int  * focusHorizon);

witReturnCode witGetDemandFssShipVol (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   float * * fssShipVol);

witReturnCode witGetDemandFssShipVolDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   double * * fssShipVol);

witReturnCode witGetDemandIntShipVols (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   witBoolean * intShipVols);

witReturnCode witGetDemandLeadTimeUB (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   int * * leadTimeUB);

witReturnCode witGetDemandPriority (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   int * * priority);

witReturnCode witGetDemandProdVolPip (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   float * *          peggedVolList);

witReturnCode witGetDemandProdVolPipDbl (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   double * *         peggedVolList);

witReturnCode witGetDemandSearchInc (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   float * searchInc);

witReturnCode witGetDemandSearchIncDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   double * searchInc);

witReturnCode witGetDemandSelForDel (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   witBoolean * selForDel);

witReturnCode witGetDemandShipConIndex (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          thePer,
   int *              shipConIndex);

witReturnCode witGetDemandShipLateAllowed (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   witBoolean * shipLateAllowed);

witReturnCode witGetDemandShipLateUB (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   int * * shipLateUB);

witReturnCode witGetDemandShipReward (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   float * * shipReward);

witReturnCode witGetDemandShipRewardDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   double * * shipReward);

witReturnCode witGetDemandShipVarIndex (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          thePer,
   int *              shipVarIndex);

witReturnCode witGetDemandShipVol (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   float * * shipVol);

witReturnCode witGetDemandShipVolDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   double * * shipVol);

witReturnCode witGetDemandSideVolPip (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   float * *          peggedVolList);

witReturnCode witGetDemandSideVolPipDbl (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   double * *         peggedVolList);

witReturnCode witGetDemandSubVolPegging (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int shipPeriod,
   int * lenLists,
   char * * * operationNameList,
   int * * bomEntryIndexList,
   int * * subsBomEntryIndexList,
   int * * execPeriodList,
   float * * peggedSubVolList);

witReturnCode witGetDemandSubVolPeggingDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int shipPeriod,
   int * lenLists,
   char * * * operationNameList,
   int * * bomEntryIndexList,
   int * * subsBomEntryIndexList,
   int * * execPeriodList,
   double * * peggedSubVolList);

witReturnCode witGetDemandSubVolPip (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         operationNameList,
   int * *            bomEntryIndexList,
   int * *            subsBomEntryIndexList,
   int * *            execPeriodList,
   float * *          peggedSubVolList);

witReturnCode witGetDemandSubVolPipDbl (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         operationNameList,
   int * *            bomEntryIndexList,
   int * *            subsBomEntryIndexList,
   int * *            execPeriodList,
   double * *         peggedSubVolList);

witReturnCode witGetDemandSupplyVolPip (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   float * *          peggedVolList);

witReturnCode witGetDemandSupplyVolPipDbl (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const int          shipPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   double * *         peggedVolList);

witReturnCode witGetEquitability (
   WitRun * const theWitRun,
   int * equitablity);

witReturnCode witGetExecEmptyBom (
   WitRun * const theWitRun,
   witBoolean * execEmptyBom);

witReturnCode witGetExpCutoff (
   WitRun * const theWitRun,
   float * expCutoff);

witReturnCode witGetExpCutoffDbl (
   WitRun * const theWitRun,
   double * expCutoff);

witReturnCode witGetExpCycle (
   WitRun * const theWitRun,
   int *          lenLists,
   char * * *     partNameList,
   char * * *     operationNameList);

witReturnCode witGetExtOptActive (
   WitRun * const theWitRun,
   witBoolean *   extOptActive);

witReturnCode witGetExtOptIntVarIndices (
   WitRun * const theWitRun,
   int * *        indices,
   int *          len);

witReturnCode witGetExtOptLpProb (
   WitRun * const theWitRun,
   int *          numcols,
   int *          numrows,
   int * *        start,
   int * *        index,
   float * *      value,
   float * *      collb,
   float * *      colub,
   float * *      obj,
   float * *      rowlb,
   float * *      rowub);

witReturnCode witGetExtOptLpProbDbl (
   WitRun * const theWitRun,
   int *          numcols,
   int *          numrows,
   int * *        start,
   int * *        index,
   double * *     value,
   double * *     collb,
   double * *     colub,
   double * *     obj,
   double * *     rowlb,
   double * *     rowub);

witReturnCode witGetFeasible (
   WitRun * const theWitRun,
   witBoolean * feasible);

witReturnCode witGetFocusShortageVol (
   WitRun * const theWitRun,
   int *          lenList,
   char * * *     partList,
   float * * *    shortageVolList);

witReturnCode witGetFocusShortageVolDbl (
   WitRun * const theWitRun,
   int *          lenList,
   char * * *     partList,
   double * * *   shortageVolList);

witReturnCode witGetForcedMultiEq (
   WitRun * const theWitRun,
   witBoolean * forcedMultiEq);

witReturnCode witGetHeurAllocActive (
   WitRun * const theWitRun,
   witBoolean * heurAllocActive);

witReturnCode witGetHighPrecisionWD (
   WitRun * const theWitRun,
   witBoolean * highPrecisionWD);

witReturnCode witGetIndependentOffsets (
   WitRun * const theWitRun,
   int * independentOffsets);

witReturnCode witGetIntCplexParSpec (
   WitRun * const     theWitRun,
   const char * const theName,
   witBoolean *       intSpecExists,
   int *              theValue);

witReturnCode witGetLotSizeTol (
   WitRun * const theWitRun,
   float * lotSizeTol);

witReturnCode witGetLotSizeTolDbl (
   WitRun * const theWitRun,
   double * lotSizeTol);

witReturnCode witGetMesgFile (
   WitRun * const theWitRun,
   FILE * * mesgFile);

witReturnCode witGetMesgFileAccessMode (
   WitRun * const theWitRun,
   char * * mesgFileAccessMode);

witReturnCode witGetMesgFileName (
   WitRun * const theWitRun,
   char * * mesgFileName);

witReturnCode witGetMesgPrintNumber (
   WitRun * const theWitRun,
   const int messageNumber,
   witBoolean * mesgPrintMunber);

witReturnCode witGetMesgStopRunning (
   WitRun * const theWitRun,
   const int messageNumber,
   witBoolean * mesgStopRunning);

witReturnCode witGetMesgThrowErrorExc (
   WitRun * const theWitRun,
   const int messageNumber,
   witBoolean * mesgThrowErrorExc);

witReturnCode witGetMesgTimesPrint (
   WitRun * const theWitRun,
   const int messageNumber,
   int * mesgTimesPrint);

witReturnCode witGetMinimalExcess (
   WitRun * const theWitRun,
   witBoolean * minimalExcess);

witReturnCode witGetMipMode (
   WitRun * const theWitRun,
   witBoolean * mipMode);

witReturnCode witGetModHeurAlloc (
   WitRun * const theWitRun,
   witBoolean * modHeurAlloc);

witReturnCode witGetMultiExec (
   WitRun * const theWitRun,
   witBoolean * multiExec);

witReturnCode witGetMultiObjMode (
   WitRun * const theWitRun,
   witBoolean * multiObjMode);

witReturnCode witGetMultiObjTol (
   WitRun * const theWitRun,
   float * multiObjTol);

witReturnCode witGetMultiObjTolDbl (
   WitRun * const theWitRun,
   double * multiObjTol);

witReturnCode witGetMultiRoute (
   WitRun * const theWitRun,
   witBoolean * multiRoute);

witReturnCode witGetNPeriods (
   WitRun * const theWitRun,
   int * nPeriods);

witReturnCode witGetNScenarios (
   WitRun * const theWitRun,
   int * nScenarios);

witReturnCode witGetNstnResidual (
   WitRun * const theWitRun,
   witBoolean * nstnResidual);

witReturnCode witGetObjItrBomEntry (
   WitRun * const theWitRun,
   char * * consumingOperationName,
   int * bomEntryIndex);

witReturnCode witGetObjItrBopEntry (
   WitRun * const theWitRun,
   char * * producingOperationName,
   int * bopEntryIndex);

witReturnCode witGetObjItrDemand (
   WitRun * const theWitRun,
   char * * demandedPartName,
   char * * demandName);

witReturnCode witGetObjItrOperation (
   WitRun * const theWitRun,
   char * * operationName);

witReturnCode witGetObjItrPart (
   WitRun * const theWitRun,
   char * * partName);

witReturnCode witGetObjItrState (
   WitRun * const theWitRun,
   witAttr * objItrState);

witReturnCode witGetObjItrSubsBomEntry (
   WitRun * const theWitRun,
   char * * consumingOperationName,
   int * bomEntryIndex,
   int * subsBomEntryIndex);

witReturnCode witGetObjValue (
   WitRun * const theWitRun,
   float *        objValue);

witReturnCode witGetObjValueDbl (
   WitRun * const theWitRun,
   double *       objValue);

witReturnCode witGetObjectiveList (
   WitRun * const theWitRun,
   int  *         lenList,
   char * * *     objectiveList);

witReturnCode witGetObjectiveListSpec (
   WitRun * const theWitRun,
   char * *       objectiveListSpec);

witReturnCode witGetObjectiveSeqNo (
   WitRun * const theWitRun,
   int * objectiveSeqNo);

witReturnCode witGetOperationAppData (
   WitRun * const theWitRun,
   const char * const operationName,
   void * * appData);

witReturnCode witGetOperationCoExecVolPip (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         operationNameList,
   int * *            bopEntryIndexList,
   int * *            execPeriodList,
   float * *          peggedVolList);

witReturnCode witGetOperationCoExecVolPipDbl (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         operationNameList,
   int * *            bopEntryIndexList,
   int * *            execPeriodList,
   double * *         peggedVolList);

witReturnCode witGetOperationConsVolPip (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   float * *          peggedVolList);

witReturnCode witGetOperationConsVolPipDbl (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   double * *         peggedVolList);

witReturnCode witGetOperationExecBounds (
   WitRun * const theWitRun,
   const char * const operationName,
   float * * hardLower,
   float * * softLower,
   float * * hardUpper);

witReturnCode witGetOperationExecBoundsDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   double * * hardLower,
   double * * softLower,
   double * * hardUpper);

witReturnCode witGetOperationExecCost (
   WitRun * const theWitRun,
   const char * const operationName,
   float * * execCost);

witReturnCode witGetOperationExecCostDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   double * * execCost);

witReturnCode witGetOperationExecPenalty (
   WitRun * const theWitRun,
   const char * const operationName,
   float * execPenalty);

witReturnCode witGetOperationExecPenaltyDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   double * execPenalty);

witReturnCode witGetOperationExecSlbConIndex (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          thePer,
   int *              execSlbConIndex);

witReturnCode witGetOperationExecSlbvVarIndex (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          thePer,
   int *              execSlbvVarIndex);

witReturnCode witGetOperationExecVarIndex (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          thePer,
   int *              execVarIndex);

witReturnCode witGetOperationExecVol (
   WitRun * const theWitRun,
   const char * const operationName,
   float * * execVol);

witReturnCode witGetOperationExecVolDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   double * * execVol);

witReturnCode witGetOperationExecVolPip (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         operationNameList,
   int * *            execPeriodList,
   float * *          peggedExecVolList);

witReturnCode witGetOperationExecVolPipDbl (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         operationNameList,
   int * *            execPeriodList,
   double * *         peggedExecVolList);

witReturnCode witGetOperationExecutable (
   WitRun * const theWitRun,
   const char * const operationName,
   witBoolean * * executable);

witReturnCode witGetOperationExists (
   WitRun * const theWitRun,
   const char * const operationNameParm,
   witBoolean * exists);

witReturnCode witGetOperationFssExecVol (
   WitRun * const theWitRun,
   const char * const operationName,
   float * * fssExecVol);

witReturnCode witGetOperationFssExecVolDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   double * * fssExecVol);

witReturnCode witGetOperationIncLotSize (
   WitRun * const theWitRun,
   const char * const operationName,
   float * * incLotSize);

witReturnCode witGetOperationIncLotSizeDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   double * * incLotSize);

witReturnCode witGetOperationIncLotSize2 (
   WitRun * const theWitRun,
   const char * const operationName,
   float * * incLotSize2);

witReturnCode witGetOperationIncLotSize2Dbl (
   WitRun * const theWitRun,
   const char * const operationName,
   double * * incLotSize2);

witReturnCode witGetOperationIntExecVols (
   WitRun * const theWitRun,
   const char * const operationName,
   witBoolean * intExecVols);

witReturnCode witGetOperationLotSize2Thresh (
   WitRun * const theWitRun,
   const char * const operationName,
   float * * lotSize2Thresh);

witReturnCode witGetOperationLotSize2ThreshDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   double * * lotSize2Thresh);

witReturnCode witGetOperationMinLotSize (
   WitRun * const theWitRun,
   const char * const operationName,
   float * * minLotSize);

witReturnCode witGetOperationMinLotSizeDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   double * * minLotSize);

witReturnCode witGetOperationMinLotSize2 (
   WitRun * const theWitRun,
   const char * const operationName,
   float * * minLotSize2);

witReturnCode witGetOperationMinLotSize2Dbl (
   WitRun * const theWitRun,
   const char * const operationName,
   double * * minLotSize2);

witReturnCode witGetOperationMrpExecVol (
   WitRun * const theWitRun,
   const char * const operationName,
   float * * mrpExecVol);

witReturnCode witGetOperationMrpExecVolDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   double * * mrpExecVol);

witReturnCode witGetOperationNBomEntries (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   int * nBomEntries);

witReturnCode witGetOperationNBopEntries (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   int * nBopEntries);

witReturnCode witGetOperationObjectStage (
   WitRun * const     theWitRun,
   const char * const operationName,
   int *              objectStage);

witReturnCode witGetOperationPipEnabled (
   WitRun * const     theWitRun,
   const char * const operationName,
   witBoolean *       pipEnabled);

witReturnCode witGetOperationPipRank (
   WitRun * const     theWitRun,
   const char * const operationName,
   int *              pipRank);

witReturnCode witGetOperationProdVolPip (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   float * *          peggedVolList);

witReturnCode witGetOperationProdVolPipDbl (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   double * *         peggedVolList);

witReturnCode witGetOperationSelForDel (
   WitRun * const theWitRun,
   const char * const operationName,
   witBoolean * selForDel);

witReturnCode witGetOperations (
   WitRun * const theWitRun,
   int * lenOperationList,
   char * * * operationList);

witReturnCode witGetOperationSideVolPip (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   float * *          peggedVolList);

witReturnCode witGetOperationSideVolPipDbl (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   double * *         peggedVolList);

witReturnCode witGetOperationSubVolPip (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         operationNameList,
   int * *            bomEntryIndexList,
   int * *            subsBomEntryIndexList,
   int * *            execPeriodList,
   float * *          peggedSubVolList);

witReturnCode witGetOperationSubVolPipDbl (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         operationNameList,
   int * *            bomEntryIndexList,
   int * *            subsBomEntryIndexList,
   int * *            execPeriodList,
   double * *         peggedSubVolList);

witReturnCode witGetOperationSupplyVolPip (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   float * *          peggedVolList);

witReturnCode witGetOperationSupplyVolPipDbl (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          execPeriod,
   int *              lenLists,
   char * * *         partNameList,
   int * *            periodList,
   double * *         peggedVolList);

witReturnCode witGetOperationTwoLevelLotSizes (
   WitRun * const theWitRun,
   const char * const operationName,
   witBoolean * twoLevelLotSizes);

witReturnCode witGetOperationYieldRate (
   WitRun * const theWitRun,
   const char * const operationName,
   float * * yieldRate);

witReturnCode witGetOperationYieldRateDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   double * * yieldRate);

witReturnCode witGetOptInitMethod (
   WitRun * const theWitRun,
   witAttr * optInitMethod);

witReturnCode witGetOutputPrecision (
   WitRun * const theWitRun,
   int * outputPrecision);

witReturnCode witGetPartAppData (
   WitRun * const theWitRun,
   const char * const partName,
   void * * appData);

witReturnCode witGetPartAsapPipOrder (
   WitRun * const theWitRun,
   const char * const partName,
   witBoolean * asapPipOrder);

witReturnCode witGetPartBelowList (
   WitRun * const theWitRun,
   const char * const partName,
   int * lenList,
   char * * * partNameList);

witReturnCode witGetPartBoundedLeadTimes (
   WitRun * const theWitRun,
   const char * const partName,
   witBoolean * boundedLeadTimes);

witReturnCode witGetPartBuildAheadUB (
   WitRun * const theWitRun,
   const char * const partName,
   int * * buildAheadUB);

witReturnCode witGetPartBuildAsap (
   WitRun * const theWitRun,
   const char * const partName,
   witBoolean * buildAsap);

witReturnCode witGetPartBuildNstn (
   WitRun * const theWitRun,
   const char * const partName,
   witBoolean * buildNstn);

witReturnCode witGetPartCategory (
   WitRun * const theWitRun,
   const char * const partName,
   witAttr * category);

witReturnCode witGetPartConsVol (
   WitRun * const theWitRun,
   const char * const partName,
   float * * consVol);

witReturnCode witGetPartConsVolDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * consVol);

witReturnCode witGetPartConsumingBomEntry (
   WitRun * const theWitRun,
   const char * const partName,
   const int consIndex,
   char * * consumingOperationName,
   int * bomEntryIndex);

witReturnCode witGetPartConsumingSubsBomEntry (
   WitRun * const theWitRun,
   const char * const partName,
   const int consIndex,
   char * * consumingOperationName,
   int * bomEntryIndex,
   int * subsBomEntryIndex);

witReturnCode witGetPartDemands (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   int * lenDemandList,
   char * * * demandList);

witReturnCode witGetPartExcessVol (
   WitRun * const theWitRun,
   const char * const partName,
   float * * excessVol);

witReturnCode witGetPartExcessVolDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * excessVol);

witReturnCode witGetPartExists (
   WitRun * const theWitRun,
   const char * const partNameParm,
   witBoolean * exists);

witReturnCode witGetPartFocusShortageVol (
   WitRun * const theWitRun,
   const char * const partName,
   float * * shortageVol);

witReturnCode witGetPartFocusShortageVolDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * shortageVol);

witReturnCode witGetPartMrpConsVol (
   WitRun * const theWitRun,
   const char * const partName,
   float * * mrpConsVol);

witReturnCode witGetPartMrpConsVolDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * mrpConsVol);

witReturnCode witGetPartMrpExcessVol (
   WitRun * const theWitRun,
   const char * const partName,
   float * * mrpExcessVol);

witReturnCode witGetPartMrpExcessVolDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * mrpExcessVol);

witReturnCode witGetPartMrpResidualVol (
   WitRun * const theWitRun,
   const char * const partName,
   float * * mrpResidualVol);

witReturnCode witGetPartMrpResidualVolDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * mrpResidualVol);

witReturnCode witGetPartNConsumingBomEntries (
   WitRun * const theWitRun,
   const char * const partName,
   int * nConsumingBomEntries);

witReturnCode witGetPartNConsumingSubsBomEntries (
   WitRun * const theWitRun,
   const char * const partName,
   int * nConsumingSubsBomEntries);

witReturnCode witGetPartNProducingBopEntries (
   WitRun * const theWitRun,
   const char * const partName,
   int * nProducingBopEntries);

witReturnCode witGetPartObjectStage (
   WitRun * const     theWitRun,
   const char * const partName,
   int *              objectStage);

witReturnCode witGetPartProdVol (
   WitRun * const theWitRun,
   const char * const partName,
   float * * prodVol);

witReturnCode witGetPartProdVolDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * prodVol);

witReturnCode witGetPartProducingBopEntry (
   WitRun * const theWitRun,
   const char * const partName,
   const int prodIndex,
   char * * producingOperationName,
   int * bopEntryIndex);

witReturnCode witGetPartPropRtg (
   WitRun * const     theWitRun,
   const char * const partName,
   witBoolean * *     propRtg);

witReturnCode witGetPartReqVol (
   WitRun * const theWitRun,
   const char * const partName,
   float * * reqVol);

witReturnCode witGetPartReqVolDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * reqVol);

witReturnCode witGetPartResidualVol (
   WitRun * const theWitRun,
   const char * const partName,
   float * * residualVol);

witReturnCode witGetPartResidualVolDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * residualVol);

witReturnCode witGetPartResourceConIndex (
   WitRun * const     theWitRun,
   const char * const partName,
   const int          thePer,
   int *              resourceConIndex);

witReturnCode witGetPartScrapAllowed (
   WitRun * const theWitRun,
   const char * const partName,
   witBoolean * scrapAllowed);

witReturnCode witGetPartScrapCost (
   WitRun * const theWitRun,
   const char * const partName,
   float * * scrapCost);

witReturnCode witGetPartScrapCostDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * scrapCost);

witReturnCode witGetPartScrapVarIndex (
   WitRun * const     theWitRun,
   const char * const partName,
   const int          thePer,
   int *              scrapVarIndex);

witReturnCode witGetPartScrapVol (
   WitRun * const theWitRun,
   const char * const partName,
   float * * scrapVol);

witReturnCode witGetPartScrapVolDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * scrapVol);

witReturnCode witGetPartSelForDel (
   WitRun * const theWitRun,
   const char * const partName,
   witBoolean * selForDel);

witReturnCode witGetPartShadowPrice (
   WitRun * const theWitRun,
   const char * const partName,
   float * * shadowPrice);

witReturnCode witGetPartShadowPriceDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * shadowPrice);

witReturnCode witGetPartSingleSource (
   WitRun * const theWitRun,
   const char * const partName,
   witBoolean * singleSource);

witReturnCode witGetPartStockBounds (
   WitRun * const theWitRun,
   const char * const partName,
   float * * hardLower,
   float * * softLower,
   float * * hardUpper);

witReturnCode witGetPartStockBoundsDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * hardLower,
   double * * softLower,
   double * * hardUpper);

witReturnCode witGetPartStockCost (
   WitRun * const theWitRun,
   const char * const partName,
   float * * stockCost);

witReturnCode witGetPartStockCostDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * stockCost);

witReturnCode witGetPartStockSlbConIndex (
   WitRun * const     theWitRun,
   const char * const partName,
   const int          thePer,
   int *              stockSlbConIndex);

witReturnCode witGetPartStockSlbvVarIndex (
   WitRun * const     theWitRun,
   const char * const partName,
   const int          thePer,
   int *              stockSlbvVarIndex);

witReturnCode witGetPartStockVarIndex (
   WitRun * const     theWitRun,
   const char * const partName,
   const int          thePer,
   int *              stockVarIndex);

witReturnCode witGetPartStockVol (
   WitRun * const theWitRun,
   const char * const partName,
   float * * stockVol);

witReturnCode witGetPartStockVolDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * stockVol);

witReturnCode witGetPartSupplyVol (
   WitRun * const theWitRun,
   const char * const partName,
   float * * supplyVol);

witReturnCode witGetPartSupplyVolDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * supplyVol);

witReturnCode witGetParts (
   WitRun * const theWitRun,
   int * lenPartList,
   char * * * partList);

witReturnCode witGetPenExec (
   WitRun * const theWitRun,
   witBoolean * penExec);

witReturnCode witGetPerfPegging (
   WitRun * const theWitRun,
   witBoolean * perfPegging);

witReturnCode witGetPeriodStage (
   WitRun * const theWitRun,
   int * *        periodStage);

witReturnCode witGetPgdCritList (
   WitRun * const theWitRun,
   int *          lenLists,
   char * * *     critPartNameList,
   int * *        critPerList,
   char * * *     demPartNameList,
   char * * *     demandNameList,
   int * *        shipPerList);

witReturnCode witGetPgdCritListMode (
   WitRun * const theWitRun,
   witBoolean * pgdCritListMode);

witReturnCode witGetPipExists (
   WitRun * const theWitRun,
   witBoolean *   pipExists);

witReturnCode witGetPipSeq (
   WitRun * const theWitRun,
   int *          lenLists,
   char * * *     partNameList,
   char * * *     demandNameList,
   int * *        shipPerList,
   float * *      incShipVolList);

witReturnCode witGetPipSeqDbl (
   WitRun * const theWitRun,
   int *          lenLists,
   char * * *     partNameList,
   char * * *     demandNameList,
   int * *        shipPerList,
   double * *     incShipVolList);

witReturnCode witGetPipSeqFromHeur (
   WitRun * const theWitRun,
   witBoolean * pipSeqFromHeur);

witReturnCode witGetPostprocessed (
   WitRun * const theWitRun,
   witBoolean * postprocessed);

witReturnCode witGetPrefHighStockSLBs (
   WitRun * const theWitRun,
   witBoolean * prefHighStockSLBs);

witReturnCode witGetPreprocessed (
   WitRun * const theWitRun,
   witBoolean * preprocessed);

witReturnCode witGetProbability (
   WitRun * const theWitRun,
   float * probability);

witReturnCode witGetProbabilityDbl (
   WitRun * const theWitRun,
   double * probability);

witReturnCode witGetRespectStockSLBs (
   WitRun * const theWitRun,
   witBoolean * respectStockSLBs);

witReturnCode witGetRoundReqVols (
   WitRun * const theWitRun,
   witBoolean * roundReqVols);

witReturnCode witGetSelSplit (
   WitRun * const theWitRun,
   witBoolean * selSplit);

witReturnCode witGetSelectionRecovery (
   WitRun * const theWitRun,
   witBoolean * selectionRecovery);

witReturnCode witGetSkipFailures (
   WitRun * const theWitRun,
   witBoolean * skipFailures);

witReturnCode witGetSolverLogFileName (
   WitRun * const theWitRun,
   char * * solverLogFileName);

witReturnCode witGetStageByObject (
   WitRun * const theWitRun,
   witBoolean *   stageByObject);

witReturnCode witGetStochMode (
   WitRun * const theWitRun,
   witBoolean *   stochMode);

witReturnCode witGetStochSolnMode (
   WitRun * const theWitRun,
   witBoolean *   stochSolnMode);

witReturnCode witGetStockReallocation (
   WitRun * const theWitRun,
   witBoolean * stockReallocation);

witReturnCode witGetSubsBomEntryAppData (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   void * * appData);

witReturnCode witGetSubsBomEntryConsRate (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   float * * consRate);

witReturnCode witGetSubsBomEntryConsRateDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   double * * consRate);

witReturnCode witGetSubsBomEntryConsumedPart (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   char  * * consumedPart);

witReturnCode witGetSubsBomEntryEarliestPeriod (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   int * earliestPeriod);

witReturnCode witGetSubsBomEntryExecPenalty (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   float * execPenalty);

witReturnCode witGetSubsBomEntryExecPenaltyDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   double * execPenalty);

witReturnCode witGetSubsBomEntryExpAllowed (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   witBoolean * expAllowed);

witReturnCode witGetSubsBomEntryExpNetAversion (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   float * expNetAversion);

witReturnCode witGetSubsBomEntryExpNetAversionDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   double * expNetAversion);

witReturnCode witGetSubsBomEntryFalloutRate (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   float * falloutRate);

witReturnCode witGetSubsBomEntryFalloutRateDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   double * falloutRate);

witReturnCode witGetSubsBomEntryFssSubVol (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   float * * fssSubVol);

witReturnCode witGetSubsBomEntryFssSubVolDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   double * * fssSubVol);

witReturnCode witGetSubsBomEntryImpactPeriod (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   int * * impactPeriod);

witReturnCode witGetSubsBomEntryIntSubVols (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   witBoolean * intSubVols);

witReturnCode witGetSubsBomEntryLatestPeriod (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   int * latestPeriod);

witReturnCode witGetSubsBomEntryMrpNetAllowed (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   witBoolean * mrpNetAllowed);

witReturnCode witGetSubsBomEntryMrpSubVol (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   float * * mrpSubVol);

witReturnCode witGetSubsBomEntryMrpSubVolDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   double * * mrpSubVol);

witReturnCode witGetSubsBomEntryNetAllowed (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   witBoolean * netAllowed);

witReturnCode witGetSubsBomEntryOffset (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   float * * offset);

witReturnCode witGetSubsBomEntryOffsetDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   double * * offset);

witReturnCode witGetSubsBomEntryRoutingShare (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   float * * routingShare);

witReturnCode witGetSubsBomEntryRoutingShareDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   double * * routingShare);

witReturnCode witGetSubsBomEntrySelForDel (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   witBoolean * selForDel);

witReturnCode witGetSubsBomEntrySubCost (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   float * * subCost);

witReturnCode witGetSubsBomEntrySubCostDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   double * * subCost);

witReturnCode witGetSubsBomEntrySubVarIndex (
   WitRun * const     theWitRun,
   const char * const consumingOperationName,
   const int          bomEntryIndex,
   const int          subsBomEntryIndex,
   const int          thePer,
   int *              subVarIndex);

witReturnCode witGetSubsBomEntrySubVol (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   float * * subVol);

witReturnCode witGetSubsBomEntrySubVolDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   double * * subVol);

witReturnCode witGetTieBreakPropRt (
   WitRun * const theWitRun,
   witBoolean * tieBreakPropRt);

witReturnCode witGetTitle (
   WitRun * const theWitRun,
   char * * title);

witReturnCode witGetTruncOffsets (
   WitRun * const theWitRun,
   witBoolean * truncOffsets);

witReturnCode witGetTwoWayMultiExec (
   WitRun * const theWitRun,
   witBoolean * twoWayMultiExec);

witReturnCode witGetUseFocusHorizons (
   WitRun * const theWitRun,
   witBoolean * focusHorizons);

witReturnCode witGetUserHeurStart (
   WitRun * const theWitRun,
   witBoolean * userHeurStart);

witReturnCode witGetWbounds (
   WitRun * const theWitRun,
   float * wbounds);

witReturnCode witGetWboundsDbl (
   WitRun * const theWitRun,
   double * wbounds);

witReturnCode witGetWit34Compatible (
   WitRun * const theWitRun,
   witBoolean * wit34Compatible);

witReturnCode witHeurImplode (
   WitRun * const theWitRun);

witReturnCode witIncHeurAlloc (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int shipPeriod,
   const float desIncVol,
   float * incVol);

witReturnCode witIncHeurAllocDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int shipPeriod,
   const double desIncVol,
   double * incVol);

witReturnCode witIncHeurAllocTwme (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int shipPeriod,
   const float desIncVol,
   float * incVol,
   witBoolean asapMultiExec);

witReturnCode witIncHeurAllocTwmeDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int shipPeriod,
   const double desIncVol,
   double * incVol,
   witBoolean asapMultiExec);

witReturnCode witInitialize (
   WitRun * const theWitRun);

witReturnCode witMrp (
   WitRun * const theWitRun);

witReturnCode witNewRun (
   WitRun * * theWitRunPtr);

witReturnCode witOptImplode (
   WitRun * const theWitRun);

witReturnCode witPostprocess (
   WitRun * const theWitRun);

witReturnCode witPreprocess (
   WitRun * const theWitRun);

witReturnCode witPurgeData (
   WitRun * const theWitRun);

witReturnCode witReadData (
   WitRun * const theWitRun,
   const char * const mesgFileName);

witReturnCode witResetObjItr (
   WitRun * const theWitRun);

witReturnCode witSetAccAfterSoftLB (
   WitRun * const theWitRun,
   const witBoolean accAfterSoftLB);

witReturnCode witSetAccAfterOptImp (
   WitRun * const theWitRun,
   const witBoolean accAfterOptImp);

witReturnCode witSetAppData (
   WitRun * const theWitRun,
   void * const appData);

witReturnCode witSetAutoPriority (
   WitRun * const theWitRun,
   const witBoolean autoPriority);

witReturnCode witSetBomEntryAppData (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   void * const appData);

witReturnCode witSetBomEntryConsRate (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const float * const consRate);

witReturnCode witSetBomEntryConsRateDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const double * const consRate);

witReturnCode witSetBomEntryEarliestPeriod (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int earliestPeriod);

witReturnCode witSetBomEntryExecPenalty (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const float execPenalty);

witReturnCode witSetBomEntryExecPenaltyDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const double execPenalty);

witReturnCode witSetBomEntryFalloutRate (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const float falloutRate);

witReturnCode witSetBomEntryFalloutRateDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const double falloutRate);

witReturnCode witSetBomEntryLatestPeriod (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int latestPeriod);

witReturnCode witSetBomEntryMandEC (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const witBoolean mandEC);

witReturnCode witSetBomEntryOffset (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const float * const offset);

witReturnCode witSetBomEntryOffsetDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const double * const offset);

witReturnCode witSetBomEntryPropRtg (
   WitRun * const           theWitRun,
   const char * const       operationName,
   const int                bomEntryIndex,
   const witBoolean * const propRtg);

witReturnCode witSetBomEntryRoutingShare (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const float * routingShare);

witReturnCode witSetBomEntryRoutingShareDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const double * routingShare);

witReturnCode witSetBomEntrySelForDel (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const witBoolean selForDel);

witReturnCode witSetBomEntrySingleSource (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const witBoolean singleSource);

witReturnCode witSetBopEntryAppData (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   void * const appData);

witReturnCode witSetBopEntryEarliestPeriod (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const int earliestPeriod);

witReturnCode witSetBopEntryExpAllowed (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const witBoolean expAllowed);

witReturnCode witSetBopEntryExpAversion (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const float expAversion);

witReturnCode witSetBopEntryExpAversionDbl (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const double expAversion);

witReturnCode witSetBopEntryLatestPeriod (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const int latestPeriod);

witReturnCode witSetBopEntryOffset (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const float * const offset);

witReturnCode witSetBopEntryOffsetDbl (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const double * const offset);

witReturnCode witSetBopEntryProductRate (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const float * const productRate);

witReturnCode witSetBopEntryProductRateDbl (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const double * const productRate);

witReturnCode witSetBopEntryRoutingShare (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const float * routingShare);

witReturnCode witSetBopEntryRoutingShareDbl (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const double * routingShare);

witReturnCode witSetBopEntrySelForDel (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const witBoolean selForDel);

witReturnCode witSetCompPrices (
   WitRun * const theWitRun,
   const witBoolean compPrices);

witReturnCode witSetComputeCriticalList (
   WitRun * const theWitRun,
   const witBoolean computeCriticalList);

witReturnCode witSetCplexParSpecDblVal (
   WitRun * const theWitRun,
   const float cplexParSpecDblVal);

witReturnCode witSetCplexParSpecDblValDbl (
   WitRun * const theWitRun,
   const double cplexParSpecDblVal);

witReturnCode witSetCplexParSpecIntVal (
   WitRun * const theWitRun,
   const int      cplexParSpecIntVal);

witReturnCode witSetCplexParSpecName (
   WitRun * const theWitRun,
   const char * const cplexParSpecName);

witReturnCode witSetCurrentObjective (
   WitRun * const theWitRun,
   const char * const theObjName);

witReturnCode witSetCurrentScenario (
   WitRun * const theWitRun,
   const int currentScenario);

witReturnCode witSetDemandAppData (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   void * const appData);

witReturnCode witSetDemandCumShipBounds (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const float * const hardLower,
   const float * const softLower,
   const float * const hardUpper);

witReturnCode witSetDemandCumShipBoundsDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const double * const hardLower,
   const double * const softLower,
   const double * const hardUpper);

witReturnCode witSetDemandCumShipReward (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const float * const cumShipReward);

witReturnCode witSetDemandCumShipRewardDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const double * const cumShipReward);

witReturnCode witSetDemandDemandName (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const demandName,
   const char * const newName);

witReturnCode witSetDemandDemandVol (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const float * const iDemandVol);

witReturnCode witSetDemandDemandVolDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const double * const iDemandVol);

witReturnCode witSetDemandFocusHorizon (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int  focusHorizon);

witReturnCode witSetDemandFssShipVol (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const float * const  fssShipVol);

witReturnCode witSetDemandFssShipVolDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const double * const  fssShipVol);

witReturnCode witSetDemandIntShipVols (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const witBoolean intShipVols);

witReturnCode witSetDemandLeadTimeUB (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int * leadTimeUB);

witReturnCode witSetDemandPriority (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int * const priority);

witReturnCode witSetDemandSearchInc (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const float searchInc);

witReturnCode witSetDemandSearchIncDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const double searchInc);

witReturnCode witSetDemandSelForDel (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const witBoolean selForDel);

witReturnCode witSetDemandShipLateAllowed (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const witBoolean shipLateAllowed);

witReturnCode witSetDemandShipLateUB (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int * shipLateUB);

witReturnCode witSetDemandShipReward (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const float * const shipReward);

witReturnCode witSetDemandShipRewardDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const double * const shipReward);

witReturnCode witSetDemandShipVol (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const float * const shipVol);

witReturnCode witSetDemandShipVolDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const double * const shipVol);

witReturnCode witSetEquitability (
   WitRun * const theWitRun,
   const int equitablity);

witReturnCode witSetExecEmptyBom (
   WitRun * const theWitRun,
   const witBoolean execEmptyBom);

witReturnCode witSetExpCutoff (
   WitRun * const theWitRun,
   const float expCutoff);

witReturnCode witSetExpCutoffDbl (
   WitRun * const theWitRun,
   const double expCutoff);

witReturnCode witSetExtOptSoln (
   WitRun * const      theWitRun,
   const float * const colsol);

witReturnCode witSetExtOptSolnDbl (
   WitRun * const       theWitRun,
   const double * const colsol);

witReturnCode witSetForcedMultiEq (
   WitRun * const theWitRun,
   const witBoolean forcedMultiEq);

witReturnCode witSetHighPrecisionWD (
   WitRun * const theWitRun,
   const witBoolean highPrecisionWD);

witReturnCode witSetIndependentOffsets (
   WitRun * const theWitRun,
   const witBoolean independentOffsets);

witReturnCode witSetLotSizeTol (
   WitRun * const theWitRun,
   const float lotSizeTol);

witReturnCode witSetLotSizeTolDbl (
   WitRun * const theWitRun,
   const double lotSizeTol);

witReturnCode witSetMesgFileAccessMode (
   WitRun * const theWitRun,
   const witBoolean quiet,
   const char * const mesgFileAccessMode);

witReturnCode witSetMesgFileName (
   WitRun * const theWitRun,
   const witBoolean quiet,
   const char * const fileName);

witReturnCode witSetMesgPrintNumber (
   WitRun * const theWitRun,
   const witBoolean quiet,
   const int messageNumber,
   const witBoolean mesgPrintMunber);

witReturnCode witSetMesgStopRunning (
   WitRun * const theWitRun,
   const witBoolean quiet,
   const int messageNumber,
   const witBoolean mesgStopRunning);

witReturnCode witSetMesgThrowErrorExc (
   WitRun * const theWitRun,
   const witBoolean quiet,
   const int messageNumber,
   const witBoolean mesgThrowErrorExc);

witReturnCode witSetMesgTimesPrint (
   WitRun * const theWitRun,
   const witBoolean quiet,
   const int messageNumber,
   const int mesgTimesPrint);

witReturnCode witSetMinimalExcess (
   WitRun * const theWitRun,
   const witBoolean minimalExcess);

witReturnCode witSetMipMode (
   WitRun * const theWitRun,
   const witBoolean mipMode);

witReturnCode witSetModHeurAlloc (
   WitRun * const theWitRun,
   const witBoolean modHeurAlloc);

witReturnCode witSetMultiExec (
   WitRun * const theWitRun,
   const witBoolean multiExec);

witReturnCode witSetMultiObjMode (
   WitRun * const theWitRun,
   const witBoolean multiObjMode);

witReturnCode witSetMultiObjTol (
   WitRun * const theWitRun,
   const float multiObjTol);

witReturnCode witSetMultiObjTolDbl (
   WitRun * const theWitRun,
   const double multiObjTol);

witReturnCode witSetMultiRoute (
   WitRun * const theWitRun,
   const witBoolean multiRoute);

witReturnCode witSetNPeriods (
   WitRun * const theWitRun,
   const int nPeriods);

witReturnCode witSetNScenarios (
   WitRun * const theWitRun,
   const int nScenarios);

witReturnCode witSetNstnResidual (
   WitRun * const theWitRun,
   const witBoolean nstnResidual);

witReturnCode witSetObjectiveList (
   WitRun * const             theWitRun,
   const int                  lenList,
   const char * const * const objectiveList);

witReturnCode witSetObjectiveListSpec (
   WitRun * const     theWitRun,
   const char * const objectiveListSpec);

witReturnCode witSetObjectiveSeqNo (
   WitRun * const theWitRun,
   const int      objectiveSeqNo);

witReturnCode witSetOperationAppData (
   WitRun * const theWitRun,
   const char * const operationName,
   void * const appData);

witReturnCode witSetOperationExecBounds (
   WitRun * const theWitRun,
   const char * const operationName,
   const float * const hardLower,
   const float * const softLower,
   const float * const hardUpper);

witReturnCode witSetOperationExecBoundsDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   const double * const hardLower,
   const double * const softLower,
   const double * const hardUpper);

witReturnCode witSetOperationExecCost (
   WitRun * const theWitRun,
   const char * const operationName,
   const float * const execCost);

witReturnCode witSetOperationExecCostDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   const double * const execCost);

witReturnCode witSetOperationExecPenalty (
   WitRun * const theWitRun,
   const char * const operationName,
   const float execPenalty);

witReturnCode witSetOperationExecPenaltyDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   const double execPenalty);

witReturnCode witSetOperationExecVol (
   WitRun * const theWitRun,
   const char * const operationName,
   const float * const execVol);

witReturnCode witSetOperationExecVolDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   const double * const execVol);

witReturnCode witSetOperationIncLotSize (
   WitRun * const theWitRun,
   const char * const operationName,
   const float * const incLotSize);

witReturnCode witSetOperationIncLotSizeDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   const double * const incLotSize);

witReturnCode witSetOperationIncLotSize2 (
   WitRun * const theWitRun,
   const char * const operationName,
   const float * const incLotSize2);

witReturnCode witSetOperationIncLotSize2Dbl (
   WitRun * const theWitRun,
   const char * const operationName,
   const double * const incLotSize2);

witReturnCode witSetOperationIntExecVols (
   WitRun * const theWitRun,
   const char * const operationName,
   const witBoolean intExecVols);

witReturnCode witSetOperationLotSize2Thresh (
   WitRun * const theWitRun,
   const char * const operationName,
   const float * const lotSize2Thresh);

witReturnCode witSetOperationLotSize2ThreshDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   const double * const lotSize2Thresh);

witReturnCode witSetOperationMinLotSize (
   WitRun * const theWitRun,
   const char * const operationName,
   const float * const minLotSize);

witReturnCode witSetOperationMinLotSizeDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   const double * const minLotSize);

witReturnCode witSetOperationMinLotSize2 (
   WitRun * const theWitRun,
   const char * const operationName,
   const float * const minLotSize2);

witReturnCode witSetOperationMinLotSize2Dbl (
   WitRun * const theWitRun,
   const char * const operationName,
   const double * const minLotSize2);

witReturnCode witSetOperationObjectStage (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          objectStage);

witReturnCode witSetOperationOperationName (
   WitRun * const     theWitRun,
   const char * const operationName,
   const char * const newName);

witReturnCode witSetOperationPipEnabled (
   WitRun * const     theWitRun,
   const char * const operationName,
   const witBoolean   pipEnabled);

witReturnCode witSetOperationPipRank (
   WitRun * const     theWitRun,
   const char * const operationName,
   const int          pipRank);

witReturnCode witSetOperationSelForDel (
   WitRun * const theWitRun,
   const char * const operationName,
   const witBoolean selForDel);

witReturnCode witSetOperationTwoLevelLotSizes (
   WitRun * const theWitRun,
   const char * const operationName,
   const witBoolean twoLevelLotSizes);

witReturnCode witSetOperationYieldRate (
   WitRun * const theWitRun,
   const char * const operationName,
   const float * const yieldRate);

witReturnCode witSetOperationYieldRateDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   const double * const yieldRate);

witReturnCode witSetOptInitMethod (
   WitRun * const theWitRun,
   const witAttr optInitMethod);

witReturnCode witSetOutputPrecision (
   WitRun * const theWitRun,
   const int outputPrecision);

witReturnCode witSetPartAppData (
   WitRun * const theWitRun,
   const char * const partName,
   void * const appData);

witReturnCode witSetPartAsapPipOrder (
   WitRun * const theWitRun,
   const char * const partName,
   const witBoolean asapPipOrder);

witReturnCode witSetPartBoundedLeadTimes (
   WitRun * const theWitRun,
   const char * const partName,
   const witBoolean boundedLeadTimes);

witReturnCode witSetPartBuildAheadUB (
   WitRun * const theWitRun,
   const char * const partName,
   const int * const buildAheadUB);

witReturnCode witSetPartBuildAsap (
   WitRun * const theWitRun,
   const char * const partName,
   const witBoolean buildAsap);

witReturnCode witSetPartBuildNstn (
   WitRun * const theWitRun,
   const char * const partName,
   const witBoolean buildNstn);

witReturnCode witSetPartObjectStage (
   WitRun * const     theWitRun,
   const char * const partName,
   const int          objectStage);

witReturnCode witSetPartPartName (
   WitRun * const     theWitRun,
   const char * const partName,
   const char * const newName);

witReturnCode witSetPartPropRtg (
   WitRun * const           theWitRun,
   const char * const       partName,
   const witBoolean * const propRtg);

witReturnCode witSetPartScrapAllowed (
   WitRun * const theWitRun,
   const char * const partName,
   const witBoolean scrapAllowed);

witReturnCode witSetPartScrapCost (
   WitRun * const theWitRun,
   const char * const partName,
   const float * const scrapCost);

witReturnCode witSetPartScrapCostDbl (
   WitRun * const theWitRun,
   const char * const partName,
   const double * const scrapCost);

witReturnCode witSetPartSelForDel (
   WitRun * const theWitRun,
   const char * const partName,
   const witBoolean selForDel);

witReturnCode witSetPartSingleSource (
   WitRun * const theWitRun,
   const char * const partName,
   const witBoolean singleSource);

witReturnCode witSetPartStockBounds (
   WitRun * const theWitRun,
   const char * const partName,
   const float * const hardLower,
   const float * const softLower,
   const float * const hardUpper);

witReturnCode witSetPartStockBoundsDbl (
   WitRun * const theWitRun,
   const char * const partName,
   const double * const hardLower,
   const double * const softLower,
   const double * const hardUpper);

witReturnCode witSetPartStockCost (
   WitRun * const theWitRun,
   const char * const partName,
   const float * const stockCost);

witReturnCode witSetPartStockCostDbl (
   WitRun * const theWitRun,
   const char * const partName,
   const double * const stockCost);

witReturnCode witSetPartSupplyVol (
   WitRun * const theWitRun,
   const char * const partName,
   const float * const supplyVol);

witReturnCode witSetPartSupplyVolDbl (
   WitRun * const theWitRun,
   const char * const partName,
   const double * const supplyVol);

witReturnCode witSetPenExec (
   WitRun * const theWitRun,
   const witBoolean penExec);

witReturnCode witSetPerfPegging (
   WitRun * const theWitRun,
   const witBoolean perfPegging);

witReturnCode witSetPeriodStage (
   WitRun * const theWitRun,
   const int *    periodStage);

witReturnCode witSetPgdCritListMode (
   WitRun * const theWitRun,
   const witBoolean pgdCritListMode);

witReturnCode witSetPipSeqFromHeur (
   WitRun * const theWitRun,
   const witBoolean pipSeqFromHeur);

witReturnCode witSetPrefHighStockSLBs (
   WitRun * const theWitRun,
   const witBoolean prefHighStockSLBs);

witReturnCode witSetProbability (
   WitRun * const theWitRun,
   const float probability);

witReturnCode witSetProbabilityDbl (
   WitRun * const theWitRun,
   const double probability);

witReturnCode witSetRespectStockSLBs (
   WitRun * const theWitRun,
   const witBoolean respectStockSLBs);

witReturnCode witSetRoundReqVols (
   WitRun * const theWitRun,
   const witBoolean roundReqVols);

witReturnCode witSetSelSplit (
   WitRun * const theWitRun,
   const witBoolean selSplit);

witReturnCode witSetSelectionRecovery (
   WitRun * const theWitRun,
   const witBoolean selectionRecovery);

witReturnCode witSetSkipFailures (
   WitRun * const theWitRun,
   const witBoolean skipFailures);

witReturnCode witSetSolverLogFileName (
   WitRun * const theWitRun,
   const char * const solverLogFileName);

witReturnCode witSetStageByObject (
   WitRun * const   theWitRun,
   const witBoolean stageByObject);

witReturnCode witSetStochMode (
   WitRun * const   theWitRun,
   const witBoolean stochMode);

witReturnCode witSetStockReallocation (
   WitRun * const theWitRun,
   const witBoolean stockReallocation);

witReturnCode witSetSubsBomEntryAppData (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   void * const appData);

witReturnCode witSetSubsBomEntryConsRate (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const float * const consRate);

witReturnCode witSetSubsBomEntryConsRateDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const double * const consRate);

witReturnCode witSetSubsBomEntryEarliestPeriod (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const int earliestPeriod);

witReturnCode witSetSubsBomEntryExecPenalty (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const float execPenalty);

witReturnCode witSetSubsBomEntryExecPenaltyDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const double execPenalty);

witReturnCode witSetSubsBomEntryExpAllowed (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const witBoolean expAllowed);

witReturnCode witSetSubsBomEntryExpNetAversion (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const float expNetAversion);

witReturnCode witSetSubsBomEntryExpNetAversionDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const double expNetAversion);

witReturnCode witSetSubsBomEntryFalloutRate (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const float falloutRate);

witReturnCode witSetSubsBomEntryFalloutRateDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const double falloutRate);

witReturnCode witSetSubsBomEntryIntSubVols (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const witBoolean intSubVols);

witReturnCode witSetSubsBomEntryLatestPeriod (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const int latestPeriod);

witReturnCode witSetSubsBomEntryMrpNetAllowed (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const witBoolean mrpNetAllowed);

witReturnCode witSetSubsBomEntryNetAllowed (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const witBoolean netAllowed);

witReturnCode witSetSubsBomEntryOffset (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const float * const offset);

witReturnCode witSetSubsBomEntryOffsetDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const double * const offset);

witReturnCode witSetSubsBomEntryRoutingShare (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const float * routingShare);

witReturnCode witSetSubsBomEntryRoutingShareDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const double * routingShare);

witReturnCode witSetSubsBomEntrySelForDel (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const witBoolean selForDel);

witReturnCode witSetSubsBomEntrySubCost (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const float * const subCost);

witReturnCode witSetSubsBomEntrySubCostDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const double * const subCost);

witReturnCode witSetSubsBomEntrySubVol (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const float * const subVol);

witReturnCode witSetSubsBomEntrySubVolDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const double * const subVol);

witReturnCode witSetTieBreakPropRt (
   WitRun * const theWitRun,
   const witBoolean tieBreakPropRt);

witReturnCode witSetTitle (
   WitRun * const theWitRun,
   const char * const title);

witReturnCode witSetTruncOffsets (
   WitRun * const theWitRun,
   const witBoolean truncOffsets);

witReturnCode witSetTwoWayMultiExec (
   WitRun * const theWitRun,
   const witBoolean twoWayMultiExec);

witReturnCode witSetUseFocusHorizons (
   WitRun * const theWitRun,
   const witBoolean useFocusHorizons);

witReturnCode witSetUserHeurStart (
   WitRun * const theWitRun,
   const witBoolean userHeurStart);

witReturnCode witSetWbounds (
   WitRun * const theWitRun,
   const float wbounds);

witReturnCode witSetWboundsDbl (
   WitRun * const theWitRun,
   const double wbounds);

witReturnCode witSetWit34Compatible (
   WitRun * const theWitRun,
   const witBoolean wit34Compatible);

witReturnCode witShutDownExtOpt (
   WitRun * const theWitRun);

witReturnCode witShutDownHeurAlloc (
   WitRun * const theWitRun);

witReturnCode witStartExtOpt (
   WitRun * const theWitRun);

witReturnCode witStartHeurAlloc (
   WitRun * const theWitRun);

witReturnCode witStochImplode (
   WitRun * const theWitRun);

witReturnCode witWriteCriticalList (
   WitRun * const theWitRun,
   const char * const fileName,
   const witFileFormat fileFormat,
   const int maxListLen);

witReturnCode witWriteData (
   WitRun * const theWitRun,
   const char * const fileName);

witReturnCode witWriteExecSched (
   WitRun * const theWitRun,
   const char * const fileName,
   const witFileFormat fileFormat);

witReturnCode witWriteReqSched (
   WitRun * const theWitRun,
   const char * const fileName,
   const witFileFormat fileFormat);

witReturnCode witWriteShipSched (
   WitRun * const theWitRun,
   const char * const fileName,
   const witFileFormat fileFormat);

/*----------------------------------------------------------------------------*/
/* The following functions that were formerly part of WIT's documented API,   */
/* but have since been removed from the documentation.                        */
/* They are included here for upward compatibility.                           */
/*----------------------------------------------------------------------------*/

witReturnCode witGetBomEntryFallout (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   int * fallout);

witReturnCode witGetBomEntryPropRouting (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   witBoolean * propRouting);

witReturnCode witGetBomEntryUsageRate (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   float * usageRate);

witReturnCode witGetBomEntryUsageTime (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   float * usageTime);

witReturnCode witGetBopEntryByProduct (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   witBoolean * byProduct);

witReturnCode witGetBopEntryProdRate (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   float * prodRate);

witReturnCode witGetBuildWhenEmpty (
   WitRun * const theWitRun,
   witBoolean * buildWhenEmpty);

witReturnCode witGetDemandBuildAheadLimit (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   int * buildAheadLimit);

witReturnCode witGetDemandBuildAheadUB (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   int * * buildAheadUB);

witReturnCode witGetDemandObj1CumShipReward (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   float * * obj1CumShipReward);

witReturnCode witGetDemandObj1CumShipRewardDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   double * * obj1CumShipReward);

witReturnCode witGetDemandObj1ShipReward (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   float * * obj1ShipReward);

witReturnCode witGetDemandObj1ShipRewardDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   double * * obj1ShipReward);

witReturnCode witGetDemandPrefBuildAhead (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   witBoolean * prefBuildAhead);

witReturnCode witGetDemandShipLateLimit (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   int * shipLateLimit);

witReturnCode witGetHashTableSize (
   WitRun * const theWitRun,
   int * hashTableSize);

witReturnCode witGetLocalBuildAhead (
   WitRun * const theWitRun,
   witBoolean * localBuildAhead);

witReturnCode witGetLocalMultiExec (
   WitRun * const theWitRun,
   witBoolean * localMultiExec);

witReturnCode witGetObjChoice (
   WitRun * const theWitRun,
   int * objChoice);

witReturnCode witGetObjValues (
   WitRun * const theWitRun,
   float * objValue,
   float * boundsValue);

witReturnCode witGetObjValuesDbl (
   WitRun * const theWitRun,
   double * objValue,
   double * boundsValue);

witReturnCode witGetOperationObj1ExecCost (
   WitRun * const theWitRun,
   const char * const operationName,
   float * * obj1ExecCost);

witReturnCode witGetOperationObj1ExecCostDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   double * * obj1ExecCost);

witReturnCode witGetOperationYield (
   WitRun * const theWitRun,
   const char * const operationName,
   int * * yield);

witReturnCode witGetPartBuildAheadLimit (
   WitRun * const theWitRun,
   const char * const partName,
   int * buildAheadLimit);

witReturnCode witGetPartCycleTime (
   WitRun * const theWitRun,
   const char * const partName,
   float * * cycleTime);

witReturnCode witGetPartIncLotSize (
   WitRun * const theWitRun,
   const char * const partName,
   float * * incLotSize);

witReturnCode witGetPartMinLotSize (
   WitRun * const theWitRun,
   const char * const partName,
   float * * minLotSize);

witReturnCode witGetPartMrpProdVol (
   WitRun * const theWitRun,
   const char * const partName,
   float * * mrpProdVol);

witReturnCode witGetPartNBomEntries (
   WitRun * const theWitRun,
   const char * const producedPartName,
   int * nBomEntries);

witReturnCode witGetPartObj1ProdCost (
   WitRun * const theWitRun,
   const char * const partName,
   float * * obj1ProdCost);

witReturnCode witGetPartObj1ScrapCost (
   WitRun * const theWitRun,
   const char * const partName,
   float * * obj1ScrapCost);

witReturnCode witGetPartObj1ScrapCostDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * obj1ScrapCost);

witReturnCode witGetPartObj1StockCost (
   WitRun * const theWitRun,
   const char * const partName,
   float * * obj1StockCost);

witReturnCode witGetPartObj1StockCostDbl (
   WitRun * const theWitRun,
   const char * const partName,
   double * * obj1StockCost);

witReturnCode witGetPartProdBounds (
   WitRun * const theWitRun,
   const char * const partName,
   float * * hardLower,
   float * * softLower,
   float * * hardUpper);

witReturnCode witGetPartPropRouting (
   WitRun * const theWitRun,
   const char * const partName,
   witBoolean * propRouting);

witReturnCode witGetPartYield (
   WitRun * const theWitRun,
   const char * const partName,
   int * * yield);

witReturnCode witGetStockRealloc (
   WitRun * const theWitRun,
   witBoolean * stockRealloc);

witReturnCode witGetSubsBomEntryFallout (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   int * fallout);

witReturnCode witGetSubsBomEntryObj1SubCost (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   float * * obj1SubCost);

witReturnCode witGetSubsBomEntryObj1SubCostDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   double * * obj1SubCost);

witReturnCode witGetSubsBomEntryProdVol (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   float * * prodVol);

witReturnCode witGetSubsBomEntryUsageRate (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   float * usageRate);

witReturnCode witOptPreprocess (
   WitRun * const theWitRun);

witReturnCode witSetBomEntryFallout (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int fallout);

witReturnCode witSetBomEntryPropRouting (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const witBoolean propRouting);

witReturnCode witSetBomEntryUsageRate (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const float usageRate);

witReturnCode witSetBomEntryUsageTime (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const float usageTime);

witReturnCode witSetBopEntryByProduct (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const witBoolean byProduct);

witReturnCode witSetBopEntryProdRate (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const float prodRate);

witReturnCode witSetBuildWhenEmpty (
   WitRun * const theWitRun,
   const witBoolean buildWhenEmpty);

witReturnCode witSetDemandBuildAheadLimit (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int buildAheadLimit);

witReturnCode witSetDemandBuildAheadUB (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int * buildAheadUB);

witReturnCode witSetDemandObj1CumShipReward (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const float * const obj1CumShipReward);

witReturnCode witSetDemandObj1CumShipRewardDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const double * const obj1CumShipReward);

witReturnCode witSetDemandObj1ShipReward (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const float * const obj1ShipReward);

witReturnCode witSetDemandObj1ShipRewardDbl (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const double * const obj1ShipReward);

witReturnCode witSetDemandPrefBuildAhead (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const witBoolean prefBuildAhead);

witReturnCode witSetDemandShipLateLimit (
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   const int shipLateLimit);

witReturnCode witSetHashTableSize (
   WitRun * const theWitRun,
   const int hashTableSize);

witReturnCode witSetLocalBuildAhead (
   WitRun * const theWitRun,
   const witBoolean localBuildAhead);

witReturnCode witSetLocalMultiExec (
   WitRun * const theWitRun,
   const witBoolean localMultiExec);

witReturnCode witSetObjChoice (
   WitRun * const theWitRun,
   const int objChoice);

witReturnCode witSetOperationObj1ExecCost (
   WitRun * const theWitRun,
   const char * const operationName,
   const float * const obj1ExecCost);

witReturnCode witSetOperationObj1ExecCostDbl (
   WitRun * const theWitRun,
   const char * const operationName,
   const double * const obj1ExecCost);

witReturnCode witSetOperationYield (
   WitRun * const theWitRun,
   const char * const operationName,
   const int * const yield);

witReturnCode witSetPartBuildAheadLimit (
   WitRun * const theWitRun,
   const char * const partName,
   const int buildAheadLimit);

witReturnCode witSetPartCycleTime (
   WitRun * const theWitRun,
   const char * const partName,
   const float * const cycleTime);

witReturnCode witSetPartIncLotSize (
   WitRun * const theWitRun,
   const char * const partName,
   const float * const incLotSize);

witReturnCode witSetPartMinLotSize (
   WitRun * const theWitRun,
   const char * const partName,
   const float * const minLotSize);

witReturnCode witSetPartObj1ProdCost (
   WitRun * const theWitRun,
   const char * const partName,
   const float * const obj1ProdCost);

witReturnCode witSetPartObj1ScrapCost (
   WitRun * const theWitRun,
   const char * const partName,
   const float * const obj1ScrapCost);

witReturnCode witSetPartObj1ScrapCostDbl (
   WitRun * const theWitRun,
   const char * const partName,
   const double * const obj1ScrapCost);

witReturnCode witSetPartObj1StockCost (
   WitRun * const theWitRun,
   const char * const partName,
   const float * const obj1StockCost);

witReturnCode witSetPartObj1StockCostDbl (
   WitRun * const theWitRun,
   const char * const partName,
   const double * const obj1StockCost);

witReturnCode witSetPartProdBounds (
   WitRun * const theWitRun,
   const char * const partName,
   const float * const hardLower,
   const float * const softLower,
   const float * const hardUpper);

witReturnCode witSetPartProdVol (
   WitRun * const theWitRun,
   const char * const partName,
   const float * const prodVol);

witReturnCode witSetPartPropRouting (
   WitRun * const theWitRun,
   const char * const partName,
   const witBoolean propRouting);

witReturnCode witSetPartYield (
   WitRun * const theWitRun,
   const char * const partName,
   const int * const yield);

witReturnCode witSetStockRealloc (
   WitRun * const theWitRun,
   const witBoolean stockRealloc);

witReturnCode witSetSubsBomEntryFallout (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const int fallout);

witReturnCode witSetSubsBomEntryObj1SubCost (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const float * const obj1SubCost);

witReturnCode witSetSubsBomEntryObj1SubCostDbl (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const double * const obj1SubCost);

witReturnCode witSetSubsBomEntryProdVol (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const float * const prodVol);

witReturnCode witSetSubsBomEntryUsageRate (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   const float usageRate);

witReturnCode witWriteProdSched (
   WitRun * const theWitRun,
   const char * const fileName,
   const witFileFormat fileFormat);

/*----------------------------------------------------------------------------*/
/* Miscellaneous undocumented functions.                                      */
/*----------------------------------------------------------------------------*/

witReturnCode witExecStandAlone (
   WitRun * const     theWitRun,
   const char * const theFileName);

witReturnCode witExhaustMemory (
   WitRun * const theWitRun);

witReturnCode witGetBopEntryMeSelSplit (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   witBoolean * meSelSplit);

witReturnCode witGetDevMode (
   WitRun * const theWitRun,
   witBoolean *   devMode);

witReturnCode witGetNewPipPggOrder (
   WitRun * const theWitRun,
   witBoolean * newPipPggOrder);

witReturnCode witGetNonMrSelSplit (
   WitRun * const theWitRun,
   witBoolean * nonMrSelSplit);

witReturnCode witGetPartBaSelSplit (
   WitRun * const theWitRun,
   const char * const partName,
   witBoolean * baSelSplit);

witReturnCode witGetSrSelSplit (
   WitRun * const theWitRun,
   witBoolean * srSelSplit);

witReturnCode witInterpretDevCmd (
   WitRun * const     theWitRun,
   const char * const theDevCmd);

witReturnCode witIssueFatalMesg (
   WitRun * const theWitRun);

witReturnCode witListUnissuedMsgs (
   WitRun * const theWitRun);

witReturnCode witMalloc (
   size_t size,
   void * * mem );

witReturnCode witNoDependantWitRunVeneersDecr (
   WitRun * const theWitRun);

witReturnCode witNoDependantWitRunVeneersIncr (
   WitRun * const theWitRun);

witReturnCode witNoDependantWitRunVeneersIsOne (
   WitRun * const theWitRun,
   int* isOne);

witReturnCode witSetBopEntryMeSelSplit (
   WitRun * const theWitRun,
   const char * const producingOperationName,
   const int bopEntryIndex,
   const witBoolean meSelSplit);

witReturnCode witSetNewPipPggOrder (
   WitRun * const theWitRun,
   const witBoolean newPipPggOrder);

witReturnCode witSetNonMrSelSplit (
   WitRun * const theWitRun,
   const witBoolean nonMrSelSplit);

witReturnCode witSetPartBaSelSplit (
   WitRun * const theWitRun,
   const char * const partName,
   const witBoolean baSelSplit);

witReturnCode witSetSrSelSplit (
   WitRun * const theWitRun,
   const witBoolean srSelSplit);

witReturnCode witSetWit34Allowed (
   WitRun * const theWitRun,
   const witBoolean wit34Allowed);

/*----------------------------------------------------------------------------*/
/* Values which can be assigned to some attributes.                           */
/*----------------------------------------------------------------------------*/

extern const witAttr       WitSTDOUT;
extern const witAttr       WitSTDIN;

extern const witAttr       WitRAW;
extern const witAttr       WitPRODUCT;
extern const witAttr       WitCAPACITY;
extern const witAttr       WitMATERIAL;

extern const int           WitINFORMATIONAL_MESSAGES;
extern const int           WitWARNING_MESSAGES;
extern const int           WitSEVERE_MESSAGES;
extern const int           WitFATAL_MESSAGES;

extern const witFileFormat WitBSV;
extern const witFileFormat WitCSV;

extern const witAttr       WitACC_OPT_INIT_METHOD;
extern const witAttr       WitHEUR_OPT_INIT_METHOD;
extern const witAttr       WitSCHED_OPT_INIT_METHOD;
extern const witAttr       WitCRASH_OPT_INIT_METHOD;

extern const witAttr       WitINACTIVE;
extern const witAttr       WitAT_PART;
extern const witAttr       WitAT_DEMAND;
extern const witAttr       WitAT_OPERATION;
extern const witAttr       WitAT_BOM_ENTRY;
extern const witAttr       WitAT_SUB_ENTRY;
extern const witAttr       WitAT_BOP_ENTRY;

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus

     }
     //
     // End of extern "C"

#endif

/*----------------------------------------------------------------------------*/
/* Class WitErrorExc                                                          */
/*                                                                            */
/* This is the type of exception that WIT throws, when it issues a message    */
/* for which the mesgThrowErrorExc attribute is WitTRUE.                      */
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus

class WitErrorExc
   {
   public:

      WitErrorExc (const char *, int, witReturnCode);

      WitErrorExc (const WitErrorExc &);

     ~WitErrorExc ();

      const char *  funcName () const;
      int           mesgNum  () const;
      witReturnCode retCode  () const;

   private:

      char                funcName_[51];
      const int           mesgNum_;
      const witReturnCode retCode_;
   };

#endif /* __cplusplus */

/*----------------------------------------------------------------------------*/
/* The following macros are not used.                                         */
/* They are included here for compatibility with older WIT application        */
/* programs that may have used them.                                          */
/*----------------------------------------------------------------------------*/

#define WIT_DECLSPEC
#define WIT_CONST    const
#define STDCALL

/*----------------------------------------------------------------------------*/

#endif
