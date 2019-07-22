
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <mcl/src/mcl.h>
#include <sce/src/paramMgr.h>
#include <sce/src/sceDefin.h>
#include <sce/src/sceParam.h>

// this will map all pegging attributes to use pip
#define PEGGING_AS_PIP

// if we are using the new parameter processing method then...
#define NEW_PROCESS
// if we are not using the advanced parameters
#define ADV_PARAM_OUT


void
LgFrSceParameterManager::setErrFacility(MclFacility * existingFacility)
{
  // assign facility pointer to this pre-existing facility
  sceErrFacility_ = existingFacility;
}

MclFacility *
LgFrSceParameterManager::getErrFacility()
{
  return sceErrFacility_;
}

void
LgFrSceParameterManager::setIOLogMgr(LgFrSceIOLogMgr * existingMgr)
{
  sceIOLogMgr_ = existingMgr;
}

LgFrSceIOLogMgr*
LgFrSceParameterManager::getIOLogMgr()
{
  return sceIOLogMgr_;
}

void
LgFrSceParameterManager::initializeParameterDefaults(const std::string & paramFile,
                                                     LgFrSceSetOfParameters & defaultSet)
const
{
  // this seems sorta silly, but it's acutally handy to know what the name
  // of the original parameter file was
  defaultSet.setString("parameterFileName", paramFile, 0, false);
  // these are the default names for certain log files
  defaultSet.setString("parameterLogFileName", "param.log", 0, false);
  defaultSet.setString("inputLogFileName", "sceInput.log", 0, false);
#ifdef DOS_FILE_NAMES
  defaultSet.setString("outputLogFileName", "sceOut.log", 0, false);
#else
  defaultSet.setString("outputLogFileName", "sceOutput.log", 0, false);
#endif


  // 7.0
  // earlySupplyRule = T means accept pre-horizon supply inputs, F means reject
  defaultSet.setBool("earlySupplyRule", false, 0, false);

  // format of setString(string paramName, string defaultValue, 0 ,bool mandatoryFlag);
  //
  //
  // 6.20
  // CPLEX, MIP
  defaultSet.setBool("useCplexSolver",true,0,false); 
  defaultSet.setBool("mipMode",false,0,false); 
  defaultSet.setString("opIntExecFile","",0,false); 



  // 6.10
  defaultSet.setString("priorityRulesFile","",0,false); 
  defaultSet.setBool("useDailyCapAvailMethod",true,0,false); 
  defaultSet.setBool("useDatedDemands", false,0,false);
  defaultSet.setBool("useMultiAttributeDemand", false,0,false);
  defaultSet.setInt("numDemandAttributes", 4,0,false);
  defaultSet.setInt("demandTypeKeyFieldNum", 3,0,false);
  defaultSet.setString("multiAttributeDemandSeparator", "%",0,false);
  defaultSet.setBool("useDemand2OrderINDP", false,0,false);
  defaultSet.setBool("truncOffsetToEol", false,0,false);
  
#ifdef MULTI_SITE_BOM
  defaultSet.setString("multiSiteBomFile","",0,false);
#endif

  defaultSet.setBool("printMrpSubUsageFile", true, 0,false);
  defaultSet.setBool("printMrpOpSubUsageFile", true, 0,false);
  defaultSet.setBool("printSubUsageFile", true, 0,false);
  defaultSet.setBool("printOpSubUsageFile", true, 0,false);
  defaultSet.setBool("printObjectiveSummaryFile", false, 0,false);
  defaultSet.setBool("printProblemStatsFile", true, 0,false);

  defaultSet.setBool("printBacklogResponseFile", true,0,false);
  defaultSet.setBool("printBuildSchedFile", true, 0,false);


  defaultSet.setBool("printIndpSupplyResponseFile", true, 0,false);
  defaultSet.setBool("printSupplyResponseFile", true, 0,false);
  defaultSet.setBool("printInterplantOrdersFile", false, 0,false);
  defaultSet.setBool("printOperationOrdersFile", false, 0,false);

  // 5.00
  defaultSet.setBool("maxImpliedCommitsForAtp", false,0,false);
  defaultSet.setBool("supplierPerspectiveInterplantPeg", false,0,false);
  defaultSet.setString("greedySearchIncFile","",0,false); 
  defaultSet.setString("indpGreedySearchIncFile","",0,false); 
  defaultSet.setString("greedySearchIncrementMethod","default",0,false);   
  defaultSet.setFloat("searchIncScaleFactor", 0.01,0,false);
  defaultSet.setFloat("searchIncAbsValue", 1.0,0,false);

  defaultSet.setString("scrapCostFile","",0,false); 
  defaultSet.setString("stockCostFile","",0,false); 



  defaultSet.setString("sourceDefFile","",0,false); 
  defaultSet.setString("buildAheadFile","",0,false); 
  defaultSet.setBool("printImpactPeriodDiagnosticFile", false,0,false);
  defaultSet.setBool("printMrpImpactPeriodDiagnosticFile", false,0,false);
  defaultSet.setString("impactPeriodDiagFileExtension",        ".impactPd",0,false);
  defaultSet.setString("mrpImpactPeriodDiagFileExtension",        ".mrpImpactPd",0,false);

  defaultSet.setBool("printOperationExecFile", false,0,false);
  defaultSet.setString("operationExecFileExtension",        ".execVolDump",0,false);




  // 4.20: change these defaults for final build
  //  defaultSet.setString("smartExplodeEngine","heuristic",0,false);
  //  defaultSet.setString("smartExplodeEngine","heurAlloc",0,false);
  //  defaultSet.setBool("usePrioritizedExplodeUtility", true,0,false);
  defaultSet.setBool("usePrioritizedExplodeUtility", false,0,false);
  defaultSet.setBool("useStockReallocationInPrioExplode", false,0,false);

  defaultSet.setString("heurAllocExplodeTraceFileExtension", ".traceExp",0,false);
  defaultSet.setBool("printHeurAllocExplodeTrace", true,0,false);

  defaultSet.setString("globalSubParentIndString", "ALL",0,false);



  // Value Add Cost calculation services
  defaultSet.setBool("calculateVac", false, 0, false);
  defaultSet.setString("vacFileExtension", ".vac",0,false);
  defaultSet.setString("compCostExtension",".compCost",0,false);
  defaultSet.setBool("printVacFile", false,0,false);
  defaultSet.setBool("printCompCostFile", false,0,false);

  defaultSet.setBool("printSubDbFile", false,0,false);
  defaultSet.setString("subDbFileExtension", ".subDB",0,false);

  defaultSet.setBool("printPeggedCritListFile", false,0,false);
  defaultSet.setString("peggedCritListFileExtension", ".pegCrit",0,false);
  defaultSet.setString("iPeggedCritListFileExtension", ".iPegCrit",0,false);

  defaultSet.setBool("printAltDbFile", false,0,false);
  defaultSet.setString("altDbFileExtension", ".altDB",0,false);



  // PIP parameters
  // usePipForPeg is a swaperoo that tells sce to use the PIP function and
  // to populate any pegging outputs requested.  When TRUE, pegging will be shut off
  // in sce and pip is turned on.  And all the Peg extensions and printPeg params are
  // mapped to pip extensions and printPip params.
  //  defaultSet.setBool("usePipForPeg", false,0,false);
  defaultSet.setString("mrpIndpExecVolPegFileExtension",    ".iMrpExecVolPeg",0,false);
  defaultSet.setString(   "indpExecVolPegFileExtension",    ".iExecVolPeg",0,false);
  defaultSet.setString("mrpExecVolPegFileExtension",        ".mrpExecVolPeg",0,false);
  defaultSet.setString(   "execVolPegFileExtension",        ".execVolPeg",0,false);
  defaultSet.setString("mrpIndpInterplantPegFileExtension", ".iMrpInterplantPeg",0,false);
  defaultSet.setString(   "indpInterplantPegFileExtension", ".iInterplantPeg",0,false);
  defaultSet.setString("mrpInterplantPegFileExtension",     ".mrpInterplantPeg",0,false);
  defaultSet.setString(   "interplantPegFileExtension",     ".interplantPeg",0,false);
  defaultSet.setString("mrpIndpDemandConsPegFileExtension", ".iMrpDemConsPeg",0,false);
  defaultSet.setString(   "indpDemandConsPegFileExtension", ".iDemConsPeg",0,false);
  defaultSet.setString("mrpDemandConsPegFileExtension",     ".mrpDemConsPeg",0,false);
  defaultSet.setString(   "demandConsPegFileExtension",     ".demConsPeg",0,false);
  defaultSet.setString("mrpIndpSupplyPegFileExtension", ".iMrpSupplyPeg",0,false);
  defaultSet.setString(   "indpSupplyPegFileExtension", ".iSupplyPeg",0,false);
  defaultSet.setString("mrpSupplyPegFileExtension",     ".mrpSupplyPeg",0,false);
  defaultSet.setString(   "supplyPegFileExtension",     ".supplyPeg",0,false);
  defaultSet.setString("mrpProdVolPegFileExtension",        ".mrpProdVolPeg",0,false);
  defaultSet.setString(   "prodVolPegFileExtension",        ".prodVolPeg",0,false);
  defaultSet.setString("mrpIndpProdVolPegFileExtension",        ".iMrpProdVolPeg",0,false);
  defaultSet.setString(   "indpProdVolPegFileExtension",        ".iProdVolPeg",0,false);
  defaultSet.setString("mrpPfProdVolPegFileExtension",        ".mrpPfProdVolPeg",0,false);
  defaultSet.setString(   "pfProdVolPegFileExtension",        ".pfProdVolPeg",0,false);
  defaultSet.setString("mrpIndpPfProdVolPegFileExtension",        ".iMrpPfProdVolPeg",0,false);
  defaultSet.setString(   "indpPfProdVolPegFileExtension",        ".iPfProdVolPeg",0,false);
  defaultSet.setString("mrpSideVolPegFileExtension",        ".mrpSideVolPeg",0,false);
  defaultSet.setString(   "sideVolPegFileExtension",        ".sideVolPeg",0,false);
  defaultSet.setString("mrpIndpSideVolPegFileExtension",        ".iMrpSideVolPeg",0,false);
  defaultSet.setString(   "indpSideVolPegFileExtension",        ".iSideVolPeg",0,false);

  defaultSet.setBool("printMrpIndpExecVolPegFile", false,0,false);
  defaultSet.setBool("printMrpExecVolPegFile", false,0,false);
  defaultSet.setBool("printIndpExecVolPegFile", false,0,false);
  defaultSet.setBool("printExecVolPegFile", false,0,false);
  defaultSet.setBool("printMrpIndpInterplantPegFile", false,0,false);
  defaultSet.setBool("printMrpInterplantPegFile", false,0,false);
  defaultSet.setBool("printIndpInterplantPegFile", false,0,false);
  defaultSet.setBool("printInterplantPegFile", false,0,false);
  defaultSet.setBool("printMrpIndpDemandConsPegFile", false,0,false);
  defaultSet.setBool("printMrpDemandConsPegFile", false,0,false);
  defaultSet.setBool("printIndpDemandConsPegFile", false,0,false);
  defaultSet.setBool("printDemandConsPegFile", false,0,false);
  defaultSet.setBool("printMrpIndpSupplyPegFile", false,0,false);
  defaultSet.setBool("printMrpSupplyPegFile", false,0,false);
  defaultSet.setBool("printIndpSupplyPegFile", false,0,false);
  defaultSet.setBool("printSupplyPegFile", false,0,false);
  defaultSet.setBool("printMrpProdVolPegFile", false,0,false);
  defaultSet.setBool("printProdVolPegFile", false,0,false);
  defaultSet.setBool("printMrpIndpProdVolPegFile", false,0,false);
  defaultSet.setBool("printIndpProdVolPegFile", false,0,false);
  defaultSet.setBool("printMrpPfProdVolPegFile", false,0,false);
  defaultSet.setBool("printPfProdVolPegFile", false,0,false);
  defaultSet.setBool("printMrpIndpPfProdVolPegFile", false,0,false);
  defaultSet.setBool("printIndpPfProdVolPegFile", false,0,false);
  defaultSet.setBool("printMrpSideVolPegFile", false,0,false);
  defaultSet.setBool("printSideVolPegFile", false,0,false);
  defaultSet.setBool("printMrpIndpSideVolPegFile", false,0,false);
  defaultSet.setBool("printIndpSideVolPegFile", false,0,false);

  defaultSet.setBool("doPegging", false,0,false);
  defaultSet.setBool("PegSubUsageToPrimary", false,0,false);
  // END PIP

  defaultSet.setString("pipPriorityFile","",0,false);
  defaultSet.setString("pipIndpPriorityFile","",0,false);
  defaultSet.setBool("printCapacityUsageFile", false,0,false);
  defaultSet.setBool("printMrpCapacityUsageFile", false,0,false);
  defaultSet.setString("capacityUsageFileExtension", ".capUsage",0,false);
  defaultSet.setString("mrpCapacityUsageFileExtension", ".mrpCapUs",0,false);


  // begin: SCE 3.30 parameters
  // run MIP
  // as of SCE 4.50, this function no longer availabe in WIT, so we remove from SCE
  //  defaultSet.setBool("optWithLotSizes", FALSE,0,FALSE);
  defaultSet.setBool("capacityOffsetToZero", FALSE,0,FALSE);
  defaultSet.setBool("combineCapacityIntoPartDB", false,0,false);


  // begin: SCE 3.20 parameters
  defaultSet.setString("subUsageReportMode","parentProd",0,FALSE);
  defaultSet.setBool("globalSubNetAllowed", true,0,false);
  defaultSet.setBool("groupSharingViaExecPenalties", FALSE,0,FALSE);
  defaultSet.setFloat("groupSharingExecPenaltyThresh", 0.0,0,FALSE);


  // begin: SCE 3.11 rev04 parameters
  defaultSet.setBool("optInitFromZero", false,0,false);
#ifdef FORCE_BOGONS
  defaultSet.setBool("forcePcfBogons", false,0,false);
#endif
  defaultSet.setString("indepRevenueFile","",0,false);
  defaultSet.setBool("aggregateSupplyInScrub", false,0,false);
  defaultSet.setBool("printLowestLevelNonPcfFile", false,0,false);
  defaultSet.setString("lowestLevelNonPcfFileExtension",".llNonPcf",0,false);
  defaultSet.setBool("printPcfBogonsFile", false,0,false);
  defaultSet.setString("pcfBogonsFileExtension",".bogons",0,false);
  // end: SCE 3.11 rev04 parameters

  // begin: SCE 3.11 parameters
  defaultSet.setString("shadowPricesFileExtension",".shadowPrices",0,false);
  defaultSet.setBool("printShadowPricesFile", false,0,false);
  // end: SCE 3.11 parameters


  // begin: SCE 3.10 parameters
  defaultSet.setBool("shutOffAllLotSizing", false,0,false);
  defaultSet.setBool("disableInterplantJumperArcs", false,0,false);
  defaultSet.setBool("disableSplitBop", false,0,false);
  // end: SCE 3.10 parameters



  // begin: SCE 3.0
  defaultSet.setString("heurAllocTraceFileExtension", ".trace",0,false);
  defaultSet.setBool("printHeurAllocTrace", false,0,false);
  defaultSet.setString("heurAllocSortStrategy", "truePriority",0,false);
  defaultSet.setString("heurAllocMinsSortStrategy", "truePriority",0,false);
  defaultSet.setString("heurAllocFeatSortStrategy", "truePriority",0,false);
  defaultSet.setString("heurAllocBacklogPriority",  "memorylessRoll",0,false);

  defaultSet.setBool("useStockReallocation", false,0,false);
  // end: 3.0

  defaultSet.setBool("useFullExplodeUtility", true,0,false);
  defaultSet.setString("calendarFormat", "G",0,true);
  defaultSet.setString("defaultPdf", "XXX",0,false);
  defaultSet.setString("featureFileOutputFormat", "absolute",0,false);
  defaultSet.setString("pdfSeparator", "_",0,false);

  // filename things
  defaultSet.setString("fssSingletonFileExtension", ".fssSingletonReport",0,false);
  defaultSet.setString("fssGeneralSetFileExtension", ".fssGeneralSetReport",0,false);
  defaultSet.setString("fssMachineFileExtension", ".fssMachineReport",0,false);
  defaultSet.setString("backlogResponseFileExtension", ".backlog",0,false);
  defaultSet.setString("buildSchedFileExtension", ".buildSched",0,false);
  defaultSet.setString("capacityDBFileExtension", ".capacityDB",0,false);
  defaultSet.setString("criticalListFileExtension", ".critList",0,false);
  defaultSet.setString("debugFileExtension", ".sce.debug",0,false);
  defaultSet.setString("demandDriversFileExtension", ".demandDrivers",0,false);
  defaultSet.setString("explodeEchoFileExtension", ".witExplode.echo",0,false);
  defaultSet.setString("explodeWitDataFileExtension", ".witExplode.data",0,false);
  defaultSet.setString("formattedSupplyReportFileExtension", ".supplyReport",0,false);
  defaultSet.setString("geoSourceDebugFileExtension", ".geoSourceDebug",0,false);
  defaultSet.setString("geoSupplyByPdfFileExtension", ".geoSupplyByPdf",0,false);
  defaultSet.setString("heurEchoFileExtension", ".witHeur.echo",0,false);
  defaultSet.setString("heurWitDataFileExtension", ".witHeur.data",0,false);
  defaultSet.setString("indepSupplyResponseFileExtension", ".indSupResp",0,false);

  defaultSet.setString("operationDemandFileExtension", ".operationDemand",0,false);
  defaultSet.setString("operationOrdersFileExtension", ".operation",0,false);

  defaultSet.setString("interplantDemandFileExtension", ".interplantDemand",0,false);
  defaultSet.setString("interplantOrdersFileExtension", ".interplant",0,false);

  defaultSet.setString("mrpDemandOrdersFileExtension", ".mrpDemOrd",0,false);
  defaultSet.setString("mrpSupplyOrdersFileExtension", ".mrpSupOrd",0,false);
  defaultSet.setString("mrpSubUsageFileExtension", ".mrpSubUs",0,false);
  defaultSet.setString("mrpOpSubUsageFileExtension", ".mrpOpSubUs",0,false);
  defaultSet.setString("objectiveSummaryFileExtension", ".optResults",0,false);

  defaultSet.setString("opExecutionVolumeFileExtension", ".opExecVol",0,false);

  defaultSet.setString("operationDBFileExtension", ".operationDB",0,false);
  defaultSet.setString("opSubUsageFileExtension", ".opSubUsage",0,false);
  defaultSet.setString("optEchoFileExtension", ".witOpt.echo",0,false);
  defaultSet.setString("optWitDataFileExtension", ".witOpt.data",0,false);
  defaultSet.setString("optionalFeatRatioFileExtension", ".optionRatio",0,false);
  defaultSet.setString("outputFilePrefix", "./output",0,false);
  defaultSet.setString("partDBFileExtension", ".partDB",0,false);
  defaultSet.setString("problemStatisticsFileExtension", ".probStats",0,false);
  defaultSet.setString("selFeatBBratioFileExtension", ".bbRatio",0,false);
  defaultSet.setString("subUsageFileExtension", ".subUsage",0,false);
  defaultSet.setString("supplyResponseFileExtension", ".supplyResp",0,false);
  defaultSet.setString("totalDemandReportFileExtension", ".totalDemand",0,false);
  defaultSet.setString("witExplodeMsgFileExtension", ".witExplode.msg",0,false);
  defaultSet.setString("witMsgFileExtension", ".wit.msg",0,false);


  // some random extensions not included before
  defaultSet.setString("witRolloverMsgFileExtension", ".wit.rollover.msg",0,false);

  // Note: put all internal use only or un-documented parameters down here
  defaultSet.setBool("printModifiedFssMachReport", false,0,false);
  defaultSet.setString("FSSModifiedMachReportExtension", "FSSModMachReport",0,false);
  defaultSet.setString("featureRatioPegMethod", "pegToMachineShipPeriod",0,false); //
  defaultSet.setString("siteCustomization", "default",0,false);


  // NEW SCE 2.00 parameters
  defaultSet.setBool("doImplode", true,0,false);
  defaultSet.setBool("heurForceEquit", false,0,false);
  defaultSet.setBool("smartExplodeForceEquit", true,0,false);
  defaultSet.setBool("useLocalBuildAhead", false,0,false);
  defaultSet.setInt("buildAheadLimit", 0, 0,false);
  defaultSet.setBool("useSmartExplodeUtility", false,0,false);
  defaultSet.setInt("smartExplodeEquit", 10,0,false);

  defaultSet.setBool("demandDriversRespectsSubs", false,0,false);

  // NEW SCE 2.30 parameters
  defaultSet.setBool("useMultiRouteImplosion", true,0,false);
  defaultSet.setFloat("expCutoffTolerance", 0.01,0,false);
  defaultSet.setString("minsEngine", "heuristic",0,false);
  defaultSet.setBool("heurSkipFailures", true,0,false);

  //----------------------------------------------------------------------
  // Most other parameters - Mandatory or not
  // *default vals for type...
  // -string -> ""
  // -int    -> 0
  // -float  -> 0.0
  // -bool   -> false=0
  //----------------------------------------------------------------------

  // fss parameters
  defaultSet.setBool("fssFlag", false,0,false);

  defaultSet.setBool("doFssSingleton", 0,0,false);
  defaultSet.setBool("doFssGeneralSet", 0,0,false);
  defaultSet.setBool("doFssMachine", 0,0,false);

  defaultSet.setString("fssSingletonMethod", "liberal",0,false);    // liberal or conservative
  defaultSet.setString("fssGeneralSetMethod", "liberal",0,false);   // liberal or conservative
  defaultSet.setString("fssMachineMethod", "conservative",0,false); // liberal or conservative

  defaultSet.setString("fssFeatureSingletonMethod", "maximal",0,false);  //  minimal or maximal
  defaultSet.setString("fssFeatureGeneralSetMethod", "maximal",0,false); //  minimal or maximal
  defaultSet.setString("fssFeatureMachineMethod", "maximal",0,false);    //  minimal or maximal

  defaultSet.setInt("fssSingletonHorizon",0,0,false);
  defaultSet.setInt("fssGeneralSetHorizon",0,0,false);
  defaultSet.setInt("fssMachineHorizon",0,0,false);


  // non-mandatory other stuff
  defaultSet.setInt("defaultShipLateLimit",0,0,false);
  defaultSet.setInt("nStandaloneFeatureArcs",0,0,false);


  // mandatory other stuff
  defaultSet.setInt("numPeriods",0,0,true);
  defaultSet.setInt("objectiveType",0,0,true); // this value valid as {1,2,3} only
  defaultSet.setString("calendarFile","",0,true);
  defaultSet.setString("defaultPDF","",0,true);


  // mandatory input files

  defaultSet.setString("bomFile","",0,true);
  defaultSet.setString("demandVolFile","",0,false);
  defaultSet.setString("partCondFile","",0,false);
  defaultSet.setString("partFile","",0,true);
  defaultSet.setString("supplyFile","",0,true);


  // fss input files (sometimes mandatory)
  defaultSet.setString("fssGeneralSetFile","",0,false);  // this is sometimes mandatory
  defaultSet.setString("fssMachineFile","",0,false);  // this is sometimes mandatory


  // sometimes mandatory files
  defaultSet.setString("profitFile","",0,false);  // mandatory when objectiveType = 2
  defaultSet.setString("revenueFile","",0,false);  // mandatory when objectiveType = 2


  // non-mandatory input files
  defaultSet.setString("aggregationFile","",0,false);
  defaultSet.setString("backlogYieldFile","",0,false);
  defaultSet.setString("bocFile","",0,false);
  defaultSet.setString("capFile","",0,false);
  defaultSet.setString("capSupplyFile","",0,false);
  defaultSet.setString("cycleTimeFile","",0,false);
  defaultSet.setString("featProfitFile","",0,false);
  defaultSet.setString("featRevFile","",0,false);
  defaultSet.setString("geoSourceFile","",0,false);
  defaultSet.setString("interPlantFile","",0,false);
  defaultSet.setString("independentDemandFile","",0,false);
  defaultSet.setString("indepPriorityFile","",0,false);
  defaultSet.setString("lotSizeFile","",0,false);
  defaultSet.setString("minDemandVolFile","",0,false);
  defaultSet.setString("minIndepDemandVolFile","",0,false);
  defaultSet.setString("machineFeatureOffsetFile","",0,false);
  defaultSet.setString("machineFeatureSupplyFile","",0,false);
  defaultSet.setString("opBomFile","",0,false);
  defaultSet.setString("opBopFile","",0,false);
  defaultSet.setString("opFile","",0,false);
  defaultSet.setString("opLotSizeFile","",0,false);
  defaultSet.setString("opExecPenaltyFile","",0,false);
  defaultSet.setString("opExecCostFile","",0,false);
  defaultSet.setString("opLowerFile","",0,false);
  defaultSet.setString("opSubBomFile","",0,false);
  defaultSet.setString("opUpperFile","",0,false);
  defaultSet.setString("opYieldFile","",0,false);
  defaultSet.setString("optionProfitFile","",0,false);
  defaultSet.setString("optionRevFile","",0,false);
  defaultSet.setString("priorityFile","",0,false);
  defaultSet.setString("prodVolLBFile","",0,false);
  defaultSet.setString("prodVolUBFile","",0,false);
  defaultSet.setString("subBomFile","",0,false);
  defaultSet.setString("topLevelToBBdefFile","",0,false);
  defaultSet.setString("topLevelToBBratioFile","",0,false);
  defaultSet.setString("topLevelToOptionFile","",0,false);
  defaultSet.setString("yieldFile","",0,false);


  // print flags : non-mandatory
  defaultSet.setBool("printCapacityDBFile",0,0,false);
  defaultSet.setBool("printCriticalListFile",0,0,false);
  defaultSet.setBool("printMrpCriticalListFile",0,0,false);
  defaultSet.setBool("printDebugFile",0,0,false);
  defaultSet.setBool("printDemandDriversFile",0,0,false);
  defaultSet.setBool("printExplodeEchoFile",0,0,false);
  defaultSet.setBool("printExplodeWitDataFile",0,0,false);
  defaultSet.setBool("printFormattedSupplyReportFile",0,0,false);
  defaultSet.setBool("printGeoSourceDebugFile",0,0,false);
  defaultSet.setBool("printGeoSupplyByPdfFile",0,0,false);
  defaultSet.setBool("printHeurEchoFile",0,0,false);
  defaultSet.setBool("printHeurWitDataFile",0,0,false);
  defaultSet.setBool("printOperationDemandFile", false,0,false);
  defaultSet.setBool("printInterplantDemandFile", false,0,false);
  defaultSet.setBool("printMrpSupplyOrdersFile", false,0,false);
  defaultSet.setBool("printMrpDemandOrdersFile", false,0,false);
  defaultSet.setBool("printOperationDBFile",0,0,false);
  defaultSet.setBool("printOptEchoFile",0,0,false);
  defaultSet.setBool("printOptWitDataFile",0,0,false);
  defaultSet.setBool("printPartDBFile",0,0,false);
  defaultSet.setBool("printTotalDemandReport",0,0,false);
  defaultSet.setBool("printWitExplodeMsgFile",0,0,false);
  defaultSet.setBool("printWitMsgFile",0,0,false);
  defaultSet.setBool("printWitRolloverMsgFile",0,0,false);

  // IMPLOSION MODEL  parameters

  defaultSet.setString("engine","heuristic",0,false);

  // heuristic
  defaultSet.setInt("equitability", 1,0,false);
  defaultSet.setInt("defaultPriority",1000,0,false);
  defaultSet.setInt("defaultIndependentDemandPriority",2000,0,false);
  defaultSet.setFloat("defaultAggregateBopPreference", 0.0,0,false);
  defaultSet.setFloat("defaultDefaultBopPreference", 0.0,0,false);
  defaultSet.setFloat("defaultExplicitBopPreference", 0.0,0,false);
  defaultSet.setFloat("defaultInterPlantBopPreference", 0.0,0,false);

  // LP basic coeff's
  defaultSet.setFloat("defaultObj1ShipReward",             1000.0   ,0,false);
  defaultSet.setFloat("periodicInterestRate",                 0.013 ,0,false);
  defaultSet.setFloat("defaultObj1ExecCostRealOperations",    0.1   ,0,false);
  defaultSet.setFloat("obj1ExecCostDiscRateRealOperations",   0.001 ,0,false);
  defaultSet.setFloat("defaultObj1ProdCostRealParts",         0.1   ,0,false);
  defaultSet.setFloat("obj1ProdCostDiscRateRealParts",        0.001 ,0,false);
  defaultSet.setFloat("obj1DefaultStockCostRealParts",        0.0   ,0,false);
  defaultSet.setFloat("defaultSubPenalty",                   10.0   ,0,false);
  // LP features/bb coeff's
  defaultSet.setFloat("defaultOptionShipReward",              5.0   ,0,false);
  defaultSet.setFloat("featureSetScrapPenalty",           99999.9   ,0,false);
  defaultSet.setFloat("featureSetStockPenalty",           99999.9   ,0,false);
  defaultSet.setFloat("optionRatioSupplyStockPenalty",       10.0   ,0,false);

  // What are these?  are they obsolete?
  //  defaultSet.setInt("featureRatioFlag",0,0,false);



  // TEST do we need these!!!   They are coming out in the param.log and the
  // scrubbed param file.   The latter can cause problems.
}




void
LgFrSceParameterManager::initializeSetOfParameters(const std::string & paramFile,
                                                   LgFrSceSetOfParameters & setOfParameters)
{
  std::string name;
  std::string stringVal;
  int       intVal;
  float     floatVal;
  bool boolVal;

  bool exists;


  // we no longer store default values in a default set of parameters,
  // we will be keeping all values in a single set of parameters and then
  // override the ones that are in the parameter file.
  //
  // a couple of important notes:
  // 1) if you want to set a parameter later on, then make sure you pass
  //    the method a value = 1 for the 'MethodSet' argument....otherwise
  //    you will replace the old default value with a new one and the new
  //    default will not hold the original default value....if you do a
  //    user defined 'set' then it will hold the original default value
  //    regardless of how many times you reset the actual value being used
  // 2) if the user does not have a "mandatory" parameter in the parameter
  //    file, SCE will exit and give an error message that indicates that
  //    the user must explicitly define a value in the parameter file.
  //

  this->initializeParameterDefaults(paramFile, setOfParameters);

  processParameterFile(paramFile,setOfParameters);

}


//----------------------------------------------------------------------------
// new processParameterFile(..) method
//----------------------------------------------------------------------------

int
LgFrSceParameterManager::processParameterFile(const std::string & parameterFileName,
                                              LgFrSceSetOfParameters & setOfParameters)
{
    std::ifstream inFile;
    std::string parameter;  // Note: initialized to a 0 string
    std::string dataLine;
    std::string paramName;
    std::string paramValue;

    long goodCount=0;
    long badCount=0;
    long commentCount=0;
    long totalCount=0;

    inFile.open(parameterFileName.c_str());
    if (! inFile)  {
      (*sceErrFacility_)("SevereCannotOpenFile",MclArgList()
                         << "parameter"
                         << parameterFileName
                         << "");
    }


    // read till end of file
    while(inFile.peek() != EOF) {
      // read next line of parameter file
        std::getline( inFile, dataLine );
      totalCount++;

      // set up a tokenizer
      SCETokenizer next(dataLine);

#ifdef ALLOW_QUOTED_STRING_PARAMS
      bool areThereQuotes = dataLine.find_first_of('"') == std::string::npos ? false : true;
#endif

      // get the parameter Keyword
      paramName = next(" \t\n,");

      //  skip blanks or comments *
      if ((paramName.empty()) || (paramName.c_str()[0] == '*')) {
        commentCount++;
        continue;
      }
      else {
        // currParamType = {0=int; 1=float; 2=string; 3=bool}
        //      "        = -1 means ERROR
        int currParamType = setOfParameters.exists(paramName);

        if (currParamType != -1) {  // -1 means invalid param name!

#ifdef ALLOW_QUOTED_STRING_PARAMS
          if (areThereQuotes) {
            // pick up to end of line
            std::string restOfLine = next("\n");
            paramValue = sce::extractQuotedPart( restOfLine );

            //RWCRegexp quotedStringREGEXP("\".*\"");
            //std::string quotedParamValue = restOfLine(quotedStringREGEXP);
            //paramValue = sce::strip(quotedParamValue,  '\"');
          }
          else
            paramValue = next(" ,\t\n");
#else
          // try to grab the actual value
          paramValue = next(" ,\t\n");
#endif


          if (paramValue.empty()) {
            (*sceErrFacility_)("UsingDefaultParameter",MclArgList() << parameterFileName << (int)totalCount << paramName << dataLine);
            // this is just a warning... we will take default val ... user may
            // not want this
            badCount++;   // missing a parameter value!!! -- error message?
            continue;     // go to next line in param file
          }



          // flags for seeing if the parameter value read in can fit the
          // appropriate type (ie it *could* be of the type it should be)
          unsigned int isInt = 1;
          unsigned int isFloat = 1;
          unsigned int isString = 1;
          unsigned int isBool = 1;



      // Conditions for a float:
      // contains a point.
      // non-points must be digits.
      // must be at least 2 characters long.
      size_t firstPointOffset = paramValue.find_first_of( '.' );
      size_t secondPointOffset = paramValue.find_first_of( '.', firstPointOffset + 1 );
      bool containsOnePoint = ( firstPointOffset != std::string::npos &&
                                secondPointOffset == std::string::npos );
      bool allDigits = true;
      for( size_t i = 0; i < paramValue.size(); i++ )
        if( !isdigit( paramValue[i] ) && paramValue[i] != '.' )
          {
            allDigits = false;
            break;
          }
      if( !containsOnePoint || !allDigits || paramValue.size() < 2 )
        isFloat = 0;

      // Conditions for an int: must only digits, and at least one.
      if( containsOnePoint || !allDigits )
        isInt = 0;

      if ((currParamType == 0) && isInt) { // type = int
        long temp;
        sce::fromString(paramValue,temp);
        setOfParameters.setInt(paramName,temp,1,false);
        goodCount++;
        continue;
      }
      else if ((currParamType == 1) && isFloat) { // type = float
        double temp;
        sce::fromString(paramValue,temp);
        setOfParameters.setFloat(paramName,temp,1,false);
        goodCount++;
            continue;
      }
      else if (currParamType == 3) { // should be a bool
        if (isInt) { // it is a number
          long temp;
          sce::fromString(paramValue,temp);
          if (temp == 0)
            setOfParameters.setBool(paramName,false,1,false);
          else
            setOfParameters.setBool(paramName,true,1,false);
          goodCount++;
          continue;
        }
      }
      else if (currParamType == 2) {  // string parameter
#ifdef ALLOW_QUOTED_STRING_PARAMS
        if (0) {
          //RWCRegexp quotedStringREGEXP("^\".*\"");
          //std::string quotedParamValue = paramValue(quotedStringREGEXP);
          paramValue = sce::extractQuotedPart( paramValue );
          paramValue = sce::strip( paramValue, "\"");
        }
        setOfParameters.setString(paramName,paramValue,1,false);
#else
        setOfParameters.setString(paramName,paramValue,1,false);
#endif
        goodCount++;
        continue;
      }
      else {
        (*sceErrFacility_)("IllegalParameterEntry",MclArgList() << paramName << parameterFileName << (int)totalCount << dataLine);
        badCount++;
      }

        }
        else {
          (*sceErrFacility_)("PossibleInvalidParameter",MclArgList() << paramName << parameterFileName << (int)totalCount << dataLine);
          badCount++;
        }

      }

    }

    inFile.close();



    //-------------------------------------------------------------------------
    // ** These are the parameters whose default values depended on other
    //    parameters read in from parameter file
    //-------------------------------------------------------------------------
    int numPeriods = setOfParameters.getInt("numPeriods");

    // Fix the FSS horizon parameters (default is numPeriods-1)
    int tempParamValue;
    int tempMethodSet;








    // fssSingletonHorizon
    tempParamValue = setOfParameters.getInt("fssSingletonHorizon");
    if (tempParamValue >= numPeriods)
      (*sceErrFacility_)("ParamOutOfRange",MclArgList()
                         << "fssSingletonHorizon"
                         << parameterFileName
                         << "Must be less than numPeriods");
    tempMethodSet = setOfParameters.getMethodSet("fssSingletonHorizon");
    setOfParameters.setInt("fssSingletonHorizon", numPeriods-1, 0, false);
    if(tempMethodSet == 1)
       setOfParameters.setInt("fssSingletonHorizon", tempParamValue, 1, false);

    // fssGeneralSetHorizon
    tempParamValue = setOfParameters.getInt("fssGeneralSetHorizon");
    if (tempParamValue >= numPeriods)
      (*sceErrFacility_)("ParamOutOfRange",MclArgList()
                         << "fssGeneralSetHorizon"
                         << parameterFileName
                         << "Must be less than numPeriods");
    tempMethodSet = setOfParameters.getMethodSet("fssGeneralSetHorizon");
    setOfParameters.setInt("fssGeneralSetHorizon", numPeriods-1, 0, false);
    if(tempMethodSet == 1)
       setOfParameters.setInt("fssGeneralSetHorizon", tempParamValue, 1, false);

    // fssMachineHorizon
    tempParamValue = setOfParameters.getInt("fssMachineHorizon");
    if (tempParamValue >= numPeriods)
      (*sceErrFacility_)("ParamOutOfRange",MclArgList()
                         << "fssMachineHorizon"
                         << parameterFileName
                         << "Must be less than numPeriods");
    tempMethodSet = setOfParameters.getMethodSet("fssMachineHorizon");
    setOfParameters.setInt("fssMachineHorizon", numPeriods-1, 0, false);
    if(tempMethodSet == 1)
       setOfParameters.setInt("fssMachineHorizon", tempParamValue, 1, false);

    // buildAheadLimit
    tempParamValue = setOfParameters.getInt("buildAheadLimit");
    if (tempParamValue >= numPeriods)
      (*sceErrFacility_)("ParamOutOfRange",MclArgList()
                         << "buildAheadLimit"
                         << parameterFileName
                         << "Must be less than numPeriods");
    tempMethodSet = setOfParameters.getMethodSet("buildAheadLimit");
    setOfParameters.setInt("buildAheadLimit", numPeriods-1, 0, false);
    if(tempMethodSet == 1)
       setOfParameters.setInt("buildAheadLimit", tempParamValue, 1, false);





    tempParamValue = setOfParameters.getInt("defaultShipLateLimit");
    if (tempParamValue >= numPeriods){
      (*sceErrFacility_)("ParamOutOfRange",MclArgList()
                         << "defaultShipLateLimit"
                         << parameterFileName
                         << "Must be less than numPeriods");
    }
    tempMethodSet = setOfParameters.getMethodSet("defaultShipLateLimit");

    // default value from this param = numPeriods - 1
    setOfParameters.setInt("defaultShipLateLimit",
                           numPeriods - 1,
                           0, false);
    if(tempMethodSet == 1)
       setOfParameters.setInt("defaultShipLateLimit", tempParamValue, 1, false);

    //-------------------------------------------------------------------------


    //-------------------------------------------------------------------------
    // ** These are the parameters whose values must be within a valid range
    //    or in a certain domain... make sure these are valid!!
    //-------------------------------------------------------------------------
    std::string tempStringValue;
    int tempIntValue;

    // calendarFormat : {U,E,G}
    // 2.11 19980407 rev05 FIX:
    // we are doing away with this parameter.
    tempStringValue = setOfParameters.getString("calendarFormat");
    if (! (tempStringValue == "G")) {
      (*sceErrFacility_)("ParamOutOfRange",MclArgList()
                         << "calendarFormat"
                         << parameterFileName
                         << "Must be: {G} -- NOTE: Case-Sensitive.");
    }

    // objectiveType : {1,2,3}
    tempIntValue = setOfParameters.getInt("objectiveType");
    if (!(tempIntValue == 1 || tempIntValue == 2 || tempIntValue == 3)) {
      (*sceErrFacility_)("ParamOutOfRange",MclArgList()
                         << "objectiveType"
                         << parameterFileName
                         << "Must be one of the following: {1,2,3}");
    }


    // objectiveType : subUsageReportMode
    tempStringValue = setOfParameters.getString("subUsageReportMode");
    if (!(tempStringValue == "parentProd" || tempStringValue == "primaryCoverage" || tempStringValue == "subUsage")) {
      (*sceErrFacility_)("ParamOutOfRange",MclArgList()
                         << "subUsageReportMode"
                         << parameterFileName
                         << "Must be one of the following: {parentProd, primaryCoverage, subUsage)");
    }

    //-------------------------------------------------------------------------

    // one check before we are done to see if any of the mandatory
    // parameters still have the methodSet flag indicating default-defined
    // which we do NOT want.  if this is the case, we will provide a
    // fatal error message
    size_t numEntries = setOfParameters.numParameters(); // how many params?

    // loop thru all parameters in sop
    int q = 0; // Pulled out of the for below by RW2STL
    for (q=0; q<numEntries; q++) {
      LgFrSceParameter temp = setOfParameters(q);  // temp handle of a param
      if (temp.getMandatory() && (temp.getMethodSet() == 0)) {
        // we DO NOT want this! ie. it is supposed to be user-defined but
        // is not.
        (*sceErrFacility_)("MandatoryParameterMissing",MclArgList() << parameterFileName << temp.getName());
      }
    }

    // this is for producing a SCE i/o log entry later
    parameterFileName_ = parameterFileName;
    goodCount_ = goodCount;
    badCount_ = badCount;
    commentCount_ = commentCount;
    totalCount_ = totalCount;

    // choose proper return value
    if (badCount > 0) {
      return 0;
    }
    else {
      return 1;
    }
}



//----------------------------------------------------------------------------
// old search methods -- used to search through param file -- no longer used
//----------------------------------------------------------------------------

const
float
LgFrSceParameterManager::searchParamFileFloat(
    bool & exists,
    const std::string & parameterFile,
    const std::string & targetParameter,
    const int mandatory)
const
{
  double theValD = 0.0;
  std::string paramAsString = searchParamFile(exists,
                                            parameterFile,
                                            targetParameter,
                                            mandatory);
  if (! paramAsString.empty()) {
    if (! sce::fromString(paramAsString, theValD))  {
      (*sceErrFacility_)("IllegalParameterEntry",MclArgList() << targetParameter << parameterFile << 0);
    }
  }

  float theVal = (float) theValD;

  return theVal;
}




const
std::string
LgFrSceParameterManager::searchParamFile(
    bool & exists,
    const std::string & parameterFile,
    const std::string & targetParameter,
    const int mandatory)
const
{
    std::ifstream inFile;
    std::string parameter;  // Note: initialized to a 0 string
    std::string dataLine;
    std::string paramKeyword;

    inFile.open(parameterFile.c_str());


    if (! inFile)  {
      (*sceErrFacility_)("SevereCannotOpenFile",MclArgList() << "parameter" << parameterFile << "");
    }

    // initialize doesParamterExist to false
    exists = false;

    while(inFile.peek() != EOF) {

      // read next line of parameter file
      std::getline( inFile, dataLine );

      // set up a tokenizer
      SCETokenizer next(dataLine);

      // get the parameter Keyword
      paramKeyword = next(" \t\n,");

      //  skip blanks or comments *
      if ((paramKeyword.empty()) || paramKeyword == "*")
        continue;

      // did we find the match?
      if (sce::compareStringsCaseIns( paramKeyword, targetParameter ) == 0)  {
        parameter = next(" \t\n,");
        // if we found the match, then test to see if there's a parameter
        if (parameter.empty())  {
          (*sceErrFacility_)("MandatoryParameterMissing",MclArgList() << parameterFile << targetParameter);
        }
        exists = true;
        break;
      }
    }


    // if its a mandatory parameter, make sure its found
    if (mandatory==MANDATORY && parameter.empty())   {
          (*sceErrFacility_)("MandatoryParameterMissing",MclArgList() << parameterFile << targetParameter);
    }

    inFile.close();

    return parameter;

}

// ---------------------------------------------------------------
const
int
LgFrSceParameterManager::searchParamFileInt(
    bool & exists,
    const std::string & parameterFile,
    const std::string & targetParameter,
    const int mandatory,
    const int defaultVal)
const
{

  long theValL = (long) defaultVal;

  std::string paramAsString = searchParamFile(exists,
                                            parameterFile,
                                            targetParameter,
                                            mandatory);

  if (! paramAsString.empty()) {

    if (! sce::fromString(paramAsString, theValL))  {
      (*sceErrFacility_)("IllegalParameterEntry",MclArgList() << targetParameter << parameterFile << 0);
    }
  }

  int theVal = (int) theValL;

  return theVal;
}


// ---------------------------------------------------------------
const
bool
LgFrSceParameterManager::searchParamFileBoolean(
    bool & exists,
    const std::string & parameterFile,
    const std::string & targetParameter,
    const int mandatory)
const
{

    std::string paramAsString = searchParamFile(exists,
                                              parameterFile,
                                              targetParameter,
                                              mandatory);

    if (paramAsString == "1")
        return 1;

    return 0;
}







bool
LgFrSceParameterManager::doesInputFileExist(
    const std::string & targetFile)
{
    std::ifstream inFile;

    inFile.open(targetFile.c_str());

    if (inFile)  {
        inFile.close();
        return true;
    }

    return false;
}


// Destructor
LgFrSceParameterManager::~LgFrSceParameterManager()
{
  // putting this here because, this cannot go in the processParameterFile
  // method due to a sort of circular dependence in ipSce.C
  // ...quick explanation: we need to have already processed the param file
  // in order for the IOLogMgr to know the names of the IO log files it creates,
  // but we need to have already reset these log files before we print anything
  // to them, including the input log entry for the parameter file which is
  // what we are printing here.
  sceIOLogMgr_->printInputLog(parameterFileName_,
                              totalCount_,
                              goodCount_,
                              badCount_,
                              commentCount_,
                              goodCount_);
}


// Assignment operator.
LgFrSceParameterManager&
LgFrSceParameterManager::operator=(
   const LgFrSceParameterManager& rhs)
{

   if (this != &rhs) {           // Check for assignment to self
     sceErrFacility_ = rhs.sceErrFacility_;
     sceIOLogMgr_ = rhs.sceIOLogMgr_;
     parameterFileName_ = rhs.parameterFileName_;
     goodCount_ = rhs.goodCount_;
     badCount_ = rhs.badCount_;
     commentCount_ = rhs.commentCount_;
     totalCount_ = rhs.totalCount_;
   }

   return *this;
}


//  copy constructor
LgFrSceParameterManager::LgFrSceParameterManager(
   const LgFrSceParameterManager& source )
  : sceIOLogMgr_(source.sceIOLogMgr_),
    sceErrFacility_(source.sceErrFacility_)
{
  // Nothing to do here
}

// Default constructor
LgFrSceParameterManager::LgFrSceParameterManager()
  : sceIOLogMgr_(0),
    sceErrFacility_(0)
{
  // Nothing to do here
}


#if 0

#ifdef NDEBUG
#undef NDEBUG
#endif

void
LgFrSceParameterManager::test()
{




}
#endif
