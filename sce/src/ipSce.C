#include <mcl/src/mdsPragma.h>
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
#include <mcl/src/mcl.h>
#include <sce/src/ipSce.h>
#include <assert.h>
#include <math.h>
#include <wit/src/wit.h>
#include <scenario/src/timeVecs.h>

#include <scenario/src/difEssSc.h>
#include <scenario/src/sOfParts.h>
#include <scenario/src/sOfDemds.h>
#include <scenario/src/generVc.h>
#include <scenario/src/scenario.h>
#include <scenario/src/idnumber.h>

#include <sce/src/scenSce.h>
#include <scenario/src/calendar.h>
#include <sce/src/mPlantH.h>
#include <sce/src/sceCrtLs.h>
#include <sce/src/oEngMgr.h>
#include <sce/src/dDriver.h>
#include <sce/src/hEngMgr.h>
#include <sce/src/priorMgr.h>
#include <sce/src/difSceF.h>
#include <sce/src/dmApData.h>
#include <sce/src/bopAData.h>
#include <sce/src/sceBop.h> 
#include <sce/src/sceParam.h>
#include <sce/src/paramMgr.h>
#include <sce/src/appData.h>

#include <mcl/src/mcl.h>

#define IPSCE_FLT_EPS  0.00001



#ifndef ELIMINATE_OLD_MAIN
// ============================================================
// ========= buildTheWitModelForSce() =========================
// ============================================================
void   
LgFrInitialProblemForSce::buildTheWitModelForSce(
    WitRun * const theWitRun, 
    LgFrCalendar & theCal,
    LgFrMultiPlantHelper & mpHelper)
{

    int nPeriods;              // Number of periods in model

    LgFrSceSetOfParameters & setOfParameters =
      this->sceScenario()->setOfParameters();

    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    int nPeriodsInCal =  setTheCalendar(theCal);
    if (!theCal.isValid()) {
      // this is an error ... formerly an assertion
      (*theErrFacil)("InvalidCalendar");
    }

    // initialize the multiPlantHelper -- set the default PDF
    std::string defaultPdf = setOfParameters.getString("defaultPdf");
    mpHelper.defaultPdf(defaultPdf);

    // initialize the multiPlantHelper -- set the default PDF separator char
    std::string pdfSeparator = setOfParameters.getString("pdfSeparator");
    mpHelper.pdfSeparator(pdfSeparator);

    // Regardless of the engine, the priorities are loaded 
    // (during optimization, they're used in the hot start). 
    // But the obj1 values are set only if engine=optimization
    // to reduce the SCE footprint.
    //
    // So, new a heurModelHelper regardless of the engine.
    // But, only new a lpModelHelper if need be.
    //
    // Testing if (lpModelHelper != 0) is equivalent
    // to testing if (engine != optimization), and
    // is better that #ifdef's.
    // This "extra layer" is helpful so we don't have to keep
    // searching through the setOfParameters and for collecting
    // model specific functions.
    //
    // But once a design is established for the scheme/sub-schema
    // of current and wish-list implosion option, revisit
    // the modelHelpers...something may make more sense and
    // be more maintainable.
   


    // Define the demandDrivers object.  This object gets instantiated
    // after the model is built
    LgFrSceDemandDriver * myDemandDriver = 0;


    // we new a helper here and delete it in the ip's destructor
    std::string engine = setOfParameters.getString("engine");
    heurModelHelper_ = new LgFrSceHeurModelHelper( setOfParameters );

    if (engine == "optimization")  
      lpModelHelper_ = new LgFrSceLpModelHelper( setOfParameters );

    // Set the wit message file
    std::string scenarioFile = setOfParameters.getString("outputFilePrefix");
    std::string witMsgFileExtension = setOfParameters.getString("witMsgFileExtension");    

    std::string witMesgFileName(scenarioFile + witMsgFileExtension);
    witSetMesgFileAccessMode( theWitRun, WitTRUE, "w" );
    witSetMesgFileName( theWitRun, WitTRUE, witMesgFileName.c_str());

    if (setOfParameters.getBool("printWitMsgFile"))  
      setWitInformationalMessaging(theWitRun, SCE_WIT_MESSAGING_ON);
    else
      setWitInformationalMessaging(theWitRun, SCE_WIT_MESSAGING_OFF);
    
    // dated demands
    // multi_attribute_demand_class
    // need to get these parameters and set them in flatFileManager
    LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
    bool useDatedDemands  = setOfParameters.getBool("useDatedDemands");
    bool useMultiAttributeDemand  = setOfParameters.getBool("useMultiAttributeDemand");
    int demandTypeKeyFieldNum  = setOfParameters.getInt("demandTypeKeyFieldNum");
    bool truncOffsetToEol  = setOfParameters.getBool("truncOffsetToEol");
    int  numDemandAttributes  = setOfParameters.getInt("numDemandAttributes");
    std::string multiAttributeDemandSeparator  = setOfParameters.getString("multiAttributeDemandSeparator");
    bool useDemand2OrderINDP  = setOfParameters.getBool("useDemand2OrderINDP");

    flatFileManager.useDatedDemands(useDatedDemands);
    flatFileManager.useMultiAttributeDemand(useMultiAttributeDemand);
    flatFileManager.demandTypeKeyFieldNum(demandTypeKeyFieldNum);
    flatFileManager.numDemandAttributes(numDemandAttributes);
    flatFileManager.multiAttributeDemandSeparator(multiAttributeDemandSeparator);
    //    flatFileManager.useDemand2OrderINDP(useDemand2OrderINDP);

    mpHelper.useMultiAttributeDemand(useMultiAttributeDemand);
    mpHelper.truncOffsetToEol(truncOffsetToEol);
    mpHelper.numDemandAttributes(numDemandAttributes);
    mpHelper.multiAttributeDemandSeparator(multiAttributeDemandSeparator);
    mpHelper.useDemand2OrderINDP(useDemand2OrderINDP);

    witInitialize( theWitRun );
    witSetWit34Compatible( theWitRun, WitTRUE );
    witSetRoundReqVols ( theWitRun, WitTRUE );


    //  2.30: multi-route 
    bool useMultiRouteImplosion = setOfParameters.getBool("useMultiRouteImplosion");
    witSetMultiRoute(theWitRun, useMultiRouteImplosion);

    //  3.00: stock reallocation
    bool useStockReallocation = setOfParameters.getBool("useStockReallocation");
    witSetStockReallocation(theWitRun, useStockReallocation);


    groupSharingViaExecPenalties_ = setOfParameters.getBool("groupSharingViaExecPenalties");
    float groupSharingExecPenaltyThresh = setOfParameters.getFloat("groupSharingExecPenaltyThresh");
    witSetPenExec(theWitRun, groupSharingViaExecPenalties_);
  
    // notes for Pegged Critical List:   After multi-route (with penalized execution) tries everything and fails,
    // it reverts to a single path.   The single path is determined by lowest aversion.   For
    // us to force the critical list to LST in servergroup, we need to set the aversion of LST to 
    // large negative ... TEST!!!!!!!!!!


    //    {
    //      float * memErrorTest = new float[10];
    //      float p = memErrorTest[10];
    //    }



    // 2.30: expCutoffTolerance
    float expCutoffTolerance = setOfParameters.getFloat("expCutoffTolerance");
    witSetExpCutoff(theWitRun, expCutoffTolerance);

    // this really means set buildNSTN true for all parts (as a default)
    bool useLocalBuildAhead = setOfParameters.getBool("useLocalBuildAhead");

    // allow Subs to have offsets different than the parent bom
    witSetIndependentOffsets(theWitRun, WitTRUE);


    // Set the objective choice based on the engine parameter.
    // This gives us a smaller footprint when using the heuristic.
    if ( lpModelHelper_ != 0 )
      witSetObjChoice(theWitRun, 1);
    else 
      witSetObjChoice( theWitRun, 0);
    
    witSetNPeriods(theWitRun, nPeriodsInCal);

    witSetExecEmptyBom(theWitRun, WitTRUE);


    // temp for JP
#if 0
    WitRun * tempWitRun;
    witNewRun(&tempWitRun);
    witInitialize(tempWitRun);
    witReadData(tempWitRun, "wit.dat");
    witHeurImplode(tempWitRun);
#endif

    // Set the equitability factor
    int equitability = setOfParameters.getInt("equitability");
    witSetEquitability(theWitRun, equitability);

    bool heurForceEquit = setOfParameters.getBool("heurForceEquit");
    witSetForcedMultiEq(theWitRun, heurForceEquit);

    bool heurSkipFailures = setOfParameters.getBool("heurSkipFailures");
    witSetSkipFailures(theWitRun, heurSkipFailures);

    std::string siteCustomization = setOfParameters.getString("siteCustomization");

    bool doPegging = setOfParameters.getBool("doPegging");
    if (doPegging) {
      if (engine != "optimization") {
	// for all  engine choices except optimization, we allow use heuristic sequence for PIPing
	witClearPipSeq(theWitRun);
	witSetPipSeqFromHeur(theWitRun, WitTRUE);
      }
    }
    
    // Do we compute criticalList?
    if (setOfParameters.getBool("printCriticalListFile"))
      witSetComputeCriticalList(theWitRun, WitTRUE);

    // Do we compute pegged criticalList?
    if (setOfParameters.getBool("printPeggedCritListFile"))
      witSetPgdCritListMode(theWitRun, WitTRUE);


    // Tell wit to comput shadow prices
    if (setOfParameters.getBool("printShadowPricesFile"))
      witSetCompPrices(theWitRun, WitTRUE);

    // 6.20 MIP mode  
    if (setOfParameters.getBool("mipMode"))   {
      if (setOfParameters.getBool("printCriticalListFile"))      
	(*theErrFacil)("MipModeAndCritListDontMix");
      if (setOfParameters.getBool("printShadowPricesFile"))
	(*theErrFacil)("MipModeAndShadowProcesDontMix");
      witSetMipMode(theWitRun, WitTRUE);
    }
	



    // 4.20 IPIP fix 
    witSetNewPipPggOrder(theWitRun, WitTRUE);

    
    // Get number of periods 
    witGetNPeriods( theWitRun, &nPeriods );
    assert (nPeriods == nPeriodsInCal);


    // ***********************
    // ***  Part Def file  ***
    // ***********************
    std::string partDefFile = setOfParameters.getString("partFile");
    processPartDefinitionFile(theWitRun, partDefFile, mpHelper);


    // ***************************
    // ***  Capacity Def file  ***
    // ***************************
    std::string capacityDefFile = setOfParameters.getString("capFile");
    if (! capacityDefFile.empty())
      processCapacityDefinitionFile( theWitRun, capacityDefFile, mpHelper);


    // ***********************
    // ***  Sourcing File  ***
    // ***********************
    std::string sourceDefFile = setOfParameters.getString("sourceDefFile");
    if (! sourceDefFile.empty())
      processSourceDefFile(theWitRun, theCal, sourceDefFile, mpHelper);


    // *************************
    // ***  Cycle Time file  ***
    // *************************
    std::string cycleTimeFile = setOfParameters.getString("cycleTimeFile");    
    if (! cycleTimeFile.empty())
	processCycleTimeFile(theWitRun, nPeriods, theCal, cycleTimeFile, mpHelper);

    // *************************
    // ***  Yield file  ********
    // *************************
    std::string yieldFile = setOfParameters.getString("yieldFile");    
    if (! yieldFile.empty())
	processYieldFile( theWitRun, nPeriods, theCal, yieldFile, mpHelper);

    
    // *************************
    // *** Min Lot Size file ***
    // *************************
    std::string lotSizeFile = setOfParameters.getString("lotSizeFile");    
    if (! lotSizeFile.empty()) 
	processLotSizeFile( theWitRun, nPeriods, theCal, lotSizeFile, mpHelper);
       

    // ***********************************
    // *** Prod Vol Soft Lower Bounds  ***
    // ***********************************
    std::string prodVolLBFile = setOfParameters.getString("prodVolLBFile");    
    if (! prodVolLBFile.empty()) 
	processProdVolLBFile( theWitRun, nPeriods, prodVolLBFile, mpHelper);

    // ***********************************
    // *** Prod Vol Hard Upper Bounds  ***
    // ***********************************
    std::string prodVolUBFile = setOfParameters.getString("prodVolUBFile");    
    if (! prodVolUBFile.empty()) 
	processProdVolUBFile( theWitRun, nPeriods, prodVolUBFile, mpHelper);

    // *****************************************************
    // *** Set up Operations                   *************
    // *****************************************************
    std::string operationDefFile = setOfParameters.getString("opFile");
    if (! operationDefFile.empty())    
      processOperationDefinitionFile(theWitRun, operationDefFile, mpHelper);
    
    // *****************************************************
    // *** Set Operation ExecPenalty           *************
    // *****************************************************
    std::string operationExecPenaltyFile = setOfParameters.getString("opExecPenaltyFile");
    if (! operationExecPenaltyFile.empty())    
      processOperationExecPenaltyFile( theWitRun, nPeriods, theCal, operationExecPenaltyFile, mpHelper);

    // *****************************************************
    // *** Set Operation ExecCosts             *************
    // *****************************************************
    int sceObjectiveType = setOfParameters.getInt("objectiveType");
    std::string operationExecCostFile = setOfParameters.getString("opExecCostFile");
    if (! operationExecCostFile.empty()) {
      if (sceObjectiveType == 1) {
	(*theErrFacil)("ExecCostInHeurMode");
      }
      else 
	processOperationExecCostFile( theWitRun, nPeriods, theCal, operationExecCostFile, mpHelper);
    }


    // *****************************************************
    // *** Set Operation Lot Sizes             *************
    // *****************************************************
    std::string operationLotSizeFile = setOfParameters.getString("opLotSizeFile");
    if (! operationLotSizeFile.empty())    
      processOperationLotSizeFile( theWitRun, nPeriods, theCal, operationLotSizeFile, mpHelper);
       
    // *****************************************************
    // *** Set up Operation Yields             *************
    // *****************************************************
    std::string operationYieldFile = setOfParameters.getString("opYieldFile");
    if (! operationYieldFile.empty())    
      processOperationYieldFile( theWitRun, nPeriods, theCal, operationYieldFile, mpHelper);

    
    // *****************************************************
    // *** Set up Operation Lower Bounds File  *************
    // *****************************************************
    std::string operationLBFile = setOfParameters.getString("opLowerFile");
    if (! operationLBFile.empty())    
      processOperationLBFile( theWitRun, nPeriods, operationLBFile, mpHelper );    

    // *****************************************************
    // *** Set up Operation Upper Bounds       *************
    // *****************************************************
    std::string operationUBFile = setOfParameters.getString("opUpperFile");
    if (! operationUBFile.empty())    
      processOperationUBFile( theWitRun, nPeriods, operationUBFile, mpHelper );    

    // *****************************************************
    // *** Set up Operation Integer Exec Vols  *************
    // *****************************************************
    std::string operationIntExecFile = setOfParameters.getString("opIntExecFile");
    if (! operationIntExecFile.empty())    
      processOperationIntegerExecFile(theWitRun, nPeriods, theCal,  operationIntExecFile, mpHelper);    



    // *****************************************************
    // *** Set up Operation BOMs               *************
    // *****************************************************
     std::string operationBomFile = setOfParameters.getString("opBomFile");
     if (! operationBomFile.empty())    
       processOperationBomFile( theWitRun, theCal, operationBomFile, mpHelper);    

    // *****************************************************
    // *** Set up Operation BOPs               *************
    // *****************************************************
     std::string operationBopFile = setOfParameters.getString("opBopFile");
     if (! operationBopFile.empty())    
       processOperationBopFile( theWitRun, theCal, operationBopFile, mpHelper );    


    // ******************
    // ***  BOM file  ***
    // ******************
    std::string bomFile = setOfParameters.getString("bomFile");
    processStandardBomFile( theWitRun, theCal, bomFile, mpHelper);


#ifdef MULTI_SITE_BOM
    // ******************
    // ***  BOM file  ***
    // ******************
    std::string multiSiteBomFile = setOfParameters.getString("multiSiteBomFile");
    processMultiSiteBomFile( theWitRun, theCal, multiSiteBomFile, mpHelper);
#endif



    // *************************
    // ***  InterPlant file  ***
    // *************************
    std::string interPlantFile = setOfParameters.getString("interPlantFile");
    if (! interPlantFile.empty())
      processInterPlantFile( theWitRun, theCal, interPlantFile, mpHelper);


    // ******************
    // ***  BOC file  ***
    // ******************
    std::string bocFile = setOfParameters.getString("bocFile");
    if (! bocFile.empty())
      processBillOfCapacitiesFile( theWitRun, theCal, bocFile, mpHelper);

    // ***************************************
    // ***  Operation BOM Substitute file  ***
    // ***************************************
    std::string opSubBomFile = setOfParameters.getString("opSubBomFile");
    if (! opSubBomFile.empty())  {
      processOperationSubBomFile( theWitRun, theCal, opSubBomFile, mpHelper);
    }


    // *****************************
    // ***  BOM Substitute file  ***
    // *****************************
    std::string subBomFile = setOfParameters.getString("subBomFile");
    if (! subBomFile.empty())  {
      processStandardBomSubFile(theWitRun, theCal, subBomFile, mpHelper);
    }

    // *****************************
    // ***  Aggregation File   *****
    // *****************************
    std::string aggregationFile = setOfParameters.getString("aggregationFile");
    if (! aggregationFile.empty())  {
      processAggregationFile( theWitRun, theCal, aggregationFile, mpHelper);
    }

    // ******************************
    // ***  Demand Volume Files *****
    // ******************************
    std::string independentDemandFile = setOfParameters.getString("independentDemandFile");
    std::string geoSourceFile = setOfParameters.getString("geoSourceFile");
    std::string demandVolFile = setOfParameters.getString("demandVolFile");
    // SCE 6.1
    if ( useMultiAttributeDemand) {
      if (! demandVolFile.empty())
	processDemandFile( theWitRun, theCal, nPeriods, demandVolFile, mpHelper);    
    }
    // SCE 5.X
    else {
      if (! independentDemandFile.empty())
	processIndependentDemandFile( theWitRun, theCal, nPeriods, independentDemandFile, mpHelper);    
      if (! geoSourceFile.empty()) {
	processGeoSourceFile( theWitRun, geoSourceFile, theCal, mpHelper);    
      }
      if (! demandVolFile.empty())
	processDemandVolumeFile( theWitRun, nPeriods, demandVolFile, mpHelper);    
    }


    // FINISH_ME:
    // Finish processing of independent Demand Vols
    //  a) check for any negative demands
    //  b) if there are any negative demands, then we need to split parts 
    //     and do some neat trickery.
    preProcessNegativeDemands(theWitRun, mpHelper);



    // *************************************
    // *** Set up the features sets ********
    // *************************************
    std::string topLevelToBBdefFile = setOfParameters.getString("topLevelToBBdefFile");
    std::string topLevelToBBratioFile = setOfParameters.getString("topLevelToBBratioFile");
    if (! topLevelToBBdefFile.empty())   {
      if ( useMultiAttributeDemand) {
	processTopLevelToFeatureSetFile61( theWitRun, theCal, topLevelToBBdefFile, mpHelper);
      }
      else {
	processTopLevelToFeatureSetFile(theWitRun, theCal, topLevelToBBdefFile, mpHelper);
      }
    }
    if (! topLevelToBBratioFile.empty())   {
      if ( useMultiAttributeDemand) {
	processFeatureSetToOptionRatioFile61(theWitRun, theCal, topLevelToBBratioFile, mpHelper);
      }
      else {
	processFeatureSetToOptionRatioFile(theWitRun, theCal, topLevelToBBratioFile, mpHelper);
      }
    }



    // *************************************
    // *** Set up the optional features ****
    // *************************************
    std::string topLevelToOptionFile = setOfParameters.getString("topLevelToOptionFile");
    if (! topLevelToOptionFile.empty()) {
      if ( useMultiAttributeDemand) {
	process61TopLevelToOptionalFeatureFile(theWitRun, theCal, topLevelToOptionFile, mpHelper);
      }
      else {
	processTopLevelToOptionalFeatureFile(theWitRun, theCal, topLevelToOptionFile, mpHelper);
      }
      setOfParameters.setInt("nStandaloneFeatureArcs", sceScenario()->nStandaloneFeatureArcs());
    }


    // *************************************
    // *** Machine -> feature Offsets ****
    // *************************************
    std::string machineFeatureOffsetFile = setOfParameters.getString("machineFeatureOffsetFile");
    if (! machineFeatureOffsetFile.empty()) {
      // we don't use this file in SCE 6.1
      if ( ! useMultiAttributeDemand) {
	processMachineFeatureOffsetFile( theWitRun, theCal, machineFeatureOffsetFile, mpHelper);
      }
    }
    
    
    // *************************************
    // *** Machine -> feature SupplyVols (FEN5) ****
    // *************************************
    std::string machineFeatureSupplyFile = setOfParameters.getString("machineFeatureSupplyFile");
    if (! machineFeatureSupplyFile.empty()) {
      // we don't use this file in SCE 6.1
      if ( ! useMultiAttributeDemand) {
	processMachineFeatureSupplyFile(theWitRun, theCal, machineFeatureSupplyFile, mpHelper);
      }
    }
    
    
    // clean up the bop offsets on GPDs.  these were temporary placeholders for 
    // GPD transitTime and will be double counted if we don't remove them now.
    // this is because the transit time is also applied to gpd bom offsets
    removeGpdBopOffsets(theWitRun, mpHelper);

    

    // *****************************************************
    // *** Set up the Priorities and Objective Function ***
    // *****************************************************
    float periodicInterestRate = setOfParameters.getFloat("periodicInterestRate");
    float defaultOptionShipReward = setOfParameters.getFloat("defaultOptionShipReward");

    // Heuristic ==> PRIORITY based objective    
    if (sceObjectiveType == 1) {

      // SCE 6.1
      // Priority_Manager
      std::string priorityRulesFile = setOfParameters.getString("priorityRulesFile");
      if (! priorityRulesFile.empty())  {
	LgFrScePriorityMgr * priorityManager = new LgFrScePriorityMgr(sceScenario(), theWitRun, &mpHelper,
								      &(this->sceScenario()->flatFileMgr()), &theCal);
	priorityManager->setPriorities(priorityRulesFile);
	delete priorityManager;
      }

      // SCE 5
      std::string priorityFile = setOfParameters.getString("priorityFile");
      if (! priorityFile.empty())  {
        if ( lpModelHelper_ != 0 ) {
	  (*theErrFacil)("OptEngineAndPriorities");
        }
        processDemandObjectiveFile( theWitRun, nPeriods, priorityFile, mpHelper, periodicInterestRate);    
      }
      
      std::string indepPriorityFile = setOfParameters.getString("indepPriorityFile");
      if ((!independentDemandFile.empty()) && (!indepPriorityFile.empty()))  {
        processIndepDemandObjectiveFile( theWitRun, nPeriods, indepPriorityFile, mpHelper, periodicInterestRate);    
      }
    }


    // Optimization: revenue based objective
    if (sceObjectiveType == 2) {
	std::string revenueFile = setOfParameters.getString("revenueFile");
        if ((! revenueFile.empty()) && (! demandVolFile.empty()))
	processDemandObjectiveFile( theWitRun, nPeriods, revenueFile, mpHelper, periodicInterestRate);
        
        std::string optionRevFile = setOfParameters.getString("optionRevFile");
        if ((! optionRevFile.empty()) && (! demandVolFile.empty()))
          processOptionRevenueFile( theWitRun, theCal, optionRevFile, mpHelper, defaultOptionShipReward, periodicInterestRate);    


        // This one is the revenue per BB/feature option shipped
        std::string featRevFile = setOfParameters.getString("featRevFile");
        if ((! featRevFile.empty())  && (! demandVolFile.empty()))
          processOptionRevenueFile( theWitRun, theCal, featRevFile, mpHelper, defaultOptionShipReward, periodicInterestRate);    

	std::string indepRevenueFile = setOfParameters.getString("indepRevenueFile");
	if ((!independentDemandFile.empty()) && (!indepRevenueFile.empty()))  {
	  processIndepDemandObjectiveFile( theWitRun, nPeriods, indepRevenueFile, mpHelper, periodicInterestRate);    
	}
    }


    // Optimization: profit based objective
    if (sceObjectiveType == 3) {
	std::string profitFile = setOfParameters.getString("profitFile");
	processDemandObjectiveFile( theWitRun, nPeriods, profitFile, mpHelper, periodicInterestRate);

        std::string optionProfitFile = setOfParameters.getString("optionProfitFile");
        if (! optionProfitFile.empty())
          processOptionRevenueFile( theWitRun, theCal, optionProfitFile, mpHelper, defaultOptionShipReward, periodicInterestRate);    

        std::string featProfitFile = setOfParameters.getString("featProfitFile");
        if (! featProfitFile.empty())
          processOptionRevenueFile( theWitRun, theCal, featProfitFile, mpHelper, defaultOptionShipReward, periodicInterestRate);
    }

    // *************************************
    // *** PIP PriorityFiles            ****
    // *************************************
    std::string pipPriorityFile = setOfParameters.getString("pipPriorityFile");
    if (doPegging && (! pipPriorityFile.empty())) {
      processPipPriorityFile( theWitRun, nPeriods, pipPriorityFile, mpHelper);
    }
    std::string pipIndpPriorityFile = setOfParameters.getString("pipIndpPriorityFile");
    if (doPegging && (! pipIndpPriorityFile.empty())) {
      processPipIndpPriorityFile( theWitRun, nPeriods, pipIndpPriorityFile, mpHelper);
    }


    // *************************************
    // *** Set up the Minimum Demand Volumes
    // *************************************
    std::string minDemandVolFile = setOfParameters.getString("minDemandVolFile");
    if (! minDemandVolFile.empty())
	processMinDemandVolFile( theWitRun, nPeriods, minDemandVolFile, mpHelper);    

    // *************************************
    // *** Set up the Independet Minimum Demand Volumes
    // *************************************
    std::string minIndepDemandVolFile = setOfParameters.getString("minIndepDemandVolFile");
    if (! minIndepDemandVolFile.empty())
	processMinIndepDemandVolFile( theWitRun, nPeriods, minIndepDemandVolFile, mpHelper);    


    // *************************************
    // *** Set up the Perishability Values
    // *************************************
    std::string backlogYieldFile = setOfParameters.getString("backlogYieldFile");
    if (! backlogYieldFile.empty())
	processBacklogYieldFile( theWitRun, nPeriods, backlogYieldFile, mpHelper);    

    // *************************************
    // *** Set up the Demand Greedy Search Increments
    // *************************************
    std::string greedySearchIncFile = setOfParameters.getString("greedySearchIncFile");
    if (! greedySearchIncFile.empty())
	processGreedySearchIncFile( theWitRun, nPeriods, greedySearchIncFile, mpHelper);    

    // *************************************
    // *** Set up the Indp demand Greedy Search Increments
    // *************************************
    std::string indpGreedySearchIncFile = setOfParameters.getString("indpGreedySearchIncFile");
    if (! indpGreedySearchIncFile.empty())
	processIndpGreedySearchIncFile( theWitRun, nPeriods, indpGreedySearchIncFile, mpHelper);    

    // *************************************
    // *** Set up Build Ahead rules  *******
    // *************************************
    std::string buildAheadFile = setOfParameters.getString("buildAheadFile");
    if (! buildAheadFile.empty())
      processBuildAheadFile( theWitRun, nPeriods, theCal, buildAheadFile, mpHelper);  
    
    // *************************************
    // *** Set up Scrap Costs    *******
    // *************************************
    std::string scrapCostFile = setOfParameters.getString("scrapCostFile");
    if (! scrapCostFile.empty())
      processScrapCostFile( theWitRun, nPeriods, theCal, scrapCostFile, mpHelper);  

    // *************************************
    // *** Set up Stock Costs    *******
    // *************************************
    std::string stockCostFile = setOfParameters.getString("stockCostFile");
    if (! stockCostFile.empty())
      processStockCostFile( theWitRun, nPeriods, theCal, stockCostFile, mpHelper);  


    // *************************************
    // *** Set up Supply Vols  *************
    // *************************************
    std::string supplyFile = setOfParameters.getString("supplyFile");
    if (useDatedDemands)  {
      processSupplyFile61( theWitRun, theCal, nPeriods, supplyFile, mpHelper);  
    }
    else   {
      processSupplyFile( theWitRun, theCal, nPeriods, supplyFile, mpHelper);  
    }

    // *****************************************************
    // *** Set up Capacity Availability  Vols  *************
    // *****************************************************
    std::string capSupplyFile = setOfParameters.getString("capSupplyFile");
    if (! capSupplyFile.empty())    {
      if (useDatedDemands) 
	processCapacitySupplyFile61( theWitRun, theCal, nPeriods, capSupplyFile, mpHelper);    
      else 
	processCapacitySupplyFile( theWitRun, nPeriods, capSupplyFile, mpHelper);    
    }
    

    
      


    
    // *************************************
    //    "shut off" childless default
    //  operations for parts that have an
    //      an alternate operation
    // NOTE: method should preceed pcf file processing
    //       and multi-bop processing
    // *************************************
    std::cout << "Analyzing SCE model (default operations) ..." << std::endl;    
    scePreProcessDefaultOperations(theWitRun, nPeriods);    
    
    // *************************************
    // *** Read Parts Condition File *******
    // *************************************
    std::string partsConditionFile = setOfParameters.getString("partCondFile");
    if (! partsConditionFile.empty())    
      processPartsConditionFile( theWitRun, theCal, nPeriods, 
				 partsConditionFile, mpHelper);    

    // *************************************
    //  Set Priorities on  Special Feature Demands
    // *************************************
    if (setOfParameters.getInt("nStandaloneFeatureArcs") > 0) {
      std::cout << "Pre-processing feature priorities ..." << std::endl;    
      sceSetPrioritiesForSpecialFeatureDemands(theWitRun, mpHelper);    
    }


    if ( lpModelHelper_ == 0 ) {    
      // NOTE: these functions only need to be used for heuristic modes.
      //  with optimization mode, we DON'T want to use them.
      
      // *****************************************************
      // ***              Split Bops !!!   *******
      // *** Analyze model for parts with multiple Bops
      // *****************************************************
      if (! setOfParameters.getBool("disableSplitBop")) {
	std::cout << "Analyzing SCE model (multiple bop entries) ..." << std::endl;
	scePreProcessMultiBopEntries ( theWitRun, useMultiRouteImplosion );
      }


      // *****************************************************
      // *** Add Interplant Jumper Arcs  ****
      // *****************************************************
      // Multi-sourcing a part from a variety of supplier
      // locations is modeled in SCE as the part being produced 
      // from a variety of "interplant" operations. 
      // 
      // The current heuristic doesn't excercise the alternative
      // ways to build a part when multiple operations exist.
      // To slightly mitigate this drawback until the heuristic
      // is improved, this method adds "jumper arcs" (aka substitutes)
      // from the supplier parts at alternative interplant locations to
      // the supplier part at the preferred interplant location.
      if (! setOfParameters.getBool("disableInterplantJumperArcs")) {
	std::cout << "Analyzing SCE model (multiple interplant entries) ..." << std::endl;
	scePreProcessMultiInterPlantEntries ( theWitRun, mpHelper );
      }
    }

    // ***************************************************
    // *** SCE PreProcess Via MRP
    // ***************************************************
    // Do some pre-processing of the model to set the:
    // optionRatioSupply supplyVols and the
    // demandUpperBound supplyVols

#ifdef BUILDING_BLOCKS_VIA_MULTI_BOPS
    std::cout << "Pre-Processing SCE model (via  Multi-route  MRP) ..." << std::endl;
    scePreProcessViaMrpViaMultiRoute(theWitRun, nPeriods, theCal, mpHelper);
#else
    std::cout << "Pre-Processing SCE model (via mrp) ..." << std::endl;
    scePreProcessViaMrp(theWitRun, nPeriods, theCal, mpHelper);
#endif

    if (groupSharingViaExecPenalties_) {    
	// this should be done in general case of penalized Exectution
	std::cout << "Checking for illegal sub netAllowed conditions ..." << std::endl;
	checkAndFixIllegalSubNetAllowedCases(theWitRun, mpHelper);
    }

    

    // ***************************************************
    // *** Do some extra Scrubbing PCF and Param files ***
    // ***************************************************
    LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
    if (myScrubber -> scrubMode())  {
      std::string outputFilePrefix = setOfParameters.getString("outputFilePrefix");
      //      if (sce::find(outputFilePrefix, "/") != sce::badIndex) {
      if (sce::find(outputFilePrefix, "/") != std::string::npos) {
	(*theErrFacil)("QualifiedOutputFilePrefix",MclArgList() 
		       << outputFilePrefix);
      }
      // Print out a scrubbed ParamFile
      std::string parameterFileName = setOfParameters.getString("parameterFileName");
      std::string scrubParamFileName = myScrubber->scrubDir() + parameterFileName;
      if (sce::find(parameterFileName, "/") != std::string::npos) {
	(*theErrFacil)("QualifiedInputFileName",MclArgList() 
		       << parameterFileName);
      }
      setOfParameters.printScrubbedParamFile(scrubParamFileName);

      if (! partsConditionFile.empty())    
	// Print out a scrubbed pcf file
	scrubPartsConditionFile(theWitRun, theCal, nPeriods, 
				partsConditionFile, mpHelper);    

      // Print out a supply availability file
      bool aggregateSupplyInScrub = setOfParameters.getBool("aggregateSupplyInScrub");
      if (aggregateSupplyInScrub)
	scrubSupplyAvailabilityFile(theWitRun, theCal, nPeriods, 
				    supplyFile, mpHelper);    


      // Print out a scrubbed demand Volume file
      if (! demandVolFile.empty())
	scrubDemandVolumeFile(theWitRun, theCal, nPeriods, 
			      demandVolFile, mpHelper);    

      // Print out a scrubbed demand Volume file
      if (! minDemandVolFile.empty())
         scrubMinDemandVolumeFile(theWitRun, theCal, nPeriods, 
			    minDemandVolFile, mpHelper);    
    }


    // *************************************
    //  Delete Unnecessary Demand App Data
    // *************************************
    // FINISH_ME:  We should probably not do this if we migrate to 
    //             incHeurAlloc in a big way
    // In order to speed up the rollover heuristic, we delete any
    // demandApp data's that have the backlogYield set to their 
    // default.

    // deleteUnnecessaryDemandAppData(theWitRun, nPeriods, mpHelper);    


    // *************************************
    //  Shut off all Lot-Sizing (if they want to) 
    // *************************************
    if (setOfParameters.getBool("shutOffAllLotSizing")) {
      std::cout << "Shutting off all Lot-sizing ..." << std::endl;    
      shutOffAllLotSizing(theWitRun, nPeriods);    
    }



    // *************************************
    // If SCRUB MODE only, then stop here
    // *************************************
    if (myScrubber->scrubOnly())
      return;
	

    // ******************************************
    // Set up Part Index System via part app data 
    // ******************************************
    // This allows a part to know its index against witGetParts()
    // BTW, Don't even THINK about adding new parts after this!!!
    mpHelper.setAllWitPartIndices(theWitRun);

    // ******************************************
    // Compute the  Demand Drivers File
    // ******************************************
    // WE only build this thing if we are going to print the demand
    // drivers file.  In the future, we could leave it around for other 
    // diagnosis or whatever.
    // NOTE: this needs to happen before hte preProcessViaMrp because
    // it needs to access the maxWithout of standalone features.  The
    // mwo is temporarily stored in supplyVol of the nullSub part AND
    // that supplyVol get overridden in preProcessViaMrp.  This should
    // eventually be cleaned up with a better use of AppData.
    if (setOfParameters.getBool("printDemandDriversFile"))  {
      std::cout << "Computing Demand Drivers  ..." << std::endl;                      
      // demandDrivers will report subs as a default.  To shut it off
      // we need to set netAllowed to False for all subs.  This may
      // also screw up indexing so we reset all wit part indices 
      // after printing, WE MUST turn subs back on (netAllowed to true)
      // UPDATE: we do need to worry about the parameter .... globalSubNetAllowed
      // see the setSubsNetAllowed method ...
      if (! setOfParameters.getBool("demandDriversRespectsSubs")) {
	//	this->setSubsNetAllowed(false, mpHelper, theWitRun);
	mpHelper.setAllWitPartIndices(theWitRun);
      }
      myDemandDriver = new LgFrSceDemandDriver(theWitRun, &mpHelper);
      
      // compute and print as one
      std::cout << "Writing the Demand Drivers file  ..." << std::endl;                      
      std::string demandDriversFileName = setOfParameters.getString("demandDriversFileName");
      myDemandDriver->computeAndPrint(demandDriversFileName, theErrFacil);
      if (! setOfParameters.getBool("demandDriversRespectsSubs")) {
	//    	this->setSubsNetAllowed(true, mpHelper, theWitRun);
	mpHelper.setAllWitPartIndices(theWitRun);
      }
      // FINISH_ME: Does this thing need to be persistent??
      delete myDemandDriver;
    }


    // *************************************
    // Do a witPreprocess before the echo file
    // *************************************
    std::cout << "Pre-Processing for wit  ..." << std::endl;          
    witPreprocess(theWitRun);


    // *************************************
    // Print out an Impact Period Report
    // *************************************
    if (setOfParameters.getBool("printImpactPeriodDiagnosticFile"))  {
      std::cout << "Writing the Impact Period Diagnostic File (Implosion view) ..." << std::endl;                      
      std::string impactPdFile = setOfParameters.getString("impactPeriodDiagFileName");
      printImpactPeriodFile(theWitRun, theCal, nPeriods, 
			    impactPdFile, mpHelper);    
    }




}
// ===============================================================
// ========== End of () ==========================
// ===============================================================







// Turn ON or OFF the subNetAllowed flag of every REAL and DUMMY sub arc or every 
// REAL and DUMMY operation
// NOTE:  
// FINISH_ME: If we ever introduce sub arcs that need to set NetAllowed=false, then this will
// screw it up and we need to use appData!!  
void
LgFrInitialProblemForSce::setSubsNetAllowed(bool subNetAllowed, 
					    LgFrMultiPlantHelper & mpHelper,
					    WitRun * const theWitRun  )
{
  int nOperations;
  char ** opList;

  bool netAllowed = subNetAllowed;
  bool expAllowed = subNetAllowed;
  bool globalSubNetAllowed = this->sceScenario()->setOfParameters().getBool("globalSubNetAllowed");
  // OK, make sure that if globalSubNetAllowed is FALSE, that you 
  // set NetAllowed to FALSE after DemandDrivers is finished.
  // we may need an app data to clean this up

  if (subNetAllowed && !globalSubNetAllowed) {
    netAllowed = FALSE;
  }
	  


  witGetOperations(theWitRun, &nOperations, &opList);
  int o = 0; // Pulled out of the for below by RW2STL
  for (o=0; o<nOperations; o++) {
    // only deal with operations that have a corresponding REAL part
    if ( ! mpHelper.isPartNormal(theWitRun, opList[o])) {
      //      witFree(opList[o]);
      continue;
    }
    int nBoms;
    witGetOperationNBomEntries(theWitRun, opList[o], &nBoms);
    int b = 0; // Pulled out of the for below by RW2STL
    for (b=0; b<nBoms; b++) {
      int nSubBoms;
      witGetBomEntryNSubsBomEntries(theWitRun, opList[o], b, &nSubBoms);
      int s = 0; // Pulled out of the for below by RW2STL
      for (s=0; s<nSubBoms; s++) {
	char * sub;
	witGetSubsBomEntryConsumedPart(theWitRun, opList[o], b, s, &sub);
	if ( mpHelper.isPartNormal(theWitRun, sub)) {

	  witSetSubsBomEntryNetAllowed(theWitRun, opList[o], b, s, netAllowed);
	  //	  witSetSubsBomEntryExpAllowed(theWitRun, opList[o], b, s, expAllowed);
	}
	witFree(sub);
      }
    }

  }

  // temporary fix of unpreprocessing bug in wit
  float * yieldRate;
  witGetOperationYieldRate(theWitRun, opList[0], &yieldRate);
  witSetOperationYieldRate(theWitRun, opList[0], yieldRate);
  witFree(yieldRate);
			   
  for ( o=0; o<nOperations; o++) 
    witFree(opList[o]);
  witFree(opList);
}


// --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::openInputFileAndScrubOutputFile(
    const std::string & fileName,
     std::string fileDescription,
    std::ifstream & inFile,
    std::ofstream & outFile,
    LgFrSceScrubber * myScrubber,
    MclFacility * theErrFacil)
{

  if (myScrubber -> scrubMode())     {
    if (sce::find(fileName, "/") != std::string::npos) {
      (*theErrFacil)("QualifiedInputFileName",MclArgList() 
		     << fileName);
    }
    
    std::string file = myScrubber -> scrubDir() + fileName;
    outFile.open(file.c_str());
    if (!outFile.good())
      {
	(*theErrFacil)("ScrubOutPutFileNameBad",MclArgList() 
		     << file);
      }
  }

  inFile.open(fileName.c_str() );//,ios::nocreate);
  if (! inFile)  {
    (*theErrFacil)("SevereCannotOpenFile",MclArgList() 
		   << fileDescription 
		   << fileName 
		   << "");
  }

}


// --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processPartDefinitionFile(
    
    WitRun * const theWitRun,   
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

    std::string mfgPartName;
    std::string pdf_PartName;
    std::string pdf;

    LgFrSceFlatFileMgr & flatFileManager =
      this->sceScenario()->flatFileMgr();

    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    std::string dataLine;
    int result;


    std::ifstream inFile;
    std::ofstream outFile;
    LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();

    this->openInputFileAndScrubOutputFile(fileName,
					  "Part Definition",
					  inFile,
					  outFile,
					  myScrubber,
					  theErrFacil);

    int nPeriods;
    witGetNPeriods(theWitRun, &nPeriods);
    bool buildNstn = this->sceScenario()->setOfParameters().getBool("useLocalBuildAhead");
    int  buildAheadLimit = this->sceScenario()->setOfParameters().getInt("buildAheadLimit");
    int * buildAheadUB = new int[nPeriods];
    int t;
    for (t=0; t<nPeriods; t++) 
      buildAheadUB[t] = buildAheadLimit;
      
    std::cout << "Processing " << fileName << ":\t" << std::flush;
    long recCount = 0;
    long badCount = 0;
    long commentCount = 0;
    long lineNo = 0;
    while(inFile.peek() != EOF) {
      
      std::getline( inFile, dataLine );
      lineNo++;
      
      //  skip blanks or comments *
      dataLine = sce::strip( dataLine );
      if ((dataLine.empty()) || (dataLine[0] == '*')) {
	commentCount++;
	continue;
      }
      
      // check if the line contains any illegal characters
      if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	(*theErrFacil)("IllegalCharacter",MclArgList() 
		       << fileName 
		       << (int)lineNo 
		       << dataLine);
      }
      
      SCETokenizer next(dataLine);
      
      // get the partname
      mfgPartName = flatFileManager.nextQuotedString( next, result, fileName, dataLine, 
						      lineNo, PURELY_OPTIONAL);
      pdf = flatFileManager.nextQuotedString( next, result, fileName, dataLine, lineNo, MANDATORY);
	
      // ===================================
      // == Only Regular parts get read here
      // ===================================
      result = sceAddNormalPart(theWitRun, 
				mfgPartName, 
				pdf, 
				buildNstn,
				buildAheadUB,
				mpHelper, 
				fileName, 
				dataLine,
				lineNo);
      
      if (! result)
	{
	  recCount ++;
	  if (myScrubber -> scrubMode() && outFile.good())
	    {
	      outFile << dataLine << std::endl;
	    }
	}
      
      else
	badCount++;
    }
    
    std::cout << recCount << " records processed." << std::endl;
    inFile.close();
    outFile.close();
    
    // if there is no data in this file .. ie: no good records read .. then error
    if (recCount == 0)
      (*theErrFacil)("DatalessMandatoryInputFile",MclArgList() << "part definition");
    
    flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
    delete [] buildAheadUB;
}



// --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processCapacityDefinitionFile(
    
    WitRun * const theWitRun,   
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

    std::string capacityName;
    std::string pdf_capacityName;
    std::string pdf;

    LgFrSceFlatFileMgr & flatFileManager =
    this->sceScenario()->flatFileMgr();
    
    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    std::string dataLine;
    int result;

    std::ifstream inFile;    
    std::ofstream outFile;
    LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();

    this->openInputFileAndScrubOutputFile(fileName,
					  "Capacity Definition",
					  inFile,
					  outFile,
					  myScrubber,
					  theErrFacil);

    std::cout << "Processing " << fileName << ":\t" << std::flush;
    long recCount = 0;
    long badCount = 0;
    long commentCount = 0;
    long lineNo = 0;
    while(inFile.peek() != EOF) {

    std::getline( inFile, dataLine );
	lineNo++;

//  skip blanks or comments *
	dataLine = sce::strip( dataLine );
	if ((dataLine.empty()) || (dataLine[0] == '*')) {
	    commentCount++;
	    continue;
	}
// check the line for illegal characters        
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
        }

        
	SCETokenizer next(dataLine);

// get the capacity name

        capacityName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
                                        lineNo, PURELY_OPTIONAL);
        
        pdf = flatFileManager.nextQuotedString(next, result, fileName,
                               dataLine, lineNo, OPTIONAL_WITH_MESSAGE);


        if (result == -1) {
	  badCount++;
          continue;
        }


	  
	int   mandatoryCapacityFlag = 1;
	result = flatFileManager.nextInt(mandatoryCapacityFlag, next, fileName, dataLine, lineNo, false, 0, 1);


// =============================================
// == Now add the capacity part
// == Only Regular capacity parts get read here
// =============================================
        result = sceAddNormalCapacityPart(theWitRun, 
                                          capacityName, 
                                          pdf, 
					  mandatoryCapacityFlag,
                                          mpHelper, 
                                          fileName, 
                                          dataLine,
                                          lineNo);
	
	if (! result)
          {
	    recCount ++;
            if (myScrubber -> scrubMode() && outFile.good())
              {
                outFile << dataLine << std::endl;
              }
          }
	else
	    badCount++;
    }

    std::cout << recCount << " records processed." << std::endl;
    inFile.close();
    outFile.close();
    
    flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

// --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processSourceDefFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

    std::string mfgPartName;
    std::string pdf_PartName;
    std::string pdf;
    std::string sourceCode;

    LgFrSceFlatFileMgr & flatFileManager =
      this->sceScenario()->flatFileMgr();

    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    std::string dataLine;
    int result;


    std::ifstream inFile;
    std::ofstream outFile;
    LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();

    this->openInputFileAndScrubOutputFile(fileName,
					  "Source Def",
					  inFile,
					  outFile,
					  myScrubber,
					  theErrFacil);

    int nPeriods;
    witGetNPeriods(theWitRun, &nPeriods);

    std::cout << "Processing " << fileName << ":\t" << std::flush;
    long recCount = 0;
    long badCount = 0;
    long commentCount = 0;
    long lineNo = 0;
    while(inFile.peek() != EOF) {

	std::getline( inFile, dataLine );
	lineNo++;

	//  skip blanks or comments *
	dataLine = sce::strip( dataLine );
	if ((dataLine.empty()) || (dataLine[0] == '*')) {
	    commentCount++;
	    continue;
	}

	// check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() 
			 << fileName 
			 << (int)lineNo 
			 << dataLine);
        }
        
	SCETokenizer next(dataLine);

	mfgPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, PURELY_OPTIONAL);
	pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);
	
	// FINISH_50

	// The rest of the fields are optional.  
	// Set the defaults, and parse the rest of the line.  
	float pref_cost = 0.0;
	int   early = 0;
	int   late = nPeriods-1;
	
	sourceCode = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);


	// usage rate        
	result = flatFileManager.nextFloat(pref_cost, next, fileName, dataLine, lineNo, false, -FLT_MAX);



	
	// effectivity Dates
	if (result >= 0)
	  result = flatFileManager.nextStartEndDates(early, late, next, fileName, lineNo, dataLine, theCal);
	if (result == 1) {
	  badCount++;
	  continue;
	}
	if ((early == nPeriods) || (late < 0)) {
	  (*theErrFacil)("OutOfRange",MclArgList() 
			 << "Effectivity dates" 
			 << fileName 
			 << (int) lineNo 
			 << dataLine 
			 << "SourceDef entry ignored.");
	  badCount++;
	  continue;
	}

	result = sceSetSourcingStructure(theWitRun, 
					 mpHelper,
					 mfgPartName, 
					 pdf, 
					 sourceCode,
					 pref_cost, 
					 early, 
					 late,
					 nPeriods,
					 fileName,
					 dataLine,
					 lineNo);


	
	if (! result)
          {
	    recCount ++;
            if (myScrubber -> scrubMode() && outFile.good())
              {
                outFile << dataLine << std::endl;
              }
          }
          
	else
	    badCount++;
    }

    std::cout << recCount << " records processed." << std::endl;
    inFile.close();
    outFile.close();

    
    flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}







// ---------------------------------------------------------------
void 
LgFrInitialProblemForSce::processStandardBomFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string mfgParentPartName;
 std::string mfgChildPartName;
 std::string pdf;
 LgFrSceFlatFileMgr & flatFileManager =
   this->sceScenario()->flatFileMgr();
 
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::string dataLine;

 int nPeriods = theCal.nPeriods();
 int result;


 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "BOM",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 while(inFile.peek() != EOF) {

     // read the next BOM line
    std::getline( inFile, dataLine );
    lineNo++;

    //  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
        commentCount++;
	continue;
    }

    // check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
        }

    
    SCETokenizer next(dataLine);

    // get the mfg Parent PartName 
    mfgParentPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					 lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
        badCount++;
	continue;
    }

    // get the mfg Child PartName 
    mfgChildPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					lineNo, MANDATORY);

    // get the PDF
    pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, MANDATORY);

    // The rest of the fields are optional.  
    // Set the defaults, and parse the rest of the line.  
    float ur = 1.0;
    float ut = 0.0;
    int   fallout = 0;
    int   early = 0;
    int   late = nPeriods-1;
    int   mec = 0;
    int   phantomFlag = 0;
    int   removeBillFlag = 0;
    int   propRtgFlag = 0;
    float   propRtgShare = 1.0;

    // usage rate        
    result = flatFileManager.nextFloat(ur, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // usage time
    if (result >= 0)
	result = flatFileManager.nextFloat(ut, next, fileName, dataLine, lineNo, false, -FLT_MAX);


    // effectivity Dates
    if (result >= 0)
	result = flatFileManager.nextStartEndDates(early, late, next, fileName, lineNo, dataLine, theCal);
    if (result == 1) {
        badCount++;
	continue;
    }
    if ((early == nPeriods) || (late < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() << "Effectivity dates" << fileName << (int) lineNo << dataLine << "BOM entry ignored.");
	badCount++;
	continue;
    }

    // fallout
    if (result >= 0)
      result = flatFileManager.nextInt(fallout, next, fileName, dataLine, lineNo, false, 0, 100);

    // mandatory EC
    if (result >= 0)
	result = flatFileManager.nextInt(mec, next, fileName, dataLine, lineNo, false, 0, 1);

    // phantom Flag
    if (result >= 0)
	result = flatFileManager.nextInt(phantomFlag, next, fileName, dataLine, lineNo, false, 0, 1);

    // remove Flag
    if (result >= 0)
	result = flatFileManager.nextInt(removeBillFlag, next, fileName, dataLine, lineNo, 
			 false, 0, 1);

    // propRtg Flag
    if (result >= 0)
	result = flatFileManager.nextInt(propRtgFlag, next, fileName, dataLine, lineNo, 
			 false, 0, 1);

    // propRtg Share
    if (result >= 0)
	result = flatFileManager.nextFloat(propRtgShare, next, fileName, dataLine, lineNo, 
			 false, 0);


#ifndef  VARIABLE_PERIODS
    // scale the  usage time
    ut = ut / theCal.workUnits(early);
#endif

    result = sceAddNormalBomStructure(theWitRun, 
                                      mpHelper,
				      theCal,
                                      mfgParentPartName, 
                                      pdf, 
                                      mfgChildPartName, 
                                      pdf, 
                                      ur,
                                      ut, 
                                      early, 
                                      late,
                                      fallout,
                                      mec,
                                      phantomFlag,
                                      removeBillFlag,
				      propRtgFlag,
				      propRtgShare,
				      nPeriods,
                                      fileName,
                                      dataLine,
                                      lineNo);
    if (result == 0)
      {
	    recCount ++;
            if (myScrubber -> scrubMode() && outFile.good())
              {
                outFile << dataLine << std::endl;
              }
      }
    else
      badCount++;
 }
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 // if no data in input file, then error
 if (recCount == 0)
   (*theErrFacil)("DatalessMandatoryInputFile",MclArgList() << "BOM file");

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

#ifdef MULTI_SITE_BOM
// ---------------------------------------------------------------
void 
LgFrInitialProblemForSce::processMultiSiteBomFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string mfgParentPartName;
 std::string mfgChildPartName;
 std::string ParentPdf;
 std::string ChildPdf;
 LgFrSceFlatFileMgr & flatFileManager =
   this->sceScenario()->flatFileMgr();
 
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::string dataLine;

 int nPeriods = theCal.nPeriods();
 int result;


 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "BOM",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 while(inFile.peek() != EOF) {

     // read the next BOM line
    std::getline( inFile, dataLine );
    lineNo++;

    //  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
        commentCount++;
	continue;
    }

    // check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
        }

    
    SCETokenizer next(dataLine);

    // get the mfg Parent PartName 
    mfgParentPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					 lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
        badCount++;
	continue;
    }

    // get the parent PDF
    ParentPdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, MANDATORY);




    // get the mfg Child PartName 
    mfgChildPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					lineNo, MANDATORY);

    // get the ChildPdf
    ChildPdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, MANDATORY);

    // The rest of the fields are optional.  
    // Set the defaults, and parse the rest of the line.  
    float ur = 1.0;
    float ut = 0.0;
    int   fallout = 0;
    int   early = 0;
    int   late = nPeriods-1;
    int   mec = 0;
    int   phantomFlag = 0;
    int   removeBillFlag = 0;
    int   propRtgFlag = 0;
    float   propRtgShare = 1.0;


    // usage rate        
    result = flatFileManager.nextFloat(ur, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // usage time
    if (result >= 0)
	result = flatFileManager.nextFloat(ut, next, fileName, dataLine, lineNo, false, -FLT_MAX);


    // effectivity Dates
    if (result >= 0)
	result = flatFileManager.nextStartEndDates(early, late, next, fileName, lineNo, dataLine, theCal);
    if (result == 1) {
        badCount++;
	continue;
    }
    if ((early == nPeriods) || (late < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() 
		     << "Effectivity dates" 
		     << fileName 
		     << (int) lineNo 
		     << dataLine 
		     << "BOM entry ignored.");
	badCount++;
	continue;
    }

    // fallout
    if (result >= 0)
      result = flatFileManager.nextInt(fallout, next, fileName, dataLine, lineNo, false, 0, 100);

    // mandatory EC
    if (result >= 0)
	result = flatFileManager.nextInt(mec, next, fileName, dataLine, lineNo, false, 0, 1);

    // phantom Flag
    if (result >= 0)
	result = flatFileManager.nextInt(phantomFlag, next, fileName, dataLine, lineNo, false, 0, 1);

    // remove Flag
    if (result >= 0)
	result = flatFileManager.nextInt(removeBillFlag, next, fileName, dataLine, lineNo, 
			 false, 0, 1);

    // propRtg Flag
    if (result >= 0)
	result = flatFileManager.nextInt(propRtgFlag, next, fileName, dataLine, lineNo, 
			 false, 0, 1);

    // propRtg Share
    if (result >= 0)
	result = flatFileManager.nextFloat(propRtgShare, next, fileName, dataLine, lineNo, 
			 false, 0);


#ifndef  VARIABLE_PERIODS
    // scale the  usage time
    ut = ut / theCal.workUnits(early);
#endif

    result = sceAddNormalBomStructure(theWitRun, 
                                      mpHelper,
				      theCal,
                                      mfgParentPartName, 
                                      ParentPdf, 
                                      mfgChildPartName, 
                                      ChildPdf, 
                                      ur,
                                      ut, 
                                      early, 
                                      late,
                                      fallout,
                                      mec,
                                      phantomFlag,
                                      removeBillFlag,
				      propRtgFlag,
				      propRtgShare,
				      nPeriods,
                                      fileName,
                                      dataLine,
                                      lineNo);
    if (result == 0)
      {
	    recCount ++;
            if (myScrubber -> scrubMode() && outFile.good())
              {
                outFile << dataLine << std::endl;
              }
      }
    else
      badCount++;
 }
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 // if no data in input file, then error
 if (recCount == 0)
   (*theErrFacil)("DatalessMandatoryInputFile",MclArgList() << "BOM file");

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}


#endif



// ---------------------------------------------------------------
void 
LgFrInitialProblemForSce::processBillOfCapacitiesFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string mfgParentPartName;
 std::string mfgChildPartName;
 std::string pdf;

 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager =
   this->sceScenario()->flatFileMgr();

 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 int nPeriods = theCal.nPeriods();
 int result;


 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "BOC",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 LgFrTimeVecFloat zeroVec(nPeriods, 0.0);
 bool capacityOffsetToZero = this->sceScenario()->setOfParameters().getBool("capacityOffsetToZero"); 
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 while(inFile.peek() != EOF) {

     // read the next BOM line
    std::getline( inFile, dataLine );
    lineNo++;

    //  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
        commentCount++;
	continue;
    }

    // check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
        }


    SCETokenizer next(dataLine);

    // get the mfg Parent PartName 
    mfgParentPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					 lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
        badCount++;
	continue;
    }

    // get the mfg Child PartName 
    mfgChildPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					lineNo, MANDATORY);

    // get the PDF
    pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, MANDATORY);

    // The rest of the fields are optional.  
    // Set the defaults, and parse the rest of the line.  
    float ur = 1.0;
    float ut = 0.0;
    int   fallout = 0;
    int   early = 0;
    int   late = nPeriods-1;

    // usage rate        
    result = flatFileManager.nextFloat(ur, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // usage time
    if (result >= 0)
	result = flatFileManager.nextFloat(ut, next, fileName, dataLine, lineNo, false, -FLT_MAX);


    // effectivity Dates
    if (result >= 0)
	flatFileManager.nextStartEndDates(early, late, next, fileName, lineNo, dataLine, theCal);
    if (result == 1) {
        badCount++;
	continue;
    }
    if ((early == nPeriods) || (late < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() << "Effectivity dates" << fileName << (int) lineNo << dataLine << "");
	badCount++;
	continue;
    }

#ifndef  VARIABLE_PERIODS
    // scale the  usage time
    ut = ut / theCal.workUnits(early);
#endif


    // FINISH: VARIABLE_PERIODS itize this
    result = sceAddNormalBocStructure(theWitRun, 
                                      mpHelper,
				      theCal,
                                      mfgParentPartName, 
                                      pdf, 
                                      mfgChildPartName, 
                                      pdf, 
                                      ur,
                                      ut, 
                                      early, 
                                      late,
				      capacityOffsetToZero,
				      zeroVec,
                                      fileName,
                                      dataLine,
                                      lineNo);
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
        outFile << dataLine << std::endl;
      }
    }
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

// ---------------------------------------------------------------
void 
LgFrInitialProblemForSce::processOperationBomFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string consumedPartName;
 std::string operationName;
 std::string pdf;

 std::string dataLine;
 
 LgFrSceFlatFileMgr & flatFileManager =
   this->sceScenario()->flatFileMgr();

 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 int nPeriods = theCal.nPeriods();
 int result;

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "Operations BOM",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 while(inFile.peek() != EOF) {

     // read the next BOM line
    std::getline( inFile, dataLine );
    lineNo++;

    //  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
        commentCount++;
	continue;
    }

    // check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo <<  dataLine);
        }


    SCETokenizer next(dataLine);

    // get the operation name 
    operationName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					 lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
        badCount++;
	continue;
    }

    // get the consumed part name 
    consumedPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					lineNo, MANDATORY);

    // get the PDF
    pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, MANDATORY);

    // The rest of the fields are optional.  
    // Set the defaults, and parse the rest of the line.  
    float ur = 1.0;
    float offset = 0.0;
    int   fallout = 0;
    int   mec = 0;
    int   early = 0;
    int   late = nPeriods-1;

    // usage rate        
    result = flatFileManager.nextFloat(ur, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // offset
    if (result >= 0)
	result = flatFileManager.nextFloat(offset, next, fileName, dataLine, lineNo, false, -FLT_MAX);


    // effectivity Dates
    if (result >= 0)
	flatFileManager.nextStartEndDates(early, late, next, fileName, lineNo, dataLine, theCal);
    if (result == 1) {
        badCount++;
	continue;
    }
    if ((early == nPeriods) || (late < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() << "Effectivity dates" << fileName << (int) lineNo << dataLine << "Operation BOM entry ignored.");
	badCount++;
	continue;
    }

    // fallout
    if (result >= 0)
	result = flatFileManager.nextInt(fallout, next, fileName, dataLine, lineNo, false, 0, 100);

    // mandatory EC
    if (result >= 0)
	result = flatFileManager.nextInt(mec, next, fileName, dataLine, lineNo, false, 0, 1);


// scale the  usage time
    offset = offset / theCal.workUnits(early);

    result = sceAddNormalOperationBomStructure(theWitRun, 
					       mpHelper,
					       operationName, 
					       pdf, 
					       consumedPartName, 
					       pdf, 
					       ur,
					       offset, 
					       early, 
					       late,
					       fallout,
					       mec,
					       fileName,
					       dataLine,
					       lineNo);
    if (result == 0)
      {
        recCount ++;
        if (myScrubber -> scrubMode() && outFile.good())
          {
            outFile << dataLine << std::endl;
          }
      }
    else
      badCount++;
    
 }
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}




// ---------------------------------------------------------------
void 
LgFrInitialProblemForSce::processOperationBopFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string producedPartName;
 std::string operationName;
 std::string pdf;

 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager =
   this->sceScenario()->flatFileMgr(); 

 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 int nPeriods = theCal.nPeriods();
 int result;
 
 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "Operation BOP",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 while(inFile.peek() != EOF) {

     // read the next BOP line
    std::getline( inFile, dataLine );
    lineNo++;

    //  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
        commentCount++;
	continue;
    }

    // check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
        }


    SCETokenizer next(dataLine);

    // get the produced part name 
    producedPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					 lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
        badCount++;
	continue;
    }

    // get the Operation Name 
    operationName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					lineNo, MANDATORY);

    // get the PDF
    pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, MANDATORY);

    // The rest of the fields are optional.  
    // Set the defaults, and parse the rest of the line.  
    float prodRate = 1.0;
    float offset = 0.0;
    int   early = 0;
    int   late = nPeriods-1;
    int   isObjectInEffectForOptExplosion = 1; // default = "in effect"
    float preference = 0.0;  // direct setting on expAversion
    int   expAllowed = 1;   // direct setting on expAllowed
    float pipShare = 1.0;    // float converted to floatVec for pipShare
    
    // Use the right model helper to get the preference info.
    // (seems like we should be able to do this with out testing every time...)
    // FINISH_ME: lp model
    preference = heurModelHelper_->defaultExplicitBopPreference();
    // if ( lpModelHelper_ != 0 )
    //  preference = lpModelHelper_->defaultExplicitBopCost(); ??
    
    // production  rate        
    result = flatFileManager.nextFloat(prodRate, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // offset
    if (result >= 0) 
      result = flatFileManager.nextFloat(offset, next, fileName, dataLine, lineNo, false, -FLT_MAX);
    
    // effectivity Dates
    if (result >= 0)
	result = flatFileManager.nextStartEndDates(early, late, next, fileName, lineNo, dataLine, theCal);
    if ((early == nPeriods) || (late < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() << "Effectivity dates" << fileName << (int)lineNo << dataLine << "BOP entry ignored.");
	badCount++;
	continue;
    }

    // preference/cost
    if (result >= 0)
      result = flatFileManager.nextFloat(preference, next, fileName, dataLine, lineNo, false, -FLT_MAX); 


    // isObjectInEffectForOptExplosion flag
    if (result >= 0)
      result = flatFileManager.nextInt(isObjectInEffectForOptExplosion, next, fileName, dataLine, lineNo, false, 0, 1); 

    // expAllowed flag
    if (result >= 0)
      result = flatFileManager.nextInt(expAllowed, next, fileName, dataLine, lineNo, false, 0, 1); 

    // pipShare
    if (result >= 0)
      result = flatFileManager.nextFloat(pipShare, next, fileName, dataLine, lineNo, false, -FLT_MAX); 



    // scale the scalar offset and
    // set the offsetTimeVec based on effectivity dates
    offset = offset / theCal.workUnits(early);

    result = sceAddNormalBopStructure(theWitRun, 
                                      mpHelper,
                                      producedPartName, 
                                      pdf, 
                                      operationName, 
                                      pdf, 
                                      prodRate,
                                      offset, 
                                      early, 
                                      late,
                                      preference,
				      pipShare,
                                      isObjectInEffectForOptExplosion,
				      expAllowed,
                                      fileName,
                                      dataLine,
                                      lineNo);
    
    if (result == 0)
      {
        recCount ++;
        if (myScrubber -> scrubMode() && outFile.good())
          {
            outFile << dataLine << std::endl;
          }
      }
      else
      badCount++;
 }
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

//

void 
LgFrInitialProblemForSce::processAggregationFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{
  // Aggregation:  The aggregation file is read and both the parent and child
  // parts are created on-the-fly if they are unknown to SCE!!!  Why is this?

 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager =
   this->sceScenario()->flatFileMgr(); 

 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 int nPeriods = theCal.nPeriods();
 int result;

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "Aggregation",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 while(inFile.peek() != EOF) {

     // read the next BOM line
    std::getline( inFile, dataLine );
    lineNo++;

    //  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
        commentCount++;
	continue;
    }

    // check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
        }

    
    SCETokenizer next(dataLine);

    // get the mfg part
    std::string mfgPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
                                         lineNo, PURELY_OPTIONAL);
    if (result == -1) {
        badCount++;
	continue;
    }

    // get the PDF
    std::string mfgPdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
                                        lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
        badCount++;
	continue;
    }

    // get the aggregate part name
    std::string aggregatePart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
                                               lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
      badCount++;
      continue;
    }


    // get the aggregate PDF
    std::string aggregatePdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
                                              lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
        badCount++;
	continue;
    }
    

    // The rest of the fields are optional.  
    // Set the defaults, and parse the rest of the line.  
    float ur = 1.0;
    float offset = 0.0;
    float preference = 0.0;
    int isObjectInEffectForOptExplosion = 1;

    // usage rate        
    result = flatFileManager.nextFloat(ur, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // offset
    if (result == 0)
	result = flatFileManager.nextFloat(offset, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // preference
    // Use the right model helper to get the preference info.
    // (seems like we should be able to do this with out testing every time...)
    // FINISH_ME: lp model    
    preference = heurModelHelper_->defaultAggregateBopPreference();
    // else if ( lpModelHelper_ != 0 )
    // preference = lpModelHelper_->defaultAggregateBopCost(); //
    
    if (result == 0)
      result = flatFileManager.nextFloat(preference, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // isObjectInEffectForOptExplosion flag
    if (result == 0)
      result = flatFileManager.nextInt(isObjectInEffectForOptExplosion, next, fileName, dataLine, lineNo, false, 0, 1);
 
    // scale the  offset
    offset = offset / theCal.workUnits(0);

    result = sceAddAggregateBomStructure(theWitRun, 
                                         mpHelper,
                                         mfgPart, 
                                         mfgPdf, 
                                         aggregatePart,  
                                         aggregatePdf, 
                                         ur,
                                         offset,
                                         preference,
                                         isObjectInEffectForOptExplosion,
                                         fileName,
                                         dataLine,
                                         lineNo);
    if (result == -1) {
      badCount++;
      continue;
    }
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
        outFile << dataLine << std::endl;
      }
 }

 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}



// ---------------------------------------------------------------
void 
LgFrInitialProblemForSce::processFeatureSetToOptionRatioFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{
 std::string plannerTopLevelPart;
 std::string bbCategory;
 std::string geo;
 std::string mfgOptionPart;

 std::string dataLine;
 
 LgFrSceFlatFileMgr & flatFileManager =
   this->sceScenario()->flatFileMgr();

 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 int nPeriods = theCal.nPeriods();
 int result;

 // Note, we'll be setting negative production costs which
 // causes a Wit warning message. Suppress it.
 witSetMesgTimesPrint(theWitRun, WitTRUE, 605, 0);
 
 float * featureRatio = new float[nPeriods];

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 std::string engine  = this->sceScenario()->setOfParameters().getString("engine"); 

 this->openInputFileAndScrubOutputFile(fileName,
				       "Feature Set to Option Ratio",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);





 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 while(inFile.peek() != EOF) {

     // read the next BOM line
    std::getline( inFile, dataLine );
    lineNo++;

    //  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
	commentCount++;
	continue;
    }

    // check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() 
			 << fileName 
			 << (int)lineNo 
			 << dataLine);
        }

    
    SCETokenizer next(dataLine);

// get the planner Top Level Part Name
    plannerTopLevelPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					   lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
	badCount++;
	continue;
    }

// get the geo
    geo = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, MANDATORY);

// get the BB category
    bbCategory = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
				  lineNo, MANDATORY);

// get the (mfg) Option Part  name
    mfgOptionPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
				     lineNo, MANDATORY);


    // The rest of the fields are optional.  
    // Set the defaults, and parse the rest of the line.  
    float usageRate;

    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++)
	featureRatio[t] = 0.0;


    // usageRate
    result = flatFileManager.nextFloat(usageRate, next, fileName, dataLine, lineNo, false, -FLT_MAX);



    // featureRatio vector
    if (result == 0)  {
	int t = 0; 
	for (t=0; t<nPeriods; t++)  {
	    result = flatFileManager.nextFloat(featureRatio[t], next, fileName, dataLine, lineNo, false, 0.0);
	    if (result != 0)
		break;
	}
    }

   
    if (engine == "optimization") 
      result = sceAddBBtoOptionBomStructure(
					    theWitRun, 
					    mpHelper,
					    theCal,
					    plannerTopLevelPart, 
					    geo,
					    bbCategory,
					    mfgOptionPart,
					    usageRate, 
					    featureRatio,
					    fileName,
					    dataLine,
					    lineNo);
    else
      result = sceAddBBtoOptionBomStructureViaProportionalRoute(
								theWitRun, 
								mpHelper,
								theCal,
								plannerTopLevelPart, 
								geo,
								bbCategory,
								mfgOptionPart,
								usageRate, 
								featureRatio,
								fileName,
								dataLine,
								lineNo);

    if (! result)
      {
        recCount ++;
        if (myScrubber -> scrubMode() && outFile.good())
          {
            outFile << dataLine << std::endl;
          }
      }
    else
      badCount++;
 }
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close(); 
 
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);

 delete [] featureRatio;
 // turn message 605 back on.
 witSetMesgTimesPrint(theWitRun, WitTRUE, 605, UCHAR_MAX); 

}


// ---------------------------------------------------------------
void 
LgFrInitialProblemForSce::processFeatureSetToOptionRatioFile61(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{
  
  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
  MclFacility * theErrFacil            = this->sceScenario()->getErrFacility();
  LgFrSceScrubber * myScrubber         = this->sceScenario()->scrubber();
  
  std::ifstream inFile;
  std::ofstream outFile;
  this->openInputFileAndScrubOutputFile(fileName, "Feature Set to Option Ratio", inFile, outFile, myScrubber, theErrFacil);
  
  // attributes to read off file
  std::string dataLine;
  
  std::string plannerTopLevelPart;
  std::string sourceLoc;
  std::string geoPreD20;
  std::string bbCategory;
  std::string mfgOptionPart;
  std::string requestDate;
  float       usageRate;
  float       attachRate;
  float       mwoVal;
  float       mfOffsetVal;
  int         mfoPhantomFlag;
  float       mfSupplyVal;
  int         earlyPeriod;
  int         latePeriod;
  
  
  std::string geo;  
  int requestPeriod;  
  std::string engine  = this->sceScenario()->setOfParameters().getString("engine"); 
  
  int nPeriods = theCal.nPeriods();
  int result;
  
  // Note, we'll be setting negative production costs which causes a Wit warning message. Suppress it.
  witSetMesgTimesPrint(theWitRun, WitTRUE, 605, 0);
  
  std::cout << "Processing " << fileName << ":\t" << std::flush;
  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;
  long lineNo = 0;
  while(inFile.peek() != EOF) {
    std::getline( inFile, dataLine );
    lineNo++;
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
      commentCount++;
      continue;
    }
    if ( flatFileManager.containsIllegalCharacters( dataLine )) {
      (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
    }
    SCETokenizer next(dataLine);
    
    plannerTopLevelPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) { badCount++; continue; }
    sourceLoc = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) { badCount++; continue; }
    geoPreD20 = flatFileManager.nextQuotedDemandString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) { badCount++; continue; }
    bbCategory = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);
    if (result == -1) { badCount++; continue; }
    mfgOptionPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) { badCount++; continue; }
    
    // requestDate and usageRate
    bool dieIfErrorFlag = true;
    result = flatFileManager.readDatedDemandFloatRecord(usageRate, requestDate, dieIfErrorFlag, -FLT_MAX, FLT_MAX,
							next, fileName, dataLine, lineNo, mpHelper, theCal);
    
    
    // Map requestDate to requestPeriod
    //       rule: reject early 
    //       rule: reject late
    requestPeriod = theCal.period(requestDate);  
    if (requestPeriod >= nPeriods)  {
      (*theErrFacil)("DatedRecordBeyondHorizonEndIgnore",MclArgList() << fileName << (int)lineNo << dataLine);
      badCount++; continue; }
    if (requestPeriod < 0)  {
     (*theErrFacil)("DatedRecordBeforeHorizonStartIgnore",MclArgList() << fileName << (int)lineNo << dataLine);
     badCount++; continue; }
   
   
    // attachRate 
    result = flatFileManager.nextFloat(attachRate, next, fileName, dataLine, lineNo, false, 0.0);
    if (result == -1) { badCount++; continue; }
    // mfOffset
    result = flatFileManager.nextFloat(mfOffsetVal, next, fileName, dataLine, lineNo, false, -FLT_MAX, FLT_MAX);
    if (result == -1) { badCount++; continue; }
    // featurePhantomFlag
    result = flatFileManager.nextInt(mfoPhantomFlag, next, fileName, dataLine, lineNo, false, 0, 1);
    if (result == -1) { badCount++; continue; }
    // mfSupply
    result = flatFileManager.nextFloat(mfSupplyVal, next, fileName, dataLine, lineNo, false, 0, FLT_MAX);
    if (result == -1) { badCount++; continue; }
   
    // effectivity Dates: earlyPeriod, latePeriod
    result = flatFileManager.nextStartEndDates(earlyPeriod, latePeriod, next, fileName, lineNo, dataLine, theCal);
    if (result == 1) { badCount++; continue; }
   
    // FINISH_ME_61: Is this the right rule ??
    if ((earlyPeriod == nPeriods) || (latePeriod < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() << "Effectivity dates" << fileName << (int) lineNo << dataLine << "Featratio entry ignored.");
      badCount++; continue; }
   
    // D2O: Append the RequestDate onto the demand key to enforce period uniqueness
    geo = mpHelper.expandDemandKeyforD2O(geoPreD20, requestDate, sourceLoc);
   
    if (engine == "optimization") {
      LgFrTimeVecFloat attachRateTV(nPeriods, attachRate);
      result = sceAddBBtoOptionBomStructure(theWitRun, mpHelper, theCal,
					    plannerTopLevelPart, geo, bbCategory, mfgOptionPart,
					    usageRate, attachRateTV.data(),
					    fileName, dataLine, lineNo);
    }
    else {
      result = sceAddBBtoOptionBomStructureViaProportionalRoute61(theWitRun, mpHelper, theCal,
								  plannerTopLevelPart, geo, bbCategory, mfgOptionPart,
								  usageRate, attachRate, mfOffsetVal, mfoPhantomFlag, mfSupplyVal,
								  earlyPeriod, latePeriod, requestPeriod, fileName, dataLine, lineNo);
    }
   
    if (! result)  {
      recCount ++;
      if (myScrubber -> scrubMode() && outFile.good())	{
	outFile << dataLine << std::endl;
      }
    }
    else
      badCount++;
  }
  std::cout << recCount << " records processed." << std::endl;
  inFile.close();
  outFile.close(); 
 
  flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);

  // turn message 605 back on.
  witSetMesgTimesPrint(theWitRun, WitTRUE, 605, UCHAR_MAX); 
 
}


// ---------------------------------------------------------
void
LgFrInitialProblemForSce::processOptionRevenueFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper,
    const float defaultOptionShipReward,
    const float periodicInterestRate)
{
 std::string plannerTopLevelPart;
 std::string geo;
 std::string mfgOptionPart;

 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager =
   this->sceScenario()->flatFileMgr(); 

 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 int nPeriods = theCal.nPeriods();
 int result;

 // optional paramter
 float * featureRev = new float[nPeriods];

 // Note, we'll be setting negative production costs which
 // causes a Wit warning message. Suppress it.
 witSetMesgTimesPrint(theWitRun, WitTRUE, 605, 0);

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "Option Revenue",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);
       
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 while(inFile.peek() != EOF) {

   // read the next BOM line
   std::getline( inFile, dataLine );
   lineNo++;

   //  skip blanks or comments *
   dataLine = sce::strip( dataLine );
   if ((dataLine.empty()) || (dataLine[0] == '*')) {
     commentCount++;
     continue;
   }

   // check if the line contains any illegal characters
   if ( flatFileManager.containsIllegalCharacters( dataLine )) {
     (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
   }

    
   SCETokenizer next(dataLine);

   // get the planner Top Level Part Name
   plannerTopLevelPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					   lineNo, OPTIONAL_WITH_MESSAGE);
   if (result == -1) {
     badCount++;
     continue;
   }

   // get the geo
   geo = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, MANDATORY);


   // get the (mfg) Option Part  name
   mfgOptionPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
				     lineNo, MANDATORY);


   // The rest of the fields are optional.  
   // Set the defaults, and parse the rest of the line.  
   
   int t = 0; // Pulled out of the for below by RW2STL
   for (t=0; t<nPeriods; t++) {
     featureRev[t] = - defaultOptionShipReward;
   }


   // featureRev vector
   if (result == 0)  {
     int t = 0; // Pulled out of the for below by RW2STL
     for (t=0; t<nPeriods; t++)  {
       result = flatFileManager.nextFloat(featureRev[t],
                                 next, fileName, dataLine, lineNo, false, -FLT_MAX);
       if (result != 0)
         break;
       featureRev[t] = - featureRev[t];
     }
   }
#ifdef BUILDING_BLOCKS_VIA_MULTI_BOPS
   result = sceSetOptionRevenueViaMultiRoute(
                                theWitRun, 
                                mpHelper,
                                plannerTopLevelPart, 
                                geo,
                                mfgOptionPart,
                                featureRev,
                                periodicInterestRate,
                                fileName,
                                dataLine,
                                lineNo);
#else
   result = sceSetOptionRevenue(
                                theWitRun, 
                                mpHelper,
                                plannerTopLevelPart, 
                                geo,
                                mfgOptionPart,
                                featureRev,
                                periodicInterestRate,
                                fileName,
                                dataLine,
                                lineNo);
#endif

   if (! result)
     {
       recCount ++;
       if (myScrubber -> scrubMode() && outFile.good())
         {
           outFile << dataLine << std::endl;
         }
     }
   else
      badCount++;
 }
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);

 delete [] featureRev;
 // turn message 605 back on.
 witSetMesgTimesPrint(theWitRun, WitTRUE, 605, UCHAR_MAX); 
}






// ---------------------------------------------------------
void
LgFrInitialProblemForSce::processTopLevelToOptionalFeatureFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

  // Detect that if a maxWithout is 1.0, then the
  // option is a PURE_OPTION, otherwise it is a GATING_OPTION
  // (aka maxWithout option).
  //
  // For PURE_OPTIONS, set up as a direct bom with substitute
  // to a global NULL_Substitute.
  //
  // For a GATING_OPTION (aka maxWithout option), set up as a
  // direct bom with substitute to a maxWithout null substitute if
  // maxWithout ratio is > 0.0. Otherwise, if maxWithout ratio
  // = 0.0 in all periods, set up as a direct bom with substutite
  // to a "global" maxWithout null substitute.
  
  std::string plannerTopLevelPart;
  std::string geo;
  std::string mfgOptionPart;
  std::string dataLine;

  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  int nPeriods = theCal.nPeriods();
  int result;

  // Note, we'll be setting negative production costs which
  // causes a Wit warning message. Suppress it.
  witSetMesgTimesPrint(theWitRun, WitTRUE, 605, 0);
  
  float * featureRatio = new float[nPeriods];
  float * maxWithout   = new float[nPeriods];
  
  float * defaultGatingOptionSubPenalty = new float[nPeriods];
  
  LgFrSceSetOfParameters & setOfParameters =
    this->sceScenario()->setOfParameters();
  
  
  float defaultOptionShipReward = setOfParameters.getFloat("defaultOptionShipReward");
  
  int tt = 0;
  for (tt=0; tt<nPeriods; tt++)
    defaultGatingOptionSubPenalty[tt] = defaultOptionShipReward;

  std::ifstream inFile;
  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  
  this->openInputFileAndScrubOutputFile(fileName,
					"Top Level to Optional Feature",
					inFile,
					outFile,
					myScrubber,
					theErrFacil);
  
  std::cout << "Processing " << fileName << ":\t" << std::flush;
  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;
  long lineNo = 0;
  while(inFile.peek() != EOF) {
    
// read the next BOM line
    std::getline( inFile, dataLine );
    lineNo++;
    
//  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
      commentCount++;
      continue;
    }

// check if the line contains any illegal characters
    if ( flatFileManager.containsIllegalCharacters( dataLine )) {
      (*theErrFacil)("IllegalCharacter",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
    }

    
    SCETokenizer next(dataLine);
    
// get the planner Top Level Part Name
    plannerTopLevelPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					   lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
	badCount++;
	continue;
    }

// get the geo
    geo = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, OPTIONAL_WITH_MESSAGE);

    if (result == -1) {
	badCount++;
	continue;
    }

// get the (mfg) Option Part  name
    mfgOptionPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
				     lineNo, OPTIONAL_WITH_MESSAGE);

    if (result == -1) {
	badCount++;
	continue;
    }

    // The rest of the fields are optional.  
    // Set the defaults, and parse the rest of the line.  
    float usageRate;

    // First set the feature ratio default to be 0.0
    // Initialize the maxWithout to be 1.0.
    int t = 0; 
    for (t=0; t<nPeriods; t++) {
      featureRatio[t] = 0.0;
      maxWithout[t] = 1.0;
    }

    // usageRate
    result = flatFileManager.nextFloat(usageRate, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // featureRatio vector
    if (result == 0)  {
	int t = 0; 
	for (t=0; t<nPeriods; t++)  {
	    result = flatFileManager.nextFloat(featureRatio[t], next, fileName, dataLine, lineNo, false, 0.0);
	    if (result != 0)
		break;
	}
    }

    // maxWithout Ratio vector must be between -1 and +1
    if (result == 0)  {
	int t = 0;
	for (t=0; t<nPeriods; t++)  {
	    result = flatFileManager.nextFloat(maxWithout[t], 
					       next, fileName, dataLine, lineNo, 
					       false, -1.0, 1.0);
	}
    }


    LgFrTimeVecFloat maxWithoutTimeVec( (size_t) nPeriods, maxWithout );

    float maxMWO = maxWithoutTimeVec.max();
    float minMWO = maxWithoutTimeVec.min();


    // Now make sure the maxWithout does not have positive and negative 
    // Numbers.  This is an error.
    if ((minMWO < -IPSCE_FLT_EPS) && (maxMWO > IPSCE_FLT_EPS)) {
	// if there are negative and positive numbers mixed up
	// then complain and  reject.  
	(*theErrFacil)("MwoPosAndNegNotSupported",MclArgList() 
		       << fileName 
		       << (int)lineNo
		       << dataLine
		       << plannerTopLevelPart
		       << geo 
		       << mfgOptionPart);
	badCount++;
	continue;
    }


    // Analyze maxWithout to determine what kind of standalone feature we have:
    // it must be one of:
    //     SCE_FEATURE_TYPE_CUSTOMER_CHOICE
    //     SCE_FEATURE_TYPE_PURELY_OPTIONAL
    //     SCE_FEATURE_TYPE_MANDATORY
    //     SCE_FEATURE_TYPE_MWO
    int featureType;
    if (minMWO < -IPSCE_FLT_EPS) 
      featureType = SCE_FEATURE_TYPE_CUSTOMER_CHOICE;
    else if (minMWO < IPSCE_FLT_EPS)
      featureType = SCE_FEATURE_TYPE_MANDATORY;
    else if (minMWO < 1.0 - IPSCE_FLT_EPS)
      featureType = SCE_FEATURE_TYPE_MWO;
    else
      featureType = SCE_FEATURE_TYPE_PURELY_OPTIONAL;



    if (featureType == SCE_FEATURE_TYPE_CUSTOMER_CHOICE)
      result = sceAddCustChoiceFeatureStructure(theWitRun, 
						mpHelper,
						theCal,
						plannerTopLevelPart, 
						geo,
						mfgOptionPart,
						usageRate, 
						featureRatio,
						defaultGatingOptionSubPenalty,
						fileName,
						dataLine,
						lineNo);
      
  
    else       {
      result = sceAddStdAloneOptionBomStructure(theWitRun, 
						mpHelper,
						plannerTopLevelPart, 
						geo,
						mfgOptionPart,
						usageRate, 
						featureRatio,
						maxWithout,
						defaultGatingOptionSubPenalty,
						featureType,
						fileName,
						dataLine,
						lineNo);
    }


      
    // increment the feature counter 
    if (! result)
      sceScenario()->incrementStandaloneFeatureArcs();


    if (! result)
      {
        recCount ++;
        if (myScrubber -> scrubMode() && outFile.good())
          {
            outFile << dataLine << std::endl;
          }
      }
    else
      badCount++;
  }

 std::cout << recCount << " records processed." << std::endl;

 inFile.close();
 outFile.close();
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);

 delete [] featureRatio;
 delete [] maxWithout;
 delete [] defaultGatingOptionSubPenalty;
 // turn message 605 back on.
 witSetMesgTimesPrint(theWitRun, WitTRUE, 605, UCHAR_MAX); 
 
}






// ---------------------------------------------------------
void
LgFrInitialProblemForSce::removeGpdBopOffsets(
    WitRun * const theWitRun,
    LgFrMultiPlantHelper & mpHelper)
{
  char ** partList;
  int nParts;
  std::string pp, geo;


  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);

  float * zeroVec = new float [nPeriods];
  int t = 0; 
  for (t=0; t<nPeriods; t++)
    zeroVec[t] = 0.0;

  witGetParts ( theWitRun, &nParts, &partList );
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    if (mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun, partList[i], pp, geo)) {
      witSetBopEntryOffset(theWitRun, partList[i], 0, zeroVec);
    }
  }

  for (i=0; i<nParts; i++) 
    witFree(partList[i]);
  witFree(partList);

  delete [] zeroVec;

}

// ---------------------------------------------------------
void
LgFrInitialProblemForSce::processMachineFeatureOffsetFile(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

  // Set an offset between a machine and a feature.
  // This would be in ADDITION to:
  //    transit_time (on a geo source)
  //    cycleTime on the machine
  //    cycleTime on the feature
  //
  // This method also allows a phantom flag to be set which
  // nullifies the feature cycleTime.
  
  std::string plannerTopLevelPart;
  std::string geo;
  std::string mfgOptionPart;
  int featurePhantomFlag;
  int start;
  int end;
  
  std::string dataLine;

  LgFrSceFlatFileMgr & flatFileManager =
    this->sceScenario()->flatFileMgr();   

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  int nPeriods = theCal.nPeriods();
  int result;

  
  LgFrSceSetOfParameters & setOfParameters =
    this->sceScenario()->setOfParameters();

  std::ifstream inFile;
  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  
  this->openInputFileAndScrubOutputFile(fileName,
					"Machine to Feature Offset File",
					inFile,
					outFile,
					myScrubber,
					theErrFacil);

  
  
  std::cout << "Processing " << fileName << ":\t" << std::flush;
  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;
  long lineNo = 0;
  while(inFile.peek() != EOF) {
    
// read the next BOM line
    std::getline( inFile, dataLine );
    lineNo++;
    
//  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
      commentCount++;
      continue;
    }

// check if the line contains any illegal characters
    if ( flatFileManager.containsIllegalCharacters( dataLine )) {
      (*theErrFacil)("IllegalCharacter",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
    }

    
    SCETokenizer next(dataLine);

    // set defaults
    featurePhantomFlag = 0;
    start = 0;
    end = nPeriods - 1;
    

    
    // get the planner Top Level Part Name
    plannerTopLevelPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					   lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
	badCount++;
	continue;
    }

    // get the geo
    geo = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, OPTIONAL_WITH_MESSAGE);

    if (result == -1) {
	badCount++;
	continue;
    }

    // get the (mfg) Option Part  name
    mfgOptionPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
				     lineNo, OPTIONAL_WITH_MESSAGE);

    if (result == -1) {
	badCount++;
	continue;
    }

    // offsetVal
    float offsetVal;
    result = flatFileManager.nextFloat(offsetVal, next, fileName, dataLine, lineNo, true, -FLT_MAX);

    if (result == -1) {
	badCount++;
	continue;
    }


    // ++++  Now comes the optional Part of the file +++++
    // featurePhantomFlag
    result = flatFileManager.nextInt(featurePhantomFlag, next, fileName, dataLine, lineNo, false, 0, 1);

    if (result >= 0)
      result = flatFileManager.nextStartEndDates(start, end, next, fileName, lineNo, dataLine, theCal);



    
    if (result == 1) {
       badCount++;        
       continue;
    }


    result = sceSetMachineFeatureOffset(theWitRun, 
					mpHelper,
					theCal,
					plannerTopLevelPart, 
					geo,
					mfgOptionPart,
					offsetVal, 
					featurePhantomFlag,
					start,
					end,
					fileName,
					dataLine,
					lineNo);
    

      
    if (! result) {
      recCount++;
      if (myScrubber -> scrubMode() && outFile.good()) {
	outFile << dataLine << std::endl;
      }
    }
    else
      badCount++;
  }

 std::cout << recCount << " records processed." << std::endl;

 inFile.close();
 outFile.close();

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);

 
}


// ---------------------------------------------------------
void
LgFrInitialProblemForSce::processMachineFeatureSupplyFile(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

  // Provide a supplyVol for the feature but peg it to a machine
  std::string plannerTopLevelPart;
  std::string geo;
  std::string mfgOptionPart;
  std::string dataLine;

  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  int nPeriods = theCal.nPeriods();
  int result;

  
  float * supplyVol = new float[nPeriods];
  
  LgFrSceSetOfParameters & setOfParameters =
    this->sceScenario()->setOfParameters();
  
  
  std::ifstream inFile;
  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  
  this->openInputFileAndScrubOutputFile(fileName,
					"Machine Feature Supply ",
					inFile,
					outFile,
					myScrubber,
					theErrFacil);
  
  std::cout << "Processing " << fileName << ":\t" << std::flush;
  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;
  long lineNo = 0;
  while(inFile.peek() != EOF) {
    
// read the next data line
    std::getline( inFile, dataLine );
    lineNo++;
    
//  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
      commentCount++;
      continue;
    }

// check if the line contains any illegal characters
    if ( flatFileManager.containsIllegalCharacters( dataLine )) {
      (*theErrFacil)("IllegalCharacter",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
    }

    
    SCETokenizer next(dataLine);
    
// get the planner Top Level Part Name
    plannerTopLevelPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					   lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
	badCount++;
	continue;
    }

// get the geo
    geo = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, OPTIONAL_WITH_MESSAGE);

    if (result == -1) {
	badCount++;
	continue;
    }

// get the (mfg) Option Part  name
    mfgOptionPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
				     lineNo, OPTIONAL_WITH_MESSAGE);

    if (result == -1) {
	badCount++;
	continue;
    }


    // Clear the data vector
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++) {
      supplyVol[t] = 0.0;
    }




    // supplyVol vector
    if (result == 0)  {
	int t = 0; // Pulled out of the for below by RW2STL
	for (t=0; t<nPeriods; t++)  {
	    result = flatFileManager.nextFloat(supplyVol[t], next, fileName, dataLine, lineNo, false, 0.0);
	    if (result != 0)
		break;
	}
    }



    result = sceAddMachineFeatureSupplyVol(theWitRun, 
					   mpHelper,
					   plannerTopLevelPart, 
					   geo,
					   mfgOptionPart,
					   supplyVol,
					   fileName,
					   dataLine,
					   lineNo);
      


    if (! result)
      {
        recCount ++;
        if (myScrubber -> scrubMode() && outFile.good())
          {
            outFile << dataLine << std::endl;
          }
      }
    else
      badCount++;
  }

 std::cout << recCount << " records processed." << std::endl;

 inFile.close();
 outFile.close();
 // flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);

 delete [] supplyVol;
}






// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processTopLevelToFeatureSetFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{
    std::string plannerTopLevelPart;
    std::string geo;
    std::string plannerFeatureSetPart;
    std::string bbCategory;
    std::string dataLine;

    LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();     
    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    int nPeriods = theCal.nPeriods();
    int result;

    std::ifstream inFile;
    std::ofstream outFile;
    LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();

    float * gammaMaxVector = new float[nPeriods];

    std::string engine  = this->sceScenario()->setOfParameters().getString("engine"); 
    
    this->openInputFileAndScrubOutputFile(fileName, "Top Level To Feature Set File", inFile, outFile, myScrubber, theErrFacil);

    std::cout << "Processing " << fileName << ":\t" << std::flush;
    long recCount = 0;
    long badCount = 0;
    long commentCount = 0;
    long lineNo = 0;
    while(inFile.peek() != EOF) {
	std::getline( inFile, dataLine );
	lineNo++;
	dataLine = sce::strip( dataLine );
	if ((dataLine.empty()) || (dataLine[0] == '*')) {
	    commentCount++;
	    continue;
	}
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
        }
	SCETokenizer next(dataLine);

	// get the planner Top Level Part Name
	plannerTopLevelPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
	if (result == -1) {
	    badCount++;
	    continue;
	}

	// get the geo
	geo = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);

	// get the bbCategory name
	bbCategory = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);


	// The rest of the fields are optional.  
	// Set the defaults, and parse the rest of the line.  
	int early = 0;
	int late = nPeriods-1;
	float gammaMin = 1.0;
	float gammaMax;

	// In new model, the gammaMax is replace with a vector.
	// for proportional split, we use the vector.   For LP model we pick up the first period 
	// as gammaMax scalar.  FINISH_ME is to convert the LP logic to a vector.


	int t = 0; // Pulled out of the for below by RW2STL
	for (t=0; t<nPeriods; t++)
	  gammaMaxVector[t] = 0.0;


	// gammaMax Vector is initiailized back to zero, before re-reading
	if (result == 0)  {
	  int t = 0;
	  for (t=0; t<nPeriods; t++)  {
	    result = flatFileManager.nextFloat(gammaMaxVector[t], next, fileName, dataLine, lineNo, false, 0.0);
	    if (result != 0)
	      break;
	  }
	}
	

	if (engine == "optimization" ) {
	  gammaMax = gammaMaxVector[0];
	  result = sceAddBbCategoryPartAndStructure(theWitRun, 
						    mpHelper,
						    plannerTopLevelPart,
						    geo,
						    bbCategory,
						    gammaMax,
						    gammaMin,
						    early, 
						    late,
						    fileName,
						    dataLine,
						    lineNo);
	}
	else {
	  result = sceAddBbCategoryPartAndStructureViaProportionalRoute(theWitRun, 
									mpHelper,
									plannerTopLevelPart,
									geo,
									bbCategory,
									gammaMaxVector,
									gammaMin,
									early, 
									late,
									fileName,
									dataLine,
									lineNo);
	}
	if (! result)
          {
	    recCount ++;
            if (myScrubber -> scrubMode() && outFile.good())
              {
                outFile << dataLine << std::endl;
              }
          }
	else
	    badCount++;
    }
    std::cout << recCount << " records processed." << std::endl;
    inFile.close();
    outFile.close();

    delete [] gammaMaxVector;
    flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}



// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processTopLevelToFeatureSetFile61(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{
  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();     
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
  
  
  std::ifstream inFile;
  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  this->openInputFileAndScrubOutputFile(fileName, "Top Level To Feature Set File", inFile, outFile, myScrubber, theErrFacil);

  int result;  
  int nPeriods = theCal.nPeriods();  
  std::string engine  = this->sceScenario()->setOfParameters().getString("engine"); 
  
  // attributes to read off file
  std::string dataLine;

  std::string plannerTopLevelPart;
  std::string sourceLoc;
  std::string geoPreD20;
  std::string bbCategory;
  std::string requestDate;
  float       gammaMax;

  int         earlyPeriod;
  int         latePeriod;

  float gammaMin = 1.0;  // default  
  
  std::string geo;  
  int requestPeriod;  
  std::string plannerFeatureSetPart;

  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;
  long lineNo = 0;
  std::cout << "Processing " << fileName << ":\t" << std::flush;
  while(inFile.peek() != EOF) {
    std::getline( inFile, dataLine );
    lineNo++;
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
      commentCount++;
      continue;
    }
    if ( flatFileManager.containsIllegalCharacters( dataLine )) {
      (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
    }
    SCETokenizer next(dataLine);
    
    plannerTopLevelPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) { badCount++; continue; }
    sourceLoc = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) { badCount++; continue; }
    geoPreD20 = flatFileManager.nextQuotedDemandString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) { badCount++; continue; }
    bbCategory = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) { badCount++; continue; }
    
    // requestDate and gammaMax
    bool dieIfErrorFlag = true;
    result = flatFileManager.readDatedDemandFloatRecord(gammaMax, requestDate, dieIfErrorFlag, 0.0, FLT_MAX,
							next, fileName, dataLine, lineNo, mpHelper, theCal);
    

    // Map requestDate to requestPeriod
    //      rule: reject early 
    //      rule: reject late
    requestPeriod = theCal.period(requestDate);  
    if (requestPeriod >= nPeriods)  {
      (*theErrFacil)("DatedRecordBeyondHorizonEndIgnore",MclArgList() << fileName << (int)lineNo << dataLine);
      badCount++; continue; }
    if (requestPeriod < 0)  {
      (*theErrFacil)("DatedRecordBeforeHorizonStartIgnore",MclArgList() << fileName << (int)lineNo << dataLine);
      badCount++; continue; }


    
    // effectivity Dates: earlyPeriod, latePeriod
    result = flatFileManager.nextStartEndDates(earlyPeriod, latePeriod, next, fileName, lineNo, dataLine, theCal);
    if (result == 1) { badCount++; continue; }

    // FINISH_ME_61: Is this the right rule ??
    if ((earlyPeriod == nPeriods) || (latePeriod < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() << "Effectivity dates" << fileName << (int) lineNo << dataLine << "Featratio entry ignored.");
      badCount++; continue; }


    // D2O:  Append the RequestDate onto the demand key to enforce period uniqueness
    geo = mpHelper.expandDemandKeyforD2O(geoPreD20, requestDate, sourceLoc);


    
    if (engine == "optimization" ) {
      result = sceAddBbCategoryPartAndStructure(theWitRun, 
						mpHelper,
						plannerTopLevelPart,
						geo,
						bbCategory,
						gammaMax,
						gammaMin,
						earlyPeriod, 
						latePeriod,
						fileName,
						dataLine,
						lineNo);
    }
    else {
      result = sceAddBbCategoryPartAndStructureViaProportionalRoute61(theWitRun, 
								      mpHelper,
								      plannerTopLevelPart,
								      geo,
								      bbCategory,
								      gammaMax,
								      gammaMin,
								      earlyPeriod, 
								      latePeriod,
								      fileName,
								      dataLine,
								      lineNo);
    }
    if (! result)
      {
	recCount ++;
	if (myScrubber -> scrubMode() && outFile.good())
	  {
	    outFile << dataLine << std::endl;
	  }
      }
    else
      badCount++;
  }
  std::cout << recCount << " records processed." << std::endl;
  inFile.close();
  outFile.close();

  flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}




// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processInterPlantFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string mfgParentPartName;
 std::string mfgChildPartName;
 std::string parentPdf;
 std::string childPdf;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 int nPeriods = theCal.nPeriods();
 int result;
 int result2;

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "InterPlant",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 long nonUniqueCount = 0;

 while(inFile.peek() != EOF) {

// read the next BOM line
    std::getline( inFile, dataLine );
    lineNo++;

//  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
	commentCount++;
	continue;
    }

// check if the line contains any illegal characters
    if ( flatFileManager.containsIllegalCharacters( dataLine )) {
      (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
    }

    SCETokenizer next(dataLine);

// get the mfg Parent PartName 
    mfgParentPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					 lineNo, OPTIONAL_WITH_MESSAGE);

    if (result == -1) {
	badCount++;
	continue;
    }

// get the parent PDF
    parentPdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
				 lineNo, MANDATORY);


// get the mfg Child PartName 
    mfgChildPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					lineNo, MANDATORY);

// get the child PDF
    childPdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
				lineNo, MANDATORY);

// The rest of the fields are optional.  
// Set the defaults, and parse the rest of the line.  
    float transitTime = 0.0;
    int   early = 0;
    int   late = nPeriods-1;
    float minLotSize = 1.0;
    float incLotSize = 1.0;
    float usageRate = 1.0;
    float preference = 0.0; // FINISH_ME: get the fault from the paramMgr...
    int   isObjectInEffectForOptExplosion = 1;
    int   expAllowed = 1;
    float   routingShare = 1.0;
    
    // transitTime  (negative usage time)
    if (result >= 0)
	result = flatFileManager.nextFloat(transitTime, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // effectivity Dates
    if (result >= 0)
	result2 = flatFileManager.nextStartEndDates(early, late, next,
                                    fileName, lineNo, dataLine, theCal);
    if (result2 == 1) {
	badCount++;
	continue;
    }
    if ((early == nPeriods) || (late < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() 
		     << "Effectivity dates" 
		     << fileName 
		     << (int)lineNo 
		     << dataLine 
		     << "Interplant entry ignored.");
	badCount++;
	continue;
    }

    // minimum and incremental lot sizes
    // note: ranges and defaults are consitent with those for 
    // manufacturing minLotSize and incLotSize. 
    if (result2 == 0)
      result =  flatFileManager.nextFloat( minLotSize, next, fileName, dataLine, lineNo, false, 0.0 );
    
    if ( result == 0 && minLotSize >= 0.0 )
      result = flatFileManager.nextFloat( incLotSize, next, fileName, dataLine,
                           lineNo, false, 1.0);

    // usageRate
    if ( result == 0 )
      result = flatFileManager.nextFloat( usageRate, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // preference
    preference = heurModelHelper_->defaultInterPlantBopPreference();
    // FINISH_ME lp model 
    // if ( lpModelHelper_ != 0 )
    // preference = lpModelHelper_->defaultInterPlantBopCost(); ??
    if ( result == 0 )
      result = flatFileManager.nextFloat( preference, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // is ObjectInEffectForOptExplosion
    if ( result == 0 )
      result = flatFileManager.nextInt( isObjectInEffectForOptExplosion, next, fileName, dataLine, lineNo, false, 0, 1);

    // expAllowed
    if ( result == 0 )
      result = flatFileManager.nextInt( expAllowed, next, fileName, dataLine, lineNo, false, 0, 1);


    if ( result == 0 )
      result = flatFileManager.nextFloat( routingShare, next, fileName, dataLine, lineNo, false, 0.0);

      
    result = addInterPlantBomStructure(
	theWitRun, 
	mpHelper,
	theCal,
	mfgParentPartName, 
	parentPdf,
	mfgChildPartName, 
	childPdf, 
	transitTime, 
	early,
	late,
        minLotSize,
        incLotSize,
        usageRate,
        preference,
        isObjectInEffectForOptExplosion,
        expAllowed,
	routingShare,
	fileName,
	dataLine,
	lineNo);
    
    if (result < 0) // something wrong
      badCount++;
    else {
      if (result == 0)   // good 
        recCount ++;
      else  //  good non-unique
	nonUniqueCount++;

      if (myScrubber -> scrubMode() && outFile.good()) {
	outFile << dataLine << std::endl;
      }
    }
 }

 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount-nonUniqueCount);
}





// ---------------------------------------------------------------
void 
LgFrInitialProblemForSce::processOperationSubBomFile(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string operationName;
 std::string mfgChildPartName;
 std::string mfgSubPartName;
 std::string pdf;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
 LgFrSceSetOfParameters & setOfParameters =
   this->sceScenario()->setOfParameters();
  
  
 bool globalSubNetAllowed = setOfParameters.getBool("globalSubNetAllowed");

 int nPeriods = theCal.nPeriods();
 int result, result2;

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "Operation Sub Bom",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 while(inFile.peek() != EOF) {

     // read the next BOM Sub line
    std::getline( inFile, dataLine );
    lineNo++;

//  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
	commentCount++;
	continue;
    }

// check if the line contains any illegal characters
    if ( flatFileManager.containsIllegalCharacters( dataLine )) {
      (*theErrFacil)("IllegalCharacter",MclArgList() 
		     << fileName << (int)lineNo << dataLine);
    }

    
    SCETokenizer next(dataLine);

// get the mfg Parent PartName 
    operationName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
						     lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
	badCount++;
	continue;
    }

// get the mfg Child PartName 
    mfgChildPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
							lineNo, MANDATORY);

// get the mfg Child PartName 
    mfgSubPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
						      lineNo, MANDATORY);

// get the PDF
    pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					   lineNo, MANDATORY);

    // The rest of the fields are optional.  
    // Set the defaults, and parse the rest of the line.  
    float ur = 1.0;
    int   fallout = 0;
    int   early = 0;
    int   late = nPeriods-1;
    float defaultSubPenalty =
           this->sceScenario()->setOfParameters().getFloat("defaultSubPenalty");
    int isObjectInEffectForOptExplosion = 0;
    int isSubstituteBuildable = 0;
    int isNetAllowed = 1;
    int offsetDeltaDays = 0;
    float offsetDelta = 0.0;
    float propRtg_Share = 1.0;
    
    // usage rate        
    result = flatFileManager.nextFloat(ur, next, fileName, 
				       dataLine, lineNo, false, -FLT_MAX);


    // effectivity Dates
    if (result >= 0)
	result2 = flatFileManager.nextStartEndDates(early, late, next, 
						    fileName, lineNo, dataLine, theCal);
    // if  dates messed up, then skip this record
    if (result2 == 1)
	continue;
    if ((early == nPeriods) || (late < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() 
		     << "Effectivity dates" 
		     << fileName 
		     << (int)lineNo 
		     << dataLine 
		     << "Operation Substitute BOM entry ignored.");
	badCount++;
	continue;
    }

    // fallout
    if (result2 == 0)
	result = flatFileManager.nextInt(fallout, next, fileName, dataLine, lineNo, false, 0, 100);

    // sub penalty cost
    if (result == 0)
	result = flatFileManager.nextFloat(defaultSubPenalty, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // isObjectInEffectForOptExplosion flag
    if (result == 0)
	result = flatFileManager.nextInt(isObjectInEffectForOptExplosion, next, fileName, 
					 dataLine, lineNo, false, 0, 1);    

    // isSubstituteBuildable flag
    if (result == 0)
	result = flatFileManager.nextInt(isSubstituteBuildable, next, fileName, 
					 dataLine, lineNo, false, 0, 1);    

    // netAllowed flag
    if (result == 0)
	result = flatFileManager.nextInt(isNetAllowed, next, fileName, 
					 dataLine, lineNo, false, 0, 1);    

    // offsetDeltaDays
    if (result == 0)
	result = flatFileManager.nextInt(offsetDeltaDays, next, fileName, dataLine, lineNo, false, -INT_MAX, INT_MAX);

    // scale the offsetDelta 
    offsetDelta = offsetDeltaDays / theCal.workUnits(early);

    // proportional routing share
    if (result == 0)
	result = flatFileManager.nextFloat(propRtg_Share, next, fileName, dataLine, lineNo, false, 0);




    // this is method for Operation Sub Bom
// Call the SCE init advanced to add the sub
    bool isParentAnOperation = true;
    result = sceAddSubstitute(
	theWitRun, 
	mpHelper,
	isParentAnOperation,
	operationName, 
	pdf, 
	mfgChildPartName, 
	pdf, 
	mfgSubPartName,
	pdf,
	ur,
	early, 
	late,
	fallout,
	defaultSubPenalty,
	isObjectInEffectForOptExplosion,
	isSubstituteBuildable,
	//	globalSubNetAllowed,
	isNetAllowed,
	0.,
	offsetDelta,
	propRtg_Share,
	fileName,
	dataLine,
	lineNo);
    if (! result)
          {
	    recCount ++;
            if (myScrubber -> scrubMode() && outFile.good())
              {
                outFile << dataLine << std::endl;
              }
          }

    else
	badCount++;


 }
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,
						 badCount,commentCount,recCount);
}






// ---------------------------------------------------------------
void 
LgFrInitialProblemForSce::processStandardBomSubFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string mfgParentPartName;
 std::string mfgChildPartName;
 std::string mfgSubPartName;
 std::string pdf;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
 

 LgFrSceSetOfParameters & setOfParameters =
   this->sceScenario()->setOfParameters();
  
  
 bool globalSubNetAllowed = setOfParameters.getBool("globalSubNetAllowed");
 float groupSharingExecPenaltyThresh = setOfParameters.getFloat("groupSharingExecPenaltyThresh");

 std::string globalSubParentIndString = setOfParameters.getString("globalSubParentIndString");


 int nPeriods = theCal.nPeriods();
 int result, result2;

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "BOM Substitute",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 while(inFile.peek() != EOF) {

     // read the next BOM Sub line
    std::getline( inFile, dataLine );
    lineNo++;

//  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
	commentCount++;
	continue;
    }

// check if the line contains any illegal characters
    if ( flatFileManager.containsIllegalCharacters( dataLine )) {
      (*theErrFacil)("IllegalCharacter",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
    }

    
    SCETokenizer next(dataLine);

// get the mfg Parent PartName 
    mfgParentPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					 lineNo, PURELY_OPTIONAL);
    if (result == -1) {
	badCount++;
	continue;
    }

// get the mfg Child PartName 
    mfgChildPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					lineNo, MANDATORY);

// get the mfg Child PartName 
    mfgSubPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
				      lineNo, MANDATORY);

// get the PDF
    pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, MANDATORY);

    // The rest of the fields are optional.  
    // Set the defaults, and parse the rest of the line.  
    float ur = 1.0;
    int   fallout = 0;
    int   early = 0;
    int   late = nPeriods-1;
    float defaultSubPenalty =
           this->sceScenario()->setOfParameters().getFloat("defaultSubPenalty");
    int isObjectInEffectForOptExplosion = 0;
    int isSubstituteBuildable = 0;
    int isNetAllowed = 1;
    float execPenalty = 0.0;
    float propRtg_Share = 1.0;
    
    // usage rate        
    result = flatFileManager.nextFloat(ur, next, fileName, dataLine, lineNo, false, -FLT_MAX);


    // effectivity Dates
    if (result >= 0)
	result2 = flatFileManager.nextStartEndDates(early, late, next, fileName, lineNo, dataLine, theCal);
    // if  dates messed up, then skip this record
    if (result2 == 1)
	continue;
    if ((early == nPeriods) || (late < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() 
		     << "Effectivity dates" 
		     << fileName 
		     << (int)lineNo 
		     << dataLine 
		     << "Substitute BOM entry ignored.");
	badCount++;
	continue;
    }

    // fallout
    if (result2 == 0)
	result = flatFileManager.nextInt(fallout, next, fileName, dataLine, lineNo, false, 0, 100);

    // sub penalty cost
    if (result == 0)
	result = flatFileManager.nextFloat(defaultSubPenalty, next, fileName, dataLine, lineNo, false, -FLT_MAX);

    // isObjectInEffectForOptExplosion flag
    if (result == 0)
	result = flatFileManager.nextInt(isObjectInEffectForOptExplosion, next, fileName, dataLine, lineNo, false, 0, 1);    

    // isSubstituteBuildable flag
    if (result == 0)
	result = flatFileManager.nextInt(isSubstituteBuildable, next, fileName, dataLine, lineNo, false, 0, 1);    

    // isNetAllowed flag
    if (result == 0)
	result = flatFileManager.nextInt(isNetAllowed, next, fileName, 
					 dataLine, lineNo, false, 0, 1);    

    // execPenalty
    if (result == 0)
	result = flatFileManager.nextFloat(execPenalty, next, fileName, 
					 dataLine, lineNo, false, -FLT_MAX);    

    // proportional routing share
    if (result == 0)
	result = flatFileManager.nextFloat(propRtg_Share, next, fileName, dataLine, lineNo, false, 0);


    

    bool isParentAnOperation = false;

    if (mfgParentPartName == globalSubParentIndString) {
      result = sceAddAlternatePartStructure( theWitRun, mpHelper, nPeriods,
					     mfgChildPartName, 
					     pdf, 
					     mfgSubPartName,
					     pdf,
					     ur,
					     early, 
					     late,
					     fallout,
					     defaultSubPenalty,
					     isObjectInEffectForOptExplosion,
					     isSubstituteBuildable,
					     // globalSubNetAllowed,
					     isNetAllowed,
					     // groupSharingExecPenaltyThresh,
					     execPenalty,
					     fileName,
					     dataLine,
					     lineNo);
    }
    else {
      result = sceAddSubstitute( theWitRun, mpHelper,
				 isParentAnOperation,
				 mfgParentPartName, 
				 pdf, 
				 mfgChildPartName, 
				 pdf, 
				 mfgSubPartName,
				 pdf,
				 ur,
				 early, 
				 late,
				 fallout,
				 defaultSubPenalty,
				 isObjectInEffectForOptExplosion,
				 isSubstituteBuildable,
				 // globalSubNetAllowed,
				 isNetAllowed,
				 // groupSharingExecPenaltyThresh,
				 execPenalty,
				 0.0,
				 propRtg_Share,
				 fileName,
				 dataLine,
				 lineNo);
    }
    
    if (! result)
      {
	    recCount ++;
            if (myScrubber -> scrubMode() && outFile.good())
              {
                outFile << dataLine << std::endl;
              }
          }

    else
	badCount++;


 }
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}





// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processSupplyFile(
					    WitRun * const theWitRun,   
					    const LgFrCalendar & theCal,
					    const int nPeriods,         
					    const std::string & fileName,
					    LgFrMultiPlantHelper & mpHelper)
{
  
 int nElementsChanged;
 std::string mfgPart;
 std::string pdf;
 std::string fullWitPart;
 std::ifstream inFile;
 float * supplyVol;
 long lineNo = 0;
 std::string dataLine;

 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();   
 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();


 std::ofstream outFile;

 // DATED_DEMANDS
 bool useDatedDemands = setOfParameters.getBool("useDatedDemands");
 float          theDatedSupplyVol = 0.0f;
 int            supplyPeriod = 0;
 std::string    supplyDate;


 bool aggregateSupplyInScrub = setOfParameters.getBool("aggregateSupplyInScrub");
 if (! aggregateSupplyInScrub)
   this->openInputFileAndScrubOutputFile(fileName,
				       "Supply",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);
 else {
   inFile.open(fileName.c_str());
   
   if (! inFile)  {
     (*theErrFacil)("SevereCannotOpenFile",MclArgList() 
		    << "Supply" 
		    << fileName 
		    << "");
   }
 }

 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 float * floatVec = new float[nPeriods];


 while(inFile.peek() != EOF) {
   int result = 0;

   std::getline( inFile, dataLine );
   lineNo ++;
   dataLine = sce::strip( dataLine );

   //  skip blanks or comments *
   if ((dataLine[0] == '*') || dataLine.empty()) {
     commentCount++;
     continue;
   }
   
   SCETokenizer next(dataLine);

   // check if the line contains any illegal characters
   if ( flatFileManager.containsIllegalCharacters( dataLine )) {
     (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
   }

 
   // get the partname
   mfgPart =  flatFileManager.nextQuotedString(next, result, fileName, 
					       dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
   if (result < 0)
     continue;
   
   
   // get the pdf
   pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					  lineNo, MANDATORY);
   
   fullWitPart = mpHelper.pdfPartname(mfgPart, pdf);
   
   // test to see that its a valid partName
   if (! mpHelper.isPartValid( theWitRun, fullWitPart, fileName, 
			       dataLine, lineNo, PURELY_OPTIONAL))  {
     (*theErrFacil)("UnrecognizedPartWarning",MclArgList() << fullWitPart << fileName << (int)lineNo << dataLine);
     continue;
   }


   // DATED_DEMANDS
   if (useDatedDemands) {
     bool dieIfErrorFlag = true;
     int result = flatFileManager.readDatedDemandFloatRecord( theDatedSupplyVol, supplyDate, 
							      dieIfErrorFlag, 0.0, FLT_MAX,
							      next, fileName, dataLine, lineNo, 
							      mpHelper, theCal);
     // Map supplyDate to supplyPeriod
     // rule: reject if supplyPeriod < 0
     // rule: reject if supplyPeriod >= nPeriods;
     
     // see what period it falls in
     // NOTE: this method uses period() ... which adhere's to strict calendar inclusive dates
     // NOTE: you can also use roundedPeriod() 
     supplyPeriod = theCal.period(supplyDate);  

     nElementsChanged = result;
   }
   
   // VECTORED_DEMANDS 
   else {
     int t=0;
     for (t = 0; t<nPeriods; t++) {
       int result = flatFileManager.nextFloat(floatVec[t], next, fileName, dataLine, lineNo, false,
					      -FLT_MAX, FLT_MAX);
       if (result == -1) 
	 break;
     }
     
     nElementsChanged = t;
   }
   


   if (nElementsChanged == 0 || nElementsChanged == -1) {
     badCount++;
     continue;
   }
   
   
   witGetPartSupplyVol(theWitRun, fullWitPart.c_str(), &supplyVol);
   
   // DATED IO
   if (useDatedDemands) {


    // Map supplyDate to supplyPeriod
    // rule: accept if supplyPeriod < 0, supplyVol[0] += theDatedSupplyVol;
    // rule: ignore if supplyPeriod >= nPeriods;









     if (supplyPeriod < 0) 
       supplyPeriod = 0;
     if (supplyPeriod < nPeriods) 
       supplyVol[supplyPeriod] += theDatedSupplyVol;
   }
   // VECTORED IO
   else {
     int t = 0; 
     for (t=0; t<nElementsChanged; t++) {
       supplyVol[t] += floatVec[t];
     }
   }
   
   
   

   witSetPartSupplyVol(theWitRun, fullWitPart.c_str(), supplyVol);
   witFree(supplyVol);
   //    std::cout << "Updated supply Volume for " << pdf_PartName << "\n";
     
   recCount++;
   
   if (! aggregateSupplyInScrub) {
     if (myScrubber -> scrubMode() && outFile.good())  {
       outFile << dataLine << std::endl;
     }
   }
  
 }
 
 delete [] floatVec;
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();


 if (! aggregateSupplyInScrub) {
   outFile.close();
 }

 
 // if no data in input file, then error
 if (recCount == 0)
   (*theErrFacil)("DatalessMandatoryInputFile",MclArgList() << "supply file");

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}





// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processSupplyFile61(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{


 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();   

 long lineNo = 0;
 std::string dataLine;

 std::ifstream inFile;
 std::ofstream outFile;
 bool aggregateSupplyInScrub = setOfParameters.getBool("aggregateSupplyInScrub");
 if (! aggregateSupplyInScrub) {
   this->openInputFileAndScrubOutputFile(fileName, "Dated Supply", inFile, outFile, myScrubber, theErrFacil);
 }
 else {
   inFile.open(fileName.c_str());
   if (! inFile)  {
     (*theErrFacil)("SevereCannotOpenFile",MclArgList() << "Dated Supply" << fileName << "");
   }
 }

 std::string pdf_PartName;
 int supplyPeriod;
 float * supplyVol;
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 float supplyVal;

 while(inFile.peek() != EOF) {
    std::string pdf;
    std::string mfgPartName;
    int result = 0;

    std::getline( inFile, dataLine );
    lineNo++;

    dataLine = sce::strip( dataLine );
    if ((dataLine[0] == '*') || dataLine.empty()){
	commentCount++;
	continue;
    }
    SCETokenizer next(dataLine);

    // check if the line contains any illegal characters
    if ( flatFileManager.containsIllegalCharacters( dataLine )) {
      (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
    }

    
    // get mfgPartName, pdf
    mfgPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);
    pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);
    pdf_PartName = mpHelper.pdfPartname(mfgPartName, pdf);
    
    if (! mpHelper.isPartValid(theWitRun, mfgPartName, pdf, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE)) {
      badCount++;
      continue;
    }
    // FINISH_ME_61: Clean this up using real messages !!
    
    // get supplyVal
    result = flatFileManager.nextFloat(supplyVal, next, fileName, dataLine, lineNo, true, -FLT_MAX);

    if (result < 0) {
      std::cerr << "\nbad supply float\n"  << lineNo << ":" << dataLine << std::endl;
      std::cerr << "ignoring the record\n" << std::endl;

      badCount++;
      continue;
    }
      
    // Now read the date
    std::string token;

    //  if the line is blank then FUTURE is assumed.
    if ((token = next(" \",\n")).empty()) {
      std::cerr << "\nDate is missing from dated supply record\n"  << lineNo << ":" << dataLine << std::endl;
      std::cerr << "putting it in week 1\n" << std::endl;
      badCount++;
      continue;
    }
      
    LgFrDatetime supplyDate(flatFileManager.parseDateString(token, theCal));
    if (! supplyDate.isValid())  {
      (*theErrFacil)("InvalidDate",MclArgList() 
			 << "supply date" 
			 << fileName 
			 << (int) lineNo 
			 << dataLine);
      badCount++;
      continue;
    }
    
    // come here now  


    // Map supplyDate to supplyPeriod
    // rule: reject if supplyPeriod < 0
    // rule: reject if supplyPeriod >= nPeriods;
    supplyPeriod = theCal.period(supplyDate);
    if (supplyPeriod >= nPeriods)  {
      (*theErrFacil)("DatedRecordBeyondHorizonEndIgnore",MclArgList() 
		     << fileName << (int)lineNo << dataLine);
      badCount++;
      continue;
    }

    // earlySupplyRule tells whether you include supply that is dated earlier than first period start
    bool earlySupplyRule = setOfParameters.getBool("earlySupplyRule");
    
    if (supplyPeriod < 0)  {
      if (earlySupplyRule) {
	  supplyPeriod = 0;
	  (*theErrFacil)("DatedRecordBeforeHorizonStartAccept",MclArgList() 
			 << fileName << (int)lineNo << dataLine);
	}

      else {
	(*theErrFacil)("DatedRecordBeforeHorizonStartIgnore",MclArgList() 
		       << fileName << (int)lineNo << dataLine);
	badCount++;
	continue;
      }
    }

    witGetPartSupplyVol(theWitRun, pdf_PartName.c_str(), &supplyVol);
    supplyVol[supplyPeriod] += supplyVal;
    witSetPartSupplyVol(theWitRun, pdf_PartName.c_str(), supplyVol);

    witFree(supplyVol);

    recCount++;

    if (! aggregateSupplyInScrub) {
      if (myScrubber -> scrubMode() && outFile.good())  {
	outFile << dataLine << std::endl;
      }
    }
          
 }

 std::cout << recCount << " records processed." << std::endl;
 inFile.close();

 if (! aggregateSupplyInScrub) {
   outFile.close();
 }
 

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}


// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processCapacitySupplyFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  int nElementsChanged;
  std::string pdf_PartName;
  float * supplyVol;
  long lineNo = 0;
  std::string dataLine;

  std::ifstream inFile;
  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  
  this->openInputFileAndScrubOutputFile(fileName,
					"Capacity Supply",
					inFile,
					outFile,
					myScrubber,
					theErrFacil);
  
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 float * floatVec = new float[nPeriods];

 while(inFile.peek() != EOF) {
    nElementsChanged = flatFileManager.readCsvPartFloatVector(
	theWitRun,        // WIT environment    
	nPeriods,         // Number of periods  
	pdf_PartName,     // legal pdf_PartName to be returned
	floatVec,         // floatVector to be returned
	inFile,           // An open stream
	fileName,         // for messaging
        dataLine,
	lineNo,           // for messaging
	mpHelper,         // The helper
	OPTIONAL_WITH_MESSAGE,  // Use one of MANDATORY, PURELY_OPTIONAL
	                        // or OPTIONAL_WITH_MESSAGE
	0.0,              // data lower limits
	FLT_MAX);         // data upper limits


    if (nElementsChanged == 0 || nElementsChanged == -1) {
       badCount++;
       continue;
    }
    else if (nElementsChanged == -2) {
       commentCount++;
       continue;
    }
    
    witGetPartSupplyVol(theWitRun, pdf_PartName.c_str(), &supplyVol);

    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nElementsChanged; t++) {
       supplyVol[t] = floatVec[t];
       }
    witSetPartSupplyVol(theWitRun, pdf_PartName.c_str(), supplyVol);
    witFree(supplyVol);
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
  }

 delete [] floatVec;
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}



// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processCapacitySupplyFile61(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,    
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{
  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
  LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();   

  bool useDailyCapAvailMethod = setOfParameters.getBool("useDailyCapAvailMethod");
  
  int nElementsChanged;
  std::string pdf_PartName;
  float * supplyVol;
  
  float capAvailQty;
  long lineNo = 0;
  std::string dataLine;
  int result;
  
  std::ifstream inFile;
  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  
  this->openInputFileAndScrubOutputFile(fileName, "Capacity Supply", inFile, outFile, myScrubber, theErrFacil);
  
  std::cout << "Processing " << fileName << ":\t" << std::flush;
  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;
  char dataDelim[] = " \",\n"; 
  
  float * floatVec = new float[nPeriods];
  
  while(inFile.peek() != EOF) {
    if (useDailyCapAvailMethod)    {    
      //   --- dailyRate  ----
      //      for (t=early, t<=Period, tt++ )
      //          numDays[t] = number of calendar working days for this period, (note, be smart about the interval start and end dates
      //                       that don't start exactly on period start date!!!
      //          supplyVol[t] += capAvailQty * (numDays[t]/workingDays[t])
      //      We use += so that if a DGR chnages mid-period, we do the math right  
      
      std::getline( inFile, dataLine );
      lineNo++;
      dataLine = sce::strip( dataLine );
      if ((dataLine[0] == '*') || dataLine.empty())
	{ commentCount++; continue; }
      
      
      SCETokenizer next(dataLine);
      if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	(*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
	badCount++; continue;
      }
      
      // get the partname, pdf, and pdf_partname
      std::string mfgPartName  = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);
      std::string pdf          = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);
      std::string pdf_PartName = mpHelper.pdfPartname(mfgPartName, pdf);
      if (! mpHelper.isPartValid(theWitRun, mfgPartName, pdf, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
	{badCount++; continue; }     
      
      // get capAvailQty
      result = flatFileManager.nextFloat(capAvailQty, next, fileName, dataLine, lineNo, true, 0.0);
      if (result == -1)
	{badCount++; continue; }     
      
      std::string token; 
      if ((token = next(dataDelim)).empty())   {
	(*theErrFacil)("InvalidDate",MclArgList() << "start date" 
		       << fileName << (int) lineNo << dataLine);
	badCount++; continue; 
      }     
      
      
      LgFrDatetime startDate(flatFileManager.parseDateString(token, theCal));
      if (! startDate.isValid())  {
	(*theErrFacil)("InvalidDate",MclArgList() << "start date" 
		       << fileName << (int) lineNo << dataLine);
	badCount++; continue; 
      }     
      //      int startPeriod = theCal.earlyEffectivePeriod(startDate);
      
      if ((token = next(dataDelim)).empty())   {
	(*theErrFacil)("InvalidDate",MclArgList() << "late effectivity date" << fileName 
		       << (int) lineNo << dataLine);
	badCount++; continue; 
      }     
      
      LgFrDatetime endDate(flatFileManager.parseDateString(token, theCal));
      if (! endDate.isValid())  {
	(*theErrFacil)("InvalidDate",MclArgList() << "late effectivity date" << fileName 
		       << (int) lineNo << dataLine);
	badCount++; continue; 
      }
      //      int endPeriod = theCal.lateEffectivePeriod(endDate);
      
      /*
      if (startDate > endDate) {
	(*theErrFacil)("BadDate",MclArgList() << fileName << (int) lineNo << dataLine);
	badCount++; continue; 
      }
      
      if (startPeriod > endPeriod) {
	(*theErrFacil)("CollapsedEffectiveInterval",MclArgList() << fileName << (int) lineNo << dataLine);
	badCount++; continue; 
      }
      */

      
      witGetPartSupplyVol(theWitRun, pdf_PartName.c_str(), &supplyVol);      
      int t;
      //      for (t=startPeriod; t<=endPeriod; t++) {
      for (t=0; t<nPeriods; t++) {
	float numWorkingDaysInPeriodOverInterval = theCal.numWorkingDaysInPeriodOverInterval(t, startDate, endDate);
	supplyVol[t] += capAvailQty * numWorkingDaysInPeriodOverInterval;
      }
      witSetPartSupplyVol(theWitRun, pdf_PartName.c_str(), supplyVol);
      witFree(supplyVol);
      recCount++;

      if (myScrubber -> scrubMode() && outFile.good()) {
	outFile << dataLine << std::endl;
      }
      
    }
    
    else {
      //   --- periodRate ----
      //        this is meant to be "compatible" to SCE5.   
      //        The capAvailQty is assumed to be the supplyVol for the period, 
      //          for t: earlyPeriod <= t <= latePeriod,  supplyVol[t] = capAvailQty
      int startPeriod, endPeriod;
      int result = flatFileManager.readCsvPartFloatDatedRecord(theWitRun, nPeriods, pdf_PartName, theCal, capAvailQty, 
							       startPeriod, endPeriod, inFile, fileName, dataLine, lineNo,mpHelper);
      if (result == 1) { badCount++; continue; }
      if (result == -2) { commentCount++; continue; }
      nElementsChanged = 1;
      
      witGetPartSupplyVol(theWitRun, pdf_PartName.c_str(), &supplyVol);
      
      int t = 0; 
      for (t=startPeriod; t<endPeriod; t++) {
	supplyVol[t] = capAvailQty;
      }
      witSetPartSupplyVol(theWitRun, pdf_PartName.c_str(), supplyVol);
      witFree(supplyVol);
      recCount++;
      
      if (myScrubber -> scrubMode() && outFile.good())	{
	outFile << dataLine << std::endl;
      }
    }
    
  }    
  std::cout << recCount << " records processed." << std::endl;
  inFile.close();
  outFile.close();
  flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

  
// ---------------------------------------------------------------
void 
LgFrInitialProblemForSce::processPartsConditionFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,    
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{
  std::string mfgPartName;
  std::string pdf_PartName;
  std::string pdf;

  std::ifstream inFile;

  std::string dataLine;
  
  LgFrSceFlatFileMgr & flatFileManager =
    this->sceScenario()->flatFileMgr();   
  
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  int result;
  int unConstrainedStartPeriod;


  LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();   
#ifdef FORCE_BOGONS
 bool forcePcfBogons = setOfParameters.getBool("forcePcfBogons");
#endif


  inFile.open(fileName.c_str());
  if (! inFile)  {
    (*theErrFacil)("SevereCannotOpenFile",MclArgList() 
		   << "Parts Condition" 
		   << fileName 
		   << "");
  }

  std::cout << "Processing " << fileName << ":\t" << std::flush;
  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;

  long lineNo = 0;
  while(inFile.peek() != EOF) {

    std::getline( inFile, dataLine );
    lineNo++;

    //  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
      commentCount++;
      continue;
    }

    // check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() 
			 << fileName 
			 << (int)lineNo 
			 << dataLine);
        }


    SCETokenizer next(dataLine);

    // get the partname
    mfgPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, PURELY_OPTIONAL);
    if (result == -1) {
      badCount++;
      continue;
    }
    // get the pdf
    pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);

    // get the constrainedHorizon End date.  (if line is blank then FUTURE is assumed.)
    std::string token;
    if ((token = next(" \",\n")).empty()) {
      unConstrainedStartPeriod = nPeriods;
    }
    else {
      LgFrDatetime startDate(flatFileManager.parseDateString(token, theCal));
      if (! startDate.isValid())  {
	(*theErrFacil)("InvalidDataWarning",MclArgList() 
		       << "constraint Horizon End Date specified.\n" 
		       << fileName 
		       << (int)lineNo 
		       << dataLine 
		       << "The date: FUTURE will be assumed.");
        unConstrainedStartPeriod = nPeriods; 
      }
      unConstrainedStartPeriod = theCal.earlyEffectivePeriod(startDate);
    }

    // test to see if the part exists.
    if (! mpHelper.isPartValid(theWitRun, mfgPartName, pdf, fileName, 
                               dataLine, lineNo, PURELY_OPTIONAL))   {
      (*theErrFacil)("InvalidPCFPart",MclArgList() 
		     << mfgPartName 
		     << pdf 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
      badCount++;
      continue;
    }
          

    // OK, we have a valid part anyway.
    pdf_PartName = mpHelper.pdfPartname(mfgPartName, pdf);

    // check to see if part is already marked as a PCF part
    if ( mpHelper.isPartPcf(theWitRun, pdf_PartName)) {
      (*theErrFacil)("DuplicatePCFPart",MclArgList() 
		     << mfgPartName 
		     << pdf 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
      badCount++;
      continue;
    }


    // Now check to see if this part can be a PCF part:
    // Conditions:
    // Case 1: default operation has children ==>
    //      Reject the record with Warning.  Do not mark the part as PCF.
    // Case 2)  default operation has no children
    //     a) if no other operations ==> accept
    //     b) if other (non-default) operation exists ==> give warning and reject.

    // assume its OK until proven otherwise ...
    bool didWeRejectTheRecord = false;


    // First find the default operation:
    witBoolean exists;
    witGetOperationExists(theWitRun, pdf_PartName.c_str(), &exists);
    assert(exists == WitTRUE);
    int nBom;
    witGetOperationNBomEntries(theWitRun, pdf_PartName.c_str(), &nBom);
    // if the default operation has  children, then we  reject it
    if (nBom > 0) {
#ifdef FORCE_BOGONS
      if (! forcePcfBogons) {
#endif
	(*theErrFacil)("ExtraPCFPart",MclArgList() 
		       << "it has BOM child components" 
		       << mfgPartName 
		       << pdf 
		       << fileName 
		       << (int)lineNo 
		       << dataLine);
	didWeRejectTheRecord = true;
	// skip to next part. No need to keep checking
	badCount++;
#ifdef FORCE_BOGONS
      }
      else {
	(*theErrFacil)("ExtraPCFPartBogon",MclArgList() 
		       << "it has BOM child components" 
		       << mfgPartName 
		       << pdf 
		       << fileName 
		       << (int)lineNo 
		       << dataLine);


      }
#endif

#ifdef MARK_BOGONS
      // mark the part as a bogon
      LgFrScePartAppData * appDataPtr;
      witGetPartAppData(theWitRun, pdf_PartName.c_str(), (void **) &appDataPtr);
      if (appDataPtr != 0) {
	appDataPtr->isPartBogon(1);
	appDataPtr->unConstrainedStartPeriod(unConstrainedStartPeriod);
      }
      else {
	appDataPtr = new LgFrScePartAppData(0, unConstrainedStartPeriod, nPeriods);
	appDataPtr->isPartBogon(1);
      }
      witSetPartAppData(theWitRun, pdf_PartName.c_str(), appDataPtr);
#endif

#ifndef FORCE_BOGONS
      continue;
#endif
    }

    // FINISH_ME: for forcing_bogons we need to set yieldRate to zero on 
    //            ALL operations which generate a bogon.   I think it's ok as it 
    //            now for ATS calculation

    // At this time, the following is true:
    //    - The default operation has no BOM entries.
    //
    // What remains is to check for non-default operations.  If we find
    // any, then we report a message.  No change to the structure is
    // necessary.
        
    int npBop;
    witGetPartNProducingBopEntries(theWitRun, pdf_PartName.c_str(), &npBop);
    int p = 0; // Pulled out of the for below by RW2STL
    for (p=0; p<npBop; p++) {
      char * opName;
      int o;
      witGetPartProducingBopEntry(theWitRun, pdf_PartName.c_str(), p, &opName, &o);
          
      // If the operation is not the default ...
      if (pdf_PartName.compare(opName)) {

	// first check to see if the producing operation is an Alternate Operation.
	// this case does not override pcf-ness
	std::string primePart;
	std::string primePdf;
	std::string altPart;
	std::string altPdf;
	// and, if the operation is NOT an alternate ...
	if (! mpHelper.isOperationSpecialAlternate(theWitRun, opName,  primePart, primePdf, altPart, altPdf)) {
	  (*theErrFacil)("ExtraPCFPart",MclArgList() 
			 << "it has a producing operation" 
			 << mfgPartName 
			 << pdf 
			 << fileName 
			 << (int)lineNo 
			 << dataLine);
	  didWeRejectTheRecord = true;
	  witFree(opName);
	  // exit out of the loop, again, becuase you don't want to generate
	  // multiple error messages per record
	  break;
	}
      }
      witFree(opName);
    }


        
    // skip to next part if you rejected
    if (didWeRejectTheRecord) {
#ifdef MARK_BOGONS
      // mark the part as a bogon
      LgFrScePartAppData * appDataPtr;
      witGetPartAppData(theWitRun, pdf_PartName.c_str(), (void **) &appDataPtr);
      if (appDataPtr != 0) {
	appDataPtr->isPartBogon(1);
	appDataPtr->unConstrainedStartPeriod(unConstrainedStartPeriod);
      }
      else {
	appDataPtr = new LgFrScePartAppData(0, unConstrainedStartPeriod, nPeriods);
	appDataPtr->isPartBogon(1);
      }
      witSetPartAppData(theWitRun, pdf_PartName.c_str(), appDataPtr);
#endif
      badCount++;
      continue;
    }

    // OK at this point we have a PCF record to process.
    // set the yield to 0.0 within the constrained horizon
    // AND mark it via appData
    if (! didWeRejectTheRecord) {

      // first shut down the operation via zero yield
      float * yieldRate;
      witGetOperationYieldRate(theWitRun, pdf_PartName.c_str(), &yieldRate);              
      int t = 0; 
      for (t=0; t<unConstrainedStartPeriod; t++)
        yieldRate[t] = 0;
      witSetOperationYieldRate(theWitRun, pdf_PartName.c_str(), yieldRate);
      witFree(yieldRate);
      
      // set isPartPcf = 1
      LgFrScePartAppData * appDataPtr;
      witGetPartAppData(theWitRun, pdf_PartName.c_str(), (void **) &appDataPtr);
      if (appDataPtr != 0) {
	appDataPtr->isPartPcf(1);
	appDataPtr->unConstrainedStartPeriod(unConstrainedStartPeriod);
      }
      else {
	appDataPtr = new LgFrScePartAppData(1, unConstrainedStartPeriod, nPeriods);
      }
      witSetPartAppData(theWitRun, pdf_PartName.c_str(), appDataPtr);
      recCount++;
     }
  }
  std::cout << recCount << " records processed." << std::endl;
  inFile.close();

  // if there is no data in input file, then error
  if (recCount == 0)
    (*theErrFacil)("DatalessMandatoryInputFile",MclArgList() 
		   << "parts condition file");

  flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}


// ---------------------------------------------------------------
void 
LgFrInitialProblemForSce::scrubPartsConditionFile(
    
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,    
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{
  std::string mfgPartName;
  std::string pdf_PartName;
  std::string pdf;

  std::ifstream inFile;

  std::string dataLine;
  
  LgFrSceFlatFileMgr & flatFileManager =
    this->sceScenario()->flatFileMgr();   
  
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  int result;
  int unConstrainedStartPeriod;



  inFile.open(fileName.c_str());
  if (! inFile)  {
    (*theErrFacil)("SevereCannotOpenFile",MclArgList() 
		   << "Parts Condition" 
		   << fileName 
		   << "");
  }

  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  if (myScrubber -> scrubMode())       {
      if (sce::find(fileName, "/") != std::string::npos) {
      (*theErrFacil)("QualifiedInputFileName",MclArgList() 
		     << fileName);
    }
    std::string file = myScrubber -> scrubDir() + fileName;
    outFile.open(file.c_str(), std::ofstream::out);
    if (!outFile.good()) {
      (*theErrFacil)("ScrubOutPutFileNameBad",MclArgList() 
		     << file);
    }
  }

  
  long lineNo = 0;
  while(inFile.peek() != EOF) {

    std::getline( inFile, dataLine );
    lineNo++;

    //  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
      continue;
    }

    SCETokenizer next(dataLine);

    // get the partname
    mfgPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, PURELY_OPTIONAL);
    if (result == -1) {
      continue;
    }

    pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);

    // pick up the constrainedHorizon End date (even though we're not really going
    // to do much with it.
    std::string token;

    //  if the line is blank then FUTURE is assumed.
    if ((token = next(" \",\n")).empty()) {
      unConstrainedStartPeriod = nPeriods;
    }
    else {
      LgFrDatetime startDate(flatFileManager.parseDateString(token, theCal));
      if (! startDate.isValid())  {
        unConstrainedStartPeriod = nPeriods; 
      }
      unConstrainedStartPeriod = theCal.earlyEffectivePeriod(startDate);
    }

    // test to see if the part exists.
    if (! mpHelper.isPartValid(theWitRun, mfgPartName, pdf, fileName, 
                               dataLine, lineNo, PURELY_OPTIONAL))   {
      continue;
    }
          

    // OK, we have a valid part anyway.
    pdf_PartName = mpHelper.pdfPartname(mfgPartName, pdf);

    
    // check to see if part is marked as a PCF part
    if ( mpHelper.isPartPcf(theWitRun, pdf_PartName)) {
      if (outFile.good())  	{
	  outFile << dataLine << std::endl;
	}
    }

  }
  inFile.close();
  outFile.close();

}




// ---------------------------------------------------------------
void 
LgFrInitialProblemForSce::printImpactPeriodFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,    
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

  int nOps;
  char ** opList;

  LgFrSceFlatFileMgr & flatFileManager =
    this->sceScenario()->flatFileMgr();   
  
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();


  std::ofstream outFile(fileName.c_str());
  if (outFile.fail()) {
    (*theErrFacil)("CannotOpenFileForWrite",MclArgList() 
		   << "ImpactPeriod" 
		   << fileName);
  }


  witGetOperations(theWitRun, &nOps, &opList);
  int nBom;
  int op = 0; 
  for (op=0; op<nOps; op++ ) {
    std::string parentPdf, parentOpName, childPartName, childPdf;
    int isItNormalOperation = 0;
    if (mpHelper.isOperationNormal(theWitRun, opList[op], parentPdf, parentOpName)) 
      isItNormalOperation = 1;
    else if (! mpHelper.isOperationSpecialInterplant(
                       theWitRun, opList[op],
                       parentOpName, parentPdf, childPartName, childPdf)) {
      witFree(opList[op]);
      continue;
    }
	
    witGetOperationNBomEntries(theWitRun, opList[op], &nBom);
    int b=0;
    for (b=0; b<nBom; b++) {
      if (isItNormalOperation) {
	char * child;
	witGetBomEntryConsumedPart(theWitRun, opList[op], b, &child);
	childPartName = mpHelper.partname(child);
	childPdf = parentPdf;
	witFree(child);
      }
      
      outFile << "\"" 
	      << parentOpName << "\",\""
	      << parentPdf << "\",\""
	      << childPartName << "\",\""
	      << childPdf << "\",";

      if (isItNormalOperation) 
	outFile << "\"F\"";
      else 
	outFile << "\"A\"";

      int * impactPeriod;
      witGetBomEntryImpactPeriod(theWitRun, opList[op], b, &impactPeriod);
      int t = 0; 
      for (t=0; t<nPeriods; t++) {
	outFile << "," << impactPeriod[t];
      }
      outFile << "\n";
      witFree(impactPeriod);
    }
      
    witFree(opList[op]);
  }

  witFree(opList);

  outFile.close();
}


// ---------------------------------------------------------------
void 
LgFrInitialProblemForSce::scrubSupplyAvailabilityFile(
    
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,    
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{
  std::string mfgPartName;
  std::string pdf_PartName;
  std::string pdf;


  int nParts;
  char ** partList;

  LgFrSceFlatFileMgr & flatFileManager =
    this->sceScenario()->flatFileMgr();   
  
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();


  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  if (myScrubber -> scrubMode())       {
    if (sce::find(fileName, "/") != std::string::npos) {
      (*theErrFacil)("QualifiedInputFileName",MclArgList() 
		     << fileName);
    }

    std::string file = myScrubber -> scrubDir() + fileName;
    outFile.open(file.c_str(), std::ofstream::out);
    if (!outFile.good()) {
      (*theErrFacil)("ScrubOutPutFileNameBad",MclArgList() 
		     << file);
    }
  }

  witGetParts(theWitRun, &nParts, &partList);

  // Loop once for each part.  
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++ ) {
    float * supplyVol;
    if ( mpHelper.isPartNormal(theWitRun, partList[i])) {
       witGetPartSupplyVol(theWitRun, partList[i], &supplyVol);
#if 1
       if (mpHelper.doesPartHaveNegativeDemands(theWitRun, partList[i])) {
	 LgFrTimeVecFloat negDemVol = mpHelper.negDemVol(theWitRun, partList[i]);
	 int tt=0;
	 for (tt=0; tt<nPeriods; tt++) 
	   supplyVol[tt] -= negDemVol[tt];
       }
#endif
       std::string partname = mpHelper.partname(partList[i]);
       std::string pdf = mpHelper.pdf(partList[i]);
       outFile << "\"" 
               << partname << "\",\"" 
               << pdf << "\"";
       flatFileManager.writeCsvFloatVector(outFile, nPeriods, supplyVol, 0, 15);
       witFree(supplyVol);
       }
    witFree(partList[i]);
   }

  witFree(partList);

  // Now undo any negative demand vol that was added to supply.dat

  outFile.close();

}






// ---------------------------------------------------------------

void 
LgFrInitialProblemForSce::scrubDemandVolumeFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,    
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

  std::string plannerPart;
  std::string geo;
  std::string geoPlannerDemandPartName;

  std::string demandName;
  std::ifstream inFile;
  float * demandVol;
  long lineNo = 0;
  std::string dataLine;
  int i, j;
  int result; 

  int nParts;
  char ** partList;
  int nDemands;
  char ** dList;

  LgFrSceDemandAppData * demandAppDataPtr;

  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  witGetParts(theWitRun, &nParts, &partList);

  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  if (myScrubber -> scrubMode())       {
    if (sce::find(fileName, "/") != std::string::npos) {
      (*theErrFacil)("QualifiedInputFileName",MclArgList() 
		     << fileName);
    }
    
    std::string file = myScrubber -> scrubDir() + fileName;
    outFile.open(file.c_str(), std::ofstream::out);
    if (!outFile.good()) {
      (*theErrFacil)("ScrubOutPutFileNameBad",MclArgList() 
		     << file);
    }
  }
  
  inFile.open(fileName.c_str());
  if (! inFile)  {
    (*theErrFacil)("SevereCannotOpenFile",MclArgList() 
		   << "demandVolume (while scrubbing) " 
		   << fileName 
		   << "");
  }
  
  
  
  // ------------
  // Before you do anything, reset all the hasDemandVolBeenSet
  // appData elements to zero
  // Loop once for each part.  
  for (i=0; i<nParts; i++ ) {
    if (mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun, partList[i],
						   plannerPart, geo)) {
      witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);
      for (j=0; j<nDemands; j++)  {
	// get the  demand app data for this demand.  get the hasDemandVolBeenSet
	witGetDemandAppData(theWitRun,
			    partList[i], 
			    dList[j],
			    (void **) &demandAppDataPtr);
	assert(demandAppDataPtr != 0);
	demandAppDataPtr->hasDemandVolBeenSet(0);
        witFree(dList[j]);
      }
      witFree(dList);
    }
  }    


  float * floatVec = new float[nPeriods];
 
  // Now read through the unscrubbed input file and use it to 
  // maintain the sequential order of the scrubbed file.
  while(inFile.peek() != EOF) {
   std::getline( inFile, dataLine );
   lineNo ++;
   dataLine = sce::strip( dataLine );
   if ((dataLine[0] == '*') || dataLine.empty())
     continue;
   SCETokenizer next(dataLine);
   plannerPart = flatFileManager.nextQuotedString(next, result, fileName, 
						  dataLine, lineNo, 
						  PURELY_OPTIONAL);
   if (result == -1) 
     continue;
   
   geo = flatFileManager.nextQuotedString(next, result, fileName, 
					  dataLine, lineNo, 
					  PURELY_OPTIONAL);
   if (result == -1)     
     continue;
   
   geoPlannerDemandPartName = mpHelper.geoPlannerDemandPartName(plannerPart, geo);

   // Check to see if the GPD is valid
   if (! mpHelper.isPartValid(theWitRun, geoPlannerDemandPartName, fileName, 
			      dataLine, lineNo, PURELY_OPTIONAL))  
    continue;
  
  if (! doesDemandExist(theWitRun, geoPlannerDemandPartName, geo)) 
    continue;

  // if you get here, then its a valid demandVol rec.
  // now check to see if one has already been added

  // get the  demand app data for this demand.  get the hasDemandVolBeenSet
  witGetDemandAppData(theWitRun,
		      geoPlannerDemandPartName.c_str(),
		      geo.c_str(),
		      (void **) &demandAppDataPtr);
  assert(demandAppDataPtr != 0);
  int hasDemandVolBeenSet = demandAppDataPtr->hasDemandVolBeenSet();

  // if demandVol was already set, then for scrubbing purposes, 
  // the demandVol record has already been printed
  if (hasDemandVolBeenSet) 
    continue;

  // otherwise, this is the first occurence of the demandVol key and
  // go and print out the total demandVol and mark it as "hasBeenSet"
  witGetDemandDemandVol(theWitRun, geoPlannerDemandPartName.c_str(), 
			geo.c_str(), &demandVol);
  outFile << "\"" << plannerPart << "\",\"" << geo << "\"";
  flatFileManager.writeCsvFloatVector(outFile, nPeriods, demandVol, 0, 15);
  witFree(demandVol);


  // set the hasDemandVolBeenSet to true
  demandAppDataPtr->hasDemandVolBeenSet(1);
 }


  for (i=0; i<nParts; i++ ) 
    witFree(partList[i]);
  witFree(partList);


 delete [] floatVec;
 inFile.close();
 outFile.close();


}

// ---------------------------------------------------------------

void 
LgFrInitialProblemForSce::scrubMinDemandVolumeFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,    
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

  std::string plannerPart;
  std::string geo;
  std::string geoPlannerDemandPartName;

  std::string demandName;
  std::ifstream inFile;
  float * cshlb;
  float * csslb;
  float * cshub;
  long lineNo = 0;
  std::string dataLine;
  int i, j, t;
  int result; 

  int nParts;
  char ** partList;
  int nDemands;
  char ** dList;

  LgFrSceDemandAppData * demandAppDataPtr;

  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  witGetParts(theWitRun, &nParts, &partList);

  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  if (myScrubber -> scrubMode())       {
      if (sce::find(fileName, "/") != std::string::npos) {
      (*theErrFacil)("QualifiedInputFileName",MclArgList() 
		     << fileName);
    }
    
    std::string file = myScrubber -> scrubDir() + fileName;
    outFile.open(file.c_str(), std::ofstream::out);
    if (!outFile.good()) {
      (*theErrFacil)("ScrubOutPutFileNameBad",MclArgList() 
		     << file);
    }
  }
  
  inFile.open(fileName.c_str());
  if (! inFile)  {
    (*theErrFacil)("SevereCannotOpenFile",MclArgList() 
		   << "minDemandVolume (while scrubbing) " 
		   << fileName 
		   << "");
  }
  
  
  
  // ------------
  // Before you do anything, reset all the hasMinDemandVolBeenSet
  // appData elements to zero
  // Loop once for each part.  
  for (i=0; i<nParts; i++ ) {
    if (mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun, partList[i],
						   plannerPart, geo)) {
      witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);
      for (j=0; j<nDemands; j++)  {
	// get the demand app data for this demand.  
        // get the hasMinDemandVolBeenSet
	witGetDemandAppData(theWitRun,
			    partList[i], 
			    dList[j],
			    (void **) &demandAppDataPtr);
	assert(demandAppDataPtr != 0);
	demandAppDataPtr->hasMinDemandVolBeenSet(0);
        witFree(dList[j]);
      }
      witFree(dList);
    }
  }    


  float * minDemandVol = new float[nPeriods];
 
  // Now read through the unscrubbed input file and use it to 
  // maintain the sequential order of the scrubbed file.
  while(inFile.peek() != EOF) {
   std::getline( inFile, dataLine );
   lineNo ++;
   dataLine = sce::strip( dataLine );
   if ((dataLine[0] == '*') || dataLine.empty())
     continue;
   SCETokenizer next(dataLine);
   plannerPart = flatFileManager.nextQuotedString(next, result, fileName, 
						  dataLine, lineNo, 
						  PURELY_OPTIONAL);
   if (result == -1) 
     continue;
   
   geo = flatFileManager.nextQuotedString(next, result, fileName, 
					  dataLine, lineNo, 
					  PURELY_OPTIONAL);
   if (result == -1)     
     continue;
   
   geoPlannerDemandPartName = mpHelper.geoPlannerDemandPartName(plannerPart, geo);

   // Check to see if the GPD is valid
   if (! mpHelper.isPartValid(theWitRun, geoPlannerDemandPartName, fileName, 
			      dataLine, lineNo, PURELY_OPTIONAL))  
    continue;
  
  if (! doesDemandExist(theWitRun, geoPlannerDemandPartName, geo)) 
    continue;

  // if you get here, then its a valid minDemandVol rec.
  // now check to see if one has already been added

  // get the  demand app data for this demand.  get the hasMinDemandVolBeenSet
  witGetDemandAppData(theWitRun,
		      geoPlannerDemandPartName.c_str(),
		      geo.c_str(),
		      (void **) &demandAppDataPtr);
  assert(demandAppDataPtr != 0);
  int hasMinDemandVolBeenSet = demandAppDataPtr->hasMinDemandVolBeenSet();

  // if minDemandVol was already set, then for scrubbing purposes, 
  // the minDemandVol record has already been printed
  if (hasMinDemandVolBeenSet) 
    continue;

  // otherwise, this is the first occurence of the minDemandVol key 
  // get the cumulative lower bound, un-cumulate it and
  // go and print out the total minDemandVol and mark it as "hasBeenSet"
  witGetDemandCumShipBounds(theWitRun, 
			      geoPlannerDemandPartName.c_str(),
		          geo.c_str(),
			      &cshlb,
			      &csslb,
			      &cshub);

  // now un-cumulate csslb to get the minDemand
  minDemandVol[0] = csslb[0];
  for(t=1; t<nPeriods; t++)
     minDemandVol[t] = csslb[t] - csslb[t-1];

  witFree(cshlb);
  witFree(csslb);
  witFree(cshub);


  outFile << "\"" << plannerPart << "\",\"" << geo << "\"";
  flatFileManager.writeCsvFloatVector(outFile, nPeriods, minDemandVol, 0, 15);



  // set the hasDemandVolBeenSet to true
  demandAppDataPtr->hasMinDemandVolBeenSet(1);
 }


  for (i=0; i<nParts; i++ ) 
    witFree(partList[i]);
  witFree(partList);


 delete [] minDemandVol;
 inFile.close();
 outFile.close();


}

//-----------------------------------------------------------

void 
LgFrInitialProblemForSce::deleteUnnecessaryDemandAppData(
    WitRun * const theWitRun,
    const int nPeriods,         
    LgFrMultiPlantHelper & mpHelper)
{
  std::string plannerPart;
  std::string geo;
  std::string geoPlannerDemandPartName;

  int i, j;

  int nParts;
  char ** partList;
  int nDemands;
  char ** dList;

  LgFrSceDemandAppData * demandAppDataPtr;

  witGetParts(theWitRun, &nParts, &partList);

  // ------------
  // so we don't bog down the rollover engine ...
  // delete any dmeand appData's that have backlogYield==1.0
  for (i=0; i<nParts; i++ ) {
    if (mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun, partList[i],
						   plannerPart, geo)) {
      witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);
      for (j=0; j<nDemands; j++)  {
	// get the  demand app data for this demand.  
	witGetDemandAppData(theWitRun, partList[i], dList[j], (void **) &demandAppDataPtr);
	assert(demandAppDataPtr != 0);
	LgFrTimeVecFloat backlogYield = demandAppDataPtr->backlogYield();
	if (backlogYield.min() > 0.9999) {
	  delete demandAppDataPtr;
	  witSetDemandAppData(theWitRun, partList[i], dList[j], (void *) 0);
	}
        witFree(dList[j]);
      }
      witFree(dList);
    }
  }    

  for (i=0; i<nParts; i++ ) 
    witFree(partList[i]);
  witFree(partList);
}


//-----------------------------------------------------------

void 
LgFrInitialProblemForSce::shutOffAllLotSizing(
    WitRun * const theWitRun,
    const int nPeriods)
{

  int o;

  LgFrTimeVecFloat zeroVec(nPeriods, 0.0);
  char ** opList;
  int nOperations;

  witGetOperations(theWitRun, &nOperations, &opList);

  for (o=0; o<nOperations; o++ ) {
    witSetOperationMinLotSize(theWitRun, opList[o], zeroVec.data());
    witSetOperationIncLotSize(theWitRun, opList[o], zeroVec.data());
    witFree(opList[o]);
  }
  witFree(opList);
}

// ---------------------------------------------------------------
int 
LgFrInitialProblemForSce::scePreProcessDefaultOperations(
    WitRun * const theWitRun,
    const int nPeriods)
{

  // to allow for error messaging
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  // This routine looks through all the parts in the WIT model
  // and for any part with (1) multiple bop entries and 
  // (2) a childless default operation, it
  // "turns off" the effectivity of the default operation. 
  //
  // Why are we bothering to do this?
  // --------------------------------
  // If a part is not in the pcf file, than the default
  // operation essentially produces it from "thin air" (subject
  // to cycle time considerations--FINISH_ME).
  // If the user has specified another means of

  // producing the part (interplant, aggregation(?), co-product
  // gerenation, user-defined op...), than that production method(s)
  // should constrain the part's production...and it won't
  // unless we turn-off the default operation.
  //
  // Can't we just not add the un-needed default operations,
  // in the first place?
  // -----------------------------------------------------------
  // The optExploder needs to have the default operations around
  // (at least in the current implementation). In the case where
  // the optExplodableFlags are set to false on all the non-default
  // operations, the optExploder needs to have the default
  // operation around to be able to build as much of the part as is
  // needed.

  char ** partList;
  int nParts;
  witBoolean defOpExists;

  witGetParts ( theWitRun, &nParts, &partList );
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    int  npBops;  // number of bops producing the i-th part
    witGetPartNProducingBopEntries ( theWitRun, partList[i], & npBops );

    if ( npBops <= 1 )  // we only care about the case with multiple bops
      continue;
    
    std::string defaultOperationName (partList[i]);
    witGetOperationExists(theWitRun, defaultOperationName.c_str(), &defOpExists);
    

    //  Capacity parts (and perhaps)
    // some dummy parts (in the future) may have more than one bop and no
    // default op.
    if (! defOpExists) {
      continue;
    }
    
    int nBom;
    witGetOperationNBomEntries(theWitRun, defaultOperationName.c_str(), &nBom);
    // if the default operation is childless, shut off the default bop's effectivity
    if (nBom == 0) {
      // find bopEntryIndex for default operation
      bool didWefindBopEntryForDefaultOperation = false;
      
      int p = 0; // Pulled out of the for below by RW2STL
      for (p=0; p<npBops; p++) { // p = prodIndex
        char * opName;
        int bopEntryIndex;
        witGetPartProducingBopEntry(theWitRun, partList[i], p, &opName, &bopEntryIndex);

        if (defaultOperationName == opName) {
          // Note: at this time there should only be one
          // bop entry for the default part
          assert(didWefindBopEntryForDefaultOperation == false);
          didWefindBopEntryForDefaultOperation = true;
          witSetBopEntryEarliestPeriod(theWitRun, defaultOperationName.c_str(), bopEntryIndex, nPeriods ); 
        }
        witFree(opName);
      }
      // make sure you found a bop entry for default to shut off
      if ( didWefindBopEntryForDefaultOperation != true ){       
	(*theErrFacil)("InternalError6");
      }
    }
  }

  for (i=0; i<nParts; i++) 
    witFree(partList[i]);
  witFree(partList);

  
  return 0;
}



// ----------------------------------------------------------------
// FINISH_ME: Create new implementation for dated demand file using local hash dictionary 
//            Invoking wit get/set for each element might be ghastly.
void 
LgFrInitialProblemForSce::processDemandVolumeFile(
    WitRun * const theWitRun,   
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 int nElementsChanged;
 std::string pdf_PartName;
 std::string demandName;
 std::ifstream inFile;
 float * demandVol;
 long lineNo = 0;
 std::string dataLine;
 
 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 inFile.open(fileName.c_str());
 if (! inFile)  {
   (*theErrFacil)("SevereCannotOpenFile",MclArgList() 
		  << "demandVolume" 
		  << fileName 
		  << "");
 }


 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 float * floatVec = new float[nPeriods];

    while(inFile.peek() != EOF) {
       nElementsChanged = readCsvDemandFloatVector(
	   theWitRun,      // WIT environment    
	   nPeriods,       // Number of periods  
	   pdf_PartName,   // pdf_PartName to be returned
	   demandName,     // demandName to be returned
	   floatVec,       // floatVector to be returned
	   inFile,         // An open input stream
	   fileName,       // for messaging
           dataLine,       // a record of file
	   lineNo,         // current line number, for messaging
	   mpHelper,       // The helper object
	   true,           // Are all elements mandatory?
	   0.0,            // data lower limits
	   FLT_MAX,        // data upper limits
	   true);          // Add demand stream to WIT if not there

       if (nElementsChanged == 0 || nElementsChanged == -1) {
       badCount++;
       continue;
    }
    else if (nElementsChanged == -2) {
       commentCount++;
       continue;
    }
    
    witGetDemandDemandVol(theWitRun, pdf_PartName.c_str(), 
			  demandName.c_str(), &demandVol);

    // get the  demand app data for this demand.  get the hasDemandVolBeenSet
    LgFrSceDemandAppData * demandAppDataPtr;
    witGetDemandAppData(theWitRun,
			pdf_PartName.c_str(), 
			demandName.c_str(),
			(void **) &demandAppDataPtr);
    assert(demandAppDataPtr != 0);
    int hasDemandVolBeenSet = demandAppDataPtr->hasDemandVolBeenSet();
    

    // note: duplicate demandVol records are incremental!!
    // print a warning when detected
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nElementsChanged; t++) {
      demandVol[t] += floatVec[t];
    }


    if (hasDemandVolBeenSet) {
      std::string pnm = mpHelper.partname(pdf_PartName);
      std::string pdfff = mpHelper.pdf(pdf_PartName);
      (*theErrFacil)("MultipleDemandVolRecs",MclArgList() 
		     << fileName 
		     << (int) lineNo 
		     << pnm 
		     << pdfff);
    }

    witSetDemandDemandVol(theWitRun, pdf_PartName.c_str(), demandName.c_str(), demandVol);
    // set the hasDemandVolBeenSet to true
    demandAppDataPtr->hasDemandVolBeenSet(1);

    witFree(demandVol);
    recCount++;
 }

 delete [] floatVec;
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();

  // if no data in input file, then error
 if (recCount == 0)
   (*theErrFacil)("DatalessMandatoryInputFile",MclArgList() 
		  << "demand volume file");

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}



// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processIndependentDemandFile(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 int nElementsChanged;
 std::string mfgPart;
 std::string pdf;
 std::string fullWitPart;
 std::string demandName;
 std::ifstream inFile;
 long lineNo = 0;
 std::string dataLine;
 int t = 0; 
 
 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
 LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();   

 int defaultIndependentDemandPriority = setOfParameters.getInt("defaultIndependentDemandPriority");
 LgFrTimeVecFloat tvDefaultPriority(nPeriods, (float) (1000000000 - defaultIndependentDemandPriority));

 /*
 // Priority Manager uses default priority of 0 !!
 std::string priorityRulesFile = setOfParameters.getString("priorityRulesFile");
 if (! priorityRulesFile.empty())  {
   LgFrTimeVecFloat zeroVec(nPeriods, (float) (-1));
   tvDefaultPriority = zeroVec;
 }
 */



 int defaultShipLateLimit = heurModelHelper_->defaultShipLateLimit();

 // DATED_DEMANDS
 bool useDatedDemands = setOfParameters.getBool("useDatedDemands");
 float theDatedDemandVol = 0.0f;
 int thePeriod = 0;
 std::string requestDate;
 
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName, "Independent Demand Volume",
				       inFile, outFile, myScrubber, theErrFacil);

 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 float * floatVec = new float[nPeriods];
 
 while(inFile.peek() != EOF) {
   int result = 0;
 
   // Read next line of file
   std::getline( inFile, dataLine );
   lineNo ++;

   dataLine = sce::strip( dataLine );
   
   //  skip blanks or comments '*'
   if ((dataLine[0] == '*') || dataLine.empty()) {
     commentCount++;
     continue;
   }
   
   // construct a tokenizer
   SCETokenizer next(dataLine);
   
   if ( flatFileManager.containsIllegalCharacters( dataLine )) {
     (*theErrFacil)("IllegalCharacter",MclArgList() 
		    << fileName 
		    << (int)lineNo 
		    << dataLine);
   }
  
   // get the partname
   mfgPart = flatFileManager.nextQuotedString(next, result, fileName, 
					      dataLine, lineNo, 
					      OPTIONAL_WITH_MESSAGE);

   // get the pdf
   pdf = flatFileManager.nextQuotedString(next, result, fileName, 
					  dataLine, lineNo, MANDATORY);

#ifdef MULTI_ATTRIBUTE_DEMAND_CLASS
   demandName = flatFileManager.nextQuotedDemandString(next, result, fileName, 
						       dataLine, lineNo, MANDATORY);




#else 

   // get the demandName
   demandName = flatFileManager.nextQuotedString(next, result, fileName, 
						 dataLine, lineNo, MANDATORY);
#endif

   // Get the witPartname
   fullWitPart = mpHelper.pdfPartname(mfgPart, pdf);



  int readCsvMessage = OPTIONAL_WITH_MESSAGE;

  if (! mpHelper.isPartValid( theWitRun, fullWitPart, fileName, 
                              dataLine, lineNo, PURELY_OPTIONAL))  {
    (*theErrFacil)("IndDemandedPartNotExist",MclArgList() 
		   << mfgPart 
		   << pdf
		   << demandName
		   << fileName 
		   << (int)lineNo 
		   << dataLine);
    continue;
  }

    

  if (! doesDemandExist(theWitRun, fullWitPart, demandName)) {
    // regardless of engine, load the priorities and shipLateLimit
    // get the default priority
    witAddDemand( theWitRun, fullWitPart.c_str(), demandName.c_str() );
    // create a demand app data for this demand.  Set the hasDemandVolBeenSet=0
    // init backlogYield to 1.0
    LgFrSceDemandAppData * demandAppDataPtr = new LgFrSceDemandAppData (0, 0, (size_t) nPeriods, 1.0);  
    demandAppDataPtr->pipPriority(tvDefaultPriority);
    //    std::cout << "Adding indp demand with default pipPiority [" 
    //	      << fullWitPart << "," << demandName << "] =" << (int) tvDefaultPriority[2] << std::endl;
    witSetDemandAppData(theWitRun,
			fullWitPart.c_str(), 
			demandName.c_str(), 
			(void *) demandAppDataPtr);
    
    witSetDemandPriority( theWitRun, fullWitPart.c_str(),
			  demandName.c_str(), tvDefaultPriority.intData() );
    witSetDemandShipLateLimit( theWitRun, fullWitPart.c_str(),
			       demandName.c_str(), defaultShipLateLimit );
    
    
    // but only load the obj1 values if we have too      
    if ( lpModelHelper_ != 0 ) {
      // get the default Obj1 Ship Reward
      float defaultObj1ShipReward = lpModelHelper_->defaultObj1ShipReward();
      
      // get the periodic Interest Rate
      float periodicInterestRate = lpModelHelper_->periodicInterestRate();
      
      float * defaultObj1ShipRewardVec = new float[nPeriods];
      float * defaultObj1CumShipRewardVec = new float[nPeriods];
      
      // make time vecs from scalars
      for (t=0; t<nPeriods; t++) {
	defaultObj1ShipRewardVec[t] = defaultObj1ShipReward;
	defaultObj1CumShipRewardVec[t] = defaultObj1ShipReward * 
	  periodicInterestRate;
      }
      
      witSetDemandObj1ShipReward( theWitRun, fullWitPart.c_str(),
				  demandName.c_str(), defaultObj1ShipRewardVec);
      witSetDemandObj1CumShipReward( theWitRun, fullWitPart.c_str(),
				     demandName.c_str(), defaultObj1CumShipRewardVec);
      delete [] defaultObj1ShipRewardVec;
      delete [] defaultObj1CumShipRewardVec;
    }
  }
  

  // IMPORTANT.   We need to go back and set the witDemandVols after all demandVol records are inputted.
  // Also need to deal with MULTI_ATTRIBUTE_DEMAND


 // DATED_DEMANDS
  if (useDatedDemands) {
    bool dieIfErrorFlag = true;
    int result = flatFileManager.readDatedDemandFloatRecord( theDatedDemandVol, requestDate, 
							     dieIfErrorFlag, -FLT_MAX, FLT_MAX,
							     next, fileName, dataLine, lineNo, 
							     mpHelper, theCal);
    // determine which period  it falls in
    // NOTE: this method uses period() ... which adhere's to strict calendar inclusive dates
    // NOTE: you can also use roundedPeriod() 
    thePeriod = theCal.period(requestDate);  
    

    nElementsChanged = result;
  }

  // VECTORED_DEMANDS 
  else {
    for (t = 0; t<nPeriods; t++) {
      int result = flatFileManager.nextFloat(floatVec[t], next, fileName, dataLine, lineNo, false,
					     -FLT_MAX, FLT_MAX);
      if (result == -1) 
	break;
    }
    
    nElementsChanged = t;
  }



  if (nElementsChanged == 0 || nElementsChanged == -1) {
    badCount++;
    continue;
  }




  // get the  demand app data for this demand.  get the hasDemandVolBeenSet
  LgFrSceDemandAppData * demandAppDataPtr;
  witGetDemandAppData(theWitRun, fullWitPart.c_str(), demandName.c_str(), (void **) &demandAppDataPtr);
  assert(demandAppDataPtr != 0);
  LgFrTimeVecFloat demandVolTVF = demandAppDataPtr->demandVol();
  int hasDemandVolBeenSet = demandAppDataPtr->hasDemandVolBeenSet();


  // DATED_DEMANDS
  if (useDatedDemands) {
    demandVolTVF[thePeriod] += theDatedDemandVol;
  }

  // VECTORED DEMANDS
  else {
    // note: duplicate demandVol records are incremental!!
    // print a warning when duplicate demandVols are detected (default is that they are incremental)
    for ( t=0; t<nElementsChanged; t++) {
      demandVolTVF[t] += floatVec[t];
    }
    
    if (hasDemandVolBeenSet) {
      (*theErrFacil)("MultipleDemandVolRecs",MclArgList() 
		     << fileName 
		     << (int) lineNo 
		     << fullWitPart
		     << demandName);
    }
  }
    

  // set the hasDemandVolBeenSet to true
  demandAppDataPtr->hasDemandVolBeenSet(1);
  // store away the original demandVol (with negatives)
  demandAppDataPtr->demandVol(demandVolTVF);

  // ENABLE_NEGATIVE_DEMANDS
  // strip off the negatives
  // FINISH_ME: How do we detect if multiple demandVol recs remove negatives?
  // Perhaps we should move the setting of doesPartHaveNegativeDemands to a
  // pre processing step
  for ( t=0; t<nPeriods; t++) {
    if (demandVolTVF[t] < 0.0) {
      demandVolTVF[t] = 0.0;
    }
  }

  witSetDemandDemandVol(theWitRun, fullWitPart.c_str(), demandName.c_str(), demandVolTVF.data());

  recCount++;
  if (myScrubber -> scrubMode() && outFile.good())
    {
      outFile << dataLine << std::endl;
    }

 }

 // delete [] defaultPriorityVec;
 delete [] floatVec;
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();


 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}





// -----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processDemandObjectiveFile(
    
    WitRun * const theWitRun,  
    const int nPeriods,        
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper,
    const float periodicInterestRate)
{

 int nElementsChanged;
 std::string pdf_PartName;
 std::string demandName;
 long lineNo = 0;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "Demand Objective Input",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);
 
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 float * floatVec = new float[nPeriods];
 
 while(inFile.peek() != EOF) {
    nElementsChanged = readCsvDemandFloatVector(
    	theWitRun,      // WIT environment    
    	nPeriods,       // Number of periods  
    	pdf_PartName,   // pdf_PartName to be returned
    	demandName,     // demandName to be returned
    	floatVec,       // floatVector to be returned
    	inFile,         // An open input stream
    	fileName,       // for messaging
        dataLine,
    	lineNo,         // current line number, for messaging
    	mpHelper,       // The helper object
    	false,          // Are all elements mandatory?
    	0.0,            // data lower limits
	1000000000,        // data upper limits
	false);         // Do NOT Add new demand stream to WIT

    if (nElementsChanged == 0 || nElementsChanged == -1) {
       badCount++;	 
       continue;
    }
    else if (nElementsChanged == -2) {
       commentCount++;
       continue;
    }

    if ( lpModelHelper_ != 0 ) {

      float  * rev;
      witGetDemandObj1ShipReward(theWitRun, pdf_PartName.c_str(), demandName.c_str(), &rev);
      
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nElementsChanged; t++) {
        rev[t] =  floatVec[t];
      }
      
      witSetDemandObj1ShipReward(theWitRun, pdf_PartName.c_str(), demandName.c_str(), rev);
      
      // Now set the CumShipReward as a timeValue of Money percentage = .10
      for ( t=0; t<nPeriods; t++) {
        rev[t] *=  periodicInterestRate;
      }
      witSetDemandObj1CumShipReward(theWitRun, pdf_PartName.c_str(), demandName.c_str(), rev);
      
      witFree(rev);
    }


    
    // Now set the priority for the heuristic, regardless of the engine.
    // NOTE: We need to have
    // the priorities be "the higer the value, the higher the priority"
    // in SCE, so to make it work in the heuristic we "flip" them around

    int * priority;
    witGetDemandPriority(theWitRun, pdf_PartName.c_str(), demandName.c_str(), &priority);
    
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nElementsChanged; t++) {
      priority[t] = 1000000000 - (int) floatVec[t];
      floatVec[t] = (float) priority[t];
    }
    
    witSetDemandPriority(theWitRun, pdf_PartName.c_str(), demandName.c_str(), priority);
    

    // if doPegging and optimization, then we might store the 
    // priorities in appData because that is where we will look for them
    bool doPegging = this->sceScenario()->setOfParameters().getBool("doPegging");
    if ( lpModelHelper_ != 0  &&  doPegging)  {
      LgFrSceDemandAppData * demandAppDataPtr;
      witGetDemandAppData(theWitRun,
			  pdf_PartName.c_str(), 
			  demandName.c_str(),
			  (void **) &demandAppDataPtr);
      assert ( demandAppDataPtr != 0);
      
      LgFrTimeVecFloat pipPriority(nPeriods, floatVec);
      demandAppDataPtr->pipPriority(pipPriority);
    }


    witFree(priority);
    
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
 }
 

 delete [] floatVec;
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}


// -----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processPipPriorityFile(
    WitRun * const theWitRun,  
    const int nPeriods,        
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

  int nElementsChanged;
  std::string pdf_PartName;
  std::string demandName;
  long lineNo = 0;
  std::string dataLine;
  
  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
  
  std::ifstream inFile;
  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  
  this->openInputFileAndScrubOutputFile(fileName,
					"PIP Demand Priority Sequencer ",
					inFile,
					outFile,
					myScrubber,
					theErrFacil);
  
  
  std::cout << "Processing " << fileName << ":\t" << std::flush;
  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;
  
  float * floatVec = new float[nPeriods];
  
  while(inFile.peek() != EOF) {
    nElementsChanged = readCsvDemandFloatVector(
	theWitRun,      // WIT environment    
	nPeriods,       // Number of periods  
	pdf_PartName,   // pdf_PartName to be returned
	demandName,     // demandName to be returned
	floatVec,       // floatVector to be returned
	inFile,         // An open input stream
	fileName,       // for messaging
	dataLine,
	lineNo,         // current line number, for messaging
	mpHelper,       // The helper object
	false,          // Are all elements mandatory?
	0.0,            // data lower limits
	1000000000,        // data upper limits
	false);         // do NOT Add new demand stream to WIT
    
    if (nElementsChanged == 0 || nElementsChanged == -1) {
      badCount++;	 
      continue;
    }
    else if (nElementsChanged == -2) {
      commentCount++;
      continue;
    }
    
    
    // put it in AppData !!
    LgFrSceDemandAppData * demandAppDataPtr;
    witGetDemandAppData(theWitRun,
			pdf_PartName.c_str(), 
			demandName.c_str(),
			(void **) &demandAppDataPtr);
    
    
    assert ( demandAppDataPtr != 0);

    
    LgFrTimeVecFloat pipPriority( demandAppDataPtr->pipPriority() );
    int t = 0; 
    // std::cout << ".....  Read new pipPriorty[" << demandName << "]=";
    for (t=0; t<nElementsChanged; t++) {
      pipPriority[t] = 1000000000 - (int) floatVec[t];
      //      std::cout << (int) floatVec[t] << "," << pipPriority.intData()[t] << "," << (int) pipPriority[t] << "  ,  ";
    }                                    
    //    std::cout << std::endl;

    demandAppDataPtr->pipPriority(pipPriority);
    
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
	outFile << dataLine << std::endl;
 }
  
  
  delete [] floatVec;
  std::cout << recCount << " records processed." << std::endl;
  inFile.close();
  outFile.close();
  
  flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}


// -----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processPipIndpPriorityFile(
    WitRun * const theWitRun,   // 
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

  int nElementsChanged;
  std::string mfgPart;
  std::string pdf;
  std::string fullWitPart;
  std::string demandName;
  long lineNo = 0;
  std::string dataLine;
  int t;
 
  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
  LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();   


  std::ifstream inFile;
  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
  this->openInputFileAndScrubOutputFile(fileName, "Pip Indpendent Priority ",
					inFile, outFile, myScrubber, theErrFacil);

 
  std::cout << "Processing " << fileName << ":\t" << std::flush;
  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;

  float * floatVec = new float[nPeriods];
 
  while(inFile.peek() != EOF) {
    int result = 0;
 
    // Read next line of file
    std::getline( inFile, dataLine );
    lineNo ++;

    dataLine = sce::strip( dataLine );
   
    //  skip blanks or comments '*'
    if ((dataLine[0] == '*') || dataLine.empty()) {
      commentCount++;
      continue;
    }
   
    // construct a tokenizer
    SCETokenizer next(dataLine);
   
    if ( flatFileManager.containsIllegalCharacters( dataLine )) {
      (*theErrFacil)("IllegalCharacter",MclArgList() 
		     << fileName << (int)lineNo << dataLine);
    }

  
    // get the partname
    mfgPart = flatFileManager.nextQuotedString(next, result, fileName, 
					       dataLine, lineNo, 
					       OPTIONAL_WITH_MESSAGE);

    // get the pdf
    pdf = flatFileManager.nextQuotedString(next, result, fileName, 
					   dataLine, lineNo, MANDATORY);

    // get the demandName
    demandName = flatFileManager.nextQuotedString(next, result, fileName, 
						  dataLine, lineNo, MANDATORY);

    // Get the witPartname
    fullWitPart = mpHelper.pdfPartname(mfgPart, pdf);



    int readCsvMessage = OPTIONAL_WITH_MESSAGE;

    if (! mpHelper.isPartValid( theWitRun, fullWitPart, fileName, 
				dataLine, lineNo, PURELY_OPTIONAL))  {
      (*theErrFacil)("IndDemandedPartNotExist",MclArgList() 
		     << mfgPart << pdf << demandName << fileName << (int)lineNo << dataLine);
      continue;
    }

    

    if (! doesDemandExist(theWitRun, fullWitPart, demandName)) {
      (*theErrFacil)("IndDemandDoesNotExist",MclArgList() 
		     << mfgPart << pdf << demandName << fileName << (int)lineNo << dataLine);
      continue;
    }
    
    
    // Now get the data
    for (t = 0; t<nPeriods; t++) {
      int result = flatFileManager.nextFloat(floatVec[t], next, fileName, dataLine, lineNo, false,
					     -FLT_MAX, FLT_MAX);
      if (result == -1) 
	break;
    }

    nElementsChanged = t;

    if (nElementsChanged == 0 || nElementsChanged == -1) {
      badCount++;
      continue;
    }

    // put it in AppData !!
    LgFrSceDemandAppData * demandAppDataPtr;
    witGetDemandAppData(theWitRun, fullWitPart.c_str(), demandName.c_str(), (void **) &demandAppDataPtr);
    
    assert ( demandAppDataPtr != 0);
    
    // If the demandAppDataPtr has been set, see if the pipPriority has been set.
    if (demandAppDataPtr == 0)  {
      demandAppDataPtr = new LgFrSceDemandAppData ((size_t) nPeriods, 1.0);  // init backlogYield to 1.0
      witSetDemandAppData(theWitRun, fullWitPart.c_str(), demandName.c_str(), (void *) demandAppDataPtr );
    }
    
    LgFrTimeVecFloat pipPriority( demandAppDataPtr->pipPriority() );
    int t = 0; 
    //    std::cout << ".....  Read new pipIndpPriorty[" << demandName << "]=";
    for (t=0; t<nElementsChanged; t++) {
      //      std::cout << floatVec[t] << ",";
      pipPriority[t] = 1000000000 - (int) floatVec[t];
    }                                    
    //    std::cout << std::endl;

    demandAppDataPtr->pipPriority(pipPriority);
    // ++++++++++++++++++++++++
    
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      outFile << dataLine << std::endl;
     
  }
 
  delete [] floatVec;
  std::cout << recCount << " records processed." << std::endl;
  inFile.close();
 

  flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}





// -----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processIndepDemandObjectiveFile(
    WitRun * const theWitRun,   // 
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper,
    const float periodicInterestRate)
{

 int nElementsChanged;
 std::string mfgPart;
 std::string pdf;
 std::string fullWitPart;
 std::string demandName;
 long lineNo = 0;
 std::string dataLine;
 int t;
 
 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
 LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();   


 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "Indpendent Demand Objective Input",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);





 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 float * floatVec = new float[nPeriods];
 
 while(inFile.peek() != EOF) {
   int result = 0;
 
   // Read next line of file
   std::getline( inFile, dataLine );
   lineNo ++;

   dataLine = sce::strip( dataLine );
   
   //  skip blanks or comments '*'
   if ((dataLine[0] == '*') || dataLine.empty()) {
    commentCount++;
    continue;
   }
   
   // construct a tokenizer
   SCETokenizer next(dataLine);
   
   if ( flatFileManager.containsIllegalCharacters( dataLine )) {
     (*theErrFacil)("IllegalCharacter",MclArgList() 
		    << fileName 
		    << (int)lineNo 
		    << dataLine);
   }

  
   // get the partname
   mfgPart = flatFileManager.nextQuotedString(next, result, fileName, 
					      dataLine, lineNo, 
					      OPTIONAL_WITH_MESSAGE);

   // get the pdf
   pdf = flatFileManager.nextQuotedString(next, result, fileName, 
					  dataLine, lineNo, MANDATORY);

   // get the demandName
   demandName = flatFileManager.nextQuotedString(next, result, fileName, 
						 dataLine, lineNo, MANDATORY);

   // Get the witPartname
   fullWitPart = mpHelper.pdfPartname(mfgPart, pdf);



  int readCsvMessage = OPTIONAL_WITH_MESSAGE;

  if (! mpHelper.isPartValid( theWitRun, fullWitPart, fileName, 
                              dataLine, lineNo, PURELY_OPTIONAL))  {
    (*theErrFacil)("IndDemandedPartNotExist",MclArgList() 
		   << mfgPart 
		   << pdf
		   << demandName
		   << fileName 
		   << (int)lineNo 
		   << dataLine);
    continue;
  }

    

  if (! doesDemandExist(theWitRun, fullWitPart, demandName)) {
    (*theErrFacil)("IndDemandDoesNotExist",MclArgList() 
		   << mfgPart 
		   << pdf
		   << demandName
		   << fileName 
		   << (int)lineNo 
		   << dataLine);
    continue;
  }

    
    
  // Now get the data
  for (t = 0; t<nPeriods; t++) {
    int result = flatFileManager.nextFloat(floatVec[t], next, fileName, dataLine, lineNo, false,
					   -FLT_MAX, FLT_MAX);
    if (result == -1) 
      break;
  }

  nElementsChanged = t;

  if (nElementsChanged == 0 || nElementsChanged == -1) {
    badCount++;
    continue;
  }

  if ( lpModelHelper_ != 0 ) {
    float  * rev;
    witGetDemandObj1ShipReward(theWitRun, fullWitPart.c_str(), demandName.c_str(), &rev);
    
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nElementsChanged; t++) {
      rev[t] =  floatVec[t];
    }
      
    witSetDemandObj1ShipReward(theWitRun, fullWitPart.c_str(), demandName.c_str(), rev);
      
    // Now set the CumShipReward as a timeValue of Money percentage = .10
    for ( t=0; t<nPeriods; t++) {
      rev[t] *=  periodicInterestRate;
    }
    witSetDemandObj1CumShipReward(theWitRun, fullWitPart.c_str(), demandName.c_str(), rev);
    
    witFree(rev);
  }

  // Now set the priority for the heuristic, regardless of the engine.
  // NOTE: We need to have
  // the priorities be "the higer the value, the higher the priority"
  // in SCE, so to make it work in the heuristic we "flip" them around

  int * priority;
  witGetDemandPriority(theWitRun, fullWitPart.c_str(), demandName.c_str(), &priority);
    
  int t = 0; // Pulled out of the for below by RW2STL
  for (t=0; t<nElementsChanged; t++) {
    priority[t] = 1000000000 - (int) floatVec[t];
    floatVec[t] = (float) priority[t];
  }
    
  witSetDemandPriority(theWitRun, fullWitPart.c_str(), demandName.c_str(), priority);

  // if doPegging and optimization, then we might store the 
  // priorities in appData because that is where we will look for them
  bool doPegging = this->sceScenario()->setOfParameters().getBool("doPegging");
  if ( lpModelHelper_ != 0  &&  doPegging)  {
    LgFrSceDemandAppData * demandAppDataPtr;
    witGetDemandAppData(theWitRun,
			fullWitPart.c_str(), 
			demandName.c_str(),
			(void **) &demandAppDataPtr);
    assert ( demandAppDataPtr != 0);
    
    LgFrTimeVecFloat pipPriority(nPeriods, floatVec);
    demandAppDataPtr->pipPriority(pipPriority);
  }

  witFree(priority);

  recCount++;
  if (myScrubber -> scrubMode() && outFile.good())
    {
      outFile << dataLine << std::endl;
    }

 }

 delete [] floatVec;
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();


 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}



// -----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processMinDemandVolFile(
    
    WitRun * const theWitRun,   // 
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{
 int nElementsChanged;
 std::string pdf_PartName;
 std::string demandName;
 float * cshlb;
 float * csslb;
 float * cshub;
 float * demandVol;
 long lineNo = 0;
 std::string dataLine;
 bool isRecordGood;
 int t;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "MinDemandVol Input",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);


 float * floatVec = new float[nPeriods];

 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 while(inFile.peek() != EOF) {
    nElementsChanged = readCsvDemandFloatVector(
	theWitRun,      // WIT environment    
	nPeriods,       // Number of periods  
	pdf_PartName,   // pdf_PartName to be returned
	demandName,     // demandName to be returned
	floatVec,       // floatVector to be returned
	inFile,         // An open input stream
	fileName,       // for messaging
        dataLine,
	lineNo,         // current line number, for messaging
	mpHelper,       // The helper object
	false,          // Are all elements mandatory?
	0.0,            // data lower limits
	FLT_MAX,        // data upper limits
	false);         // Do NOT Add new demand stream to WIT

    if (nElementsChanged == 0 || nElementsChanged == -1) {
       badCount++; 
       continue;
    }
    else if (nElementsChanged == -2) {
       commentCount++;
       continue;
    }

    witGetDemandCumShipBounds(theWitRun, 
			      pdf_PartName.c_str(), 
			      demandName.c_str(), 
			      &cshlb,
			      &csslb,
			      &cshub);

    witGetDemandDemandVol(theWitRun,
                          pdf_PartName.c_str(),
                          demandName.c_str(),
                          &demandVol);

 

    // make sure the demand exceeds the minDemand, period by period
    isRecordGood = true;
    for(t=0; t<nElementsChanged; t++){
       if ((demandVol[t] + .001) < floatVec[t]){
          // reject this record, call the error routine
          // print out error
          badCount ++;
	  (*theErrFacil)("MinDemandExceedsDemand",MclArgList() 
			 << fileName 
			 << (int)lineNo 
			 << dataLine
			 << t);
          isRecordGood = false;          
          break;
       }
    }
    witFree(demandVol);

    if (isRecordGood){
       // put in some check to see if this minDemand has already been touched
       // get the demand app data for this demand.  
       // get the hasMinDemandVolBeenSet
       LgFrSceDemandAppData * demandAppDataPtr;
       witGetDemandAppData(theWitRun,
			pdf_PartName.c_str(), 
			demandName.c_str(),
			(void **) &demandAppDataPtr);
       assert(demandAppDataPtr != 0);
       int hasMinDemandVolBeenSet = demandAppDataPtr->hasMinDemandVolBeenSet();

       if (hasMinDemandVolBeenSet) {
          badCount ++;
          std::string pnm = mpHelper.partname(pdf_PartName);
          std::string pdfff = mpHelper.pdf(pdf_PartName);
          (*theErrFacil)("MultipleMinDemandVolRecs",MclArgList() 
		     << fileName 
		     << (int) lineNo 
		     << pnm 
		     << pdfff);
       }
       else
          recCount ++;

    // update the demand cumulative ship bounds
    csslb[0] = floatVec[0];
    for(t=1; t<nElementsChanged; t++)
       csslb[t] = csslb[t-1] + floatVec[t];
    // 2.30 fix: don't forget to populate the rest of the mins file
    for(t=nElementsChanged; t<nPeriods; t++)
       csslb[t] = csslb[t-1];
    witSetDemandCumShipBounds(theWitRun, 
                              pdf_PartName.c_str(), 
                              demandName.c_str(), 
                              cshlb,
                              csslb,
                              cshub);
    // increment the nDemandsWithNonZeroMins counter if the last period of 
    // the csslb is greater than 0
    
    if (csslb[nPeriods-1] > 0 && !hasMinDemandVolBeenSet)
        sceScenario()->incNDemandsWithNonZeroMins();

    if (csslb[nPeriods-1] < 0.00001 && hasMinDemandVolBeenSet){
       (*theErrFacil)("ResetMinDemandVolRecs",MclArgList() 
		     << fileName 
		     << (int) lineNo 
                     << dataLine);
        sceScenario()->decNDemandsWithNonZeroMins();
    }
    // set the hasDemandVolBeenSet to true if last period of csslb > 0
    if (csslb[nPeriods-1] > 0) 
      demandAppDataPtr->hasMinDemandVolBeenSet(1);
    else 
      demandAppDataPtr->hasMinDemandVolBeenSet(0);

    witFree(cshlb);
    witFree(csslb);
    witFree(cshub);
  
  
    }

 }

 delete [] floatVec;

 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
  outFile.close();
  
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processMinIndepDemandVolFile(
    WitRun * const theWitRun,   // 
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{
  
  int nElementsChanged;
  std::string mfgPart;
  std::string pdf;
  std::string fullWitPart;
  std::string demandName;
  
  int t;
  float * cshlb;
  float * csslb;
  float * cshub;
  float * demandVol;
  bool isRecordGood;
  
  std::ifstream inFile;
  long lineNo = 0;
  std::string dataLine;
  
  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
  LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();   
  
  
  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  
  this->openInputFileAndScrubOutputFile(fileName,
					"Min Independent Demand Volume",
					inFile,
					outFile,
					myScrubber,
					theErrFacil);
  
  
  
 
  std::cout << "Processing " << fileName << ":\t" << std::flush;
  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;
  
  float * floatVec = new float[nPeriods];
  
  while(inFile.peek() != EOF) {
    int result = 0;
    
    // Read next line of file
    std::getline( inFile, dataLine );
    lineNo ++;
    
    dataLine = sce::strip( dataLine );
    
    //  skip blanks or comments '*'
    if ((dataLine[0] == '*') || dataLine.empty()) {
      commentCount++;
      continue;
    }
   
    // construct a tokenizer
    SCETokenizer next(dataLine);
    
    if ( flatFileManager.containsIllegalCharacters( dataLine )) {
      (*theErrFacil)("IllegalCharacter",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
    }

  
    // get the partname
    mfgPart = flatFileManager.nextQuotedString(next, result, fileName, 
					       dataLine, lineNo, 
					       OPTIONAL_WITH_MESSAGE);
    
    // get the pdf
    pdf = flatFileManager.nextQuotedString(next, result, fileName, 
					   dataLine, lineNo, MANDATORY);
    
    // get the demandName
    demandName = flatFileManager.nextQuotedString(next, result, fileName, 
						  dataLine, lineNo, MANDATORY);
    
    // Get the witPartname
    fullWitPart = mpHelper.pdfPartname(mfgPart, pdf);
    
    
    
    int readCsvMessage = OPTIONAL_WITH_MESSAGE;

    if (! mpHelper.isPartValid( theWitRun, fullWitPart, fileName, 
				dataLine, lineNo, PURELY_OPTIONAL))  {
      (*theErrFacil)("IndDemandedPartNotExist",MclArgList() 
		     << mfgPart 
		     << pdf
		     << demandName
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
      continue;
    }
    
    if (! doesDemandExist(theWitRun, fullWitPart, demandName)) {
      (*theErrFacil)("IndDemandDoesNotExist",MclArgList() 
		     << mfgPart 
		     << pdf
		     << demandName
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
      continue;
    }
    
    
    
    
    
    
    // Now get the data
    bool isMinDemandVolNegative = 0;
    for (t = 0; t<nPeriods; t++) {
      int result = flatFileManager.nextFloat(floatVec[t], next, fileName, dataLine, lineNo, false,
					     -FLT_MAX, FLT_MAX);
      if (floatVec[t] < -0.00001)  {
	isMinDemandVolNegative = 1;
	floatVec[t] = 0.0;
      }

      if (result == -1) 
	break;
    }
  
  


    nElementsChanged = t;
    
    if (nElementsChanged == 0 || nElementsChanged == -1) {
      badCount++;
      continue;
    }


    if (isMinDemandVolNegative) {
      (*theErrFacil)("MinIndDemVolIsNegative",MclArgList() 
		     << mfgPart 
		     << pdf
		     << demandName
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
    }
    





    // ================
    // from minDemandVol
    witGetDemandCumShipBounds(theWitRun, 
			      fullWitPart.c_str(), 
			      demandName.c_str(), 
			      &cshlb,
			      &csslb,
			      &cshub);
    
    witGetDemandDemandVol(theWitRun,
			  fullWitPart.c_str(),
			  demandName.c_str(),
			  &demandVol);
    
    // tomtodo: put an if condition around this.   Only enforce this 
    //          restriction for heuristic engines.   OR, maybe an algorithm!!
    //          Do the same for minDemands as well.
    
    // make sure the demand exceeds the minDemand, period by period
    isRecordGood = true;
    for(t=0; t<nElementsChanged; t++){
      if ((demandVol[t] + .001) < floatVec[t]){
	// reject this record, call the error routine
	badCount ++;
	(*theErrFacil)("MinDemandExceedsDemand",MclArgList() 
		       << fileName 
		       << (int)lineNo 
		       << dataLine
		       << t);
	isRecordGood = false;          
	break;
      }
    }
    witFree(demandVol);




  
    if (isRecordGood){
      // put in some check to see if this minDemand has already been touched
      // get the demand app data for this demand.  
      // get the hasMinDemandVolBeenSet
      LgFrSceDemandAppData * demandAppDataPtr;
      witGetDemandAppData(theWitRun,
			  fullWitPart.c_str(), 
			  demandName.c_str(),
			  (void **) &demandAppDataPtr);
      assert(demandAppDataPtr != 0);
      int hasMinDemandVolBeenSet = demandAppDataPtr->hasMinDemandVolBeenSet();
      
      if (hasMinDemandVolBeenSet) {
	// finish_me: need new message!! (include demandName)
	badCount ++;
	std::string pnm = mpHelper.partname(fullWitPart);
	std::string pdfff = mpHelper.pdf(fullWitPart);
	(*theErrFacil)("MultipleMinDemandVolRecs",MclArgList() 
		       << fileName 
		       << (int) lineNo 
		       << pnm 
		       << pdfff);
      }
    
      // update the demand cumulative ship bounds
      csslb[0] = floatVec[0];
      for(t=1; t<nElementsChanged; t++)
	csslb[t] = csslb[t-1] + floatVec[t];
      for(t=nElementsChanged; t<nPeriods; t++)
	csslb[t] = csslb[t-1];
      witSetDemandCumShipBounds(theWitRun, 
				fullWitPart.c_str(), 
				demandName.c_str(), 
				cshlb,
				csslb,
				cshub);
      // increment the nDemandsWithNonZeroMins counter if the last period of 
      // the csslb is greater than 0
      
      if (csslb[nPeriods-1] > 0 && !hasMinDemandVolBeenSet)
	sceScenario()->incNDemandsWithNonZeroMins();
      
      if (csslb[nPeriods-1] < 0.00001 && hasMinDemandVolBeenSet){
	(*theErrFacil)("ResetMinDemandVolRecs",MclArgList() 
		       << fileName 
		       << (int) lineNo 
		       << dataLine);
	sceScenario()->decNDemandsWithNonZeroMins();
      }
   
      // set the hasDemandVolBeenSet to true if last period of csslb > 0
      if (csslb[nPeriods-1] > 0) 
	demandAppDataPtr->hasMinDemandVolBeenSet(1);
      else 
	demandAppDataPtr->hasMinDemandVolBeenSet(0);
      
      witFree(cshlb);
      witFree(csslb);
      witFree(cshub);

      recCount++;
      if (myScrubber -> scrubMode() && outFile.good())
	{
	  outFile << dataLine << std::endl;
	}
    }

  }

  delete [] floatVec;
  std::cout << recCount << " records processed." << std::endl;
  inFile.close();
  
  
  flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}


// -----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processBacklogYieldFile(
    
    WitRun * const theWitRun,   // 
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{
  int nElementsChanged;
  std::string pdf_PartName;
  std::string demandName;
  long lineNo = 0;
  std::string dataLine;
  
  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  std::ifstream inFile;
  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  
  this->openInputFileAndScrubOutputFile(fileName,
				       "backlogYield",
					inFile,
					outFile,
					myScrubber,
					theErrFacil);
  
  float * floatVec = new float[nPeriods];
  
  std::cout << "Processing " << fileName << ":\t" << std::flush;
  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;
  
  while(inFile.peek() != EOF) {
    nElementsChanged = readCsvDemandFloatVector(
	theWitRun,      // WIT environment    
	nPeriods,       // Number of periods  
	pdf_PartName,   // pdf_PartName to be returned
	demandName,     // demandName to be returned
	floatVec,       // floatVector to be returned
	inFile,         // An open input stream
	fileName,       // for messaging
        dataLine,
	lineNo,         // current line number, for messaging
	mpHelper,       // The helper object
	false,          // Are all elements mandatory?
	0.0,            // data lower limits
	1.0,            // data upper limits
	false);         // do NOT add new demand stream to WIT

    if (nElementsChanged == 0 || nElementsChanged == -1) {
      badCount++;
      continue;
    }
    else if (nElementsChanged == -2) {
      commentCount++;
      continue;
    }
        
    LgFrSceDemandAppData * demandAppDataPtr;
    witGetDemandAppData(theWitRun,
			pdf_PartName.c_str(), 
			demandName.c_str(),
			(void **) &demandAppDataPtr);


    
    // If the demandAppDataPtr hasn't been set (i.e., its 0) then "new" one
    // and set the backlogYield_.
    // If the demandAppDataPtr has been set, see if the backlogYield has been set.
    // If it hasn't (i.e. the backlogYield is 0), then set it. Otherwise, do nothing.
    if (demandAppDataPtr == 0)  {
      demandAppDataPtr = new LgFrSceDemandAppData ((size_t) nPeriods, 1.0);  // init backlogYield to 1.0
      assert ( demandAppDataPtr != 0);

      witSetDemandAppData(theWitRun,
                          pdf_PartName.c_str(), 
                          demandName.c_str(), 
                          (void *) demandAppDataPtr );
    }

    LgFrTimeVecFloat backlogYield( demandAppDataPtr->backlogYield() );
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nElementsChanged; t++) {
      backlogYield[t] = floatVec[t]; 
    }                                          
    
    demandAppDataPtr->backlogYield(backlogYield);


    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
  }

 delete [] floatVec;
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
  outFile.close();
  
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

// -----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processGreedySearchIncFile(
    
    WitRun * const theWitRun,   // 
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{
  int nElementsChanged;
  std::string pdf_PartName;
  std::string demandName;
  long lineNo = 0;
  std::string dataLine;
  
  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  std::ifstream inFile;
  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  
  this->openInputFileAndScrubOutputFile(fileName,
				       "greedySearchInc",
					inFile,
					outFile,
					myScrubber,
					theErrFacil);
  
  float greedySearchInc = 1.0;
  
  std::cout << "Processing " << fileName << ":\t" << std::flush;
  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;
  
  while(inFile.peek() != EOF) {
    nElementsChanged = readCsvDemandFloatScalar(
	theWitRun,      // WIT environment    
	nPeriods,       // Number of periods  
	pdf_PartName,   // pdf_PartName to be returned
	demandName,     // demandName to be returned
	greedySearchInc,       // float to be returned
	inFile,         // An open input stream
	fileName,       // for messaging
        dataLine,
	lineNo,         // current line number, for messaging
	mpHelper,       // The helper object
	OPTIONAL_WITH_MESSAGE,          // Is the element  mandatory?
	0.0,            // data lower limits
	FLT_MAX,            // data upper limits
	false);         // do NOT add new demand stream to WIT

    if (nElementsChanged == 0 || nElementsChanged == -1) {
      badCount++;
      continue;
    }
    else if (nElementsChanged == -2) {
      commentCount++;
      continue;
    }
        
    float gsi;
    witSetDemandSearchInc(theWitRun,
			  pdf_PartName.c_str(), 
			  demandName.c_str(),
			  greedySearchInc);

    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
  }

  std::cout << recCount << " records processed." << std::endl;
  inFile.close();
  outFile.close();
  
  flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}




// -----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processIndpGreedySearchIncFile(
    
    WitRun * const theWitRun,   // 
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string mfgPart;
 std::string pdf;
 std::string fullWitPart;
 std::string demandName;
 long lineNo = 0;
 std::string dataLine;
 
 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
 LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();   


 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "Indpendent Greedy Search Increment",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);





 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 float * floatVec = new float[nPeriods];
 
 while(inFile.peek() != EOF) {
   int result = 0;
 
   // Read next line of file
   std::getline( inFile, dataLine );
   lineNo ++;

   dataLine = sce::strip( dataLine );
   
   //  skip blanks or comments '*'
   if ((dataLine[0] == '*') || dataLine.empty()) {
    commentCount++;
    continue;
   }
   
   // construct a tokenizer
   SCETokenizer next(dataLine);
   
   if ( flatFileManager.containsIllegalCharacters( dataLine )) {
     (*theErrFacil)("IllegalCharacter",MclArgList() 
		    << fileName 
		    << (int)lineNo 
		    << dataLine);
   }

  
   // get the partname
   mfgPart = flatFileManager.nextQuotedString(next, result, fileName, 
					      dataLine, lineNo, 
					      OPTIONAL_WITH_MESSAGE);

   // get the pdf
   pdf = flatFileManager.nextQuotedString(next, result, fileName, 
					  dataLine, lineNo, MANDATORY);

   // get the demandName
   demandName = flatFileManager.nextQuotedString(next, result, fileName, 
						 dataLine, lineNo, MANDATORY);

   // Get the witPartname
   fullWitPart = mpHelper.pdfPartname(mfgPart, pdf);



  int readCsvMessage = OPTIONAL_WITH_MESSAGE;

  if (! mpHelper.isPartValid( theWitRun, fullWitPart, fileName, 
                              dataLine, lineNo, PURELY_OPTIONAL))  {
    (*theErrFacil)("IndDemandedPartNotExist",MclArgList() 
		   << mfgPart 
		   << pdf
		   << demandName
		   << fileName 
		   << (int)lineNo 
		   << dataLine);
    continue;
  }

    

  if (! doesDemandExist(theWitRun, fullWitPart, demandName)) {
    (*theErrFacil)("IndDemandDoesNotExist",MclArgList() 
		   << mfgPart 
		   << pdf
		   << demandName
		   << fileName 
		   << (int)lineNo 
		   << dataLine);
    continue;
  }

    
    
  // Now get the data
  float greedySearchInc;
  result = flatFileManager.nextFloat(greedySearchInc, next, fileName, dataLine, lineNo, false,
				     0, FLT_MAX);
  
  if (result == -1) {
    badCount++;
    continue;
  }
  
  
  witSetDemandSearchInc(theWitRun,
			fullWitPart.c_str(), 
			demandName.c_str(),
			greedySearchInc);
  
  recCount++;
  if (myScrubber -> scrubMode() && outFile.good())
    {
      outFile << dataLine << std::endl;
    }
 }
 
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}



// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processProdVolLBFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 int nElementsChanged;
 std::string pdf_PartName;
 float * pvhLB;
 float * pvsLB;
 float * pvhUB;
 long lineNo = 0;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "prodVolLBFile",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 float * floatVec = new float[nPeriods];

 while(inFile.peek() != EOF) {
    nElementsChanged = flatFileManager.readCsvPartFloatVector(
	theWitRun,        // WIT environment    
	nPeriods,         // Number of periods  
	pdf_PartName,     // legal pdf_PartName to be returned
	floatVec,         // floatVector to be returned
	inFile,           // An open stream
	fileName,         // for messaging
        dataLine,
	lineNo,           // for messaging
	mpHelper,         // The helper
	OPTIONAL_WITH_MESSAGE,  // Use one of MANDATORY, PURELY_OPTIONAL
	                        // or OPTIONAL_WITH_MESSAGE
	0.0,              // data lower limits
	FLT_MAX);         // data upper limits


    if (nElementsChanged == 0 || nElementsChanged == -1) {
       badCount++;
       continue;
    }
    else if (nElementsChanged == -2) {
       commentCount++;
       continue;
    }
    
    witGetPartProdBounds(theWitRun, pdf_PartName.c_str(), &pvhLB, &pvsLB, &pvhUB);

    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nElementsChanged; t++) {
       pvsLB[t] = floatVec[t];
       }
    witSetPartProdBounds(theWitRun, pdf_PartName.c_str(), pvhLB, pvsLB, pvhUB);
    witFree(pvhLB);
    witFree(pvsLB);
    witFree(pvhUB);
//    std::cout << "Updated prodVol Soft Lower Bound  for " << pdf_PartName << "\n";
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
 }

 delete [] floatVec;
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processProdVolUBFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 int nElementsChanged;
 std::string pdf_PartName;
 float * pvhLB;
 float * pvsLB;
 float * pvhUB;
 long lineNo = 0;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "prodVolUBFile",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 float * floatVec = new float[nPeriods];

 while(inFile.peek() != EOF) {
    nElementsChanged = flatFileManager.readCsvPartFloatVector(
	theWitRun,        // WIT environment    
	nPeriods,         // Number of periods  
	pdf_PartName,     // legal pdf_PartName to be returned
	floatVec,         // floatVector to be returned
	inFile,           // An open stream
	fileName,         // for messaging
        dataLine,
	lineNo,           // for messaging
	mpHelper,         // The helper
	OPTIONAL_WITH_MESSAGE,  // Use one of MANDATORY, PURELY_OPTIONAL
	                        // or OPTIONAL_WITH_MESSAGE
	-FLT_MAX,              // data lower limits
	FLT_MAX);         // data upper limits

    if (nElementsChanged == 0 || nElementsChanged == -1) {
       badCount++; 
       continue;
    }
    else if (nElementsChanged == -2) {
       commentCount++;
       continue;
    }
    
    witGetPartProdBounds(theWitRun, pdf_PartName.c_str(), &pvhLB, &pvsLB, &pvhUB);

    int lastPeriodWithUBltLB = -1;
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nElementsChanged; t++) {
      if ((floatVec[t] < pvsLB[t]) && (floatVec[t] >= 0.0)) {
	lastPeriodWithUBltLB = t;
	pvhUB[t] = pvsLB[t];
      }
      else {
	pvhUB[t] = floatVec[t];
      }
    }
    if (lastPeriodWithUBltLB > -1) {
      (*theErrFacil)("ConflictingBounds",MclArgList() << "ProdVol" << pdf_PartName << fileName << (int)lineNo << (int)lastPeriodWithUBltLB);
    }
    witSetPartProdBounds(theWitRun, pdf_PartName.c_str(), pvhLB, pvsLB, pvhUB);
    witFree(pvhLB);
    witFree(pvsLB);
    witFree(pvhUB);
//    std::cout << "Updated prodVol Hard Upper Bound  for " << pdf_PartName << "\n";
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
      
    }

 delete [] floatVec;
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
  outFile.close();
  
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processOperationLBFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 int nElementsChanged;
 std::string pdf_OperationName;
 float * hardLB;
 float * softLB;
 float * hardUB;
 long lineNo = 0;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "opLBFile",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 float * floatVec = new float[nPeriods];

 while(inFile.peek() != EOF) {
    nElementsChanged = flatFileManager.readCsvOperationFloatVector(
	theWitRun,        // WIT environment    
	nPeriods,         // Number of periods  
	pdf_OperationName,// legal pdf_OperationName to be returned
	floatVec,         // floatVector to be returned
	inFile,           // An open stream
	fileName,         // for messaging
        dataLine,
	lineNo,           // for messaging
	mpHelper,         // The helper
	OPTIONAL_WITH_MESSAGE,  // Use one of MANDATORY, PURELY_OPTIONAL
	                        // or OPTIONAL_WITH_MESSAGE
	0.0,              // data lower limits
	FLT_MAX);         // data upper limits


    if (nElementsChanged == 0 || nElementsChanged == -1) {
       badCount++; 
       continue;
    }
    else if (nElementsChanged == -2) {
       commentCount++;
       continue;
    }
    
    witGetOperationExecBounds(theWitRun, pdf_OperationName.c_str(), &hardLB, &softLB, &hardUB);

    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nElementsChanged; t++) {
       softLB[t] = floatVec[t];
       }
    witSetOperationExecBounds(theWitRun, pdf_OperationName.c_str(), hardLB, softLB, hardUB);
    witFree(hardLB);
    witFree(softLB);
    witFree(hardUB);
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
 }

 delete [] floatVec;
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processOperationUBFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 int nElementsChanged;
 std::string pdf_OperationName;
 float * hardLB;
 float * softLB;
 float * hardUB;
 long lineNo = 0;
 std::string dataLine;
 
 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "opUBFile",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 float * floatVec = new float[nPeriods];

 while(inFile.peek() != EOF) {
    nElementsChanged = flatFileManager.readCsvOperationFloatVector(
	theWitRun,        // WIT environment    
	nPeriods,         // Number of periods  
	pdf_OperationName,// legal pdf_OperationName to be returned
	floatVec,         // floatVector to be returned
	inFile,           // An open stream
	fileName,         // for messaging
        dataLine,
	lineNo,           // for messaging
	mpHelper,         // The helper
	OPTIONAL_WITH_MESSAGE,  // Use one of MANDATORY, PURELY_OPTIONAL
	                  // or OPTIONAL_WITH_MESSAGE
	-FLT_MAX,         // data lower limits
	FLT_MAX);         // data upper limits

    if (nElementsChanged == 0 || nElementsChanged == -1) {
       badCount++; 
       continue;
    }
    else if (nElementsChanged == -2) {
       commentCount++;
       continue;
    }
    
    witGetOperationExecBounds(theWitRun, pdf_OperationName.c_str(), &hardLB, &softLB, &hardUB);

    int lastPeriodWithUBltLB = -1;
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nElementsChanged; t++) {
      if ((floatVec[t] < softLB[t]) && (floatVec[t] >= 0.0)) {
	lastPeriodWithUBltLB = t;
	hardUB[t] = softLB[t];
      }
      else {
	hardUB[t] = floatVec[t];
      }
    }
    if (lastPeriodWithUBltLB > -1) {
      (*theErrFacil)("ConflictingBounds",MclArgList() << "Operation ExecutionVol" << pdf_OperationName << fileName << (int)lineNo << lastPeriodWithUBltLB);
    }
    witSetOperationExecBounds(theWitRun, pdf_OperationName.c_str(), hardLB, softLB, hardUB);
    witFree(hardLB);
    witFree(softLB);
    witFree(hardUB);
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
 }

 delete [] floatVec;
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}



// --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processBuildAheadFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string partName;
 std::string pdf;
 std::string buildAheadType;

 int result;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "buildAhead",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);



 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 while(inFile.peek() != EOF) {

     // read the next line
    std::getline( inFile, dataLine );
    lineNo++;

    //  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
        commentCount++;
	continue;
    }

    // check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
        }


    SCETokenizer next(dataLine);

    // get the partName 
    partName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					 lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
        badCount++;
	continue;
    }

    // get the PDF
    pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, MANDATORY);


    // get the buildAheadCode
    buildAheadType = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, PURELY_OPTIONAL);


    // The rest of the fields are optional.  
    // Set the defaults, and parse the rest of the line.  
    int buildAheadLimitPeriods = nPeriods-1;
    float buildAheadLimitDays = -1.0;
    int   early = 0;
    int   late = nPeriods-1;

    // buildAheadLimitDays        
    result = flatFileManager.nextFloat(buildAheadLimitDays, next, fileName, dataLine, lineNo, false, -INT_MAX);

    // effectivity Dates
    if (result >= 0)
	flatFileManager.nextStartEndDates(early, late, next, fileName, lineNo, dataLine, theCal);
    if (result == 1) {
        badCount++;
	continue;
    }
    if ((early == nPeriods) || (late < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() << "Effectivity dates" << fileName << (int) lineNo << dataLine << "");
	badCount++;
	continue;
    }

    // scale the buildAheadUBDays
    buildAheadLimitPeriods = floor(buildAheadLimitDays / theCal.workUnits(early));
    if (buildAheadLimitPeriods < 0)
      buildAheadLimitPeriods = nPeriods - 1;

    if (buildAheadLimitPeriods > nPeriods - 1)
      buildAheadLimitPeriods = nPeriods - 1;


    std::string fullPartName(mpHelper.pdfPartname(partName, pdf));
	
    if (! mpHelper.isPartValid(theWitRun, partName, pdf, 
			       fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE)) {
      	badCount++;
	continue;
    }


    int * buildAheadUB;
    witGetPartBuildAheadUB(theWitRun, fullPartName.c_str(), &buildAheadUB);


#if 0
    std::cout << "Setting buildAheadUB \n    From:\n ";
    std::string blankVector(6, ' ');
    int ttt = 0; 
    for (ttt=0; ttt < nPeriods; ttt++) 
      {
	std::string sNum(sce::stringFrom((float) buildAheadUB[ttt], (int) 1));
	int blankSpace = 6 - sNum.length();
	sNum.insert(0, blankVector.c_str(), blankSpace);
	std::cout << sNum;
      }
    std::cout  << std::endl;
#endif    


    int t;
    for (t=early; t<=late; t++) 
      buildAheadUB[t] = buildAheadLimitPeriods;

#if 0
    std::cout << "      To:\n ";
    std::string blankVector2(6, ' ');
    for (ttt=0; ttt < nPeriods; ttt++) 
      {
	std::string sNum(sce::stringFrom((float) buildAheadUB[ttt], (int) 1));
	int blankSpace = 6 - sNum.length();
	sNum.insert(0, blankVector2.c_str(), blankSpace);
	std::cout << sNum;
      }
    std::cout  << std::endl;
#endif




    if (buildAheadType == "N") {
      witSetPartBuildAsap(theWitRun, fullPartName.c_str(), WitFALSE);
      witSetPartBuildNstn(theWitRun, fullPartName.c_str(), WitTRUE);
      witSetPartBuildAheadUB(theWitRun, fullPartName.c_str(), buildAheadUB);
      witFree(buildAheadUB);
    }

    else if (buildAheadType == "A") {
      witSetPartBuildNstn(theWitRun, fullPartName.c_str(), WitFALSE);
      witSetPartBuildAsap(theWitRun, fullPartName.c_str(), WitTRUE);
      witSetPartBuildAheadUB(theWitRun, fullPartName.c_str(), buildAheadUB);
      witFree(buildAheadUB);
    }

    else {
      (*theErrFacil)("BadBuildAheadCode",MclArgList() << partName 
		     << pdf 
		     << buildAheadType 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
      witSetPartBuildNstn(theWitRun, fullPartName.c_str(), WitFALSE);
      witSetPartBuildAsap(theWitRun, fullPartName.c_str(), WitTRUE);
      witFree(buildAheadUB);
    }

    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
        outFile << dataLine << std::endl;
      }
    }
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

// --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processScrapCostFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string partName;
 std::string pdf;
 std::string buildAheadType;

 int result;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "scrapCost",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 LgFrTimeVecFloat bigM(nPeriods, 99999.9);
 int nParts;
 char ** partList;
 witGetParts( theWitRun, &nParts, &partList );
 int iii;
 for (iii=0; iii<nParts; iii++) 
   witSetPartScrapCost(theWitRun, partList[iii], bigM.data());




 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 while(inFile.peek() != EOF) {

     // read the next line
    std::getline( inFile, dataLine );
    lineNo++;

    //  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
        commentCount++;
	continue;
    }

    // check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
        }


    SCETokenizer next(dataLine);

    // get the partName 
    partName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					 lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
        badCount++;
	continue;
    }

    // get the PDF
    pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, MANDATORY);




    // The rest of the fields are optional.  
    // Set the defaults, and parse the rest of the line.  
    float scrapCost = 0.0;
    int   early = 0;
    int   late = nPeriods-1;

    // scrapCost        
    result = flatFileManager.nextFloat(scrapCost, next, fileName, dataLine, lineNo, false, -INT_MAX);

    // effectivity Dates
    if (result >= 0)
	flatFileManager.nextStartEndDates(early, late, next, fileName, lineNo, dataLine, theCal);
    if (result == 1) {
        badCount++;
	continue;
    }
    if ((early == nPeriods) || (late < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() << "Effectivity dates" << fileName << (int) lineNo << dataLine << "");
	badCount++;
	continue;
    }


    std::string fullPartName(mpHelper.pdfPartname(partName, pdf));
	
    if (! mpHelper.isPartValid(theWitRun, partName, pdf, 
			       fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE)) {
      	badCount++;
	continue;
    }


    float  * scrapCostVol;
    witGetPartScrapCost(theWitRun, fullPartName.c_str(), &scrapCostVol);

    int t;
    for (t=early; t<=late; t++) 
      scrapCostVol[t] = scrapCost;

    witSetPartScrapCost(theWitRun, fullPartName.c_str(), scrapCostVol);
    witFree(scrapCostVol);


    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
        outFile << dataLine << std::endl;
      }
    }
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}




// --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processStockCostFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string partName;
 std::string pdf;
 std::string buildAheadType;

 int result;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "stockCost",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 LgFrTimeVecFloat bigM(nPeriods, 99999.9);
 int nParts;
 char ** partList;
 witGetParts( theWitRun, &nParts, &partList );
 int iii;
 for (iii=0; iii<nParts; iii++) 
   witSetPartObj1StockCost(theWitRun, partList[iii], bigM.data());




 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;
 long lineNo = 0;
 while(inFile.peek() != EOF) {

     // read the next line
    std::getline( inFile, dataLine );
    lineNo++;

    //  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
        commentCount++;
	continue;
    }

    // check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
        }


    SCETokenizer next(dataLine);

    // get the partName 
    partName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
					 lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) {
        badCount++;
	continue;
    }

    // get the PDF
    pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, MANDATORY);




    // The rest of the fields are optional.  
    // Set the defaults, and parse the rest of the line.  
    float stockCost = 0.0;
    int   early = 0;
    int   late = nPeriods-1;

    // scrapCost        
    result = flatFileManager.nextFloat(stockCost, next, fileName, dataLine, lineNo, false, -INT_MAX);

    // effectivity Dates
    if (result >= 0)
	flatFileManager.nextStartEndDates(early, late, next, fileName, lineNo, dataLine, theCal);
    if (result == 1) {
        badCount++;
	continue;
    }
    if ((early == nPeriods) || (late < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() << "Effectivity dates" << fileName << (int) lineNo << dataLine << "");
	badCount++;
	continue;
    }


    std::string fullPartName(mpHelper.pdfPartname(partName, pdf));
	
    if (! mpHelper.isPartValid(theWitRun, partName, pdf, 
			       fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE)) {
      	badCount++;
	continue;
    }


    float  * stockCostVol;
    witGetPartObj1StockCost(theWitRun, fullPartName.c_str(), &stockCostVol);

    int t;
    for (t=early; t<=late; t++) 
      stockCostVol[t] = stockCost;

    witSetPartObj1StockCost(theWitRun, fullPartName.c_str(), stockCostVol);
    witFree(stockCostVol);


    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
        outFile << dataLine << std::endl;
      }
    }
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}



#ifdef VARIABLE_PERIODS
// --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processCycleTimeFile(
    
     WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string pdf_PartName;
 float * ct;
 int result;
 int start, end;
 float theVal;
 long lineNo = 0;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "cycleTime",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 int t = 0;
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 while(inFile.peek() != EOF) {
   result = flatFileManager.readCsvPartFloatDatedRecord(theWitRun, nPeriods, pdf_PartName, theCal, theVal, 
							start, end, inFile, fileName, dataLine, lineNo,mpHelper);
   if (result == 1) {
     badCount++;        
     continue;
   }
   else if (result == -2) {
     commentCount++;
     continue;
   }
   
   // get the existing cycleTime for this part (it's in app data
   LgFrTimeVecFloat cycleTimeDays = mpHelper.getCycleTimeDays(theWitRun, pdf_PartName);
   LgFrTimeVecFloat cycleTime = mpHelper.getCycleTime(theWitRun, pdf_PartName);

   for (t=start; t<=end; t++) {
      cycleTimeDays[t] = theVal;
      cycleTime[t] = mpHelper.calculateVariableOffset(cycleTimeDays[t], t, theCal, nPeriods);
   }
   
   // Comment this out when ready to ditch wit34
   // witSetPartCycleTime(theWitRun, pdf_PartName.c_str(), ct);

   // set cycleTimeDays and CycleTime in AppData
   mpHelper.setCycleTimeDays(theWitRun, pdf_PartName, cycleTimeDays);
   mpHelper.setCycleTime(theWitRun, pdf_PartName, cycleTime);

#if 0
   std::cout << "CycleTimeDays for [" << pdf_PartName << "] = (";
   for (t=0; t<nPeriods; t++) 
     std::cout << cycleTimeDays[t] << ",";
   std::cout << std::endl;

   std::cout << "CycleTime for [" << pdf_PartName << "] = (";
   for (t=0; t<nPeriods; t++) 
     std::cout << cycleTime[t] << ",";
   std::cout << std::endl;
#endif

   //   witFree(ct);
   recCount++;
   if (myScrubber -> scrubMode() && outFile.good())        {
     outFile << dataLine << std::endl;
   }
 }
 
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}





#else
// the OLD WAY
// --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processCycleTimeFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string pdf_PartName;
 float * ct;
 int result;
 int start, end;
 float theVal;
 long lineNo = 0;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "cycleTime",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);
 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 while(inFile.peek() != EOF) {
    result = flatFileManager.readCsvPartFloatDatedRecord(
	theWitRun, nPeriods, pdf_PartName, theCal, theVal, 
	start, end, inFile, fileName, dataLine, lineNo,mpHelper);
    if (result == 1) {
       badCount++;        
       continue;
    }
    else if (result == -2) {
       commentCount++;
       continue;
    }

    witGetPartCycleTime(theWitRun, pdf_PartName.c_str(), &ct);

    int t = 0; // Pulled out of the for below by RW2STL
    for (t=start; t<=end; t++) {
       ct[t] = theVal/theCal.workUnits(t);
       }
    witSetPartCycleTime(theWitRun, pdf_PartName.c_str(), ct);
    witFree(ct);
//    std::cout << "Updated cycleTimes for " << pdf_PartName << "\n";
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
 }


 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 outFile.close();
 
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}
#endif

//  --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processYieldFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string pdf_PartName;

 // YIELD_RATE
 //   int * yield;
 float * yieldRate;
 int result;
 int start, end;
 float theVal;
 long lineNo = 0;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "yield",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 while(inFile.peek() != EOF) {

    result = flatFileManager.readCsvPartFloatDatedRecord(
	theWitRun, nPeriods, pdf_PartName, theCal, theVal, 
	start, end, inFile, fileName, dataLine, lineNo, mpHelper);

    if (result == 1) {
       badCount++; 
       continue;
    }
    else if (result == -2) {
       commentCount++;
       continue;
    }
    
    // YIELD_RATE
    witGetOperationYieldRate(theWitRun, pdf_PartName.c_str(), &yieldRate);
    //    witGetPartYield(theWitRun, pdf_PartName.c_str(), &yield);

    int t = 0;
    for (t=start; t<=end; t++) {
      // YIELD_RATE
       yieldRate[t] = theVal/100.0;
       //       yield[t] = theVal;
    }
    witSetOperationYieldRate(theWitRun, pdf_PartName.c_str(), yieldRate);
    //    witSetPartYield(theWitRun, pdf_PartName.c_str(), yield);

    //    witFree(yield);
    witFree(yieldRate);
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
 }

 inFile.close();
 outFile.close();
 
 std::cout << recCount << " records processed." << std::endl;

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

//  --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processOperationYieldFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string pdf_OperationName;
 //   int * yield;
 float * yieldRate;
 int result;
 int start, end;
 float theVal;
 long lineNo = 0;
 std::string dataLine;
 
 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "operation yield",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 while(inFile.peek() != EOF) {

    result = flatFileManager.readCsvOperationFloatDatedRecord(
	theWitRun, nPeriods, pdf_OperationName, theCal, theVal, 
	start, end, inFile, fileName, dataLine,lineNo, mpHelper);

    if (result == 1) {
       badCount++; 
       continue;
    }
    else if (result == -2) {
       commentCount++;
       continue;
    }

    //    witGetOperationYield(theWitRun, pdf_OperationName.c_str(), &yield);
    witGetOperationYieldRate(theWitRun, pdf_OperationName.c_str(), &yieldRate);

    int t = 0; // Pulled out of the for below by RW2STL
    for (t=start; t<=end; t++) {
      // yield[t] = (int) theVal;
      yieldRate[t] = theVal/100.0;
    }
    //    witSetOperationYield(theWitRun, pdf_OperationName.c_str(), yield);
    witSetOperationYieldRate(theWitRun, pdf_OperationName.c_str(), yieldRate);
    //    witFree(yield);
    witFree(yieldRate);
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
      
    }

 inFile.close();
 outFile.close();
 std::cout << recCount << " records processed." << std::endl;

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}



//  --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processLotSizeFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string pdf_PartName;
 float * minLotSize;
 float * incLotSize;
 int result;
 int start, end;
 float theVal, theVal2;
 long lineNo = 0;
 std::string dataLine;

 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "lot size",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 while(inFile.peek() != EOF) {

    result = flatFileManager.readCsvPart2FloatsDatedRecord(
	theWitRun, nPeriods, pdf_PartName, theCal, theVal, theVal2, 
	start, end, inFile, fileName, dataLine,lineNo, mpHelper);

    if (result == 1) {
       badCount++; 
       continue;
    }
    else if (result == -2) {
       commentCount++;
       continue;
    }

    witGetPartMinLotSize(theWitRun, pdf_PartName.c_str(), &minLotSize);
    witGetPartIncLotSize(theWitRun, pdf_PartName.c_str(), &incLotSize);
    
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=start; t<=end; t++) {
       minLotSize[t] = theVal;
       incLotSize[t] = theVal2;
       }
    witSetPartMinLotSize(theWitRun, pdf_PartName.c_str(), minLotSize);
    witSetPartIncLotSize(theWitRun, pdf_PartName.c_str(), incLotSize);
    witFree(minLotSize);
    witFree(incLotSize);
//    std::cout << "Updated lot sizes for " << pdf_PartName << "\n";
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
      
    }

 inFile.close();
  outFile.close();
 std::cout << recCount << " records processed." << std::endl;

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}



//  --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processOperationLotSizeFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string pdf_OperationName;
 float * minLotSize;
 float * incLotSize;
 int result;
 int start, end;
 float theVal, theVal2;
 long lineNo = 0;
 std::string dataLine;
 
 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "operation lot size",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 while(inFile.peek() != EOF) {

    result = flatFileManager.readCsvOperation2FloatsDatedRecord(
	theWitRun, nPeriods, pdf_OperationName, theCal, theVal, theVal2, 
	start, end, inFile, fileName, dataLine, lineNo, mpHelper);

    if (result == 1) {
       badCount++; 
       continue;
    }
    else if (result == -2) {
       commentCount++;
       continue;
    }

    witGetOperationMinLotSize(theWitRun, pdf_OperationName.c_str(), &minLotSize);
    witGetOperationIncLotSize(theWitRun, pdf_OperationName.c_str(), &incLotSize);
    
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=start; t<=end; t++) {
       minLotSize[t] = theVal;
       incLotSize[t] = theVal2;
       }
    witSetOperationMinLotSize(theWitRun, pdf_OperationName.c_str(), minLotSize);
    witSetOperationIncLotSize(theWitRun, pdf_OperationName.c_str(), incLotSize);
    witFree(minLotSize);
    witFree(incLotSize);
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
 }

 inFile.close();
  outFile.close();
 std::cout << recCount << " records processed." << std::endl;

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}



//  --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processOperationExecPenaltyFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string pdf_OperationName;
 float execPenalty;
 int result;
 long lineNo = 0;
 std::string dataLine;
 
 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "operation exec penalty",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 while(inFile.peek() != EOF) {

   execPenalty = 0.0;

    result = flatFileManager.readCsvOperationFloatRecord(
	theWitRun, nPeriods, pdf_OperationName, theCal, execPenalty,
	inFile, fileName, dataLine, lineNo, mpHelper);

    if (result == 1) {
       badCount++; 
       continue;
    }
    else if (result == -2) {
       commentCount++;
       continue;
    }

    witSetOperationExecPenalty(theWitRun, pdf_OperationName.c_str(), execPenalty);
    
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
 }

 inFile.close();
  outFile.close();
 std::cout << recCount << " records processed." << std::endl;

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

//  --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processOperationIntegerExecFile(
							  WitRun * const theWitRun,   
							  const int nPeriods,         
							  const LgFrCalendar & theCal,
							  const std::string & fileName,
							  LgFrMultiPlantHelper & mpHelper)
{
 std::string pdf_OperationName;
 float integerExecVal;
 int result;
 long lineNo = 0;
 std::string dataLine;
 
 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "operation integer exec vols",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 while(inFile.peek() != EOF) {

   integerExecVal = 0.0;

   result = flatFileManager.readCsvOperationFloatRecord(
							theWitRun, nPeriods, pdf_OperationName, theCal, integerExecVal,
							inFile, fileName, dataLine, lineNo, mpHelper);
   
    if (result == 1) {
       badCount++; 
       continue;
    }
    else if (result == -2) {
       commentCount++;
       continue;
    }

    if (integerExecVal > 0.50)
      witSetOperationIntExecVols(theWitRun, pdf_OperationName.c_str(), WitTRUE);
    
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
	outFile << dataLine << std::endl;
      }
 }
 
 inFile.close();
 outFile.close();
 std::cout << recCount << " records processed." << std::endl;
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

//  --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processOperationExecCostFile(
    
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 std::string pdf_OperationName;
 float * execCost;
 int result;
 int start, end;
 float theVal;
 long lineNo = 0;
 std::string dataLine;
 
 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

 std::ifstream inFile;
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName,
				       "operation execCost",
				       inFile,
				       outFile,
				       myScrubber,
				       theErrFacil);

 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 while(inFile.peek() != EOF) {

    result = flatFileManager.readCsvOperationFloatDatedRecord(
	theWitRun, nPeriods, pdf_OperationName, theCal, theVal, 
	start, end, inFile, fileName, dataLine,lineNo, mpHelper);

    if (result == 1) {
       badCount++; 
       continue;
    }
    else if (result == -2) {
       commentCount++;
       continue;
    }

    witGetOperationObj1ExecCost(theWitRun, pdf_OperationName.c_str(), &execCost);

    int t = 0; // Pulled out of the for below by RW2STL
    for (t=start; t<=end; t++) {
       execCost[t] = theVal;
       }
    witSetOperationObj1ExecCost(theWitRun, pdf_OperationName.c_str(), execCost);
    witFree(execCost);
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
      {
                outFile << dataLine << std::endl;
      }
      
    }

 inFile.close();
 outFile.close();
 std::cout << recCount << " records processed." << std::endl;

 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}


// -----------------------------------------------------------------
int 
LgFrInitialProblemForSce::setTheCalendar(
    LgFrCalendar & theCal)

{
    int numPeriods;
    int period;
    long pd;

    double wu;
    char dataDelim[] = " \",\n";  

    LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
    LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();   
    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    numPeriods = setOfParameters.getInt("numPeriods");

    std::string fileName = setOfParameters.getString("calendarFile");

    std::ifstream inFile;
    std::ofstream outFile;
    LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();

    this->openInputFileAndScrubOutputFile(fileName,
					  "Calendar file",
					  inFile,
					  outFile,
					  myScrubber,
					  theErrFacil);

    long recCount = 0;
    long badCount = 0;
    long commentCount = 0;
    long lineNo = 0;
   
    int t = 0;
    while(inFile.peek() != EOF) {

	std::string dataLine;
	std::string token;


	std::getline( inFile, dataLine );
	lineNo++;

        //  skip blanks or comments *
	dataLine = sce::strip( dataLine );
	if ((dataLine.empty()) || (dataLine[0] == '*')) {
	    commentCount++;
	    continue;
        }

        // check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() 
			 << fileName 
			 << (int)lineNo 
			 << dataLine);
        }

        
	SCETokenizer next(dataLine);

        // get the period
	token = next(dataDelim);

        // make sure period number is valid
    if (! sce::fromString(token, pd))  {
	  (*theErrFacil)("CalendarPeriod",MclArgList() 
			 << fileName 
			 << (int)lineNo 
			 << dataLine 
			 << "Invalid period number in the calendar file.");
	}
	period = (int) pd;

	if (period != t) {
	  (*theErrFacil)("CalendarPeriod",MclArgList() 
			 << fileName 
			 << (int)lineNo 
			 << dataLine 
			 << "The periods are out of sequence.");
	}

// Note: you must specifiy a "startDate" for the numPeriods bucket.
// This essentially gives us an ending date for bucket numPeriods-1
	if (period > numPeriods) {
	  (*theErrFacil)("CalendarPeriod",MclArgList() 
			 << fileName 
			 << (int)lineNo 
			 << dataLine 
			 << "Too many periods. Check the 'numPeriods' parameter in the parameter file.");
	}

// read the period start date and make sure its ok
	if ((token = next(dataDelim)).empty())    {
	  (*theErrFacil)("InvalidDataWarning",MclArgList() 
			 << "calendar record" 
			 << fileName 
			 << (int)lineNo 
			 << dataLine 
			 << "The date is missing.");
	}


	LgFrDatetime startDate(flatFileManager.parseDateString(token, theCal));
	if (! startDate.isValid())  {
	  // error
	  (*theErrFacil)("InvalidCalendarDate",MclArgList() 
			 << fileName 
			 << (int)lineNo 
			 << dataLine);
	}
	
// Set the period start date in the calendar
	if (period < numPeriods)
	  theCal.periodStart(period, startDate);
	else 
	  theCal.horizonEnd(startDate);

// Test the sequential-ness of the calendar now
	if (period > 0  && period < numPeriods
	    && (theCal.periodStart(period-1) >= theCal.periodStart(period))) {
	  (*theErrFacil)("InvalidDataWarning",MclArgList() 
			 << "period start date in Calendar File" 
			 << fileName 
			 << (int)lineNo 
			 << dataLine 
			 << "Date is out of sequence with previous period.");
	}

// increment the period counter 
// we want to enforce that they enter every period
	t++;

// read the workUnits field.  
        // If this is the last Period, then skip it.
	if (period == numPeriods) {
	    recCount++;
            if (myScrubber -> scrubMode() && outFile.good())
              {
                outFile << dataLine << std::endl;
              }
          
	    continue;
	}

	// see if workunits is there
	if ((token = next(dataDelim)).empty())    {
	    theCal.workUnits(period, 20.0);
	    (*theErrFacil)("CalendarWorkUnits",MclArgList() 
			   << period 
			   << fileName 
			   << (int)lineNo 
			   << dataLine);
	}

	// is it a valid number
	else if (! sce::fromString(token, wu))  {
	  (*theErrFacil)("InvalidDataWarning",MclArgList() 
			 << "workUnits field in Calendar File" 
			 << fileName 
			 << (int)lineNo 
			 << dataLine 
			 << "Default of 20.0 will be used");
	  theCal.workUnits(period, 20.0);
	}

	else if (wu <= -0.0001) {
	  (*theErrFacil)("InvalidDataWarning",MclArgList() 
			 << "workUnits field in Calendar File" 
			 << fileName 
			 << (int)lineNo 
			 << dataLine 
			 << "Default of 20.0 will be used.");
	    theCal.workUnits(period, 20.0);
	}

	else if (wu < 0.0001) {
	  (*theErrFacil)("InvalidDataWarning",MclArgList() 
			 << "workingDays is zero. in Calendar File" 
			 << fileName 
			 << (int)lineNo 
			 << dataLine 
			 << "This is informational message.  Zero will be used but possibly may cause unexpected behavior.");
	    theCal.workUnits(period, 0.0);
	}


	else 
	    theCal.workUnits(period, (float) wu);
    

	recCount++;
        if (myScrubber -> scrubMode() && outFile.good())
          {
            outFile << dataLine << std::endl;
          }
          
    }

    if (period < numPeriods) {
      (*theErrFacil)("CalendarPeriod",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << "\n"
		     <<  "Not enough periods specified. Remember to specify a start date for\n period numPeriods (this is the end date of the last period).");
	}

    inFile.close();
    outFile.close();
    flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);

    return numPeriods;
}


// tommy come here


// ====================================
// SCE 6.1: 
// ====================================
// Finish Me:   For now we will assume that records are DATED   
// ----------------------------------------------------------------
void 
LgFrInitialProblemForSce::processDemandFile(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

 int nElementsChanged;
 std::string partNumber; //mfgPart;
 std::string sourceLoc; // pdf
 std::string fullWitPart;
 std::string demandName;
 std::ifstream inFile;
 long lineNo = 0;
 std::string dataLine;
 int t = 0; 
 
 LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
 MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
 LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();   

 int defaultIndependentDemandPriority = setOfParameters.getInt("defaultIndependentDemandPriority");
 LgFrTimeVecFloat tvDefaultPriority(nPeriods, (float) (1000000000 - defaultIndependentDemandPriority));

 int demandTypeKeyFieldNum = setOfParameters.getInt("demandTypeKeyFieldNum");
 bool useDatedDemands = setOfParameters.getBool("useDatedDemands");
 float theDatedDemandVol = 0.0f;
 int thePeriod = 0;
 std::string requestDate;

 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName, "Demand Volume",
				       inFile, outFile, myScrubber, theErrFacil);

 
 std::cout << "Processing (6.1) " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;

 float * floatVec = new float[nPeriods];
 
 while(inFile.peek() != EOF) {
   int result = 0;
   std::getline( inFile, dataLine );
   lineNo ++;
   dataLine = sce::strip( dataLine );
   if ((dataLine[0] == '*') || dataLine.empty()) { commentCount++; continue; }
   SCETokenizer next(dataLine);
   if ( flatFileManager.containsIllegalCharacters( dataLine )) {
     (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
   }
  
   // get the partname
   partNumber = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);

   // get the sourceLoc (pdf)
   sourceLoc = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);

   // get the demandName
   demandName = flatFileManager.nextQuotedDemandString(next, result, fileName, dataLine, lineNo, MANDATORY);

   // get the demandQty and requestPeriod (need requestDate perhaps??)


   int requestPeriod = 0;
   float demandQty = 0.0;
   bool dieIfErrorFlag = true;
   result = flatFileManager.readDatedDemandFloatRecord( demandQty, requestDate, 
							dieIfErrorFlag, -FLT_MAX, FLT_MAX,
							next, fileName, dataLine, lineNo, 
							mpHelper, theCal);
   

  


   // determine Demand_Type ... must be one of: DEMAND_TYPE_ERROR, DEMAND_TYPE_INDP, DEMAND_TYPE_TINP 
   int Demand_Type = flatFileManager.demandType(demandName);


   // **** TINP ****
   if (Demand_Type == DEMAND_TYPE_TINP) {
     addGeoSourceAndDemandVolFromTINP(theWitRun, dataLine, 
				      partNumber, demandName, sourceLoc, requestDate, demandQty, 
				      fileName, lineNo, theCal, mpHelper);
   }
   
   else if  (Demand_Type == DEMAND_TYPE_INDP) { 
     addDemandVolFromINDP(theWitRun, dataLine, nPeriods,
			  partNumber, demandName, sourceLoc, requestDate, demandQty, 
			  fileName, lineNo, theCal, mpHelper);
   }
   
   // **** INDP ****
   else if  (Demand_Type == DEMAND_TYPE_ERROR) { 
       ;
     // print a message and reject or assume INDP(?)
   }

   else {
     // should never come here
     assert(Demand_Type == DEMAND_TYPE_ERROR);
   }

   recCount++;
   if (myScrubber -> scrubMode() && outFile.good())
     {
       outFile << dataLine << std::endl;
     }
 }
 
 // delete [] defaultPriorityVec;
 delete [] floatVec;
 std::cout << recCount << " records processed." << std::endl;
 inFile.close();
 
 
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}

// --------------------------------------------------------------------- 
// SCE 6.1
int 
LgFrInitialProblemForSce::addDemandVolFromINDP(WitRun * const theWitRun,
					       const std::string &  dataLine,
					       const int nPeriods,
					       const std::string &  partNumber,
					       const std::string &  demandName,
					       const std::string &  sourceLoc,
					       const std::string & requestDate,
					       float demandQty,
					       const std::string & fileName,
					       const long  lineNo,
					       const LgFrCalendar & theCal,
					       LgFrMultiPlantHelper & mpHelper)
{

  std::string fullWitPart;
  int t = 0; 
 
  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
  LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();   
  int defaultIndependentDemandPriority = setOfParameters.getInt("defaultIndependentDemandPriority");
  LgFrTimeVecFloat tvDefaultPriority(nPeriods, (float) (1000000000 - defaultIndependentDemandPriority));
  int defaultShipLateLimit = heurModelHelper_->defaultShipLateLimit();
 
  // create a witPartname
  fullWitPart = mpHelper.pdfPartname(partNumber, sourceLoc);
 
  int readCsvMessage = OPTIONAL_WITH_MESSAGE;
  if (! mpHelper.isPartValid( theWitRun, fullWitPart, fileName, dataLine, lineNo, PURELY_OPTIONAL))  {
    (*theErrFacil)("IndDemandedPartNotExist",MclArgList() 
		   << partNumber << sourceLoc << demandName << fileName << (int)lineNo << dataLine);
    return 0;
  }


  // see what period it falls in
  // NOTE: this method uses period() ... which adhere's to strict calendar inclusive dates
  // NOTE: you can also use roundedPeriod() 
  int requestPeriod = theCal.period(requestDate);  
  if (requestPeriod >= nPeriods)  {
    (*theErrFacil)("DatedRecordBeyondHorizonEndIgnore",MclArgList() 
		   << fileName << (int)lineNo << dataLine);
    return 0;
  }
  if (requestPeriod < 0)  {
    (*theErrFacil)("DatedRecordBeforeHorizonStartAccept",MclArgList() 
		   << fileName << (int)lineNo << dataLine);
    requestPeriod = 0;
  }
    
  
  
  if (! doesDemandExist(theWitRun, fullWitPart, demandName)) {
    // regardless of engine, load the priorities and shipLateLimit
    // get the default priority
    witAddDemand( theWitRun, fullWitPart.c_str(), demandName.c_str() );
    // create a demand app data for this demand.  Set the hasDemandVolBeenSet=0
    // init backlogYield to 1.0
    LgFrSceDemandAppData * demandAppDataPtr = new LgFrSceDemandAppData (0, 0, (size_t) nPeriods, 1.0);  
    demandAppDataPtr->pipPriority(tvDefaultPriority);
    //    std::cout << "Adding indp demand with default pipPiority [" 
    //	      << fullWitPart << "," << demandName << "] =" << (int) tvDefaultPriority[2] << std::endl;
    witSetDemandAppData(theWitRun, fullWitPart.c_str(), demandName.c_str(), (void *) demandAppDataPtr);
    
    witSetDemandPriority(theWitRun, fullWitPart.c_str(),
			 demandName.c_str(), tvDefaultPriority.intData() );
    witSetDemandShipLateLimit(theWitRun, fullWitPart.c_str(),
			      demandName.c_str(), defaultShipLateLimit );
    
    
    // but only load the obj1 values if we have too      
    if ( lpModelHelper_ != 0 ) {
      // get the default Obj1 Ship Reward
      float defaultObj1ShipReward = lpModelHelper_->defaultObj1ShipReward();
      
      // get the periodic Interest Rate
      float periodicInterestRate = lpModelHelper_->periodicInterestRate();
      
      float * defaultObj1ShipRewardVec = new float[nPeriods];
      float * defaultObj1CumShipRewardVec = new float[nPeriods];
      
      // make time vecs from scalars
      for (t=0; t<nPeriods; t++) {
	defaultObj1ShipRewardVec[t] = defaultObj1ShipReward;
	defaultObj1CumShipRewardVec[t] = defaultObj1ShipReward * 
	  periodicInterestRate;
      }
      
      witSetDemandObj1ShipReward( theWitRun, fullWitPart.c_str(),
				  demandName.c_str(), defaultObj1ShipRewardVec);
      witSetDemandObj1CumShipReward( theWitRun, fullWitPart.c_str(),
				     demandName.c_str(), defaultObj1CumShipRewardVec);
      delete [] defaultObj1ShipRewardVec;
      delete [] defaultObj1CumShipRewardVec;
    }
  }
  

  LgFrSceDemandAppData * demandAppDataPtr;
  witGetDemandAppData(theWitRun, fullWitPart.c_str(), demandName.c_str(), (void **) &demandAppDataPtr);
  assert(demandAppDataPtr != 0);
  LgFrTimeVecFloat demandVolTVF = demandAppDataPtr->demandVol();
  int hasDemandVolBeenSet = demandAppDataPtr->hasDemandVolBeenSet();

  // increment the demandVol timeVec for wit
  demandVolTVF[requestPeriod] += demandQty;


  // store away the original demandVol (with negatives)
  demandAppDataPtr->demandVol(demandVolTVF);
  demandAppDataPtr->hasDemandVolBeenSet(1);

  // DEMAND2ORDER
  // save the request date
  demandAppDataPtr->requestDate(requestDate);
  



  // ENABLE_NEGATIVE_DEMANDS
  // must strip out any negative demandVol qty's before feeding to WIT
  // FINISH_ME: we can be smarter here.  Directly interrogate the qty as we get it.
  //            if it's negative do something with it directly. ?!?!?
  // FINISH_ME: How do we detect if multiple demandVol recs remove negatives?
  // Perhaps we should move the setting of doesPartHaveNegativeDemands to a
  // pre processing step
  for ( t=0; t<nPeriods; t++) {
    if (demandVolTVF[t] < 0.0) {
      demandVolTVF[t] = 0.0;
    }
  }
  witSetDemandDemandVol(theWitRun, fullWitPart.c_str(), demandName.c_str(), demandVolTVF.data());

  return 1;
}
 

// --------------------------------------------------------------------- 
// time for the big Merge ... come here when you detect a TINP demand Vol
// this sets up the GPD bom structure
int 
LgFrInitialProblemForSce::addGeoSourceAndDemandVolFromTINP(
					       WitRun * const theWitRun,
					       const std::string &  dataLine,
					       const std::string &  plannerPartName,
					       const std::string &  geoPreD20,
					       const std::string &  pdf,
					       const std::string & requestDate,
					       float demandQty,
					       const std::string & fileName,
					       const long  lineNo,
					       const LgFrCalendar & theCal,
					       LgFrMultiPlantHelper & mpHelper)
{

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  int nPeriods = theCal.nPeriods();

  LgFrSceSetOfParameters & setOfParameters = this->sceScenario()->setOfParameters();   

  std::string engine = setOfParameters.getString("engine");
    
  long numAddedParts = 0; // this is the return value

  int early = 0;
  int late = nPeriods - 1;
  float transitTime = 0.0;
  int result = 0;

  float * demandVol;

  
  // see what period it falls in
  // NOTE: this method uses period() ... which adhere's to strict calendar inclusive dates
  // NOTE: you can also use roundedPeriod() 

  // ++++ >>>  Let's see, how many ways can we read a date?
  //  - Straight from calendar.   Maybe we need an SCE calendar manager who sets it up.
  //  - 
  // 
  

  //            ==============================
  //            !!!!  DATE CONSOLIDATION   !!! 
  //            ==============================
  // Things to think about when reading dates:
  //   1. which period do you map it to when it's in the middle somewhere (deal with this in BOM)
  //   2. Dates that fall earlier/later than horizon.
  //   3. LgFrDateTime might be good object to use ... can it be portable to granularity less than days??
  //   4. 
  // SCE Date Methods: 
  //       int requestPeriod = theCal.period(requestDate);  
  //            used by: processSupplyFile


  // see what period it falls in
  // NOTE: this method uses period() ... which adhere's to strict calendar inclusive dates
  // NOTE: you can also use roundedPeriod() 
  int requestPeriod = theCal.period(requestDate);  
  if (requestPeriod >= nPeriods)  {
    (*theErrFacil)("DatedRecordBeyondHorizonEndIgnore",MclArgList() 
		   << fileName << (int)lineNo << dataLine);
    return 0;
  }
  if (requestPeriod < 0)  {
    (*theErrFacil)("DatedRecordBeforeHorizonStartIgnore",MclArgList() 
		   << fileName << (int)lineNo << dataLine);
    return 0;
  }



  // D2O 
  // Append the RequestDate onto the demand key to enforce period uniqueness
  std::string geo = mpHelper.expandDemandKeyforD2O(geoPreD20, requestDate, pdf);
  
  
  // make valid wit part numbers for these dudes
  std::string geoPlannerDemandPartName = mpHelper.geoPlannerDemandPartName(plannerPartName, geo);
  std::string pdfPartname = mpHelper.pdfPartname(plannerPartName, pdf);
  
  // first check to see if the child exists  
  // Note this routine need s to be finished and extended to
  // allow for DEFAULT  sourcing rules
  if (! mpHelper.isPartValid(theWitRun, pdfPartname, 
			     fileName, dataLine, lineNo, PURELY_OPTIONAL))  {
    (*theErrFacil)("UnknownMfgPartPDFPair",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << dataLine);
    return 0;
  }


  
  
  // Now analyze the Demand,GEO.  
  // If a "part" already exists then ok,
  // otherwise, create a part and add it to the WIT Run.
  int geoPlannerPartAlreadyExist = mpHelper.isPartValid(theWitRun, 
							geoPlannerDemandPartName, 
							fileName, dataLine, lineNo, PURELY_OPTIONAL);
  
  if (! geoPlannerPartAlreadyExist)  {
    result = sceAddPartToRepresentDemand(theWitRun, geoPlannerDemandPartName, mpHelper, 
					 fileName, dataLine, lineNo);
    numAddedParts++;
    
    if (engine == "optimization") {
      // Only need to do this when using optimization          
      std::string geoPlannerDemandUbPartName = mpHelper.geoPlannerDemandUbPartName(geoPlannerDemandPartName, geo);
      
      result = sceAddPartToRepresentDemandUpperBound(theWitRun, geoPlannerDemandUbPartName, mpHelper, 
						     fileName, dataLine, lineNo);
      
      result = sceAddDemandUbBomStructure(theWitRun, mpHelper,
					  geoPlannerDemandPartName, geoPlannerDemandUbPartName,
					  fileName, dataLine, lineNo);
    }
    
  }
  
  // Now add a bill record from the the dummy demand part 
  // to the real part
  // taking into account the effectivity
  sceAddDemandBomStructure(theWitRun, mpHelper, theCal,
			   geoPlannerDemandPartName, pdfPartname,
			   early, late, transitTime,
			   fileName, dataLine, lineNo);


  // +++++++++++++++++++++
  int doesTheGpd_DEMAND_Exist = doesDemandExist(theWitRun, geoPlannerDemandPartName, geo);

  if (! doesTheGpd_DEMAND_Exist) {
    // regardless of engine, load the priorities and shipLateLimit
    // get the default priority
    int defaultPriority = heurModelHelper_->defaultPriority();
    int   * defaultPriorityVec = new int[nPeriods];
    int t=0;
    for (t=0; t<nPeriods; t++) {
      defaultPriorityVec[t] = 1000000000 - (int) defaultPriority;
    }
    // get the default shipLateLimit
    int defaultShipLateLimit = heurModelHelper_->defaultShipLateLimit();
    witAddDemand( theWitRun, geoPlannerDemandPartName.c_str(), geo.c_str() );
    // create a default demand app data for this demand, with  hasDemandVolBeenSet=0 and  backlogYield= 1.0
    LgFrSceDemandAppData * demandAppDataPtr = new LgFrSceDemandAppData (0, 0, (size_t) nPeriods, 1.0);  
    witSetDemandAppData(theWitRun,
			geoPlannerDemandPartName.c_str(), 
			geo.c_str(), 
			(void *) demandAppDataPtr);
    
    witSetDemandPriority( theWitRun, geoPlannerDemandPartName.c_str(),
			  geo.c_str(), defaultPriorityVec );
    witSetDemandShipLateLimit( theWitRun, geoPlannerDemandPartName.c_str(),
			       geo.c_str(), defaultShipLateLimit );
    delete [] defaultPriorityVec;
    
    // but only load the obj1 values if we have too      
    if ( lpModelHelper_ != 0 ) {
      // get the default Obj1 Ship Reward
      float defaultObj1ShipReward = lpModelHelper_->defaultObj1ShipReward();
      
      // get the periodic Interest Rate
      float periodicInterestRate = lpModelHelper_->periodicInterestRate();
      
      float * defaultObj1ShipRewardVec = new float[nPeriods];
      float * defaultObj1CumShipRewardVec = new float[nPeriods];
      
      // make time vecs from scalars
      for (t=0; t<nPeriods; t++) {
	defaultObj1ShipRewardVec[t] = defaultObj1ShipReward;
	defaultObj1CumShipRewardVec[t] = defaultObj1ShipReward * 
	  periodicInterestRate;
      }
      
      witSetDemandObj1ShipReward( theWitRun, geoPlannerDemandPartName.c_str(),
                                    geo.c_str(), defaultObj1ShipRewardVec);
      witSetDemandObj1CumShipReward( theWitRun, geoPlannerDemandPartName.c_str(),
				     geo.c_str(), defaultObj1CumShipRewardVec);
      delete [] defaultObj1ShipRewardVec;
      delete [] defaultObj1CumShipRewardVec;
    }
  }
  // +++++++++++++++++++++
    
  witGetDemandDemandVol(theWitRun, geoPlannerDemandPartName.c_str(), 
			geo.c_str(), &demandVol);

  // get the  demand app data for this demand.  get the hasDemandVolBeenSet
  LgFrSceDemandAppData * demandAppDataPtr;
  witGetDemandAppData(theWitRun,
		      geoPlannerDemandPartName.c_str(), 
		      geo.c_str(),
		      (void **) &demandAppDataPtr);
  assert(demandAppDataPtr != 0);
  int hasDemandVolBeenSet = demandAppDataPtr->hasDemandVolBeenSet();
  
  
  // note: duplicate demandVol records are incremental!!
  // print a warning when detected
  demandVol[requestPeriod] += demandQty;


  if (hasDemandVolBeenSet) {
    std::string pnm = mpHelper.partname(geoPlannerDemandPartName);
    std::string pdfff = mpHelper.pdf(geoPlannerDemandPartName);
    (*theErrFacil)("MultipleDemandVolRecs",MclArgList() 
		   << fileName 
		   << (int) lineNo 
		   << pnm 
		   << pdfff);
  }
  
  witSetDemandDemandVol(theWitRun, geoPlannerDemandPartName.c_str(), geo.c_str(), demandVol);
  // set the hasDemandVolBeenSet to true
  demandAppDataPtr->hasDemandVolBeenSet(1);

  // DEMAND2ORDER
  // save the request date
  demandAppDataPtr->requestDate(requestDate);
  
  witFree(demandVol);

  return 1;

}






// ---------------------------------------------------------
void
LgFrInitialProblemForSce::process61TopLevelToOptionalFeatureFile(
    
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

  // This method just reads the file and passes all logic to either
  LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();   
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  int nPeriods = theCal.nPeriods();
  int result;

  // Note, we'll be setting negative production costs which
  // causes a Wit warning message. Suppress it.
  witSetMesgTimesPrint(theWitRun, WitTRUE, 605, 0);


  // attributes to read off file
  std::string dataLine;

  std::string plannerTopLevelPart;
  std::string sourceLoc;
  std::string geoPreD20;
  std::string mfgOptionPart;
  std::string requestDate;
  float       usageRate;
  float       attachRate;
  float       mwoVal;
  float       mfOffsetVal;
  int         mfoPhantomFlag;
  float       mfSupplyVal;
  int         earlyPeriod;
  int         latePeriod;
  
  
  std::string geo;  
  int requestPeriod;  
  
  float * defaultGatingOptionSubPenalty = new float[nPeriods];
  
  LgFrSceSetOfParameters & setOfParameters = this->sceScenario()->setOfParameters();
    
  float defaultOptionShipReward = setOfParameters.getFloat("defaultOptionShipReward");
  
  int tt = 0;
  for (tt=0; tt<nPeriods; tt++)
    defaultGatingOptionSubPenalty[tt] = defaultOptionShipReward;

  std::ifstream inFile;
  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();
  
  this->openInputFileAndScrubOutputFile(fileName, "Top Level to Optional Feature", inFile, outFile, myScrubber, theErrFacil);
  
  std::cout << "Processing " << fileName << ":\t" << std::flush;
  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;
  long lineNo = 0;

  while(inFile.peek() != EOF) {

    // read the next record
    std::getline( inFile, dataLine );
    lineNo++;
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
      commentCount++;
      continue;
    }
    if ( flatFileManager.containsIllegalCharacters( dataLine )) {
      (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
    }
    SCETokenizer next(dataLine);

    plannerTopLevelPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) { badCount++; continue; }
    sourceLoc = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) { badCount++; continue; }
    geoPreD20 = flatFileManager.nextQuotedDemandString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) { badCount++; continue; }
    mfgOptionPart = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
    if (result == -1) { badCount++; continue; }

    // requestDate and usageRate
    bool dieIfErrorFlag = true;
    result = flatFileManager.readDatedDemandFloatRecord(usageRate, requestDate, dieIfErrorFlag, -FLT_MAX, FLT_MAX,
							next, fileName, dataLine, lineNo, mpHelper, theCal);
    

    // Map requestDate to requestPeriod
    // rule: reject early 
    // rule: reject late
    requestPeriod = theCal.period(requestDate);  
    if (requestPeriod >= nPeriods)  {
      (*theErrFacil)("DatedRecordBeyondHorizonEndIgnore",MclArgList() << fileName << (int)lineNo << dataLine);
      badCount++; continue; }
    if (requestPeriod < 0)  {
      (*theErrFacil)("DatedRecordBeforeHorizonStartIgnore",MclArgList() << fileName << (int)lineNo << dataLine);
      badCount++; continue; }


    // attachRate 
    result = flatFileManager.nextFloat(attachRate, next, fileName, dataLine, lineNo, false, 0.0);
    if (result == -1) { badCount++; continue; }
    // maxWithOut Value must  be between -1 and +1
    result = flatFileManager.nextFloat(mwoVal, next, fileName, dataLine, lineNo, false, -1.0, 1.0);
    if (result == -1) { badCount++; continue; }
    // mfOffset
    result = flatFileManager.nextFloat(mfOffsetVal, next, fileName, dataLine, lineNo, false, -FLT_MAX, FLT_MAX);
    if (result == -1) { badCount++; continue; }
    // featurePhantomFlag
    result = flatFileManager.nextInt(mfoPhantomFlag, next, fileName, dataLine, lineNo, false, 0, 1);
    if (result == -1) { badCount++; continue; }
    // mfSupply
    result = flatFileManager.nextFloat(mfSupplyVal, next, fileName, dataLine, lineNo, false, 0, FLT_MAX);
    if (result == -1) { badCount++; continue; }
    
    // effectivity Dates: earlyPeriod, latePeriod
    result = flatFileManager.nextStartEndDates(earlyPeriod, latePeriod, next, fileName, lineNo, dataLine, theCal);
    if (result == 1) { badCount++; continue; }

    // FINISH_ME_61: Is this the right rule ??
    if ((earlyPeriod == nPeriods) || (latePeriod < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() << "Effectivity dates" << fileName << (int) lineNo << dataLine << "Featratio entry ignored.");
      badCount++; continue; }


    // D2O 
    // Append the RequestDate onto the demand key to enforce period uniqueness
    geo = mpHelper.expandDemandKeyforD2O(geoPreD20, requestDate, sourceLoc);




    // Analyze maxWithout to determine what kind of standalone feature we have:
    // it must be one of:
    //     SCE_FEATURE_TYPE_CUSTOMER_CHOICE
    //     SCE_FEATURE_TYPE_PURELY_OPTIONAL
    //     SCE_FEATURE_TYPE_MANDATORY
    //     SCE_FEATURE_TYPE_MWO
    int featureType;
    if (mwoVal < -IPSCE_FLT_EPS) 
      featureType = SCE_FEATURE_TYPE_CUSTOMER_CHOICE;
    else if (mwoVal < IPSCE_FLT_EPS)
      featureType = SCE_FEATURE_TYPE_MANDATORY;
    else if (mwoVal < 1.0 - IPSCE_FLT_EPS)
      featureType = SCE_FEATURE_TYPE_MWO;
    else
      featureType = SCE_FEATURE_TYPE_PURELY_OPTIONAL;



    if (featureType == SCE_FEATURE_TYPE_CUSTOMER_CHOICE)
      result = sceAddCustChoiceFeatureStructure61(theWitRun, mpHelper, theCal,
						  plannerTopLevelPart, geo, mfgOptionPart,
						  usageRate, attachRate, 
						  mfOffsetVal, mfoPhantomFlag, mfSupplyVal,
						  earlyPeriod, latePeriod,
						  defaultGatingOptionSubPenalty, fileName, dataLine, lineNo);
      
  
    else       
      result = sceAddStdAloneOptionBomStructure61(theWitRun, mpHelper, theCal,
						  plannerTopLevelPart, geo, mfgOptionPart,
						  usageRate, attachRate, mwoVal,
						  mfOffsetVal, mfoPhantomFlag, mfSupplyVal,
						  defaultGatingOptionSubPenalty, 
						  earlyPeriod, latePeriod, featureType,
						  fileName, dataLine, lineNo);
   


      
    // increment the feature counter 
    if (! result)
      sceScenario()->incrementStandaloneFeatureArcs();


    if (! result)
      {
        recCount ++;
        if (myScrubber -> scrubMode() && outFile.good())
          {
            outFile << dataLine << std::endl;
          }
      }
    else
      badCount++;
  }

 std::cout << recCount << " records processed." << std::endl;

 inFile.close();
 outFile.close();
 flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);

 delete [] defaultGatingOptionSubPenalty;
 // turn message 605 back on.
 witSetMesgTimesPrint(theWitRun, WitTRUE, 605, UCHAR_MAX); 
 
}







// --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processGeoSourceFile(
					       WitRun * const theWitRun,
					       const std::string & geoSourceFile,
					       const LgFrCalendar & theCal,
					       LgFrMultiPlantHelper & mpHelper)
{
  LgFrSceFlatFileMgr & flatFileManager =
    this->sceScenario()->flatFileMgr();   

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  int nPeriods = theCal.nPeriods();
  // read source file 
  
  long lineNo = 0;

  LgFrSceSetOfParameters & setOfParameters = this->sceScenario()->setOfParameters();   
  // get the engine
  std::string engine = setOfParameters.getString("engine");
    
  std::string fileName(geoSourceFile);

  std::ifstream inFile;
  std::ofstream outFile;
  LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();

  inFile.open(fileName.c_str());
  if (! inFile)  {
    (*theErrFacil)("WarningCannotOpenFile",MclArgList() 
		   << "geoSourceFile" 
		   << fileName 
		   << "Will attempt to source parts to the default PDF.");
    return;
  }
  inFile.close();
    
  this->openInputFileAndScrubOutputFile(fileName,
					"geo source file",
					inFile,
					outFile,
					myScrubber,
					theErrFacil);

  std::cout << "Processing " << fileName << ":\t" << std::flush;
  long recCount = 0;
  long badCount = 0;
  long commentCount = 0;    
  long numAddedParts = 0; // this is to count parts added to wit run





  std::string dataLine;
  std::string plannerPartName;
  std::string geo;
  std::string partName;
  std::string pdf;
  int early = 0;
  int late = nPeriods - 1;
  float transitTime = 0.0;


 
  while(inFile.peek() != EOF) {
    int result = 0;
 
    // Read next line of file
    std::getline( inFile, dataLine );
    lineNo ++;

    //  skip blanks or comments '*'
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
      commentCount++;
      continue;
    }

    // check if the line contains any illegal characters
    if ( flatFileManager.containsIllegalCharacters( dataLine )) {
      (*theErrFacil)("IllegalCharacter",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
    }
        
    // construct a tokenizer
    SCETokenizer next(dataLine);

    // get the plannerPartName
    plannerPartName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, PURELY_OPTIONAL);
    if (result < 0) {
      badCount++;
      continue;
    }

    // get the geo 
    geo = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);

    // get the partName
    partName = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);

    // get the pdf
    pdf = flatFileManager.nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);

    // now get the effectivity dates
    result = flatFileManager.nextStartEndDates(early, late, next, fileName, lineNo, dataLine, theCal);
    if ((early == nPeriods) || (late < 0)) {
      (*theErrFacil)("OutOfRange",MclArgList() << "Effectivity dates" << fileName << (int)lineNo << dataLine << "entry ignored.");
      badCount++;
      continue;
    }

    // this is where i am



    flatFileManager.nextFloat(transitTime, next, fileName, dataLine, lineNo, false, -FLT_MAX);

#ifndef VARIABLE_PERIODS
    // scale the transit time ... (leave it unscaled with Variable Periods
    transitTime = transitTime / theCal.workUnits(early);
#endif
	
    // make valid wit part numbers for these dudes
    std::string geoPlannerDemandPartName = 
      mpHelper.geoPlannerDemandPartName(plannerPartName, geo);
    std::string pdfPartname = mpHelper.pdfPartname(partName, pdf);
	
    // first check to see if the child exists  
    // Note this routine need s to be finished and extended to
    // allow for DEFAULT  sourcing rules
    if (! mpHelper.isPartValid(theWitRun, pdfPartname, 
			       fileName, dataLine, lineNo, PURELY_OPTIONAL))  {
      (*theErrFacil)("UnknownMfgPartPDFPair",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
      badCount++;
      continue;
    }
    

    // Now analyze the Demand,GEO.  
    // If a "part" already exists then ok,
    // otherwise, create a part and add it to the WIT Run.

    int geoPlannerPartAlreadyExist = mpHelper.isPartValid(theWitRun, 
							  geoPlannerDemandPartName, 
							  fileName, dataLine, lineNo, PURELY_OPTIONAL);
    
    if (! geoPlannerPartAlreadyExist)  {
      result = sceAddPartToRepresentDemand(theWitRun, geoPlannerDemandPartName, mpHelper, 
					   fileName, dataLine, lineNo);
      numAddedParts++;

      if (engine == "optimization") {
	// Only need to do this when using optimization          
	std::string geoPlannerDemandUbPartName = mpHelper.geoPlannerDemandUbPartName(plannerPartName, geo);
            
	result = sceAddPartToRepresentDemandUpperBound(theWitRun, geoPlannerDemandUbPartName, mpHelper, 
						       fileName, dataLine, lineNo);
            
	result = sceAddDemandUbBomStructure(theWitRun, mpHelper,
					    geoPlannerDemandPartName, geoPlannerDemandUbPartName,
					    fileName, dataLine, lineNo);
      }
	  
    }
	
    // Now add a bill record from the the dummy demand part 
    // to the real part
    // taking into account the effectivity
    sceAddDemandBomStructure(theWitRun, mpHelper, theCal,
			     geoPlannerDemandPartName, pdfPartname,
			     early, late, transitTime,
			     fileName, dataLine, lineNo);
	
    recCount++;
    if (myScrubber -> scrubMode() && outFile.good())
	outFile << dataLine << std::endl;
  }

  inFile.close();
  outFile.close();
  std::cout << recCount << " records processed." << std::endl;

  flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,numAddedParts);
}




// --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::processOperationDefinitionFile(
    
    WitRun * const theWitRun,   
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

    std::string operationName;
    std::string pdf;
    std::string dataLine;

    LgFrSceFlatFileMgr & flatFileManager = this->sceScenario()->flatFileMgr();
    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    int result;

    std::ifstream inFile;
    std::ofstream outFile;
    LgFrSceScrubber * myScrubber = this->sceScenario()->scrubber();

    this->openInputFileAndScrubOutputFile(fileName,
					  "operation definition",
					  inFile,
					  outFile,
					  myScrubber,
					  theErrFacil);

    std::cout << "Processing " << fileName << ":\t" << std::flush;
    long recCount = 0;
    long badCount = 0;
    long commentCount = 0;

    long lineNo = 0;
    while(inFile.peek() != EOF) {
	std::getline( inFile, dataLine );
	lineNo++;

//  skip blanks or comments *
	dataLine = sce::strip( dataLine );
	if ((dataLine.empty()) || (dataLine[0] == '*')) {
	    commentCount++;
	    continue;
	}

// check if the line contains any illegal characters
        if ( flatFileManager.containsIllegalCharacters( dataLine )) {
	  (*theErrFacil)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
        }

        
	SCETokenizer next(dataLine);

// get the operation name
        operationName = flatFileManager.nextQuotedString(
                next, result, fileName, dataLine, 
		lineNo, PURELY_OPTIONAL);
	pdf = flatFileManager.nextQuotedString(
	        next, result, fileName, dataLine, lineNo, MANDATORY);



// =======================
// Now add the operation
// =======================

// ===================================
// == Only User-defined operations get read here
// ===================================
        result = sceAddNormalOperation(theWitRun, 
                                 operationName, 
                                 pdf, 
                                 mpHelper, 
                                 fileName, 
                                 dataLine,
                                 lineNo);
	
	if (! result)
          {
	    recCount ++;
            if (myScrubber -> scrubMode() && outFile.good())
              {
                outFile << dataLine << std::endl;
              }
          }

	else
	    badCount++;
    }

    std::cout << recCount << " records processed." << std::endl;
    inFile.close();
    outFile.close();
    flatFileManager.sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}


//  ---------------------------------------------------------------
int  
LgFrInitialProblemForSce::readCsvDemandFloatVector(
                     WitRun * const theWitRun,         // WIT environment    
                     const int nPeriods,               // Number of periods  
                     std::string & geoPlannerDemandPartName,   // special partname for  this demand
                     std::string & geo,                  // demandName to be returned
                     float *  floatVec,                // floatVector to be returned
                     std::ifstream & inFile,                 // An open stream
                     const std::string & fileName,       // for messaging
                     std::string & dataLine,
                     long & lineNo,
                     LgFrMultiPlantHelper & mpHelper,
                     bool Mandatory,              // Are all elements mandatory?
                     float lowerLimit,                 // data lower limits
                     float upperLimit,                 // data upper limits
                     bool addDemandIfFound)       // Add demand stream if not there
{
  std::string plannerPartName;
  int t = 0;
  int result = 0;
 
  // Read next line of file
  std::getline( inFile, dataLine );
  lineNo ++;

  dataLine = sce::strip( dataLine );

  //  skip blanks or comments '*'
  if ((dataLine[0] == '*') || dataLine.empty())
    return -2;


  // construct a tokenizer
  SCETokenizer next(dataLine);
  
  // get a reference to the flat file manager and 
  // check if the line contains any illegal characters
  LgFrSceFlatFileMgr & flatFileManager =
    this->sceScenario()->flatFileMgr();   

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  if ( flatFileManager.containsIllegalCharacters( dataLine )) {
    (*theErrFacil)("IllegalCharacter",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << dataLine);
  }

  
  // get the partname
  plannerPartName = flatFileManager.nextQuotedString(next, result, fileName, 
                                     dataLine, lineNo, 
                                     OPTIONAL_WITH_MESSAGE);

  // get the geo
  geo = flatFileManager.nextQuotedString(next, result, fileName, 
                         dataLine, lineNo, MANDATORY);

  // Get the unique special geoPlannerDemandPartName for this pair
  geoPlannerDemandPartName = mpHelper.geoPlannerDemandPartName(plannerPartName, geo);


  // If the  specialgeoPlannerDemandPart name is not a WIT part 
  // (ie, the source for this GEO is not known), then attempt to source
  // it to a pdf_PartName as follows:
  //         Set:      mfgPartName = plannerPartName
  //                   pdf         = defaultPdf

  int readCsvMessage = OPTIONAL_WITH_MESSAGE;
  if (addDemandIfFound)
    readCsvMessage = PURELY_OPTIONAL;

  int doesTheGpd_PART_Exist = mpHelper.isPartValid(theWitRun, 
						   geoPlannerDemandPartName, 
						   fileName, dataLine, lineNo, PURELY_OPTIONAL);
  
  if (! doesTheGpd_PART_Exist)  {
    (*theErrFacil)("CannotSourceDemandRecord",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << plannerPartName 
		   << geo 
		   << geoPlannerDemandPartName  
		   << dataLine);
    return 0;
  }

  int doesTheGpd_DEMAND_Exist = doesDemandExist(theWitRun, geoPlannerDemandPartName, geo);

  if (! doesTheGpd_DEMAND_Exist) {
    if (addDemandIfFound){
      // regardless of engine, load the priorities and shipLateLimit
      // get the default priority
      int defaultPriority = heurModelHelper_->defaultPriority();
      int   * defaultPriorityVec = new int[nPeriods];
      for (t=0; t<nPeriods; t++) {
	defaultPriorityVec[t] = 1000000000 - (int) defaultPriority;
      }
      // get the default shipLateLimit
      int defaultShipLateLimit = heurModelHelper_->defaultShipLateLimit();
      witAddDemand( theWitRun, geoPlannerDemandPartName.c_str(), geo.c_str() );
      // create a default demand app data for this demand, with  hasDemandVolBeenSet=0 and  backlogYield= 1.0
      LgFrSceDemandAppData * demandAppDataPtr = new LgFrSceDemandAppData (0, 0, (size_t) nPeriods, 1.0);  
      witSetDemandAppData(theWitRun,
                          geoPlannerDemandPartName.c_str(), 
                          geo.c_str(), 
                          (void *) demandAppDataPtr);

      witSetDemandPriority( theWitRun, geoPlannerDemandPartName.c_str(),
                            geo.c_str(), defaultPriorityVec );
      witSetDemandShipLateLimit( theWitRun, geoPlannerDemandPartName.c_str(),
                                 geo.c_str(), defaultShipLateLimit );
      delete [] defaultPriorityVec;

      // but only load the obj1 values if we have too      
      if ( lpModelHelper_ != 0 ) {
        // get the default Obj1 Ship Reward
        float defaultObj1ShipReward = lpModelHelper_->defaultObj1ShipReward();

        // get the periodic Interest Rate
        float periodicInterestRate = lpModelHelper_->periodicInterestRate();

        float * defaultObj1ShipRewardVec = new float[nPeriods];
        float * defaultObj1CumShipRewardVec = new float[nPeriods];

        // make time vecs from scalars
        for (t=0; t<nPeriods; t++) {
          defaultObj1ShipRewardVec[t] = defaultObj1ShipReward;
          defaultObj1CumShipRewardVec[t] = defaultObj1ShipReward * 
            periodicInterestRate;
        }

        witSetDemandObj1ShipReward( theWitRun, geoPlannerDemandPartName.c_str(),
                                    geo.c_str(), defaultObj1ShipRewardVec);
        witSetDemandObj1CumShipReward( theWitRun, geoPlannerDemandPartName.c_str(),
                                       geo.c_str(), defaultObj1CumShipRewardVec);
        delete [] defaultObj1ShipRewardVec;
        delete [] defaultObj1CumShipRewardVec;
      }

    }
    else  {
      (*theErrFacil)("CannotSourceDemandRecord",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << plannerPartName 
		     << geo 
		     << geoPlannerDemandPartName
		     << dataLine);
      return 0;
    }

  }


  // Now get the data
  for (t = 0; t<nPeriods; t++) {
    int result = flatFileManager.nextFloat(floatVec[t], next, fileName, dataLine, lineNo, false);
    if (result == -1) 
      return t;
  }

  return t;

}

//  ---------------------------------------------------------------
int  
LgFrInitialProblemForSce::readCsvDemandFloatScalar(
                     WitRun * const theWitRun,         // WIT environment    
                     const int nPeriods,               // Number of periods  
                     std::string & geoPlannerDemandPartName,   // special partname for  this demand
                     std::string & geo,                  // demandName to be returned
                     float theValue,                // float to be returned
                     std::ifstream & inFile,                 // An open stream
                     const std::string & fileName,       // for messaging
                     std::string & dataLine,
                     long & lineNo,
                     LgFrMultiPlantHelper & mpHelper,
                     bool Mandatory,              // Are all elements mandatory?
                     float lowerLimit,                 // data lower limits
                     float upperLimit,                 // data upper limits
                     bool addDemandIfFound)       // Add demand stream if not there
{
  std::string plannerPartName;
  int t = 0;
  int result = 0;
 
  // Read next line of file
  std::getline( inFile, dataLine );
  lineNo ++;

  dataLine = sce::strip( dataLine );

  //  skip blanks or comments '*'
  if ((dataLine[0] == '*') || dataLine.empty())
    return -2;


  // construct a tokenizer
  SCETokenizer next(dataLine);
  
  // get a reference to the flat file manager and 
  // check if the line contains any illegal characters
  LgFrSceFlatFileMgr & flatFileManager =
    this->sceScenario()->flatFileMgr();   

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  if ( flatFileManager.containsIllegalCharacters( dataLine )) {
    (*theErrFacil)("IllegalCharacter",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << dataLine);
  }

  
  // get the partname
  plannerPartName = flatFileManager.nextQuotedString(next, result, fileName, 
                                     dataLine, lineNo, 
                                     OPTIONAL_WITH_MESSAGE);

  // get the geo
  geo = flatFileManager.nextQuotedString(next, result, fileName, 
                         dataLine, lineNo, MANDATORY);

  // Get the unique special geoPlannerDemandPartName for this pair
  geoPlannerDemandPartName = mpHelper.geoPlannerDemandPartName(plannerPartName, geo);


  // If the  specialgeoPlannerDemandPart name is not a WIT part 
  // (ie, the source for this GEO is not known), then attempt to source
  // it to a pdf_PartName as follows:
  //         Set:      mfgPartName = plannerPartName
  //                   pdf         = defaultPdf

  int readCsvMessage = OPTIONAL_WITH_MESSAGE;
  if (addDemandIfFound)
    readCsvMessage = PURELY_OPTIONAL;

  int doesTheGpd_PART_Exist = mpHelper.isPartValid(theWitRun, 
						   geoPlannerDemandPartName, 
						   fileName, dataLine, lineNo, PURELY_OPTIONAL);
  
  if (! doesTheGpd_PART_Exist)  {
    (*theErrFacil)("CannotSourceDemandRecord",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << plannerPartName 
		   << geo 
		   << geoPlannerDemandPartName  
		   << dataLine);
    return 0;
  }

  int doesTheGpd_DEMAND_Exist = doesDemandExist(theWitRun, geoPlannerDemandPartName, geo);

  if (! doesTheGpd_DEMAND_Exist) {
    if (addDemandIfFound){
      // regardless of engine, load the priorities and shipLateLimit
      // get the default priority
      int defaultPriority = heurModelHelper_->defaultPriority();
      int   * defaultPriorityVec = new int[nPeriods];
      for (t=0; t<nPeriods; t++) {
	defaultPriorityVec[t] = 1000000000 - (int) defaultPriority;
      }
      // get the default shipLateLimit
      int defaultShipLateLimit = heurModelHelper_->defaultShipLateLimit();
      witAddDemand( theWitRun, geoPlannerDemandPartName.c_str(), geo.c_str() );
      // create a default demand app data for this demand, with  hasDemandVolBeenSet=0 and  backlogYield= 1.0
      LgFrSceDemandAppData * demandAppDataPtr = new LgFrSceDemandAppData (0, 0, (size_t) nPeriods, 1.0);  
      witSetDemandAppData(theWitRun,
                          geoPlannerDemandPartName.c_str(), 
                          geo.c_str(), 
                          (void *) demandAppDataPtr);

      witSetDemandPriority( theWitRun, geoPlannerDemandPartName.c_str(),
                            geo.c_str(), defaultPriorityVec );
      witSetDemandShipLateLimit( theWitRun, geoPlannerDemandPartName.c_str(),
                                 geo.c_str(), defaultShipLateLimit );
      delete [] defaultPriorityVec;

      // but only load the obj1 values if we have too      
      if ( lpModelHelper_ != 0 ) {
        // get the default Obj1 Ship Reward
        float defaultObj1ShipReward = lpModelHelper_->defaultObj1ShipReward();

        // get the periodic Interest Rate
        float periodicInterestRate = lpModelHelper_->periodicInterestRate();

        float * defaultObj1ShipRewardVec = new float[nPeriods];
        float * defaultObj1CumShipRewardVec = new float[nPeriods];

        // make time vecs from scalars
        for (t=0; t<nPeriods; t++) {
          defaultObj1ShipRewardVec[t] = defaultObj1ShipReward;
          defaultObj1CumShipRewardVec[t] = defaultObj1ShipReward * 
            periodicInterestRate;
        }

        witSetDemandObj1ShipReward( theWitRun, geoPlannerDemandPartName.c_str(),
                                    geo.c_str(), defaultObj1ShipRewardVec);
        witSetDemandObj1CumShipReward( theWitRun, geoPlannerDemandPartName.c_str(),
                                       geo.c_str(), defaultObj1CumShipRewardVec);
        delete [] defaultObj1ShipRewardVec;
        delete [] defaultObj1CumShipRewardVec;
      }

    }
    else  {
      (*theErrFacil)("CannotSourceDemandRecord",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << plannerPartName 
		     << geo 
		     << geoPlannerDemandPartName
		     << dataLine);
      return 0;
    }

  }


  // Now get the data
  result = flatFileManager.nextFloat(theValue, next, fileName, dataLine, lineNo, false);
  if (result == -1) 
    return 0;
  
  
  return 1;

}




// ---------------------------------------------------------------
bool
LgFrInitialProblemForSce::doesInputFileExist(
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


// --------------------------------------------------------------- 
//           Test to see if a PART,DEMAND pair exists              
// ---------------------------------------------------------------
int 
LgFrInitialProblemForSce::doesDemandExist(
    WitRun * const theWitRun,       
    const std::string &  partName,
    const std::string &  demandName)

{
  int nDemands;                         // Number of demands on part
  char ** demandList;                   // List of demands on part  
  int j;
  witBoolean exists;
  int result;

  witGetPartExists(theWitRun, partName.c_str(), &exists);
  if (exists == WitFALSE)  
    return 0;

  //  Get list of demands defined for part. 
  witGetPartDemands(theWitRun, partName.c_str(), &nDemands, &demandList);

  // Loop thru this parts demands until we find one that 
  // matches demandName 
  result = 0;
  for ( j=0; j<nDemands; j++ ) {
    if (demandName != demandList[j])
      continue;
    result = 1;
    break;
  }
 
  // free demand list storage   
  for (j=0; j<nDemands; j++)  
    witFree(demandList[j]);
  witFree(demandList);

  return result;
}


// ============================================================
// ============================================================
// =========(this is sceInitAdvanced.C ========================


// Add part and any attributes to wit for NORMAL parts
// Return --->  0  if succesful
//             -1  if un-succesful with warning
//             >0  if Severe error
int
LgFrInitialProblemForSce::sceAddNormalPart(
    WitRun * const theWitRun, 
    const std::string & part, 
    const std::string & pdf,
    const bool buildNstn,
    const int * buildAheadUB,
    LgFrMultiPlantHelper & mpHelper,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    std::string partName = mpHelper.pdfPartname(part, pdf);

    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    // check to see if the part has already been defined
    if (mpHelper.isPartValid(theWitRun, partName, fileName, 
			     dataLine, lineNo, PURELY_OPTIONAL))  {
      (*theErrFacil)("AlreadyDefined",MclArgList() 
		     << partName 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
      return -1;
    }
    
    // put a small prodCost in for every part that is slightly
    // decreasing over time.  It's a FINISH_ME to both implement
    // a better (ie, exponential) curve, and, a more efficient method
    // so that we don't have to new and delete a vector for every part.
    int nPeriods;
    witGetNPeriods(theWitRun, &nPeriods);
    
    witAddPartWithOperation(theWitRun, partName.c_str());
    
    // regardless of engine, load the BopAppData with the preference
    float preference = heurModelHelper_->defaultDefaultBopPreference ();
    LgFrSceBopAppData * myBadPtr; // my "b"op "a"pp "d"ata ptr
    myBadPtr = new LgFrSceBopAppData (preference, 1, 0);  
    assert ( myBadPtr != 0 );
    witSetBopEntryAppData( theWitRun, partName.c_str(), 0, (void *) myBadPtr );
    
    
    // set default inc and min lot sizes to 1.0.
    float * vecOfOnes = new float[nPeriods];
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++)
      vecOfOnes[t] = 1.0;
    
    witSetOperationIncLotSize(theWitRun, partName.c_str(), vecOfOnes);
    witSetOperationMinLotSize(theWitRun, partName.c_str(), vecOfOnes);
    delete [] vecOfOnes;
    
    // but only set LP coeficients only if engine = optimization
    // Multi-critera note:  why not move this to lpModelHelper 
    if (lpModelHelper_ != 0) {
      // we'll reuse this generic cost vector
      float * obj1CostVec = new float[nPeriods];

      // set the default Exec Cost on default operations
      float defaultObj1ProdCostRealParts = lpModelHelper_->defaultObj1ProdCostRealParts();
      float obj1ProdCostDiscRateRealParts = lpModelHelper_->obj1ProdCostDiscRateRealParts();      
      obj1CostVec[0] = defaultObj1ProdCostRealParts;
      int t = 0; 
      for (t=1; t<nPeriods; t++)
        obj1CostVec[t] = obj1CostVec[t-1] - obj1ProdCostDiscRateRealParts;
      witSetOperationObj1ExecCost(theWitRun, partName.c_str(), obj1CostVec);

      // this is really only used for GPS.   They set it really high and it blocks all stock.
      float obj1DefaultStockCostRealParts = lpModelHelper_->obj1DefaultStockCostRealParts();      
      for (t=0; t<nPeriods; t++)      
        obj1CostVec[t] = obj1DefaultStockCostRealParts;
      witSetPartStockCost(theWitRun, partName.c_str(), obj1CostVec);
      
      delete [] obj1CostVec;
    }
    
    
    // now set buildahead things
    // fix for 4.30.   This is legacy.   
    // In 4.30, user's can set by partnumber, and, can set to either NSTN or ASAP
    // SCE6.0 fix this.   either get rid of NSTN or override with ASAP/NSTN by guildAhead.
    witSetPartBuildNstn(theWitRun, partName.c_str(), buildNstn);
    if (buildNstn) 
      witSetPartBuildAheadUB(theWitRun, partName.c_str(), buildAheadUB);

    return 0;
}



// Add part and any attributes to wit for NORMAL parts
// Return --->  0  if succesful
//             -1  if un-succesful with warning
//             >0  if Severe error
int
LgFrInitialProblemForSce::sceAddNormalCapacityPart(
    WitRun * const theWitRun, 
    const std::string & capacity, 
    const std::string & pdf,
    const int & mandatoryCapacityFlag,
    LgFrMultiPlantHelper & mpHelper,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    std::string capacityName = mpHelper.pdfPartname(capacity, pdf);

    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

// check to see if the part has already been defined
    if (mpHelper.isPartValid(theWitRun, capacityName, fileName, 
			     dataLine, lineNo, PURELY_OPTIONAL))  {
      (*theErrFacil)("AlreadyDefined",MclArgList() << capacityName << fileName << (int)lineNo << dataLine);

	return -1;
    }
    
    witAddPart(theWitRun, capacityName.c_str(), WitCAPACITY);

    // Add the operation to Generate Capacity (regardless of whether it's mandatory or not)
    std::string capGeneratorOperationName = mpHelper.operationForCapacityGeneration(capacityName.c_str());
    witAddOperation(theWitRun, capGeneratorOperationName.c_str());
    if (mandatoryCapacityFlag) {
      int nPeriods;
      witGetNPeriods(theWitRun, &nPeriods);
      LgFrTimeVecFloat zeroYieldRate(nPeriods, 0.0);
      witSetOperationYieldRate(theWitRun, capGeneratorOperationName.c_str(), zeroYieldRate.data());
    }

    witAddBopEntry(theWitRun, capGeneratorOperationName.c_str(), capacityName.c_str());
    // for VCM
    witSetBopEntryExpAversion(theWitRun, capGeneratorOperationName.c_str(), 0, 9999999.9);

    return 0;
}


// Add operation and any default attributes
// to wit for NORMAL operation
// Return --->  0  if succesful
//             -1  if un-succesful with warning
//             >0  if Severe error
int
LgFrInitialProblemForSce::sceAddNormalOperation(
    WitRun * const theWitRun, 
    const std::string & operationName, 
    const std::string & pdf,
    LgFrMultiPlantHelper & mpHelper,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    std::string pdfOperationName = mpHelper.pdfOperationName(operationName, pdf);

    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    // check to see if the operation has already been defined
    if (mpHelper.isOperationValid(theWitRun, pdfOperationName, fileName, 
			     dataLine, lineNo, PURELY_OPTIONAL))  {
      (*theErrFacil)("AlreadyDefined",MclArgList() << operationName << fileName << (int)lineNo << dataLine);

	return -1;
    }

    // check to see if a pdf-part was given the same name as this pdf-operation
    if (!  mpHelper.isPdfOperationUnique(theWitRun, pdfOperationName, fileName, 
                                         dataLine, lineNo, PURELY_OPTIONAL)) {
      (*theErrFacil)("DuplicatePartPDFPairWarning",MclArgList() << pdfOperationName << fileName << (int)lineNo << dataLine);
        return -1;
    }
        
    // put a small execCost in for every part that is slightly
    // decreasing over time.  It's a FINISH_ME to both implement
    // a better (ie, exponential) curve, and, a more efficient method
    // so that we don't have to new and delete a vector for every part.
    // FINISH_ME: set up the LP cost model for operations
    witAddOperation(theWitRun, pdfOperationName.c_str());

    int nPeriods;
    witGetNPeriods(theWitRun, &nPeriods);
    // set default lot sizes to 1.0.
    float * vecOfOnes = new float[nPeriods];
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++)
      vecOfOnes[t] = 1.0;
    witSetOperationMinLotSize(theWitRun, pdfOperationName.c_str(), vecOfOnes);
    witSetOperationIncLotSize(theWitRun, pdfOperationName.c_str(), vecOfOnes);
    delete [] vecOfOnes;
    
    if (lpModelHelper_ != 0) {
      float defaultObj1ExecCostRealOperations =
        lpModelHelper_->defaultObj1ExecCostRealOperations();
    
      float obj1ExecCostDiscRateRealOperations =
        lpModelHelper_->obj1ExecCostDiscRateRealOperations();      

      float * obj1ExecCost = new float[nPeriods];
      obj1ExecCost[0] = defaultObj1ExecCostRealOperations;
      
      for (t=1; t<nPeriods; t++)
        obj1ExecCost[t] = obj1ExecCost[t-1] - obj1ExecCostDiscRateRealOperations;

      witSetOperationObj1ExecCost(theWitRun, pdfOperationName.c_str(), obj1ExecCost);
 
      delete [] obj1ExecCost;
    }

    return 0;
}


// Add BOP entries and any default attributes
// to wit for NORMAL BOPs
// Return --->  0  if succesful
//             -1  if un-succesful with warning
//             +1  if un-sucessful & input record should be ignored
int
LgFrInitialProblemForSce::sceAddNormalBopStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & producedPart, 
    const std::string & producedPartPdf,
    const std::string & operation, 
    const std::string & operationPdf, 
    float prodRate,
    float offset,
    int early,
    int late,
    float pref,
    int isObjectInEffectForOptExplosion,
    int expAllowed,
    float pipShare,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    LgFrSceBopAppData * myBadPtr; // my "b"op "a"pp "d"ata ptr

    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    std::string pdfProducedPartName( mpHelper.pdfPartname(producedPart, producedPartPdf) );
    std::string pdfOperationName( mpHelper.pdfOperationName(operation, operationPdf)  );

    if (! mpHelper.isPartValid(theWitRun, producedPart, producedPartPdf, 
			       fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE)) 
      return 1;
    
    if (! mpHelper.isOperationValid(theWitRun, pdfOperationName, 
                                    fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE)) 
      return 1;




    int bopEntryIndex;
    witGetOperationNBopEntries( theWitRun, pdfOperationName.c_str(), &bopEntryIndex);

    
    // check to see if this operation is already "connected" to the produced part
    // we only do this to possibly increment the partAppdata for the pdfProducedPArt
    bool isOperationAlreadyUsedbyProducedPart = false;
    int b = 0; // Pulled out of the for below by RW2STL
    for (b=0; b<bopEntryIndex; b++) {
      char * ppName;
      witGetBopEntryProducedPart(theWitRun, pdfOperationName.c_str(), b, &ppName);
      if (ppName == pdfProducedPartName) {
        isOperationAlreadyUsedbyProducedPart = true;
        witFree(ppName);
        break;
      }
      witFree(ppName);
    }
    // If this is a new User-defined Operation for the customer loc, increment
    // the nUserDefinedOps counter in the CustLoc Parts appdata.  (Being careful,
    // of course, to ensure the non-null-ness of the appDataPtr for the part
    if (! isOperationAlreadyUsedbyProducedPart) {
      LgFrScePartAppData * custPartAppData;
      witGetPartAppData(theWitRun, pdfProducedPartName.c_str(), (void **) &custPartAppData);
      if (custPartAppData == 0) {
	int nPeriods;
	witGetNPeriods(theWitRun, &nPeriods);
        custPartAppData = new LgFrScePartAppData(0, nPeriods, nPeriods);
        custPartAppData->nUserDefinedOps(1);
        witSetPartAppData(theWitRun, pdfProducedPartName.c_str(), (void *) custPartAppData);
      }
      else 
        custPartAppData->nUserDefinedOps(custPartAppData->nUserDefinedOps() + 1);
    }

    

      
    witAddBopEntry( theWitRun, pdfOperationName.c_str(), pdfProducedPartName.c_str() );
    witSetBopEntryProdRate( theWitRun, pdfOperationName.c_str(), bopEntryIndex, prodRate );

    float * offsetTimeVec;
    witGetBopEntryOffset( theWitRun, pdfOperationName.c_str(), bopEntryIndex, &offsetTimeVec );
    int i = 0; // Pulled out of the for below by RW2STL
    for (i=early; i<=late; i++)
      offsetTimeVec[i] = offset;
    witSetBopEntryOffset( theWitRun, pdfOperationName.c_str(), bopEntryIndex, offsetTimeVec );
    witFree(offsetTimeVec);
    witSetBopEntryEarliestPeriod( theWitRun, pdfOperationName.c_str(), bopEntryIndex, early );
    witSetBopEntryLatestPeriod( theWitRun, pdfOperationName.c_str(), bopEntryIndex, late );

    // Put the preference/cost and explosion flag 
    // information into bopDemandAppData
    // bopAppVoidPtr should NOT be set in WIT -- double checking, just to be safe.
    // New some bopAppData calling the preferred constructor with the pref parameter.
    // Note: In the calling member function, pref is initialized with the
    // defaultExplicitBopPreferenence from the setOfParameters and
    // the explosionFlag is set to the default off.
    // FINISH_ME: set the explosionFlag default in the setOfParameters
    // FINISH_ME: see if the design can be improved so (1) we don't have to search for the 
    // default each and every time and (2) we can encapsulate/organize the LP vs Heur setup
    // and "helper" routines.
    
    witGetBopEntryAppData( theWitRun, pdfOperationName.c_str(), bopEntryIndex, (void **) &myBadPtr );
    if ( myBadPtr != 0 ) {
      (*theErrFacil)("InternalError2",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << producedPart 
		     << producedPartPdf 
		     << operation 
		     << dataLine);
    }
    
    myBadPtr = new LgFrSceBopAppData (pref, isObjectInEffectForOptExplosion, 0);  
    assert ( myBadPtr != 0 );
    witSetBopEntryAppData( theWitRun,
                           pdfOperationName.c_str(),
                           bopEntryIndex,
                           (void *) myBadPtr );

    // Added for 2.30
    //   witSetBopEntryExpAllowed(theWitRun, pdfOperationName.c_str(), bopEntryIndex, WitTRUE);
    // as of sce 3.11, this is now user setabble
    witSetBopEntryExpAllowed(theWitRun, pdfOperationName.c_str(), bopEntryIndex, expAllowed);
    witSetBopEntryExpAversion(theWitRun, pdfOperationName.c_str(), bopEntryIndex, pref);
    
#if 0 
    // pipshare is obsolete ... should remove everywhere 
    float * pipShareVec;
    witGetBopEntryPipShare( theWitRun, pdfOperationName.c_str(), bopEntryIndex, &pipShareVec );
    for (i=early; i<=late; i++)
      pipShareVec[i] = pipShare;
    witSetBopEntryPipShare( theWitRun, pdfOperationName.c_str(), bopEntryIndex, pipShareVec );
    witFree(pipShareVec);
#endif    


    
    return 0;
}

// Add Operation BOM entries and any default attributes
// to wit for NORMAL Operation BOMs
// Return --->  0  if succesful
//             -1  if un-succesful with warning
//             +1  if un-sucessful & input record should be ignored
int
LgFrInitialProblemForSce::sceAddNormalOperationBomStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & operation, 
    const std::string & operationPdf,
    const std::string & consumedPart, 
    const std::string & consumedPartPdf, 
    float usageRate,
    float offset,
    int early,
    int late,
    int fallout,
    int mec,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    std::string pdfConsumedPartName( mpHelper.pdfPartname(consumedPart, consumedPartPdf) );
    std::string pdfOperationName( mpHelper.pdfOperationName(operation, operationPdf)  );

    if (! mpHelper.isPartValid(theWitRun, consumedPart, consumedPartPdf,
			       fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE)) 
      return 1;
    
    if (! mpHelper.isOperationValid(theWitRun, pdfOperationName, 
                                    fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE)) 
      return 1;

      
    int bomEntryIndex;
    witGetOperationNBomEntries( theWitRun, pdfOperationName.c_str(), &bomEntryIndex);
    witAddBomEntry( theWitRun, pdfOperationName.c_str(), pdfConsumedPartName.c_str() );
    witSetBomEntryUsageRate( theWitRun, pdfOperationName.c_str(), bomEntryIndex, usageRate );

#ifdef VARIABLE_PERIODS
    // FINISH_ME: adust offsets for userDefined operations
#endif
    float * offsetTimeVec;
    witGetBomEntryOffset( theWitRun, pdfOperationName.c_str(), bomEntryIndex, &offsetTimeVec );
    int i = 0; // Pulled out of the for below by RW2STL
    for (i=early; i<=late; i++)
      offsetTimeVec[i] = offset;
    witSetBomEntryOffset( theWitRun, pdfOperationName.c_str(), bomEntryIndex, offsetTimeVec );
    witSetBomEntryEarliestPeriod( theWitRun, pdfOperationName.c_str(), bomEntryIndex, early );
    witSetBomEntryLatestPeriod( theWitRun, pdfOperationName.c_str(), bomEntryIndex, late );
    float falloutRate = (float) fallout/100.f;
    witSetBomEntryFalloutRate( theWitRun, pdfOperationName.c_str(), bomEntryIndex, falloutRate );
    witBoolean mandEC = WitFALSE;
    if (mec == 1) {
      mandEC = WitTRUE;
      std::cout << "gotta mandEC at " << pdfOperationName << " bomEntryIndex=" << bomEntryIndex << std::endl;
    }
    witSetBomEntryMandEC( theWitRun, pdfOperationName.c_str(), bomEntryIndex, mec );
    witFree(offsetTimeVec);

    return 0;
}

//--------------------------------------------------------------------
// Add part(s) and any structure to wit for SCE partType=2 (aggregation)
// REturn --->  0  if succesful
//             -1  if un-succesful with warning
//             >0  if Severe error
int
LgFrInitialProblemForSce::sceAddAggregatePart(
    WitRun * const theWitRun, 
    const std::string & part, 
    const std::string & pdf,
    LgFrMultiPlantHelper & mpHelper,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    std::cerr << "sceAddAggregatePart() not implemented yet\n\n";
    return -1;
}

int
LgFrInitialProblemForSce::sceAddPartToRepresentDemand(
    WitRun * const theWitRun, 
    const std::string & fullPartname, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    witAddPartWithOperation(theWitRun, fullPartname.c_str());

    return 0;
}





int
LgFrInitialProblemForSce::sceAddPartToRepresentDemandUpperBound(
    WitRun * const theWitRun, 
    const std::string & fullPartname, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    witAddPart(theWitRun, fullPartname.c_str(), WitRAW);

    LgFrSetOfParts & univSOP = sceScenario()->setOfParts(LGFR_UNIVERSAL_SET);
    LgFrPart partToBeAdded   = univSOP . newPart ( fullPartname );
    LgFrSetOfParts & defSOP  = sceScenario()->setOfParts(LGFR_DEFAULT_SET);
    if ( !defSOP.contains( partToBeAdded ) )
      defSOP.insert(partToBeAdded);

    LgFrSetOfParts & intrPlant = sceScenario()->setOfParts(LGFR_INTERPLANT_SET);
    if ( !intrPlant.contains( partToBeAdded ) )
      intrPlant.insert(partToBeAdded);

    return 0;
}


// ---------------------------------------------------------------
//  return values: -1=ERROR; 0=GOOD/ADDED; 1=GOOD/ALREADY_EXISTED
// ---------------------------------------------------------------
// + About: Interplant Structure
// +
// + - Exactly ONE SpecialInterplant Operation (SIO) per
// +            (destPart_Pdf,sourcePart_Pdf) pair
// +    - Lot size: set as per user on SIO (set over effective periods only)
// +            (can change over time via multiple records)
// + - Exactly ONE BOM entry from SIO to sourcePart_Pdf
// +    - early=0,late=nPeriods regardless of true ip effectivity
// +    - bom offset = transitTime (set over effective periods only)
// +            (can change over time via multiple records)
// +    - bom usageRate set as per user input.  Usage rate not allowed
// +            to change over time!
// +            (can change over time via multiple records)
// + - BOP Entries:  Multiple BOPS per (destPart_Pdf,sourcePart_Pdf) pair OK
// +           - One per each valid record
// +           - INVALID if effectivity overlaps with existing BOP
// +           - INVALID if usageRate is different than existing usageRate
// +    - early,late set to effectivity of record
// +
// + - APP DATA:
// +        PART: destinationPart_Pdf: keep count of nInterplantBops
// +        BOP:  contains bop preference, isInEffectForExplosionFlag, and expAllowed
int
LgFrInitialProblemForSce::addInterPlantBomStructure(
                                                    WitRun * const theWitRun, 
                                                    LgFrMultiPlantHelper & mpHelper,
                                                    const LgFrCalendar & theCal,
                                                    const std::string & destinationPart,
                                                    const std::string & destinationPdf,
                                                    const std::string & sourcePart, 
                                                    const std::string & sourcePdf,
                                                    float transitTime,
                                                    int early,
                                                    int late,
                                                    float minLotSize,
                                                    float incLotSize,
                                                    float usageRate,
                                                    float preference,
                                                    int isObjectInEffectForOptExplosionFlag,
                                                    int expAllowed,
						    float routingShare,
                                                    const std::string & fileName,
                                                    const std::string & dataLine,
                                                    long lineNo)
{

  witBoolean isUnique = true;
  int t;
  witBoolean exists;
  int nPeriods = theCal.nPeriods();    

  std::string fullDestinationPartName(mpHelper.pdfPartname(destinationPart, destinationPdf));
  std::string fullSourcePartName( mpHelper.pdfPartname(sourcePart,  sourcePdf));
    
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  // check to see if destinationPartpart is valid
  if (! mpHelper.isPartValid(theWitRun, destinationPart, destinationPdf, fileName, 
                             dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
    return -1;



  // check to see if sourcePartPart is valid
  if (! mpHelper.isPartValid(theWitRun, sourcePart, sourcePdf, fileName, 
                             dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
    return -1;



  // SCE 5.00 ... check to see if destinationPart and Source Part are the same
  //  if so, then check to see if propRtg is true on the parent

  // first make sure source code is OK
  std::string sourceCode("AF");
  LgFrScePartAppData * appDataPtr;
  witGetPartAppData(theWitRun, fullDestinationPartName.c_str(), (void **) &appDataPtr);
  if (appDataPtr != 0) {
    sourceCode = appDataPtr->sourceCode();
  }


  if (fullDestinationPartName == fullSourcePartName) {

    // if sourceCode != AFP then this record is not relevant
    if (sourceCode != "AFP") {
      (*theErrFacil)("InterplantToMyselfWithoutMultiSourcing",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << destinationPart 
		     << destinationPdf 
		     << sourceCode
		     << dataLine);
      return (-1);
    }

    // FINISH_ME 5.00  (we need to address sourcing that changes over time)

    // OK, you have a valid attempt to set a RoutingShare on the default operation
    int npBops;
    witGetPartNProducingBopEntries(theWitRun, fullDestinationPartName.c_str(), &npBops);
    if ( npBops == 0 ) {
      std::cout << "attempting to set a  RoutingShare on default operation when npBops=0\n"
		<< "part: " << fullDestinationPartName 
		<< "continueing"  << std::endl;
      return (0);
    }

    // OK, we need to deal with Routing Share of Zero.
    float effectiveProdRate = 1.0;
    float effectiveRoutingShare = routingShare;
    if (routingShare < 1.0) {
      effectiveRoutingShare = 1.0;
      effectiveProdRate = 0.0;
    }


    // now loop through bops looking for default operation.  
    // when you find it, set yield to whatever it should be, and set routingShare appropriately
    char * opName;
    int bopIndex;
    int bp = 0;
    for (bp=0; bp < npBops; bp++) {
      witGetPartProducingBopEntry ( theWitRun, fullDestinationPartName.c_str(), bp, &opName, &bopIndex);
      // NOTE: a bit of a shortcut.  We check the operation to see if it is the same as normal part.
      //       if so, this is the default operation.   
      //   FINISH_ME: 5.00 .. 
      //   (1) there is no way to set routingShare on bops that are produced by userDefined ops.
      //   (2) We keep looping, which enables the possibility that there are multiple bops that lead
      //       to defaultOp (maybe with diffenrt effectivities.  It is a finishme in genreal to deal with
      //       routing shares varying over time.
      if (mpHelper.isPartNormal(theWitRun, opName)) {
	//	LgFrTimeVecFloat routingShareVec(nPeriods, routingShare);
	float * routingShareVec;
	float * productRateVec;
	witGetBopEntryRoutingShare(theWitRun, opName, bopIndex, &routingShareVec);
	witGetBopEntryProductRate(theWitRun, opName, bopIndex, &productRateVec);
	int tbt = early;
	for (tbt=early; tbt<=late; tbt++) {
	  routingShareVec[tbt] = effectiveRoutingShare;
	  productRateVec[tbt] = effectiveProdRate;
	}
	witSetBopEntryRoutingShare(theWitRun, opName, bopIndex, routingShareVec);
	witSetBopEntryProductRate(theWitRun, opName, bopIndex, productRateVec);

	witFree (routingShareVec);
	witFree (productRateVec);
      }
      witFree ( opName );   
    }
      
    // good job, you can now return successfully
    return (0);


  }

  // ==== if you got here, it is a normal interplant arc

  // IF AFP and routingShare is zero or less than 1, then
  //   we basically return without entering any interplant BOP. 
  //   if it's zero we, return quietly, if it's between (0,1) we complain mildly  
  if ((sourceCode == "AFP") && (routingShare < 1.0))  {
    if (routingShare > 1.00001)  
      (*theErrFacil)("RoutingShareOfZero",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << destinationPart 
		     << destinationPdf 
		     << sourcePart 
		     << sourcePdf 
		     << sourceCode
		     << routingShare
		     << dataLine);
    return (0);
  }




  std::string interPlantOperationName =
    mpHelper.interPlantOperationName(destinationPart, destinationPdf, sourcePart, sourcePdf);
	
  // test to see if the interplant operation already exists
  witGetOperationExists(theWitRun, interPlantOperationName.c_str(), &exists);


  // if the interplant operation does NOT exist, then add it along with the
  // bom record to the source part, and set some defaults.
  //
  // if the interplant operation DOES exists, check to see if this record is a
  // "legal duplicate". To be legal it has to satisfy 2 conditions:
  //   (1) it the same usage rate as the previous records (which is mapped
  //       to the usage rate on the one and only bom for this interplant
  //       operation 
  //   (2) the effectivity dates are disjoint from the effectivity
  //       dates on all the other bops for this interplant operation
  //   If either (1) or (2) does not hold, reject the record and print an
  //   error message.

  if ( ! exists ) {
    // we initialize the default lot sizes on the dummy interplant part.
    // we will update it later.
    float * defaultLotSize = new float[nPeriods];
    for (t=0; t<nPeriods; t++) {
      defaultLotSize[t] = 1.0;
    }

    // add the dummy interplant operation and the source part bom entry to WIT
    witAddOperation( theWitRun, interPlantOperationName.c_str() );
    witSetOperationMinLotSize( theWitRun, interPlantOperationName.c_str(), defaultLotSize );
    witSetOperationIncLotSize( theWitRun, interPlantOperationName.c_str(), defaultLotSize );
    witAddBomEntry( theWitRun, interPlantOperationName.c_str(), fullSourcePartName.c_str() );
    witSetBomEntryUsageRate( theWitRun, interPlantOperationName.c_str(), 0, usageRate );
    // In our model, there is one and only bom hanging off the interplant operation.
    // In our model, the bom is effective through out the entire planning horizon.
    witSetBomEntryEarliestPeriod( theWitRun, interPlantOperationName.c_str(), 0, 0 );
    witSetBomEntryLatestPeriod( theWitRun, interPlantOperationName.c_str(), 0, nPeriods-1 );
    if (groupSharingViaExecPenalties_) {
      witSetBomEntryExecPenalty(theWitRun, interPlantOperationName.c_str(), 0, preference);
    }

    // Since this is a new interplant Operation for the customer loc, increment
    // the nInterplantOps counter in the CustLoc Parts appdata.  (Being careful,
    // of course, to ensure the non-null-ness of the appDataPtr for the part
    LgFrScePartAppData * custPartAppData;
    witGetPartAppData(theWitRun, fullDestinationPartName.c_str(), (void **) &custPartAppData);
    if (custPartAppData == 0) {
      custPartAppData = new LgFrScePartAppData(0, nPeriods, nPeriods);
      custPartAppData->nInterplantOps(1);
      witSetPartAppData(theWitRun, fullDestinationPartName.c_str(), (void *) custPartAppData);
    }
    else 
      custPartAppData->nInterplantOps(custPartAppData->nInterplantOps() + 1);      

    delete [] defaultLotSize;
  }

  if ( exists ) {
    // set unique flag = false
    isUnique = false;

    if ( ! mpHelper.isDuplicateInterplantRecordUsageRateValid(
                     theWitRun, fullSourcePartName, interPlantOperationName,
                     usageRate)) {
      (*theErrFacil)("ConflictingInterplantRecordsUR",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << destinationPart 
		     << destinationPdf 
		     << sourcePart 
		     << sourcePdf 
		     << dataLine);
      return (-1);
    }
    
    if ( ! mpHelper.isDuplicateInterplantRecordEffectivityDatesValid(
                      theWitRun, interPlantOperationName, early, late )) {
      (*theErrFacil)("ConflictingInterplantRecordsED",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << destinationPart 
		     << destinationPdf 
		     << sourcePart 
		     << sourcePdf 
		     << dataLine);
      return (-1);
    }
  }


  // do a sanity check ... THERE MUST BE ONE AND ONLY ONE BOM
  int nBom;
  witGetOperationNBomEntries(theWitRun, interPlantOperationName.c_str(), &nBom);
  assert(nBom == 1);

  // if we made it here, we either have a "valid duplicate" interplant
  // record or a new one. In either case, we now create a bop entry
  // between destination part and interplant operation,
  // with effectivity = (early, late)
  int nbBop;
  witGetOperationNBopEntries( theWitRun, interPlantOperationName.c_str(), &nbBop );      
  witAddBopEntry( theWitRun, interPlantOperationName.c_str(), fullDestinationPartName.c_str() );
  witSetBopEntryEarliestPeriod( theWitRun, interPlantOperationName.c_str(),
                                nbBop, early );      
  witSetBopEntryLatestPeriod( theWitRun, interPlantOperationName.c_str(),
                              nbBop, late );

  // SCE 5.00 ... set the routing share
  LgFrTimeVecFloat routingShareVec(nPeriods, routingShare);
  witSetBopEntryRoutingShare(theWitRun, interPlantOperationName.c_str(), 
			     nbBop, routingShareVec.data());


#ifdef VARIABLE_PERIODS
  // FINISH_ME: interplant transit times
#endif

  // Now set the transitTime using bom offset.  
  // the effectivity range are left as they were.
  float * offset;
  witGetBomEntryOffset(theWitRun, interPlantOperationName.c_str(), 0, &offset);
  for (t=early; t<=late; t++)
      offset[t] = transitTime / theCal.workUnits(t);
  witSetBomEntryOffset(theWitRun, interPlantOperationName.c_str(), 0, offset);
  witFree(offset);
  
  // Set the minLotSize and incLotSize on the  interplant operation.
  // Note that the min/incLotSize size is only set in the effective range.
  // The values outside the effectivity range are left as they were.
  float * interPlantMinLotSize;
  float * interPlantIncLotSize;
  witGetOperationMinLotSize( theWitRun, interPlantOperationName.c_str(), &interPlantMinLotSize );
  witGetOperationIncLotSize( theWitRun, interPlantOperationName.c_str(), &interPlantIncLotSize );
  
  for (t=early; t<=late; t++) {
    interPlantMinLotSize[t] = minLotSize ;
    interPlantIncLotSize[t] = incLotSize ;    
  }
  
  witSetOperationMinLotSize(theWitRun, interPlantOperationName.c_str(), interPlantMinLotSize);
  witSetOperationIncLotSize(theWitRun, interPlantOperationName.c_str(), interPlantIncLotSize);
  witFree(interPlantMinLotSize);
  witFree(interPlantIncLotSize);


  // Add an app data for the bop preference and explosion flag
  
  void * bopAppVoidPtr;
  LgFrSceBopAppData * myBopAppDataPtr; 
  witGetBopEntryAppData( theWitRun, interPlantOperationName.c_str(), nbBop, &bopAppVoidPtr );
  if ( bopAppVoidPtr != 0 ) {
    (*theErrFacil)("InternalError3",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << destinationPart 
		   << destinationPdf 
		   << sourcePart 
		   << sourcePdf 
		   << dataLine);
  }
  myBopAppDataPtr = new LgFrSceBopAppData (preference, 
					   isObjectInEffectForOptExplosionFlag, 
					   expAllowed);  
  assert ( myBopAppDataPtr != 0 );
  witSetBopEntryAppData( theWitRun, interPlantOperationName.c_str(),
                         nbBop, (void *) myBopAppDataPtr );


  // 2.30 Note:
  //  we don't set expAllowed or expAversion yet.  It will be done in the split bop.
  // FINISH_ME 2.31:  WHY,??  I'm going to set it
  witSetBopEntryExpAllowed(theWitRun, interPlantOperationName.c_str(), nbBop, expAllowed);
  witSetBopEntryExpAversion(theWitRun, interPlantOperationName.c_str(), nbBop, preference);

  if ( lpModelHelper_ != 0 ) {    
    float * opExecCost;
    witGetOperationObj1ExecCost(theWitRun, interPlantOperationName.c_str(), &opExecCost);
    for (t=early; t<=late; t++)
      opExecCost[t] = preference;
    witSetOperationObj1ExecCost(theWitRun, interPlantOperationName.c_str(), opExecCost);
    witFree(opExecCost);
  }


  if (isUnique)
    return 0;
  else
    return 1;
}




// --------------------------------------------------------------------------

// Add part(s) and any structure to wit for BB Category parts (BB/feature set)
// REturn --->  0  if succesful
//             -1  if un-succesful with warning
//             >0  if Severe error
int
LgFrInitialProblemForSce::sceAddBbCategoryPartAndStructureViaProportionalRoute(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & plannerTopLevelPart, 
    const std::string & geo,
    const std::string & bbCategory, 
    const float * gammaMaxVector,
    float gammaMin,
    int early,
    int late,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    int nPeriods;

    witGetNPeriods(theWitRun, &nPeriods);

    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    // get a unique part name for the bbCategory
    std::string fullBbCategoryPart = mpHelper.bbCategoryPartName(plannerTopLevelPart, geo,
							       bbCategory);

    // Now check to see if the  bbCategory does not already exist
    if (mpHelper.isPartValid(theWitRun, fullBbCategoryPart, fileName, 
			       dataLine, lineNo, PURELY_OPTIONAL)) {
      (*theErrFacil)("BBCategoryAlreadyDefined",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << plannerTopLevelPart 
		     << geo 
		     << bbCategory 
		     << fullBbCategoryPart 
		     << dataLine);
	return -1;
    }


    // make sure the plannerTopLevel Special Demand Part exists
    std::string geoPlannerDemandPart = mpHelper.geoPlannerDemandPartName(plannerTopLevelPart, geo);
    if (! mpHelper.isPartValid(theWitRun, geoPlannerDemandPart, fileName, 
			       dataLine, lineNo, PURELY_OPTIONAL))   {
      (*theErrFacil)("PartDoesNotExist",MclArgList() 
		     << plannerTopLevelPart 
		     << geo 
		     << fileName 
		     << bbCategory 
		     << (int)lineNo 
		     << dataLine);
      return -1;
    }

    LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();
   
    // get the default stock and scrap penalty for featureSet part
    float featureSetStockPenalty = setOfParameters.getFloat("featureSetStockPenalty");
    float featureSetScrapPenalty = setOfParameters.getFloat("featureSetScrapPenalty");    


    float * featureSetStockPenaltyVec = new float[nPeriods];
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++)  
	featureSetStockPenaltyVec[t] = featureSetStockPenalty;

    float * featureSetScrapPenaltyVec = new float[nPeriods];
    for (t=0; t<nPeriods; t++)  
	featureSetScrapPenaltyVec[t] = featureSetScrapPenalty;


    // Add the BB category  part and operation 
    // We'll use the default operation for MRP purposes only
    // NOW_FINISH_ME: should this be a capacity part??
    witAddPart( theWitRun, fullBbCategoryPart.c_str(), WitMATERIAL);
    witSetPartPropRouting(theWitRun, fullBbCategoryPart.c_str(), WitTRUE);
    //    witSetBopEntryExpAversion(theWitRun, fullBbCategoryPart.c_str(), 0, -999999.0);
    if ( lpModelHelper_ != 0) {
      witSetPartObj1StockCost( theWitRun, fullBbCategoryPart.c_str(), featureSetStockPenaltyVec );
      witSetPartObj1ScrapCost( theWitRun, fullBbCategoryPart.c_str(), featureSetScrapPenaltyVec );
    }


    delete [] featureSetScrapPenaltyVec;
    delete [] featureSetStockPenaltyVec;

    // BOM structure between gpd and bbCategory
    int nBomGPD;
    witGetOperationNBomEntries(theWitRun, geoPlannerDemandPart.c_str(), &nBomGPD);
    witAddBomEntry(theWitRun, geoPlannerDemandPart.c_str(), fullBbCategoryPart.c_str());
    witSetBomEntryConsRate(theWitRun, geoPlannerDemandPart.c_str(), nBomGPD, gammaMaxVector);
    // NOW_FINISH_ME: what to do about gammaMin??
    


    return 0;
}


// --------------------------------------------------------------------------
// SCE 6.1
// Add part(s) and any structure to wit for BB Category parts (BB/feature set)
// REturn --->  0  if succesful
//             -1  if un-succesful with warning
//             >0  if Severe error
int
LgFrInitialProblemForSce::sceAddBbCategoryPartAndStructureViaProportionalRoute61(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & plannerTopLevelPart, 
    const std::string & geo,
    const std::string & bbCategory, 
    const float gammaMax,
    float gammaMin,
    int earlyPeriod,
    int latePeriod,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    int nPeriods;
    int t = 0; 

    witGetNPeriods(theWitRun, &nPeriods);

    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    std::string fullBbCategoryPart = mpHelper.bbCategoryPartName(plannerTopLevelPart, geo, bbCategory);
    if (mpHelper.isPartValid(theWitRun, fullBbCategoryPart, fileName, dataLine, lineNo, PURELY_OPTIONAL)) {
      (*theErrFacil)("BBCategoryAlreadyDefined",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart << geo 
		     << bbCategory << fullBbCategoryPart << dataLine); 
      return -1;
    }


    std::string geoPlannerDemandPart = mpHelper.geoPlannerDemandPartName(plannerTopLevelPart, geo);
    if (! mpHelper.isPartValid(theWitRun, geoPlannerDemandPart, fileName, dataLine, lineNo, PURELY_OPTIONAL))   {
      (*theErrFacil)("PartDoesNotExist",MclArgList() << plannerTopLevelPart << geo << fileName << bbCategory << (int)lineNo << dataLine);
      return -1;
    }

    LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();
   
    
    // Add the BB category  part and operation 
    // We'll use the default operation for MRP purposes only
    //   ... should this be a capacity part??
    witAddPart( theWitRun, fullBbCategoryPart.c_str(), WitMATERIAL);
    witSetPartPropRouting(theWitRun, fullBbCategoryPart.c_str(), WitTRUE);
    //    witSetBopEntryExpAversion(theWitRun, fullBbCategoryPart.c_str(), 0, -999999.0);

    if ( lpModelHelper_ != 0) {
      float featureSetStockPenalty = setOfParameters.getFloat("featureSetStockPenalty");
      float featureSetScrapPenalty = setOfParameters.getFloat("featureSetScrapPenalty");    
      float * featureSetStockPenaltyVec = new float[nPeriods];
      float * featureSetScrapPenaltyVec = new float[nPeriods];
      for (t=0; t<nPeriods; t++)  {
	featureSetStockPenaltyVec[t] = featureSetStockPenalty;
	featureSetScrapPenaltyVec[t] = featureSetScrapPenalty;
      }
      witSetPartObj1StockCost( theWitRun, fullBbCategoryPart.c_str(), featureSetStockPenaltyVec );
      witSetPartObj1ScrapCost( theWitRun, fullBbCategoryPart.c_str(), featureSetScrapPenaltyVec );
      delete [] featureSetScrapPenaltyVec;
      delete [] featureSetStockPenaltyVec;
    }

    // BOM structure between gpd and bbCategory
    int nBomGPD;
    witGetOperationNBomEntries(theWitRun, geoPlannerDemandPart.c_str(), &nBomGPD);
    witAddBomEntry(theWitRun, geoPlannerDemandPart.c_str(), fullBbCategoryPart.c_str());

    float * gammaMaxVector;
    witGetBomEntryConsRate(theWitRun, geoPlannerDemandPart.c_str(), nBomGPD, &gammaMaxVector);
    for (t=earlyPeriod; t<latePeriod; t++)  {
      gammaMaxVector[t] = gammaMax;
    }
    witSetBomEntryConsRate(theWitRun, geoPlannerDemandPart.c_str(), nBomGPD, gammaMaxVector);
    witFree(gammaMaxVector);
    
    // No use for gammaMin in the proportional split mode

    return 0;
}



// --------------------------------------------------------------------------

// Add part(s) and any structure to wit for BB Category parts (BB/feature set)
// REturn --->  0  if succesful
//             -1  if un-succesful with warning
//             >0  if Severe error
int
LgFrInitialProblemForSce::sceAddBbCategoryPartAndStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & plannerTopLevelPart, 
    const std::string & geo,
    const std::string & bbCategory, 
    float gammaMax,
    float gammaMin,
    int early,
    int late,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    int nPeriods;

    witGetNPeriods(theWitRun, &nPeriods);

    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    // get a unique part name for the bbCategory
    std::string fullBbCategoryPart = mpHelper.bbCategoryPartName(plannerTopLevelPart, geo,
							       bbCategory);

    // Now check to see if the  bbCategory does not already exist
    if (mpHelper.isPartValid(theWitRun, fullBbCategoryPart, fileName, 
			       dataLine, lineNo, PURELY_OPTIONAL)) {
      (*theErrFacil)("BBCategoryAlreadyDefined",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << plannerTopLevelPart 
		     << geo 
		     << bbCategory 
		     << fullBbCategoryPart 
		     << dataLine);
	return -1;
    }


    // make sure the plannerTopLevel Special Demand Part exists
    std::string geoPlannerDemandPart = mpHelper.geoPlannerDemandPartName(plannerTopLevelPart, geo);
    if (! mpHelper.isPartValid(theWitRun, geoPlannerDemandPart, fileName, 
			       dataLine, lineNo, PURELY_OPTIONAL))   {
      (*theErrFacil)("PartDoesNotExist",MclArgList() 
		     << plannerTopLevelPart 
		     << geo 
		     << fileName 
		     << bbCategory 
		     << (int)lineNo 
		     << dataLine);
      return -1;
    }

    LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();
   
    // get the default stock and scrap penalty for featureSet part
    float featureSetStockPenalty = setOfParameters.getFloat("featureSetStockPenalty");
    float featureSetScrapPenalty = setOfParameters.getFloat("featureSetScrapPenalty");    


    float * featureSetStockPenaltyVec = new float[nPeriods];
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++)  
	featureSetStockPenaltyVec[t] = featureSetStockPenalty;

    float * featureSetScrapPenaltyVec = new float[nPeriods];
    for (t=0; t<nPeriods; t++)  
	featureSetScrapPenaltyVec[t] = featureSetScrapPenalty;


    // Add the BB category  part to WIT
    witAddPartWithOperation( theWitRun, fullBbCategoryPart.c_str() );
    if ( lpModelHelper_ != 0) {
      witSetPartObj1StockCost( theWitRun, fullBbCategoryPart.c_str(), featureSetStockPenaltyVec );
      witSetPartObj1ScrapCost( theWitRun, fullBbCategoryPart.c_str(), featureSetScrapPenaltyVec );
    }


    delete [] featureSetScrapPenaltyVec;
    delete [] featureSetStockPenaltyVec;

    // Now add the BOM structure between topLevelPart and bbCategory
    witAddBomEntry(theWitRun, geoPlannerDemandPart.c_str(), fullBbCategoryPart.c_str());

    

    // Add the LT and GT capacity parts to WIT (they shouldn't already exist)
    std::string LTbbCapacityPartName = mpHelper.LTbbCapacityPartName(plannerTopLevelPart, geo,
								   bbCategory);
    std::string GTbbCapacityPartName = mpHelper.GTbbCapacityPartName(plannerTopLevelPart, geo,
								   bbCategory);
    assert (! mpHelper.isPartValid(theWitRun, LTbbCapacityPartName, fileName, 
				   dataLine, lineNo, PURELY_OPTIONAL));
    assert (! mpHelper.isPartValid(theWitRun, GTbbCapacityPartName, fileName, 
				   dataLine, lineNo, PURELY_OPTIONAL));

    float bigFloat = 1000000000000.0;
    float * infiniteCapacity = new float[nPeriods];
    for (t=0; t<nPeriods; t++)
      infiniteCapacity[t] = bigFloat;
    

    witAddPart(theWitRun, LTbbCapacityPartName.c_str(), WitCAPACITY);
    witAddPart(theWitRun, GTbbCapacityPartName.c_str(), WitCAPACITY);

    witSetPartSupplyVol(theWitRun, GTbbCapacityPartName.c_str(), infiniteCapacity);
    witSetPartSupplyVol(theWitRun, LTbbCapacityPartName.c_str(), infiniteCapacity);

    delete [] infiniteCapacity;    


    // add the BOM structure between the featureSetPart and LT and GT

    float * gammaVec = new float[nPeriods];
    witAddBomEntry(theWitRun, fullBbCategoryPart.c_str(), LTbbCapacityPartName.c_str());

    for (t=0; t<nPeriods; t++)  
	gammaVec[t] = gammaMin;

    witSetBomEntryConsRate(theWitRun, fullBbCategoryPart.c_str(), 0, gammaVec);

    witAddBomEntry(theWitRun, fullBbCategoryPart.c_str(), GTbbCapacityPartName.c_str());

    for (t=0; t<nPeriods; t++)  
	gammaVec[t] = -gammaMax;

    witSetBomEntryConsRate(theWitRun, fullBbCategoryPart.c_str(), 1, gammaVec);

    delete [] gammaVec;





    return 0;
}



//  FEATURE_SETS_VIA_PROPORTIONAL_ROUTING
// this is what is used for engine not = optimization
#ifdef VARIABLE_PERIODS

// FINISH_ME: for variable periods, need to address all instances of MFO
#endif
int
LgFrInitialProblemForSce::sceAddBBtoOptionBomStructureViaProportionalRoute(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar & theCal,
    const std::string & plannerTopLevelPart, 
    const std::string & geo,
    const std::string & bbCategory, 
    const std::string & mfgOptionPart, 
    float usageRate,
    float * featureRatio,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    int nPeriods;
    witGetNPeriods(theWitRun, &nPeriods);

    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();        
    LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();

    // determine if the option is real or is a nullOption
    bool nullOption = false;
    if (0 == sce::compareStringsCaseIns( mfgOptionPart, "nullOption" ))
      nullOption = true;
    
    // get the full wit name for the bbCategory
    std::string fullBbCategoryPart = mpHelper.bbCategoryPartName(plannerTopLevelPart, geo, 
							       bbCategory);

    // Now make sure that the bbCategory part already exists
    if (! mpHelper.isPartValid(theWitRun, fullBbCategoryPart, fileName, 
			       dataLine, lineNo, PURELY_OPTIONAL)) {
      (*theErrFacil)("BBCategoryNotDefined",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << plannerTopLevelPart 
		     << geo
		     << bbCategory
		     << mfgOptionPart
		     << fullBbCategoryPart
		     << dataLine);
      return -1;
    }

    //  Determine the first period that has non-Zero Ratio.
    int firstPeriodWithRatio = nPeriods;
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++)
	if (featureRatio[t] > 0.0)  {
	    firstPeriodWithRatio = t;
	    break;
	}

    if (firstPeriodWithRatio == nPeriods)  {
      (*theErrFacil)("OptionAllZeroRatios",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << plannerTopLevelPart 
		     << geo 
		     << bbCategory 
		     << mfgOptionPart 
		     << fullBbCategoryPart 
		     << dataLine);

	return -1;
    }
	    
	    
    // get a unique name for the SOD (Special Option Dummy) operation
    std::string optionDummy = mpHelper.optionDummyPartName(plannerTopLevelPart, geo,
							 bbCategory, mfgOptionPart);


    // see if SOD operation already exists
    if ( mpHelper.isOperationValid(theWitRun, optionDummy, fileName, 
			      dataLine, lineNo, PURELY_OPTIONAL))     {
      (*theErrFacil)("OptionRatiosAlreadySpecified",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << plannerTopLevelPart 
		     << geo << bbCategory 
		     << mfgOptionPart 
		     << fullBbCategoryPart 
		     << dataLine);
      return -1;
    }



    // -------------------------++++++++++++++++++
    // NOTE: We assume that GPD's are single sourced (this is true for Server Group
    // determine the PDF and then the mfgOptionPart that the feature sources to.

    int sourceBeginPeriod = firstPeriodWithRatio;
    int sourceEndPeriod = nPeriods - 1;


    // derive the mfg part for the option (the real part)
#ifdef VARIABLE_PERIODS

    LgFrTimeVecFloat transitTimeOffsetTV(nPeriods, 0.0);
    std::string mfgTopLevelPart = mpHelper.demandSource(theWitRun,
							plannerTopLevelPart,
							geo,
							sourceBeginPeriod,
							sourceEndPeriod,
							transitTimeOffsetTV,
							fileName,
							dataLine,
							lineNo,
							MANDATORY);
#else
    float transitTimeScaled;
    std::string mfgTopLevelPart = mpHelper.demandSource(theWitRun,
						      plannerTopLevelPart,
						      geo,
						      sourceBeginPeriod,
						      sourceEndPeriod,
						      transitTimeScaled,
						      fileName,
						      dataLine,
						      lineNo,
						      MANDATORY);
#endif



    // determine the pdf for this source
    std::string pdf(mpHelper.pdf(mfgTopLevelPart));
    
    // get the full wit name for the real option part
    std::string fullOptionPartName(mpHelper.pdfPartname(mfgOptionPart, pdf));
// -------------------------++++++++++++++++++

    
    // test to see if the real option part exists at the pdf.  IF NOT,
    // then we can't just ignore the record because we've already created
    // half the structure.  Hence, we bow out gracefully and treat this
    // particular pdf source as if it were a nullOption (ie, always
    // meetable from thin air).  
    if (! mpHelper.isPartValid(theWitRun, fullOptionPartName, fileName, 
			       dataLine, lineNo, PURELY_OPTIONAL))  {

      (*theErrFacil)("CannotSourceFeature",MclArgList() 
		     << fileName
		     << (int)lineNo
		     << plannerTopLevelPart
		     << geo
		     << mfgTopLevelPart
		     << bbCategory
		     << mfgOptionPart
		     << pdf
		     << mfgOptionPart
		     << sourceBeginPeriod
		     << sourceEndPeriod
		     << dataLine);
      return -1;
    }

    

   
    // -----------------------------------
    // add OPERATION:   SOD 
    // -----------------------------------
    witAddOperation(theWitRun, optionDummy.c_str());

    if (! nullOption) {
      // Set the execCost as the option reward
      if ( lpModelHelper_ != 0 )  {
        // setup the default option Revenue

        float defaultOptionShipReward = setOfParameters.getFloat("defaultOptionShipReward");

        float * optionDummyExecCost = new float[nPeriods];
        int t = 0; // Pulled out of the for below by RW2STL
        for (t=0; t<nPeriods; t++)  
            optionDummyExecCost[t] = -defaultOptionShipReward;
        witSetOperationObj1ExecCost( theWitRun, optionDummy.c_str(), optionDummyExecCost );
        delete [] optionDummyExecCost;
      }
    }
    
    // -------------------------
    // add BOP: BBCat --> SOD
    // -------------------------
    int nBopSOD;
    witGetOperationNBomEntries(theWitRun, optionDummy.c_str(), &nBopSOD);
    witAddBopEntry(theWitRun, optionDummy.c_str(), fullBbCategoryPart.c_str());

    LgFrTimeVecFloat routingShare(nPeriods, featureRatio);
    LgFrTimeVecFloat scaledRoutingShare(routingShare * 1000.0);
    float * sodYieldRate;
    witGetOperationYieldRate(theWitRun, optionDummy.c_str(), &sodYieldRate);
    // Note: Adjustment for periods with ratio=zero
    //     Make yieldRate(SOD)[t] = 0 and routingShare=1
    for (t=0; t<nPeriods; t++) {
      if (scaledRoutingShare[t] < 1.0) {
	sodYieldRate[t] = 0.0;
	scaledRoutingShare[t] = 1.0;
      }
    }

    witSetBopEntryRoutingShare(theWitRun, optionDummy.c_str(), nBopSOD, scaledRoutingShare.data());
    witSetOperationYieldRate(theWitRun, optionDummy.c_str(), sodYieldRate);
    witFree(sodYieldRate);


    // --------------------
    // add Bom: SOD -> Real Option part
    // --------------------
    int nBomSOD;
    witGetOperationNBomEntries(theWitRun, optionDummy.c_str(), &nBomSOD);
    witAddBomEntry(theWitRun, optionDummy.c_str(), fullOptionPartName.c_str());
    LgFrTimeVecFloat usageRateVector(nPeriods, usageRate);
    //    witSetBomEntryUsageRate(theWitRun, optionDummy.c_str(), nBomSOD, usageRate);
    witSetBomEntryConsRate(theWitRun, optionDummy.c_str(), nBomSOD, usageRateVector.data());


    // ------------------------------------------------------------------------
    // Compute Bom Offsets: transitTime(pdf->geoSource) + cycleTime(mfgTopLevelPart)
    // ------------------------------------------------------------------------
#ifdef VARIABLE_PERIODS
    LgFrTimeVecFloat mfgTopLevelCycleTime = mpHelper.getCycleTime(theWitRun, mfgTopLevelPart);
    LgFrTimeVecFloat optionOffset(nPeriods, 0.0);
    int ttt = 0;
    for (ttt=0; ttt<nPeriods; ttt++) {
      optionOffset[ttt] = mfgTopLevelCycleTime[ttt] - transitTimeOffsetTV[ttt];
    }
    witSetBomEntryOffset(theWitRun, optionDummy.c_str(), nBomSOD, optionOffset.data());
#else
    float * mfgTopLevelCycleTime;
    witGetPartCycleTime(theWitRun, mfgTopLevelPart.c_str(), &mfgTopLevelCycleTime);
    int ttt = 0;
    for (ttt=0; ttt<nPeriods; ttt++) {
      mfgTopLevelCycleTime[ttt] -= transitTimeScaled;
    }
    //    witSetBomEntryOffset(theWitRun, optionDummy.c_str(), nBomSOD, mfgTopLevelCycleTime);
    witFree(mfgTopLevelCycleTime);
#endif
    return 0;
}



//  SCE 6.1
int
LgFrInitialProblemForSce::sceAddBBtoOptionBomStructureViaProportionalRoute61(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar & theCal,
    const std::string  & plannerTopLevelPart, 
    const std::string  & geo,
    const std::string  & bbCategory, 
    const std::string  & mfgOptionPart, 
    float                usageRate,
    float                attachRate,
    float                mfOffsetVal,
    int                  mfoPhantomFlag,
    float                mfSupplyVal,
    int                  earlyPeriod,
    int                  latePeriod,
    int                  requestPeriod,
    const std::string  & fileName,
    const std::string  & dataLine,
    long                 lineNo)
{
  int t;
  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);
  
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();        
  LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();
  
  // determine if the option is real or is a nullOption
  bool nullOption = false;
  if (0 == sce::compareStringsCaseIns( mfgOptionPart, "nullOption" ))
    nullOption = true;
    

  std::string fullBbCategoryPart = mpHelper.bbCategoryPartName(plannerTopLevelPart, geo, bbCategory);
  if (! mpHelper.isPartValid(theWitRun, fullBbCategoryPart, fileName, dataLine, lineNo, PURELY_OPTIONAL)) {
    (*theErrFacil)("BBCategoryNotDefined",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart << geo
		   << bbCategory << mfgOptionPart << fullBbCategoryPart << dataLine);
    return -1;
  }

  std::string optionDummy = mpHelper.optionDummyPartName(plannerTopLevelPart, geo, bbCategory, mfgOptionPart);
  if ( mpHelper.isOperationValid(theWitRun, optionDummy, fileName, dataLine, lineNo, PURELY_OPTIONAL))     {
    (*theErrFacil)("OptionRatiosAlreadySpecified",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart 
		   << geo << bbCategory << mfgOptionPart << fullBbCategoryPart << dataLine);
    return -1;
  }

  std::string sourceLocPdf;
  // determine the full wit name for the Box at the sourceLocPdf
  std::string mfgBoxPartName = mpHelper.mfgTopPartForGpd(theWitRun, plannerTopLevelPart, geo, sourceLocPdf);
    
  std::string fullOptionPartName(mpHelper.pdfPartname(mfgOptionPart, sourceLocPdf));
  if (! mpHelper.isPartValid(theWitRun, fullOptionPartName, fileName, 
			     dataLine, lineNo, PURELY_OPTIONAL))  {
    (*theErrFacil)("CannotSourceFeature",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart << geo << mfgBoxPartName 
		   << bbCategory << mfgOptionPart << sourceLocPdf << mfgOptionPart << earlyPeriod << latePeriod << dataLine);
    return -1;
  }

  // -----------------------------------
  // add OPERATION:   SOD 
  // -----------------------------------
  witAddOperation(theWitRun, optionDummy.c_str());

  if (! nullOption) {
    // Set the execCost as the option reward
    if ( lpModelHelper_ != 0 )  {
      float defaultOptionShipReward = setOfParameters.getFloat("defaultOptionShipReward");
      float * optionDummyExecCost = new float[nPeriods];
      int t = 0; 
      for (t=0; t<nPeriods; t++)  
	optionDummyExecCost[t] = -defaultOptionShipReward;
      witSetOperationObj1ExecCost( theWitRun, optionDummy.c_str(), optionDummyExecCost );
      delete [] optionDummyExecCost;
    }
  }
    
  // -------------------------
  // add BOP: BBCat --> SOD
  // -------------------------
  int nBopSOD;
  witGetOperationNBomEntries(theWitRun, optionDummy.c_str(), &nBopSOD);
  witAddBopEntry(theWitRun, optionDummy.c_str(), fullBbCategoryPart.c_str());

  LgFrTimeVecFloat routingShare(nPeriods, attachRate);
  LgFrTimeVecFloat scaledRoutingShare(routingShare * 1000.0);
  float * sodYieldRate;
  witGetOperationYieldRate(theWitRun, optionDummy.c_str(), &sodYieldRate);
  // Note: Adjustment for periods with ratio=zero
  //     Make yieldRate(SOD)[t] = 0 and routingShare=1
  for (t=0; t<nPeriods; t++) {
    if (scaledRoutingShare[t] < 1.0) {
      sodYieldRate[t] = 0.0;
      scaledRoutingShare[t] = 1.0;
    }
  }
  witSetBopEntryRoutingShare(theWitRun, optionDummy.c_str(), nBopSOD, scaledRoutingShare.data());
  witSetOperationYieldRate(theWitRun, optionDummy.c_str(), sodYieldRate);
  witFree(sodYieldRate);

  // --------------------
  // add Bom: SOD -> Real Option part
  // --------------------
  int nBomSOD;
  witGetOperationNBomEntries(theWitRun, optionDummy.c_str(), &nBomSOD);
  witAddBomEntry(theWitRun, optionDummy.c_str(), fullOptionPartName.c_str());
  float * consRate;
  witGetBomEntryConsRate(theWitRun, optionDummy.c_str(), nBomSOD, &consRate);
  for (t=earlyPeriod; t<=latePeriod; t++)
    consRate[t] = usageRate;
  witSetBomEntryConsRate(theWitRun, optionDummy.c_str(), nBomSOD, consRate);
  witFree(consRate);

  // -------------------------------------------------
  // MFSupply:  supplyVol(optionDummy)  += mfSupplyVal
  // -------------------------------------------------
  // FINISH_ME_61:  we need a SOD part to add supply on, this modelling trick only has an SOD operation. 
#if 0  
  if (mfSupplyVal > 0.0) {
    float * mfSupplyVec;
    witGetPartSupplyVol(theWitRun, optionDummy.c_str(), &mfSupplyVec);
    mfSupplyVec[requestPeriod] += mfSupplyVal;
    witSetPartSupplyVol(theWitRun, optionDummy.c_str(), mfSupplyVec);
    witFree(mfSupplyVec);
  }
#endif
    

  // ------------------------------------------------------------------------
  // Bom Offset(optionDummy->fullOptionPart)  = mfgBoxCycleTimeDays + mfOffset - (mfoPhantomFlag * mfgOptionPartCycleTimeDays)
  // ------------------------------------------------------------------------
  // FINISH_ME_61: doesn't work for duplicate records
  LgFrTimeVecFloat offsetTV(nPeriods, 0.0);
  LgFrTimeVecFloat mfgBoxCycleTimeDays = mpHelper.getCycleTimeDays(theWitRun, mfgBoxPartName);
  if (mfoPhantomFlag) {
    LgFrTimeVecFloat optionCycleTimeDays = mpHelper.getCycleTimeDays(theWitRun, fullOptionPartName);
    for (t=earlyPeriod; t<=latePeriod; t++) {
      offsetTV[t] = mpHelper.calculateVariableOffset(mfgBoxCycleTimeDays[t] + mfOffsetVal - optionCycleTimeDays[t], t, theCal, nPeriods);
    }
  }
  else   {
    for (t=earlyPeriod; t<=latePeriod; t++) {
      offsetTV[t] = mpHelper.calculateVariableOffset(mfgBoxCycleTimeDays[t] + mfOffsetVal, t, theCal, nPeriods);
    }
  }
  witSetBomEntryOffset(theWitRun, optionDummy.c_str(), nBomSOD, offsetTV.data());

  return 0;
}




// ==============================================================
//
// This method is used for engine=optimization 
//
// FINISH_ME: add modification for VARIABLE_PERIODS
// 
// Add the feature to a BB SET and set up the ratio logic
//
//  This routine takes plannerTopLevelPart, a geo, a bbCategory, and a
//  optionPart (which is assumed to be a mfgPart)
//  and creates some dummy parts and structure:
//     -- a dummy part (the option dummy) is created for the optionPart
//     -- the real optionPart part hangs off of this option dummy
//     -- there may be multiple option parts if the geoPlannerPart is
//        multiply sourced.
//     -- another dummy part is created to represent the option ratio supply
//     -- the option ratio supply part also hangs off the option dummy
//     -- the LT and GT capacity parts (which are assocaited with the
//        bbCategory for this plannerTopLevel/geo, and, which should already
//        have been created)  also hang off the option dummy
//
//   NOTE:  If the mfgOptionPart is "nullOption"  (case-insensitive), then
//          we do not source the part.  (ie, we do not try to find a real
//          pdf,part called nullOption.  Instead we just leave the option dummy
//          with one child.)  In this case, we also do not give any "reward"
//          for the production of a nullOption.
int
LgFrInitialProblemForSce::sceAddBBtoOptionBomStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar & theCal,
    const std::string & plannerTopLevelPart, 
    const std::string & geo,
    const std::string & bbCategory, 
    const std::string & mfgOptionPart, 
    float usageRate,
    const float * featureRatio,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    int nPeriods;
    witGetNPeriods(theWitRun, &nPeriods);

    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    // determine if the option is real or is a nullOption
    bool nullOption = false;
    if (0 == sce::compareStringsCaseIns( mfgOptionPart, "nullOption" ))
      nullOption = true;
    
    // get the full wit name for the bbCategory
    std::string fullBbCategoryPart = mpHelper.bbCategoryPartName(plannerTopLevelPart, geo, 
							       bbCategory);

    // Now make sure that the bbCategory part already exists
    if (! mpHelper.isPartValid(theWitRun, fullBbCategoryPart, fileName, 
			       dataLine, lineNo, PURELY_OPTIONAL)) {
      (*theErrFacil)("BBCategoryNotDefined",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << plannerTopLevelPart 
		     << geo
		     << bbCategory
		     << mfgOptionPart
		     << fullBbCategoryPart
		     << dataLine);
      return -1;
    }


    //  Determine the first period that has non-Zero Ratio.
    int firstPeriodWithRatio = nPeriods;
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++)
	if (featureRatio[t] > 0.0)  {
	    firstPeriodWithRatio = t;
	    break;
	}

    if (firstPeriodWithRatio == nPeriods)  {
      (*theErrFacil)("OptionAllZeroRatios",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << plannerTopLevelPart 
		     << geo 
		     << bbCategory 
		     << mfgOptionPart 
		     << fullBbCategoryPart 
		     << dataLine);

	return -1;
    }


    LgFrTimeVecFloat usageRateVector(nPeriods, usageRate);
	    
    // get a unique name for the option dummy  part
    std::string optionDummy = mpHelper.optionDummyPartName(plannerTopLevelPart, geo,
							 bbCategory, mfgOptionPart);

    // get a unique name for the option Ratio Supply part
    std::string optionRatioSupply = 
	mpHelper.optionRatioSupplyPartName(plannerTopLevelPart, geo,
					   bbCategory, mfgOptionPart);

    // see if option dummy already exists
    if ( mpHelper.isPartValid(theWitRun, optionDummy, fileName, 
			      dataLine, lineNo, PURELY_OPTIONAL))     {
      (*theErrFacil)("OptionRatiosAlreadySpecified",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << plannerTopLevelPart 
		     << geo << bbCategory 
		     << mfgOptionPart 
		     << fullBbCategoryPart 
		     << dataLine);
      return -1;
    }


    // if the option dummy does not exist, then we must create the initial
    // structures in WIT.  Otherwise, we can skip over and just reset some 
    // ratios.

    // since the option dummy and the option ratio supply are created as
    // a pair, assert that the option ratio dummy doesn't already exist
    assert(! mpHelper.isPartValid(theWitRun, optionRatioSupply, fileName, 
				  dataLine, lineNo, PURELY_OPTIONAL));

    // create the option dummy part in WIT.  If its a nullOption
    // then we let the default prodCost of zero happen.  Otherwise
    // get the default prodReward for shipping this option
    if (nullOption) {
      witAddPartWithOperation(theWitRun, optionDummy.c_str());
    }
    else {
      LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();
      float defaultOptionShipReward = setOfParameters.getFloat("defaultOptionShipReward");
      
      float * optionDummyProdCost = new float[nPeriods];
      for (t=0; t<nPeriods; t++)  
	optionDummyProdCost[t] = -defaultOptionShipReward;
      
    
      // create the option dummy part in WIT
      witAddPartWithOperation( theWitRun, optionDummy.c_str() );

      // in wit4.0+ land, the old prod costs have been reborn as exec costs
      if ( lpModelHelper_ != 0 )
        witSetOperationObj1ExecCost( theWitRun, optionDummy.c_str(), optionDummyProdCost );


      delete [] optionDummyProdCost;
    }



    // now add a bom entry from bbCategory part to the optionDummy
    // this is done solely for making the structure  work in MRP
    // mode and heuristic mode.  In optimization mode, these bom
    // arcs invalidate the model.  We get around this in the routine
    // called reStructureForOptimization(), where these bom usage
    // rates are set to 0.0.

    // The strategy is to set the usage rates on these bom entries to
    // the feature ratio. 
    int nBom;
    witGetOperationNBomEntries(theWitRun, fullBbCategoryPart.c_str(), &nBom);
    witAddBomEntry(theWitRun, fullBbCategoryPart.c_str(), optionDummy.c_str());
    witSetBomEntryConsRate(theWitRun, fullBbCategoryPart.c_str(), nBom, featureRatio);
       
    // get the stock cost for this part
    LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();
    float optionRatioSupplyStockPenalty =
      setOfParameters.getFloat("optionRatioSupplyStockPenalty");
    

    // create the option ratio supply  part in WIT
    //      1) set up a stock cost using the default
    //      2) set the supplyVec to be the feature ratio over
    //         the effective periods.  NOTE: later on we'll do 
    //         an mrp to explode the demand and populate the final
    //         supplyVols
    float * optionRatioSupplyStockPenaltyVec = new float[nPeriods];
    for (t=0; t<nPeriods; t++)  
	optionRatioSupplyStockPenaltyVec[t] = optionRatioSupplyStockPenalty;
    
    witAddPart( theWitRun, optionRatioSupply.c_str(), WitMATERIAL );
    //     witAddPartWithOperation( theWitRun, optionRatioSupply.c_str() );
    if ( lpModelHelper_ != 0 )
      witSetPartObj1StockCost( theWitRun, optionRatioSupply.c_str(),
                               optionRatioSupplyStockPenaltyVec );


    delete [] optionRatioSupplyStockPenaltyVec;

    
    // add Bom: from optionDummy ---> optionRatioSupply
    witAddBomEntry(theWitRun, optionDummy.c_str(), optionRatioSupply.c_str());


    // add Bom:  optionDummy -->  Real Option Part, FOR EACH GE0-->PDF source!!
    // This operation is NOT done for nullOption.
    // NOTE:  The usage time must "equal" the cumulative lead time
    // associated with the pdf->geo transit time and the cycleTime
    // on the mfgTopLevelPart.  
     
    if (! nullOption) {
      int sourceBeginPeriod = firstPeriodWithRatio;
      int sourceEndPeriod = nPeriods - 1;
#ifdef VARIABLE_PERIODS
      LgFrTimeVecFloat transitTimeOffsetTV(nPeriods, 0.0);
#else
      float transitTimeScaled;
#endif
      while (sourceBeginPeriod < nPeriods) {
#ifdef VARIABLE_PERIODS
        std::string mfgTopLevelPart = mpHelper.demandSource(theWitRun,
							    plannerTopLevelPart,
							    geo,
							    sourceBeginPeriod,
							    sourceEndPeriod,
							    transitTimeOffsetTV,
							    fileName,
							    dataLine,
							    lineNo,
							    MANDATORY);
#else
        std::string mfgTopLevelPart = mpHelper.demandSource(theWitRun,
							    plannerTopLevelPart,
							    geo,
							    sourceBeginPeriod,
							    sourceEndPeriod,
							    transitTimeScaled,
							    fileName,
							    dataLine,
							    lineNo,
							    MANDATORY);
#endif
        // determine the pdf for this source
        std::string pdf(mpHelper.pdf(mfgTopLevelPart));
	
        // get the full wit name for the real option part
        std::string fullOptionPartName(mpHelper.pdfPartname(mfgOptionPart, pdf));
	
        // test to see if the real option part exists at the pdf.  IF NOT,
        // then we can't just ignore the record because we've already created
        // half the structure.  Hence, we bow out gracefully and treat this
        // particular pdf source as if it were a nullOption (ie, always
        // meetable from thin air).  
        if (! mpHelper.isPartValid(theWitRun, fullOptionPartName, fileName, 
                                   dataLine, lineNo, PURELY_OPTIONAL))  {
	  
	  (*theErrFacil)("CannotSourceFeature",MclArgList() 
			 << fileName
			 << (int)lineNo
			 << plannerTopLevelPart
			 << geo
			 << mfgTopLevelPart
			 << bbCategory
			 << mfgOptionPart
			 << pdf
			 << mfgOptionPart
			 << sourceBeginPeriod
			 << sourceEndPeriod
			 << dataLine);
          sourceBeginPeriod = sourceEndPeriod + 1;
          continue;
        }

        std::string geoPlannerDemandPart = mpHelper.geoPlannerDemandPartName(plannerTopLevelPart, geo);

	int nBom;
	witGetOperationNBomEntries(theWitRun, optionDummy.c_str(), &nBom);
	witAddBomEntry(theWitRun, optionDummy.c_str(), fullOptionPartName.c_str());
	witSetBomEntryEarliestPeriod(theWitRun, optionDummy.c_str(), nBom, sourceBeginPeriod);
	witSetBomEntryLatestPeriod(theWitRun, optionDummy.c_str(), nBom, sourceEndPeriod);
	witSetBomEntryConsRate(theWitRun, optionDummy.c_str(), nBom, usageRateVector.data());
	witSetBomEntryMandEC(theWitRun, optionDummy.c_str(), nBom, WitTRUE);
	// now compute offset
#ifdef VARIABLE_PERIODS
	LgFrTimeVecFloat mfgTopLevelCycleTime = mpHelper.getCycleTime(theWitRun, mfgTopLevelPart);
	LgFrTimeVecFloat optionOffset(nPeriods, 0.0);
	int ttt = 0;
	for (ttt=0; ttt<nPeriods; ttt++) {
	  optionOffset[t] = mfgTopLevelCycleTime[t] - transitTimeOffsetTV[t];
	}
	witSetBomEntryOffset(theWitRun, optionDummy.c_str(), nBom, optionOffset.data());
#else
        float * mfgTopLevelCycleTime;
        witGetPartCycleTime(theWitRun, mfgTopLevelPart.c_str(), &mfgTopLevelCycleTime);
	int ttt = 0;
	for (ttt=0; ttt<nPeriods; ttt++) {
	  mfgTopLevelCycleTime[ttt] -= transitTimeScaled;
	}
	witSetBomEntryOffset(theWitRun, optionDummy.c_str(), nBom, mfgTopLevelCycleTime);
        witFree(mfgTopLevelCycleTime);
#endif
        // Now reset the sourceBeginPeriod to bb the period after the  sourceEndPeriod
        sourceBeginPeriod = sourceEndPeriod + 1;
      }
    }


    // add the Bom from optionDummy to the LT and GT FeatureSetCapacity
    // parts.  Remember that the LT gets a negative usage rate!
    std::string myLTpart = mpHelper.LTbbCapacityPartName(plannerTopLevelPart, geo,
						       bbCategory);
    
    std::string myGTpart = mpHelper.GTbbCapacityPartName(plannerTopLevelPart, geo,
						       bbCategory);

    float * vecOfOnes = new float[nPeriods];
    int ttt = 0;

    witGetOperationNBomEntries(theWitRun, optionDummy.c_str(), &nBom);

    witAddBomEntry(theWitRun, optionDummy.c_str(), myLTpart.c_str());
    for (ttt=0; ttt<nPeriods; ttt++) 
      vecOfOnes[ttt] = -1.0;
    witSetBomEntryConsRate(theWitRun, optionDummy.c_str(), nBom, vecOfOnes);
    

    witAddBomEntry(theWitRun, optionDummy.c_str(), myGTpart.c_str());
    for (ttt=0; ttt<nPeriods; ttt++) 
      vecOfOnes[ttt] = 1.0;
    witSetBomEntryConsRate(theWitRun, optionDummy.c_str(), nBom+1, vecOfOnes);

    delete [] vecOfOnes;

    //   Set the OptionRatioSupply Vols
    //      Re-set the supplyVol to be the feature ratio over
    //         the effective periods.  NOTE: later on we'll do 
    //         an mrp to explode the demand and populate the final
    //         supplyVols

    witSetPartSupplyVol(theWitRun, optionRatioSupply.c_str(), featureRatio);    
    return 0;
}


#ifdef BUILDING_BLOCKS_VIA_MULTI_BOPS
int
LgFrInitialProblemForSce::sceSetOptionRevenueViaMultiRoute(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & plannerTopLevelPart, 
    const std::string & geo,
    const std::string & mfgOptionPart, 
    float * featureRev,
    float periodicInterestRate,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
  // FINISH_ME:  detect if the option is a PURE_OPTION, or if it
  // is a GATING_OPTION.  If its a PURE_OPTION, then put the
  // option revenue, on the substitute arc to NULL_Substitute
  
  int result;
  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);

  bool foundMatch = false;
  
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  // make sure the GPD plannerTopLevel Special Demand Part exists
  std::string geoPlannerDemandPart = mpHelper.geoPlannerDemandPartName(plannerTopLevelPart, geo);
  if (! mpHelper.isOperationValid(theWitRun, geoPlannerDemandPart, fileName, 
                             dataLine, lineNo, PURELY_OPTIONAL))   {
    (*theErrFacil)("PartDoesNotExistb",MclArgList() 
		   << plannerTopLevelPart 
		   << geo 
		   << mfgOptionPart 
		   << fileName 
		   << (int)lineNo 
		   << dataLine);
    return -1;
  }

  // Look for the BBCat operation that calls the right option dummy
  int nBom;
  witGetOperationNBomEntries(theWitRun, geoPlannerDemandPart.c_str(), &nBom);
  int j = 0; // Pulled out of the for below by RW2STL
  for (j=0; j<nBom; j++) {
    char * child;
    witGetBomEntryConsumedPart(theWitRun, geoPlannerDemandPart.c_str(), j, &child);
    std::string pp, g, bb;
    if (mpHelper.isPartSpecialBbCategory(theWitRun, child, pp, g, bb)) {
      // now plug in the mfgOptionPart and create an optionDummy partname
      std::string optionDummy = mpHelper.optionDummyPartName(plannerTopLevelPart, geo, 
                                                           bb, mfgOptionPart);
      if (mpHelper.isOperationValid(theWitRun, optionDummy, fileName,
                               dataLine, lineNo, PURELY_OPTIONAL))   {
        if ( lpModelHelper_ != 0) 
          witSetOperationObj1ExecCost(theWitRun, optionDummy.c_str(), featureRev);
	witSetBopEntryExpAversion(theWitRun, optionDummy.c_str(), 0, -featureRev[0]);
        witFree(child);
        return 0;
      }
    }

    // otherwise, check to see if the child is a PURE Option.  That is, if the
    // bom entry has a substitute to a specialNullSubstitute part, then it is
    // a pure option.  The featureRev vector is applied to the obj1SubCost for
    // this subEntry.  Note, there may be multiple bom arcs associated with
    // this option (for featureRatio changes and for pdf source changes).  Therefore
    // don't immediately return after you find one.
    else if (mpHelper.isPartSpecialStandaloneFeature(theWitRun, child)) {      
      std::string childPart = mpHelper.partname(child);
      int nSubs;
      witGetBomEntryNSubsBomEntries(theWitRun, geoPlannerDemandPart.c_str(), j, &nSubs);
      if (nSubs == 1) {
        // note: feature revenue is negative.  Need to set it back to positive.
        int t = 0; // Pulled out of the for below by RW2STL
        for (t=0; t<nPeriods; t++)
          featureRev[t] = - featureRev[t];
        if (lpModelHelper_ != 0) 
          witSetSubsBomEntryObj1SubCost(theWitRun, geoPlannerDemandPart.c_str(), j, 0, featureRev);
        foundMatch = true;  
      }
    }
    
    witFree(child);

  }

  if (! foundMatch) {
    (*theErrFacil)("MissingMfgOptionPart",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << plannerTopLevelPart 
		   << geo 
		   << mfgOptionPart 
		   << dataLine);
    return -1;
  }
  return 0;
}
#else
int
LgFrInitialProblemForSce::sceSetOptionRevenue(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & plannerTopLevelPart, 
    const std::string & geo,
    const std::string & mfgOptionPart, 
    float * featureRev,
    float periodicInterestRate,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
  // FINISH_ME:  detect if the option is a PURE_OPTION, or if it
  // is a GATING_OPTION.  If its a PURE_OPTION, then put the
  // option revenue, on the substitute arc to NULL_Substitute
  
  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);

  bool foundMatch = false;
  
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  // make sure the plannerTopLevel Special Demand Part exists
  std::string geoPlannerDemandPart = mpHelper.geoPlannerDemandPartName(plannerTopLevelPart, geo);
  if (! mpHelper.isPartValid(theWitRun, geoPlannerDemandPart, fileName, 
                             dataLine, lineNo, PURELY_OPTIONAL))   {
    (*theErrFacil)("PartDoesNotExistb",MclArgList() 
		   << plannerTopLevelPart 
		   << geo 
		   << mfgOptionPart 
		   << fileName 
		   << (int)lineNo 
		   << dataLine);
    return -1;
  }

  // figure out a unique bbCategory name
  // use the nBom of the topLevel Planner Part
  // bbCategory name is:   STANDALONE:n
  // where n is the current number of BOM entries, NOTE that
  // the optional bbCategory parts will not necessarily be
  // sequential, nor will they start at 0.  But who cares.
  int nBom;
  witGetOperationNBomEntries(theWitRun, geoPlannerDemandPart.c_str(), &nBom);
  int j = 0; // Pulled out of the for below by RW2STL
  for (j=0; j<nBom; j++) {
    char * child;
    witGetBomEntryConsumedPart(theWitRun, geoPlannerDemandPart.c_str(), j, &child);
    std::string pp, g, bb;
    if (mpHelper.isPartSpecialBbCategory(theWitRun, child, pp, g, bb)) {
      // now plug in the mfgOptionPart and create an optionDummy partname
      std::string optionDummy = mpHelper.optionDummyPartName(plannerTopLevelPart, geo, 
                                                           bb, mfgOptionPart);
      if (mpHelper.isPartValid(theWitRun, optionDummy, fileName,
                               dataLine, lineNo, PURELY_OPTIONAL))   {
        if ( lpModelHelper_ != 0) 
          witSetPartObj1ProdCost(theWitRun, optionDummy.c_str(), featureRev);
        witFree(child);
        return 0;
      }
    }

    // otherwise, check to see if the child is a PURE Option.  That is, if the
    // bom entry has a substitute to a specialNullSubstitute part, then it is
    // a pure option.  The featureRev vector is applied to the obj1SubCost for
    // this subEntry.  Note, there may be multiple bom arcs associated with
    // this option (for featureRatio changes and for pdf source changes).  Therefore
    // don't immediately return after you find one.
    else if (mpHelper.isPartSpecialStandaloneFeature(theWitRun, child)) {      
      std::string childPart = mpHelper.partname(child);
      int nSubs;
      witGetBomEntryNSubsBomEntries(theWitRun, geoPlannerDemandPart.c_str(), j, &nSubs);
      if (nSubs == 1) {
        // note: feature revenue is negative.  Need to set it back to positive.
        int t = 0; // Pulled out of the for below by RW2STL
        for (t=0; t<nPeriods; t++)
          featureRev[t] = - featureRev[t];
        if (lpModelHelper_ != 0) 
          witSetSubsBomEntryObj1SubCost(theWitRun, geoPlannerDemandPart.c_str(), j, 0, featureRev);
        foundMatch = true;  
      }
    }
    
    witFree(child);

  }

  if (! foundMatch) {
    (*theErrFacil)("MissingMfgOptionPart",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << plannerTopLevelPart 
		   << geo 
		   << mfgOptionPart 
		   << dataLine);
    return -1;
  }
  return 0;
}
#endif
      
// ------------------------------------------------------------------
int
LgFrInitialProblemForSce::sceAddMachineFeatureSupplyVol(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & plannerTopLevelPart, 
    const std::string & geo,
    const std::string & mfgOptionPart, 
    float *  supplyVol,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{


  // + See comments on ::sceAddStdAloneOptionBomStructure for feature model
  
  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
  
  // make sure the plannerTopLevel Special Demand Part exists
  std::string geoPlannerDemandPart =
        mpHelper.geoPlannerDemandPartName(plannerTopLevelPart, geo);
  if (! mpHelper.isPartValid(theWitRun, geoPlannerDemandPart, fileName, 
                             dataLine, lineNo, PURELY_OPTIONAL))   {
    (*theErrFacil)("PartDoesNotExistb",MclArgList() 
		   << plannerTopLevelPart 
		   << geo 
		   << mfgOptionPart 
		   << fileName 
		   << (int)lineNo
		   << dataLine);
    return -1;
  }



  // determine demand sources (pdf's) (hopefully its single sourced)

  int start=0;
  int late=-1;
  while (start < nPeriods) {
    float offset; // not used but needed in method call
    std::string mfgTopLevelBuild = mpHelper.demandSource(theWitRun, 
						       plannerTopLevelPart,
						       geo,
						       start,
						       late,
						       offset,
						       fileName, 
						       dataLine, 
						       lineNo, 
						       PURELY_OPTIONAL);
    std::string pdf = mpHelper.pdf(mfgTopLevelBuild);


    // Now create a specialFeaturePartName of type Customer Choice and check its existence
    // Return a unique standalone Feature Part Name
    std::string specialFeaturePartName;
    
    specialFeaturePartName = mpHelper.custChoiceFeaturePartName(plannerTopLevelPart,
								geo,
								mfgOptionPart,
								pdf);
    witBoolean exists;
    witGetPartExists(theWitRun, specialFeaturePartName.c_str(), &exists);
    // if it doesn't exist, then try for a normal standalone 
    if (! exists) {
      // Now create a specialFeaturePartName of type Standalone and check its existence
      // Return a unique standalone Feature Part Name
      specialFeaturePartName = mpHelper.standaloneFeaturePartName(plannerTopLevelPart,
								  geo,
								  mfgOptionPart,
								  pdf);

      witGetPartExists(theWitRun, specialFeaturePartName.c_str(), &exists);
      if (! exists) {
	(*theErrFacil)("CannotFindFeatureRel",MclArgList() 
		       << fileName 
		       << (int)lineNo 
		       << plannerTopLevelPart 
		       << geo 
		       << mfgOptionPart 
		       << dataLine);
	return -1;
      }
    }

    // if you get to here, then the part exists and we go ahead and
    // set the feature supply vol (incrementally).  Note: for type
    // customerChoice, we will have to come back later and spread it!
    float * supVol;
    witGetPartSupplyVol(theWitRun, specialFeaturePartName.c_str(), &supVol);
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=start; t<=late; t++) {
      supVol[t] += supplyVol[t];
    }
    witSetPartSupplyVol(theWitRun, specialFeaturePartName.c_str(), supVol);
    witFree(supVol);

    start = late+1;
  }      

  return 0;

}


// ------------------------------------------------------------------
int
LgFrInitialProblemForSce::sceSetMachineFeatureOffset(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar & theCal,
    const std::string & plannerTopLevelPart, 
    const std::string & geo,
    const std::string & mfgOptionPart, 
    float offsetVal,
    int featurePhantomFlag,
    int start,
    int end,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{


  // + See comments on ::sceAddStdAloneOptionBomStructure for feature model
  
  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
  
  // make sure the plannerTopLevel Special Demand Part exists
  std::string geoPlannerDemandPart =
        mpHelper.geoPlannerDemandPartName(plannerTopLevelPart, geo);
  if (! mpHelper.isPartValid(theWitRun, geoPlannerDemandPart, fileName, 
                             dataLine, lineNo, PURELY_OPTIONAL))   {
    (*theErrFacil)("PartDoesNotExistb",MclArgList() 
		   << plannerTopLevelPart 
		   << geo 
		   << mfgOptionPart 
		   << fileName 
		   << (int)lineNo
		   << dataLine);
    return -1;
  }
  

  int nBomEntries;
  witGetOperationNBomEntries( theWitRun, geoPlannerDemandPart.c_str(), &nBomEntries );

  // Now find any bom arcs of fof the GPD that correspond to this 
  // machine feature record.  Note also the the overlaps of effectivities
  // and report errors/warning if necessary.

  bool didWeFindAValidFeatureRelationship = false;
  std::string pdf;
  int j = 0; // Pulled out of the for below by RW2STL
  for (j=0; j<nBomEntries; j++) {
    char * specialFeaturePartName;
    witGetBomEntryConsumedPart(theWitRun, geoPlannerDemandPart.c_str(), j, &specialFeaturePartName);
    std::string mfgop;

    // if its NOT a  special feature part then continue
    std::string gg, mm;
    // Returns 1 if operation is EITHER:
    //     special Standalone Feature, 
    //  OR  special Customer Choice Feature,
    if (! mpHelper.isOperationSpecialFeature(theWitRun, specialFeaturePartName, gg, mm, pdf, mfgop)) {
      witFree(specialFeaturePartName);
      continue;
    }

    // Now check to see that the mfgOptionPart of the record matches the 
    // mfgOptionPart of the specialFeaturePart.  IF it doesn't then continue.
    if (mfgop != mfgOptionPart) {
      witFree(specialFeaturePartName);
      continue;
    }
 
    // at this point you have a match with plannerPart.c_str(),geo.c_str(),mfgOptionPart.
    didWeFindAValidFeatureRelationship = true;


    // OVerride the bom offset between specialFeature PArt and the realFeaturePart 
    // but only over the effectivity  specified on the record.  

    float * offset;
    witGetBomEntryOffset(theWitRun, specialFeaturePartName, 0, &offset);
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=start; t<=end; t++) {
      offset[t] = offsetVal/theCal.workUnits(t);
    }

    // Now apply a phantome adjustment to the offset
    if (featurePhantomFlag) {
      std::string realFeaturePartName = mpHelper.pdfPartname(mfgop, pdf);
      witBoolean exists;
      witGetPartExists(theWitRun, realFeaturePartName.c_str(), &exists);
      assert(exists);
      float * realFeatureCycleTime;
      witGetPartCycleTime(theWitRun, realFeaturePartName.c_str(), &realFeatureCycleTime);
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=start; t<=end; t++) {
	offset[t] -= realFeatureCycleTime[t];
	// FINISH_ME: adjust for VARIABLE_PERIODS
      }
      witFree(realFeatureCycleTime);
    }
    witSetBomEntryOffset(theWitRun, specialFeaturePartName, 0, offset);

    witFree(offset);
    witFree(specialFeaturePartName);

  }

  // What to do if you can't find a valid Feature Relationship?
  if (! didWeFindAValidFeatureRelationship)   {
    (*theErrFacil)("CannotFindFeatureRel",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << plannerTopLevelPart 
		   << geo 
		   << mfgOptionPart 
		   << dataLine);
    return -1;
  }



  return 0;

}


// This is current solution for SCE 5.X
// ------------------------------------------------------------------
int
LgFrInitialProblemForSce::sceAddCustChoiceFeatureStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar & theCal,
    const std::string & plannerTopLevelPart, 
    const std::string & geo,
    const std::string & featurePart, 
    float usageRate,
    float * featureRatio,
    float * defaultGatingOptionSubPenalty,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{

  // + -------------------------------------------------
  // + SCE's CUstomer Choice Feature Model is as follows:
  // + -------------------------------------------------
  // + For each cust choice feature record with
  // +     plannerPart, geo, featurePart, f1,f2,...,fNp-1
  // +
  // + 0) We detect customer choice features as a special case of standalone features if the
  // +    maxWithout vector contains an element < 0.  It's kludgy, but hey.
  // +     a) if there  are positive and negative elements to this vector, then it is an error.  
  // +        Furthermore, SCE uses +1.0 as the default for maxwithout.  Therefore, if the 
  // +        feature record is to be a custChoice, then you must fully populate the record.  
  // +     b) For custchoice features, we must have 0.0 <= featratio[t] <= 1.0 for all periods t.
  // + 1) Add specialCustChoiceFeaturePart (one and only one dummy part per machine-feature relationship
  // + 2) Figure out what the realWitFeaturePart is by tracing the geoSource to a realWitMachine
  // +    part to get the PDF and then using the mpHelper to catenate featurePart and PDF.
  // +    This realWitFeaturePart must already exist.
  // +    NOTE: We currently do NOT support the customer choice feature model for Machines that have
  // +    more than one geo source (this is true even if the multiple geo source is changing over time).
  // +    This makes the implementation much easier (although its still pretty intense).
  // + 3) Add bom entry from specialFeaturePart to realWitFeaturePart
  // + 4) Add bom entry between GPD part and specialFeaturePart 
  // +    Set usageRate=1.0 (regardless of featureRatio).  
  // +    Store featureRatio vector in bomAppdata for this bom entry (gpd->sfp)
  // + 5) Add/Locate the custChoiceNullSubstitute part
  // +    If the nullSub part doesn't exist, then create it
  // + 6) Create a subBomEntry between the bom entry for GPD->specialFeaturePart.  Sub part is the
  // +    nullSubPart defined in step 5.
  // +    a) set supplyVol(custChoiceNullSubstitute) = (1-featratio) * mrpProdVol(GPD).
  // +       i) this is done by temporarily storing featratio as the supplyVol of the nullSub
  // +          and then doing an mrp after the model is built.  then we do the arithmetic and
  // +          reset the supplyVol in a method called scePreProcessViaMrp().
  // +    (Note that the featratio gets "stored" in two places during the building of the
  // +    model.  This is simply for convenience so that we don't have to go searching up and
  // +    down through structure to find it.  The storage in bomAppData(gpd->sfp) is permanent
  // +    and needed later in the processing of explosion, second pass implosion,  and reporting.)
  // +    Store featureRatio vector in bomAppdata for this bom entry (gpd->sfp)
  // + 7) Offsets:
  // +    Offsetting is the same for customer choice features and standalone features.
  // +    a)The geo source transit time is applied first:
  // +       bomOffset(GPD->realWitMachine) = geoSourcetransitTime
  // +       bomOffset(GPD->specialFeaturePart) = geoSourceTransitTime
  // +       subBomOffset(GPD->specialFeaturePart,custChoiceNullSubstitute) = geoSourceTransitTime
  // +    b) Now, since features are essentially disconnected from machines, we must ensure
  // +       that the features are subject to the machineCycleTime (in addition to any
  // +       geoSourceTransitTime):
  // +          bomOffset(specialFeaturePart->realWitFeaturePart) = machineCycleTime
  // +    c) Any machine-feature offset (set in a subsequent input file) 
  // +       is an override to the bom offset determined by machineCycleTime.  
  // +          bomOffset(specialFeaturePart->realWitFeaturePart) = machineFeatureOffset
  // +       Note: we do NOT let the mahcineFeatureOffset override the machineCycleTime 
  // +       (perhaps we should).
  // +    d) a phantom flag (set via machine-feature offset file) will cause the
  // +       offset between specialFeaturePart and realFeaturePart to be adjusted 
  // +       so that it cancels out the cycleTime on the real feature part.  
  // +          if (phantomFlag)
  // +             bomOffset(specialFeaturePart->realWitFeaturePart) -= cycleTime(realWitFeaturePart)
  // +    e) Note: the structure of specialFeaturePart and above is "above" the machine 
  // +       cycleTime and feature offsets.  This means that the featureRatio constraint
  // +       mechanism applies to the shipment of product out of the PDF (prior to 
  // +       geoSourceTransitTime).
  // +    f) (IBM NOTE) CycleTime on the topLevelMfgPart (machine at PDF) should not be used for ETIS type
  // +       feeds.  This is because the MTM,PDF will have no bom content, yet the cycleTime 
  // +       will gate anyway.  Plus, the ESAT extract will strictly feed all machine offset
  // +       via machine-feature offset. 
  // + 8) Add a demand to the specialFeaturePart.  This is for the two-pass heuristic. 
  // +    The demandVol is 0.0 for the first pass of heuristic.
  // +    The priority is copied over from the priority of the Machine demand in a later 
  // +    post-processing step.
  // + 9) This model does not work for MRP, so we make an adjustment in the exploder witRun.  
  // +    Namely, since the usageRate along the gpd->specialFeaturePart->realWitFeaturepart chain
  // +    of boms is 1.0 (regardless of featratio), we will be exploding too much demand.  So, to
  // +    remedy this, we add a dummy supply onto the specialFeaturePart.  This supply gets netted
  // +    at the top and explodes the right qty down the structure.  (note: we could have adjusted
  // +    the usageRate on the bom within the exploder witRun, but since feature ratio's can change
  // +    over time, this would have required multiple arcs -- yuck!!!).
  // +      supplyVol(exploderWitRun, specialFeaturePart) += (1-featureRatio) * mrpProdVol(GPD)
  // +    Note that we += this in as supply because there may already be supplyVol of the 
  // +    specialFeaturePart due to MachineFeature supply.
  // + 10) Compute Second Pass Feature Demand Vol:
  // +       Note: this is done after the "first pass" implosion run, in preperation for the second
  // +             pass (feature) implosion.
  // +             a) tiedBackExecVol(GPD): roll the execVol(GPD) (ie, first pass machine ships) back
  // +                                      to the period in which the demand was due.  We do this for
  // +                                      cases where the featureRatio changes over time.
  // +             b) featuresNeeded  = tiedBackExecVol(GPD) * featureRatio (this is the total features
  // +                                  that need to be shipped based on machine commits).
  // +             c) featuresShipped = consVol(specialFeaturePart) + excessVol(specialFeaturePart)
  // +             d) secondPassDemandVol = featuresNeeded - featuresShipped 
  // +             e) "Roll" secondPassDemandVol to carry negatives forward (can happen due to machine
  // +                 feature supply fen5, or weird offset problems).
  // + 
  // + 11) How to calculate OptionRatio output (ie, features shipped):  
  // +       optionRatio = consVol(specialFeaturePart) 
  // +                     + shipVol(specialFeaturePartDemand) 
  // +                     + excessVol(specialFeaturePart)
  // + 
  // + 12) The FSS model for this is, as yet, not implemented.  
  // +
  // + Additional Comments:
  // +   a) Base model is implemented and working.  
  // +   b) There are many special cases to be tested.  Further, these cases can work in a "unit"
  // +      test environment but not work when they occur together.  
  // +     1) ratio's changing over time
  // +     2) feature is primary  gating part
  // +     3) feature is short, but not primary gate.
  // +     4) offsets: transittime, machine cycleTime, mach->feat offset, fature cycletime, feature phantom
  // +   c) This model is similar to standalone features.  Major differences are:
  // +        1) this model can have ratio's change over time and not have to split the bill
  // +        2) we restrict multiple sourcing on the machines for these mach->feature records -- this is 
  // +           not the case for stdAlone features.
  // +        3) because of 1 and 2 above, we assert that there's one and only one:
  // +             gpd->sfp bom
  // +             gpd->sfp, nullSub subBom arc

  
  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
   
  // make sure the plannerTopLevel Special Demand Part exists
  std::string geoPlannerDemandPart =
        mpHelper.geoPlannerDemandPartName(plannerTopLevelPart, geo);
  if (! mpHelper.isPartValid(theWitRun, geoPlannerDemandPart, fileName, 
                             dataLine, lineNo, PURELY_OPTIONAL))   {
    (*theErrFacil)("PartDoesNotExistb",MclArgList() << plannerTopLevelPart << geo << featurePart << fileName << (int)lineNo << dataLine);
    return -1;
  }

  // FINISH_ME: look for any multiple souce opportunities for the GPD.
  // This is not supported and can screw things up later.  Thus we will
  // reject it the feature record here.
  

  // check to see if this record is a duplicate. If there's any 
  // relationship between the geoPlannerDemandPart and
  // a specialFeaturePart whose partName token is the featurePart,
  // then we reject the record.

  int nBomEntries;
  witGetOperationNBomEntries( theWitRun, geoPlannerDemandPart.c_str(), &nBomEntries );
  char * child;
  
  int j = 0; 
  for (j=0; j<nBomEntries; j++)  {
    witGetBomEntryConsumedPart( theWitRun, geoPlannerDemandPart.c_str(), j, &child );
    // Note check against ANY types of feature records
    if ( (featurePart == mpHelper.partname(child)) && mpHelper.isPartSpecialFeature(theWitRun, child)) {
      //    if ( mpHelper.isPartSpecialFeature(theWitRun, child)) {
      // uh-oh, we've got a duplicate record
      (*theErrFacil)("OptionAlreadyDefined",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart << geo << featurePart << dataLine);
      witFree ( child );
      return -1;
    }
    witFree ( child );    
  }    

  
  LgFrTimeVecFloat frTVF(nPeriods, featureRatio);

  if (frTVF.max() < 0.00001)  {
    (*theErrFacil)("OptionAllZeroRatiosPO",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart << geo << featurePart << dataLine);
    return -1;
  }

  if (frTVF.max() > 1.0)  {
    (*theErrFacil)("CustChoiceFeatRatioGreaterThanOne",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart << geo << featurePart << dataLine);
    return -1;
  }

   

  // add Bom:  geoPlannerDemand -->  Real Option Part, FOR EACH GE0-->PDF source!!
  // NOTE:  The usageTime must "equal" the cumulative lead time
  // associated with the pdf->geo transit time and the machineCycleTime
  
  //  We must add a bom record , along with a
  // subBom entry to a maxW/o null part

  // FINISH_ME: for now we are assuming single source gpd.  We must search
  // over the interval and probably put a while loop in that starts at
  // period zero and moves along
  int intervalStartPeriod = 0;
  // changed 03_15_2011
  int geoSourceIntervalEndPeriod = nPeriods - 1;

#ifdef VARIABLE_PERIODS
  LgFrTimeVecFloat transitTimeOffsetTV(nPeriods, 0.0);
  std::string machineFullWitPart = mpHelper.demandSource(theWitRun, plannerTopLevelPart, geo, intervalStartPeriod, geoSourceIntervalEndPeriod,
							 transitTimeOffsetTV,
							 fileName, dataLine, lineNo, MANDATORY);
#else
  float transitTimeScaled;
  std::string machineFullWitPart = mpHelper.demandSource(theWitRun, plannerTopLevelPart, geo, intervalStartPeriod, geoSourceIntervalEndPeriod,
							 transitTimeScaled,
							 fileName, dataLine, lineNo, MANDATORY);
#endif

  // determine the pdf for this source
  std::string pdf(mpHelper.pdf(machineFullWitPart));
  
  // get the full wit name for the real option part
  std::string fullFeaturePart(mpHelper.pdfPartname(featurePart, pdf));
    
  
  // test to see if the real feature part exists at the pdf.  IF NOT,
  // then print a warning and ignore the record 
  if (! mpHelper.isPartValid(theWitRun, fullFeaturePart, fileName, 
			     dataLine, lineNo, PURELY_OPTIONAL))  {
    (*theErrFacil)("CannotSourcePureOption",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart 
		   << geo << machineFullWitPart << featurePart << pdf << featurePart << intervalStartPeriod 
		   << geoSourceIntervalEndPeriod << dataLine);
    return -1;
  }


    
  // get the name of the specialFeature part
  std::string specialCCFeaturePartName(mpHelper.custChoiceFeaturePartName(plannerTopLevelPart, 
									geo, 
									featurePart, 
									pdf));
  
  // Add the part and bom structure connecting specialFeature to real feature.
  // This is only done if the part does not already exist
  if (! mpHelper.isPartValid(theWitRun, specialCCFeaturePartName, fileName, 
			     dataLine, lineNo, PURELY_OPTIONAL))  {
    
    // Add the part as a product (everything is at its default)
    witAddPartWithOperation(theWitRun, specialCCFeaturePartName.c_str());
    
    // Add the bom entry from special-->real (use default attributes)
    witAddBomEntry(theWitRun, specialCCFeaturePartName.c_str(), fullFeaturePart.c_str());
  }  
  


  // bomIndex = The bom entry number of GPD->SpecialCustChoiceFeatPart
  //      The bom is added here and can be used below
  int bomIndex;
  witGetOperationNBomEntries( theWitRun, geoPlannerDemandPart.c_str(), &bomIndex );
  witAddBomEntry( theWitRun, geoPlannerDemandPart.c_str(), specialCCFeaturePartName.c_str());
  witSetBomEntryUsageRate( theWitRun,  geoPlannerDemandPart.c_str(), bomIndex, usageRate);
  witSetBomEntryMandEC( theWitRun,  geoPlannerDemandPart.c_str(), bomIndex, WitFALSE );
  // store the feature ratio in two places (one is temporary).
  // First store in the bomAppData of the GPD->SCCF bom (this is permanent)
  LgFrSceCustChoiceBomAppData * bomAppDataPtr = new LgFrSceCustChoiceBomAppData(frTVF);
  witSetBomEntryAppData(theWitRun, geoPlannerDemandPart.c_str(), bomIndex, (void *) bomAppDataPtr);
  // Further down, we will store featratio in the  supplyVol of the nullsub

  
  // Set the offsets for the feature structure

  // transit time is applied as a bomOffset between GPD and specialFeaturePart.
  // this transittime qty is stored for our convenience as a BOP entry offset
  // beteen gpd part and gpd operation.  Later this bop offset will be reset to 0.
  // VARIABLE_PERIODS note: transitTimeOffset has already been adjusted for Var Periods
  float * transitTimeOffset;
  witGetBopEntryOffset(theWitRun, geoPlannerDemandPart.c_str(), 0, &transitTimeOffset);

  // this is where I am ...

  // The cycleTime of the machine (ie, the topLevelPDF Part for which this feature
  // is associated must be applied as the default machine-feature offset.  
  float * machineCycleTime;
  witGetPartCycleTime(theWitRun, machineFullWitPart.c_str(), &machineCycleTime);


  // machine cycletime is applied to bomOffset between specialCCFeature and featurePart.
  // This will be overridden later possibly by specific machine->feature offset file.
  witSetBomEntryOffset(theWitRun, specialCCFeaturePartName.c_str(), 0, machineCycleTime);
  // transit time gets applied to the bom offset between GPD and specialFeaturePart
  witSetBomEntryOffset(theWitRun, geoPlannerDemandPart.c_str(), bomIndex, transitTimeOffset);


  // Now work on adding a substitute arc to the geoPlannerDemandOperation to featurePart BOM entry.

  // First see if a null substitute exists for this
  // geo_plannerDemand_featurePart combination (if not then create one
  // NOTE: for CC features, the nullSub and sccf parts are one to one!

  std::string ccNullSubstitute;
  ccNullSubstitute = mpHelper.custChoiceNullSubstitute(geo, plannerTopLevelPart, featurePart);
  assert(0 == mpHelper.isPartValid(theWitRun, ccNullSubstitute, fileName, 
			     dataLine, lineNo, PURELY_OPTIONAL));

  // FINISH_ME: verify that this nullSub should be a  material instead of capacity
  witAddPart(theWitRun, ccNullSubstitute.c_str(), WitMATERIAL);

  // store the feature ratio in the supplyVol as a temporary means.  Later in a 
  // sce-preprocessing step we will do an explode and set the supplyVol to
  // (1-featratio) * demandVol
  witSetPartSupplyVol(theWitRun, ccNullSubstitute.c_str(), featureRatio);

  // Now add sub arc: (GPD->sCCF)->NullSub. 
  //
  // The defaultGatingOptionSubPenalty is the optionShipReward.  In
  // this case it is put on the sub arc as a cost.  ie, the cost of
  // substituting null, is the lost reward for shipping the option
  int nSubBom;
  witGetBomEntryNSubsBomEntries( theWitRun, geoPlannerDemandPart.c_str(), bomIndex, &nSubBom);
  witAddSubsBomEntry( theWitRun, geoPlannerDemandPart.c_str(), bomIndex, ccNullSubstitute.c_str() );
  // FINISH_ME: wha is the effectivity of the custchoice sub arcs?
  // we are assuming tha they are full.
  witSetSubsBomEntryOffset(theWitRun, geoPlannerDemandPart.c_str(), bomIndex, nSubBom, transitTimeOffset);

  // 2.30: we must shut off the expAllowed flag
  witSetSubsBomEntryExpAllowed(theWitRun, geoPlannerDemandPart.c_str(), bomIndex, nSubBom, WitFALSE);

  // if using the lp engine, then set the obj1 sub cost
  if ( lpModelHelper_ != 0 )
    witSetSubsBomEntryObj1SubCost ( theWitRun, geoPlannerDemandPart.c_str(), bomIndex, nSubBom,
				    defaultGatingOptionSubPenalty );
  


  witFree(machineCycleTime);
  witFree(transitTimeOffset);
    
  // Add exactly one demand on the option for every unique geoPlannerDemandPart--option pair.
  // When the heuristic engine implodes, it uses the null substitute before building the feature.
  // To figure out the amount of features that can be built, the
  // hEngMgr or aEngMgr loads the demands on the features and does an additional implosion.
  std::string returnedPlannerTopLevelPart;
  std::string returnedGeo;
  std::string demandName = mpHelper.custChoiceFeatureDemandName( geoPlannerDemandPart );
  if (!( mpHelper.isDemandSpecialCustChoiceFeature(theWitRun,
						   specialCCFeaturePartName,
						   demandName,
						   returnedPlannerTopLevelPart,
						   returnedGeo))) {
    witAddDemand ( theWitRun, specialCCFeaturePartName.c_str(), demandName.c_str() );


  }

  return 0;

}


  // + -------------------------------------------------
  // + ----------- SCE 6.1 -----------------------------
  // + -------------------------------------------------
  // + SCE's CUstomer Choice Feature Model is as follows:
  // + -------------------------------------------------
  // + For each cust choice feature record with
  // +     plannerPart, geo, featurePart, f1,f2,...,fNp-1
  // +
  // + 0) We detect customer choice features as a special case of standalone features if the
  // +    maxWithout vector contains an element < 0.  It's kludgy, but hey.
  // +     a) if there  are positive and negative elements to this vector, then it is an error.  
  // +        Furthermore, SCE uses +1.0 as the default for maxwithout.  Therefore, if the 
  // +        feature record is to be a custChoice, then you must fully populate the record.  
  // +     b) For custchoice features, we must have 0.0 <= featratio[t] <= 1.0 for all periods t.
  // + 1) Add specialCustChoiceFeaturePart (one and only one dummy part per machine-feature relationship
  // + 2) Figure out what the realWitFeaturePart is by tracing the geoSource to a realWitMachine
  // +    part to get the PDF and then using the mpHelper to catenate featurePart and PDF.
  // +    This realWitFeaturePart must already exist.
  // +    NOTE: We currently do NOT support the customer choice feature model for Machines that have
  // +    more than one geo source (this is true even if the multiple geo source is changing over time).
  // +    This makes the implementation much easier (although its still pretty intense).
  // + 3) Add bom entry from specialFeaturePart to realWitFeaturePart
  // + 4) Add bom entry between GPD part and specialFeaturePart 
  // +    Set usageRate=1.0 (regardless of featureRatio).  
  // +    Store featureRatio vector in bomAppdata for this bom entry (gpd->sfp)
  // + 5) Add/Locate the custChoiceNullSubstitute part
  // +    If the nullSub part doesn't exist, then create it
  // + 6) Create a subBomEntry between the bom entry for GPD->specialFeaturePart.  Sub part is the
  // +    nullSubPart defined in step 5.
  // +    a) set supplyVol(custChoiceNullSubstitute) = (1-featratio) * mrpProdVol(GPD).
  // +       i) this is done by temporarily storing featratio as the supplyVol of the nullSub
  // +          and then doing an mrp after the model is built.  then we do the arithmetic and
  // +          reset the supplyVol in a method called scePreProcessViaMrp().
  // +    (Note that the featratio gets "stored" in two places during the building of the
  // +    model.  This is simply for convenience so that we don't have to go searching up and
  // +    down through structure to find it.  The storage in bomAppData(gpd->sfp) is permanent
  // +    and needed later in the processing of explosion, second pass implosion,  and reporting.)
  // +    Store featureRatio vector in bomAppdata for this bom entry (gpd->sfp)
  // + 7) Offsets:
  // +    Offsetting is the same for customer choice features and standalone features.
  // +    a)The geo source transit time is applied first:
  // +       bomOffset(GPD->realWitMachine) = geoSourcetransitTime
  // +       bomOffset(GPD->specialFeaturePart) = geoSourceTransitTime
  // +       subBomOffset(GPD->specialFeaturePart,custChoiceNullSubstitute) = geoSourceTransitTime
  // +    b) Now, since features are essentially disconnected from machines, we must ensure
  // +       that the features are subject to the machineCycleTime (in addition to any
  // +       geoSourceTransitTime):
  // +          bomOffset(specialFeaturePart->realWitFeaturePart) = machineCycleTime
  // +    c) Any machine-feature offset (set in a subsequent input file) 
  // +       is an override to the bom offset determined by machineCycleTime.  
  // +          bomOffset(specialFeaturePart->realWitFeaturePart) = machineFeatureOffset
  // +       Note: we do NOT let the mahcineFeatureOffset override the machineCycleTime 
  // +       (perhaps we should).
  // +    d) a phantom flag (set via machine-feature offset file) will cause the
  // +       offset between specialFeaturePart and realFeaturePart to be adjusted 
  // +       so that it cancels out the cycleTime on the real feature part.  
  // +          if (phantomFlag)
  // +             bomOffset(specialFeaturePart->realWitFeaturePart) -= cycleTime(realWitFeaturePart)
  // +    e) Note: the structure of specialFeaturePart and above is "above" the machine 
  // +       cycleTime and feature offsets.  This means that the featureRatio constraint
  // +       mechanism applies to the shipment of product out of the PDF (prior to 
  // +       geoSourceTransitTime).
  // +    f) (IBM NOTE) CycleTime on the topLevelMfgPart (machine at PDF) should not be used for ETIS type
  // +       feeds.  This is because the MTM,PDF will have no bom content, yet the cycleTime 
  // +       will gate anyway.  Plus, the ESAT extract will strictly feed all machine offset
  // +       via machine-feature offset. 
  // + 8) Add a demand to the specialFeaturePart.  This is for the two-pass heuristic. 
  // +    The demandVol is 0.0 for the first pass of heuristic.
  // +    The priority is copied over from the priority of the Machine demand in a later 
  // +    post-processing step.
  // + 9) This model does not work for MRP, so we make an adjustment in the exploder witRun.  
  // +    Namely, since the usageRate along the gpd->specialFeaturePart->realWitFeaturepart chain
  // +    of boms is 1.0 (regardless of featratio), we will be exploding too much demand.  So, to
  // +    remedy this, we add a dummy supply onto the specialFeaturePart.  This supply gets netted
  // +    at the top and explodes the right qty down the structure.  (note: we could have adjusted
  // +    the usageRate on the bom within the exploder witRun, but since feature ratio's can change
  // +    over time, this would have required multiple arcs -- yuck!!!).
  // +      supplyVol(exploderWitRun, specialFeaturePart) += (1-featureRatio) * mrpProdVol(GPD)
  // +    Note that we += this in as supply because there may already be supplyVol of the 
  // +    specialFeaturePart due to MachineFeature supply.
  // + 10) Compute Second Pass Feature Demand Vol:
  // +       Note: this is done after the "first pass" implosion run, in preperation for the second
  // +             pass (feature) implosion.
  // +             a) tiedBackExecVol(GPD): roll the execVol(GPD) (ie, first pass machine ships) back
  // +                                      to the period in which the demand was due.  We do this for
  // +                                      cases where the featureRatio changes over time.
  // +             b) featuresNeeded  = tiedBackExecVol(GPD) * featureRatio (this is the total features
  // +                                  that need to be shipped based on machine commits).
  // +             c) featuresShipped = consVol(specialFeaturePart) + excessVol(specialFeaturePart)
  // +             d) secondPassDemandVol = featuresNeeded - featuresShipped 
  // +             e) "Roll" secondPassDemandVol to carry negatives forward (can happen due to machine
  // +                 feature supply fen5, or weird offset problems).
  // + 
  // + 11) How to calculate OptionRatio output (ie, features shipped):  
  // +       optionRatio = consVol(specialFeaturePart) 
  // +                     + shipVol(specialFeaturePartDemand) 
  // +                     + excessVol(specialFeaturePart)
  // + 
  // + 12) The FSS model for this is, as yet, not implemented.  
  // +
  // + Additional Comments:
  // +   a) Base model is implemented and working.  
  // +   b) There are many special cases to be tested.  Further, these cases can work in a "unit"
  // +      test environment but not work when they occur together.  
  // +     1) ratio's changing over time
  // +     2) feature is primary  gating part
  // +     3) feature is short, but not primary gate.
  // +     4) offsets: transittime, machine cycleTime, mach->feat offset, fature cycletime, feature phantom
  // +   c) This model is similar to standalone features.  Major differences are:
  // +        1) this model can have ratio's change over time and not have to split the bill
  // +        2) we restrict multiple sourcing on the machines for these mach->feature records -- this is 
  // +           not the case for stdAlone features.
  // +        3) because of 1 and 2 above, we assert that there's one and only one:
  // +             gpd->sfp bom
  // +             gpd->sfp, nullSub subBom arc
// ------------------------------------------------------------------

int
LgFrInitialProblemForSce::sceAddCustChoiceFeatureStructure61(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar   & theCal,
    const std::string    & plannerTopLevelPart, 
    const std::string    & geo,
    const std::string    & mfgOptionPart, 
    float                  usageRate,
    float                  attachRate,
    float                  mfOffsetVal,
    int                    mfoPhantomFlag,
    float                  mfSupplyVol,
    int                    earlyPeriod,
    int                    latePeriod,
    float                * defaultGatingOptionSubPenalty,
    const std::string    & fileName,
    const std::string    & dataLine,
    long                   lineNo)
{

  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);
  int t;

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  // Determine the GPD part name ... and see if it exists   
  std::string geoPlannerDemandPart = mpHelper.geoPlannerDemandPartName(plannerTopLevelPart, geo);
  if (! mpHelper.isPartValid(theWitRun, geoPlannerDemandPart, fileName, dataLine, lineNo, PURELY_OPTIONAL))   {
    (*theErrFacil)("PartDoesNotExistb",MclArgList() << plannerTopLevelPart << geo << mfgOptionPart << fileName << (int)lineNo << dataLine);
    return -1;
  }

  // reject if attachRate=0.0, unless we need to set it for effectivity reasons
  if (attachRate < 0.00001)  {
    (*theErrFacil)("OptionAllZeroRatiosPO",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart << geo << mfgOptionPart << dataLine);
    return -1;
  }

  if (attachRate > 1.0)  {
    (*theErrFacil)("CustChoiceFeatRatioGreaterThanOne",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart << geo << mfgOptionPart << dataLine);
    return -1;
  }


  // FINISH_ME_61: D2O guarantees no multi-source right ???? Can we get rid of these checks ???



  // check to see if this record is a duplicate. If there's any 
  // relationship between the geoPlannerDemandPart and
  // a specialFeaturePart whose partName token is the mfgOptionPart,
  // then we reject the record.
  int nBomEntries;
  witGetOperationNBomEntries( theWitRun, geoPlannerDemandPart.c_str(), &nBomEntries );
  char * child;
  
  // Duplicate records:  For now we reject if there is already a GPD->specialFeature relationship.  
  //  We need to be smarter.  
  //     -- compare horizons, and modify/etc.  this is a FINISH_ME_61
  int j = 0; 
  for (j=0; j<nBomEntries; j++)  {
    witGetBomEntryConsumedPart( theWitRun, geoPlannerDemandPart.c_str(), j, &child );
    // Note check against ANY types of feature records.  Reject if match
    if ( (mfgOptionPart == mpHelper.partname(child)) && mpHelper.isPartSpecialFeature(theWitRun, child)) {
      (*theErrFacil)("OptionAlreadyDefined",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart << geo << mfgOptionPart << dataLine);
      witFree ( child );
      return -1;
    }
    witFree ( child );    
  }    


  std::string sourceLocPdf;
  // determine the full wit name for the Box at the sourceLocPdf
  std::string mfgBoxPartName = mpHelper.mfgTopPartForGpd(theWitRun, plannerTopLevelPart, geo, sourceLocPdf);

  // get the full wit name for the real option part
  std::string fullOptionPartName(mpHelper.pdfPartname(mfgOptionPart, sourceLocPdf));
  
    
  
  // test to see if the real feature part exists at the pdf.  IF NOT,
  // then print a warning and ignore the record 
  if (! mpHelper.isPartValid(theWitRun, fullOptionPartName, fileName, dataLine, lineNo, PURELY_OPTIONAL))  {
    (*theErrFacil)("CannotSourcePureOption",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart 
		   << geo << mfgBoxPartName << mfgOptionPart << sourceLocPdf << mfgOptionPart << earlyPeriod 
		   << latePeriod << dataLine);
    return -1;
  }
  

  // determine  name of specialCCFeaturePart
  std::string specialCCFeaturePartName(mpHelper.custChoiceFeaturePartName(plannerTopLevelPart, geo, mfgOptionPart, sourceLocPdf));
  int doesSpecialCCFeaturePartAlreadyExist = mpHelper.isPartValid(theWitRun, specialCCFeaturePartName, fileName, dataLine, lineNo, PURELY_OPTIONAL);
    

  // Easy Case !!  create new part/structure
  if (! doesSpecialCCFeaturePartAlreadyExist)  {
    witAddPartWithOperation(theWitRun, specialCCFeaturePartName.c_str());
    witAddBomEntry(theWitRun, specialCCFeaturePartName.c_str(), fullOptionPartName.c_str());
    
    
    // ==> Add BOM entry   connecting GPD->SpecialCustChoiceFeatPart
    //              bomIndex = The bom entry number of GPD->SpecialCustChoiceFeatPart
    int bomIndex;
    witGetOperationNBomEntries( theWitRun, geoPlannerDemandPart.c_str(), &bomIndex );
    witAddBomEntry( theWitRun, geoPlannerDemandPart.c_str(), specialCCFeaturePartName.c_str());
    LgFrTimeVecFloat consRate(nPeriods, usageRate);
    witSetBomEntryConsRate( theWitRun,  geoPlannerDemandPart.c_str(), bomIndex, consRate.data());
    witSetBomEntryMandEC( theWitRun,  geoPlannerDemandPart.c_str(), bomIndex, WitFALSE );
    
    // store the feature ratio in two places (one is temporary).
    // for legacy reasons, we store it as a timeVec.
    // First store in the bomAppData of the GPD->SCCF bom (this is permanent)
    // Further down, we will store featratio in the  supplyVol of the nullsub
    LgFrTimeVecFloat frTVF(nPeriods, attachRate);
    LgFrSceCustChoiceBomAppData * bomAppDataPtr = new LgFrSceCustChoiceBomAppData(frTVF);
    witSetBomEntryAppData(theWitRun, geoPlannerDemandPart.c_str(), bomIndex, (void *) bomAppDataPtr);


    // Set Offsets on specialFeaturePartName
    //       ... function of (mfgBoxCycleTimeDays, mfOffset, mfoPhantomFlag, mfgOptionPartCycleTimeDays)
    LgFrTimeVecFloat offsetTV(nPeriods, 0.0);
    LgFrTimeVecFloat mfgBoxCycleTimeDays = mpHelper.getCycleTimeDays(theWitRun, mfgBoxPartName);

    // mfoPhantom bills get a cycleTimeDays that is offset of mfgOptionPart's cycleTime.  
    if (mfoPhantomFlag) {
      LgFrTimeVecFloat optionCycleTimeDays = mpHelper.getCycleTimeDays(theWitRun, fullOptionPartName);
      for (t=earlyPeriod; t<=latePeriod; t++) {
	offsetTV[t] = mpHelper.calculateVariableOffset(mfgBoxCycleTimeDays[t] + mfOffsetVal - optionCycleTimeDays[t], t, theCal, nPeriods);
      }
    }
    else   {
      for (t=earlyPeriod; t<=latePeriod; t++) {
	offsetTV[t] = mpHelper.calculateVariableOffset(mfgBoxCycleTimeDays[t] + mfOffsetVal, t, theCal, nPeriods);
      }
    }
    witSetBomEntryOffset(theWitRun, specialCCFeaturePartName.c_str(), 0, offsetTV.data());
    

    // Now work on adding a substitute arc to the geoPlannerDemandOperation to mfgOptionPart BOM entry.
    
    // First see if a null substitute exists for this
    // geo_plannerDemand_mfgOptionPart combination (if not then create one
    // NOTE: for CC features, the nullSub and sccf parts are one to one!
    std::string ccNullSubstitute = mpHelper.custChoiceNullSubstitute(geo, plannerTopLevelPart, mfgOptionPart);
    assert(0 == mpHelper.isPartValid(theWitRun, ccNullSubstitute, fileName, dataLine, lineNo, PURELY_OPTIONAL));
    
    // FINISH_ME: verify that this nullSub should be a  material instead of capacity
    witAddPart(theWitRun, ccNullSubstitute.c_str(), WitMATERIAL);
    
    // store the feature ratio in the supplyVol as a temporary means.  Later in a 
    // sce-preprocessing step we will do an explode and set the supplyVol to
    // (1-featratio) * demandVol
    witSetPartSupplyVol(theWitRun, ccNullSubstitute.c_str(), frTVF.data());
    
    // Now add sub arc: (GPD->sCCF)->NullSub. 
    //
    // The defaultGatingOptionSubPenalty is the optionShipReward.  In
    // this case it is put on the sub arc as a cost.  ie, the cost of
    // substituting null, is the lost reward for shipping the option
    int nSubBom;
    witGetBomEntryNSubsBomEntries( theWitRun, geoPlannerDemandPart.c_str(), bomIndex, &nSubBom);
    witAddSubsBomEntry( theWitRun, geoPlannerDemandPart.c_str(), bomIndex, ccNullSubstitute.c_str() );
    // FINISH_ME: what is the effectivity of the custchoice sub arcs?
    //            we are assuming that they are full.
    // confirm that subBomOffset is zero (used to be transitTime on GPD)
    //  witSetSubsBomEntryOffset(theWitRun, geoPlannerDemandPart.c_str(), bomIndex, nSubBom, offsetTV.data());
    
    // Shut off the expAllowed flag
    // NOte that offest on the subBom to ccNullSub should be zero
    // witSetSubsBomEntryExpAllowed(theWitRun, geoPlannerDemandPart.c_str(), bomIndex, nSubBom, WitFALSE);
    
    // if using the lp engine, then set the obj1 sub cost
    if ( lpModelHelper_ != 0 )
      witSetSubsBomEntryObj1SubCost (theWitRun, geoPlannerDemandPart.c_str(), bomIndex, nSubBom, defaultGatingOptionSubPenalty );
    
    
    // Add exactly one demand on the option for every unique geoPlannerDemandPart--option pair.
    // When the heuristic engine implodes, it uses the null substitute before building the feature.
    // To figure out the amount of features that can be built, the
    // hEngMgr or aEngMgr loads the demands on the features and does an additional implosion.
    std::string returnedPlannerTopLevelPart;
    std::string returnedGeo;
    std::string demandName = mpHelper.custChoiceFeatureDemandName( geoPlannerDemandPart );
    if (!( mpHelper.isDemandSpecialCustChoiceFeature(theWitRun, specialCCFeaturePartName, demandName, returnedPlannerTopLevelPart, returnedGeo))) {
      witAddDemand ( theWitRun, specialCCFeaturePartName.c_str(), demandName.c_str() );
    }
    
  }
  
  // harder case, what to do if part already exists ...
  if (doesSpecialCCFeaturePartAlreadyExist)  {
    
  }

  return 0;

}








// ------------------------------------------------------------------
int
LgFrInitialProblemForSce::sceAddStdAloneOptionBomStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & plannerTopLevelPart, 
    const std::string & geo,
    const std::string & mfgOptionPart, 
    float usageRate,
    float * featureRatio,
    float * maxWithout,
    float * defaultGatingOptionSubPenalty,
    int   featureType,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{

  // FINISH_ME:  we can now use wit's consRate (timeVec) instead of usageRate (scalar) to possibly 
  //             simplify this model!!

  // + SCE's Feature Model is as follows:
  // + For each feature record with
  // +     plannerPart, geo, mfgOptionPart, f1,f2,...,fNp-1,mwo1,mwo2,...,mwoNp-1
  // +
  // + 1) Add specialFeaturePart (one and only one dummy part per machine-feature relationship
  // +    NOte: this is in test, its a bigger model but fixes some issues with feature offsets
  // + 2) Figure out what the mfgOptionPart is by tracing the geoSource to a mfgTopLevel
  // +    part to get the PDF.  Then create a mfgOptionPart using mfgOptionPart and PDF.
  // +    Hence mfgOptionPart is a full Wit part name
  // + 3) Add bom entry from specialFeaturePart to mfgOptionPart
  // + 4) Add bom entry between specialGPD part and specialFeaturePart (featureRatio determines
  // +    usageRate.  Also, usageTime must be set specially ... see note below)
  // + 5) Add/Locate a NullSubstitute part
  // +    i) determine what type of nullSub to use:
  // +         if (the option is NOT GATING (ie, mwo=1 for all periods))
  // +         then use a globalNullSubstitute 
  // +         else if (the option is GATING AND MANDATORY (ie, mwo=0 for all periods))
  // +         then use a globalMaxWithoutNullSubstitute 
  // +         else (the option is GATING AND NON-MANDATORY (ie, 0<mwo<1 in some period))
  // +         then use a maxWithoutNullSubstitute
  // +       Note: that for Gating and Non-Mandatory, a dummy part is created for each record,
  // +             whereas a global one is used for the other two cases.
  // +    ii) If the nullSub part doesn't exist, then create it
  // + 6) Create a subBomEntry between the bom entry for GPD->specialFeaturePart.  Sub part is the
  // +    nullSubPart defined in step 5.
  // +    a) subBom offset is the transitTime 
  // + 7) Offsets:
  // +    a) TransitTime is set via BOM offset between GPD operation and  TopLevelPart (machine).
  // +       TransitTime is also set via BOM offset between GPD operation and  special Feature part.
  // +       TransitTime is also set via SubBOM offset between GPD, specialFeaturePart, and NullSub.
  // +    b) topLevelPartPDF cycleTime is set as the bom offset between  special feature part and
  // +       the real feature part.
  // +    c) Any machine-feature offset is an override to the bom offset between
  // +       specialFeaturePart and the real feature part.  (ie, it overides the machine offset
  // +       default that was previosuly set).  
  // +    d) a phantom flag (set via machine-feature offset file) will cause the
  // +       offset between specialFeaturePart and realFeaturePart to be adjusted 
  // +       so that it cancels out the cycleTime on the real feature part.  
  // + 8) Additional gotcha's and bugginess.
  // +    a) Feature ratios can change over time and this must be mapped to multiple
  // +       bom arcs with effective intervals
  // + 9) CycleTime on the topLevelMfgPart (machine at PDF) should not be used for ETIS type
  // +    feeds.  This is because the MTM,PDF will have no bom content, yet the cycleTime 
  // +    will gate anyway.  Plus, the ESAT extract will strictly feed all machine offset
  // +    via machine-feature offset. 
  // + 10) A specific machine->feature supplyVol is added (accumulated) at the specialFeaturePart
  // +    This allows the user to "adjust" machine content/ratio's (in IBM this applies to 
  // +    TYPE 5 front end netting (FEN(5))
  
  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
  
  // make sure the plannerTopLevel Special Demand Part exists
  std::string geoPlannerDemandPart =
        mpHelper.geoPlannerDemandPartName(plannerTopLevelPart, geo);
  if (! mpHelper.isPartValid(theWitRun, geoPlannerDemandPart, fileName, 
                             dataLine, lineNo, PURELY_OPTIONAL))   {
    (*theErrFacil)("PartDoesNotExistb",MclArgList() 
		   << plannerTopLevelPart 
		   << geo 
		   << mfgOptionPart 
		   << fileName 
		   << (int)lineNo
		   << dataLine);
    return -1;
  }
  

  // check to see if this record is a duplicate. If there's any 
  // relationship between the geoPlannerDemandPart and
  // a specialFeaturePart whose partName token is the mfgOptionPart,
  // then we reject the record.
  int nBomEntries;
  witGetOperationNBomEntries( theWitRun, geoPlannerDemandPart.c_str(), &nBomEntries );
  char * child;
  int j = 0; 
  for (j=0; j<nBomEntries; j++)  {
    witGetBomEntryConsumedPart( theWitRun, geoPlannerDemandPart.c_str(), j, &child );
    std::string pdf;
    std::string childFeaturePart;
    std::string gg, mm;
    if ( mpHelper.isPartSpecialStandaloneFeature(theWitRun, child, gg, mm, pdf, childFeaturePart) ) {
      //    if ( mpHelper.isPartSpecialStandaloneFeature(theWitRun, child, gg, mm, pdf, mfgOptionPart) ) {
      if (childFeaturePart == mfgOptionPart) {
        // uh-oh, we've got a duplicate record
	(*theErrFacil)("OptionAlreadyDefined",MclArgList() 
		       << fileName 
		       << (int)lineNo 
		       << plannerTopLevelPart 
		       << geo 
		       << mfgOptionPart 
		       << dataLine);
        witFree ( child );
        return -1;
      }
    }
    witFree ( child );    
  }    



  
  
  //  Determine the first period that has non-Zero Ratio.
  //  Note: the logic of this method does NOT maintain independence
  //        between the feature ratios and the maxWithOut ratios.

  int firstPeriodWithRatio = nPeriods;
  int t = 0; // Pulled out of the for below by RW2STL
  for (t=0; t<nPeriods; t++)
    if (featureRatio[t] > 0.0)  {
      firstPeriodWithRatio = t;
      break;
    }

  if (firstPeriodWithRatio == nPeriods)  {
    (*theErrFacil)("OptionAllZeroRatiosPO",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << plannerTopLevelPart 
		   << geo 
		   << mfgOptionPart 
		   << dataLine);
    return -1;
  }

  // The strategy is to set the usage rates on these bom entries to
  // the feature ratio.  One blip is that the ratio's are input as
  // vectors, and now must be mapped to scalars.  This is done with
  // multiple arcs -- yukk!
  int intervalStartPeriod = firstPeriodWithRatio;
  int intervalEndPeriod;
  int ratioIntervalEndPeriod;

  while (intervalStartPeriod < nPeriods) {

    // first compute the interval End period based on ratios.  We  will
    // also check for a tighter interval end period based on geo sourcing
    // later in this loop
    ratioIntervalEndPeriod = nPeriods - 1;
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=intervalStartPeriod; t<nPeriods; t++) {
      if (fabs(featureRatio[intervalStartPeriod] - featureRatio[t]) > 0.0001) {
        ratioIntervalEndPeriod = t-1;
        assert(intervalStartPeriod <= ratioIntervalEndPeriod);
        break;
      }
    }

    // add Bom:  geoPlannerDemand -->  Real Option Part, FOR EACH GE0-->PDF source!!
    // NOTE:  The usageTime must "equal" the cumulative lead time
    // associated with the pdf->geo transit time and the cycleTime
    // on the mfgTopLevelPart.

    // OK, We now have identified an interval where the feature ratio is
    // constant.  We must add a bom record for each interval, along with a
    // subBom entry to a maxW/o null part
    int geoSourceIntervalEndPeriod = nPeriods - 1;
    float transitTimeScaled;
    std::string mfgTopLevelPart = mpHelper.demandSource(theWitRun,
                                                      plannerTopLevelPart,
                                                      geo,
                                                      intervalStartPeriod,
                                                      geoSourceIntervalEndPeriod,
                                                      transitTimeScaled,
                                                      fileName,
                                                      dataLine,
                                                      lineNo,
                                                      MANDATORY);

    // the interval end is the min of the ratio end and the geo source end
    intervalEndPeriod = (ratioIntervalEndPeriod < geoSourceIntervalEndPeriod)
      ? ratioIntervalEndPeriod : geoSourceIntervalEndPeriod;


    // determine the pdf for this source
    std::string pdf(mpHelper.pdf(mfgTopLevelPart));
    
    // get the full wit name for the real option part
    std::string fullOptionPartName(mpHelper.pdfPartname(mfgOptionPart, pdf));
    

    // test to see if the real option part exists at the pdf.  IF NOT,
    // then print a warning and ignore the record 
    if (! mpHelper.isPartValid(theWitRun, fullOptionPartName, fileName, 
                               dataLine, lineNo, PURELY_OPTIONAL))  {
      (*theErrFacil)("CannotSourcePureOption",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << plannerTopLevelPart 
		     << geo 
		     << mfgTopLevelPart 
		     << mfgOptionPart 
		     << pdf 
		     << mfgOptionPart 
		     << intervalStartPeriod 
		     << geoSourceIntervalEndPeriod 
		     << dataLine);
      intervalStartPeriod = intervalEndPeriod + 1;
      return -1;
    }


    
    // get the name of the specialFeature part
    std::string specialFeaturePartName(mpHelper.standaloneFeaturePartName(plannerTopLevelPart, 
									geo, 
									mfgOptionPart, 
									pdf));
    
    // Add the part and bom structure connecting specialFeature to real feature.
    // This is only done if the part does not already exist
    if (! mpHelper.isPartValid(theWitRun, specialFeaturePartName, fileName, 
                               dataLine, lineNo, PURELY_OPTIONAL))  {
      // Add the part as a product (everything is at its default)
      witAddPartWithOperation(theWitRun, specialFeaturePartName.c_str());

#if 0
      // Lotsizing of Features ... This needs to be thought about some more ...
      int nPeriods;
      witGetNPeriods(theWitRun, &nPeriods);
      LgFrTimeVecFloat nonIntLotSize(nPeriods, 0.01);
      witSetOperationMinLotSize(theWitRun, specialFeaturePartName.c_str(), nonIntLotSize.data());
      witSetOperationIncLotSize(theWitRun, specialFeaturePartName.c_str(), nonIntLotSize.data());
#endif

      // Add the bom entry from special-->real (use default attributes)
      witAddBomEntry(theWitRun, specialFeaturePartName.c_str(), fullOptionPartName.c_str());
    }  
    


    int bomIndex;
    witGetOperationNBomEntries( theWitRun, geoPlannerDemandPart.c_str(), &bomIndex );
    witAddBomEntry( theWitRun, geoPlannerDemandPart.c_str(), specialFeaturePartName.c_str());
    witSetBomEntryEarliestPeriod ( theWitRun, geoPlannerDemandPart.c_str(), bomIndex,  intervalStartPeriod );
    witSetBomEntryLatestPeriod ( theWitRun, geoPlannerDemandPart.c_str(), bomIndex,  intervalEndPeriod );
    float finalUsageRate = usageRate * featureRatio[intervalStartPeriod];
    witSetBomEntryUsageRate( theWitRun,  geoPlannerDemandPart.c_str(), bomIndex, finalUsageRate );
    witSetBomEntryMandEC( theWitRun,  geoPlannerDemandPart.c_str(), bomIndex, WitFALSE );


    // Set the offsets for the feature structure

    // transit time is applied as a bomOffset between GPD and specialFeaturePart.
    // this transittime qty is stored for our convenience as a BOP entry offset
    // beteen gpd part and gpd operation.  Later this bop offset will be reset to 
    // 0.
    float * transitTimeOffset;
    witGetBopEntryOffset(theWitRun, geoPlannerDemandPart.c_str(), 0, &transitTimeOffset);
    witSetBomEntryOffset(theWitRun, geoPlannerDemandPart.c_str(), 0, transitTimeOffset);

    // The cycleTime of the machine (ie, the topLevelPDF Part for which this feature
    // is associated must be applied as the default machine-feature offset.  
    float * mfgTopLevelCycleTime;
    witGetPartCycleTime(theWitRun, mfgTopLevelPart.c_str(), &mfgTopLevelCycleTime);


    // machine cycletime is applied to bomOffset between specialFeature and mfgOptionPart.
    // This will be overridden later possibly by specific machine->feature offset file.
    witSetBomEntryOffset(theWitRun, specialFeaturePartName.c_str(), 0, mfgTopLevelCycleTime);
    // transit time gets applied to the bom offset between GPD and specialFeaturePart
    witSetBomEntryOffset(theWitRun, geoPlannerDemandPart.c_str(), bomIndex, transitTimeOffset);


    // Now work on adding a substitute arc to the geoPlannerDemandPart to mfgOptionPart BOM entry.
    //
    // Pick the right kind of nullSub to substitute: either a globalMaxWithoutNullSubstitute,
    //   a maxWithoutNullSubstitute, or a globalNullSubstitute.
    //
    // If featureType=SCE_FEATURE_TYPE_MANDATORY,       use a globalMaxWithoutNullSubstitute.
    // If featureType=SCE_FEATURE_TYPE_MWO,             use a maxWithoutNullSubstitute.
    // If featureType=SCE_FEATURE_TYPE_PURELY_OPTIONAL, use a globalNullSubstitute.
    //

    std::string nullSubstitute;
    if (featureType == SCE_FEATURE_TYPE_MANDATORY) {
      nullSubstitute = mpHelper.globalMaxWithoutNullSubstitute(pdf);
      if (! mpHelper.isPartValid(theWitRun, nullSubstitute, fileName, 
				 dataLine, lineNo, PURELY_OPTIONAL)) {
	witAddPart(theWitRun, nullSubstitute.c_str(), WitMATERIAL);
      }
    }
    else if (featureType == SCE_FEATURE_TYPE_MWO) {
      nullSubstitute = mpHelper.maxWithoutNullSubstitute(geo, plannerTopLevelPart, mfgOptionPart);
      if (! mpHelper.isPartValid(theWitRun, nullSubstitute, fileName, 
				 dataLine, lineNo, PURELY_OPTIONAL)) {
	witAddPart(theWitRun, nullSubstitute.c_str(), WitMATERIAL);
	// Sneak the maxWithout into the supplyVol of the maxWithoutNullSubstitite
	// unless its a very small number
	if (finalUsageRate > IPSCE_FLT_EPS) 
	  witSetPartSupplyVol(theWitRun, nullSubstitute.c_str(), maxWithout);
      }
    }
    else if (featureType == SCE_FEATURE_TYPE_PURELY_OPTIONAL) {
      nullSubstitute = mpHelper.globalNullSubstitute(pdf);
      if (! mpHelper.isPartValid(theWitRun, nullSubstitute, fileName, 
                                 dataLine, lineNo, PURELY_OPTIONAL)) {
	witAddPart(theWitRun, nullSubstitute.c_str(), WitMATERIAL);
	LgFrTimeVecFloat bigMsupply(nPeriods, 1000000000000.0);
	witSetPartSupplyVol(theWitRun, nullSubstitute.c_str(), bigMsupply.data());
      }
    }

    else {
      std::cerr << "About to die: featureType =" << featureType << "\n";
      assert(featureType == -500);
    }

    // store the maxWithout in the appdata of the SpecialFeaturePart
    LgFrScePartAppData * appDataPtr;
    LgFrTimeVecFloat maxWithoutTV(nPeriods, maxWithout);
    witGetPartAppData(theWitRun, specialFeaturePartName.c_str(), (void **) &appDataPtr);
    if (appDataPtr == 0) {
      appDataPtr = new LgFrScePartAppData(0, nPeriods, nPeriods);
      appDataPtr->maxWithout(maxWithoutTV);
      witSetPartAppData(theWitRun, specialFeaturePartName.c_str(), (void *) appDataPtr);
    }
    else 
      appDataPtr->maxWithout(maxWithoutTV);      
    


    // Regardless of featureType:
    // now add the sub arc.  You must get the bom number of the bom
    // to be subbed off of.  Fortunately, you can get it easily.
    // How?, You just added the "last" bom entry and now you want
    // to sub off it.  So the bom entry number is nBom - 1.
    //
    // The defaultGatingOptionSubPenalty is the optionShipReward.  In
    // this case it is put on the sub arc as a cost.  ie, the cost of
    // substituting null, is the lost reward for shipping the option
    int nBom;
    witGetOperationNBomEntries(theWitRun, geoPlannerDemandPart.c_str(), &nBom);
    assert(nBom > 0);
    int nSubBom;
    witGetBomEntryNSubsBomEntries(theWitRun, geoPlannerDemandPart.c_str(), nBom-1, &nSubBom);
    witAddSubsBomEntry(theWitRun, geoPlannerDemandPart.c_str(), nBom-1, nullSubstitute.c_str());
    // 2.30: must shutt off expAllowed
    witSetSubsBomEntryExpAllowed(theWitRun, geoPlannerDemandPart.c_str(), nBom-1, nSubBom, WitFALSE);
    witSetSubsBomEntryEarliestPeriod(theWitRun, geoPlannerDemandPart.c_str(), nBom-1, nSubBom, intervalStartPeriod);
    witSetSubsBomEntryLatestPeriod(theWitRun, geoPlannerDemandPart.c_str(), nBom-1, nSubBom, intervalEndPeriod);
    witSetSubsBomEntryOffset(theWitRun, geoPlannerDemandPart.c_str(), nBom-1, nSubBom, transitTimeOffset);
    // if using the lp engine, then set the obj1 sub cost
    if (lpModelHelper_ != 0)
      witSetSubsBomEntryObj1SubCost(theWitRun, geoPlannerDemandPart.c_str(), nBom-1, nSubBom, defaultGatingOptionSubPenalty);


    witFree(mfgTopLevelCycleTime);
    witFree(transitTimeOffset);
    
    // Add exactly one demand on the option for every unique geoPlannerDemandPart--option pair.
    // When the heuristic engine implodes, it uses the null substitute before building the feature.
    // To figure out the amount of features that can be built, the
    // hEngMgr or aEngMgr loads the demands on the features and does an additional implosion.
    std::string returnedPlannerTopLevelPart;
    std::string returnedGeo;
    std::string demandName = mpHelper.pureOptionBuildDemandName( geoPlannerDemandPart );
    if (!( mpHelper.isDemandSpecialPureOptionBuildDemand( theWitRun,
                                                           specialFeaturePartName,
                                                           demandName,
                                                           returnedPlannerTopLevelPart,
                                                           returnedGeo))) {
      witAddDemand(theWitRun, specialFeaturePartName.c_str(), demandName.c_str() );

    }
    
    intervalStartPeriod = intervalEndPeriod + 1;
  }

  return 0;

}






// ------------------------------------------------------------------
int
LgFrInitialProblemForSce::sceAddStdAloneOptionBomStructure61(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar   & theCal,
    const std::string    & plannerTopLevelPart, 
    const std::string    & geo,
    const std::string    & mfgOptionPart, 
    float                  usageRate,
    float                  attachRate,
    float                  maxWithout,
    float                  mfOffsetVal,
    int                    mfoPhantomFlag,
    float                  mfSupplyVal,
    float                * defaultGatingOptionSubPenalty,
    int                    earlyPeriod,
    int                    latePeriod,
    int                    featureType,
    const std::string    & fileName,
    const std::string    & dataLine,
    long                   lineNo)
{

  // + SCE's Feature Model is as follows:
  // + For each feature record with
  // +     plannerPart, geo, mfgOptionPart, f1,f2,...,fNp-1,mwo1,mwo2,...,mwoNp-1
  // +
  // + 1) Add specialFeaturePart (one and only one dummy part per machine-feature relationship
  // +    NOte: this is in test, its a bigger model but fixes some issues with feature offsets
  // + 2) Figure out what the mfgOptionPart is by tracing the geoSource to a mfgTopLevel
  // +    part to get the PDF.  Then create a mfgOptionPart using mfgOptionPart and PDF.
  // +    Hence mfgOptionPart is a full Wit part name
  // + 3) Add bom entry from specialFeaturePart to mfgOptionPart
  // + 4) Add bom entry between specialGPD part and specialFeaturePart (featureRatio determines
  // +    usageRate.  Also, usageTime must be set specially ... see note below)
  // + 5) Add/Locate a NullSubstitute part
  // +    i) determine what type of nullSub to use:
  // +         if (the option is NOT GATING (ie, mwo=1 for all periods))
  // +         then use a globalNullSubstitute 
  // +         else if (the option is GATING AND MANDATORY (ie, mwo=0 for all periods))
  // +         then use a globalMaxWithoutNullSubstitute 
  // +         else (the option is GATING AND NON-MANDATORY (ie, 0<mwo<1 in some period))
  // +         then use a maxWithoutNullSubstitute
  // +       Note: that for Gating and Non-Mandatory, a dummy part is created for each record,
  // +             whereas a global one is used for the other two cases.
  // +    ii) If the nullSub part doesn't exist, then create it
  // + 6) Create a subBomEntry between the bom entry for GPD->specialFeaturePart.  Sub part is the
  // +    nullSubPart defined in step 5.
  // +    a) subBom offset is the transitTime 
  // + 7) Offsets:
  // +    a) TransitTime is set via BOM offset between GPD operation and  TopLevelPart (machine).
  // +       TransitTime is also set via BOM offset between GPD operation and  special Feature part.
  // +       TransitTime is also set via SubBOM offset between GPD, specialFeaturePart, and NullSub.
  // +    b) topLevelPartPDF cycleTime is set as the bom offset between  special feature part and
  // +       the real feature part.
  // +    c) Any machine-feature offset is an override to the bom offset between
  // +       specialFeaturePart and the real feature part.  (ie, it overides the machine offset
  // +       default that was previosuly set).  
  // +    d) a phantom flag (set via machine-feature offset file) will cause the
  // +       offset between specialFeaturePart and realFeaturePart to be adjusted 
  // +       so that it cancels out the cycleTime on the real feature part.  
  // + 8) Additional gotcha's and bugginess.
  // +    a) Feature ratios can change over time and this must be mapped to multiple
  // +       bom arcs with effective intervals
  // + 9) CycleTime on the topLevelMfgPart (machine at PDF) should not be used for ETIS type
  // +    feeds.  This is because the MTM,PDF will have no bom content, yet the cycleTime 
  // +    will gate anyway.  Plus, the ESAT extract will strictly feed all machine offset
  // +    via machine-feature offset. 
  // + 10) A specific machine->feature supplyVol is added (accumulated) at the specialFeaturePart
  // +    This allows the user to "adjust" machine content/ratio's (in IBM this applies to 
  // +    TYPE 5 front end netting (FEN(5))
  
  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);
  int t;

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
  
  // Determine the GPD part name ... and see if it exists
  std::string geoPlannerDemandPart = mpHelper.geoPlannerDemandPartName(plannerTopLevelPart, geo);
  if (! mpHelper.isPartValid(theWitRun, geoPlannerDemandPart, fileName, dataLine, lineNo, PURELY_OPTIONAL))   {
    (*theErrFacil)("PartDoesNotExistb",MclArgList() << plannerTopLevelPart << geo << mfgOptionPart << fileName << (int)lineNo << dataLine);
    return -1;
  }
  
  
  // reject if attachRate=0.0, unless we need to set it for effectivity reasons
  if (attachRate == 0.0)  {
    (*theErrFacil)("OptionAllZeroRatiosPO",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart << geo << mfgOptionPart << dataLine);
    return -1;
  }

  std::string sourceLocPdf;
  // determine the full wit name for the Box at the sourceLocPdf
  std::string mfgBoxPartName = mpHelper.mfgTopPartForGpd(theWitRun, plannerTopLevelPart, geo, sourceLocPdf);
  
  std::string fullOptionPartName(mpHelper.pdfPartname(mfgOptionPart, sourceLocPdf));
  if (! mpHelper.isPartValid(theWitRun, fullOptionPartName, fileName, dataLine, lineNo, PURELY_OPTIONAL))  {
    (*theErrFacil)("CannotSourcePureOption",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart << geo << mfgBoxPartName 
		   << mfgOptionPart << sourceLocPdf << mfgOptionPart << earlyPeriod << latePeriod << dataLine);
    return -1;
  }
  
  // determine  name of specialFeaturePart
  std::string specialFeaturePartName(mpHelper.standaloneFeaturePartName(plannerTopLevelPart, geo, mfgOptionPart, sourceLocPdf));
  int doesSpecialFeaturePartAlreadyExist = mpHelper.isPartValid(theWitRun, specialFeaturePartName, fileName, dataLine, lineNo, PURELY_OPTIONAL);


  // Easy Case !!  create new part/structure
  if (! doesSpecialFeaturePartAlreadyExist)  {

    // Add specialFeaturePart 
    witAddPartWithOperation(theWitRun, specialFeaturePartName.c_str());
    // Add BOM specialFeaturePart ==> realFeaturePart
    witAddBomEntry(theWitRun, specialFeaturePartName.c_str(), fullOptionPartName.c_str());

    // Add BOM ... GPD==>specialFeaturePart
    int bomIndexGpdToSfp;
    witGetOperationNBomEntries(theWitRun, geoPlannerDemandPart.c_str(), &bomIndexGpdToSfp);
    witAddBomEntry(theWitRun, geoPlannerDemandPart.c_str(), specialFeaturePartName.c_str());
    
    // FINISH_ME_61:
    //   I think effectivity should always be (0,nPeriods-1)
    //   the date sensitive changes should be managed by consRate and offset vectors (????
    //    witSetBomEntryEarliestPeriod ( theWitRun, geoPlannerDemandPart.c_str(), bomIndexGpdToSfp,  earlyPeriod );
    //    witSetBomEntryLatestPeriod ( theWitRun, geoPlannerDemandPart.c_str(), bomIndexGpdToSfp,  latePeriod );


    // set the consRate as adjusted featratio*usageRate
    // FINISH_ME_61
    // that's wrong, isn't it?  usageRate should apply after attachRate ?? on the bomEntry to mfgOptionPart??

    // gpdSfpConsRate = (0,0,aR,aR,...,aR,0,0);  (over effectity interval).   Default is 0.0 outside effectivity
    LgFrTimeVecFloat  gpdSfpConsRate(nPeriods, 0.0);
    //    witGetBomEntryConsRate(theWitRun, geoPlannerDemandPart.c_str(), bomIndexGpdToSfp, &gpdSfpConsRate );
    //        we don't use the witGetConsRate becuase that will default to 1.0 

    for (t=earlyPeriod; t<=latePeriod; t++) 
      gpdSfpConsRate[t] = usageRate * attachRate;
    witSetBomEntryConsRate(theWitRun, geoPlannerDemandPart.c_str(), bomIndexGpdToSfp, gpdSfpConsRate.data());


    // Set Offsets on specialFeaturePartName
    //       ... function of (mfgBoxCycleTimeDays, mfOffset, mfoPhantomFlag, mfgOptionPartCycleTimeDays)
    LgFrTimeVecFloat offsetTV(nPeriods, 0.0);
    LgFrTimeVecFloat mfgBoxCycleTimeDays = mpHelper.getCycleTimeDays(theWitRun, mfgBoxPartName);

    // mfoPhantom bills get a cycleTimeDays that is offset of mfgOptionPart's cycleTime.  
    if (mfoPhantomFlag) {
      LgFrTimeVecFloat optionCycleTimeDays = mpHelper.getCycleTimeDays(theWitRun, fullOptionPartName);
      for (t=earlyPeriod; t<=latePeriod; t++) {
	offsetTV[t] = mpHelper.calculateVariableOffset(mfgBoxCycleTimeDays[t] + mfOffsetVal - optionCycleTimeDays[t], t, theCal, nPeriods);
      }
    }
    else   {
      for (t=earlyPeriod; t<=latePeriod; t++) {
	offsetTV[t] = mpHelper.calculateVariableOffset(mfgBoxCycleTimeDays[t] + mfOffsetVal, t, theCal, nPeriods);
      }
    }
    witSetBomEntryOffset(theWitRun, specialFeaturePartName.c_str(), 0, offsetTV.data());

    // Lotsizing of Features ... This needs to be thought about some more ...
    //    LgFrTimeVecFloat nonIntLotSize(nPeriods, 0.01);
    //    witSetOperationMinLotSize(theWitRun, specialFeaturePartName.c_str(), nonIntLotSize.data());
    //    witSetOperationIncLotSize(theWitRun, specialFeaturePartName.c_str(), nonIntLotSize.data());


    // store the maxWithout in the appdata of the SpecialFeaturePart
    LgFrScePartAppData * appDataPtr;
    LgFrTimeVecFloat maxWithoutTV(nPeriods, maxWithout);
    witGetPartAppData(theWitRun, specialFeaturePartName.c_str(), (void **) &appDataPtr);
    if (appDataPtr == 0) {
      appDataPtr = new LgFrScePartAppData(0, nPeriods, nPeriods);
      appDataPtr->maxWithout(maxWithoutTV);
      witSetPartAppData(theWitRun, specialFeaturePartName.c_str(), (void *) appDataPtr);
    }
    else 
      appDataPtr->maxWithout(maxWithoutTV);      


    


    // Add "nullSub" part and structure to the geoPlannerDemandPart to mfgOptionPart BOM entry.
    //    If featureType=SCE_FEATURE_TYPE_MANDATORY,       use a globalMaxWithoutNullSubstitute.
    //    If featureType=SCE_FEATURE_TYPE_MWO,             use a maxWithoutNullSubstitute.
    //    If featureType=SCE_FEATURE_TYPE_PURELY_OPTIONAL, use a globalNullSubstitute.
    std::string nullSubstitute;
    if (featureType == SCE_FEATURE_TYPE_MANDATORY) {
      nullSubstitute = mpHelper.globalMaxWithoutNullSubstitute(sourceLocPdf);
      if (! mpHelper.isPartValid(theWitRun, nullSubstitute, fileName, 
				 dataLine, lineNo, PURELY_OPTIONAL)) {
	witAddPart(theWitRun, nullSubstitute.c_str(), WitMATERIAL);
      }
    }
    else if (featureType == SCE_FEATURE_TYPE_MWO) {
      nullSubstitute = mpHelper.maxWithoutNullSubstitute(geo, plannerTopLevelPart, mfgOptionPart);
      if (! mpHelper.isPartValid(theWitRun, nullSubstitute, fileName, dataLine, lineNo, PURELY_OPTIONAL)) {
	witAddPart(theWitRun, nullSubstitute.c_str(), WitMATERIAL);
	// Sneak the maxWithout into the supplyVol of the maxWithoutNullSubstitite
	// unless its a very small number
	// FINISH_ME_61: does this option still work ????????    check!!
	if ((attachRate * usageRate) > IPSCE_FLT_EPS) 
	  witSetPartSupplyVol(theWitRun, nullSubstitute.c_str(), maxWithoutTV.data());
      }
    }
    else if (featureType == SCE_FEATURE_TYPE_PURELY_OPTIONAL) {
      nullSubstitute = mpHelper.globalNullSubstitute(sourceLocPdf);
      if (! mpHelper.isPartValid(theWitRun, nullSubstitute, fileName, dataLine, lineNo, PURELY_OPTIONAL)) {
	witAddPart(theWitRun, nullSubstitute.c_str(), WitMATERIAL);
	LgFrTimeVecFloat bigMsupply(nPeriods, 1000000000000.0);
	witSetPartSupplyVol(theWitRun, nullSubstitute.c_str(), bigMsupply.data());
      }
    }
    
    else {
      std::cerr << "About to die: featureType =" << featureType << "\n";
      assert(featureType == -500);
    }
    
    
    
    
    // Add subBom GPD ==> specialFeaturePart => nullSub
    int nSubBom;
    witGetBomEntryNSubsBomEntries(theWitRun, geoPlannerDemandPart.c_str(), bomIndexGpdToSfp, &nSubBom);
    witAddSubsBomEntry(theWitRun, geoPlannerDemandPart.c_str(), bomIndexGpdToSfp, nullSubstitute.c_str());
    witSetSubsBomEntryExpAllowed(theWitRun, geoPlannerDemandPart.c_str(), bomIndexGpdToSfp, nSubBom, WitFALSE);
    //    witSetSubsBomEntryEarliestPeriod(theWitRun, geoPlannerDemandPart.c_str(), bomIndexGpdToSfp, nSubBom, intervalStartPeriod);
    //    witSetSubsBomEntryLatestPeriod(theWitRun, geoPlannerDemandPart.c_str(), bomIndexGpdToSfp, nSubBom, intervalEndPeriod);
    // if using the lp engine, then set the obj1 sub cost
    if (lpModelHelper_ != 0)
      witSetSubsBomEntryObj1SubCost(theWitRun, geoPlannerDemandPart.c_str(), bomIndexGpdToSfp, nSubBom, defaultGatingOptionSubPenalty);
    
    
    // Add demand on specialFeaturePart, if not already there
    std::string returnedPlannerTopLevelPart;
    std::string returnedGeo;
    std::string demandName = mpHelper.pureOptionBuildDemandName( geoPlannerDemandPart );
    if (!( mpHelper.isDemandSpecialPureOptionBuildDemand( theWitRun, specialFeaturePartName, demandName, returnedPlannerTopLevelPart, returnedGeo))) {
      witAddDemand(theWitRun, specialFeaturePartName.c_str(), demandName.c_str());
    }


  }  
  
  
  // if this is a duplicate ... then we need to think a bit.   FINISH_ME_61
  if (doesSpecialFeaturePartAlreadyExist)  {

#if 0
    // FINISH_ME_61: Now we reject duplicate: GPD, FeaturePart records
    //               Need to support EC's
    // Duplicate Records rules: can we get rid of multiple arcs??????
    //     Can't switch between mwo values !!
    //     OK to change attachRates?   yes, but won't support changes 
    
    // two ways to do this ... 
    //           1:   loop thru gpd boms looking for a match 
    //           2:   create the specialFeaturePArt and test for existence 
    //                     check for cc or sfo.   Can't mix mwo's!!
    // ===> Choice 1
    int nBomEntries;
    witGetOperationNBomEntries( theWitRun, geoPlannerDemandPart.c_str(), &nBomEntries );
    char * child;
    int j = 0; 
    for (j=0; j<nBomEntries; j++)  {
      witGetBomEntryConsumedPart( theWitRun, geoPlannerDemandPart.c_str(), j, &child );
      std::string pdf, mfgOptionPart, gg, mm;
      if ( mpHelper.isPartSpecialStandaloneFeature(theWitRun, child, gg, mm, pdf, mfgOptionPart) ) {
	if (mfgOptionPart == mfgOptionPart) {
	  (*theErrFacil)("OptionAlreadyDefined",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart << geo << mfgOptionPart << dataLine);
        witFree ( child );
        return -1;
      }
      }
      witFree ( child );    
    }    
    // ===> Choice 2
    //           2:   create the specialFeaturePArt and test for existence 
    //                     check for cc or sfo.   Can't mix mwo's!!
#endif



    
    (*theErrFacil)("OptionAlreadyDefined",MclArgList() << fileName << (int)lineNo << plannerTopLevelPart << geo << mfgOptionPart << dataLine);
    return -1;


    /*
    // find the one and only one bom entry
    int bomIndex;
    //    witGetOperationNBomEntries(theWitRun, geoPlannerDemandPart.c_str(), &bomIndex);
    
    witSetBomEntryEarliestPeriod ( theWitRun, geoPlannerDemandPart.c_str(), bomIndex,  earlyPeriod );
    witSetBomEntryLatestPeriod ( theWitRun, geoPlannerDemandPart.c_str(), bomIndex,  latePeriod );
    LgFrTimeVecFloat gpdConsRate finalUsageRate = usageRate * featureRatio[intervalStartPeriod];
    witGetBomSetBomConsRate( theWitRun,  geoPlannerDemandPart.c_str(), bomIndex, &consRate );
    int tt;
    for (tt=earlyPeriod; tt<=latePeriod; tt++) 
      consRate[tt] = usageRate + attachRate;
    witSetBomEntryConsRate(theWitRun,  geoPlannerDemandPart.c_str(), bomIndex, consRate );
  
  
    // FINISH_ME_61: convert for VARIABLE_PERIODS, deal with mfo and mfoPhantomFlag
    float * mfgTopLevelCycleTime;
    witGetPartCycleTime(theWitRun, mfgBoxPartName.c_str(), &mfgTopLevelCycleTime);
    witSetBomEntryOffset(theWitRun, specialFeaturePartName.c_str(), 0, mfgTopLevelCycleTime);
  
 
    // Add "nullSub" part and structure to the geoPlannerDemandPart to mfgOptionPart BOM entry.
    //
    // If featureType=SCE_FEATURE_TYPE_MANDATORY,       use a globalMaxWithoutNullSubstitute.
    // If featureType=SCE_FEATURE_TYPE_MWO,             use a maxWithoutNullSubstitute.
    // If featureType=SCE_FEATURE_TYPE_PURELY_OPTIONAL, use a globalNullSubstitute.
    //
    
    std::string nullSubstitute;
    if (featureType == SCE_FEATURE_TYPE_MANDATORY) {
      nullSubstitute = mpHelper.globalMaxWithoutNullSubstitute(pdf);
      if (! mpHelper.isPartValid(theWitRun, nullSubstitute, fileName, 
				 dataLine, lineNo, PURELY_OPTIONAL)) {
	witAddPart(theWitRun, nullSubstitute.c_str(), WitMATERIAL);
      }
    }
    else if (featureType == SCE_FEATURE_TYPE_MWO) {
      nullSubstitute = mpHelper.maxWithoutNullSubstitute(geo, plannerTopLevelPart, mfgOptionPart);
      if (! mpHelper.isPartValid(theWitRun, nullSubstitute, fileName, dataLine, lineNo, PURELY_OPTIONAL)) {
	witAddPart(theWitRun, nullSubstitute.c_str(), WitMATERIAL);
	// Sneak the maxWithout into the supplyVol of the maxWithoutNullSubstitite
	// unless its a very small number
	if (finalUsageRate > IPSCE_FLT_EPS) 
	  witSetPartSupplyVol(theWitRun, nullSubstitute.c_str(), maxWithout);
      }
    }
    else if (featureType == SCE_FEATURE_TYPE_PURELY_OPTIONAL) {
      nullSubstitute = mpHelper.globalNullSubstitute(pdf);
      if (! mpHelper.isPartValid(theWitRun, nullSubstitute, fileName, dataLine, lineNo, PURELY_OPTIONAL)) {
	witAddPart(theWitRun, nullSubstitute.c_str(), WitMATERIAL);
	LgFrTimeVecFloat bigMsupply(nPeriods, 1000000000000.0);
	witSetPartSupplyVol(theWitRun, nullSubstitute.c_str(), bigMsupply.data());
      }
    }
    
    else {
      std::cerr << "About to die: featureType =" << featureType << "\n";
      assert(featureType == -500);
    }
    
    // store the maxWithout in the appdata of the SpecialFeaturePart
    LgFrScePartAppData * appDataPtr;
    LgFrTimeVecFloat maxWithoutTV(nPeriods, maxWithout);
    witGetPartAppData(theWitRun, specialFeaturePartName.c_str(), (void **) &appDataPtr);
    if (appDataPtr == 0) {
      appDataPtr = new LgFrScePartAppData(0, nPeriods, nPeriods);
      appDataPtr->maxWithout(maxWithoutTV);
      witSetPartAppData(theWitRun, specialFeaturePartName.c_str(), (void *) appDataPtr);
    }
    else 
      appDataPtr->maxWithout(maxWithoutTV);      
    
    
    
    // Add subBom GPD ==> specialFeaturePart => nullSub
    int nBom;
    witGetOperationNBomEntries(theWitRun, geoPlannerDemandPart.c_str(), &nBom);
    assert(nBom > 0);
    int nSubBom;
    witGetBomEntryNSubsBomEntries(theWitRun, geoPlannerDemandPart.c_str(), nBom-1, &nSubBom);
    witAddSubsBomEntry(theWitRun, geoPlannerDemandPart.c_str(), nBom-1, nullSubstitute.c_str());
    witSetSubsBomEntryExpAllowed(theWitRun, geoPlannerDemandPart.c_str(), nBom-1, nSubBom, WitFALSE);
    witSetSubsBomEntryEarliestPeriod(theWitRun, geoPlannerDemandPart.c_str(), nBom-1, nSubBom, intervalStartPeriod);
    witSetSubsBomEntryLatestPeriod(theWitRun, geoPlannerDemandPart.c_str(), nBom-1, nSubBom, intervalEndPeriod);
    // if using the lp engine, then set the obj1 sub cost
    if (lpModelHelper_ != 0)
      witSetSubsBomEntryObj1SubCost(theWitRun, geoPlannerDemandPart.c_str(), nBom-1, nSubBom, defaultGatingOptionSubPenalty);
    
    witFree(mfgTopLevelCycleTime);
    
    // Add demand on specialFeaturePart, if not already there
    std::string returnedPlannerTopLevelPart;
    std::string returnedGeo;
    std::string demandName = mpHelper.pureOptionBuildDemandName( geoPlannerDemandPart );
    if (!( mpHelper.isDemandSpecialPureOptionBuildDemand( theWitRun, specialFeaturePartName, demandName, returnedPlannerTopLevelPart, returnedGeo))) {
      witAddDemand(theWitRun, specialFeaturePartName.c_str(), demandName.c_str());
    }
    */
  }
    
  return 0;
  
}
























  // copy over the priorities from the GPD to all the specialFeatureDemands
  // associated with this gpd.  
int 
LgFrInitialProblemForSce::sceSetPrioritiesForSpecialFeatureDemands(
    WitRun * const theWitRun,
    LgFrMultiPlantHelper & mpHelper)
{
  int nPeriods;
  int nParts;  
  char ** partList; 

  witGetParts( theWitRun, &nParts, &partList );
  witGetNPeriods(theWitRun, &nPeriods);
  

  std::string ppForGpd;
  std::string geoForGpd;

  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++)  { 
    std::string geoPlannerDemandPart(partList[i]);
      
    // Skip this one if its not a GPD
    if (! mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun, geoPlannerDemandPart, ppForGpd, geoForGpd))
      continue;

    int * priority;        
    // first get the priority of the GPD 
    int gpdNdemands;
    char ** gpdDlist;
    witGetPartDemands(theWitRun, geoPlannerDemandPart.c_str(),  &gpdNdemands, &gpdDlist);

    if (gpdNdemands == 0)
      continue;

    // assert that there is at most one demand per gpd part
    assert(gpdNdemands == 1);
    witGetDemandPriority ( theWitRun, geoPlannerDemandPart.c_str(), gpdDlist[0], &priority );

    int nBoms;
    witGetOperationNBomEntries(theWitRun, geoPlannerDemandPart.c_str(), &nBoms);
    
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nBoms; j++) {
      char * specialFeaturePartName;
      witGetBomEntryConsumedPart(theWitRun, geoPlannerDemandPart.c_str(), j, &specialFeaturePartName);
      // if the part is not a special customer choice feature, then skip it

      std::string pp;
      std::string geo;
      std::string pdf;
      std::string mfgOptionPart;

      int nDemandsSFP;
      char ** dListSFP;

      if (mpHelper.isPartSpecialCustChoiceFeature(theWitRun, specialFeaturePartName,
						    geo, pp, pdf, mfgOptionPart)) {
    	witGetPartDemands(theWitRun, specialFeaturePartName,  &nDemandsSFP, &dListSFP);
	    int jSFP = 0; // Pulled out of the for below by RW2STL
	    for (jSFP=0; jSFP<nDemandsSFP; jSFP++) {
	        if ((pp == ppForGpd) && (geo == geoForGpd)) {
	            witSetDemandPriority(theWitRun, specialFeaturePartName, dListSFP[jSFP], priority);
	        } 
	        witFree(dListSFP[jSFP]);
	    }
	    witFree(dListSFP);
      } 

      if (mpHelper.isPartSpecialStandaloneFeature(theWitRun, specialFeaturePartName,
    						  geo, pp, pdf, mfgOptionPart)) {
	    witGetPartDemands(theWitRun, specialFeaturePartName,  &nDemandsSFP, &dListSFP);
	    int jSFP = 0; // Pulled out of the for below by RW2STL
	    for (jSFP=0; jSFP<nDemandsSFP; jSFP++) {
	        if ((pp == ppForGpd) && (geo == geoForGpd)) {
	            witSetDemandPriority(theWitRun, specialFeaturePartName, dListSFP[jSFP], priority);
	        }
	        witFree(dListSFP[jSFP]);
	    }
	    witFree(dListSFP);
      }
      witFree(specialFeaturePartName);
    }

    witFree(priority);
    witFree(gpdDlist[0]);
    witFree(gpdDlist);

  }

  // Cleanup memory
  for ( i=0; i<nParts; i++) 
    witFree(partList[i]);
  witFree( partList );

  return 0;
}




int
LgFrInitialProblemForSce::preProcessNegativeDemands (
 WitRun * const theWitRun,
 LgFrMultiPlantHelper & mpHelper )

{
  char ** partList;
  int nParts, nPeriods;
  int i;

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
  
  witGetParts ( theWitRun, &nParts, &partList );
  witGetNPeriods ( theWitRun, &nPeriods );

  std::string ptlpDummy;
  std::string geoDummy;
  // loop throught the parts
  for (i=0; i<nParts; i++) {
    std::string partName (partList[i]);
    if (! mpHelper.isPartNormal(theWitRun, partName))
      continue;
    char ** dList;
    int nDemands;


    witGetPartDemands(theWitRun, partName.c_str(), &nDemands, &dList);
    int doesPartHaveNegativeDemands = 0;
    int didWeAlreadyAdjustSupplyVolForNegativeDemandVols = 0;

    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nDemands; j++) {

	
      // skip special feature demands
      if (mpHelper.isDemandSpecialPureOptionBuildDemand(theWitRun, partName, dList[j], ptlpDummy, geoDummy)) {
	witFree(dList[j]);
	continue;
      }

      // get the  demand app data for this demand.  get the demandVol
      LgFrSceDemandAppData * demandAppDataPtr;
      witGetDemandAppData(theWitRun, partName.c_str(), dList[j], (void **) &demandAppDataPtr);
      assert(demandAppDataPtr != 0);
      LgFrTimeVecFloat demandVolTVF = demandAppDataPtr->demandVol();

      
      if (demandVolTVF.min() < -0.001) {

	demandAppDataPtr->doesDemandHaveNegativeValues(1);

	// fix for SCE 5, 20080919 ... bug was: overadjusting of supplyVol when
	// there are multiple negative demand vol streams for a single part
	// fix: once you find one negative demand, skip the others
	if (! didWeAlreadyAdjustSupplyVolForNegativeDemandVols) {
	  mpHelper.setDoesPartHaveNegativeDemands(theWitRun, partName, 1);
	  // adjust the supplyVol 
	  // negDemVol = demandVol - appDataDemandVol;
	  // supplyVol += negDemVol
	  
	  LgFrTimeVecFloat negDemVol(mpHelper.negDemVol(theWitRun, partName));
	  float * supplyVol;
	  witGetPartSupplyVol(theWitRun, partName.c_str(), &supplyVol);
	  int t = 0; // Pulled out of the for below by RW2STL
	  for (t=0; t<nPeriods; t++) {
	    supplyVol[t] += negDemVol[t];
	  }
	  witSetPartSupplyVol(theWitRun, partName.c_str(), supplyVol);
	  witFree(supplyVol);
	  didWeAlreadyAdjustSupplyVolForNegativeDemandVols = 1;
	}
      }
      witFree(dList[j]);
      continue;
    }
    witFree(dList);
    witFree(partList[i]);
  }
  witFree(partList);

  return 0;
}								   



// preprocess multiple bop entries
// This routine looks through all the parts in the WIT model
// and for any part with multiple bop entries "turns on" the lowest
// preference bop in the appropriate effectivity period and
// "turns off" all the other bops.
int
LgFrInitialProblemForSce::scePreProcessMultiBopEntries (
 WitRun * const theWitRun,
 bool useMultiRouteImplosion)

{
  char ** partList;
  int nParts;
  int nPeriods;
  int i; // the part index
  
  // FINISH_ME: 2.31 TEST, TEST, TEST  
  // need to figure out what the ramification is of this now in release 4.0
  if (useMultiRouteImplosion) {
    std::cout << "    .... multi-bop OK (using multi-route implosion) ..." << std::endl;
    return 0;
  }


  witGetParts ( theWitRun, &nParts, &partList );
  witGetNPeriods ( theWitRun, &nPeriods );


  
  for (i=0; i<nParts; i++) {
    // A 2.30 fix for useMultiRouteImplosion ONLY ...
    // For 2.30, if the user has specified UseMultiRouteImplosion, then we
    // want to only do splitBop on parts that are produced by interplants.  This
    // is because the interplant logic is based on multiBop's AND subs where we 
    // want the sub logic to be used (ie, net primary, net sub, explode primary,
    // explode sub) rather than the straight multiRoute bop logic (ie, net primary
    // explode primary, net sub, explode sub).
    //
    // So as a quick fix for MD (what else is new), we'll only do this splitbop
    // thing for parts which have at least one bop from an inpterplant operation.
    // This will cause parts which are "build or buy" to go through the splitbop and
    // they won't be handled smartly through multiRoute  (ie, only the primary route 
    // will be available).
    // 
    // get the number of interplant operations producing this part
    // if there aren't multiple, then there's nothing to process
    // go on to the next part
    if (useMultiRouteImplosion) {
      LgFrScePartAppData * appDataPtr;
      witGetPartAppData(theWitRun, partList[i], (void **) &appDataPtr);
      
      if ( appDataPtr == 0) // if there's no appData, than no interplants..
	continue;
      int nInterplantOps = appDataPtr->nInterplantOps();
      if ( nInterplantOps < 1)
	continue;
    }


    
    int  npBops;  // number of bops producing the i-th part
    witGetPartNProducingBopEntries ( theWitRun, partList[i], & npBops );

    if ( npBops == 0 )
      continue;

    else if ( npBops == 1 ) {
      // be safe and set expAllowed=true (though this is the default and it should
      // already be set as such)
      char * opName;
      int bopIndex;
      witGetPartProducingBopEntry ( theWitRun, partList[i], 0, &opName, &bopIndex);
      witSetBopEntryExpAllowed ( theWitRun, opName, bopIndex, WitTRUE );
      witFree ( opName );   
    }

    else { // npBops > 1
      std::string partName (partList[i]);
      LgFrScePartSetOfBops setOfBops(theWitRun, partName);

      // initialize with all bops "turned off" throughout time...
      bool * bopOnInPeriod = new bool[nPeriods];
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++)
        bopOnInPeriod[t] = false; 
      
      // loop through the bop entries in order of increasing preference value...
      int p = 0; // Pulled out of the for below by RW2STL
      for (p=0; p<npBops; p++) {
        LgFrSceBop & myBop = setOfBops[p];

        // if the bop is not effective, then the bop is "turned off". go to the next bop.
        if ( myBop.early () > myBop.late() ) 
          witSetBopEntryExpAllowed ( theWitRun, myBop.opName().c_str(), myBop.bopIndex(), WitFALSE );

        // else bop is effective.
        //      find the first and last periods of a time segment that this bop
        //      can be "turned on", if they exist.
        else {
          int first;  // first period of segment where bop is lowest priority
          int last;   // last period consecutive from first, where bop is lowest priority
          first = myBop.early();
          last = - 1;

          // while you haven't looked at every period this bop is effective, keep going...
          while ( last < myBop.late() ) {
            
            // find the "first" period this bop has lowest priority
            while ( first <= myBop.late() ) {
              if ( bopOnInPeriod[first] != true )
                break;
              first++;
            }
            
            // if we couldn't find a "first" period to turn on the bop,
            // then it's not explodeable. Go on to the next bop.
            if (first > myBop.late() ) {
              witSetBopEntryExpAllowed( theWitRun,
                                       myBop.opName().c_str(),
                                       myBop.bopIndex(),
                                       WitFALSE );
              break; 
            }
            
            // find the "last" period consecutive from the first that this bop is effective.
            // we know that there is at least one (namely period "first").
            last = first;
            while ( last <= myBop.late() ) {
              if ( bopOnInPeriod[last] != false )
                break;
              last++;
            }
            last = last - 1;
          
            // if you got here, you have a segment of time from "first" to "last"
            // where this bop is effective and has lowest priority.
            // turn the bop on in the periods from first to last...
            int t = 0; // Pulled out of the for below by RW2STL
            for (t=first; t<=last; t++)
              bopOnInPeriod[t] = true;
            
            // Now, it is one of 4 possible cases.
            // Case 1: first-last is the whole effectivity range.
            //         (The nicest case) Turn "on" the bop for the whole range.
            //         Process the byProduct flag in WIT and be done.
            // Case 2: first-last is the latter part of the effectivity range
            //         (almost as nice) 
            //         Split the bop into 2 parts.
            //         The first part is "off", the last part is "on".
            //         Process the byProduct flags in WIT and be done.
            // Case 3: first-last is the early part of the effectivity range
            //         Split the bop into 2 parts.
            //         The first part is "on", the last part still needs to be looked at.
            //         Process the first parts' byProduct flag in WIT, and
            //         set up the latter part to be looked at in the next iteration.
            // Case 4: first-last is strictly contained in the effectivity range
            //         This is the messiest case.
            //         Split the bop into 3 parts.
            //         The first part is "off", the middle part is "on", and
            //         the last part still needs to be looked at.
            //         Process the byProduct flags of the first and middle parts in WIT.
            //         Then, set up the latter part to be looked at in the next iteration.

            // Case 1: first-last is the whole effectivity range
            if (( first == myBop.early() ) && ( last == myBop.late() )) 
              witSetBopEntryExpAllowed ( theWitRun, myBop.opName().c_str(),
                                         myBop.bopIndex(),
                                         WitTRUE );
            // Case 2: first-last is the latter part of the effectivity range
            if (( first > myBop.early() ) && ( last == myBop.late() )) {
              LgFrSceBop newBop = myBop.splitBop( theWitRun, first-1);
              witSetBopEntryExpAllowed ( theWitRun, myBop.opName().c_str(),
                                        myBop.bopIndex(),
                                        WitFALSE );
              witSetBopEntryExpAllowed ( theWitRun, newBop.opName().c_str(),
                                        newBop.bopIndex(),
                                        WitTRUE );
            }
            // Case 3: first-last is the early part of the effectivity range
            if (( first == myBop.early() ) && ( last < myBop.late() )) {
              LgFrSceBop newBop = myBop.splitBop( theWitRun, last );
              witSetBopEntryExpAllowed ( theWitRun, myBop.opName().c_str(),
                                        myBop.bopIndex(),
                                        WitTRUE );
              
              // myBop becomes "newBop" (RLH: there's got to be a cleaner way....)
              myBop.early(newBop.early());
              myBop.late(newBop.late());
              myBop.bopIndex(newBop.bopIndex());
              // still have to process the rest of the effectivity periods....
              first = myBop.early();
            }
            // Case 4: first-last is strictly contained in the effectivity range         
            if (( first > myBop.early() ) && ( last < myBop.late() )) {
              LgFrSceBop newBop = myBop.splitBop( theWitRun, first-1);
              LgFrSceBop newerBop = newBop.splitBop( theWitRun, last );
              witSetBopEntryExpAllowed ( theWitRun, myBop.opName().c_str(),
                                        myBop.bopIndex(),
                                        WitFALSE );
              witSetBopEntryExpAllowed ( theWitRun, newBop.opName().c_str(),
                                        newBop.bopIndex(),
                                        WitTRUE );
              // myBop becomes newerBop 
              myBop.early(newerBop.early());
              myBop.late(newerBop.late());
              myBop.bopIndex(newerBop.bopIndex());
              // still have to process the rest of the effectivity periods...
              first = myBop.early();
            }
            
          } // end while loop
          
        } // end "else bop is effective"
        
      } //  end "p loop over npBops for part i"
      
      delete []  bopOnInPeriod;
      
    } // end "else npBops > 1"
  } // end  "i loop over parts"

  for (i=0; i<nParts; i++) 
    witFree(partList[i]);
  witFree(partList);
    
  return (0); 
}



// preprocess multiple interplant entries

// RLH: note
// A part can be produced by many interplantOps
// and there can be multiple bops b/t the part
// and any given interplantOp. But each interplantOp
// -------- has only bom ---------------------
//
// This routine looks through all the parts in the WIT model
// for those produced from multiple interplant operations.
// If multiple interplant operations exist, then the
// supplier part at alternative
// interplant locations are made substitutes for the 
// supplier part at the preferred interplant location.
//
// Note: the 2.0 implementation of this method makes
// some strong assumptions about the input 
// data into SCE:
// - that the transit time for alternative interplants is equal
// to the transit time of the preferred interplant (not valid for ECA
// but currently offsets on subBomEntries are not available in WIT).
int
LgFrInitialProblemForSce::scePreProcessMultiInterPlantEntries (
 WitRun * const theWitRun,
 LgFrMultiPlantHelper & mpHelper )

{
  char ** partList;
  int nParts;
  int i; // the part index
  int p; // the "bops producing the ith-part" index

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
  
  witGetParts ( theWitRun, &nParts, &partList );

  // loop throught the parts
  for (i=0; i<nParts; i++) {
    std::string partName (partList[i]);
    
    // get the number of interplant operations producing this part
    // if there aren't multiple, then there's nothing to process
    // go on to the next part
    LgFrScePartAppData * appDataPtr;
    witGetPartAppData(theWitRun, partName.c_str(), (void **) &appDataPtr);

    if ( appDataPtr == 0) // if there's no appData, than no interplants..
      continue;

    // SCE 5.00
    // We need to disableJumperArcs for cases where the part uses proportional sourcing
    if (appDataPtr->sourceCode() == "AFP")
      continue;

    int nInterplantOps = appDataPtr->nInterplantOps();
    if ( nInterplantOps <= 1)
      continue;

    // ok, we have a winner. This part has multiple interplant
    // operations that produce it
    int  npBops;  // number of bops producing the i-th part
    witGetPartNProducingBopEntries ( theWitRun, partName.c_str(), & npBops );
    assert ( npBops > 1 ); // can't have multiInterplant without multiBops!

    LgFrScePartSetOfBops setOfBops(theWitRun, partName);

    // we're going to have to working with the interplant operations for
    // this part. so create an vector of them, no duplicates allowed.
    // (FINISH_ME: It's be nice to have a new class, something like
    // "LgFrScePartSetOfInterplantOps"
    // where each member of the class would know the interplant
    // operation name and the fullWitPartName for the source part
    // but this is only needed for the jumperArcs so...maybe not.

    std::string * setOfInterplantOps = new std::string[nInterplantOps];
    int interplantOpIndex=0;
    for (p=0; p<npBops; p++) {
      LgFrSceBop producingBop = setOfBops[p];
      std::string interplantOpName = producingBop.opName();
      // if the operation isn't an interplant op, then we're not
      // interested in it
      std::string dPart;
      std::string dPdf;
      std::string sPart;
      std::string sPdf;
      if (! mpHelper.isOperationSpecialInterplant(
                       theWitRun, interplantOpName.c_str(),
                       dPart, dPdf, sPart, sPdf))
        continue;
      // otherwise if it's not in our list, add it
      int isADuplicate = 0;
      int k = 0; // Pulled out of the for below by RW2STL
      for (k=0; k<nInterplantOps; k++) {
        if (0 == interplantOpName.compare(setOfInterplantOps[k])) {
          isADuplicate = 1;
          break;
        }
      }
      if ( ! isADuplicate ) {
        setOfInterplantOps[interplantOpIndex] = interplantOpName;
        interplantOpIndex++;
      }
    } // ok, we've got our set of interplant ops for this part.
    // there should be multiple interplant operations
    assert (interplantOpIndex > 1);
    assert (interplantOpIndex == nInterplantOps);
    
    // loop through the bops in order of increasing preference
    // note: the lower the preference, the more "preferred" it is.
    // looking for bops to interplantOperations that have expAllowed=false.
    for (p=0; p<npBops; p++) {
      LgFrSceBop & currentBop = setOfBops[p];
      std::string currentInterplantOp = currentBop.opName();
      int currentInterplantBopIndex = currentBop.bopIndex();
      
      std::string destPart;
      std::string destPdf;
      std::string sourcePart;
      std::string sourcePdf;
        
      if (! mpHelper.isOperationSpecialInterplant(
                       theWitRun, currentInterplantOp.data(),
                       destPart, destPdf, sourcePart, sourcePdf))
        continue;
      
      witBoolean expAllowed;
      witGetBopEntryExpAllowed( theWitRun, currentInterplantOp.c_str(),
                               currentInterplantBopIndex, &expAllowed);

      // FINISH_ME: 2.31 THIS IS A TEST!!!!!!
      //      if ( expAllowed )
      //        continue;

      // the currentInterplantOp is a winner!
      // we've found an interplant operation with a bop that's "off"
      // (i.e. expAllowed = false)
      // Make the source part for this interplant operation a sub
      // for the source parts at every other interplant location.
      // (actually, you'd only have to make it a sub for the source
      //  that's at the location which is in effect...but
      //  finding that bugger would be more work than we want to do
      //  right now...)

      // 2.30: Ok, look at the bopAppData and if the expAllowed flag
      // is true, then we will need to make the sub arc explodeable 
      
      int k = 0; // Pulled out of the for below by RW2STL
      for (k=0; k<nInterplantOps; k++) {
        // avoid self-substitution
        if (0 == currentInterplantOp.compare(setOfInterplantOps[k]))
          continue;

        std::string sourcePdfPartName;
        mpHelper.interplantSourcePdfPartName( theWitRun, currentInterplantOp.c_str(),
                                              sourcePdfPartName );
        
        std::string primaryInterplantOp = setOfInterplantOps[k];
        int nBomEntries;        
        witGetOperationNBomEntries( theWitRun,
                                    primaryInterplantOp.c_str(), &nBomEntries );
        // by construction ea interplant
        // op has exactly one bom, but just to be safe...
        if ( nBomEntries != 1 ) {
	  (*theErrFacil)("InterplantOperationBomError");
        }

        int subBomEntryIndex ;
        witGetBomEntryNSubsBomEntries( theWitRun, primaryInterplantOp.c_str(),
                                       0, &subBomEntryIndex );
        witAddSubsBomEntry( theWitRun, primaryInterplantOp.c_str(), 0,
                            sourcePdfPartName.c_str() );
        
        // the usageRate from the interplant record has since become the usage rate
        // on the BOM b/t the current interplant operation and sourcePdfPart. Get this
        // value and set the usageRate of the substitute equal to this value.
        float usageRate;
        witGetBomEntryUsageRate( theWitRun, currentInterplantOp.c_str(), 0, &usageRate );
        witSetSubsBomEntryUsageRate( theWitRun, primaryInterplantOp.c_str(), 0,
                                     subBomEntryIndex, usageRate );

	// 2.30:  We must shut all expAllowed off as a default.  We'll turn them back
	//   on later for those that qualify.
        witSetSubsBomEntryExpAllowed( theWitRun, primaryInterplantOp.c_str(), 0,
                                     subBomEntryIndex, WitFALSE );


        // the earliest/latest periods from the interplant record have since
        // become the earliest/latest period attributes on the current
        // interplant bop. Get these
        // values and set the earliest/latest periods of the substitue equal to these
        // values.
        int early = currentBop.early();
        int late = currentBop.late();      
        witSetSubsBomEntryEarliestPeriod( theWitRun, primaryInterplantOp.c_str(),
                                          0, subBomEntryIndex, early );
        witSetSubsBomEntryLatestPeriod( theWitRun, primaryInterplantOp.c_str(),
                                        0, subBomEntryIndex, late );

        // the SUB BOM offset (transitTime), must be set to the existing BOM offset
        // for the subbed interplant arc
        float * offset;
        witGetBomEntryOffset( theWitRun, currentInterplantOp.c_str(), 0, &offset );
        witSetSubsBomEntryOffset( theWitRun, primaryInterplantOp.c_str(), 0,
                                     subBomEntryIndex, offset );
        witFree(offset);
        

        // get the "explodable" flag for the alternative interplant bop
        // and use it to set the "explodable" flag for the new sub
        LgFrSceBopAppData * currentBopAppDataPtr;
        witGetBopEntryAppData( theWitRun, currentInterplantOp.c_str(), currentInterplantBopIndex,
                                (void **) &currentBopAppDataPtr );
        assert( currentBopAppDataPtr != 0 );
        

	int expAllowed = currentBopAppDataPtr->expAllowed();
	if (expAllowed) {
	  witSetSubsBomEntryExpAllowed( theWitRun, primaryInterplantOp.c_str(), 0, subBomEntryIndex, 
					expAllowed);
	  witSetSubsBomEntryExpNetAversion( theWitRun, primaryInterplantOp.c_str(), 0, subBomEntryIndex, 
					    currentBop.preference());
	}

        // the app data new'd here get's cleaned up in the scenario destructor
        LgFrSceSubBomAppData * subBomAppDataPtr =
          new LgFrSceSubBomAppData( currentBop.preference(),
                                    currentBopAppDataPtr->isObjectInEffectForOptExplosion() );
        assert (subBomAppDataPtr != 0 );
        witSetSubsBomEntryAppData( theWitRun, primaryInterplantOp.c_str(), 0,
                                   subBomEntryIndex, (void *) subBomAppDataPtr );

      } // end k-loop over nInterplantOps
      
    }  // end p-loop over npBops

    delete [] setOfInterplantOps;
    
  } // loop over nParts

  for (i=0; i<nParts; i++) 
    witFree(partList[i]);
  witFree(partList);

  
  return 0;
}




#ifdef BUILDING_BLOCKS_VIA_MULTI_BOPS
// This routine uses MRP to explode the demand vols down to 
// the featureSetPart
int 
LgFrInitialProblemForSce::scePreProcessViaMrpViaMultiRoute(
    WitRun * const theWitRun, 
    int nperiods,
    const LgFrCalendar & theCal,
    LgFrMultiPlantHelper & mpHelper)
{

  int nPeriods;
  int nParts;  
  char ** partList; 
  int i;            
  int nOperations;  
  char ** opList; 
  int o;
  
  witGetParts( theWitRun, &nParts, &partList );
  nPeriods = theCal.nPeriods();




  
  // run MRP
  std::cout << "    ... running witMrp() ..." << std::endl;    
  witSetTruncOffsets(theWitRun, WitTRUE);
  witMrp(theWitRun);
  witSetTruncOffsets(theWitRun, WitFALSE);
  
  std::string plannerTopLevelPart;
  std::string geo;
  std::string bbCategory;
  std::string mfgOptionPart;
  
  float * mrpProdVol;

  float * supplyVol;

  // Loop thru every part, looking for optionRatioSupply parts OR demandUpperBound parts
  // or maxWithoutNullSubstitutes
  std::cout << "    ... scanning partlist ..." << std::endl;        
  for (i=0; i<nParts; i++)  { 
    std::string fullWitPartname(partList[i]);

    // This is what you do for OptionRatioSupply parts
    if (mpHelper.isPartSpecialOptionRatioSupply(theWitRun, fullWitPartname, plannerTopLevelPart,
					  geo, bbCategory, mfgOptionPart))   {

      std::cout << "    ... ..... found specialOptionRatioSupply ..." << std::endl;        
      
      // get the reqVol for the OptionRatioSupply part.  This is equivalent to 
      // the exploded demand for the BB option.  Set the supplyVol of the 
      // OptionRatioSupply part to the reqVol and we're done!
      float * reqVol;
      witGetPartReqVol(theWitRun, fullWitPartname.c_str(), &reqVol);
      
      witSetPartSupplyVol(theWitRun, fullWitPartname.c_str(), reqVol);
      witFree(reqVol);
    }
    
    // This is what you do for Demand UpperBound Parts
    if (mpHelper.isPartSpecialGeoPlannerDemandUbPart(theWitRun, fullWitPartname,
						     plannerTopLevelPart,
						     geo))   {
      float * reqVol;
      // get the reqVol for this DemandUB Part.  
      witGetPartReqVol(theWitRun, fullWitPartname.c_str(), &reqVol);
      
      // And just to make life more complicated, we need to pick up any
      // reqVol for the parent part which is the Special Demand part
      std::string demandPart = mpHelper.geoPlannerDemandPartName(plannerTopLevelPart,
							       geo);
      float * reqVolParent;
      witGetPartReqVol(theWitRun, demandPart.c_str(), &reqVolParent);
      
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++)
	reqVol[t] += reqVolParent[t];
      
      witSetPartSupplyVol(theWitRun, fullWitPartname.c_str(), reqVol);
      witFree(reqVol);
      witFree(reqVolParent);
    }
    
    // This is what you do for maxWithoutNullSubstitues
    if (mpHelper.isPartSpecialMaxWithoutNullSubstitute(theWitRun, fullWitPartname, geo,
						       plannerTopLevelPart,
						       mfgOptionPart))   {
      std::string maxWithoutNullSubPart(fullWitPartname);
      
      // get the mrpProdVol for the geoPlannerDemandPart.
      std::string geoPlannerDemandPart = mpHelper.geoPlannerDemandPartName(
									 plannerTopLevelPart, geo);
      witGetPartMrpProdVol(theWitRun, geoPlannerDemandPart.c_str(), &mrpProdVol);
      
      // get the supplyVol for the maxWithoutNullSubstitute part
      // which is temporarily holding the maxWithout ratio
      witGetPartSupplyVol(theWitRun, maxWithoutNullSubPart.c_str(), &supplyVol);
      
      // Now multiply the demand for the geoPlannerDemandPart by the maxWithout
      // ratio for the feature and stuff that back in as the supplyVol for the
      // maxWithoutNullSubPart.
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++)  
	supplyVol[t] *= mrpProdVol[t];
      
      witSetPartSupplyVol(theWitRun, maxWithoutNullSubPart.c_str(), supplyVol);
      witFree(mrpProdVol);
      witFree(supplyVol);
    }
    
    // CUSTOMER_CHOICE_FEATURES
    // This is what you do for customer choice features
    std::string pppdf;
    if (mpHelper.isPartSpecialCustChoiceFeature(theWitRun, fullWitPartname, geo,
						plannerTopLevelPart, pppdf,
						mfgOptionPart))   {
      // HOT
      // Find the name of nullSub for this cc feature
      std::string ccNullSubPart(mpHelper.custChoiceNullSubstitute(geo, plannerTopLevelPart, mfgOptionPart));

      // get the mrpProdVol for the geoPlannerDemandPart.
      std::string geoPlannerDemandPart = mpHelper.geoPlannerDemandPartName(
									 plannerTopLevelPart, geo);
      witGetPartMrpProdVol(theWitRun, geoPlannerDemandPart.c_str(), &mrpProdVol);
      
      // get the supplyVol for the maxWithoutNullSubstitute part
      // which is temporarily holding the featureRatio
      witGetPartSupplyVol(theWitRun, ccNullSubPart.c_str(), &supplyVol);
      
      // Now multiply the demand for the geoPlannerDemandPart by the complement
      // of the feature ratio  and stuff that back in as the supplyVol for the
      // ccNullSubPart.
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++)  
	supplyVol[t] = (1.0 - supplyVol[t]) * mrpProdVol[t];
      
      witSetPartSupplyVol(theWitRun, ccNullSubPart.c_str(), supplyVol);

      // Now set this supplyVol away for future use in MRP
      LgFrTimeVecFloat suppTVF(nPeriods, supplyVol);
      mpHelper.custChoiceDummySupplyVolForMrpAdjustment(theWitRun, fullWitPartname, suppTVF);

      witFree(mrpProdVol);
      witFree(supplyVol);
    }

  }

  std::cout << "    ... finished SCE pre-processing ..." << std::endl;        
  
  // Cleanup memory
  for ( i=0; i<nParts; i++) 
    witFree(partList[i]);
  witFree( partList );

  
  return 0;
}

#else
// This routine uses MRP to explode the demand vols down to 
// the featureSetPart
int 
LgFrInitialProblemForSce::scePreProcessViaMrp(
    WitRun * const theWitRun, 
    int nperiods,
    const LgFrCalendar & theCal,
    LgFrMultiPlantHelper & mpHelper)
{

  int nPeriods;
  int nParts;  
  char ** partList; 
  int i;            
  
  witGetParts( theWitRun, &nParts, &partList );
  nPeriods = theCal.nPeriods();
  
  // run MRP
  std::cout << "    ... running witMrp() ..." << std::endl;    
  witSetTruncOffsets(theWitRun, WitTRUE);
  witMrp(theWitRun);
  witSetTruncOffsets(theWitRun, WitFALSE);
  
  std::string plannerTopLevelPart;
  std::string geo;
  std::string bbCategory;
  std::string mfgOptionPart;
  
  float * mrpProdVol;
  float * supplyVol;

  // Loop thru every part, looking for optionDummy parts OR demandUpperBound parts
  // or maxWithoutNullSubstitutes
  std::cout << "    ... scanning partlist ..." << std::endl;        
  for (i=0; i<nParts; i++)  { 
    std::string fullWitPartname(partList[i]);

    // This is what you do for OptionDummy's
    if (mpHelper.isPartSpecialOptionDummy(theWitRun, fullWitPartname, plannerTopLevelPart,
					  geo, bbCategory, mfgOptionPart))   {
      
      // figure out the full BB category part  for which this option is part of
      std::string fullBbCategoryPart = mpHelper.bbCategoryPartName(plannerTopLevelPart,
								 geo, bbCategory);



      // get the mrpProdVol for the bbCategoryPart.  This is equivalent to 
      // the demand for the featureSet
      witGetPartMrpProdVol(theWitRun, fullBbCategoryPart.c_str(), &mrpProdVol);
      
      // get the unique optionRatioSupply partname for this option
      std::string optionRatioSupplyPart = mpHelper.optionRatioSupplyPartName(
									   plannerTopLevelPart, geo, bbCategory, mfgOptionPart);
      
      // get the supplyVol for this part which is temporarily being used 
      // as a placeholder for the option ratio
      witGetPartSupplyVol(theWitRun, optionRatioSupplyPart.c_str(), &supplyVol);
      
      // Now multiply the demand for the feature by the ratio for the
      // option and stuff that back in as the supplyVol for the
      // optionRatioSupplyPart.
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++)  
	supplyVol[t] *= mrpProdVol[t];
      
      witSetPartSupplyVol(theWitRun, optionRatioSupplyPart.c_str(), supplyVol);
      witFree(mrpProdVol);
      witFree(supplyVol);
    }
    
    // This is what you do for Demand UpperBound Parts
    if (mpHelper.isPartSpecialGeoPlannerDemandUbPart(theWitRun, fullWitPartname,
						     plannerTopLevelPart,
						     geo))   {
      
      float * reqVol;
      // get the reqVol for this DemandUB Part.  
      witGetPartReqVol(theWitRun, fullWitPartname.c_str(), &reqVol);
      
      // And just to make life more complicated, we need to pick up any
      // reqVol for the parent part which is the Special Demand part
      std::string demandPart = mpHelper.geoPlannerDemandPartName(plannerTopLevelPart,
							       geo);
      float * reqVolParent;
      witGetPartReqVol(theWitRun, demandPart.c_str(), &reqVolParent);
      
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++)
	reqVol[t] += reqVolParent[t];
      
      witSetPartSupplyVol(theWitRun, fullWitPartname.c_str(), reqVol);
      witFree(reqVol);
      witFree(reqVolParent);
    }
    
    // This is what you do for maxWithoutNullSubstitues
    if (mpHelper.isPartSpecialMaxWithoutNullSubstitute(theWitRun, fullWitPartname, geo,
						       plannerTopLevelPart,
						       mfgOptionPart))   {
      std::string maxWithoutNullSubPart(fullWitPartname);
      
      // get the mrpProdVol for the geoPlannerDemandPart.
      std::string geoPlannerDemandPart = mpHelper.geoPlannerDemandPartName(
									 plannerTopLevelPart, geo);
      witGetPartMrpProdVol(theWitRun, geoPlannerDemandPart.c_str(), &mrpProdVol);
      
      // get the supplyVol for the maxWithoutNullSubstitute part
      // which is temporarily holding the maxWithout ratio
      witGetPartSupplyVol(theWitRun, maxWithoutNullSubPart.c_str(), &supplyVol);
      
      // Now multiply the demand for the geoPlannerDemandPart by the maxWithout
      // ratio for the feature and stuff that back in as the supplyVol for the
      // maxWithoutNullSubPart.
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++)  
	supplyVol[t] *= mrpProdVol[t];
      
      witSetPartSupplyVol(theWitRun, maxWithoutNullSubPart.c_str(), supplyVol);
      witFree(mrpProdVol);
      witFree(supplyVol);
    }
    
    // CUSTOMER_CHOICE_FEATURES
    // This is what you do for customer choice features
    std::string pppdf;
    if (mpHelper.isPartSpecialCustChoiceFeature(theWitRun, fullWitPartname, geo,
						plannerTopLevelPart, pppdf,
						mfgOptionPart))   {
      // HOT
      // Find the name of nullSub for this cc feature
      std::string ccNullSubPart(mpHelper.custChoiceNullSubstitute(geo, plannerTopLevelPart, mfgOptionPart));

      // get the mrpProdVol for the geoPlannerDemandPart.
      std::string geoPlannerDemandPart = mpHelper.geoPlannerDemandPartName(
									 plannerTopLevelPart, geo);
      witGetPartMrpProdVol(theWitRun, geoPlannerDemandPart.c_str(), &mrpProdVol);
      
      // get the supplyVol for the maxWithoutNullSubstitute part
      // which is temporarily holding the featureRatio
      witGetPartSupplyVol(theWitRun, ccNullSubPart.c_str(), &supplyVol);
      
      // Now multiply the demand for the geoPlannerDemandPart by the complement
      // of the feature ratio  and stuff that back in as the supplyVol for the
      // ccNullSubPart.
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++)  {
	//	std::cout << "fr(ccNullSub,"         << t << "," << ccNullSubPart << " = " << supplyVol[t] << std::endl;
	//	std::cout << "mrpProdVol(ccNullSub," << t << "," << ccNullSubPart << " = " << mrpProdVol[t] << std::endl;
    	supplyVol[t] = (1.0f - supplyVol[t]) * mrpProdVol[t];
	//	std::cout << "supplyVol(ccNullSub," << ccNullSubPart << " = " << supplyVol[t] << std::endl;
      }
      
      witSetPartSupplyVol(theWitRun, ccNullSubPart.c_str(), supplyVol);

      // Now set this supplyVol away for future use in MRP
      LgFrTimeVecFloat suppTVF(nPeriods, supplyVol);
      mpHelper.custChoiceDummySupplyVolForMrpAdjustment(theWitRun, fullWitPartname, suppTVF);

      witFree(mrpProdVol);
      witFree(supplyVol);
    }

  }

  std::cout << "    ... finished SCE pre-processing ..." << std::endl;        
  
  // Cleanup memory
  for ( i=0; i<nParts; i++) 
    witFree(partList[i]);
  witFree( partList );
  
  return 0;
}
#endif


// This routine uses MRP to explode the demand vols down to 
// the featureSetPart
int 
LgFrInitialProblemForSce::checkAndFixIllegalSubNetAllowedCases(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper)
{

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
   


  witAttr objItrState;
  witResetObjItr(theWitRun);
  
  witBoolean areWeDoneYet = WitFALSE;
  
  while ( ! areWeDoneYet) {
    witAdvanceObjItr(theWitRun);
    witGetObjItrState(theWitRun, &objItrState);
    
    if (objItrState == WitAT_SUB_ENTRY) {
      char * opName;
      int b, s;
      witGetObjItrSubsBomEntry(theWitRun, &opName, &b, &s);

      // we are looking for case where sub has netAllowed=T and execPen>0 on the bomEntry or any Subs, then we
      // force the netAllowed==>FALSE
      witBoolean netAllowed;
      witGetSubsBomEntryNetAllowed(theWitRun, opName, b, s, &netAllowed);

      witBoolean doWeNeedToForceNetAllowedToZero = WitFALSE;
      if (netAllowed) {
	// first test if the subExecPen>0
	float thisSubExecPen;
	witGetSubsBomEntryExecPenalty(theWitRun, opName, b, s, &thisSubExecPen);
	if (thisSubExecPen > 0.0)
	  doWeNeedToForceNetAllowedToZero = WitTRUE;
	else {
	  // now test for bomExecPen>0
	  float bomExecPenalty;
	  witGetBomEntryExecPenalty(theWitRun, opName, b, &bomExecPenalty);
	  if (bomExecPenalty > 0.0) {
	    doWeNeedToForceNetAllowedToZero = WitTRUE;
	  }
	  else {
	    // now check all the other subs execPen > 0
	    int nSubsBomEntries;
	    witGetBomEntryNSubsBomEntries(theWitRun, opName, b, &nSubsBomEntries);
	    int sb;
	    for (sb=0; sb<nSubsBomEntries; sb++) {
	      float otherSubExecPen;
	      witGetSubsBomEntryExecPenalty(theWitRun, opName, b, sb, &otherSubExecPen);
	      if (otherSubExecPen > 0.0) {
		doWeNeedToForceNetAllowedToZero=WitTRUE;
		break;
	      }
	    }
	  }
	}
	if (doWeNeedToForceNetAllowedToZero) {
	  witSetSubsBomEntryNetAllowed(theWitRun, opName, b, s, WitFALSE);

	  
	  std::string parent = mpHelper.partname(opName);
	  std::string pdf = mpHelper.pdf(opName);
	  char * primary;
	  witGetBomEntryConsumedPart(theWitRun, opName, b, &primary);
	  char * sub;
	  witGetSubsBomEntryConsumedPart(theWitRun, opName, b, s, &sub);
	  (*theErrFacil)("ForcingSubNetAllowedFalse",MclArgList() 
			 << parent
			 << primary
			 << sub 
			 << pdf );
	  witFree(sub);
	  witFree(primary);
	}

      }
      witFree(opName);
    }
    
    if (objItrState == WitINACTIVE) {
      areWeDoneYet = WitTRUE;
    }
    
  }
  return 0;
}












// Add a substitute BOM record:  
// Note: you must set the flag isParentAnOperation to true or false
int
LgFrInitialProblemForSce::sceAddSubstitute(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    bool isParentAnOperation,
    const std::string & mfgParentPartName, 
    const std::string & parentPdf, 
    const std::string & mfgChildPartName, 
    const std::string & childPdf, 
    const std::string & mfgSubPartName,
    const std::string & subPdf,
    float usageRate,
    int  early, 
    int late,
    int fallout,
    float subCost,
    int isObjectInEffectForOptExplosionFlag,
    int isSubstituteBuildable,
    bool subNetAllowed,
    //    float groupSharingExecPenaltyThresh,
    float execPenalty,
    float   offsetDelta,
    float propRtg_Share,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{

    int nBomEntries;
    char * child;

    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    std::string fullParentName(mpHelper.pdfPartname(mfgParentPartName, parentPdf));
    std::string fullChildName( mpHelper.pdfPartname(mfgChildPartName,  childPdf));
    std::string fullSubName( mpHelper.pdfPartname(mfgSubPartName,  subPdf));
    

    // first make sure the parts/operations are valid based on what they were 
    // supposed to be.  NOTE: its a FINISH_ME for future release to let the 
    // op sub bom subs and children be anything.
    int isParentValidPart = mpHelper.isPartValid(theWitRun, mfgParentPartName, parentPdf, 
						 fileName, dataLine, lineNo, PURELY_OPTIONAL);
    int isParentValidOperation = mpHelper.isOperationValid(theWitRun, mfgParentPartName, parentPdf, 
							   fileName, dataLine, lineNo, PURELY_OPTIONAL);

    // The first case is for Records where the parent is supposed to be an Operation
    if (isParentAnOperation && (isParentValidPart || !isParentValidOperation)) {
      (*theErrFacil)("UnrecognizedOperationPdfWarning",MclArgList() 
		     << mfgParentPartName 
		     << parentPdf 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
      return -1;	
    }

    // The second case is for Records where the parent is supposed to be a Part
    if (!isParentAnOperation && !isParentValidPart) {
      (*theErrFacil)("UnrecognizedOperationPdfWarning",MclArgList() 
		     << mfgParentPartName 
		     << parentPdf 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
      return -1;	
    }
	

    // AT this time we only support the children and subs to be parts.  In the
    // future we'll allow any combination of subbability
    if (! mpHelper.isPartValid(theWitRun, mfgChildPartName, childPdf, 
			       fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
	return -1;

    if (! mpHelper.isPartValid(theWitRun, mfgSubPartName, subPdf, 
			       fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
	return -1;


    witGetOperationNBomEntries(theWitRun, fullParentName.c_str(), &nBomEntries);
    // Now loop through the BOM of the parent and stop when you find a child match
    int foundChild = 0;
    int bestMatchingBomEntry;
    int bestOverlap = -1;
    int candidateEarly;
    int candidateLate;
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nBomEntries; j++)  {
	witGetBomEntryConsumedPart(theWitRun, fullParentName.c_str(), j, &child);
	if (child == fullChildName)  {
	    // OK, we found a candidate match.  
            // Now compute the overlap of their effectivity ranges.
	    witGetBomEntryEarliestPeriod(theWitRun, fullParentName.c_str(), j, &candidateEarly);
	    witGetBomEntryLatestPeriod(theWitRun, fullParentName.c_str(), j, &candidateLate);
	    int candidateOverlap =    (late > candidateLate ? candidateLate : late )
		                    - (early < candidateEarly ? candidateEarly : early )
                       		    + 1;
	    // Now determine if the candidate is the BEST match so far
	    if ((! foundChild) || (foundChild && (candidateOverlap > bestOverlap))) {
		bestMatchingBomEntry = j;	    
		bestOverlap = candidateOverlap;
	    }
	    foundChild = 1;

	    
	    // Now, if the candidateOverlap is positive, then we need to add a sub record 
	    // for this bomentry.  This fix means that there may be multiple subBoms per sub record.
	    // the old way used a bestOverlap mechnaism to find at most one match between sub record
	    // and bom.

	    if (candidateOverlap > 0) {

	      // make subCost(/preference) into a vector
	      int nPeriods;
	      witGetNPeriods(theWitRun, &nPeriods);
	      float * obj1SubCost = new float[nPeriods];
    
	      int t = 0; // Pulled out of the for below by RW2STL
	      for (t=0; t<nPeriods; t++)
		obj1SubCost[t] = subCost;

	      // note that in 4.0 mode, "fullParentName" is these API calls is the operation name
	      int nSubBoms;
	      witGetBomEntryNSubsBomEntries( theWitRun, fullParentName.c_str(), j, & nSubBoms);
	      float * offset;
	      witGetBomEntryOffset(theWitRun, fullParentName.c_str(), j, &offset);

	      for (t=0; t<nPeriods; t++)
		offset[t] += offsetDelta;

	      witAddSubsBomEntry( theWitRun, fullParentName.c_str(), j, fullSubName.c_str());
	      witSetSubsBomEntryEarliestPeriod( theWitRun, fullParentName.c_str(), j, nSubBoms, early);
	      witSetSubsBomEntryLatestPeriod( theWitRun, fullParentName.c_str(), j, nSubBoms, late);
	      witSetSubsBomEntryOffset(theWitRun, fullParentName.c_str(), j, nSubBoms, offset);
	      witSetSubsBomEntryExpNetAversion(theWitRun, fullParentName.c_str(), j, nSubBoms, subCost);
	      witFree(offset);


	      if (groupSharingViaExecPenalties_) {
		witSetSubsBomEntryExecPenalty(theWitRun, fullParentName.c_str(), j, 
					      nSubBoms, execPenalty);
	      }


	      // 4.10: explicitly set netAllowed as part of the subBom input file.   
	      // the DK should re-generate the subpn.dat file and set netAllowed=F if childPart is pcf
	      // default for netallowed is true



	      // 2.30: can you net  it?
	      if (subNetAllowed)
		witSetSubsBomEntryNetAllowed(theWitRun, fullParentName.c_str(), j, nSubBoms, WitTRUE);
	      else
		witSetSubsBomEntryNetAllowed(theWitRun, fullParentName.c_str(), j, nSubBoms, WitFALSE);

	      // 2.30: can you build it?
	      if (isSubstituteBuildable)
		witSetSubsBomEntryExpAllowed(theWitRun, fullParentName.c_str(), j, nSubBoms, WitTRUE);
	      else
		witSetSubsBomEntryExpAllowed(theWitRun, fullParentName.c_str(), j, nSubBoms, WitFALSE);

	      witSetSubsBomEntryFallout( theWitRun, fullParentName.c_str(), j, nSubBoms, fallout);
	      witSetSubsBomEntryUsageRate( theWitRun, fullParentName.c_str(), j, nSubBoms, usageRate);
	      // only set the obj1SubCost if we're dealing with the lp engine
	      if ( lpModelHelper_ != 0 )
		witSetSubsBomEntryObj1SubCost( theWitRun, fullParentName.c_str(), j, nSubBoms, obj1SubCost);
	      delete [] obj1SubCost;



	      float * prShare;
	      witGetSubsBomEntryRoutingShare(theWitRun, fullParentName.c_str(), j, nSubBoms, &prShare);
	      int ttt;
	      for (ttt=early; ttt <= late; ttt++) {
		prShare[ttt] = propRtg_Share;
	      }
	      witSetSubsBomEntryRoutingShare(theWitRun, fullParentName.c_str(), j, nSubBoms, prShare);
	      witFree(prShare);
	      




	      // store "preference" and OptExplosionFlag in the subBom app data
	      void * subBomAppVoidPtr;
	      LgFrSceSubBomAppData * mySubBomAppDataPtr;

	      witGetSubsBomEntryAppData( theWitRun, fullParentName.c_str(), j, nSubBoms, &subBomAppVoidPtr );
	      if ( subBomAppVoidPtr != 0 ) {
		(*theErrFacil)("InternalError4",MclArgList() 
			       << mfgParentPartName 
			       << parentPdf 
			       << mfgChildPartName 
			       << childPdf 
			       << fileName 
			       << (int)lineNo 
			       << dataLine);
	      }

	      mySubBomAppDataPtr = new LgFrSceSubBomAppData (subCost, isObjectInEffectForOptExplosionFlag);  
	      assert ( mySubBomAppDataPtr != 0 );
	      witSetSubsBomEntryAppData( theWitRun,
					 fullParentName.c_str(),
					 j,
					 nSubBoms,
					 (void *) mySubBomAppDataPtr );



	    }

	}
	witFree(child);
    }


    if (! foundChild) {
      (*theErrFacil)("MissingBillRecord",MclArgList() 
		     << mfgParentPartName 
		     << parentPdf 
		     << mfgChildPartName 
		     << childPdf 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
      return -1;
    }
    if ((foundChild) && (bestOverlap < 1)) {
      (*theErrFacil)("MissingBOMMatch",MclArgList() 
		     << mfgParentPartName 
		     << parentPdf 
		     << mfgChildPartName 
		     << childPdf 
		     << fileName 
		     << (int)lineNo 
		     << dataLine);
      return -1;
    }

    return 0;
}




#ifdef ALTERNATE_AS_SUB
// *************************************************
// Add  Alternate Part Operation and structure As SUBSTITUTE
// **************************************************

int
LgFrInitialProblemForSce::sceAddAlternatePartStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    int nPeriods,
    const std::string & primePart, 
    const std::string & primePdf, 
    const std::string & altPart,
    const std::string & altPdf,
    float usageRate,
    int  early, 
    int late,
    int fallout,
    float preference,
    int isObjectInEffectForOptExplosionFlag,
    int isExpAllowed,
    bool subNetAllowed, // not used
    float groupSharingExecPenaltyThresh,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{


  witBoolean isUnique = true;
  witBoolean exists;
  
  
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();
  
  std::string fullPrimeName( mpHelper.pdfPartname(primePart,  primePdf));
  std::string fullAltName( mpHelper.pdfPartname(altPart,  altPdf));
  
  
  // first make sure the parts are valid
  if (! mpHelper.isPartValid(theWitRun, primePart, primePdf, 
			     fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
    return -1;
  
  if (! mpHelper.isPartValid(theWitRun, altPart, altPdf, 
			     fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
    return -1;
  
  
  
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  // const std::string & destinationPart == primePart
  // const std::string & destinationPdf == primePdf
  // const std::string & sourcePart == altPart
  // const std::string & sourcePdf == altPdf
  
  
  std::string alternatePartOperationName =
    mpHelper.alternatePartOperationName(primePart, primePdf, altPart, altPdf);

  std::string globalAltDummyPartName =
    mpHelper.globalAltDummyPartName(primePdf);


  
  // test to see if the alternatePart operation already exists
  witGetOperationExists(theWitRun, alternatePartOperationName.c_str(), &exists);
  
  
  // if the alternatePart operation does NOT exist, 
  //    add the operation
  // bom record to the source part, and set some defaults.
  //
  // if the alternatePart operation DOES exists, check to see if this record is a
  // "legal duplicate". To be legal it has to satisfy 2 conditions:
  //   (1) it the same usage rate as the previous records (which is mapped
  //       to the usage rate on the one and only bom for this alternatePart
  //       operation 
  //   (2) the effectivity dates are disjoint from the effectivity
  //       dates on all the other bops for this alternatePart operation
  //   If either (1) or (2) does not hold, reject the record and print an
  //   error message.
  
  if ( ! exists ) {
    // add the dummy alternatePart operation and the source part bom entry to WIT
    witAddOperation( theWitRun, alternatePartOperationName.c_str() );

    witBoolean doesGlobalAltDummyExist;    
    witGetPartExists(theWitRun, globalAltDummyPartName.c_str(), &doesGlobalAltDummyExist);
    if (! doesGlobalAltDummyExist)
      witAddPart(theWitRun, globalAltDummyPartName.c_str(), WitCAPACITY);

    witAddBomEntry( theWitRun, alternatePartOperationName.c_str(), globalAltDummyPartName.c_str() );
    witSetBomEntryEarliestPeriod( theWitRun, alternatePartOperationName.c_str(), 0, early);
    witSetBomEntryLatestPeriod( theWitRun, alternatePartOperationName.c_str(), 0, late);

    witAddSubsBomEntry(theWitRun, alternatePartOperationName.c_str(), 0, fullAltName.c_str());
    witSetSubsBomEntryUsageRate( theWitRun, alternatePartOperationName.c_str(), 0, 0, usageRate );
    float falloutRate = (float) fallout/100.f;
    witSetSubsBomEntryFalloutRate( theWitRun, alternatePartOperationName.c_str(), 0, 0, falloutRate );
    // In our model, there is one and only bom hanging off the alternatePart operation.
    // In our model, the bom is effective through out the entire planning horizon.
    witSetSubsBomEntryEarliestPeriod( theWitRun, alternatePartOperationName.c_str(), 0, 0, early );
    witSetSubsBomEntryLatestPeriod( theWitRun, alternatePartOperationName.c_str(), 0, 0, late );
    witSetSubsBomEntryExpAllowed(theWitRun, alternatePartOperationName.c_str(), 0, 0, isExpAllowed);
    witSetSubsBomEntryExpNetAversion(theWitRun, alternatePartOperationName.c_str(), 0, 0, preference);
    if (groupSharingViaExecPenalties_) {
      witSetSubsBomEntryExecPenalty(theWitRun, alternatePartOperationName.c_str(), 0, 0, preference);
    }

    // Since this is a new alternatePart Operation for the customer loc, increment
    // the nAlternatePartOps counter in the CustLoc Parts appdata.  (Being careful,
    // of course, to ensure the non-null-ness of the appDataPtr for the part
    LgFrScePartAppData * custPartAppData;
    witGetPartAppData(theWitRun, fullPrimeName.c_str(), (void **) &custPartAppData);
    if (custPartAppData == 0) {
      custPartAppData = new LgFrScePartAppData(0, nPeriods, nPeriods);
      custPartAppData->nAlternatePartOps(1);
      witSetPartAppData(theWitRun, fullPrimeName.c_str(), (void *) custPartAppData);
    }
    else 
      custPartAppData->nAlternatePartOps(custPartAppData->nAlternatePartOps() + 1);      

  }

  if ( exists ) {
    // set unique flag = false
    isUnique = false;

    if ( ! mpHelper.isDuplicateAlternatePartRecordUsageRateValid(
                     theWitRun, fullAltName, alternatePartOperationName,
                     usageRate)) {
      (*theErrFacil)("ConflictingAlternatePartRecordsUR",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << primePart 
		     << primePdf 
		     << altPart 
		     << altPdf 
		     << dataLine);
      return (-1);
    }
    
    if ( ! mpHelper.isDuplicateAlternatePartRecordEffectivityDatesValid(
                      theWitRun, alternatePartOperationName, early, late )) {
      (*theErrFacil)("ConflictingAlternatePartRecordsED",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << primePart 
		     << primePdf 
		     << altPart 
		     << altPdf 
		     << dataLine);
      return (-1);
    }
  }


  // do a sanity check ... THERE MUST BE ONE AND ONLY ONE BOM
  int nBom;
  witGetOperationNBomEntries(theWitRun, alternatePartOperationName.c_str(), &nBom);
  assert(nBom == 1);

  // if we made it here, we either have a "valid duplicate" alternatePart
  // record or a new one. In either case, we now create a bop entry
  // between destination part and alternatePart operation,
  // with effectivity = (early, late)
  int nbBop;
  witGetOperationNBopEntries( theWitRun, alternatePartOperationName.c_str(), &nbBop );      
  witAddBopEntry( theWitRun, alternatePartOperationName.c_str(), fullPrimeName.c_str() );
  witSetBopEntryEarliestPeriod( theWitRun, alternatePartOperationName.c_str(),
                                nbBop, early );      
  witSetBopEntryLatestPeriod( theWitRun, alternatePartOperationName.c_str(),
                              nbBop, late );

  

  // Add an app data for the bop preference and explosion flag
  
  void * bopAppVoidPtr;
  LgFrSceBopAppData * myBopAppDataPtr; 
  witGetBopEntryAppData( theWitRun, alternatePartOperationName.c_str(), nbBop, &bopAppVoidPtr );
  if ( bopAppVoidPtr != 0 ) {
    (*theErrFacil)("InternalError3",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << primePart 
		   << primePdf 
		   << altPart 
		   << altPdf 
		   << dataLine);
  }
  myBopAppDataPtr = new LgFrSceBopAppData (preference, 
					   isObjectInEffectForOptExplosionFlag, 
					   isExpAllowed);  
  assert ( myBopAppDataPtr != 0 );
  witSetBopEntryAppData( theWitRun, alternatePartOperationName.c_str(),
                         nbBop, (void *) myBopAppDataPtr );


  witSetBopEntryExpAllowed(theWitRun, alternatePartOperationName.c_str(), nbBop, isExpAllowed);
  witSetBopEntryExpAversion(theWitRun, alternatePartOperationName.c_str(), nbBop, preference);

  if ( lpModelHelper_ != 0 ) {    
    float * opExecCost;
    witGetOperationObj1ExecCost(theWitRun, alternatePartOperationName.c_str(), &opExecCost);
    int t=0;
    for (t=early; t<=late; t++)
      opExecCost[t] = preference;
    witSetOperationObj1ExecCost(theWitRun, alternatePartOperationName.c_str(), opExecCost);
    witFree(opExecCost);
  }


  if (isUnique)
    return 0;
  else
    return 1;


    return 0;
}
#else
// *************************************************
// Add  Alternate Part Operation and structure As OPERATION
// **************************************************
int
LgFrInitialProblemForSce::sceAddAlternatePartStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    int nPeriods,
    const std::string & primePart, 
    const std::string & primePdf, 
    const std::string & altPart,
    const std::string & altPdf,
    float usageRate,
    int  early, 
    int late,
    int fallout,
    float preference,
    int isObjectInEffectForOptExplosionFlag,
    int isExpAllowed,
    bool subNetAllowed, // not used
    float groupSharingExecPenaltyThresh,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{


    witBoolean isUnique = true;
    witBoolean exists;


    MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

    std::string fullPrimeName( mpHelper.pdfPartname(primePart,  primePdf));
    std::string fullAltName( mpHelper.pdfPartname(altPart,  altPdf));
    

    // first make sure the parts are valid
    if (! mpHelper.isPartValid(theWitRun, primePart, primePdf, 
			       fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
	return -1;

    if (! mpHelper.isPartValid(theWitRun, altPart, altPdf, 
			       fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
	return -1;



    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // const std::string & destinationPart == primePart
    // const std::string & destinationPdf == primePdf
    // const std::string & sourcePart == altPart
    // const std::string & sourcePdf == altPdf


    std::string alternatePartOperationName =
      mpHelper.alternatePartOperationName(primePart, primePdf, altPart, altPdf);
	
    // test to see if the alternatePart operation already exists
    witGetOperationExists(theWitRun, alternatePartOperationName.c_str(), &exists);


  // if the alternatePart operation does NOT exist, then add it along with the
  // bom record to the source part, and set some defaults.
  //
  // if the alternatePart operation DOES exists, check to see if this record is a
  // "legal duplicate". To be legal it has to satisfy 2 conditions:
  //   (1) it the same usage rate as the previous records (which is mapped
  //       to the usage rate on the one and only bom for this alternatePart
  //       operation 
  //   (2) the effectivity dates are disjoint from the effectivity
  //       dates on all the other bops for this alternatePart operation
  //   If either (1) or (2) does not hold, reject the record and print an
  //   error message.

  if ( ! exists ) {

    // add the dummy alternatePart operation and the source part bom entry to WIT
    witAddOperation( theWitRun, alternatePartOperationName.c_str() );
    witAddBomEntry( theWitRun, alternatePartOperationName.c_str(), fullAltName.c_str() );
    witSetBomEntryUsageRate( theWitRun, alternatePartOperationName.c_str(), 0, usageRate );
    float falloutRate = (float) fallout/100.f;
    witSetBomEntryFalloutRate( theWitRun, alternatePartOperationName.c_str(), 0, falloutRate );
    // In our model, there is one and only bom hanging off the alternatePart operation.
    // In our model, the bom is effective through out the entire planning horizon.
    witSetBomEntryEarliestPeriod( theWitRun, alternatePartOperationName.c_str(), 0, 0 );
    witSetBomEntryLatestPeriod( theWitRun, alternatePartOperationName.c_str(), 0, nPeriods-1 );
    if (groupSharingViaExecPenalties_) {
      witSetBomEntryExecPenalty(theWitRun, alternatePartOperationName.c_str(), 0, preference);
    }

    // Since this is a new alternatePart Operation for the customer loc, increment
    // the nAlternatePartOps counter in the CustLoc Parts appdata.  (Being careful,
    // of course, to ensure the non-null-ness of the appDataPtr for the part
    LgFrScePartAppData * custPartAppData;
    witGetPartAppData(theWitRun, fullPrimeName.c_str(), (void **) &custPartAppData);
    if (custPartAppData == 0) {
      custPartAppData = new LgFrScePartAppData(0, nPeriods, nPeriods);
      custPartAppData->nAlternatePartOps(1);
      witSetPartAppData(theWitRun, fullPrimeName.c_str(), (void *) custPartAppData);
    }
    else 
      custPartAppData->nAlternatePartOps(custPartAppData->nAlternatePartOps() + 1);      

  }

  if ( exists ) {
    // set unique flag = false
    isUnique = false;

    if ( ! mpHelper.isDuplicateAlternatePartRecordUsageRateValid(
                     theWitRun, fullAltName, alternatePartOperationName,
                     usageRate)) {
      (*theErrFacil)("ConflictingAlternatePartRecordsUR",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << primePart 
		     << primePdf 
		     << altPart 
		     << altPdf 
		     << dataLine);
      return (-1);
    }
    
    if ( ! mpHelper.isDuplicateAlternatePartRecordEffectivityDatesValid(
                      theWitRun, alternatePartOperationName, early, late )) {
      (*theErrFacil)("ConflictingAlternatePartRecordsED",MclArgList() 
		     << fileName 
		     << (int)lineNo 
		     << primePart 
		     << primePdf 
		     << altPart 
		     << altPdf 
		     << dataLine);
      return (-1);
    }
  }


  // do a sanity check ... THERE MUST BE ONE AND ONLY ONE BOM
  int nBom;
  witGetOperationNBomEntries(theWitRun, alternatePartOperationName.c_str(), &nBom);
  assert(nBom == 1);

  // if we made it here, we either have a "valid duplicate" alternatePart
  // record or a new one. In either case, we now create a bop entry
  // between destination part and alternatePart operation,
  // with effectivity = (early, late)
  int nbBop;
  witGetOperationNBopEntries( theWitRun, alternatePartOperationName.c_str(), &nbBop );      
  witAddBopEntry( theWitRun, alternatePartOperationName.c_str(), fullPrimeName.c_str() );
  witSetBopEntryEarliestPeriod( theWitRun, alternatePartOperationName.c_str(),
                                nbBop, early );      
  witSetBopEntryLatestPeriod( theWitRun, alternatePartOperationName.c_str(),
                              nbBop, late );

  

  // Add an app data for the bop preference and explosion flag
  
  void * bopAppVoidPtr;
  LgFrSceBopAppData * myBopAppDataPtr; 
  witGetBopEntryAppData( theWitRun, alternatePartOperationName.c_str(), nbBop, &bopAppVoidPtr );
  if ( bopAppVoidPtr != 0 ) {
    (*theErrFacil)("InternalError3",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << primePart 
		   << primePdf 
		   << altPart 
		   << altPdf 
		   << dataLine);
  }
  myBopAppDataPtr = new LgFrSceBopAppData (preference, 
					   isObjectInEffectForOptExplosionFlag, 
					   isExpAllowed);  
  assert ( myBopAppDataPtr != 0 );
  witSetBopEntryAppData( theWitRun, alternatePartOperationName.c_str(),
                         nbBop, (void *) myBopAppDataPtr );


  witSetBopEntryExpAllowed(theWitRun, alternatePartOperationName.c_str(), nbBop, isExpAllowed);
  witSetBopEntryExpAversion(theWitRun, alternatePartOperationName.c_str(), nbBop, preference);

  if ( lpModelHelper_ != 0 ) {    
    float * opExecCost;
    witGetOperationObj1ExecCost(theWitRun, alternatePartOperationName.c_str(), &opExecCost);
    int t=0;
    for (t=early; t<=late; t++)
      opExecCost[t] = preference;
    witSetOperationObj1ExecCost(theWitRun, alternatePartOperationName.c_str(), opExecCost);
    witFree(opExecCost);
  }


  if (isUnique)
    return 0;
  else
    return 1;


    return 0;
}

#endif



// Set the Part Sourcing Structure (SCE 5.00)
int
LgFrInitialProblemForSce::sceSetSourcingStructure(
						  WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & part, 
    const std::string & pdf,
    const std::string & sourceCode, 
    float pref_cost,
    int early,
    int late,
    int nPeriods,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  std::string fullPartName(mpHelper.pdfPartname(part, pdf));


  if (! mpHelper.isPartValid(theWitRun, part, pdf, 
			     fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
    return -1;
  
  // parse the sourceCode
  
  //  A-sourced means can buy through interplant
  //  F-sourced means can Fabricate (build) in house using default operation
  //  P-sourced means it uses proportional routing
  //  S-sourced means it is single-sourced in the WIT sense

  // P-sourced and S-sourced are mutually exclusive
  size_t A_sourced = (sourceCode.find_first_of("A") == sourceCode.npos) ? 0 : 1;
  size_t F_sourced = (sourceCode.find_first_of("F") == sourceCode.npos) ? 0 : 1;
  size_t S_sourced = (sourceCode.find_first_of("S") == sourceCode.npos) ? 0 : 1;
  size_t P_sourced = (sourceCode.find_first_of("P") == sourceCode.npos) ? 0 : 1;
  
  
  
  if (P_sourced) {
    witSetPartPropRouting(theWitRun, fullPartName.c_str(), WitTRUE);
  }
  
  // if not P-sourced , then check for S-sourced
  else {
    witSetPartPropRouting(theWitRun, fullPartName.c_str(), WitFALSE);
    if (S_sourced) {
      witSetPartSingleSource(theWitRun, fullPartName.c_str(), WitTRUE);
    }
  }
  



  if  (!P_sourced  && !A_sourced && !P_sourced && !S_sourced)  {
    (*theErrFacil)("UnrecognizedSourceCode",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << part
		   << pdf
		   << dataLine);
  }




  /*
  if (sourceCode == "AFP") {
     witSetPartPropRouting(theWitRun, fullPartName.c_str(), WitTRUE);
   }
  else if (sourceCode == "AF") {
    witSetPartPropRouting(theWitRun, fullPartName.c_str(), WitFALSE);
  }
  else {
    (*theErrFacil)("UnrecognizedSourceCode",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << part
		   << pdf
		   << dataLine);
  }
  */



  


  LgFrScePartAppData * appDataPtr;
  witGetPartAppData(theWitRun, fullPartName.c_str(), (void **) &appDataPtr);
  if (appDataPtr != 0) {
    appDataPtr->sourceCode(sourceCode);
  }
  else {
    appDataPtr = new LgFrScePartAppData(0, nPeriods, nPeriods);
    appDataPtr->sourceCode(sourceCode);
  }
  witSetPartAppData(theWitRun, fullPartName.c_str(), appDataPtr);


//  if (sourceCode == "AFP") { 
  if (A_sourced && F_sourced && P_sourced) {
    // shut prodRate to 0 on bop to default operation
    // whenever we get an interplant, we turn it back on 
    witBoolean exists;
    witGetOperationExists(theWitRun, fullPartName.c_str(), &exists);
    if (exists) {
      int nBopEntries;
      witGetOperationNBopEntries(theWitRun, fullPartName.c_str(), &nBopEntries);
      assert(nBopEntries == 1);
      LgFrTimeVecFloat zeroVec(nPeriods, 0.0);
      witSetBopEntryProductRate(theWitRun, fullPartName.c_str(), 0, zeroVec.data());
    }
  }
   

    
  return 0;
}


#ifdef VARIABLE_PERIODS
// Normal BOM entry 
int
LgFrInitialProblemForSce::sceAddNormalBomStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar & theCal,
    const std::string & parent, 
    const std::string & parentPdf,
    const std::string & child, 
    const std::string & childPdf, 
    float usageRate,
    float usageTime,
    int early,
    int late,
    int fallout,
    int mec,
    int phantomFlag,
    int removeBillFlag,
    int propRtgFlag,
    float propRtgShare,
    int nPeriods,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
  std::string fullParentName(mpHelper.pdfPartname(parent, parentPdf));
  std::string fullChildName( mpHelper.pdfPartname(child,  childPdf));
  
  // remove bills get usage negated
  if (removeBillFlag)  
    usageRate = -usageRate;
  
  if (! mpHelper.isPartValid(theWitRun, parent, parentPdf, 
			     fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
    return -1;
  if (! mpHelper.isPartValid(theWitRun, child, childPdf,
			     fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
    return -1;
  
  int t = 0;
  
  // Need to convert cycleTime and (usageTime (in days, over effective interval))  ==> offset timeVec
  // Notes: cycleTime can be positive or negative
  //
  // 
  LgFrTimeVecFloat offsetTV(nPeriods, 0.0);
  LgFrTimeVecFloat parentCycleTimeDays = mpHelper.getCycleTimeDays(theWitRun, fullParentName);

  // phantom bills get a usage time that is equal to the cycle Time of the child.  
  if (phantomFlag) {
    LgFrTimeVecFloat childCycleTimeDays = mpHelper.getCycleTimeDays(theWitRun, fullChildName);
      for (t=early; t<=late; t++) {
      offsetTV[t] = mpHelper.calculateVariableOffset(parentCycleTimeDays[t] - usageTime - childCycleTimeDays[t], t, theCal, nPeriods);
    }
  }
  else   {
    for (t=early; t<=late; t++) {
      offsetTV[t] = mpHelper.calculateVariableOffset(parentCycleTimeDays[t] - usageTime, t, theCal, nPeriods);
    }
  }
  
  
  LgFrTimeVecFloat consRate(nPeriods, usageRate);
  int nBom;
  witGetOperationNBomEntries(theWitRun, fullParentName.c_str(), &nBom);
  witAddBomEntry(theWitRun, fullParentName.c_str(), fullChildName.c_str());
  witSetBomEntryConsRate(theWitRun, fullParentName.c_str(), nBom, consRate.data());
  witSetBomEntryOffset(theWitRun, fullParentName.c_str(), nBom, offsetTV.data());
  //  witSetBomEntryUsageTime(theWitRun, fullParentName.c_str(), nBom, usageTime);
  float falloutRate = (float) fallout / 100.f;
  witSetBomEntryFalloutRate(theWitRun, fullParentName.c_str(), nBom, falloutRate);
  witSetBomEntryEarliestPeriod(theWitRun, fullParentName.c_str(), nBom, early);
  witSetBomEntryLatestPeriod(theWitRun, fullParentName.c_str(), nBom, late);
  witSetBomEntryMandEC(theWitRun, fullParentName.c_str(), nBom, mec);
  if (propRtgFlag) {
    witBoolean * prFlag;
    float * prShare;
    witGetBomEntryPropRtg(theWitRun, fullParentName.c_str(), nBom, &prFlag);
    witGetBomEntryRoutingShare(theWitRun, fullParentName.c_str(), nBom, &prShare);
    int ttt;
    for (ttt=0; ttt<nPeriods; ttt++) {
      prFlag[ttt] = WitTRUE;
      prShare[ttt] = propRtgShare;
    }
    witSetBomEntryPropRtg(theWitRun, fullParentName.c_str(), nBom, prFlag);
    witSetBomEntryRoutingShare(theWitRun, fullParentName.c_str(), nBom, prShare);
    witFree(prFlag);
    witFree(prShare);
  }
#if 0
  std::cout << "Bom offset for [" << fullParentName << "->" << fullChildName << "] = (";
  for (t=0; t<nPeriods; t++) 
    std::cout << offsetTV[t] << ",";
  std::cout << std::endl;
#endif
  
  return 0;
}

#else
// the OLD WAY 
// Normal BOM entry (mandatory)
int
LgFrInitialProblemForSce::sceAddNormalBomStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar & theCal,
    const std::string & parent, 
    const std::string & parentPdf,
    const std::string & child, 
    const std::string & childPdf, 
    float usageRate,
    float usageTime,
    int early,
    int late,
    int fallout,
    int mec,
    int phantomFlag,
    int removeBillFlag,
    int propRtgFlag,
    float propRtgShare,
    int nPeriods,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    std::string fullParentName(mpHelper.pdfPartname(parent, parentPdf));
    std::string fullChildName( mpHelper.pdfPartname(child,  childPdf));

	
    // remove bills get usage negated
    if (removeBillFlag)  
	usageRate = -usageRate;


	
    if (! mpHelper.isPartValid(theWitRun, parent, parentPdf, 
			       fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
	return -1;

    if (! mpHelper.isPartValid(theWitRun, child, childPdf,
			       fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
	return -1;






#if 0
    // OLD attempt to convert usageTime to offsets
    // Need to convert from cycleTime ==> offset
    float * offset;
    int t=0;

    witGetPartCycleTime(theWitRun, fullParentName.c_str(), &offset);
    for (t=0; t<nPeriods; t++) 
      offset[t] -= usageTime;

    if (phantomFlag) {
      float * childCycleTime;
      witGetPartCycleTime(theWitRun, fullChildName.c_str(), &childCycleTime);
      for (t=0; t<nPeriods; t++) 
	offset[t] = - childCycleTime[t];
      witFree(childCycleTime);
    }
#endif


    // phantom bills get a usage time that is equal to the cycle Time
    // of the child.  The implementation is to set the usage time (a scalar)
    // to the cycleTime that is in effect for  period=early.   When WIT 4.0
    // gets implemented, we'll simply set the offset to be vector equal to the
    // negative of the childs cycleTime.  For now this is a FINISH_ME.
    if (phantomFlag) {
	int nPeriods;
	witGetNPeriods(theWitRun, &nPeriods);
	float * cycleTime;
	witGetPartCycleTime(theWitRun, fullChildName.c_str(), &cycleTime);
	usageTime = cycleTime[early];
	witFree(cycleTime);
    }
    // #endif


    LgFrTimeVecFloat consRate(nPeriods, usageRate);
    int nBom;
    witGetOperationNBomEntries(theWitRun, fullParentName.c_str(), &nBom);
    witAddBomEntry(theWitRun, fullParentName.c_str(), fullChildName.c_str());
    witSetBomEntryConsRate(theWitRun, fullParentName.c_str(), nBom, consRate.data());
    //    witSetBomEntryOffset(theWitRun, fullParentName.c_str(), nBom, offset);
    witSetBomEntryUsageTime(theWitRun, fullParentName.c_str(), nBom, usageTime);

    float falloutRate = (float) fallout / 100.f;
    witSetBomEntryFalloutRate(theWitRun, fullParentName.c_str(), nBom, falloutRate);
    witSetBomEntryEarliestPeriod(theWitRun, fullParentName.c_str(), nBom, early);
    witSetBomEntryLatestPeriod(theWitRun, fullParentName.c_str(), nBom, late);
    witSetBomEntryMandEC(theWitRun, fullParentName.c_str(), nBom, mec);
    if (propRtgFlag) {
      witBoolean * prFlag;
      float * prShare;
      witGetBomEntryPropRtg(theWitRun, fullParentName.c_str(), nBom, &prFlag);
      witGetBomEntryRoutingShare(theWitRun, fullParentName.c_str(), nBom, &prShare);
      int ttt;
      for (ttt=0; ttt<nPeriods; ttt++) {
	prFlag[ttt] = WitTRUE;
	prShare[ttt] = propRtgShare;
      }
      witSetBomEntryPropRtg(theWitRun, fullParentName.c_str(), nBom, prFlag);
      witSetBomEntryRoutingShare(theWitRun, fullParentName.c_str(), nBom, prShare);
      witFree(prFlag);
      witFree(prShare);
    }


    //    witFree(offset);


    return 0;
}

#endif






// Normal BOC entry (mandatory)
int
LgFrInitialProblemForSce::sceAddNormalBocStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar & theCal,
    const std::string & parent, 
    const std::string & parentPdf,
    const std::string & child, 
    const std::string & childPdf, 
    float usageRate,
    float usageTime,
    int early,
    int late,
    bool capacityOffsetToZero,
    const LgFrTimeVecFloat & zeroVec,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
    std::string fullParentName(mpHelper.pdfPartname(parent, parentPdf));
    std::string fullChildName( mpHelper.pdfPartname(child,  childPdf));

	
    if (! mpHelper.isPartValid(theWitRun, parent, parentPdf, 
			       fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
	return -1;

    if (! mpHelper.isPartValid(theWitRun, child, childPdf, 
			       fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
	return -1;


    int nBom;
    witGetOperationNBomEntries(theWitRun, fullParentName.c_str(), &nBom);
    witAddBomEntry(theWitRun, fullParentName.c_str(), fullChildName.c_str());
    // witSetBomEntryUsageRate(theWitRun, fullParentName.c_str(), nBom, usageRate);
    float * consRate;
    witGetBomEntryConsRate(theWitRun, fullParentName.c_str(), nBom, &consRate);
    int t;

    int nPeriods = theCal.nPeriods();
    LgFrTimeVecFloat offset(nPeriods, 0.0);
    
    if (! capacityOffsetToZero)   {
#ifdef VARIABLE_PERIODS
      LgFrTimeVecFloat parentCycleTimeDays = mpHelper.getCycleTimeDays(theWitRun, fullParentName);
#endif
      
      for (t=early; t<=late; t++)  {
#ifdef VARIABLE_PERIODS
	//       offset[t] = mpHelper.calculateVariableOffset(usageTime, t, theCal, nPeriods);
	offset[t] = mpHelper.calculateVariableOffset(parentCycleTimeDays[t] - usageTime, t, theCal, nPeriods);
#else
	offset[t] = usageTime;
#endif
      }
    }
    
    
    for (t=early; t<=late; t++)   {
      consRate[t] = usageRate;
      // come here
    }
    witSetBomEntryConsRate(theWitRun, fullParentName.c_str(), nBom, consRate);
    witFree(consRate);
    witSetBomEntryEarliestPeriod(theWitRun, fullParentName.c_str(), nBom, early);
    witSetBomEntryLatestPeriod(theWitRun, fullParentName.c_str(), nBom, late);

    witSetBomEntryOffset(theWitRun, fullParentName.c_str(), nBom, offset.data());


    return 0;
}



// Add aggregation part
int
LgFrInitialProblemForSce::sceAddAggregateBomStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & mfgPart, 
    const std::string & mfgPdf,
    const std::string & aggregatePart, 
    const std::string & aggregatePdf, 
    float usageRate,
    float offset,
    float preference,
    int isObjectInEffectForOptExplosionFlag,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{

  // Add an aggregation operation 
  witBoolean exists;

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  std::string fullMfgPart(mpHelper.pdfPartname(mfgPart, mfgPdf));
  std::string fullAggregatePart(mpHelper.pdfPartname(aggregatePart,  aggregatePdf));
 
  // if the parent part does not already exist, then reject the record!
  if (! mpHelper.isPartValid(theWitRun, mfgPart, mfgPdf,
                             fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
    return -1;

  // if the child part does not already exist, then complain and reject!!!
  if (! mpHelper.isPartValid(theWitRun, aggregatePart, aggregatePdf, 
                             fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
    return -1;


  std::string aggOpName(mpHelper.aggregationOperationName(mfgPart, mfgPdf,
                                                        aggregatePart, aggregatePdf));

  // if the agg operation already exists, then reject it.  No duplicates allowed
  // NOTE: We do allow multiple aggregation operations for a given real part,
  // i.e. it is now ok to multiply aggregate a single part
  witGetOperationExists(theWitRun, aggOpName.c_str(), &exists);
  if (exists) {
    (*theErrFacil)("DuplicateAggRecord",MclArgList() 
		   << fileName 
		   << (int)lineNo 
		   << dataLine);
    return -1;
  }

  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);
  float * offsetVec = new float[nPeriods];
  int t = 0; // Pulled out of the for below by RW2STL
  for (t=0; t<nPeriods; t++)
    offsetVec[t] = offset;
    
  witAddOperation(theWitRun, aggOpName.c_str());

  witAddBopEntry(theWitRun, aggOpName.c_str(), fullMfgPart.c_str());
  // FINISH_ME:  How do we set preference on Aggregation and explosion flag
  LgFrSceBopAppData * appDataPtr = new LgFrSceBopAppData(preference, isObjectInEffectForOptExplosionFlag, 0);
  witSetBopEntryAppData(theWitRun, aggOpName.c_str(), 0, (void *) appDataPtr);

    
  witAddBomEntry(theWitRun, aggOpName.c_str(), fullAggregatePart.c_str());
  witSetBomEntryUsageRate(theWitRun, aggOpName.c_str(), 0, usageRate);
  witSetBomEntryOffset(theWitRun, aggOpName.c_str(), 0, offsetVec);
  delete [] offsetVec;

  // Since this is a new aggregation Operation for the customer loc, increment
  // the nAggregationOps counter in the CustLoc Parts appdata.  (Being careful,
  // of course, to ensure the non-null-ness of the appDataPtr for the part
  LgFrScePartAppData * custPartAppData;
  witGetPartAppData(theWitRun, fullMfgPart.c_str(), (void **) &custPartAppData);
  if (custPartAppData == 0) {
    custPartAppData = new LgFrScePartAppData(0, nPeriods, nPeriods);
    custPartAppData->nAggregationOps(1);
    witSetPartAppData(theWitRun, fullMfgPart.c_str(), (void *) custPartAppData);
  }
  else 
    custPartAppData->nAggregationOps(custPartAppData->nAggregationOps() + 1);      

  

  return 0;
}


// Add BOM entry for a GeoPlannerDemandPart --> to a Real Part
int 
LgFrInitialProblemForSce::sceAddDemandBomStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar & theCal,
    const std::string & geoPlannerDemandPartName,
    const std::string & pdf_PartName,
    int early,
    int late,
    float transitTime,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{

  // Test to see that you are not trying to set up a multiple source demand DURING
  // the same period.  To do this, we look at every BOM entry off of the
  // geoPlannerDemandPartName and map the effectivity ranges onto a vector.
  // Then we check to see if we've overlapping the effectivity with the new
  // Source effectivity range.

  int nBom;
  witGetOperationNBomEntries(theWitRun, geoPlannerDemandPartName.c_str(), &nBom);

  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);

  int t = 0;

  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();

  // if there are no BOM's then there's no conflict
  if (nBom > 0) {
    int * effectRange = new int[nPeriods];
    int t = 0;
    for (t=0; t<nPeriods; t++)
      effectRange[t] = 0;
    
    // first set the effectRange vector for the new source record
    for (t=early; t<=late; t++)
      effectRange[t] = 1;

    // Now check this with the other source boms already in the system
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nBom; j++ ) {

      // we need only consider Normal parts since any special parts
      // hanging off the GPD are not "sources"
      char * child;
      witGetBomEntryConsumedPart(theWitRun, geoPlannerDemandPartName.c_str(), j, &child);
      if (! mpHelper.isPartNormal(theWitRun, child)) {
        witFree(child);
        continue;
      }

      
      int bomEarly;
      int bomLate;
      witGetBomEntryEarliestPeriod(theWitRun, geoPlannerDemandPartName.c_str(), j, &bomEarly);
      witGetBomEntryLatestPeriod(theWitRun, geoPlannerDemandPartName.c_str(), j, &bomLate);
      for (t=bomEarly; t<=bomLate; t++) {
        if (effectRange[t] == 1) {
          // Aha! we've found a demand being sourced to two locations at the
          // same time (you dirty scoundrel).  Bad user. Bad. Bad. Bad.
          std::string geo;
          std::string plannerPart;
          assert (1 == mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun,
                                                                  geoPlannerDemandPartName,
                                                                  plannerPart,
                                                                  geo));
	  (*theErrFacil)("MultipleSourceAttempt",MclArgList() 
			 << geo 
			 << plannerPart 
			 << mpHelper.pdf(pdf_PartName) 
			 << mpHelper.partname(pdf_PartName) 
			 << mpHelper.pdf(child) 
			 << mpHelper.partname(child) 
			 << t 
			 << fileName 
			 << (int)lineNo 
			 << dataLine);
          witFree(child);
          delete [] effectRange;
          return -1;
        }
        else
          effectRange[t] = 1;
      }
      witFree(child);
    }
    delete [] effectRange;
  }



  witAddBomEntry(theWitRun, geoPlannerDemandPartName.c_str(), pdf_PartName.c_str());
  witSetBomEntryEarliestPeriod(theWitRun, geoPlannerDemandPartName.c_str(), nBom, early);
  witSetBomEntryLatestPeriod(theWitRun, geoPlannerDemandPartName.c_str(), nBom, late);

  // the transit time goes on the BOM offset
  float * bomOffset;
  witGetBomEntryOffset(theWitRun, geoPlannerDemandPartName.c_str(), nBom, &bomOffset);

  for (t=early; t<=late; t++)  {
#ifdef VARIABLE_PERIODS
    bomOffset[t] = mpHelper.calculateVariableOffset(transitTime, t, theCal, nPeriods);
#else
    bomOffset[t] = transitTime;
#endif
  }
  witSetBomEntryOffset(theWitRun, geoPlannerDemandPartName.c_str(), nBom, bomOffset);
  witFree(bomOffset);


  // NOTE: TEMPORARILY set the transit time as a BOP Offset also.
  // This makes it easier to add it to feature offsets later.  Remember
  // to shut these off later.
  float * bopOffset;
  witGetBopEntryOffset(theWitRun, geoPlannerDemandPartName.c_str(), 0, &bopOffset);
  for ( t=early; t<=late; t++)
    bopOffset[t] = transitTime;
  witSetBopEntryOffset(theWitRun, geoPlannerDemandPartName.c_str(), 0, bopOffset);
  witFree(bopOffset);
  
  return 0;
}

  // Add BOM entry for a DemandPart --> to a Demand UpperBound Part
int 
LgFrInitialProblemForSce::sceAddDemandUbBomStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & demandPart,
    const std::string & demandUpperBoundPart, 
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo)
{
  witAddBomEntry(theWitRun, demandPart.c_str(), demandUpperBoundPart.c_str());

  return 0;
}


// ============================================================
// ============================================================
// =========(this is sceReslt.C ===============================


//  ---------------------------------------------------------------------
//  Print Heuristic data
//  --------------------------------------------------------------------- 
void 
LgFrInitialProblemForSce::printOverallHeuristicValues(
         WitRun * const theWitRun,   // WIT environment   
         const int nPeriods,         // Number of periods 
         const int nParts,           // Number of parts   
         char ** partList,           // List of part names
         std::string fileName)
{

   std::string partName;

   std::ofstream outFile(fileName.c_str());//, ios::out);

   int *   priority;
   float * shipVol;
   float * demandVol;
   float cumpts = 0.0;
   float cumblog = 0.0;
   int nDemands;
   char ** dList;
   float cumdvol = 0.0;
   float newBlog;
   int i, j, t;

   // Loop once for each part.  
   for ( i=0; i<nParts; i++ ) {

       witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);
       for (j =0; j<nDemands; j++)  {
	   witGetDemandPriority(theWitRun, partList[i], dList[j], &priority);
	   witGetDemandShipVol(theWitRun, partList[i], dList[j], &shipVol);
	   witGetDemandDemandVol(theWitRun, partList[i], dList[j], &demandVol);
	   for (t=0; t<nPeriods; t++) {
	       cumpts += (shipVol[t] * priority[t]);
	       newBlog = (demandVol[t] > shipVol[t]) ? demandVol[t] - shipVol[t] : 0.0f;
	       cumblog += newBlog;
	       cumdvol += demandVol[t];
	   }
	   witFree(dList[j]);
	   witFree(priority);
	   witFree(shipVol);
	   witFree(demandVol);
       }
       witFree(dList);

   }

   outFile << " Cum OBJ*Ship = " << cumpts << "\n";
   if (cumdvol > 0.0001) 
     outFile << " Cum Serv = " << 1.0 - (cumblog/cumdvol) << "\n";
   else 
     outFile << " Cum Serv = "
             << "uncomputable ... There are NO demand vols\n";     

   outFile.close();

}




// One stop WIT messaging control
void 
LgFrInitialProblemForSce::setWitInformationalMessaging(WitRun * const theWitRun,   
                                                       int onOrOff)
{
  this->sceScenario()->setWitInformationalMessaging(theWitRun, onOrOff);
}








#endif


// Create a copy of the *this on the heap and return a pointer to it
// Even thought this returns a LgFrInitialProblemAndParameters*, the pointer
// must point to a LgFrInitialProblemForSce.
LgFrInitialProblemAndParameters*
LgFrInitialProblemForSce::clone ()
     const
{
  return new LgFrInitialProblemForSce(*this);
}

// Make scenario clone copy of object
LgFrInitialProblemAndParameters *
LgFrInitialProblemForSce::clone(
   LgFrScenarioSmartPointer newScenario)
const
{
  LgFrInitialProblemAndParameters* newInst =
      new LgFrInitialProblemForSce(newScenario);
  return (newInst);
}



LgFrInitialProblemForSce::~LgFrInitialProblemForSce()
{
  // Nothing to do, so do nothing but return
  delete heurModelHelper_;
  delete lpModelHelper_;
}

// Assignment operator.
LgFrInitialProblemForSce&
LgFrInitialProblemForSce::operator=(
            const LgFrInitialProblemForSce& rhs)
{
  
  return *new LgFrInitialProblemForSce(rhs.scenario());
}

// Copy constructor
LgFrInitialProblemForSce::LgFrInitialProblemForSce(
            const LgFrInitialProblemForSce& source)
:
   LgFrInitialProblemAndParameters(source.scenario()),
   implementSmartFeatures_(source.implementSmartFeatures_),

   heurModelHelper_(source.heurModelHelper_),
   lpModelHelper_(source.lpModelHelper_),
  groupSharingViaExecPenalties_(source.groupSharingViaExecPenalties_)
{
  // All the work is done in the initialization part
  // except this
}

// Default Constructor
LgFrInitialProblemForSce::LgFrInitialProblemForSce()
:
    LgFrInitialProblemAndParameters(
       LgFrScenarioSmartPointer::nullScenarioSmartPointer),
       implementSmartFeatures_(true),

    heurModelHelper_(0),
    lpModelHelper_(0),
    groupSharingViaExecPenalties_(FALSE)
{
  // All the work is done in the initialization part
}

// the preferred Constructor
LgFrInitialProblemForSce::LgFrInitialProblemForSce(LgFrScenarioSmartPointer scenario)
:
    LgFrInitialProblemAndParameters(scenario),
    implementSmartFeatures_(true),

    heurModelHelper_(0),
    lpModelHelper_(0),
    groupSharingViaExecPenalties_(false)
{
  // All the work is done in the initialization part
}

// Returns a description of the problem that is suitable for use as a
// window heading. This can not be used to set the title; only to query
// it.
std::string
LgFrInitialProblemForSce::title(const LgFrDataInterfaceEssentials* difEssenPtr)
     const
{
  std::string theTitle("Supply Capability Engine: ");
  //  theTitle.append(dataInterfacePtr()->title(difEssenPtr));
  theTitle.append(sceScenario()->dataInterface()->title(difEssenPtr));  
  return theTitle;
}


// get the scenario
LgFrScenarioForSceSmartPointer
LgFrInitialProblemForSce::sceScenario()
     const
{
    if (scenario()->GetScenarioType() != __LGFRSCENARIOFORSCE)  {
        std::cerr << "the object is not a SCE scenario and it is supposed to be"
             << std::endl;
        exit(8);
    }

    return LgFrScenarioForSceSmartPointer(scenario());
}


// Populates the WitRun's data structures 
void
LgFrInitialProblemForSce::feedInto(
				     LgFrLastProblemSolved & lps)
{
  LgFrDataInterface * dif  =
    (LgFrDataInterface*) (sceScenario()->dataInterface());
  LgFrCalendar&     calendar   = sceScenario()->calendar();
  
  assert( dif->dataInterfaceImplementation()->GetDIIType() ==
          __LGFRDATAINTERFACEIMPLEMENTATIONFROMSCEFILES );
  LgFrDataInterfaceImplementationFromSceFiles *difSceF    = 
      (LgFrDataInterfaceImplementationFromSceFiles *) dif->dataInterfaceImplementation();

  

//  std::cout << "\nLgFrInitialProblemForSce::feedinto() ---> " 
//       << this->title() << "\n";
    
  // Get sce file name
  std::string parameterFilename = difSceF->filename();

  // get the witRun pointer from lps
  WitRun * theWitRun = lps.witRun();

  // **********************************
  // Now populate the wit run and sets of parts and demands
  // **********************************

#ifndef ELIMINATE_OLD_MAIN  
  completeTheInitialization(parameterFilename, calendar, theWitRun);
#endif

}


#ifndef ELIMINATE_OLD_MAIN

// ============================================================
// ========= completeTheInitialization() ======================
// ============================================================
void 
LgFrInitialProblemForSce::completeTheInitialization (
    std::string & parameterFileName,
    LgFrCalendar & theCal,
    WitRun * theWitRun)
{
  MclFacility * theErrFacil = this->sceScenario()->getErrFacility();


  if (! doesInputFileExist(parameterFileName))  {
    // Error
    (*theErrFacil)("SevereCannotOpenFile",MclArgList() << "Parameter" << parameterFileName << "");
  }

   // get the global soParms from scenario.
   LgFrSceSetOfParameters & setOfParameters =  this->sceScenario()->setOfParameters();
  
   LgFrSceParameterManager myParamMgr;
   // this shares the scenario's error with the parameter manager
   myParamMgr.setErrFacility(theErrFacil);
   myParamMgr.initializeSetOfParameters(parameterFileName, setOfParameters);

   // initialize the set of illegal characters in the flat file manager
   LgFrSceFlatFileMgr & myFlatFileMgr = this->sceScenario()->flatFileMgr();
   myFlatFileMgr.appendToTheSetOfIllegalCharacters( setOfParameters.getString("pdfSeparator"));
   if (setOfParameters.getBool("useMultiAttributeDemand"))
     myFlatFileMgr.appendToTheSetOfIllegalCharacters( setOfParameters.getString("multiAttributeDemandSeparator"));

   myFlatFileMgr.sceIOLogManager().copySetOfParams(this->sceScenario()->setOfParameters());
   
   // reset the IO log
   myFlatFileMgr.sceIOLogManager().resetInputLog();
   myFlatFileMgr.sceIOLogManager().resetOutputLog();

   // also get give paramMgr a handle for iolog manager
   myParamMgr.setIOLogMgr(&(myFlatFileMgr.sceIOLogManager()));


   // Read the calendarFormat code and set it in the flatFileMgr
   std::string calendarFormat = setOfParameters.getString("calendarFormat");
   myFlatFileMgr.setGlobalCalendarFormat(calendarFormat);

   // set variable engine
   sceScenario()->setEngine(setOfParameters.getString("engine"));

#ifndef SCE_OPT_IMPLODE
   if (setOfParameters.getString("engine") == "optimization") {
     (*theErrFacil)("OptEngineNotSupported");
     // this will exit
   }
#endif


   theCal.nPeriods(setOfParameters.getInt("numPeriods"));
   
   // create a multiPlantHelper Object
   LgFrMultiPlantHelper & mpHelper = sceScenario()->multiPlantHelper();


   // Call the API program to read all the datafiles and populate theWitRun
   buildTheWitModelForSce(
       theWitRun, 
       theCal,
       mpHelper);
}

#endif
