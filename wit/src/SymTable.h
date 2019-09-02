//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef SymTableH
#define SymTableH

//------------------------------------------------------------------------------
//
//     Source file: SymTable.h
//     Definition of classes SymTabEntry and SymTable.
//
//------------------------------------------------------------------------------

#include <string>
#include <map>

#include <Util.h>

/*------------------------------------------------------------------------------
 *
 *     Definition of class SymTabEntry.  Each object of type SymTabEntry
 *     represents a symbol table entry.  For each rule there is a corresponding
 *     symbol table entry.  The collection of all symbol table entries is the
 *     symbol table, which is used by the parser to verify the number and types
 *     of arguments associated with attributes and to invoke the appropriate
 *     WIT API function.
 *
 *------------------------------------------------------------------------------
 */

class WitSymTabEntry 
{
public:

typedef WitParseRC (* functionP)();

typedef struct witSymbolDef {
  const char *       rule;           // rule recognized by the parser
  int                nparms;         // number of parameters
  functionP          func;           // pointer to local function wrapper
  WitDRParmType::Tag parmTypes[10];  // types of parameters
} witSymbolDef;

public:

  // Constructor and destructor
  WitSymTabEntry();
  WitSymTabEntry(int, functionP, WitDRParmType::Tag *p);
  ~WitSymTabEntry();

  // Accessors
  int        nparms() const;
  functionP  func() const;
  WitDRParmType::Tag *parmTypes() const;  // a pointer to an array of type indicators

private:

  int        nparms_;             // number of parameters for the function
  functionP  func_;               // pointer to local attribute function
  WitDRParmType::Tag *parmTypes_; // datatype of function parameters
};

//------------------------------------------------------------------------------
//
//     Definition of class SymTable -- this class encapsulates the
//     symbol table structure which associates WIT data-format rules
//     with Wit API functions.
//
//------------------------------------------------------------------------------

#define  HASH_SIZE   120

class WitSymTable
{

  // Iterator for class SymTable.  Definition follows below.
  friend class WitSymTableIterator;

public:

   typedef std::map<std::string,WitSymTabEntry*> WitSymTableDict;

  // Constructors and destructors
  WitSymTable(WitRun *theRun);
  ~WitSymTable();

  // Operations
  int registerRules();

  // Accessors
  const WitSymTableDict&  theDictionary();
  static WitRun     *myWitRun();

  //---------------------------------------------------------------------------
  // API Wrapper functions
  //--------------------------------------------------------------------------

  static WitParseRC  witAddBomEntry();
  static WitParseRC  witAddBopEntry();
  static WitParseRC  witAddDemand();
  static WitParseRC  witAddPart();
  static WitParseRC  witAddPartWithOperation();
  static WitParseRC  witAddOperation();
  static WitParseRC  witAddSubEntry();

  static WitParseRC  witSetAccAfterOptImp();
  static WitParseRC  witSetAccAfterSoftLB();
  static WitParseRC  witSetAutoPriority();
  static WitParseRC  witSetCompPrices();
  static WitParseRC  witSetComputeCriticalList();
  static WitParseRC  witSetCplexParSpecDblVal();
  static WitParseRC  witSetCplexParSpecIntVal();
  static WitParseRC  witSetCplexParSpecName();
  static WitParseRC  witSetCurrentObjective();
  static WitParseRC  witSetCurrentScenario();
  static WitParseRC  witSetEquitability();
  static WitParseRC  witSetExecEmptyBom();
  static WitParseRC  witSetExpCutoff();
  static WitParseRC  witSetForcedMultiEq();
  static WitParseRC  witSetHashTableSize();
  static WitParseRC  witSetHighPrecisionWD();
  static WitParseRC  witSetIndependentOffsets();
  static WitParseRC  witSetLocalBuildAhead();
  static WitParseRC  witSetLocalMultiExec();
  static WitParseRC  witSetLotSizeTol();
  static WitParseRC  witSetMinimalExcess();
  static WitParseRC  witSetMipMode();
  static WitParseRC  witSetModHeurAlloc();
  static WitParseRC  witSetMultiExec();
  static WitParseRC  witSetMultiObjMode();
  static WitParseRC  witSetMultiObjTol();
  static WitParseRC  witSetMultiRoute();
  static WitParseRC  witSetNPeriods();
  static WitParseRC  witSetNScenarios();
  static WitParseRC  witSetNewPipPggOrder();
  static WitParseRC  witSetNonMrSelSplit();
  static WitParseRC  witSetNstnResidual();
  static WitParseRC  witSetObjChoice();
  static WitParseRC  witSetObjectiveListSpec();
  static WitParseRC  witSetObjectiveSeqNo();
  static WitParseRC  witSetOutputPrecision();
  static WitParseRC  witSetPenExec();
  static WitParseRC  witSetPeriodStage();
  static WitParseRC  witSetPerfPegging();
  static WitParseRC  witSetPeriodsPerYear();
  static WitParseRC  witSetPgdCritListMode();
  static WitParseRC  witSetPipSeqClearsNow();
  static WitParseRC  witSetPipSeqFromHeur();
  static WitParseRC  witSetPrefHighStockSLBs();
  static WitParseRC  witSetProbability();
  static WitParseRC  witSetRespectStockSLBs();
  static WitParseRC  witSetRoundReqVols();
  static WitParseRC  witSetSelSplit();
  static WitParseRC  witSetSelectionRecovery();
  static WitParseRC  witSetShipPerability();
  static WitParseRC  witSetSkipFailures();
  static WitParseRC  witSetSolverLogFileName();
  static WitParseRC  witSetStageByObject();
  static WitParseRC  witSetStochMode();
  static WitParseRC  witSetStockRealloc();
  static WitParseRC  witSetStockReallocation();
  static WitParseRC  witSetSrSelSplit();
  static WitParseRC  witSetTieBreakPropRt();
  static WitParseRC  witSetTitle();
  static WitParseRC  witSetTruncOffsets();
  static WitParseRC  witSetTwoWayMultiExec();
  static WitParseRC  witSetUseFocusHorizons();
  static WitParseRC  witSetUserHeurStart();
  static WitParseRC  witSetWbounds();
  static WitParseRC  witSetWit34Compatible();

  static WitParseRC  witSetPartAsapPipOrder();
  static WitParseRC  witSetPartBaSelSplit();
  static WitParseRC  witSetPartBoundedLeadTimes();
  static WitParseRC  witSetPartBuildAheadUB();
  static WitParseRC  witSetPartBuildAheadLimit();
  static WitParseRC  witSetPartBuildAsap();
  static WitParseRC  witSetPartBuildNstn();
  static WitParseRC  witSetPartObj1ScrapCost();
  static WitParseRC  witSetPartObj1StockCost();
  static WitParseRC  witSetPartObjectStage();
  static WitParseRC  witSetPartPropRouting();
  static WitParseRC  witSetPartPropRtg();
  static WitParseRC  witSetPartScrapAllowed();
  static WitParseRC  witSetPartScrapCost();
  static WitParseRC  witSetPartSingleSource();
  static WitParseRC  witSetPartStockBounds();
  static WitParseRC  witSetPartStockCost();
  static WitParseRC  witSetPartSupplyVol();
  static WitParseRC  witSetPartSelForDel();
  static WitParseRC  witSetPartUnitCost();

  static WitParseRC  witSetDemandBuildAheadLimit();
  static WitParseRC  witSetDemandBuildAheadUB();
  static WitParseRC  witSetDemandCumShipBounds();
  static WitParseRC  witSetDemandCumShipReward();
  static WitParseRC  witSetDemandDemandVol();
  static WitParseRC  witSetDemandFocusHorizon();
  static WitParseRC  witSetDemandGrossRev();
  static WitParseRC  witSetDemandIntShipVols();
  static WitParseRC  witSetDemandLeadTimeUB();
  static WitParseRC  witSetDemandObj1CumShipReward();
  static WitParseRC  witSetDemandObj1ShipReward();
  static WitParseRC  witSetDemandPipSeqIncShipVol();
  static WitParseRC  witSetDemandPipSeqShipPeriod();
  static WitParseRC  witSetDemandPrefBuildAhead();
  static WitParseRC  witSetDemandPriority();
  static WitParseRC  witSetDemandSearchInc();
  static WitParseRC  witSetDemandSelForDel();
  static WitParseRC  witSetDemandShipLateAllowed();
  static WitParseRC  witSetDemandShipLateLimit();
  static WitParseRC  witSetDemandShipLateUB();
  static WitParseRC  witSetDemandShipReward();
  static WitParseRC  witSetDemandShipVol();

  static WitParseRC  witSetOperationExecBounds();
  static WitParseRC  witSetOperationExecCost();
  static WitParseRC  witSetOperationExecPenalty();
  static WitParseRC  witSetOperationIncLotSize();
  static WitParseRC  witSetOperationIncLotSize2();
  static WitParseRC  witSetOperationIntExecVols();
  static WitParseRC  witSetOperationLotSize2Thresh();
  static WitParseRC  witSetOperationMinLotSize();
  static WitParseRC  witSetOperationMinLotSize2();
  static WitParseRC  witSetOperationObj1ExecCost();
  static WitParseRC  witSetOperationObjectStage();
  static WitParseRC  witSetOperationPipEnabled();
  static WitParseRC  witSetOperationPipRank();
  static WitParseRC  witSetOperationSelForDel();
  static WitParseRC  witSetOperationTwoLevelLotSizes();
  static WitParseRC  witSetOperationYield();
  static WitParseRC  witSetOperationYieldRate();
  static WitParseRC  witSetOperationCycleTime34();
  static WitParseRC  witSetOperationExecVol();

  static WitParseRC  witSetBomEntry();
  static WitParseRC  witSetBomEntryConsRate();
  static WitParseRC  witSetBomEntryEarliestPeriod();
  static WitParseRC  witSetBomEntryExecPenalty();
  static WitParseRC  witSetBomEntryFallout();
  static WitParseRC  witSetBomEntryFalloutRate();
  static WitParseRC  witSetBomEntryLatestPeriod();
  static WitParseRC  witSetBomEntryMandEC();
  static WitParseRC  witSetBomEntryOffset();
  static WitParseRC  witSetBomEntryPropRouting();
  static WitParseRC  witSetBomEntryPropRtg();
  static WitParseRC  witSetBomEntryRoutingShare();
  static WitParseRC  witSetBomEntrySelForDel();
  static WitParseRC  witSetBomEntrySingleSource();
  static WitParseRC  witSetBomEntryUsageRate();

  static WitParseRC  witSetSubEntry();
  static WitParseRC  witSetSubEntryConsRate();
  static WitParseRC  witSetSubEntryEarliestPeriod();
  static WitParseRC  witSetSubEntryExecPenalty();
  static WitParseRC  witSetSubEntryExpAllowed();
  static WitParseRC  witSetSubEntryExpNetAversion();
  static WitParseRC  witSetSubEntryFallout();
  static WitParseRC  witSetSubEntryFalloutRate();
  static WitParseRC  witSetSubEntryIntSubVols();
  static WitParseRC  witSetSubEntryLatestPeriod();
  static WitParseRC  witSetSubEntryMrpNetAllowed();
  static WitParseRC  witSetSubEntryNetAllowed();
  static WitParseRC  witSetSubEntryUsageRate();
  static WitParseRC  witSetSubEntryObj1SubCost();
  static WitParseRC  witSetSubEntryOffset();
  static WitParseRC  witSetSubEntryRoutingShare();
  static WitParseRC  witSetSubEntrySelForDel();
  static WitParseRC  witSetSubEntrySubCost();
  static WitParseRC  witSetSubEntrySubVol();

  static WitParseRC  witSetBopEntryByProduct();
  static WitParseRC  witSetBopEntryEarliestPeriod();
  static WitParseRC  witSetBopEntryExpAllowed();
  static WitParseRC  witSetBopEntryExpAversion();
  static WitParseRC  witSetBopEntryLatestPeriod();
  static WitParseRC  witSetBopEntryMeSelSplit();
  static WitParseRC  witSetBopEntryOffset();
  static WitParseRC  witSetBopEntryProdRate();
  static WitParseRC  witSetBopEntryProductRate();
  static WitParseRC  witSetBopEntryRoutingShare();
  static WitParseRC  witSetBopEntrySelForDel();

private:

  static WitSymTabEntry::witSymbolDef symbolTable_[];
  static WitRun *                     myWitRun_;

  WitSymTableDict                     theDictionary_;
};


// Iterator for SymTable class
class WitSymTableIterator
{

public:

  // Constructor and destructor
  WitSymTableIterator(WitSymTable *s);
  ~WitSymTableIterator();

  // Operations

  // Advance to next symbol table entry
  WitSymTabEntry::witSymbolDef  *operator ()();

private:

  WitSymTable * pst_;
  int           index_;
};

#endif     // SymTableH
