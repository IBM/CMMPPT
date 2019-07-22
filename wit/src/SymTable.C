//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Watson Implosion Technology
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
//==============================================================================

//------------------------------------------------------------------------------
//
//     Source file:  SymTable.C
//     Implementation of class SymTabEntry, rules and symbol table
//
//------------------------------------------------------------------------------

#include <Parlex.h>
#include <SymTable.h>
#include <VecBS.h>
#include <Entity.h>
#include <Parser.h>
#include <WitRun.h>
#include <PipMgr.h>
#include <StochImpMgr.h>
#include <Demand.h>
#include <Material.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <BoundSet.h>
#include <ISRealArg.h>
#include <IVRealArg.h>
#include <wit.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class SymTabEntry.
//------------------------------------------------------------------------------

// Constructor and destructor

WitSymTabEntry::WitSymTabEntry():
  nparms_ (0), func_ (NULL), parmTypes_ (NULL)
{
}

WitSymTabEntry::WitSymTabEntry(int n, functionP fp, WitDRParmType::Tag pt[]):
  nparms_ (n), func_ (fp), parmTypes_ (pt)
{
}

WitSymTabEntry::~WitSymTabEntry()
{
}

// Operations

// Accessors

int WitSymTabEntry::nparms() const
{ return nparms_; }

WitSymTabEntry::functionP WitSymTabEntry::func() const
{ return func_; }

WitDRParmType::Tag *WitSymTabEntry::parmTypes() const
{ return parmTypes_; }

//------------------------------------------------------------------------------
// Implementation of class SymTable.
//------------------------------------------------------------------------------

WitRun*  WitSymTable::myWitRun_ = NULL;

// Constructor and destructor

WitSymTable::WitSymTable (WitRun * theRun)
   {
   myWitRun_ = theRun;
   }

WitSymTable::~WitSymTable()
{
  // remove all entries and delete each value
   if( theDictionary_.size() )
   {
      WitSymTableDict::iterator it = theDictionary_.begin();
      do
      {
         delete it->second;
      } while( ++it != theDictionary_.end() );
   }
}

// Operations
int WitSymTable::registerRules()
{
  int    i;
  WitSymTabEntry      *entryPtr;

  // printf("We will now register the rules\n");
  // iterate through the symbol-table structure and register each
  // rule in the hash dictionary.
  for (i = 0; symbolTable_[i].rule != NULL; i++)
  {
    entryPtr = new WitSymTabEntry(symbolTable_[i].nparms,
         symbolTable_[i].func, &symbolTable_[i].parmTypes[0]);

    typedef std::pair<std::string,WitSymTabEntry*> SymTableDictPair;
    theDictionary_.insert( SymTableDictPair( symbolTable_[i].rule, entryPtr ) );
  }

  return 0;
}

// Accesors
const WitSymTable::WitSymTableDict& WitSymTable::theDictionary()
{
  return theDictionary_;
}

WitRun* WitSymTable::myWitRun()
{ return myWitRun_; }

// Initialize static member data

  WitSymTabEntry::witSymbolDef  WitSymTable::symbolTable_[] = {

    {"add_bomEntry", 2, WitSymTable::witAddBomEntry,
      {WitDRParmType::string, WitDRParmType::string}},

    {"add_bopEntry", 2, WitSymTable::witAddBopEntry,
      {WitDRParmType::string, WitDRParmType::string}},

    {"add_demand", 2, WitSymTable::witAddDemand,
      {WitDRParmType::string, WitDRParmType::string}},

    {"add_part", 2, WitSymTable::witAddPart,
      {WitDRParmType::string, WitDRParmType::partCat}},

    {"add_partWithOperation", 1, WitSymTable::witAddPartWithOperation,
      {WitDRParmType::string}},

    {"add_operation", 1, WitSymTable::witAddOperation,
      {WitDRParmType::string}},

    {"add_subEntry", 3, WitSymTable::witAddSubEntry,
      {WitDRParmType::string, WitDRParmType::intScalar, WitDRParmType::string}},

    {"read", 1, NULL,
      {WitDRParmType::string}},

    {"set_problem_accAfterOptImp", 1, WitSymTable::witSetAccAfterOptImp,
      {WitDRParmType::boolScalar}},

    {"set_problem_accAfterSoftLB", 1, WitSymTable::witSetAccAfterSoftLB,
      {WitDRParmType::boolScalar}},

    {"set_problem_autoPriority", 1, WitSymTable::witSetAutoPriority,
      {WitDRParmType::boolScalar}},

    {"set_problem_compPrices", 1, WitSymTable::witSetCompPrices,
      {WitDRParmType::boolScalar}},

    {"set_problem_computeCriticalList", 1, 
      WitSymTable::witSetComputeCriticalList,
      {WitDRParmType::boolScalar}},

    {"set_problem_cplexParSpecDblVal", 1, WitSymTable::witSetCplexParSpecDblVal,
      {WitDRParmType::doubleScalar}},

    {"set_problem_cplexParSpecIntVal", 1, WitSymTable::witSetCplexParSpecIntVal,
      {WitDRParmType::intScalar}},

    {"set_problem_cplexParSpecName", 1, WitSymTable::witSetCplexParSpecName,
      {WitDRParmType::string}},
 
    {"set_problem_currentObjective", 1, WitSymTable::witSetCurrentObjective,
      {WitDRParmType::string}},
 
    {"set_problem_currentScenario", 1, WitSymTable::witSetCurrentScenario,
      {WitDRParmType::intScalar}},

    {"set_problem_equitability", 1, WitSymTable::witSetEquitability,
      {WitDRParmType::intScalar}},

    {"set_problem_execEmptyBom", 1, WitSymTable::witSetExecEmptyBom,
      {WitDRParmType::boolScalar}},

    {"set_problem_expCutoff", 1, WitSymTable::witSetExpCutoff,
      {WitDRParmType::doubleScalar}},

    {"set_problem_forcedMultiEq", 1, WitSymTable::witSetForcedMultiEq,
      {WitDRParmType::boolScalar}},

    {"set_problem_hashTableSize", 1, WitSymTable::witSetHashTableSize,
      {WitDRParmType::intScalar}},

    {"set_problem_highPrecisionWD", 1, WitSymTable::witSetHighPrecisionWD,
      {WitDRParmType::boolScalar}},

    {"set_problem_independentOffsets", 1, WitSymTable::witSetIndependentOffsets,
      {WitDRParmType::boolScalar}},

    {"set_problem_localBuildAhead", 1, WitSymTable::witSetLocalBuildAhead,
      {WitDRParmType::boolScalar}},

    {"set_problem_localMultiExec", 1, WitSymTable::witSetLocalMultiExec,
      {WitDRParmType::boolScalar}},

    {"set_problem_lotSizeTol", 1, WitSymTable::witSetLotSizeTol,
      {WitDRParmType::doubleScalar}},

    {"set_problem_minimalExcess", 1, WitSymTable::witSetMinimalExcess,
      {WitDRParmType::boolScalar}},

    {"set_problem_mipMode", 1, WitSymTable::witSetMipMode,
      {WitDRParmType::boolScalar}},

    {"set_problem_modHeurAlloc", 1, WitSymTable::witSetModHeurAlloc,
      {WitDRParmType::boolScalar}},

    {"set_problem_multiExec", 1, WitSymTable::witSetMultiExec,
      {WitDRParmType::boolScalar}},

    {"set_problem_multiObjMode", 1, WitSymTable::witSetMultiObjMode,
      {WitDRParmType::boolScalar}},

    {"set_problem_multiObjTol", 1, WitSymTable::witSetMultiObjTol,
      {WitDRParmType::doubleScalar}},

    {"set_problem_multiRoute", 1, WitSymTable::witSetMultiRoute,
      {WitDRParmType::boolScalar}},

    {"set_problem_nPeriods", 1, WitSymTable::witSetNPeriods,
      {WitDRParmType::intScalar}},

    {"set_problem_nScenarios", 1, WitSymTable::witSetNScenarios,
      {WitDRParmType::intScalar}},

    {"set_problem_newPipPggOrder", 1, WitSymTable::witSetNewPipPggOrder,
      {WitDRParmType::boolScalar}},

    {"set_problem_nonMrSelSplit", 1, WitSymTable::witSetNonMrSelSplit,
      {WitDRParmType::boolScalar}},

    {"set_problem_nstnResidual", 1, WitSymTable::witSetNstnResidual,
      {WitDRParmType::boolScalar}},

    {"set_problem_objChoice", 1, WitSymTable::witSetObjChoice,
      {WitDRParmType::intScalar}},

    {"set_problem_objectiveListSpec", 1, WitSymTable::witSetObjectiveListSpec,
      {WitDRParmType::string}},
 
    {"set_problem_objectiveSeqNo", 1, WitSymTable::witSetObjectiveSeqNo,
      {WitDRParmType::intScalar}},

    {"set_problem_outputPrecision", 1, WitSymTable::witSetOutputPrecision,
      {WitDRParmType::intScalar}},

    {"set_problem_penExec", 1, WitSymTable::witSetPenExec,
      {WitDRParmType::boolScalar}},

    {"set_problem_perfPegging", 1, WitSymTable::witSetPerfPegging,
      {WitDRParmType::boolScalar}},

    {"set_problem_periodStage", 1, WitSymTable::witSetPeriodStage,
      {WitDRParmType::intVector}},

    {"set_problem_periodsPerYear", 1, WitSymTable::witSetPeriodsPerYear,
      {WitDRParmType::doubleScalar}},

    {"set_problem_pgdCritListMode", 1, 
      WitSymTable::witSetPgdCritListMode,
      {WitDRParmType::boolScalar}},

    {"set_problem_pipSeqClearsNow", 1, WitSymTable::witSetPipSeqClearsNow,
      {WitDRParmType::boolScalar}},

    {"set_problem_pipSeqFromHeur", 1, WitSymTable::witSetPipSeqFromHeur,
      {WitDRParmType::boolScalar}},

    {"set_problem_prefHighStockSLBs", 1, WitSymTable::witSetPrefHighStockSLBs,
      {WitDRParmType::boolScalar}},

    {"set_problem_probability", 1, WitSymTable::witSetProbability,
      {WitDRParmType::doubleScalar}},

    {"set_problem_respectStockSLBs", 1, WitSymTable::witSetRespectStockSLBs,
      {WitDRParmType::boolScalar}},

    {"set_problem_roundReqVols", 1, WitSymTable::witSetRoundReqVols,
      {WitDRParmType::boolScalar}},

    {"set_problem_selSplit", 1, WitSymTable::witSetSelSplit,
      {WitDRParmType::boolScalar}},

    {"set_problem_selectionRecovery", 1, WitSymTable::witSetSelectionRecovery,
      {WitDRParmType::boolScalar}},

    {"set_problem_skipFailures", 1, WitSymTable::witSetSkipFailures,
      {WitDRParmType::boolScalar}},

    {"set_problem_solverLogFileName", 1, WitSymTable::witSetSolverLogFileName,
      {WitDRParmType::string}},
 
    {"set_problem_stageByObject", 1, WitSymTable::witSetStageByObject,
      {WitDRParmType::boolScalar}},

    {"set_problem_stochMode", 1, WitSymTable::witSetStochMode,
      {WitDRParmType::boolScalar}},

    {"set_problem_stockRealloc", 1, WitSymTable::witSetStockRealloc,
      {WitDRParmType::boolScalar}},

    {"set_problem_stockReallocation", 1, WitSymTable::witSetStockReallocation,
      {WitDRParmType::boolScalar}},

    {"set_problem_srSelSplit", 1, WitSymTable::witSetSrSelSplit,
      {WitDRParmType::boolScalar}},

    {"set_problem_tieBreakPropRt", 1, WitSymTable::witSetTieBreakPropRt,
      {WitDRParmType::boolScalar}},

    {"set_problem_title", 1, WitSymTable::witSetTitle,
      {WitDRParmType::string}},
 
    {"set_problem_truncOffsets", 1, WitSymTable::witSetTruncOffsets,
      {WitDRParmType::boolScalar}},

    {"set_problem_twoWayMultiExec", 1, WitSymTable::witSetTwoWayMultiExec,
      {WitDRParmType::boolScalar}},

    {"set_problem_useFocusHorizons", 1, WitSymTable::witSetUseFocusHorizons,
      {WitDRParmType::boolScalar}},

    {"set_problem_userHeurStart", 1, WitSymTable::witSetUserHeurStart,
      {WitDRParmType::boolScalar}},

    {"set_problem_wbounds", 1, WitSymTable::witSetWbounds,
      {WitDRParmType::doubleScalar}},

    {"set_problem_wit34Compatible", 1, WitSymTable::witSetWit34Compatible,
      {WitDRParmType::boolScalar}},
      
    {"set_part", 1, NULL, {WitDRParmType::string}},

    {"set_part_asapPipOrder", 1, WitSymTable::witSetPartAsapPipOrder,
      {WitDRParmType::boolScalar}},

    {"set_part_baSelSplit", 1, WitSymTable::witSetPartBaSelSplit,
      {WitDRParmType::boolScalar}},

    {"set_part_boundedLeadTimes", 1, WitSymTable::witSetPartBoundedLeadTimes,
      {WitDRParmType::boolScalar}},

    {"set_part_buildAheadUB", 1, WitSymTable::witSetPartBuildAheadUB,
      {WitDRParmType::intVector}},

    {"set_part_buildAheadLimit", 1, WitSymTable::witSetPartBuildAheadLimit,
      {WitDRParmType::intScalar}},

    {"set_part_buildAsap", 1, WitSymTable::witSetPartBuildAsap,
      {WitDRParmType::boolScalar}},

    {"set_part_buildNstn", 1, WitSymTable::witSetPartBuildNstn,
      {WitDRParmType::boolScalar}},

    {"set_part_obj1ScrapCost", 1, WitSymTable::witSetPartObj1ScrapCost,
      {WitDRParmType::doubleVector}},

    {"set_part_obj1StockCost", 1, WitSymTable::witSetPartObj1StockCost,
      {WitDRParmType::doubleVector}},

    {"set_part_objectStage", 1, WitSymTable::witSetPartObjectStage,
      {WitDRParmType::intScalar}},

    {"set_part_propRouting", 1, WitSymTable::witSetPartPropRouting,
      {WitDRParmType::boolScalar}},

    {"set_part_propRtg", 1, WitSymTable::witSetPartPropRtg,
      {WitDRParmType::boolVector}},

    {"set_part_scrapAllowed", 1, WitSymTable::witSetPartScrapAllowed,
      {WitDRParmType::boolScalar}},

    {"set_part_scrapCost", 1, WitSymTable::witSetPartScrapCost,
      {WitDRParmType::doubleVector}},

    {"set_part_singleSource", 1, WitSymTable::witSetPartSingleSource,
      {WitDRParmType::boolScalar}},

    {"set_part_stockBounds", 1, WitSymTable::witSetPartStockBounds,
      {WitDRParmType::boundSet}},

    {"set_part_stockCost", 1, WitSymTable::witSetPartStockCost,
      {WitDRParmType::doubleVector}},

    {"set_part_supplyVol", 1, WitSymTable::witSetPartSupplyVol,
      {WitDRParmType::doubleVector}},

    {"set_part_selForDel", 1, WitSymTable::witSetPartSelForDel,
      {WitDRParmType::boolScalar}},

    {"set_part_unitCost", 1, WitSymTable::witSetPartUnitCost,
      {WitDRParmType::doubleScalar}},

    {"set_demand", 2, NULL, {WitDRParmType::string, WitDRParmType::string}},

    {"set_demand_buildAheadLimit", 1,
      WitSymTable::witSetDemandBuildAheadLimit,
      {WitDRParmType::intScalar}},

    {"set_demand_buildAheadUB", 1,
      WitSymTable::witSetDemandBuildAheadUB,
      {WitDRParmType::intVector}},

    {"set_demand_cumShipBounds", 1, WitSymTable::witSetDemandCumShipBounds,
      {WitDRParmType::boundSet}},

    {"set_demand_cumShipReward", 1,
      WitSymTable::witSetDemandCumShipReward,
      {WitDRParmType::doubleVector}},

    {"set_demand_demandVol", 1, WitSymTable::witSetDemandDemandVol,
      {WitDRParmType::doubleVector}},

    {"set_demand_focusHorizon", 1, WitSymTable::witSetDemandFocusHorizon,
      {WitDRParmType::intScalar}},

    {"set_demand_grossRev", 1, WitSymTable::witSetDemandGrossRev,
      {WitDRParmType::doubleScalar}},

    {"set_demand_intShipVols", 1, WitSymTable::witSetDemandIntShipVols,
      {WitDRParmType::boolScalar}},

    {"set_demand_leadTimeUB", 1, WitSymTable::witSetDemandLeadTimeUB,
      {WitDRParmType::intVector}},

    {"set_demand_obj1CumShipReward", 1,
      WitSymTable::witSetDemandObj1CumShipReward,
      {WitDRParmType::doubleVector}},

    {"set_demand_obj1ShipReward", 1, WitSymTable::witSetDemandObj1ShipReward,
      {WitDRParmType::doubleVector}},

    {"set_demand_pipSeqIncShipVol", 1,
      WitSymTable::witSetDemandPipSeqIncShipVol,
      {WitDRParmType::doubleScalar}},

    {"set_demand_pipSeqShipPeriod", 1,
      WitSymTable::witSetDemandPipSeqShipPeriod,
      {WitDRParmType::intScalar}},

    {"set_demand_prefBuildAhead", 1, WitSymTable::witSetDemandPrefBuildAhead,
      {WitDRParmType::boolScalar}},

    {"set_demand_priority", 1, WitSymTable::witSetDemandPriority,
      {WitDRParmType::intVector}},

    {"set_demand_searchInc", 1, WitSymTable::witSetDemandSearchInc,
      {WitDRParmType::doubleScalar}},

    {"set_demand_selForDel", 1, WitSymTable::witSetDemandSelForDel,
      {WitDRParmType::boolScalar}},

    {"set_demand_shipLateAllowed", 1, WitSymTable::witSetDemandShipLateAllowed,
      {WitDRParmType::boolScalar}},

    {"set_demand_shipLateLimit", 1, WitSymTable::witSetDemandShipLateLimit,
      {WitDRParmType::intScalar}},

    {"set_demand_shipLateUB", 1, WitSymTable::witSetDemandShipLateUB,
      {WitDRParmType::intVector}},

    {"set_demand_shipReward", 1, WitSymTable::witSetDemandShipReward,
      {WitDRParmType::doubleVector}},

    {"set_demand_shipVol", 1, WitSymTable::witSetDemandShipVol,
      {WitDRParmType::doubleVector}},

    {"set_operation", 1, NULL, {WitDRParmType::string}},
 
    {"set_operation_execBounds", 1, WitSymTable::witSetOperationExecBounds,
      {WitDRParmType::boundSet}},

    {"set_operation_execCost", 1, WitSymTable::witSetOperationExecCost,
      {WitDRParmType::doubleVector}},

    {"set_operation_execPenalty", 1, WitSymTable::witSetOperationExecPenalty,
      {WitDRParmType::doubleScalar}},

    {"set_operation_incLotSize", 1, WitSymTable::witSetOperationIncLotSize,
      {WitDRParmType::doubleVector}},

    {"set_operation_incLotSize2", 1, WitSymTable::witSetOperationIncLotSize2,
      {WitDRParmType::doubleVector}},

    {"set_operation_intExecVols", 1,
          WitSymTable::witSetOperationIntExecVols,
      {WitDRParmType::boolScalar}},

    {"set_operation_lotSize2Thresh", 1,
                                    WitSymTable::witSetOperationLotSize2Thresh,
      {WitDRParmType::doubleVector}},

    {"set_operation_minLotSize", 1, WitSymTable::witSetOperationMinLotSize,
      {WitDRParmType::doubleVector}},

    {"set_operation_minLotSize2", 1, WitSymTable::witSetOperationMinLotSize2,
      {WitDRParmType::doubleVector}},

    {"set_operation_obj1ExecCost", 1, WitSymTable::witSetOperationObj1ExecCost,
      {WitDRParmType::doubleVector}},

    {"set_operation_objectStage", 1, WitSymTable::witSetOperationObjectStage,
      {WitDRParmType::intScalar}},

    {"set_operation_pipEnabled", 1,
          WitSymTable::witSetOperationPipEnabled,
      {WitDRParmType::boolScalar}},

    {"set_operation_pipRank", 1,
          WitSymTable::witSetOperationPipRank,
      {WitDRParmType::intScalar}},

    {"set_operation_selForDel", 1, WitSymTable::witSetOperationSelForDel,
      {WitDRParmType::boolScalar}},

    {"set_operation_twoLevelLotSizes", 1,
          WitSymTable::witSetOperationTwoLevelLotSizes,
      {WitDRParmType::boolScalar}},

    {"set_operation_yield", 1, WitSymTable::witSetOperationYield,
      {WitDRParmType::intVector}},

    {"set_operation_yieldRate", 1, WitSymTable::witSetOperationYieldRate,
      {WitDRParmType::doubleVector}},

    {"set_operation_cycleTime34", 1,
      WitSymTable::witSetOperationCycleTime34, {WitDRParmType::doubleVector}},

    {"set_operation_execVol", 1,
      WitSymTable::witSetOperationExecVol, {WitDRParmType::doubleVector}},

    {"set_bomEntry", 2, NULL,
       {WitDRParmType::string, WitDRParmType::intScalar}},

    {"set_bomEntry_consRate", 1, WitSymTable::witSetBomEntryConsRate,
      {WitDRParmType::doubleVector}}, 

    {"set_bomEntry_earliestPeriod", 1, 
      WitSymTable::witSetBomEntryEarliestPeriod,
      {WitDRParmType::intScalar}},

    {"set_bomEntry_execPenalty", 1, WitSymTable::witSetBomEntryExecPenalty,
      {WitDRParmType::doubleScalar}},

    {"set_bomEntry_fallout", 1, WitSymTable::witSetBomEntryFallout,
      {WitDRParmType::intScalar}},

    {"set_bomEntry_falloutRate", 1, WitSymTable::witSetBomEntryFalloutRate,
      {WitDRParmType::doubleScalar}},

    {"set_bomEntry_latestPeriod", 1,
      WitSymTable::witSetBomEntryLatestPeriod,
      {WitDRParmType::intScalar}},

    {"set_bomEntry_mandEC", 1, WitSymTable::witSetBomEntryMandEC,
      {WitDRParmType::boolScalar}},

    {"set_bomEntry_offset", 1, WitSymTable::witSetBomEntryOffset,
      {WitDRParmType::doubleVector}},

    {"set_bomEntry_propRouting", 1, WitSymTable::witSetBomEntryPropRouting,
      {WitDRParmType::boolScalar}},

    {"set_bomEntry_propRtg", 1, WitSymTable::witSetBomEntryPropRtg,
      {WitDRParmType::boolVector}},

    {"set_bomEntry_routingShare", 1, WitSymTable::witSetBomEntryRoutingShare,
      {WitDRParmType::doubleVector}},

    {"set_bomEntry_selForDel", 1, WitSymTable::witSetBomEntrySelForDel,
      {WitDRParmType::boolScalar}},

    {"set_bomEntry_singleSource", 1, WitSymTable::witSetBomEntrySingleSource,
      {WitDRParmType::boolScalar}},

    {"set_bomEntry_usageRate", 1, WitSymTable::witSetBomEntryUsageRate,
      {WitDRParmType::doubleScalar}}, 

    {"set_subEntry", 3, NULL,
       {WitDRParmType::string,
        WitDRParmType::intScalar, 
        WitDRParmType::intScalar}},

    {"set_subEntry_consRate", 1, WitSymTable::witSetSubEntryConsRate,
      {WitDRParmType::doubleVector}},  

    {"set_subEntry_earliestPeriod", 1, 
      WitSymTable::witSetSubEntryEarliestPeriod,
      {WitDRParmType::intScalar}},

    {"set_subEntry_execPenalty", 1, WitSymTable::witSetSubEntryExecPenalty,
      {WitDRParmType::doubleScalar}},

    {"set_subEntry_expAllowed", 1,
      WitSymTable::witSetSubEntryExpAllowed, {WitDRParmType::boolScalar}},

    {"set_subEntry_expNetAversion", 1,
      WitSymTable::witSetSubEntryExpNetAversion, {WitDRParmType::doubleScalar}},

    {"set_subEntry_fallout", 1, WitSymTable::witSetSubEntryFallout,
      {WitDRParmType::intScalar}},

    {"set_subEntry_falloutRate", 1, WitSymTable::witSetSubEntryFalloutRate,
      {WitDRParmType::doubleScalar}},

    {"set_subEntry_intSubVols", 1,
      WitSymTable::witSetSubEntryIntSubVols, {WitDRParmType::boolScalar}},

    {"set_subEntry_latestPeriod", 1,
      WitSymTable::witSetSubEntryLatestPeriod,
      {WitDRParmType::intScalar}},

    {"set_subEntry_mrpNetAllowed", 1,
      WitSymTable::witSetSubEntryMrpNetAllowed, {WitDRParmType::boolScalar}},

    {"set_subEntry_netAllowed", 1,
      WitSymTable::witSetSubEntryNetAllowed, {WitDRParmType::boolScalar}},

    {"set_subEntry_usageRate", 1, WitSymTable::witSetSubEntryUsageRate,
      {WitDRParmType::doubleScalar}},  

    {"set_subEntry_obj1SubCost", 1,
      WitSymTable::witSetSubEntryObj1SubCost,
      {WitDRParmType::doubleVector}},

    {"set_subEntry_offset", 1,
      WitSymTable::witSetSubEntryOffset,
      {WitDRParmType::doubleVector}},

    {"set_subEntry_routingShare", 1, WitSymTable::witSetSubEntryRoutingShare,
      {WitDRParmType::doubleVector}},

    {"set_subEntry_selForDel", 1,
      WitSymTable::witSetSubEntrySelForDel, {WitDRParmType::boolScalar}},

    {"set_subEntry_subCost", 1,
      WitSymTable::witSetSubEntrySubCost, {WitDRParmType::doubleVector}},

    {"set_subEntry_subVol", 1, WitSymTable::witSetSubEntrySubVol,
      {WitDRParmType::doubleVector}}, 

    {"set_bopEntry", 2, NULL,
       {WitDRParmType::string, WitDRParmType::intScalar}},

    {"set_bopEntry_byProduct", 1, WitSymTable::witSetBopEntryByProduct,
      {WitDRParmType::boolScalar}},

    {"set_bopEntry_earliestPeriod", 1,
      WitSymTable::witSetBopEntryEarliestPeriod,
      {WitDRParmType::intScalar}},

    {"set_bopEntry_expAllowed", 1, WitSymTable::witSetBopEntryExpAllowed,
      {WitDRParmType::boolScalar}},

    {"set_bopEntry_expAversion", 1,
      WitSymTable::witSetBopEntryExpAversion, {WitDRParmType::doubleScalar}},

    {"set_bopEntry_latestPeriod", 1,
      WitSymTable::witSetBopEntryLatestPeriod,
      {WitDRParmType::intScalar}},

    {"set_bopEntry_meSelSplit", 1, WitSymTable::witSetBopEntryMeSelSplit,
      {WitDRParmType::boolScalar}},

    {"set_bopEntry_offset", 1, WitSymTable::witSetBopEntryOffset,
      {WitDRParmType::doubleVector}},

    {"set_bopEntry_prodRate", 1,
      WitSymTable::witSetBopEntryProdRate, {WitDRParmType::doubleScalar}},

    {"set_bopEntry_productRate", 1,
      WitSymTable::witSetBopEntryProductRate, {WitDRParmType::doubleVector}},

    {"set_bopEntry_routingShare", 1,
      WitSymTable::witSetBopEntryRoutingShare, {WitDRParmType::doubleVector}},

    {"set_bopEntry_selForDel", 1, WitSymTable::witSetBopEntrySelForDel,
      {WitDRParmType::boolScalar}},

    {"set_partWithOperation_asapPipOrder", 1,
      WitSymTable::witSetPartAsapPipOrder, {WitDRParmType::boolScalar}},

    {"set_partWithOperation_baSelSplit", 1,
      WitSymTable::witSetPartBaSelSplit, {WitDRParmType::boolScalar}},

    {"set_partWithOperation_boundedLeadTimes", 1,
      WitSymTable::witSetPartBoundedLeadTimes, {WitDRParmType::boolScalar}},

    {"set_partWithOperation_buildAheadUB", 1,
      WitSymTable::witSetPartBuildAheadUB, {WitDRParmType::intVector}},

    {"set_partWithOperation_buildAheadLimit", 1,
      WitSymTable::witSetPartBuildAheadLimit, {WitDRParmType::intScalar}},

    {"set_partWithOperation_buildAsap", 1,
      WitSymTable::witSetPartBuildAsap, {WitDRParmType::boolScalar}},

    {"set_partWithOperation_buildNstn", 1,
      WitSymTable::witSetPartBuildNstn, {WitDRParmType::boolScalar}},

    {"set_partWithOperation_obj1ScrapCost", 1,
      WitSymTable::witSetPartObj1ScrapCost, {WitDRParmType::doubleVector}},

    {"set_partWithOperation_obj1StockCost", 1,
      WitSymTable::witSetPartObj1StockCost, {WitDRParmType::doubleVector}},

    {"set_partWithOperation_objectStage", 1,
      WitSymTable::witSetPartObjectStage, {WitDRParmType::intScalar}},

    {"set_partWithOperation_propRouting", 1,
      WitSymTable::witSetPartPropRouting, {WitDRParmType::boolScalar}},

    {"set_partWithOperation_propRtg", 1,
      WitSymTable::witSetPartPropRtg, {WitDRParmType::boolVector}},

    {"set_partWithOperation_scrapAllowed", 1,
      WitSymTable::witSetPartScrapAllowed, {WitDRParmType::boolScalar}},

    {"set_partWithOperation_scrapCost", 1,
      WitSymTable::witSetPartScrapCost, {WitDRParmType::doubleVector}},

    {"set_partWithOperation_selForDel", 1,
      WitSymTable::witSetPartSelForDel, {WitDRParmType::boolScalar}},

    {"set_partWithOperation_singleSource", 1,
      WitSymTable::witSetPartSingleSource, {WitDRParmType::boolScalar}},

    {"set_partWithOperation_stockBounds", 1,
      WitSymTable::witSetPartStockBounds, {WitDRParmType::boundSet}},

    {"set_partWithOperation_supplyVol", 1,
      WitSymTable::witSetPartSupplyVol, {WitDRParmType::doubleVector}},

    {"set_partWithOperation_stockCost", 1,
      WitSymTable::witSetPartStockCost, {WitDRParmType::doubleVector}},

    {"set_partWithOperation_unitCost", 1,
      WitSymTable::witSetPartUnitCost, {WitDRParmType::doubleScalar}},

    {NULL,0,0,{WitDRParmType::undefined}}
  };


// Implementation of API function wrappers

WitParseRC WitSymTable::witAddBomEntry()
{
  myWitRun()->witAddBomEntry
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getPartName());
  return 0;   
}

WitParseRC WitSymTable::witAddBopEntry()
{
  myWitRun()->witAddBopEntry
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getPartName());
  return 0;
}

WitParseRC WitSymTable::witAddDemand()
{
  myWitRun()->witAddDemand
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName());
  return 0;
}

WitParseRC WitSymTable::witAddPart()
{
  switch (WitParser::instance ()->currentEntity()->getCatArg(1)) {
    case WitDRPartCat::capacity:
      myWitRun()->witAddPart
        (WitParser::instance ()->currentEntity()->getPartName(), WitCAPACITY);
      break;
    case WitDRPartCat::material:
      myWitRun()->witAddPart
        (WitParser::instance ()->currentEntity()->getPartName(), WitMATERIAL);
      break;
  };  
  return 0;
} 

WitParseRC WitSymTable::witAddPartWithOperation()
{
  myWitRun()->witAddPartWithOperation
    (WitParser::instance ()->currentEntity()->getPartName());
  return 0;
}

WitParseRC WitSymTable::witAddOperation()
{
  myWitRun()->witAddOperation
    (WitParser::instance ()->currentEntity()->getOperationName());
  return 0;
}

WitParseRC WitSymTable::witAddSubEntry()
{
  myWitRun()->witAddSubsBomEntry
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getPartName());
  return 0;
}

WitParseRC WitSymTable::witSetAccAfterOptImp()
{
  myWitRun()->witSetAccAfterOptImp
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetAccAfterSoftLB()
{
  myWitRun()->witSetAccAfterSoftLB
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetAutoPriority()
{
  myWitRun()->witSetAutoPriority
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetCompPrices()
{
  myWitRun()->witSetCompPrices
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetComputeCriticalList()
{
  myWitRun()->witSetComputeCriticalList
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetCplexParSpecDblVal()
{
  myWitRun()->witSetCplexParSpecDblVal
    (WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetCplexParSpecIntVal()
{
  myWitRun()->witSetCplexParSpecIntVal
    (WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetCplexParSpecName()
{
  myWitRun()->witSetCplexParSpecName
    (WitParser::instance ()->currentAttr()->getStringArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetCurrentObjective()
{
  myWitRun()->witSetCurrentObjective
    (WitParser::instance ()->currentAttr()->getStringArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetCurrentScenario()
{
  myWitRun()->witSetCurrentScenario
    (WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetEquitability()
{
  myWitRun()->witSetEquitability
    (WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetExecEmptyBom()
{
  myWitRun()->witSetExecEmptyBom
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetExpCutoff()
{
  myWitRun()->witSetExpCutoff
    (WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetForcedMultiEq()
{
  myWitRun()->witSetForcedMultiEq
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetHashTableSize()
{
  myWitRun()->witSetHashTableSize
    (WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetHighPrecisionWD()
{
  myWitRun()->witSetHighPrecisionWD
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetIndependentOffsets()
{
  myWitRun()->witSetIndependentOffsets
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetLocalBuildAhead()
{
  myWitRun()->witSetLocalBuildAhead
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetLocalMultiExec()
{
  myWitRun()->witSetLocalMultiExec
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetLotSizeTol()
{
  myWitRun()->witSetLotSizeTol
    (WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetMinimalExcess()
{
  myWitRun()->witSetMinimalExcess
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetMipMode()
{
  myWitRun()->witSetMipMode
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetModHeurAlloc()
{
  myWitRun()->witSetModHeurAlloc
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetMultiExec()
{
  myWitRun()->witSetMultiExec
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetMultiObjMode()
{
  myWitRun()->witSetMultiObjMode
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetMultiObjTol()
{
  myWitRun()->witSetMultiObjTol
    (WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetMultiRoute()
{
  myWitRun()->witSetMultiRoute
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetNPeriods()
{
  myWitRun()->witSetNPeriods
    (WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetNScenarios()
{
  myWitRun()->witSetNScenarios
    (WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetNewPipPggOrder()
{
  myWitRun()->witSetNewPipPggOrder
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetNonMrSelSplit()
{
  myWitRun()->witSetNonMrSelSplit
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetNstnResidual()
{
  myWitRun()->witSetNstnResidual
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetObjChoice()
{
  myWitRun()->witSetObjChoice
    (WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetObjectiveListSpec()
{
  myWitRun()->witSetObjectiveListSpec
    (WitParser::instance ()->currentAttr()->getStringArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetObjectiveSeqNo()
{
  myWitRun()->witSetObjectiveSeqNo
    (WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetOutputPrecision()
{
  myWitRun()->witSetOutputPrecision
    (WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPenExec()
{
  myWitRun()->witSetPenExec
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPerfPegging()
{
  myWitRun()->witSetPerfPegging
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPeriodStage()
{
  int    *iptr;
  iptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getIntVec 
  (WitStochImpMgr::defPeriodStageIdx ());

  if (iptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetPeriodStage (iptr);
  return 0;
}

WitParseRC WitSymTable::witSetPeriodsPerYear ()
   {
   myWitRun ()->myMsgFac () ("setObsAttFromFileWmsg", "periodsPerYear");

   return 0;
   }

WitParseRC WitSymTable::witSetPgdCritListMode()
{
  myWitRun()->witSetPgdCritListMode
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPipSeqClearsNow()
{
  if (WitParser::instance ()->currentAttr ()->getBoolArg (0))
     myWitRun()->witClearPipSeq ();

  return 0;
}

WitParseRC WitSymTable::witSetPipSeqFromHeur()
{
  myWitRun()->witSetPipSeqFromHeur
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPrefHighStockSLBs()
{
  myWitRun()->witSetPrefHighStockSLBs
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetProbability()
{
  myWitRun()->witSetProbability
    (WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetRespectStockSLBs()
{
  myWitRun()->witSetRespectStockSLBs
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetRoundReqVols()
{
  myWitRun()->witSetRoundReqVols
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSelSplit()
{
  myWitRun()->witSetSelSplit
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSelectionRecovery()
{
  myWitRun()->witSetSelectionRecovery
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSkipFailures()
{
  myWitRun()->witSetSkipFailures
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSolverLogFileName()
{
  myWitRun()->witSetSolverLogFileName
    (WitParser::instance ()->currentAttr()->getStringArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetStageByObject()
{
  myWitRun()->witSetStageByObject
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetStochMode()
{
  myWitRun()->witSetStochMode
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetStockRealloc()
{
  myWitRun()->witSetStockRealloc
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetStockReallocation()
{
  myWitRun()->witSetStockReallocation
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSrSelSplit()
{
  myWitRun()->witSetSrSelSplit
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetTieBreakPropRt()
{
  myWitRun()->witSetTieBreakPropRt
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetTitle()
{
  myWitRun()->witSetTitle
    (WitParser::instance ()->currentAttr()->getStringArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetTruncOffsets()
{
  myWitRun()->witSetTruncOffsets
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetTwoWayMultiExec()
{
  myWitRun()->witSetTwoWayMultiExec
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetUseFocusHorizons()
{
  myWitRun()->witSetUseFocusHorizons
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetUserHeurStart()
{
  myWitRun()->witSetUserHeurStart
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetWbounds()
{
  myWitRun()->witSetWbounds
    (WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetWit34Compatible()
{
  myWitRun()->witSetWit34Compatible
    (WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPartAsapPipOrder()
{
  myWitRun()->witSetPartAsapPipOrder
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPartBaSelSplit()
{
  myWitRun()->witSetPartBaSelSplit
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPartBoundedLeadTimes()
{
  myWitRun()->witSetPartBoundedLeadTimes
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPartBuildAheadUB()
{
  int  *iptr;
  iptr =  WitParser::instance ()->currentAttr()->getVectorArg(0)->getIntVec
    (WitMaterial::defBuildAheadUB(myWitRun()->myProblem ()));
  if (iptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetPartBuildAheadUB
    (WitParser::instance ()->currentEntity()->getPartName(), iptr);
  return 0;
}

WitParseRC WitSymTable::witSetPartBuildAheadLimit()
{
  myWitRun()->witSetPartBuildAheadLimit
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPartBuildAsap()
{
  myWitRun()->witSetPartBuildAsap
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPartBuildNstn()
{
  myWitRun()->witSetPartBuildNstn
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPartObj1ScrapCost()
{
  double *dptr;
  dptr =  WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitPart::defScrapCost());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetPartObj1ScrapCost
    (WitParser::instance ()->currentEntity()->getPartName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetPartObj1StockCost()
{
  double * dptr;
  dptr =  WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitMaterial::defStockCost());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetPartObj1StockCost
    (WitParser::instance ()->currentEntity()->getPartName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetPartObjectStage()
{
  myWitRun()->witSetPartObjectStage
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPartPropRouting()
{
  myWitRun()->witSetPartPropRouting
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPartPropRtg()
{
  WitBoolean * bptr;

  bptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getBoolVec
    (WitPart::defPropRtg());
  if (bptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetPartPropRtg
    (WitParser::instance ()->currentEntity()->getPartName(), bptr);
  return 0;
}

WitParseRC WitSymTable::witSetPartScrapAllowed()
{
  myWitRun()->witSetPartScrapAllowed
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPartScrapCost()
{
  double *dptr;
  dptr =  WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitPart::defScrapCost());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetPartScrapCost
    (WitParser::instance ()->currentEntity()->getPartName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetPartSingleSource()
{
  myWitRun()->witSetPartSingleSource
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPartStockBounds()
{
  WitVectorSpec * vecPtr;
  double *        arg2 = NULL;
  double *        arg3 = NULL;
  double *        arg4 = NULL;

  vecPtr =
     WitParser::instance ()->currentAttr()->getBoundSetArg(0)->hardLBVec();

  if (vecPtr != NULL) {
    arg2 = vecPtr->getDblVec (WitBoundSet::defHardLB());
    if (arg2 == NULL) return PERIOD_ERROR;
  };

  vecPtr =
     WitParser::instance ()->currentAttr()->getBoundSetArg(0)->softLBVec();

  if (vecPtr != NULL) {
    arg3 = vecPtr->getDblVec (WitBoundSet::defSoftLB());
    if (arg3 == NULL) return PERIOD_ERROR;
  };

  vecPtr =
     WitParser::instance ()->currentAttr()->getBoundSetArg(0)->hardUBVec();

  if (vecPtr != NULL) {
    arg4 = vecPtr->getDblVec (WitBoundSet::defHardUB());
    if (arg4 == NULL) return PERIOD_ERROR;
  };

  myWitRun()->witSetPartStockBounds
    (WitParser::instance ()->currentEntity()->getPartName(), arg2, arg3, arg4);
  return 0;
}

WitParseRC WitSymTable::witSetPartStockCost()
{
  double * dptr;
  dptr =  WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitMaterial::defStockCost());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetPartStockCost
    (WitParser::instance ()->currentEntity()->getPartName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetPartSupplyVol()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitPart::defSupplyVol());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetPartSupplyVol
    (WitParser::instance ()->currentEntity()->getPartName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetPartSelForDel()
{
  myWitRun()->witSetPartSelForDel
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetPartUnitCost ()
   {
   myWitRun ()->myMsgFac () ("setObsAttFromFileWmsg", "unitCost");

   return 0;
   }

WitParseRC WitSymTable::witSetDemandBuildAheadLimit()
{
  myWitRun()->witSetDemandBuildAheadLimit
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetDemandBuildAheadUB()
{
  int  *iptr;

  iptr =  WitParser::instance ()->currentAttr()->getVectorArg(0)->getIntVec
    (WitDemand::defBuildAheadUB());
  if (iptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetDemandBuildAheadUB
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(),
     iptr);

  return 0;
}

WitParseRC WitSymTable::witSetDemandCumShipBounds()
{
  WitVectorSpec * vecPtr;
  double *        arg3 = NULL;
  double *        arg4 = NULL;
  double *        arg5 = NULL;

  vecPtr =
     WitParser::instance ()->currentAttr()->getBoundSetArg(0)->hardLBVec();
  if (vecPtr != NULL) {
    arg3 = vecPtr->getDblVec (WitBoundSet::defHardLB());
    if (arg3 == NULL) return PERIOD_ERROR;
  };

  vecPtr =
     WitParser::instance ()->currentAttr()->getBoundSetArg(0)->softLBVec();
  if (vecPtr != NULL) {
    arg4 = vecPtr->getDblVec (WitBoundSet::defSoftLB());
    if (arg4 == NULL) return PERIOD_ERROR;
  };

  vecPtr =
     WitParser::instance ()->currentAttr()->getBoundSetArg(0)->hardUBVec();
  if (vecPtr != NULL) {
    arg5 = vecPtr->getDblVec (WitBoundSet::defHardUB());
    if (arg5 == NULL) return PERIOD_ERROR;
  };

  myWitRun()->witSetDemandCumShipBounds
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(),
     arg3,
     arg4,
     arg5);
  return 0;
}

WitParseRC WitSymTable::witSetDemandCumShipReward()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitDemand::defCumShipReward());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetDemandCumShipReward
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetDemandDemandVol()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitDemand::defDemandVol());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetDemandDemandVol
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetDemandFocusHorizon()
{
  myWitRun()->witSetDemandFocusHorizon
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetDemandGrossRev ()
   {
   myWitRun ()->myMsgFac () ("setObsAttFromFileWmsg", "grossRev");

   return 0;
   }

WitParseRC WitSymTable::witSetDemandIntShipVols()
{
  myWitRun()->witSetDemandIntShipVols
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetDemandLeadTimeUB()
{
  int  *iptr;

  iptr =  WitParser::instance ()->currentAttr()->getVectorArg(0)->getIntVec
    (WitDemand::defLeadTimeUB(myWitRun()->myProblem ()));
  if (iptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetDemandLeadTimeUB
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(),
     iptr);

  return 0;
}

WitParseRC WitSymTable::witSetDemandObj1CumShipReward()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitDemand::defCumShipReward());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetDemandObj1CumShipReward
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetDemandObj1ShipReward()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitDemand::defShipReward());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetDemandObj1ShipReward
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetDemandPipSeqIncShipVol()
{
  myWitRun()->witAppendToPipSeq
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(),
     myWitRun ()->myProblem ()->myPipMgr ()->shipSeqShipPer (),
     WitParser::instance ()->currentAttr()->getDoubleArg(0));

  myWitRun ()->myProblem ()->myPipMgr ()->setShipSeqShipPer (-1);

  return 0;
}

WitParseRC WitSymTable::witSetDemandPipSeqShipPeriod()
{
  myWitRun ()->myProblem ()->myPipMgr ()->setShipSeqShipPer (
     WitParser::instance ()->currentAttr()->getIntArg(0));

  return 0;
}

WitParseRC WitSymTable::witSetDemandPrefBuildAhead()
{
  myWitRun()->witSetDemandPrefBuildAhead
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetDemandPriority()
{ 
  int    *iptr;

  iptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getIntVec
    (WitDemand::defPriority());
  if (iptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetDemandPriority
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(), iptr);
  return 0;
}

WitParseRC WitSymTable::witSetDemandSearchInc()
{
  myWitRun()->witSetDemandSearchInc
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(),
     WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetDemandSelForDel()
{
  myWitRun()->witSetDemandSelForDel
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetDemandShipLateAllowed()
{
  myWitRun()->witSetDemandShipLateAllowed
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetDemandShipLateLimit()
{
  myWitRun()->witSetDemandShipLateLimit
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetDemandShipLateUB()
{
  int  *iptr;

  iptr =  WitParser::instance ()->currentAttr()->getVectorArg(0)->getIntVec
    (WitDemand::defShipLateUB(myWitRun()->myProblem ()));
  if (iptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetDemandShipLateUB
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(),
     iptr);

  return 0;
}

WitParseRC WitSymTable::witSetDemandShipReward()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitDemand::defShipReward());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetDemandShipReward
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetDemandShipVol()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitDemand::defShipVol());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetDemandShipVol
    (WitParser::instance ()->currentEntity()->getPartName(),
     WitParser::instance ()->currentEntity()->getDemandName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetOperationExecBounds()
{
  WitVectorSpec * vecPtr;
  double *        arg2 = NULL;
  double *        arg3 = NULL;
  double *        arg4 = NULL;

  vecPtr =
     WitParser::instance ()->currentAttr()->getBoundSetArg(0)->hardLBVec();
  if (vecPtr != NULL) {
    arg2 = vecPtr->getDblVec (WitBoundSet::defHardLB());
    if (arg2 == NULL) return PERIOD_ERROR;
  };

  vecPtr =
     WitParser::instance ()->currentAttr()->getBoundSetArg(0)->softLBVec();
  if (vecPtr != NULL) {
    arg3 = vecPtr->getDblVec (WitBoundSet::defSoftLB());
    if (arg3 == NULL) return PERIOD_ERROR;
  };

  vecPtr =
     WitParser::instance ()->currentAttr()->getBoundSetArg(0)->hardUBVec();
  if (vecPtr != NULL) {
    arg4 = vecPtr->getDblVec (WitBoundSet::defHardUB());
    if (arg4 == NULL) return PERIOD_ERROR;
  };

  myWitRun()->witSetOperationExecBounds (
     WitParser::instance ()->currentEntity()->getOperationName(),
     arg2,
     arg3,
     arg4);
  return 0;
}

WitParseRC WitSymTable::witSetOperationExecCost()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitOperation::defExecCost());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetOperationExecCost
    (WitParser::instance ()->currentEntity()->getOperationName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetOperationExecPenalty()
{
  myWitRun()->witSetOperationExecPenalty
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetOperationIncLotSize()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitOperation::defIncLotSize());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetOperationIncLotSize
    (WitParser::instance ()->currentEntity()->getOperationName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetOperationIncLotSize2()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitOperation::defIncLotSize2());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetOperationIncLotSize2
    (WitParser::instance ()->currentEntity()->getOperationName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetOperationIntExecVols()
{
  myWitRun()->witSetOperationIntExecVols
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetOperationLotSize2Thresh()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitOperation::defLotSize2Thresh());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetOperationLotSize2Thresh
    (WitParser::instance ()->currentEntity()->getOperationName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetOperationMinLotSize()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitOperation::defMinLotSize());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetOperationMinLotSize
    (WitParser::instance ()->currentEntity()->getOperationName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetOperationMinLotSize2()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitOperation::defMinLotSize2());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetOperationMinLotSize2
    (WitParser::instance ()->currentEntity()->getOperationName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetOperationObj1ExecCost()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitOperation::defExecCost());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetOperationObj1ExecCost
    (WitParser::instance ()->currentEntity()->getOperationName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetOperationObjectStage()
{
  myWitRun()->witSetOperationObjectStage
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetOperationPipEnabled()
{
  myWitRun()->witSetOperationPipEnabled
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetOperationPipRank()
{
  myWitRun()->witSetOperationPipRank
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetOperationSelForDel()
{
  myWitRun()->witSetOperationSelForDel
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetOperationTwoLevelLotSizes()
{
  myWitRun()->witSetOperationTwoLevelLotSizes
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetOperationYield()
{
  int    *iptr;

  iptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getIntVec
    (WitOperation::defYield());
  if (iptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetOperationYield
    (WitParser::instance ()->currentEntity()->getOperationName(), iptr);
  return 0;
}

WitParseRC WitSymTable::witSetOperationYieldRate()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitOperation::defYieldRate());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetOperationYieldRate
    (WitParser::instance ()->currentEntity()->getOperationName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetOperationCycleTime34()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitOperation::defCycleTime34());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetOperationCycleTime34
    (WitParser::instance ()->currentEntity()->getOperationName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetOperationExecVol()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitOperation::defExecVol());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetOperationExecVol
    (WitParser::instance ()->currentEntity()->getOperationName(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetBomEntryConsRate()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitConsEntry::defConsRate());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetBomEntryConsRate
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetBomEntryEarliestPeriod()
{
  myWitRun()->witSetBomEntryEarliestPeriod
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBomEntryExecPenalty()
{
  myWitRun()->witSetBomEntryExecPenalty
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBomEntryFallout()
{
  myWitRun()->witSetBomEntryFallout
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBomEntryFalloutRate()
{
  myWitRun()->witSetBomEntryFalloutRate
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBomEntryLatestPeriod()
{
  myWitRun()->witSetBomEntryLatestPeriod
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBomEntryMandEC()
{
  myWitRun()->witSetBomEntryMandEC
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBomEntryOffset()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitBillEntry::defOffset());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetBomEntryOffset
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetBomEntryPropRouting()
{
  myWitRun()->witSetBomEntryPropRouting
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBomEntryPropRtg()
{
  WitBoolean * bptr;

  bptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getBoolVec
    (WitBomEntry::defPropRtg());
  if (bptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetBomEntryPropRtg
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     bptr);
  return 0;
}

WitParseRC WitSymTable::witSetBomEntryRoutingShare()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitBillEntry::defRoutingShare());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetBomEntryRoutingShare
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetBomEntrySelForDel()
{
  myWitRun()->witSetBomEntrySelForDel
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBomEntrySingleSource()
{
  myWitRun()->witSetBomEntrySingleSource
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBomEntryUsageRate()
{
  myWitRun()->witSetBomEntryUsageRate
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSubEntryConsRate()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitConsEntry::defConsRate());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetSubsBomEntryConsRate
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetSubEntryEarliestPeriod()
{
  myWitRun()->witSetSubsBomEntryEarliestPeriod
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSubEntryExecPenalty()
{
  myWitRun()->witSetSubsBomEntryExecPenalty
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(),
     WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSubEntryExpAllowed()
{
  myWitRun()->witSetSubsBomEntryExpAllowed
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSubEntryExpNetAversion()
{
  myWitRun()->witSetSubsBomEntryExpNetAversion
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(),
     WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSubEntryFallout()
{
  myWitRun()->witSetSubsBomEntryFallout
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSubEntryFalloutRate()
{
  myWitRun()->witSetSubsBomEntryFalloutRate
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(),
     WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSubEntryIntSubVols()
{
  myWitRun()->witSetSubsBomEntryIntSubVols
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSubEntryLatestPeriod()
{
  myWitRun()->witSetSubsBomEntryLatestPeriod
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSubEntryMrpNetAllowed()
{
  myWitRun()->witSetSubsBomEntryMrpNetAllowed
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSubEntryNetAllowed()
{
  myWitRun()->witSetSubsBomEntryNetAllowed
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSubEntryUsageRate()
{
  myWitRun()->witSetSubsBomEntryUsageRate
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(),
     WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;  
}

WitParseRC WitSymTable::witSetSubEntryObj1SubCost()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitSubEntry::defSubCost());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetSubsBomEntryObj1SubCost
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(), dptr);
  return 0; 
}

WitParseRC WitSymTable::witSetSubEntryOffset()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitBillEntry::defOffset());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetSubsBomEntryOffset
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetSubEntryRoutingShare()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitBillEntry::defRoutingShare());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetSubsBomEntryRoutingShare
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetSubEntrySelForDel()
{
  myWitRun()->witSetSubsBomEntrySelForDel
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetSubEntrySubCost()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitSubEntry::defSubCost());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetSubsBomEntrySubCost
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(), dptr);
  return 0; 
}

WitParseRC WitSymTable::witSetSubEntrySubVol()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitSubEntry::defSubVol());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetSubsBomEntrySubVol
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBomEntryIndex(),
     WitParser::instance ()->currentEntity()->getSubEntryIndex(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetBopEntryByProduct()
{
  myWitRun()->witSetBopEntryByProduct
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBopEntryIndex(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0; 
}

WitParseRC WitSymTable::witSetBopEntryEarliestPeriod()
{
  myWitRun()->witSetBopEntryEarliestPeriod
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBopEntryIndex(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBopEntryExpAllowed()
{
  myWitRun()->witSetBopEntryExpAllowed
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBopEntryIndex(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBopEntryExpAversion()
{
  myWitRun()->witSetBopEntryExpAversion
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBopEntryIndex(),
     WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBopEntryLatestPeriod()
{
  myWitRun()->witSetBopEntryLatestPeriod
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBopEntryIndex(),
     WitParser::instance ()->currentAttr()->getIntArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBopEntryMeSelSplit()
{
  myWitRun()->witSetBopEntryMeSelSplit
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBopEntryIndex(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBopEntryOffset()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
       (WitBillEntry::defOffset());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetBopEntryOffset
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBopEntryIndex(), dptr);
  return 0;
}

WitParseRC WitSymTable::witSetBopEntryProdRate()
{
  myWitRun()->witSetBopEntryProdRate
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBopEntryIndex(),
     WitParser::instance ()->currentAttr()->getDoubleArg(0));
  return 0;
}

WitParseRC WitSymTable::witSetBopEntryProductRate()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitBopEntry::defProductRate());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetBopEntryProductRate
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBopEntryIndex(),
     dptr);

  return 0;
}

WitParseRC WitSymTable::witSetBopEntryRoutingShare()
{
  double * dptr;

  dptr = WitParser::instance ()->currentAttr()->getVectorArg(0)->getDblVec
    (WitBillEntry::defRoutingShare());
  if (dptr == NULL) return PERIOD_ERROR;

  myWitRun()->witSetBopEntryRoutingShare
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBopEntryIndex(),
     dptr);

  return 0;
}

WitParseRC WitSymTable::witSetBopEntrySelForDel()
{
  myWitRun()->witSetBopEntrySelForDel
    (WitParser::instance ()->currentEntity()->getOperationName(),
     WitParser::instance ()->currentEntity()->getBopEntryIndex(),
     WitParser::instance ()->currentAttr()->getBoolArg(0));
  return 0;
}


// Implementation of class SymTableIterator

// Constructor and destructor
WitSymTableIterator::WitSymTableIterator(WitSymTable *s):
  pst_(s), index_(0)
{
}

WitSymTableIterator::~WitSymTableIterator()
{
}

WitSymTabEntry::witSymbolDef *WitSymTableIterator::operator ()()
{
  if (index_ < sizeof WitSymTable::symbolTable_
    /sizeof(WitSymTabEntry::witSymbolDef))
      return(&WitSymTable::symbolTable_[index_++]);
  return (&WitSymTable::symbolTable_[index_ = 0]);
}
