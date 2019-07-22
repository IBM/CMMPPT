
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.

#include <stdlib.h>

#include <mcl/src/mcl.h>


#include <scenario/src/calendar.h>
#include <scenario/src/dif.h>
#include <scenario/src/difPWcmr.h>
//#include <scenario/src/difPWatp.h>
#include <scenario/src/scenDrct.h>
#include <scenario/src/schFCMRP.h>
#include <scenario/src/schdDeSt.h>
#include <scenario/src/timeVecs.h>

#include <scenario/src/part.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/pairComp.h>
#include <sce/src/scePegging.h>



#include <sce/src/ipSce.h>
#include <sce/src/dmApData.h>
#include <sce/src/bopAData.h>
#include <sce/src/appData.h> 
#include <sce/src/exploders.h>
#include <sce/src/scBdSce.h>
#include <sce/src/scenSce.h>
#include <sce/src/sceFssMg.h>
#include <sce/src/hEngMgr.h>
#include <sce/src/oEngMgr.h>
#include <sce/src/aEngMgr.h>
#include <sce/src/sceBop.h>



// forward declarations
class LgFrSetOfParts; 
class LgFrSetOfDemands;

#define SCENARIOSCE_FLT_EPS  0.00001
#define DEMAND_PEGGING
// #define SHADOW_PRICE_DEBUG
// #define CRIT_PEG_DEBUG




void 
LgFrScenarioForSce::analyzeRunAndPrintResults()
{

    int nPeriods;                         //  Number of periods in model  
    int nParts;                           //  Number of parts in model     
    char ** partList;                     //  List of parts in model       
    int i;                                //  Loop index                  
    int nOperations;
    char ** opList;                     //  List of parts in model       


    WitRun * const theWitRun = lastProblemSolved().witRun();
    LgFrCalendar & theCal = calendar();
    LgFrMultiPlantHelper & mpHelper = multiPlantHelper();
    LgFrSceCritList & theCritList = sceCritList();
      
    witGetParts( theWitRun, &nParts, &partList );
    witGetOperations( theWitRun, &nOperations, &opList );
    nPeriods = theCal.nPeriods();

    LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();

    std::string siteCustomization = setOfParameters.getString("siteCustomization");
    bool useMultiAttributeDemand = setOfParameters.getBool("useMultiAttributeDemand");


    // VERY IMPORTANT!!  This step sets App data for Interplant
    // operations in the primary witRun!!  It must be done before any interplant
    // execVols can be accessed (in either mrp or implosion witRuns). The app data
    // is actually store in the priamry witRun.  At this point the appdata's 
    // should ahve been created during the printExplosion results and here we are
    // merely going to set the implosion values (which were initialized to zero)
    if (siteCustomization != "skipTheCrapola") {
      setInterplantOperationsAppDataForImplosion(theWitRun,
						 nPeriods,
						 nOperations,
						 opList);
    }


    // Geo Source Debug File: optional file
    if (setOfParameters.getBool("printGeoSourceDebugFile"))  {    
      std::string geoSourceDebugFileName =
	setOfParameters.getString("geoSourceDebugFileName");
      printGeoSourceDebugFile(theWitRun, nPeriods, nParts, partList,
			      geoSourceDebugFileName, theCal, mpHelper);
    }
    
    
    
    // Formatted Supply Response Report File: optional file
    if (setOfParameters.getBool("printFormattedSupplyReportFile"))  {    
      std::string formattedSupplyReportFileName =
	setOfParameters.getString("formattedSupplyReportFileName");
      printHumanReadableSupplyPlanSoln(theWitRun, nPeriods, nParts, partList,
				       formattedSupplyReportFileName,
				       theCal, mpHelper);
    }

    // Supply Response  File: 
    // 6.1: Supply Response legacy (sce 5 and earlier) prints two commit files, one for TIMP/GPD 
    //      and the oter fir INDP demands.   In SCE 6.1, and specifically with, multiAttributeDemand,
    //      a single supply response file is printed
    if (setOfParameters.getBool("useMultiAttributeDemand"))  {
      int tinpIndpOrBoth = 3;
      
      if (setOfParameters.getBool("printSupplyResponseFile"))  {
	std::string supplyResponseFileName = setOfParameters.getString("supplyResponseFileName");
	printSupplyPlanSoln(theWitRun, nPeriods, nParts, partList, tinpIndpOrBoth,
			    supplyResponseFileName, theCal, mpHelper);
      }
    }
    
    else    {
      int tinpIndpOrBoth = 1;
      if (setOfParameters.getBool("printSupplyResponseFile"))  {
	std::string supplyResponseFileName = setOfParameters.getString("supplyResponseFileName");
	printSupplyPlanSoln(theWitRun, nPeriods, nParts, partList, tinpIndpOrBoth,
			    supplyResponseFileName, theCal, mpHelper);
      }

      tinpIndpOrBoth = 2;      
      // Independent Supply Response  File
      // 6.1 temporary function ... need more flexibility to support legacy
      std::string independentDemandFile = setOfParameters.getString("independentDemandFile");
      if (setOfParameters.getBool("printIndpSupplyResponseFile") && ! independentDemandFile.empty())   {
	std::string indepSupplyResponseFileName =	setOfParameters.getString("indepSupplyResponseFileName");
	printIndepSupplyPlanSoln(theWitRun, nPeriods, nParts, partList,
				 indepSupplyResponseFileName, theCal, mpHelper);
      
      }
    }
      
      
    // Backlog Response File
    if (setOfParameters.getBool("printBacklogResponseFile") && (siteCustomization != "skipTheCrapola")) {  
      std::string backlogResponseFileName = setOfParameters.getString("backlogResponseFileName");
      printBacklogSchedule(theWitRun, nPeriods, nParts, partList,
			   backlogResponseFileName, theCal, mpHelper);
    }

    

    // Critical List
    if (setOfParameters.getBool("printCriticalListFile"))  {    
      std::string criticalListFileName = setOfParameters.getString("criticalListFileName");
      std::string engine = setOfParameters.getString( "engine");
      this->printCriticalList(theWitRun, engine, nPeriods, nParts, partList,
			      criticalListFileName, theCal, mpHelper, theCritList);
    }

    // Pegged Critical List
    if (useMultiAttributeDemand)  {
      // SCE 6.1: for multi-attribute demand, we must combine both pegged critLists into one file
      if (setOfParameters.getBool("printPeggedCritListFile"))  {    
	std::string peggedCriticalListFileName  = setOfParameters.getString("peggedCritListFileName");
	std::string engine = setOfParameters.getString( "engine");
	this->printPeggedCriticalList(theWitRun, engine, peggedCriticalListFileName, theCal, mpHelper);
      }
    }
    // else, for non-multi-attribute, we print the two pegged critLists as separate files (this is SCE 5)
    else {
      if (setOfParameters.getBool("printPeggedCritListFile"))  {    
	std::string peggedCriticalListFileName  = setOfParameters.getString("peggedCritListFileName");
	std::string iPeggedCriticalListFileName =	setOfParameters.getString("iPeggedCritListFileName");
	std::string engine = setOfParameters.getString( "engine");
	this->printPeggedCriticalList(theWitRun, engine, peggedCriticalListFileName, iPeggedCriticalListFileName, theCal, mpHelper);
      }
    }


      // Build Schedule  File
    if (setOfParameters.getBool("printBuildSchedFile") && (siteCustomization != "skipTheCrapola")) {
      std::string buildSchedFileName =
	setOfParameters.getString("buildSchedFileName");
      printBuildSchedule(theWitRun, nPeriods, nParts, partList, buildSchedFileName, theCal, mpHelper);
    }


    // Interplant Order File
    if (setOfParameters.getBool("printInterplantOrdersFile") && (siteCustomization != "skipTheCrapola")) {
      std::string interPlantFile = setOfParameters.getString("interPlantFile");
      if (! interPlantFile.empty())   {
	std::string interplantOrdersFileName = setOfParameters.getString("interplantOrdersFileName");
	bool isThisMrp = false;
	printInterplantVolumesFile(theWitRun, nPeriods, 
				   nOperations, opList,
				   isThisMrp,
				   interplantOrdersFileName,
				   mpHelper);
      }
    }


    // Print Alternate DB File
    bool printAltDbFile = setOfParameters.getBool("printAltDbFile");
    if (printAltDbFile )   {
      std::string altDbFileName = setOfParameters.getString("altDbFileName");
      bool isThisMrp = false;
      printAlternateDbFile(theWitRun, nPeriods,
		     nOperations, opList,
		     isThisMrp,
		     altDbFileName,
		     mpHelper);
    }

    // Print Shadow Price File: optional file (3.11)
    // prints only when asked for AND if engine="optimization"
    bool printShadowPricesFile = setOfParameters.getBool("printShadowPricesFile");
    std::string engine = this->getEngine();
    if ((! engine.compare("optimization")) && printShadowPricesFile) {
      std::string shadowPricesFileName = setOfParameters.getString("shadowPricesFileName");
      printShadowPrices(theWitRun, nPeriods, 
			nParts, partList,
			shadowPricesFileName,
			mpHelper);
    }
    



    // Operation Orders File: mandatory file
    // Prints execVol of operations that are:
    // (1) explicitely defined by the user in the operation Definition File
    // (2) SCE-defined generated from parts defined in the part Definition File
    if (setOfParameters.getBool("printOperationOrdersFile") && (siteCustomization != "skipTheCrapola")) {
      std::string operationOrdersFileName =
	setOfParameters.getString("operationOrdersFileName");
      printOperationOrdersFile(theWitRun, nPeriods,
				    operationOrdersFileName,
				    mpHelper);
    }




    // Capacity Usage Volume File: optional file
    if (setOfParameters.getBool("printCapacityUsageFile"))   {
      bool isItMrp = false;
      printCapUsageVolume(theWitRun,isItMrp); 
    }
    

    // Sub DB File
    // (prints only when there is substitute  input data)
    std::string subBomFile = setOfParameters.getString("subBomFile");    
    if (! subBomFile.empty() && setOfParameters.getBool("printSubDbFile"))   {
      bool parentMustBeUserDefinedOperation = false;
      bool engineMustBeImplode = true;
      std::string subDbFileName = setOfParameters.getString("subDbFileName");
      printSubDbDumpFile(theWitRun, 
			 parentMustBeUserDefinedOperation,
			 engineMustBeImplode,
			 nPeriods, nOperations, opList,
			 subDbFileName,
			 mpHelper);
    }


    // Substitute Usage Volume File
    // (prints only when there is substitute  input data)
    if (setOfParameters.getBool("printSubUsageFile") && ! subBomFile.empty())   {
      bool parentMustBeUserDefinedOperation = false;
      bool engineMustBeImplode = true;
      std::string subUsageFileName = setOfParameters.getString("subUsageFileName");
      printSubstituteUsageVolume(theWitRun, parentMustBeUserDefinedOperation,
				 engineMustBeImplode,
				 nPeriods, nOperations, opList, subUsageFileName,
				 mpHelper);
    }

    // Operation Substitute Usage Volume File: 
    std::string opSubBomFile = setOfParameters.getString("opSubBomFile");    
    if (setOfParameters.getBool("printOpSubUsageFile") && ! opSubBomFile.empty())   {
      bool parentMustBeUserDefinedOperation = true;
      bool engineMustBeImplode = true;
      std::string opSubUsageFileName = setOfParameters.getString("opSubUsageFileName");
      printSubstituteUsageVolume(theWitRun, 
				 parentMustBeUserDefinedOperation,
				 engineMustBeImplode,
				 nPeriods, nOperations, opList,
				 opSubUsageFileName,
				 mpHelper);
    }



    
    // -----------------------------------------
    // ----- Print Feature Attach Volumes ---------
    // -----------------------------------------    
    std::string featureFileOutputFormat =
      setOfParameters.getString("featureFileOutputFormat");

    // see what format the user wants his feature outputs as
    int printDataAsPercentage = 0;
    if (featureFileOutputFormat == "percentage")
      printDataAsPercentage = 1;
    
    
    // Standalone Feature Attach Volume File
    // (prints only when there is optional feature  input data)
    std::string topLevelToOptionFile = setOfParameters.getString("topLevelToOptionFile");
    if (! topLevelToOptionFile.empty()) {
      std::string optionalFeatRatioFileName = setOfParameters.getString("optionalFeatRatioFileName");
      printStandAloneFeatureAnalysisReport(theWitRun, nPeriods, nParts, partList,
					   optionalFeatRatioFileName,
					   theCal, mpHelper, printDataAsPercentage);
    }

    // Selective Set Feature Attach Volume File
    std::string topLevelToBBratioFile = setOfParameters.getString("topLevelToBBratioFile");
    if (! topLevelToBBratioFile.empty()) {
      std::string selFeatBBratioFileName = setOfParameters.getString("selFeatBBratioFileName");

      if (engine == "optimization")
	printFeatureAnalysisReport(theWitRun, nPeriods, nParts, partList,
				   selFeatBBratioFileName,
				   theCal, mpHelper, printDataAsPercentage);
      else
	printFeatureAnalysisReportViaProportionalRoute(theWitRun, nPeriods, nOperations, opList,
						       selFeatBBratioFileName,
						       theCal, mpHelper, printDataAsPercentage);
    }


    // Part Database Dump File
    if (setOfParameters.getBool("printPartDBFile"))  {
      std::string partDBFileName = setOfParameters.getString("partDBFileName");
      printPartDataBaseDumpFile(theWitRun, mpHelper, nPeriods, nParts, 
				partList, partDBFileName);
    }

    // Capacity Database Dump File: optional file    
    if (setOfParameters.getBool("printCapacityDBFile"))  {
      std::string capacityDBFileName = setOfParameters.getString("capacityDBFileName");
      printCapacityDataBaseDumpFile(theWitRun, mpHelper, nPeriods, nParts, 
				    partList, capacityDBFileName);
    }


    // Operation Database Dump File: optional file
    if (setOfParameters.getBool("printOperationDBFile"))  {
      std::string operationDBFileName =	setOfParameters.getString("operationDBFileName");
      printOperationDataBaseDumpFile(theWitRun, mpHelper, nPeriods, 
				     operationDBFileName);
    }

    // Operation ExecVol  Dump File: optional file
    if (setOfParameters.getBool("printOperationExecFile"))  {
      std::string operationExecFileName = setOfParameters.getString("operationExecFileName");
      printOperationExecDumpFile(theWitRun, mpHelper, nPeriods, operationExecFileName);
    }

    
    // Debug Dump File (internal SCE parts)
    if (setOfParameters.getBool("printDebugFile"))  {
      std::string debugFileName = setOfParameters.getString("debugFileName");
      printDummyPartDataBaseDumpFile(theWitRun, mpHelper, nPeriods, nParts, 
				     partList, debugFileName);
    }
    
    // Problem Statistics Summary File
    if (setOfParameters.getBool("printProblemStatsFile"))  {
      std::string problemStatisticsFileName = setOfParameters.getString("problemStatisticsFileName");
      printOverallProblemStatistics(theWitRun, mpHelper, nPeriods, nParts, partList,
				    problemStatisticsFileName);
    }


    // Objective Summary Report
    if (setOfParameters.getBool("printObjectiveSummaryFile"))  {
      std::string objectiveSummaryFileName = setOfParameters.getString("objectiveSummaryFileName");
      printOverallOptimizationValues(theWitRun, nPeriods, nParts, partList, engine,
				     objectiveSummaryFileName);
    }
    
    // Cleanup memory
    for ( i=0; i<nParts; i++) 
      witFree(partList[i]);
    witFree( partList );
    
    int o = 0; // Pulled out of the for below by RW2STL
    for (o=0; o<nOperations; o++) 
      witFree(opList[o]);
    witFree( opList );

}






// SCE 6.1
int
LgFrScenarioForSce::printAPegFile(WitRun * const theWitRun, 
				  LgFrMultiPlantHelper & mpHelper,
				  LgFrCalendar & theCal,
				  int pegType,
				  int tinpIndpOrBoth,
				  std::string & pegFileName,
				  int nParts,
				  char ** partList,
				  int nPeriods)
{

  std::ofstream dPIPFile;
  dPIPFile.open(pegFileName.c_str(), std::ofstream::out);
  if (dPIPFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() << pegFileName << pegFileName);
  }

  int goodRec = 0;
  int printIfZero = 0;
  
  char * fullWitDemandedPartName;
  char * demandName;
  int isDemandedPartGPD;

  std::string pn1,customerPdf,pn2,supplierPdf;
  
  // Loop over parts to get demands
  int i = 0;
  for(i=0; i<nParts; i++){
    fullWitDemandedPartName = partList[i];
    std::string plannerPart, geo;
    if (mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun, fullWitDemandedPartName, plannerPart, geo)) 
      isDemandedPartGPD = TRUE;
    else 
      isDemandedPartGPD = FALSE;


    // If you only want INDP type demands, and this one is a GPD, then skip
    if (isDemandedPartGPD == TRUE && tinpIndpOrBoth == 2 )  {
      continue;
    }

    // If you only want GPD type demands, and this one is NOT a GPD, then skip
    if (isDemandedPartGPD == FALSE && tinpIndpOrBoth == 1)  {
      continue;
    }

    
    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun, fullWitDemandedPartName, &nDemands, &dList);
    
    // For GPD demands, can assert there's at most one demand. (get rid of this at some point)
    if (isDemandedPartGPD)   
      assert(nDemands < 2);
    
    int j = 0; 
    for(j=0; j<nDemands; j++){
      demandName = dList[j];
      
      // get the demandAppData, if none exists, then skip it
      LgFrSceDemandAppData * demandAppDataPtr = 0 ;
      witGetDemandAppData( theWitRun, fullWitDemandedPartName, demandName, (void **) &demandAppDataPtr );
      if ( demandAppDataPtr == 0 ) {
	witFree (dList[j]);
	continue;
      }
      
      // Get const references to the correct PIP  schedules stored in app data
      const LgFrSortingPartScheduleFloat & PIPSched = demandAppDataPtr->PIPSched(pegType);
      
      // Loop over each consumed part
      int nConsumedParts = PIPSched.size();
      int cp = 0; 
      for ( cp=0; cp<nConsumedParts; cp++ ) {
	// get part
	const LgFrPart & consumedPart = PIPSched.part(cp);
	
	if (pegType != PEG_TYPE_INTERPLANT)  {
	  // for all other pegTypes, we only report for normal pegged-to parts
	  if (! mpHelper.isPartNormal(theWitRun, consumedPart.name())) {
	    continue;
	  }
	}
	else {
	  // for pegType=interplant, report only for interplant operations

	  if (! mpHelper.isOperationSpecialInterplant(theWitRun, consumedPart.name(), pn1, customerPdf, pn2, supplierPdf)) {
	    continue;
	  }
	}
	
	LgFrTimeVecFloat PIPTV = PIPSched[cp].timeVecFloat();
	
	// for non-interplant  peg types do ...
	if (pegType != PEG_TYPE_INTERPLANT) {
	  if (isDemandedPartGPD)   {
	    flatFileMgr_.writeGpdPegRecord(theCal, mpHelper, 
					   plannerPart, geo, mpHelper.partname(consumedPart.name()), mpHelper.pdf(consumedPart.name()), 
					   PIPTV.data(), dPIPFile, nPeriods, printIfZero);
	    goodRec++;
	  }
	  else   {
	    flatFileMgr_.writeIndpPegRecord(theCal, mpHelper, 
					    mpHelper.partname(fullWitDemandedPartName), mpHelper.pdf(fullWitDemandedPartName), demandName,
					    mpHelper.partname(consumedPart.name()), mpHelper.pdf(consumedPart.name()), 
					    PIPTV.data(), dPIPFile, nPeriods, printIfZero);
	    goodRec++;
	  }
	}
	// for interplant  peg types do ...
	else {
	  if (isDemandedPartGPD)   {
	    flatFileMgr_.writeGpdInterplantPegRecord(theCal, mpHelper, 
						     plannerPart, geo, 
						     pn1, customerPdf, pn2, supplierPdf,
						     PIPTV.data(), dPIPFile, nPeriods, printIfZero);
	    goodRec++;
	  }
	  else   {
	    flatFileMgr_.writeIndpInterplantPegRecord(theCal, mpHelper, 
						      mpHelper.partname(fullWitDemandedPartName), mpHelper.pdf(fullWitDemandedPartName), demandName,
						      pn1, customerPdf, pn2, supplierPdf,
						      PIPTV.data(), dPIPFile, nPeriods, printIfZero);
	    goodRec++;
	  }
	}	
      }
      
      witFree(dList[j]);
    }
    witFree(dList);
  }
  
  dPIPFile.close();
  return goodRec;
}



// SCE 6 version
//   - SCE 6 combines the INDP and TINP pegging files into a single one
//  -     - this is possible because of MultiAttributeDemand
//     SCE 6 allows for dated output records
//     this function must support SCE 5 back compatibility
void
LgFrScenarioForSce::printPIPData(WitRun * const theWitRun, int isItMrp)
{


  LgFrMultiPlantHelper & mpHelper = multiPlantHelper();
  LgFrCalendar & theCal = calendar();
  LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();


  int nonZeroOnly = 0;
  int alwaysPrint = 1;
  

  bool useMultiAttributeDemand = setOfParameters.getBool("useMultiAttributeDemand");
  int tinpIndpOrBoth;

  
  bool printSupplyPegFile = setOfParameters.getBool("printSupplyPegFile");
  std::string supplyPegFileName = setOfParameters.getString("supplyPegFileName");
  bool printIndpSupplyPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printIndpSupplyPegFile");
  std::string indpSupplyPegFileName = setOfParameters.getString("indpSupplyPegFileName");


  bool printDemandConsPegFile = setOfParameters.getBool("printDemandConsPegFile");
  std::string demandConsPegFileName = setOfParameters.getString("demandConsPegFileName");
  bool printIndpDemandConsPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printIndpDemandConsPegFile");
  std::string indpDemandConsPegFileName = setOfParameters.getString("indpDemandConsPegFileName");
  
  bool printInterplantPegFile = setOfParameters.getBool("printInterplantPegFile");
  std::string interplantPegFileName = setOfParameters.getString("interplantPegFileName");
  bool printIndpInterplantPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printIndpInterplantPegFile");
  std::string indpInterplantPegFileName = setOfParameters.getString("indpInterplantPegFileName");

  //  bool printExecVolPegFile = setOfParameters.getBool("printExecVolPegFile");
  //  std::string execVolPegFileName = setOfParameters.getString("execVolPegFileName");
  //  bool printIndpExecVolPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printIndpExecVolPegFile");
  //  std::string indpExecVolPegFileName = setOfParameters.getString("indpExecVolPegFileName");

  bool printProdVolPegFile = setOfParameters.getBool("printProdVolPegFile");
  std::string prodVolPegFileName = setOfParameters.getString("prodVolPegFileName");
  bool printIndpProdVolPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printIndpProdVolPegFile");
  std::string indpProdVolPegFileName = setOfParameters.getString("indpProdVolPegFileName");

  bool printPfProdVolPegFile = setOfParameters.getBool("printPfProdVolPegFile");
  std::string pfProdVolPegFileName = setOfParameters.getString("pfProdVolPegFileName");
  bool printIndpPfProdVolPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printIndpPfProdVolPegFile");
  std::string indpPfProdVolPegFileName = setOfParameters.getString("indpPfProdVolPegFileName");

  bool printSideVolPegFile = setOfParameters.getBool("printSideVolPegFile");
  std::string sideVolPegFileName = setOfParameters.getString("sideVolPegFileName");
  bool printIndpSideVolPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printIndpSideVolPegFile");
  std::string indpSideVolPegFileName = setOfParameters.getString("indpSideVolPegFileName");


  if (isItMrp) {
    printSupplyPegFile = setOfParameters.getBool("printMrpSupplyPegFile");
    supplyPegFileName = setOfParameters.getString("mrpSupplyPegFileName");
    printIndpSupplyPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printMrpIndpSupplyPegFile");
    indpSupplyPegFileName = setOfParameters.getString("mrpIndpSupplyPegFileName");

    printDemandConsPegFile = setOfParameters.getBool("printMrpDemandConsPegFile");
    demandConsPegFileName = setOfParameters.getString("mrpDemandConsPegFileName");
    printIndpDemandConsPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printMrpIndpDemandConsPegFile");
    indpDemandConsPegFileName = setOfParameters.getString("mrpIndpDemandConsPegFileName");

    printInterplantPegFile = setOfParameters.getBool("printMrpInterplantPegFile");
    interplantPegFileName = setOfParameters.getString("mrpInterplantPegFileName");    
    printIndpInterplantPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printMrpIndpInterplantPegFile");
    indpInterplantPegFileName = setOfParameters.getString("mrpIndpInterplantPegFileName");


    //    printExecVolPegFile = setOfParameters.getBool("printMrpExecVolPegFile");
    //    execVolPegFileName = setOfParameters.getString("mrpExecVolPegFileName");    
    //    printIndpExecVolPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printMrpIndpExecVolPegFile");
    //    indpExecVolPegFileName = setOfParameters.getString("mrpIndpExecVolPegFileName");


    printProdVolPegFile = setOfParameters.getBool("printMrpProdVolPegFile");
    prodVolPegFileName = setOfParameters.getString("mrpProdVolPegFileName");
    printIndpProdVolPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printMrpIndpProdVolPegFile");
    indpProdVolPegFileName = setOfParameters.getString("mrpIndpProdVolPegFileName");


    printPfProdVolPegFile = setOfParameters.getBool("printMrpPfProdVolPegFile");
    pfProdVolPegFileName = setOfParameters.getString("mrpPfProdVolPegFileName");
    printIndpPfProdVolPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printMrpIndpPfProdVolPegFile");
    indpPfProdVolPegFileName = setOfParameters.getString("mrpIndpPfProdVolPegFileName");


    printSideVolPegFile = setOfParameters.getBool("printMrpSideVolPegFile");
    sideVolPegFileName = setOfParameters.getString("mrpSideVolPegFileName");
    printIndpSideVolPegFile = (1 - useMultiAttributeDemand) * setOfParameters.getBool("printMrpIndpSideVolPegFile");
    indpSideVolPegFileName = setOfParameters.getString("mrpIndpSideVolPegFileName");

  }

  std::cout << "Writing Pegging information..." << std::endl;    

  int goodRec = 0;  
  int nPeriods;  
  witGetNPeriods(theWitRun, &nPeriods);
  int nParts;
  char  **partList;
  witGetParts(theWitRun, &nParts, &partList);

  // if useMultiAttributeDemand is true then we print BOTH tinp and indp demands in the same file, 
  if (useMultiAttributeDemand)    {
    tinpIndpOrBoth = 3;
    if (printSupplyPegFile) 
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_SUPPLY, tinpIndpOrBoth, supplyPegFileName, nParts, partList, nPeriods);
    if (printDemandConsPegFile)  
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_CONSVOL, tinpIndpOrBoth, demandConsPegFileName, nParts, partList, nPeriods);
    if (printPfProdVolPegFile) 
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_PRODVOL, tinpIndpOrBoth, pfProdVolPegFileName, nParts, partList, nPeriods);
    if (printSideVolPegFile) 
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_SIDEVOL, tinpIndpOrBoth, sideVolPegFileName, nParts, partList, nPeriods);
    if (printProdVolPegFile)  
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_EXECVOL, tinpIndpOrBoth, prodVolPegFileName, nParts, partList, nPeriods);
    if (printInterplantPegFile)  
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_INTERPLANT, tinpIndpOrBoth, interplantPegFileName, nParts, partList, nPeriods);
  }

  // otherwise, we need to print two files for each Peg attribute, one for TINP, the other for INDP
  else {
    // first write the TINP files
    tinpIndpOrBoth = 1;
  
    if (printSupplyPegFile) 
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_SUPPLY, tinpIndpOrBoth, supplyPegFileName, nParts, partList, nPeriods);
    if (printDemandConsPegFile)  
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_CONSVOL, tinpIndpOrBoth, demandConsPegFileName, nParts, partList, nPeriods);
    if (printPfProdVolPegFile) 
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_PRODVOL, tinpIndpOrBoth, pfProdVolPegFileName, nParts, partList, nPeriods);
    if (printSideVolPegFile) 
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_SIDEVOL, tinpIndpOrBoth, sideVolPegFileName, nParts, partList, nPeriods);
    if (printProdVolPegFile)  
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_EXECVOL, tinpIndpOrBoth, prodVolPegFileName, nParts, partList, nPeriods);
    if (printInterplantPegFile)  
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_INTERPLANT, tinpIndpOrBoth, interplantPegFileName, nParts, partList, nPeriods);
    
    // second write the INDP files
    tinpIndpOrBoth = 2;
    if (printIndpSupplyPegFile) 
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_SUPPLY, tinpIndpOrBoth, indpSupplyPegFileName, nParts, partList, nPeriods);
    if (printIndpDemandConsPegFile)  
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_CONSVOL, tinpIndpOrBoth, indpDemandConsPegFileName, nParts, partList, nPeriods);
    if (printIndpPfProdVolPegFile) 
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_PRODVOL, tinpIndpOrBoth, indpPfProdVolPegFileName, nParts, partList, nPeriods);
    if (printIndpSideVolPegFile) 
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_SIDEVOL, tinpIndpOrBoth, indpSideVolPegFileName, nParts, partList, nPeriods);
    if (printIndpProdVolPegFile)  
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_EXECVOL, tinpIndpOrBoth, indpProdVolPegFileName, nParts, partList, nPeriods);
    if (printIndpInterplantPegFile)  
      goodRec += printAPegFile(theWitRun, mpHelper, theCal, PEG_TYPE_INTERPLANT, tinpIndpOrBoth, indpInterplantPegFileName, nParts, partList, nPeriods);
  }
  
  int i = 0; 
  for(i=0; i<nParts; i++) {
    witFree(partList[i]);
  }
  witFree(partList);
}    



// ---------------------------------------------------------------------------
LgFrSceBaseExploder *
LgFrScenarioForSce::createAppropriateSceExploder(
						 LgFrScenarioForSceSmartPointer scenSceSP)
{
  WitRun * const theWitRun        = lastProblemSolved().witRun();
  
  // FINISH_ME: implement new parameter scheme and then modify this 
  std::string engine = setOfParameters().getString("engine");
  bool useFullExplodeUtility = setOfParameters().getBool("useFullExplodeUtility");
  bool fssFlag = setOfParameters().getBool("fssFlag");
  bool useSmartExplodeUtility = setOfParameters().getBool("useSmartExplodeUtility");
  bool usePrioritizedExplodeUtility = setOfParameters().getBool("usePrioritizedExplodeUtility");
  
  LgFrSceBaseExploder             * theConcreteExploder;
  LgFrSceFullExploderDecorator    * theFullDecorator;
  LgFrSceSmartExploderDecorator   * theSmartDecorator;
  LgFrSceFssableExploderDecorator * theFssDecorator;
  LgFrSceBaseExploder             * theCurrentExploder;  
   

  // First create the concrete Exploder
  // Its going to be a 
  if ( (useFullExplodeUtility)
       || (useSmartExplodeUtility)
       || (usePrioritizedExplodeUtility)
       || (fssFlag) ) {
    theConcreteExploder = new LgFrSceExploder(lastProblemSolved().witRun(),
					      &(multiPlantHelper()),
					      &(setOfParameters()));
    theCurrentExploder = theConcreteExploder;
  }
  else {
    theConcreteExploder = new LgFrSceBaseExploder(lastProblemSolved().witRun(),
						  &(multiPlantHelper()),
						  &(setOfParameters()));
    return theConcreteExploder;
  }
  
  
  // now see if you want a SMART decorator
  if (useSmartExplodeUtility || usePrioritizedExplodeUtility) {

    theSmartDecorator = new LgFrSceSmartExploderDecorator(theCurrentExploder,
							  scenSceSP,
							  &(multiPlantHelper()));
    theCurrentExploder = theSmartDecorator;
  }
  
  // now see if you want a FULL decorator
  if (useFullExplodeUtility) {
    if (! usePrioritizedExplodeUtility) {
      theFullDecorator = new LgFrSceFullExploderDecorator(theCurrentExploder);
      theCurrentExploder = theFullDecorator;
    }
  }
  // now see if you want a FSS decorator
  if (fssFlag) {
    theFssDecorator = new LgFrSceFssableExploderDecorator(theCurrentExploder);
    theCurrentExploder = theFssDecorator;
  }
  
  return theCurrentExploder;
  
}

// ---------------------------------------------------------------------------
void 
LgFrScenarioForSce::setHeuristicSearchIncrement(WitRun * const theWitRun,   
						std::string greedySearchIncrementMethod, 
						float anIncValue) 
{
  int nParts;
  int nDemands;
  char ** partList;
  char ** demandList;
  int nPeriods;
  
  float searchInc;
  
  witGetNPeriods(theWitRun, &nPeriods);
  witGetParts(theWitRun, &nParts, &partList);
  witBoolean isItAbosluteInc =  (sce::compareStringsCaseIns(greedySearchIncrementMethod, "autoScale")) ? WitTRUE : WitFALSE;
  
  //  std::cout << "HeuristicSearchIncrement: isItAbosluteInc=" << isItAbosluteInc << std::endl;

  int i = 0; 
  for (i=0; i<nParts; i++ ) {
    
    // get demands on the part. 
    witGetPartDemands(theWitRun, partList[i], &nDemands, &demandList);
    
    int j = 0; 
    for (j=0; j<nDemands; j++) {
      if (isItAbosluteInc) {
	witSetDemandSearchInc(theWitRun, partList[i], demandList[j], anIncValue);
      }
      else {
	float * demandVol;
	witGetDemandDemandVol(theWitRun, partList[i], demandList[j], &demandVol);
	int t = 0; 
	float	 maxVol=0.0;
	float minVol= 9999999.9;
	for (t=0; t<nPeriods; t++) {
	  maxVol = (demandVol[t] > maxVol) ? demandVol[t] : maxVol;
	  if (demandVol[t] > 0) {
	    minVol = (demandVol[t] < minVol) ? demandVol[t] :  maxVol;
	  }
	}
	
	witFree(demandVol);
	
	if (maxVol > 0) {
	  searchInc = (maxVol / anIncValue > 0.001) ? maxVol/anIncValue : 0.001;
	  witSetDemandSearchInc(theWitRun, partList[i], demandList[j], searchInc);
	  std::cout << "HeuristicSearchIncrement: searchInc=" << searchInc << std::endl;
	}
      }
      witFree(demandList[j]);

    }
    witFree(demandList);
    witFree(partList[i]);
    
  }
  witFree(partList);
    
}

// ---------------------------------------------------------------------------
//  -------------------
//  Print Total Demand 
//  -------------------
void 
LgFrScenarioForSce::printTotalDemandFile(
	 WitRun * const theWitRun,   // WIT environment
	 LgFrSceBaseExploder * exploder,
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName,
	 LgFrMultiPlantHelper & mpHelper)
{

  LgFrCalendar theCal = this->calendar();

   std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
   if (outFile.fail()) {
     (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
			<< "Total Demand" 
			<< fileName);
   }

   bool useDatedDemands = setOfParameters().getBool("useDatedDemands");
   int printIfZero;
   if (useDatedDemands)
     printIfZero = 0;
   else 
     printIfZero = 1;

   long numRecords = 0;  // keep track of number of records
   int nDemands;
   char ** dList;

#ifdef ENABLE_NEGATIVE_DEMANDS
   LgFrTimeVecFloat negDemVol;
#endif

   // Loop once for each part.  
   int i = 0; // Pulled out of the for below by RW2STL
   for (i=0; i<nParts; i++ ) {
     // Compute :=======> mrpConsVol
     float * mrpConsVol;
     if ( mpHelper.isPartNormal(theWitRun, partList[i])) {
       exploder->mrpConsVol(partList[i], &mrpConsVol);

       // Now add in direct demand vol
       // get demands on the part.  These are needed for direct demands
       witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);

       int j = 0; // Pulled out of the for below by RW2STL
       for (j=0; j<nDemands; j++) {
	 float * indepDemandVol;
	 witGetDemandDemandVol(theWitRun, partList[i], dList[j], &indepDemandVol);
	 int tt = 0; // Pulled out of the for below by RW2STL
	 for (tt=0; tt<nPeriods; tt++) {
	   mrpConsVol[tt] += indepDemandVol[tt];
	 }
	 witFree(indepDemandVol);
	 witFree(dList[j]);
       }
       witFree(dList);

#ifdef ENABLE_NEGATIVE_DEMANDS
       // Compute :=======> negDemVol
       int doesPartHaveNegativeDemands = mpHelper.doesPartHaveNegativeDemands(theWitRun, partList[i]);
       if (doesPartHaveNegativeDemands) {
	 negDemVol = mpHelper.negDemVol(theWitRun, partList[i]);

	 LgFrTimeVecFloat witMrpConsVol((size_t) nPeriods, mrpConsVol);
	 LgFrTimeVecFloat negDemMrpConsVol((size_t) nPeriods, 0.0);

	 float rolledNegDemVol = 0.0;
	 int tt = 0; // Pulled out of the for below by RW2STL
	 for (tt=0; tt<nPeriods; tt++) {
	   rolledNegDemVol += negDemVol[tt];
	   if (mrpConsVol[tt] > rolledNegDemVol) {
	     mrpConsVol[tt] -= rolledNegDemVol;
	     negDemMrpConsVol[tt] += rolledNegDemVol;
	     rolledNegDemVol = 0.0;
	   }
	   else {
	     // Note: if mrpConsVol < 0 then there are negative usage rates
	     // these can realy screw up our calculation
	     if (mrpConsVol[tt] > 0) {
	       rolledNegDemVol -= mrpConsVol[tt];
	       negDemMrpConsVol[tt] += mrpConsVol[tt];
	       mrpConsVol[tt] = 0.0;
	     }
	   }
	 }
       }
#endif


       std::string leadingString = "\"" 
	 + mpHelper.partname(partList[i]) + "\",\"" 
	 + mpHelper.pdf(partList[i]) + "\"";
       flatFileMgr_.writeFloatVecToStream(theCal, outFile, nPeriods, printIfZero, leadingString, mrpConsVol); 


       witFree(mrpConsVol);
       numRecords++;
     }
   }
   outFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
   
   
}


// return a exploder object
LgFrSceBaseExploder *
LgFrScenarioForSce::exploder()
{
  if (exploderPtr_ == 0) {
    assert(exploderPtr_ != 0);
  }
  return exploderPtr_;    
}

// ---------------------------------------------------------------------------
void   
LgFrScenarioForSce::runExplosion( LgFrScenarioForSceSmartPointer scenSceSP )
{

  LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();
  
  //this is the first time an exploder is created!!
  if (exploderPtr_ == 0) {
    exploderPtr_ = this->createAppropriateSceExploder(scenSceSP);
  }
  else {
    std::cerr << "trying to access an exploder for first time and it is non-null\n "
	      << std::endl;
    exit (1);
  }


  LgFrSceBaseExploder * myExploder = this->exploder();

  WitRun * const theExplodeWitRun        = myExploder->localWitRun();  


  // Dump the explode version of the echo file
  if (setOfParameters.getBool("printExplodeEchoFile"))  {
    std::cout << "Writing the Explode echo file  ..." << std::endl;                
    std::string explodeEchoFileName = setOfParameters.getString("explodeEchoFileName");
    setWitInformationalMessaging(theExplodeWitRun, SCE_WIT_MESSAGING_ON);
    witDisplayData( theExplodeWitRun, explodeEchoFileName.c_str());
    if (! setOfParameters.getBool("printWitExplodeMsgFile"))   {
      setWitInformationalMessaging(theExplodeWitRun, SCE_WIT_MESSAGING_OFF);          
    }
  }

  // Dump the explode version of the wit.data file
  if (setOfParameters.getBool("printExplodeWitDataFile"))  {
    std::cout << "Writing the (Explode) wit data file ..." << std::endl;                      
    std::string explodeWitDataFileName = setOfParameters.getString("explodeWitDataFileName");
    setWitInformationalMessaging(theExplodeWitRun, SCE_WIT_MESSAGING_ON);
    witWriteData(theExplodeWitRun, explodeWitDataFileName.c_str());
    if (! setOfParameters.getBool("printWitExplodeMsgFile"))   {
      setWitInformationalMessaging(theExplodeWitRun, SCE_WIT_MESSAGING_OFF);          
    }
  }




  std::cout << "Executing explosion engine using (" << myExploder->title() << ") scheme  ..." << std::endl;  


  // run the explosion engine !!
  myExploder->explode();

  bool doPegging = setOfParameters.getBool("doPegging");

  // MRP TESTING ... print the Critical List 

  // need a flag for whtether it's mrp or implosion PIP
  if (doPegging) {
    printPIPData(theExplodeWitRun, 1);
  }
    
}
// ---------------------------------------------------------------------------
void   
LgFrScenarioForSce::printExplosionResults()
{
  int      nPeriods;
  int      nParts;
  char  ** partList;
  int      nOperations;
  char  ** opList;


  LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();
  LgFrMultiPlantHelper & mpHelper = multiPlantHelper();
  // WitRun * const theWitRun        = this->exploder()->localWitRun();
  WitRun * const theWitRun        = lastProblemSolved().witRun();
  LgFrSceBaseExploder * myExploder = this->exploder();
  
  std::cout << "Writing explosion output results  ..." << std::endl;  

  witGetNPeriods(theWitRun, &nPeriods);
  witGetParts(theWitRun, &nParts, &partList);
  witGetOperations(theWitRun, &nOperations, &opList);

  // Adjust Interplant execVols for Jumper ARCS
  // this is done via operation App data.

  // VERY IMPORTANT!!  This step initializes App data for Interplant
  // operations in the primary witRun!!  It must be done before any interplant
  // execVols can be accessed (in either mrp or implosion witRuns). The app data
  // is actually store in the priamry witRun

  // NOTE: to get an interplant (mrp)execVol, use scenario's helper function
  // interplantExecVol(opName, execVol &)
  // interplantMrpExecVol(opName, mrpExecVol &)
  initializeInterplantOperationsAppData(theWitRun,
					nPeriods,
					nOperations,
					opList);


  bool printTotalDemandReport =
    setOfParameters.getBool("printTotalDemandReport");

  if (printTotalDemandReport) {
    std::string totalDemandReportFileName = setOfParameters.getString("totalDemandReportFileName");
   printTotalDemandFile(theWitRun,  myExploder,
			nPeriods, nParts, partList,
			totalDemandReportFileName,
			mpHelper);
  }



  // Operations Demand File: optional file (new 2.31)
  // prints only when asked for 
  // prints BOTH userDefined Operations and Default Operations

  bool printOperationDemandFile = setOfParameters.getBool("printOperationDemandFile");
  if (printOperationDemandFile )   {
    std::string operationDemandFileName = setOfParameters.getString("operationDemandFileName");
    printOperationDemandVolFile(theWitRun, 
			     nPeriods, 
			     operationDemandFileName,
			     mpHelper);
  }


  

  // Interplant Demand File: optional file (new 2.00)
  // prints only when asked for and there are interplant records

  bool printInterplantDemandFile = setOfParameters.getBool("printInterplantDemandFile");
  std::string interPlantFile = setOfParameters.getString("interPlantFile");
  if ((! interPlantFile.empty()) && printInterplantDemandFile )   {
    std::string interplantDemandFileName =
      setOfParameters.getString("interplantDemandFileName");
    bool isThisMrp = true;
    printInterplantVolumesFile(theWitRun, nPeriods, 
			       nOperations, opList,
			       isThisMrp,
			       interplantDemandFileName,
			       mpHelper);
  }



  // Capacity Usage Volume File: optional file
  // (prints only when there is substitute  input data)
  bool printMrpCapacityUsageFile = setOfParameters.getBool("printMrpCapacityUsageFile");    
  if (printMrpCapacityUsageFile)   {
    bool isItMrp = true;
    printCapUsageVolume(myExploder->localWitRun(), isItMrp); 
  }
  

  
  // Mrp Substitute Usage Volume File
  // (prints only when (1) usingSmartExplode and (2) there is substitute  input data)
  bool printMrpSubUsageFile = setOfParameters.getBool("printMrpSubUsageFile");    
  bool useSmartExplodeUtility = setOfParameters.getBool("useSmartExplodeUtility");
  std::string subBomFile = setOfParameters.getString("subBomFile");    
  if (printMrpSubUsageFile && useSmartExplodeUtility && (! subBomFile.empty()))   {
    std::string mrpSubUsageFileName = setOfParameters.getString("mrpSubUsageFileName");
    bool parentMustBeUserDefinedOperation = false;
    bool engineMustBeImplode = false;
    printSubstituteUsageVolume(theWitRun, 
			       parentMustBeUserDefinedOperation,
			       engineMustBeImplode,
			       nPeriods, nOperations, opList,
			       mrpSubUsageFileName, mpHelper);
  }



  // Mrp Operation Substitute Usage Volume File
  // (prints only when (1) usingSmartExplode and (2) there is operation substitute  input data)
 
  bool printMrpOpSubUsageFile = setOfParameters.getBool("printMrpOpSubUsageFile");    
  std::string opSubBomFile = setOfParameters.getString("opSubBomFile");    
  if (printMrpOpSubUsageFile && useSmartExplodeUtility && (! opSubBomFile.empty()))   {
    std::string mrpOpSubUsageFileName = setOfParameters.getString("mrpOpSubUsageFileName");
    bool parentMustBeUserDefinedOperation = true;
    bool engineMustBeImplode = false;
    printSubstituteUsageVolume(theWitRun, 
			       parentMustBeUserDefinedOperation,
			       engineMustBeImplode,
			       nPeriods, nOperations, opList,
			       mrpOpSubUsageFileName, mpHelper);
  }


  // Print the MRP Supply Orders File
  if (setOfParameters.getBool("printMrpSupplyOrdersFile"))  {
    std::string mrpSupplyOrdersFileName = setOfParameters.getString("mrpSupplyOrdersFileName");
    printMrpSupplyOrdersFile(mpHelper, nPeriods, mrpSupplyOrdersFileName);
  }
  
  // Print the MRP Demand Orders File
  if (setOfParameters.getBool("printMrpDemandOrdersFile"))  {
    std::string mrpDemandOrdersFileName = setOfParameters.getString("mrpDemandOrdersFileName");
    printMrpDemandOrdersFile(mpHelper, nPeriods, mrpDemandOrdersFileName);
  }

  // MRP Critical List   EXPERIMENTAL
  if (setOfParameters.getBool("printMrpCriticalListFile"))  {    
    std::string criticalListFileName = "tomout.mrpCritList";
    std::string engine = setOfParameters.getString( "engine");
    LgFrCalendar theCal = this->calendar();
    LgFrSceCritList & theCritList = sceCritList();
    this->printDebugCriticalList(theWitRun, engine, nPeriods, nParts, partList,
				 criticalListFileName, theCal, mpHelper, theCritList);
  }
  
  

  // Note: if we are not doing an implosion, then we must still write the
  // Dump files.
  bool doImplode = setOfParameters.getBool("doImplode");
  if (! doImplode) {
    // Part Database Dump File:  optional file
    if (setOfParameters.getBool("printPartDBFile"))  {
      std::string partDBFileName = setOfParameters.getString("partDBFileName");
      printPartDataBaseDumpFile(theWitRun, mpHelper, nPeriods, nParts, 
				partList, partDBFileName);
    }
    // Capacity Database Dump File: optional file    
    if (setOfParameters.getBool("printCapacityDBFile"))  {
      std::string capacityDBFileName =
	setOfParameters.getString("capacityDBFileName");
      printCapacityDataBaseDumpFile(theWitRun, mpHelper, nPeriods, nParts, 
				    partList, capacityDBFileName);
    }


    // Operation Database Dump File
    if (setOfParameters.getBool("printOperationDBFile"))  {
      std::string operationDBFileName =
	setOfParameters.getString("operationDBFileName");
      printOperationDataBaseDumpFile(theWitRun, mpHelper, nPeriods, 
	    operationDBFileName);
    }




    
  }

  // free the parts 
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++)  
    witFree(partList[i]);
  witFree(partList);

  int o = 0; // Pulled out of the for below by RW2STL
  for (o=0; o<nOperations; o++)  
    witFree(opList[o]);
  witFree(opList);
}




// *********************************************************************************

int
LgFrScenarioForSce::nDemandsWithNonZeroMins()
const
{
  return nDemandsWithNonZeroMins_;
}

void
LgFrScenarioForSce::nDemandsWithNonZeroMins(int nMins)
{
  nDemandsWithNonZeroMins_ = nMins;
}
// ---------------------------------------------------------------------------
void
LgFrScenarioForSce::incNDemandsWithNonZeroMins()
{
  nDemandsWithNonZeroMins_++;
}
// ---------------------------------------------------------------------------
void
LgFrScenarioForSce::decNDemandsWithNonZeroMins()
{
  nDemandsWithNonZeroMins_--;
}

// ---------------------------------------------------------------------------
int
LgFrScenarioForSce::nStandaloneFeatureArcs()
const
{
  return nStandaloneFeatureArcs_;
}  
// ---------------------------------------------------------------------------  
void
LgFrScenarioForSce::nStandaloneFeatureArcs(int nFeats)
{
  nStandaloneFeatureArcs_ = nFeats;
}
// ---------------------------------------------------------------------------
void
LgFrScenarioForSce::incrementStandaloneFeatureArcs()
{
  nStandaloneFeatureArcs_++;
}
// ---------------------------------------------------------------------------
void
LgFrScenarioForSce::setEngine(const std::string & engine)
{
    engine_ = engine;
    sce::toLower( engine_ );
}

// ---------------------------------------------------------------------------
std::string 
LgFrScenarioForSce::getEngine()
{
    return engine_;
}

// ---------------------------------------------------------------------------
// generate a GeoDemand vectorized SOURCE record for one particular demand
// Returns period number in which there is a "hole".  -1 if no holes
int 
LgFrScenarioForSce::determineGeoToPdfSourceVector(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & geoPlannerDemandPartName,
    std::string * sourceList)
{
  std::string mfgPart;
  std::string pdf;
  std::string geo;
  std::string plannerPart;

  

  int t;
  int nBom;
  int b;
  char * child;
  int early, late;

  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);

  assert(mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun,
						    geoPlannerDemandPartName,
						    plannerPart, geo));

  std::string unknownPdf("???");
  std::string unknownMfgPart("??????");
  std::string unknownPartPdf = mpHelper.pdfPartname(unknownMfgPart, unknownPdf);

  
  // initialize the vector with all unknown Part,PDF for this GPD part
  for (t=0; t<nPeriods; t++) {
    sourceList[t] = unknownPartPdf;
  }
     
  // We need to check the children of this geoPlannerDemand part.
  // The non-normal children can be skipped.  The remaining children
  // represent mfgPart,pdf that are true suppliers of the demanded part.
  // When we find a normal child, we pcik up the effectivity interval of
  // the BOM record.  For that interval, any demand that is shipped to the
  // geo must have been sourced from that pdf-part.  
  witGetOperationNBomEntries(theWitRun, geoPlannerDemandPartName.c_str(), &nBom);
  for (b=0; b<nBom; b++) {
    witGetBomEntryConsumedPart(theWitRun, geoPlannerDemandPartName.c_str(), b, &child);
    if (! mpHelper.isPartNormal(theWitRun, child)) {
      witFree(child);
      continue;
    }

    witGetBomEntryEarliestPeriod(theWitRun, geoPlannerDemandPartName.c_str(), b, &early);
    witGetBomEntryLatestPeriod(theWitRun, geoPlannerDemandPartName.c_str(), b, &late);
    // now set the indicator vector and set the vector geoByPdfSupplyVol
    for (t=early; t<=late; t++) {
      sourceList[t] = child;
    }

    witFree(child);
  }

  // now check to see if anything is unsourced
  int unsourcedPeriod = -1;
  for (t=0; t<nPeriods; t++) {
    if (sourceList[t] == unknownPartPdf) {
      unsourcedPeriod = t;
      break;
    }
  }
  return unsourcedPeriod;
}

//  ---------------------------------------------------------------------
//  Print Overall Problem Statistics
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printOverallProblemStatistics(
	 WitRun * const theWitRun,   // WIT environment
	 LgFrMultiPlantHelper & mpHelper,
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName)
{
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() << "Problem Statistics" << fileName);
  }


   long numRecords = 0;  // for counting output lines

   int i, t, o;
   int nBom;
   int nOps;
   int npBops;
   int nBopTot = 0;
   int nBomTot = 0;
   int nNormalParts = 0;
   int nNormalCapacityParts = 0;
   int nPcfParts = 0;
   int nGeoPlannerDemandParts = 0;
   int nGeoPlannerDemandUBParts = 0;
   int nOptionRatioSupplyParts = 0;
   int nOptionDummyParts = 0;
   int nOptionDummyOperations = 0;
   int nBbCategoryParts = 0;
   int nInterplantOperations = 0;
   int nAggregationOperations = 0;   
   int nUserSpecifiedOperations = 0;
   int nLtGtParts = 0;
   int nGlobalNullSubParts = 0;
   int nGlobalMaxWithoutNullSubParts = 0;
   int nMaxWithoutNullSubParts = 0;
   int nStandaloneFeatureParts = 0;
   int nCustChoiceFeatureParts = 0;
   int nPartsBuildableFromThinAir = 0;
   int nWaifs = 0;
   int nPureOptionBuildDemands = 0;
   int nCustChoiceOptionBuildDemands = 0;

   int * buildableFromThinAir = new int[nParts];
  
   
   std::string d1, d2, d3, d4;

   // Loop once for each part.  
   for ( i=0; i<nParts; i++ ) {
     buildableFromThinAir[i] = 0;
     int constrained = 0;
     std::string thePart = partList[i];

     // count how many bop arcs in total
     witGetPartNProducingBopEntries(theWitRun, thePart.c_str(), &npBops);
     nBopTot += npBops;

     if (mpHelper.isPartNormalCapacity(theWitRun, thePart)) {
       nNormalCapacityParts++;
     }
     else if (mpHelper.isPartNormal(theWitRun, thePart)) {
       nNormalParts++;
       if (mpHelper.isPartPcf(theWitRun, thePart))
	 nPcfParts++;
     }
     else if (mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun, thePart, d1, d2)) {
       nGeoPlannerDemandParts++;
     }
     else if (mpHelper.isPartSpecialGeoPlannerDemandUbPart(theWitRun, thePart, d1, d2)) {
       nGeoPlannerDemandUBParts++;
     }
     else if (mpHelper.isPartSpecialGlobalNullSubstitute(theWitRun, thePart, d1)) {
       nGlobalNullSubParts++;
     }
     else if (mpHelper.isPartSpecialGlobalMaxWithoutNullSubstitute(theWitRun, thePart, d1)) {
       nGlobalMaxWithoutNullSubParts++;
     }     
     else if (mpHelper.isPartSpecialMaxWithoutNullSubstitute(theWitRun, thePart, d1, d2, d3)) {
       nMaxWithoutNullSubParts++;
     }
     else if (mpHelper.isPartSpecialStandaloneFeature(theWitRun, thePart)) {
       nStandaloneFeatureParts++;
     }
     else if (mpHelper.isPartSpecialCustChoiceFeature(theWitRun, thePart)) {
       nCustChoiceFeatureParts++;
     }
     
#if 0
     else if (mpHelper.isPartSpecialPhantom(theWitRun, thePart)) {
       nPhantomParts++;
     }
#endif
     else if (mpHelper.isPartSpecialBbCategory(theWitRun, thePart, d1, d2, d3)) {
       nBbCategoryParts++;
     }
     else if (mpHelper.isPartSpecialLTbbCapacity(theWitRun, thePart, d1, d2, d3)) {
       nLtGtParts++;
     }
     else if (mpHelper.isPartSpecialGTbbCapacity(theWitRun, thePart, d1, d2, d3)) {
       nLtGtParts++;
     }
     else if (mpHelper.isPartSpecialOptionDummy(theWitRun, thePart, d1, d2, d3,d4)) {
       nOptionDummyParts++;
     }
     else if (mpHelper.isPartSpecialOptionRatioSupply(theWitRun, thePart, d1, d2, d3,d4)) {
       nOptionRatioSupplyParts++;
     }
   }

   char ** opList;
   witGetOperations(theWitRun, &nOps, &opList);
   for (o=0; o<nOps; o++) {
     std::string theOperation(opList[o]);
     // count total BOM entries in model
     witGetOperationNBomEntries(theWitRun, theOperation.c_str(), &nBom);
     nBomTot += nBom;

     if (mpHelper.isOperationSpecialInterplant(theWitRun, theOperation, d1, d2, d3, d4)) {
       nInterplantOperations++;
     }

     else if (mpHelper.isOperationSpecialOptionDummy(theWitRun, theOperation, d1, d2, d3,d4)) {
       nOptionDummyOperations++;
     }


     else if (mpHelper.isOperationSpecialAggregation(theWitRun, theOperation, d1, d2, d3, d4)) {
       nAggregationOperations++;
     }
     else {
       witBoolean exists;
       witGetPartExists(theWitRun, theOperation.c_str(), &exists);
       if (! exists)
	 nUserSpecifiedOperations++;
     }
     witFree(opList[o]);
   }
   witFree(opList);


   // CUSTOMER_CHOICE_FEATURES
   // compute the number of Customer Choice OptionBuildDemands ... this should equal the number
   // of feature ratio records accepted?
   for ( i=0; i<nParts; i++ ) {
     std::string fullWitFeaturePartName(partList[i]);
     if (mpHelper.isPartSpecialCustChoiceFeature(theWitRun, fullWitFeaturePartName)) {
       int lenDemandList;
       char ** demandList;
       witGetPartDemands( theWitRun, fullWitFeaturePartName.c_str(), &lenDemandList, &demandList);
       int j = 0; // Pulled out of the for below by RW2STL
       for (j=0; j<lenDemandList; j++) {
	 std::string fullWitGeoPlannerDemandName(demandList[j]);
	 std::string plannerPart;
	 std::string geo;
	 if ( mpHelper.isDemandSpecialCustChoiceFeature( theWitRun, fullWitFeaturePartName,
							 fullWitGeoPlannerDemandName,
							 plannerPart, geo ))
	   nCustChoiceOptionBuildDemands++;
	 witFree(demandList[j]);
       }
       witFree(demandList);
     }
   }


   // compute the number of PureOptionBuildDemands ... this should equal the number
   // of feature ratio records accepted?
   for ( i=0; i<nParts; i++ ) {
     std::string fullWitFeaturePartName(partList[i]);
     if (mpHelper.isPartSpecialStandaloneFeature(theWitRun, fullWitFeaturePartName)) {
       int lenDemandList;
       char ** demandList;
       witGetPartDemands( theWitRun, fullWitFeaturePartName.c_str(), &lenDemandList, &demandList);
       int j = 0; // Pulled out of the for below by RW2STL
       for (j=0; j<lenDemandList; j++) {
	 std::string fullWitGeoPlannerDemandName(demandList[j]);
	 std::string plannerPart;
	 std::string geo;
	 if ( mpHelper.isDemandSpecialPureOptionBuildDemand( theWitRun, fullWitFeaturePartName,
							     fullWitGeoPlannerDemandName,
							     plannerPart, geo ))
	   nPureOptionBuildDemands++;
	 witFree(demandList[j]);
       }
       witFree(demandList);
     }
   }




   // Look for unconstrained and constrained waif parts
   // A waif part is:
   //    - Only have one bop entry
   //    - Only have a default operation
   // An unconstrained waif (unc-waif)
   //    - yield(defaultOp) > 0.0 for all periods
   // A constrained waif has
   //    - yield(defaultOp) = 0.0 in at least one period
   for ( i=0; i<nParts; i++ ) {
     buildableFromThinAir[i] = 0;
     // only consider normal parts
     std::string thePart = partList[i];
     if (! mpHelper.isPartNormal(theWitRun, thePart))
       continue;
     witBoolean exists;
     witGetPartNProducingBopEntries(theWitRun, thePart.c_str(), &npBops);
     // if it doesn't have exactly one part then its not a unc-waif
     if (npBops != 1)
       continue;
     // if default operation does not exist, then skip it .. its not a unc-waif
     witGetOperationExists(theWitRun, thePart.c_str(), &exists);
     if (! exists)
       continue;
     // if default operation has any bom entries then its not an unc-waif
     witGetOperationNBomEntries(theWitRun, thePart.c_str(), &nBom);
     if (nBom > 0)
       continue;
     // if the part is a PCF then its not an unc-waif
     if (mpHelper.isPartPcf(theWitRun, thePart))
       continue;
     // OK the part is a waif
     nWaifs++;
     // now see if its an unc-waif
     float * yieldRate;
     witGetOperationYieldRate(theWitRun, thePart.c_str(), &yieldRate);
     bool uncWaif=true;
     for (t=0; t<nPeriods; t++) {
       if (yieldRate[t] = 0.0) {
	 uncWaif = false;
	 break;
       }
     }
     if (uncWaif) {
       nPartsBuildableFromThinAir++;
       buildableFromThinAir[i] = 1;
     }
     witFree(yieldRate);
   }

   
     
   
   outFile << " Problem Size Statistics\n\n";
   outFile << " Number of Real parts: = " << nNormalParts << "\n";
   outFile << " Number of Capacities: = " << nNormalCapacityParts << "\n";
   outFile << " Number of Operations: = " << nOps << "\n";
   outFile << " Number of Artificial (interplant) \"operations\": = " << nInterplantOperations << "\n";
   outFile << " Number of Artificial (aggregation) \"operations\": = " << nAggregationOperations << "\n";      
   outFile << " Number of User-Defined Operations: = " << nUserSpecifiedOperations << "\n";
   outFile << " Number of lowest level, non-pcf, lead-time constrained real parts : = " << nWaifs - nPartsBuildableFromThinAir << "\n";
   outFile << " Number of lowest level, non-pcf, unconstrained real parts : = " << nPartsBuildableFromThinAir << "\n";
   outFile << " Number of Artificial (GPD demand) \"parts\": = " << nGeoPlannerDemandParts << "\n";
   outFile << " Number of Artificial (demandUB) \"parts\": = " << nGeoPlannerDemandUBParts << "\n";
   outFile << " Number of Artificial (bbCategory) \"parts\": = " << nBbCategoryParts << "\n";
   outFile << " Number of Artificial (SOD) \"parts\": = " << nOptionDummyParts << "\n";
   outFile << " Number of Artificial (SOD) \"operations\": = " << nOptionDummyOperations << "\n";
   outFile << " Number of Artificial (ORS) \"parts\": = " << nOptionRatioSupplyParts << "\n";
   outFile << " Number of Artificial (LT/GT) \"parts\": = " << nLtGtParts << "\n";
   outFile << " Number of Artificial (pureOption build ) \"demands\": = " << nPureOptionBuildDemands << "\n";
   outFile << " Number of Artificial (custChoice build ) \"demands\": = " << nCustChoiceOptionBuildDemands << "\n";
   outFile << " Number of Artificial (global nullSub) \"parts\": = " << nGlobalNullSubParts << "\n";
   outFile << " Number of Artificial (global mwo nullSub) \"parts\": = " << nGlobalMaxWithoutNullSubParts << "\n";
   outFile << " Number of Artificial (mwo nullSub) \"parts\": = " << nMaxWithoutNullSubParts << "\n";
   outFile << " Number of Artificial (custChoice feature) \"parts\": = " << nCustChoiceFeatureParts << "\n";
   outFile << " Number of Artificial (standAlone feature) \"parts\": = " << nStandaloneFeatureParts << "\n";
   outFile << " Total \"Parts\" in SCE model: = " << nParts << "\n";
   outFile << " Total \"Bop Arcs\" in SCE model: = " << nBopTot << "\n";   
   outFile << " Total \"Bom Arcs\" in SCE model: = " << nBomTot << "\n\n\n";

   // Note: for now, since the above print statements are hard-coded, if 
   //       someone changes these in this file, then later on in the
   //       output log, the number of output lines will be inconsistent with
   //       the number actual output lines....so change the following line:
   numRecords = 23;

   outFile << " Unconstrained, lowest level, non-pcf  Parts List:\n";

   for ( i=0; i<nParts; i++ ) {
     if (buildableFromThinAir[i]) {
       outFile << "\"" << mpHelper.partname(partList[i]) << "\",\""
	       << mpHelper.pdf(partList[i]) << "\"\n";
       numRecords++;
     }
   }
   
   outFile.close();
   delete [] buildableFromThinAir;
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
 }



//  ---------------------------------------------------------------------
//  Print Lowest Level, unconstrained parts (non-pcf)
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printLowestLevelNonPcfParts(
	 WitRun * const theWitRun,   // WIT environment
	 LgFrMultiPlantHelper & mpHelper,
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName)
{
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() << "Lowest Level, Non-pcf" << fileName);
  }


   long numRecords = 0;  // for counting output lines

   int i;
   int nBom;
   int npBops;
   witBoolean exists;  
   

   // Look for Lowest Level, non-pcf parts:
   //    - Only have one bop entry
   //    - Only have a default operation
   //    - not a pcf part
   for (i=0; i<nParts; i++ ) {
     // only consider normal parts
     std::string thePart = partList[i];
     if (! mpHelper.isPartNormal(theWitRun, thePart))
       continue;

     witGetPartNProducingBopEntries(theWitRun, thePart.c_str(), &npBops);
     // if it doesn't have exactly one part then its not lowest-level-non-pcf
     if (npBops != 1)
       continue;
     // if default operation does not exist, then skip it .. its not lowest-level-non-pcf
     witGetOperationExists(theWitRun, thePart.c_str(), &exists);
     if (! exists)
       continue;
     // if default operation has any bom entries then its not lowest-level-non-pcf
     witGetOperationNBomEntries(theWitRun, thePart.c_str(), &nBom);
     if (nBom > 0)
       continue;
     // if the part is a PCF then its not lowest-level-non-pcf
     if (mpHelper.isPartPcf(theWitRun, thePart))
       continue;
     // OK the part is lowest-level, non-pcf
     outFile << "\"" << mpHelper.partname(partList[i]) << "\",\""
	     << mpHelper.pdf(partList[i]) << "\"\n";
     numRecords++;
   }

   
   outFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
 }


#ifdef MARK_BOGONS
//  ---------------------------------------------------------------------
//  Print PCF Bogons (parts which were specified as PCF but got rejected due to child parts)
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printBogonParts(
	 WitRun * const theWitRun,   // WIT environment
	 LgFrMultiPlantHelper & mpHelper,
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName)
{
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() << "Bogon List" << fileName);
  }


   long numRecords = 0;  // for counting output lines

   int i;
   // Look for Lowest Level, non-pcf parts:
   //    - Only have one bop entry
   //    - Only have a default operation
   //    - not a pcf part
   for (i=0; i<nParts; i++ ) {
     std::string thePart = partList[i];
     if (! mpHelper.isPartBogon(theWitRun, thePart))
       continue;
     outFile << "\"" << mpHelper.partname(partList[i]) << "\",\""
	     << mpHelper.pdf(partList[i]) << "\"\n";
     numRecords++;
   }

   
   outFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
 }
#endif



//  ---------------------------------------------------------------------
//  Calculate and Print Value Added Costs
//  --------------------------------------------------------------------- 
//  calculation of vac is limited to default operations only.   that means that
//  VAc and compCost will be based on in-plant mfg cost or rawCost at that site.
//
void 
LgFrScenarioForSce::calculateAndPrintValueAddedCosts(
	 WitRun * const theWitRun,   // WIT environment
	 LgFrMultiPlantHelper & mpHelper,
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string vacFileName,
	 std::string compCostFileName)
{
  std::ofstream vacFile(vacFileName.c_str());//, ios::out, 0666);
  if (vacFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() << "Value Added Cost" << vacFileName);
  }

  std::ofstream compCostFile(compCostFileName.c_str());//, ios::out, 0666);
  if (compCostFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() << "Comp cost " << compCostFileName);
  }

   long numRecords = 0;  // for counting output lines

   std::string leadingString;

   int i, t;

   std::cout << "   ... number of parts to process: " << nParts << std::endl;                

  //input: rawCost of each part
  //output: vacCost of each part
  //        compCost of each part

  // loop over parts in sorted list 
  //   get next part on list: part-x (if no more parts, then DONE)
  //   if Part-x has No children then  {
  //         vacCost(part-x) = rawCost(part-x)
  //         compCost(x) = 0
  //   if Part-x has children then {
  //      initialize: vacCost(x) = rawCost(x)
  //      initialize: compCost(x) = 0
  //      for each child of Part-x:    {
  //           compCost(x) += (compCost(child) + vacCost(child))  * usageRate / ((1-fallout) * yield(x))
  //
  //      }
  //      vacCost(x) = max{0, rawCost(x) - compCost(x)
  //   }

   int kParts, lastK;
   lastK=0;

   for (i=0; i<nParts; i++ ) {
     std::string thePart = partList[i];

     kParts = i/10;
     if (kParts > lastK) {
       lastK = kParts;
       //       std::cout << "   ... processed " << i << " parts out of " << nParts << std::endl;                
     }


     LgFrTimeVecFloat vacCost(nPeriods, 0.0);
     LgFrTimeVecFloat compCost(nPeriods, 0.0);

     witBoolean exists;
     witGetOperationExists(theWitRun, partList[i], &exists);
     if (exists) {
       float * rawCost;
       witGetOperationObj1ExecCost(theWitRun, partList[i], &rawCost);
       for (t=0; t<nPeriods; t++)
	 vacCost[t] = rawCost[t];

       int nBom;
       witGetOperationNBomEntries(theWitRun, partList[i], &nBom);
       
       float * yieldRate;
       witGetOperationYieldRate(theWitRun, partList[i], &yieldRate);

       
       int b;
       for (b=0; b<nBom; b++) {
	 char * child;
	 witGetBomEntryConsumedPart(theWitRun, partList[i], b, &child);
	 float * consRate;
	 witGetBomEntryConsRate(theWitRun, partList[i], b, &consRate);
	 float falloutRate;
	 witGetBomEntryFalloutRate(theWitRun, partList[i], b, &falloutRate);
	 int * impactPeriod;
	 witGetBomEntryImpactPeriod(theWitRun, partList[i], b, &impactPeriod);
	 

	 LgFrScePartAppData * childAppDataPtr;    
	 witGetPartAppData(theWitRun, child, (void **) &childAppDataPtr);

	 // child MUST have been seen alrady, and hence has an appData
	 assert (childAppDataPtr != 0);

	 LgFrTimeVecFloat childVac = childAppDataPtr->vac();
	 LgFrTimeVecFloat childCompCost = childAppDataPtr->compCost();


	 for (t=0; t<nPeriods; t++) {
	   if ( (yieldRate[t] > 0.0) && (impactPeriod[t] > -1) ) {
	     compCost[t] += 
	       ((childCompCost[impactPeriod[t]] + childVac[impactPeriod[t]]) * consRate[t] )
	       / ((1.0 - falloutRate) * yieldRate[t]);
	   }
	 }

	 witFree(child);
	 witFree(consRate);
	 witFree(impactPeriod);
       }

       for (t=0; t<nPeriods; t++)
	 vacCost[t] =  (rawCost[t] - compCost[t] > 0.0) ? rawCost[t] - compCost[t] : 0.0f;


       witFree(yieldRate);
       witFree(rawCost);
     }

     LgFrScePartAppData * appDataPtr;    
     witGetPartAppData(theWitRun, partList[i], (void **) &appDataPtr);
     if (appDataPtr == 0) {
       appDataPtr = new LgFrScePartAppData(0, nPeriods, nPeriods);
       appDataPtr->vac(vacCost);
       appDataPtr->compCost(compCost);
       witSetPartAppData(theWitRun, partList[i], (void *) appDataPtr);
     }
     else {
       appDataPtr->vac(vacCost);
       appDataPtr->compCost(compCost);
     }


     // only write the NORMAL records,
     if (mpHelper.isPartNormal(theWitRun, partList[i])) {
       leadingString = "\""
	 + mpHelper.partname(partList[i]) + "\",\""
	 + mpHelper.pdf(partList[i]) + "\"";
       flatFileMgr_.writeFloatVecToStream(this->calendar(), vacFile, nPeriods, 0, leadingString, vacCost.data());
       
       leadingString = "\""
	 + mpHelper.partname(partList[i]) + "\",\""
	 + mpHelper.pdf(partList[i]) + "\"";
       flatFileMgr_.writeFloatVecToStream(this->calendar(), compCostFile, nPeriods, 0, leadingString, compCost.data());
       numRecords++;
     }
   }

   vacFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(vacFileName,numRecords,000,numRecords);
   compCostFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(compCostFileName,numRecords,000,numRecords);

 }

//  ---------------------------------------------------------------------
//  Print Optimization data
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printOverallOptimizationValues(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string & engine,
	 std::string fileName)
{

  std::string partName;

  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Overall Optimization Values" << fileName);
  }


   long numRecords = 0;

   float * obj1Reward;
   float * obj1CSReward;
   int   * priority;
   float cumShipVol;
   float * shipVol;
   float * demandVol;
   float cumpts = 0.0;
   float cumblog = 0.0;
   int nDemands;
   char ** dList;
   float cumDemVol = 0.0;
   float newBlog;
   float onTimeServ = 0.0;
   float cumInt = 0.0;
   int i, j, t;
   float cumShipOnTime = 0.0;
   float cumTotalDemVol = 0.0;
   float shipToDemThisPeriod;
   float objValue;
   float boundsValue;


   // Loop once for each part.  
   for ( i=0; i<nParts; i++ ) {

       witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);
       for (j =0; j<nDemands; j++)  {
	 if (! sce::compareStringsCaseIns( engine, "optimization" )) {
	   witGetDemandObj1ShipReward(theWitRun, partList[i], dList[j], &obj1Reward);
	   witGetDemandObj1CumShipReward(theWitRun, partList[i], dList[j], &obj1CSReward);
	 }
	 else {
	   witGetDemandPriority(theWitRun, partList[i], dList[j], &priority);
	 }
	 witGetDemandShipVol(theWitRun, partList[i], dList[j], &shipVol);
	 witGetDemandDemandVol(theWitRun, partList[i], dList[j], &demandVol);
	 cumShipVol = 0.0;
	 cumDemVol = 0.0;
	 for (t=0; t<nPeriods; t++) {
	   cumShipVol += shipVol[t];
	   if (! sce::compareStringsCaseIns( engine, "optimization" )) {
	     cumpts += (shipVol[t] * obj1Reward[t]);
	     cumInt += (cumShipVol * obj1CSReward[t]);
	   }
	   else
	       cumpts += (shipVol[t] * priority[t]);             
	   newBlog = (demandVol[t] > shipVol[t]) ? demandVol[t] - shipVol[t] : 0.0f;
	   shipToDemThisPeriod = (cumShipVol > cumDemVol) ? cumShipVol - cumDemVol : 0.0f;
	   cumShipOnTime += shipToDemThisPeriod;
				      
	   cumblog += newBlog;
	       cumDemVol += demandVol[t];
	       cumTotalDemVol += demandVol[t];
	   }
	   witFree(dList[j]);
	   if (! sce::compareStringsCaseIns( engine, "optimization" )) {           
	     witFree(obj1Reward);
	     witFree(obj1CSReward);
	   }
	   else 
	     witFree(priority);           
	   witFree(shipVol);
	   witFree(demandVol);
       }
       witFree(dList);
   }
   if (cumTotalDemVol > 0.0001)
     onTimeServ = cumShipOnTime / cumTotalDemVol;
#ifdef SCE_OPT_IMPLODE   
   witGetObjValues(theWitRun, &objValue, &boundsValue);
#endif
   
   outFile << "SCE Optimization Summary Report:\n";
   outFile << "  - Engine used:" << engine <<"\n";   
   
   outFile << "  - Primary  objective value: = " << cumpts << "\n";
   numRecords += 2;

   if (cumTotalDemVol > 0.0001)   
     outFile << "  - On time serviceability: = " << onTimeServ << "\n";
   else
     outFile << "  - On time serviceability: = ERROR all demands have ZERO volume\n";
   numRecords++;

   if (! sce::compareStringsCaseIns( engine, "optimization" )) {
     outFile << "  - Additional interest reward : = " << cumInt << "\n";
     numRecords++;

#ifdef SCE_OPT_IMPLODE        
     outFile << "  - Total WIT optimization objective value: = " << objValue << "\n";
     outFile << "  - Penalty for violating Min Demand Volumes: = " << boundsValue << "\n";
     numRecords += 2;
#endif     
     // outFile << " Additional cost due to Substitutes = " << 0 << "\n";     
     numRecords++;
   }

   outFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}

//  ---------------------------------------------------------------------
//  Print Capacity   Information         
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printCapacityDataBaseDumpFile(
    WitRun * const theWitRun,   // WIT environment
    LgFrMultiPlantHelper & mpHelper,
    const int nPeriods,         // Number of periods 
    const int nParts,           // Number of parts   
    char ** partList,           // List of part names
    std::string fileName)
{

  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Capacity Database Dump" << fileName);
  }

  LgFrCalendar & theCal = calendar();

  long numRecords = 0;
  long numUniquePairs = 0;

  float * witVector;
  float * reqVol;
  float * mrpExcessVol;      

  int i, p, b, npBops;

  LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();
  LgFrSceBaseExploder * myExploder = this->exploder();          

  std::string siteCustomization = setOfParameters.getString("siteCustomization");
  bool doImplode = setOfParameters.getBool("doImplode");

  int nonZeroOnly = 0;
  int alwaysPrint = 1;

  //  - userOpVol, mrpUserOpVol  
  bool anyUserDefinedOperations = false;
  float * userOpVol;
  float * mrpUserOpVol;    

  if (! setOfParameters.getString("opFile").empty()) {
    anyUserDefinedOperations = true;
    userOpVol = new float[nPeriods];
    mrpUserOpVol = new float[nPeriods];    
  }


  //  float * bldVol = new float[nPeriods];
  //  float * mrpBldVol = new float[nPeriods];    


   
  // Loop once for each part.  
  for ( i=0; i<nParts; i++ ) {
    std::string fullPartName(partList[i]);
    if (! mpHelper.isPartNormalCapacity(theWitRun, fullPartName))
      continue;
     
    std::string pn(mpHelper.partname(fullPartName));
    std::string pdf(mpHelper.pdf(fullPartName));

     
    // supplyVol
    witGetPartSupplyVol(theWitRun, fullPartName.c_str(), &witVector);
    //    numRecords += printDbDumpRec(pn, pdf, "supplyVol", witVector, outFile, nPeriods, alwaysPrint);
    numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "supplyVol", witVector, outFile, nPeriods, alwaysPrint);
    witFree(witVector);
     

       
    // mrpConsVol
    myExploder->mrpConsVol(partList[i], &witVector);
    numRecords += printDbDumpRec(pn, pdf, "mrpConsVol", witVector, outFile, nPeriods, nonZeroOnly);
    witFree(witVector);

    // reqVol
    myExploder->reqVol(partList[i], &reqVol);
    numRecords += printDbDumpRec(pn, pdf, "reqVol", reqVol, outFile, nPeriods, nonZeroOnly);

    // mrpExcessVol
    myExploder->mrpExcessVol(partList[i], &mrpExcessVol);
    numRecords += printDbDumpRec(pn, pdf, "mrpExcessVol", mrpExcessVol, outFile, nPeriods, nonZeroOnly);

    // mrpRequirements
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++) {
      reqVol[t] -= mrpExcessVol[t];
    }
    numRecords += printDbDumpRec(pn, pdf, "mrpRequirements", reqVol, outFile, nPeriods, nonZeroOnly);
    witFree(reqVol);
    witFree(mrpExcessVol);
       

    // Use nBops instead of partCategory
    witGetPartNProducingBopEntries(theWitRun, fullPartName.c_str(), &npBops);
    // Now report on all the execVol types:
    // MRP -------->  mrpUserOpVol
    // Implosion -->  userOpVol
    
    if (npBops > 0) {
      // first intialize the vectors to zero
      if (anyUserDefinedOperations) {
	int t = 0; // Pulled out of the for below by RW2STL
	for (t=0; t<nPeriods; t++) {
	  userOpVol[t] = 0;
	  mrpUserOpVol[t] = 0;
	}
      }

      // loop through each of the bop entries and get the execVol once for each
      // bop entry (this is a bit inefficient, but too bad).
      for (p=0; p<npBops; p++) {
	bool foundOpCategory = false;          
	  
	std::string cp1, pdf1, sp2, pdf2;
	int early, late;
	float  prodRate;
	char * witOperationName;
	float * tempExecVol;
	float * tempMrpExecVol;
	float * yieldRate;
	float * mrpYieldRate;        
	  
	witGetPartProducingBopEntry(theWitRun, fullPartName.c_str(), p, &witOperationName, &b);
	std::string operationName(witOperationName);
	witGetBopEntryProdRate(theWitRun, witOperationName, b, &prodRate);
	witGetBopEntryEarliestPeriod(theWitRun, witOperationName, b, &early);
	witGetBopEntryLatestPeriod(theWitRun, witOperationName, b, &late);
	myExploder->mrpExecVol(witOperationName, &tempMrpExecVol);          
	if (doImplode) {
	  witGetOperationExecVol(theWitRun, witOperationName, &tempExecVol);
	}
	witGetOperationYieldRate(theWitRun, witOperationName, &yieldRate);
	witGetOperationYieldRate(myExploder->localWitRun(), witOperationName, &mrpYieldRate);        

#if 0
	// Is it the default Operation
	std::string cp1, pdf1;
	if ( mpHelper.isOperationForCapacityGeneration(theWitRun, witOperationName, cp1, pdf1)) {
	  int t = 0; 
	  for (t=0; t<nPeriods; t++) {
	    if (doImplode) {
	      bldVol[t]     += yieldRate[t] * tempExecVol[t];
	    }
	    mrpBldVol[t]  += mrpYieldRate[t] * tempMrpExecVol[t];
	  }
	  foundOpCategory = true;
	}
#endif

	// is it a UserDefined Operation (it must be, at this point)
	if (! foundOpCategory
	    && anyUserDefinedOperations) {
	  int * impactPeriod;
	  int * mrpImpactPeriod;          
	  witGetBopEntryImpactPeriod(theWitRun, witOperationName, b, &impactPeriod);
	  witGetBopEntryImpactPeriod(myExploder->localWitRun(), witOperationName, b, &mrpImpactPeriod);
	  int t = 0; // Pulled out of the for below by RW2STL
	  for (t=early; t<=late; t++) {
	    if (impactPeriod[t] > -1)
	      if (doImplode) {
		userOpVol[impactPeriod[t]]       += yieldRate[t] * prodRate * tempExecVol[t];
	      }
	    if (mrpImpactPeriod[t] > -1)            
	      mrpUserOpVol[mrpImpactPeriod[t]] += mrpYieldRate[t] * prodRate * tempMrpExecVol[t];
	  }
	
	  witFree(impactPeriod);
	  witFree(mrpImpactPeriod);
	  foundOpCategory = true;
	}

	// Now assert that the operation resolved into exactly one of the types!!
	//	assert(foundOpCategory);

	// clean-up the memory for this bop
	witFree(witOperationName);
	if (doImplode) {
	  witFree(tempExecVol);
	}
	witFree(tempMrpExecVol);
	witFree(yieldRate);
	witFree(mrpYieldRate);        
      }
	
      // print userOpVol and mrpUserOpVol
      if (anyUserDefinedOperations) {
	numRecords += printDbDumpRec(pn, pdf, "mrpUserOpVol", mrpUserOpVol, outFile, nPeriods, nonZeroOnly);
      }

      if (doImplode) {        
	if (anyUserDefinedOperations) {
	  numRecords += printDbDumpRec(pn, pdf, "userOpVol", userOpVol, outFile, nPeriods, nonZeroOnly);
	}
      }
    }
    


    if (doImplode) {
       
      // ExcessVol
      witGetPartExcessVol(theWitRun, fullPartName.c_str(), &witVector);
      numRecords += printDbDumpRec(pn, pdf, "excessVol", witVector, outFile, nPeriods, nonZeroOnly);
      witFree(witVector);

      // scrapVol
      witGetPartScrapVol(theWitRun, fullPartName.c_str(), &witVector);
      numRecords += printDbDumpRec(pn, pdf, "scrapVol", witVector, outFile, nPeriods, nonZeroOnly);
      witFree(witVector);

      // consVol
      witGetPartConsVol(theWitRun, fullPartName.c_str(), &witVector);
      numRecords += printDbDumpRec(pn, pdf, "consVol", witVector, outFile, nPeriods, nonZeroOnly);
      witFree(witVector);

    }

    numUniquePairs++;
  }

  //  - userOpVol, mrpUserOpVol  
  if (anyUserDefinedOperations) {
    if (doImplode) {
      delete [] userOpVol;
    }
    delete [] mrpUserOpVol; 
  }
  
  outFile.close();
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numUniquePairs);
}

void 
LgFrScenarioForSce::printStandAloneFeatureAnalysisReport(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper,
	 int printDataAsPercentage)
{
  std::string partName;

  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() << "Feasible Optional Feature Ratio" << fileName);
  }


  long numRecords = 0;

  int t, i;

  std::string plannerTopLevelPart;
  std::string geo;
  std::string mfgOptionPart;
  std::string bbCategory;

  float * feasibleFeatureRatio = new float[nPeriods];

  std::string leadingString;  

  // -----------------------------------------------------------------
  // Now look at CUSTOMER_CHOICE_ FEATURES
  // -----------------------------------------------------------------
  for ( i=0; i<nParts; i++ ) {
    std::string custChoiceFeaturePart(partList[i]);
    std::string featurePart;
    std::string ppp;
    std::string mmm, ggg;
    if (mpHelper.isPartSpecialCustChoiceFeature(theWitRun, custChoiceFeaturePart, ggg, mmm, ppp, featurePart)) {
      int lenDemandList;
      char ** demandList;
      witGetPartDemands( theWitRun, custChoiceFeaturePart.c_str(), &lenDemandList, &demandList);
      if ( lenDemandList == 0) {
	witFree (demandList);  
	continue;
      }
	
      int j = 0; 
      for (j=0; j<lenDemandList; j++) {
	std::string custChoiceDemandName(demandList[j]);
	std::string plannerPart;
	std::string geo;
	if (! mpHelper.isDemandSpecialCustChoiceFeature( theWitRun, custChoiceFeaturePart, custChoiceDemandName, plannerPart, geo ))
	  continue;

	// We've found a stand-alone option and planner part pair.
	// The option comes from the custChoiceFeaturePart and the planner part comes 
	// from the fullWitDemandName
	// There's only one "build demand" for this pair, but there may be many BOM arc each with subs
	// to account for.

	int ncbe;
	witGetPartNConsumingBomEntries(theWitRun, custChoiceFeaturePart.c_str(), &ncbe);
	assert(ncbe == 1);
	char * gpdWitPartName;
	int bomIndex;
	witGetPartConsumingBomEntry(theWitRun, custChoiceFeaturePart.c_str(), 0, &gpdWitPartName, &bomIndex);

	// what is the gpd exec Vol?
	float * gpdExecVol;
	witGetOperationExecVol(theWitRun, gpdWitPartName, &gpdExecVol);

	// What was the usageRate on the bom entry
	float usageRate;
	witGetBomEntryUsageRate(theWitRun, gpdWitPartName, bomIndex, &usageRate);

	float * sfpSupplyVol;
	witGetPartSupplyVol(theWitRun, custChoiceFeaturePart.c_str(), &sfpSupplyVol);

	// get  execVol  of the specal feature part
	float * sfpExecVol;
	witGetOperationExecVol(theWitRun, custChoiceFeaturePart.c_str(), &sfpExecVol);

	int t = 0; 
	for (t=0; t<nPeriods; t++)  
	  feasibleFeatureRatio[t] = sfpExecVol[t] + sfpSupplyVol[t];

	// get  consVol of the specal feature part
	float * featureConsVol;
	witGetPartConsVol(theWitRun, custChoiceFeaturePart.c_str(), &featureConsVol);

	// Pick up any excessVol of the custChoiceFeaturePart.  This would be 
	// generated from Machine Feature Supply (ie, FEN5) 
	float * ccfeatureExcessVol;
	witGetPartExcessVol(theWitRun, custChoiceFeaturePart.c_str(), &ccfeatureExcessVol);
	
	// Account for the amount of feature we built (i.e. ship to the PureOptionBuildDemand)
	// Initialize feasibleFeatureRatio using shipped amount, then incremently add to it
	float * shipVol = 0;
	witGetDemandShipVol( theWitRun, custChoiceFeaturePart.c_str(), custChoiceDemandName.c_str(), &shipVol);

	// okay TOTAL CONS VOL of the FEATURE PART (post usageRate) is
	for (t=0; t<nPeriods; t++)  
	  featureConsVol[t] += (ccfeatureExcessVol[t] + shipVol[t]);

	// and now divide by the usageRate to get the pre usageRate
	if (usageRate > .001)
	  for ( t=0; t<nPeriods; t++) 
	    featureConsVol[t] /= usageRate;
	
	witFree (shipVol);
	witFree(gpdExecVol);
	witFree(featureConsVol);
	witFree(ccfeatureExcessVol);
	witFree(sfpExecVol);
	witFree(sfpSupplyVol);
	witFree(gpdWitPartName);

	mpHelper.populateLeadingStringGPD(plannerPart, geo, leadingString);
	leadingString += ",\"" 	  + featurePart + "\"";
	flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, feasibleFeatureRatio);

	numRecords++;
      }
      
      for (j=0; j<lenDemandList; j++)
	witFree (demandList[j]);
      witFree (demandList);
    }
  }



  // -----------------------------------------------------------------
  // Now look at STANDALONE OPTIONS modeled via 0 SUBSTITUTE !!
  // -----------------------------------------------------------------

  // Loop once for each part, looking for stand alone options
  // Here we are going to take into account the 2-passes of implosions needed to "build features"
  for ( i=0; i<nParts; i++ ) {
    std::string fullWitFeaturePartName(partList[i]);
    std::string featurePart;
    std::string ppp;
    std::string mmm, ggg;
    if (mpHelper.isPartSpecialStandaloneFeature(theWitRun, 
						fullWitFeaturePartName, 
						ggg, 
						mmm, 
						ppp, 
						featurePart)) {
      int lenDemandList;
      char ** demandList;
      witGetPartDemands( theWitRun, fullWitFeaturePartName.c_str(), &lenDemandList, &demandList);
      if ( lenDemandList == 0) {
	witFree (demandList);  
	continue;
      }
	
      int j = 0; // Pulled out of the for below by RW2STL
      for (j=0; j<lenDemandList; j++) {
	std::string fullWitStandaloneFeatureDemandName(demandList[j]);
	std::string plannerPart;
	std::string geo;
	if (! mpHelper.isDemandSpecialPureOptionBuildDemand( theWitRun, fullWitFeaturePartName,
							     fullWitStandaloneFeatureDemandName,
							     plannerPart, geo ))
	  continue;
	// We've found a stand-alone option and planner part pair.
	// The option comes from the fullWitPartName and the planner part comes from the fullWitDemandName
	// There's only one "build demand" for this pair, but there may be many BOM arc each with subs
	// to account for.

	std::string plannerTopLevelPart = mpHelper.geoPlannerDemandPartName(plannerPart, geo);
	
	// What was the total production on the geoPlannerDemandPart? 
	float * gpdProdVol;
	witGetPartProdVol(theWitRun, plannerTopLevelPart.c_str(), &gpdProdVol);
	
	// Account for the amount of feature we built (i.e. ship to the PureOptionBuildDemand)
	// Initialize feasibleFeatureRatio using:
	//  shipped amount and the machine Feature supply (fen5) amount, then incremently add to it
	float * shipVol;
	witGetDemandShipVol( theWitRun, fullWitFeaturePartName.c_str(), fullWitStandaloneFeatureDemandName.c_str(), &shipVol);
	float * fen5supplyVol;
	witGetPartSupplyVol(theWitRun, fullWitFeaturePartName.c_str(), &fen5supplyVol);
	
	
	if (printDataAsPercentage) {
	  int t = 0; // Pulled out of the for below by RW2STL
	  for (t=0; t<nPeriods; t++)   {
	    if (gpdProdVol[t] < 0.01) 
	      feasibleFeatureRatio[t] = 0.0;
	    else 
	      feasibleFeatureRatio[t] = shipVol[t]/gpdProdVol[t];
	  }
	}
	else {
	  int t = 0; // Pulled out of the for below by RW2STL
	  for (t=0; t<nPeriods; t++)   {
	    feasibleFeatureRatio[t] = shipVol[t] + fen5supplyVol[t];
	  }
	}
	witFree (shipVol);
	witFree(fen5supplyVol);

	// Account for the amount of feature that was in supply...
	int nBom;
	witGetOperationNBomEntries(theWitRun, plannerTopLevelPart.c_str(), &nBom);
	int k = 0; // Pulled out of the for below by RW2STL
	for (k=0; k<nBom; k++)  {
	  char * child;
	  witGetBomEntryConsumedPart(theWitRun, plannerTopLevelPart.c_str(), k, &child);
	  std::string childPart(child);
	  if ( !(childPart == fullWitFeaturePartName)){
	    witFree( child);
	    continue;
	  }

	  // We've got a BOM arc between the plannerTopLevelPart and the option
	  // Now, consider all the substitues...
	  int nSubBom;
	  witGetBomEntryNSubsBomEntries(theWitRun, plannerTopLevelPart.c_str(), k, &nSubBom);
	  int sj = 0; // Pulled out of the for below by RW2STL
	  for (sj=0; sj<nSubBom; sj++)  {
	    char * sub;
	    float * subProdVol;
	    witGetSubsBomEntryConsumedPart(theWitRun, plannerTopLevelPart.c_str(), k, sj, &sub);
	    // if the subPart is NOT a nullSubstitute then forget it
	    if (! mpHelper.isPartSpecialNullSubstitute(theWitRun, sub)) {
	      witFree(sub);
	      continue;
	    }
	    witGetSubsBomEntryProdVol(theWitRun, plannerTopLevelPart.c_str(), k, sj, &subProdVol);
	    
	    float usageRatio;
	    witGetBomEntryUsageRate(theWitRun, plannerTopLevelPart.c_str(), k, &usageRatio);
	    int early, late;
	    witGetBomEntryEarliestPeriod(theWitRun, plannerTopLevelPart.c_str(), k, &early);
	    witGetBomEntryLatestPeriod(theWitRun, plannerTopLevelPart.c_str(), k, &late);            
	      
	    if (printDataAsPercentage) {
	      for (t=early; t<=late; t++)   {
		if (gpdProdVol[t] > 0.01) 
		  feasibleFeatureRatio[t] += usageRatio * (gpdProdVol[t] - subProdVol[t])/gpdProdVol[t];
	      }
	    }
	    else {
	      for (t=early; t<=late; t++)   {
		feasibleFeatureRatio[t] += usageRatio * (gpdProdVol[t] - subProdVol[t]);
	      }
	    }
	    witFree(sub);
	    witFree(subProdVol);
	  }
	  witFree(child);
	}
	witFree(gpdProdVol);


	mpHelper.populateLeadingStringGPD(plannerPart, geo, leadingString);
	leadingString += ",\"" 	  + featurePart + "\"";
	flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, feasibleFeatureRatio);
	numRecords++;
      }
      
      for (j=0; j<lenDemandList; j++)
	witFree (demandList[j]);
      witFree (demandList);
    }
  }
  delete [] feasibleFeatureRatio;

  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}

  //***ended





#ifdef BUILDING_BLOCKS_VIA_MULTI_BOPS

// THIS IS OBSOLTETE, code left around for possible re-use
  // this method writes FEATURE SET report  
void 
LgFrScenarioForSce::printFeatureAnalysisReportViaMultiRoute(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper,
	 int printDataAsPercentage)
{
   std::string partName;

   std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Feasible Selective Feature/Building Block Ratio" 
		       << fileName);
  }


   long numRecords = 0;

   int t, i;

   std::string plannerTopLevelPart;
   std::string geo;
   std::string mfgOptionPart;
   std::string bbCategory;

   float * feasibleFeatureRatio = new float[nPeriods];


   // Loop once for each part, looking for specialOptionDummy's.  
   for ( i=0; i<nParts; i++ ) {
     std::string fullWitPartName(partList[i]);
     if (mpHelper.isPartSpecialOptionRatioSupply(theWitRun, fullWitPartName, plannerTopLevelPart, 
						 geo, bbCategory, mfgOptionPart))   {
       if (bbCategory.contains("STANDALONE:", std::string::ignoreCase))
	 continue;

       // find the consVol of the specialOptionRatioSupply
       float * orsConsVol;
       witGetPartConsVol(theWitRun, fullWitPartName.c_str(), &orsConsVol);


      // if the user wants data as a percentage of geoPlanner ship vol,
      // then we must get that and do the math.
       if (printDataAsPercentage) {
	 // find the build volume of the geoPLannerDemandPart
	 std::string geoPlannerDemandPart(mpHelper.geoPlannerDemandPartName(
									  plannerTopLevelPart, geo));
	 float * gpdExecVol;
	 witGetOperationExecVol(theWitRun, geoPlannerDemandPart.c_str(), &gpdExecVol);

	 for (t=0; t<nPeriods; t++)   {
	   if (gpdExecVol[t] < 0.01) 
	     feasibleFeatureRatio[t] = 0.0;
	   else 
	     feasibleFeatureRatio[t] = orsConsVol[t]/gpdExecVol[t];
	 }
	 witFree(gpdExecVol);
       }

       // first print the preamble of the record
       outFile << "\"" << plannerTopLevelPart << "\",\"" << geo 
	       << "\",\"" << mfgOptionPart << "\"";

       if (printDataAsPercentage) 
	 flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, feasibleFeatureRatio, 4, 15);
       else
	 flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, orsConsVol, 4, 15);      
       numRecords++;
       witFree(orsConsVol);      
       }
   }

   delete [] feasibleFeatureRatio;
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}
#endif




  // this method writes FEATURE SET report  when using Proportional routing approach
void 
LgFrScenarioForSce::printFeatureAnalysisReportViaProportionalRoute(
	 WitRun * const theWitRun,
	 const int nPeriods,
	 const int nOperations,
	 char ** operationList,
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper,
	 int printDataAsPercentage)
{
  
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() << "Feasible Selective Feature/Building Block Ratio" << fileName);
  }
  
  
  long numRecords = 0;
  
  int t;
  
  std::string plannerTopLevelPart;
  std::string geo;
  std::string mfgOptionPart;
  std::string bbCategory;
  
  std::string leadingString;  

  int o = 0; 
  for (o=0; o<nOperations; o++ ) {
    std::string fullWitSodName(operationList[o]);
    if (! mpHelper.isOperationSpecialOptionDummy(theWitRun, fullWitSodName, plannerTopLevelPart, geo, bbCategory, mfgOptionPart)) {
      continue;
    }

    // find the execVol of the SOD
    float * sodExecVol;
    witGetOperationExecVol(theWitRun, fullWitSodName.c_str(), &sodExecVol);

    mpHelper.populateLeadingStringGPD(plannerTopLevelPart, geo, leadingString);
    leadingString += ",\""  + bbCategory + "\",\"" + mfgOptionPart + "\"";
    flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, sodExecVol);
    numRecords++;
    witFree(sodExecVol);    
  }
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}



  // this method writes FEATURE SET report  when using LP approach
void 
LgFrScenarioForSce::printFeatureAnalysisReport(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper,
	 int printDataAsPercentage)
{
   std::string partName;

   std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Feasible Selective Feature/Building Block Ratio" 
		       << fileName);
  }


   long numRecords = 0;

   int t, i;

   std::string plannerTopLevelPart;
   std::string geo;
   std::string mfgOptionPart;
   std::string bbCategory;

   float * feasibleFeatureRatio = new float[nPeriods];

   std::string leadingString;

   // Loop once for each part, looking for specialOptionDummy's.  
   for ( i=0; i<nParts; i++ ) {
     std::string fullWitPartName(partList[i]);
     if (mpHelper.isPartSpecialOptionDummy(theWitRun, fullWitPartName, plannerTopLevelPart, 
					   geo, bbCategory, mfgOptionPart))   {
       //         if (sce::findCaseIns(bbCategory, "STANDALONE:") != sce::badIndex)
       //	 continue;

       // find the buildVolume of the specialOptionDummy
       float * sodProdVol;
       witGetPartProdVol(theWitRun, fullWitPartName.c_str(), &sodProdVol);


      // if the user wants data as a percentage of geoPlanner ship vol,
      // then we must get that and do the math.
       if (printDataAsPercentage) {
	 // find the build volume of the geoPLannerDemandPart
	 std::string geoPlannerDemandPart(mpHelper.geoPlannerDemandPartName(
									  plannerTopLevelPart, geo));
	 float * gpdProdVol;
	 witGetPartProdVol(theWitRun, geoPlannerDemandPart.c_str(), &gpdProdVol);

	 for (t=0; t<nPeriods; t++)   {
	   if (gpdProdVol[t] < 0.01) 
	     feasibleFeatureRatio[t] = 0.0;
	   else 
	     feasibleFeatureRatio[t] = sodProdVol[t]/gpdProdVol[t];
	 }
	 witFree(gpdProdVol);
       }

       // first print the preamble of the record
       leadingString = "\"" 
	 + plannerTopLevelPart + "\",\"" 
	 + geo + "\",\"" 
	 + mfgOptionPart + "\"";


       if (printDataAsPercentage) 
	 flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, feasibleFeatureRatio);
       else
	 flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, sodProdVol);
       numRecords++;
       witFree(sodProdVol);      
       }
   }

   delete [] feasibleFeatureRatio;
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}



//  ---------------------------------------------------------------------
//  Print Substitute Usage Volumes
//  Generalized procedure: Use it for printing:
//     subUsage, mrpSubUsage, opSubUsage, mrpOpSubUsage
//  As more and more of these sub usage beasts are needed in the future, 
//  keep expanding the boolean arguments (for example isChildACapacity ...)
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printSubstituteUsageVolume(
	 WitRun * const theWitRun,
	 bool parentMustBeUserDefinedOperation,
	 bool engineMustBeImplode,
	 const int nPeriods,      
	 const int nOperations, 
	 char ** opList,        
	 std::string fileName,
	 LgFrMultiPlantHelper & mpHelper)
{
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "<one of the>  Substitute Usage files" 
		       << fileName);
  }

  // declare an exploder, but define it only if you are looking for a 
  // an MRP based sub usage info.
  LgFrSceBaseExploder * myExploder;
  if (! engineMustBeImplode)
    myExploder = this->exploder();

  // use the correct witRun for all the wit api calling ....
  WitRun *  theRightWitRun;

  if (engineMustBeImplode)
    theRightWitRun = theWitRun;
  else
    theRightWitRun = myExploder->localWitRun();

  std::string subUsageReportMode =  this->setOfParameters().getString("subUsageReportMode");
  bool isDataTransformNecessary =  ( sce::compareStringsCaseIns( subUsageReportMode, "parentprod" ) != 0 );  
  bool transformToPrimary = TRUE;
  if (isDataTransformNecessary) {
    transformToPrimary = ( sce::compareStringsCaseIns( subUsageReportMode ,"primaryCoverage" ) == 0 );  
  }

  long numRecords = 0;
  std::string parentOperationName;
  std::string pdf;

  // Loop once for each operation
  int o = 0; // Pulled out of the for below by RW2STL
  for (o=0; o<nOperations; o++ ) {
    std::string fullWitOperationName = opList[o];

    // For OpSubUsage and mrpOpSubUsage, we only consider "user-defined" operations
    if (parentMustBeUserDefinedOperation) {
      if (! mpHelper.isOperationUserDefined(theRightWitRun, 
					    fullWitOperationName, 
					    pdf, 
					    parentOperationName))
	continue;
    }

    // Otherwise, for subUsage and mrpSubUsage, we only consider normal part
    else {
      if (! (mpHelper.isOperationNormal(theRightWitRun, 
					fullWitOperationName, 
					pdf, 
					parentOperationName)
	     &&
	     mpHelper.isPartNormal(theRightWitRun,
				   fullWitOperationName))
	  )
	continue;
    }

    float * yieldRate;
    witGetOperationYieldRate(theRightWitRun, fullWitOperationName.c_str(), &yieldRate);

    
    int nBom;
    witGetOperationNBomEntries(theRightWitRun, fullWitOperationName.c_str(), &nBom);

    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nBom; j++)  {
      int nSubBom;
      witGetBomEntryNSubsBomEntries(theRightWitRun, fullWitOperationName.c_str(), j, &nSubBom);
      if (nSubBom > 0)   {
	char * child;
	witGetBomEntryConsumedPart(theRightWitRun, fullWitOperationName.c_str(), j, &child);
	std::string childPartname =  mpHelper.partname(child);
	int sj = 0; // Pulled out of the for below by RW2STL
	for (sj=0; sj<nSubBom; sj++)  {
	  char * sub;
	  float * subProdVol;
	  witGetSubsBomEntryConsumedPart(theRightWitRun, fullWitOperationName.c_str(), j, sj, &sub);
	  if (! mpHelper.isPartNormal(theRightWitRun, sub)) {
	    witFree(sub);
	    continue;
	  }
	  std::string subPartname =  mpHelper.partname(sub);
		   
	  if (engineMustBeImplode)
	    witGetSubsBomEntrySubVol(theRightWitRun, fullWitOperationName.c_str(), j, sj, &subProdVol);
	  else
	    myExploder->subUsageVol(fullWitOperationName, j, sj, &subProdVol);      


	  // transform if necessary 
	  // initialize transformed vector to subProdVol
	  LgFrTimeVecFloat transformedTimeVec( (size_t) nPeriods, subProdVol );
	  if (isDataTransformNecessary) {
	    // we'll need to get consRate, falloutRate and impactPeriods to calculate the
	    // part usage.  IF we transformToPrimary, then we get these attributes off the 
	    // bom entry.   Otherwise, if we transform to sub, we get them off the sub bom entry
	    transformedTimeVec = 0.0;
	    float * consRate;
	    float falloutRate;
	    int * impactPeriod;
	    if (transformToPrimary) {
	      // get bom consumption rate
	      witGetBomEntryConsRate(theRightWitRun, fullWitOperationName.c_str(), j, &consRate);
	      // get bom fallout rate
	      witGetBomEntryFalloutRate(theRightWitRun, fullWitOperationName.c_str(), j, &falloutRate);
	      // get bom impact periods
	      witGetBomEntryImpactPeriod(theRightWitRun, fullWitOperationName.c_str(), j, &impactPeriod);
	    }
	    else {
	      // get sub bom consumption rate
	      witGetSubsBomEntryConsRate(theRightWitRun, fullWitOperationName.c_str(), j, sj, &consRate);
	      // get sub bom fallout rate
	      witGetSubsBomEntryFalloutRate(theRightWitRun, fullWitOperationName.c_str(), j, sj, &falloutRate);
	      // get sub bom impact periods
	      witGetSubsBomEntryImpactPeriod(theRightWitRun, fullWitOperationName.c_str(), j, sj, &impactPeriod);
	    }
	    int t;
	    for(t=0; t<nPeriods; t++) {	      
	      if (impactPeriod[t] != -1 ) {
		transformedTimeVec[impactPeriod[t]] += subProdVol[t] * 
		  consRate[t]/(1.0-falloutRate);
	      }
	    }
	    
	    witFree(impactPeriod);
	    witFree(consRate);
	  }
	  else {   
	    int t;
	    for(t=0; t<nPeriods; t++) {	      
	      transformedTimeVec[t] *= yieldRate[t];
	    }
	  }

	  /*
	  outFile << "\"" << parentOperationName << "\",\"" 
		  << childPartname << "\",\""
		  << subPartname << "\",\""
		  << pdf  << "\"";
	  */
	  std::string leadingString = "\"" 
	    + parentOperationName + "\",\"" 
	    + childPartname + "\",\""
	    + subPartname + "\",\""
	    + pdf  + "\"";
	  int printIfZero = 1;
	  flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, printIfZero, leadingString, transformedTimeVec.data());
	  numRecords++;
	  witFree(sub);
	  witFree(subProdVol);
	}
	witFree(child);
      }
    }
    witFree(yieldRate);
  }

  outFile.close();
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}



//  ---------------------------------------------------------------------
//  Print Sub DB Dump File
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printSubDbDumpFile(
	 WitRun * const theWitRun,
	 bool parentMustBeUserDefinedOperation,
	 bool engineMustBeImplode,
	 const int nPeriods,      
	 const int nOperations, 
	 char ** opList,        
	 std::string fileName,
	 LgFrMultiPlantHelper & mpHelper)
{
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "<one of the>  Sub DB Dump" 
		       << fileName);
  }

  // declare an exploder, but define it only if you are looking for a 
  // an MRP based sub usage info.
  LgFrSceBaseExploder * myExploder;
  if (! engineMustBeImplode)
    myExploder = this->exploder();

  // use the correct witRun for all the wit api calling ....
  WitRun *  theRightWitRun;

  if (engineMustBeImplode)
    theRightWitRun = theWitRun;
  else
    theRightWitRun = myExploder->localWitRun();


  long numRecords = 0;
  std::string parentOperationName;
  std::string pdf;

  // Loop once for each operation
  int o = 0; 
  for (o=0; o<nOperations; o++ ) {
    std::string fullWitOperationName = opList[o];

    // For OpSubUsage and mrpOpSubUsage, we only consider "user-defined" operations
    if (parentMustBeUserDefinedOperation) {
      if (! mpHelper.isOperationUserDefined(theRightWitRun, 
					    fullWitOperationName, 
					    pdf, 
					    parentOperationName))
	continue;
    }

    // Otherwise, for subUsage and mrpSubUsage, we only consider normal part
    else {
      if (! (mpHelper.isOperationNormal(theRightWitRun, 
					fullWitOperationName, 
					pdf, 
					parentOperationName)
	     &&
	     mpHelper.isPartNormal(theRightWitRun,
				   fullWitOperationName))
	  )
	continue;
    }


    /*  NOTE: 
       Assume that: 
       1)  the operation has one and only one BOP and that the triplets: 
       2)  there is a unique subBomEntry for parent, primary, sub 
    */



    float * yieldRate;
    witGetOperationYieldRate(theRightWitRun, fullWitOperationName.c_str(), &yieldRate);
    
    int nBom;
    witGetOperationNBomEntries(theRightWitRun, fullWitOperationName.c_str(), &nBom);

    int j = 0; 
    for (j=0; j<nBom; j++)  {
      int nSubBom;
      witGetBomEntryNSubsBomEntries(theRightWitRun, fullWitOperationName.c_str(), j, &nSubBom);
      if (nSubBom > 0)   {
	char * child;
	witGetBomEntryConsumedPart(theRightWitRun, fullWitOperationName.c_str(), j, &child);
	std::string childPartname =  mpHelper.partname(child);
	int sj = 0; 
	for (sj=0; sj<nSubBom; sj++)  {
	  char * sub;
	  witGetSubsBomEntryConsumedPart(theRightWitRun, fullWitOperationName.c_str(), j, sj, &sub);
	  if (! mpHelper.isPartNormal(theRightWitRun, sub)) {
	    witFree(sub);
	    continue;
	  }
	  
	  std::string subPartname =  mpHelper.partname(sub);
	  
	  // Note that we use subProdVol rather than subVol 
	  float * subVol;
	  if (engineMustBeImplode)
	    witGetSubsBomEntrySubVol(theRightWitRun, fullWitOperationName.c_str(), j, sj, &subVol);
	  else
	    myExploder->subUsageVol(fullWitOperationName, j, sj, &subVol);      
	  
	  // Initialize all three to subVol
	  LgFrTimeVecFloat parentProd(nPeriods, 0.0);
	  LgFrTimeVecFloat primaryCov(nPeriods, 0.0);
	  LgFrTimeVecFloat subConsumed(nPeriods, 0.0);
	  
	  // Step 1: parentProd
	  //     Done!!   For now, directly use subVol (should finish me on this)
	  
	  
	  float * consRate;
	  float falloutRate;
	  int * impactPeriod;
	  float * subConsRate;
	  float subFalloutRate;
	  int * subImpactPeriod;
	  // get bom consumption rate
	  witGetBomEntryConsRate(theRightWitRun, fullWitOperationName.c_str(), j, &consRate);
	  // get bom fallout rate
	  witGetBomEntryFalloutRate(theRightWitRun, fullWitOperationName.c_str(), j, &falloutRate);
	  // get bom impact periods
	  witGetBomEntryImpactPeriod(theRightWitRun, fullWitOperationName.c_str(), j, &impactPeriod);
	  // get sub bom consumption rate
	  witGetSubsBomEntryConsRate(theRightWitRun, fullWitOperationName.c_str(), j, sj, &subConsRate);
	  // get sub bom fallout rate
	  witGetSubsBomEntryFalloutRate(theRightWitRun, fullWitOperationName.c_str(), j, sj, &subFalloutRate);
	  // get sub bom impact periods
	  witGetSubsBomEntryImpactPeriod(theRightWitRun, fullWitOperationName.c_str(), j, sj, &subImpactPeriod);
	  
	  int t;
	  for(t=0; t<nPeriods; t++) {	      
	    parentProd[t] += subVol[t] * yieldRate[t];

	    if (impactPeriod[t] != -1 ) {
	      primaryCov[impactPeriod[t]] += subVol[t] * consRate[t] / (1.0-falloutRate);
	    }
	    
	    if (subImpactPeriod[t] != -1 ) {
	      subConsumed[subImpactPeriod[t]] += subVol[t] * subConsRate[t] / (1.0-subFalloutRate);
	    }
	  }
	  
	  witFree(impactPeriod);
	  witFree(consRate);
	  witFree(subImpactPeriod);
	  witFree(subConsRate);


	  outFile << "\"" 
		  << parentOperationName << "\",\"" 
		  << childPartname << "\",\""
		  << subPartname << "\",\""
		  << pdf  << "\",\""
		  << "parentProd\"";
	  flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, parentProd.data(), 0, 15);

	  outFile << "\"" 
		  << parentOperationName << "\",\"" 
		  << childPartname << "\",\""
		  << subPartname << "\",\""
		  << pdf  << "\",\""
		  << "primaryCov\"";
	  flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, primaryCov.data(), 0, 15);

	  outFile << "\"" 
		  << parentOperationName << "\",\"" 
		  << childPartname << "\",\""
		  << subPartname << "\",\""
		  << pdf  << "\",\""
		  << "subConsumed\"";
	  flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, subConsumed.data(), 0, 15);
	  
	  numRecords++;
	  witFree(sub);
	  witFree(subVol);
	}
	witFree(child);
      }
    }
    witFree(yieldRate);
  }
  
  outFile.close();
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}








//  ---------------------------------------------------------------------
//  Print Optimization Shadow Prices 
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printShadowPrices(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 int nParts,
	 char ** partList,
	 std::string fileName,
	 LgFrMultiPlantHelper & mpHelper)
{
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Shadow Prices File" 
		       << fileName);
  }



   long numRecords = 0; 
   long numZeroVec = 0;

   float * shadowPrice;

   // Loop once for each part
   int i = 0; // Pulled out of the for below by RW2STL
   for (i=0; i<nParts; i++ ) {
#ifndef SHADOW_PRICE_DEBUG
     if (! mpHelper.isPartNormal(theWitRun, partList[i]))
       continue;
#endif
     std::string part = mpHelper.partname(partList[i]);
     std::string pdf = mpHelper.pdf(partList[i]);


     // get the shadowPrice
     witGetPartShadowPrice (theWitRun, partList[i], &shadowPrice);


#if 0
     // check to see that execVol is not Zero vector,
     // if it is then skip it.
     bool isVecNonZero = false;
     int t = 0; // Pulled out of the for below by RW2STL
     for (t=0; t<nPeriods; t++) {
       if (shadowPrice[t] > 0.0001) {
	 isVecNonZero = true;
	 break;
       }
     }
     if (! isVecNonZero) {
       numZeroVec++;
       continue;
     }
#endif
	
#ifndef SHADOW_PRICE_DEBUG 
     outFile << "\"" 
	     << part << "\",\"" 
	     << pdf << "\""; 
#else
     outFile << "\"" 
	     << partList[i] << "\""; 
#endif

     flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, shadowPrice, 0, 15);
     numRecords++;
   }

   outFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,numZeroVec,numRecords+numZeroVec); 
}



//  ---------------------------------------------------------------------
//  Print Interplant Volumes (can be used by implosion or explosion)
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printInterplantVolumesFile(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 int nOps,
	 char ** opList,
	 bool isThisMrp,
	 std::string fileName,
	 LgFrMultiPlantHelper & mpHelper)
{
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    if (isThisMrp) {
      (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
			 << "Interplant Demand File" 
			 << fileName);
    }
    else {
      (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
			 << "Interplant Orders" 
			 << fileName);
    }

  }



   long numRecords = 0; 
   long numZeroVec = 0;

   std::string destination;
   std::string destinationPdf;
   std::string source;
   std::string sourcePdf;

   LgFrTimeVecFloat execVol;

   // Loop once for each operation
   int o = 0; // Pulled out of the for below by RW2STL
   for (o=0; o<nOps; o++ ) {
     if (! mpHelper.isOperationSpecialInterplant(theWitRun, opList[o], destination, 
						 destinationPdf, source, sourcePdf))
       continue;

     std::string operationName(opList[o]);

     // get the execVol
     if (isThisMrp)
       execVol = this->interplantMrpExecVol(operationName);
     else
       execVol = this->interplantExecVol(operationName);

     // check to see that execVol is not Zero vector,
     // if it is then skip it.
     bool isVecNonZero = false;
     int t = 0; // Pulled out of the for below by RW2STL
     for (t=0; t<nPeriods; t++) {
       if (execVol[t] > 0.01) {
	 isVecNonZero = true;
	 break;
       }
     }
     if (! isVecNonZero) {
       numZeroVec++;
       continue;
     }
	 
     outFile << "\"" 
	     << destination << "\",\"" 
	     << destinationPdf << "\",\"" 
	     << source    << "\",\"" 
	     << sourcePdf << "\""; 

     flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, execVol.data(), 0, 15);
     numRecords++;
   }

   outFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,numZeroVec,numRecords+numZeroVec); 
}



//  ---------------------------------------------------------------------
//  Print AlternatePart Volumes (can be used by implosion or explosion)
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printAlternateDbFile(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 int nOps,
	 char ** opList,
	 bool isThisMrp,
	 std::string fileName,
	 LgFrMultiPlantHelper & mpHelper)
{
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    if (isThisMrp) {
      (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
			 << "Alternate Part (explode) File" 
			 << fileName);
    }
    else {
      (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
			 << "Alternate Part (implode) File" 
			 << fileName);
    }

  }



   long numRecords = 0; 
   long numZeroVec = 0;

   std::string primePart;
   std::string primePdf;
   std::string altPart;
   std::string altPdf;


   // Loop once for each operation
   int o = 0; // Pulled out of the for below by RW2STL
   for (o=0; o<nOps; o++ ) {
     if (! mpHelper.isOperationSpecialAlternate(theWitRun, opList[o], primePart, 
						    primePdf, altPart, altPdf))
       continue;
     
     std::string operationName(opList[o]);

     // get the execVol
     float * execVol;
     witGetOperationExecVol(theWitRun, opList[o], &execVol);
     
     // check to see that execVol is not Zero vector,
     // if it is then skip it.
     bool isVecNonZero = false;
     int t = 0; // Pulled out of the for below by RW2STL
     for (t=0; t<nPeriods; t++) {
       if (execVol[t] > 0.01) {
	 isVecNonZero = true;
	 break;
       }
     }
     if (! isVecNonZero) {
       numZeroVec++;
       witFree(execVol);
       continue;
     }


     LgFrTimeVecFloat altConsumed(nPeriods, 0.0);


     // should be one and only one bom entry 
     int nbom;
     witGetOperationNBomEntries(theWitRun, opList[o], &nbom);
     assert(nbom == 1);


     // should be one and only one subbom entry 
     int nSubBom;
     witGetBomEntryNSubsBomEntries(theWitRun, opList[o], 0, &nSubBom);
     assert(nSubBom == 1);

     float * consRate;
     float falloutRate;
     // get bom consumption rate
     witGetSubsBomEntryConsRate(theWitRun, opList[o], 0, 0, &consRate);
     // get bom fallout rate
     witGetSubsBomEntryFalloutRate(theWitRun, opList[o], 0, 0, &falloutRate);
	  
     for(t=0; t<nPeriods; t++) {	      
       altConsumed[t] += execVol[t] * consRate[t] / (1.0-falloutRate);
     }
     
     witFree(consRate);
     
     outFile << "\"" 
	     << primePart  << "\",\"" 
	     << primePdf   << "\",\"" 
	     << altPart    << "\",\"" 
	     << altPdf     << "\",\""
	     << "primaryCov\"";
     flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, execVol, 0, 15);

     witFree(execVol);

     outFile << "\"" 
	     << primePart  << "\",\"" 
	     << primePdf   << "\",\"" 
	     << altPart    << "\",\"" 
	     << altPdf     << "\",\""
	     << "altConsumed\"";
     flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, altConsumed.data(), 0, 15);
     
     numRecords++;
   }

   outFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,numZeroVec,numRecords+numZeroVec); 
}






  //  ---------------------------------------------------------------------
  //  sets the adjusted interplant execVols for  implosion
  //  ---------------------------------------------------------------------
void 
LgFrScenarioForSce::setInterplantOperationsAppDataForImplosion(
							       WitRun * const theWitRun,
							       const int nPeriods,                              
							       const int nOperations,
							       char ** operationList)
{
  LgFrMultiPlantHelper & mpHelper = this->multiPlantHelper();
  
  int o = 0; // Pulled out of the for below by RW2STL
  for ( o=0; o<nOperations; o++) {
    std::string operationName (operationList[o]);
    // if it's not an interplant operation, get the next operation
    std::string dPart;
    std::string dPdf;
    std::string sPart;    
    std::string sPdf;
    if ( ! mpHelper.isOperationSpecialInterplant( theWitRun,
		       operationName, dPart, dPdf, sPart, sPdf ))
      continue;

    // if we're here, we have an interplant operation.
    // there should only be one BOM hanging off the ip operation. Let's check.
    int nBoms;
    witGetOperationNBomEntries( theWitRun, operationName.c_str(), &nBoms);
    if ( nBoms != 1) {
	  std::cerr << "\nJumper Arc Error SYS268E: Interplant operations"
	       << operationName
	       << " does not have exactly one bom entry. Contact SCE Support\n" << std::endl;
	  exit (1);
    }
    
    // initialize the interplant operation's working execVol
    LgFrSceInterplantOpAppData * operationAppDataPtr = 0 ;
    witGetOperationAppData( theWitRun, operationName.c_str(),
			    (void **) &operationAppDataPtr );

    // NOTE: this assert assumes that the mrp has already created the 
    // the appdata.  
    assert ( operationAppDataPtr != 0 );

    float * execVol;
    witGetOperationExecVol(theWitRun, operationName.c_str(), &execVol);

    // now set the execVol
    LgFrTimeVecFloat workingExecVolTimeVec( (size_t) nPeriods, execVol );
    operationAppDataPtr->execVol(workingExecVolTimeVec);

    witFree( execVol ); 
  } 


  // second loop we compute them
  for ( o=0; o<nOperations; o++) {
    std::string operationName (operationList[o]);

    // if the operation isn't an interplant op, then we're not
    // interested in it
    std::string dPart;
    std::string dPdf;
    std::string sPart;
    std::string sPdf;
    if (! mpHelper.isOperationSpecialInterplant(
		       theWitRun, operationName,
		       dPart, dPdf, sPart, sPdf))
      continue;

    std::string sourcePdfPart = mpHelper.pdfPartname( sPart, sPdf );
    
    LgFrSceInterplantOpAppData * operationAppDataPtr = 0;
    witGetOperationAppData( theWitRun, operationName.c_str(),
			      (void **) &operationAppDataPtr );
    assert ( operationAppDataPtr != 0 );
    LgFrTimeVecFloat execVol = operationAppDataPtr->execVol();
    
    // the only subs on this bom should be to alternative interplant locations
    int nSubBoms;
    witGetBomEntryNSubsBomEntries( theWitRun, operationName.c_str(), 0, &nSubBoms );
    int s = 0; // Pulled out of the for below by RW2STL
    for ( s=0; s<nSubBoms; s++) {
      char * subPdfPart;
      witGetSubsBomEntryConsumedPart( theWitRun,
				      operationName.c_str(), 0, s,
				      &subPdfPart );

      // break the subPdfPart name into it's pdf and part pieces
      // to get the interplant operation between the destationPdfPart
      // and the substitute sourcePdfPart. Call it the subIpOp.
      std::string subPart = mpHelper.partname( subPdfPart );
      std::string subPdf = mpHelper.pdf( subPdfPart );
      std::string subIpOp = mpHelper.interPlantOperationName(
					 dPart, dPdf, subPart, subPdf );
      std::string destPart;
      std::string destPdf;
      std::string sourcePart;      
      std::string sourcePdf;
      if ( ! mpHelper.isOperationSpecialInterplant( theWitRun, subIpOp,
						    destPart, destPdf,
						    sourcePart, sourcePdf) ) {
	(*sceErrFacility_)("JumperArcError",MclArgList() << operationName << subPdfPart);
      }

      
      // get the execVol of the substitute interplant operation
      LgFrSceInterplantOpAppData * subIpOpAppDataPtr = 0;
      witGetOperationAppData( theWitRun, subIpOp.c_str(),
			      (void **) &subIpOpAppDataPtr );
      assert ( subIpOpAppDataPtr != 0 );
      LgFrTimeVecFloat subIpOpExecVol = subIpOpAppDataPtr->execVol();
      
      // get the substitute's usageRate
      float subUsageRate;
      witGetSubsBomEntryUsageRate( theWitRun, operationName.c_str(), 0, s, &subUsageRate );
      
      // get the sub vol
      float * subVol;
      witGetSubsBomEntrySubVol( theWitRun, operationName.c_str(), 0, s, &subVol );
      
      // adjust the execVol of the current interplant operation
      // and the execVol of the substitute
      // interplant operations
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++) {
	execVol[t] -= subVol[t];
	subIpOpExecVol[t] += subVol[t] * subUsageRate;
      } // end t
      
      // set substitute interplant operation's adjusted exec vol in
      // the substitute interplant operations appData
      subIpOpAppDataPtr->execVol(subIpOpExecVol);
      
      witFree ( subVol );
      witFree ( subPdfPart );
	
    } // end for s


    // set interplant operation's adjusted exec vol in
    // the interplant operations appData
    operationAppDataPtr->execVol(execVol);
    
  } // end for o

}



LgFrTimeVecFloat
LgFrScenarioForSce::interplantExecVol(std::string & operationName)
{
  WitRun * const theWitRun = lastProblemSolved().witRun();
  LgFrSceInterplantOpAppData * operationAppDataPtr = 0 ;
  witGetOperationAppData( theWitRun, operationName.c_str(),
			  (void **) &operationAppDataPtr );
  assert ( operationAppDataPtr != 0 );

  return operationAppDataPtr->execVol();
}

LgFrTimeVecFloat
LgFrScenarioForSce::interplantMrpExecVol(std::string & operationName)
{
  WitRun * const theWitRun = lastProblemSolved().witRun();
  LgFrSceInterplantOpAppData * operationAppDataPtr = 0 ;
  witGetOperationAppData( theWitRun, operationName.c_str(),
			  (void **) &operationAppDataPtr );
  assert ( operationAppDataPtr != 0 );

  return operationAppDataPtr->mrpExecVol();
}



// intiialize the app data's with straight execVols, and then compute them.
void
LgFrScenarioForSce::initializeInterplantOperationsAppData(
							  WitRun * const theWitRun,
							  const int nPeriods,                              
							  const int nOperations,
							  char ** operationList)
{
  LgFrSceBaseExploder * myExploder = myExploder = this->exploder();

  LgFrMultiPlantHelper & mpHelper = this->multiPlantHelper();

  // first loop we initialize them
  int o = 0; // Pulled out of the for below by RW2STL
  for ( o=0; o<nOperations; o++) {
    std::string operationName (operationList[o]);
    // if it's not an interplant operation, get the next operation
    std::string dPart;
    std::string dPdf;
    std::string sPart;    
    std::string sPdf;
    if ( ! mpHelper.isOperationSpecialInterplant( theWitRun,
		       operationName, dPart, dPdf, sPart, sPdf ))
      continue;

    // if we're here, we have an interplant operation.
    // there should only be one BOM hanging off the ip operation. Let's check.
    int nBoms;
    witGetOperationNBomEntries( theWitRun, operationName.c_str(), &nBoms);
    if ( nBoms != 1) {
	  std::cerr << "\nJumper Arc Error SYS269E: Interplant operations"
	       << operationName.c_str()
	       << " does not have exactly one bom entry. Contact SCE Support\n" << std::endl;
	  exit (1);
    }
    
    // initialize the interplant operation's working mrpExecVol
    // to be the operation's mrp exec vol.
    // Store it for further mucking
    // in the interplant operation's app data
    // Caller needs to clean up the appData.
    LgFrSceInterplantOpAppData * operationAppDataPtr = 0 ;
    witGetOperationAppData( theWitRun, operationName.c_str(),
			    (void **) &operationAppDataPtr );
    assert ( operationAppDataPtr == 0 );
    float * mrpExecVol;
    myExploder->mrpExecVol(operationName, &mrpExecVol);

    LgFrTimeVecFloat workingExecVolTimeVec( (size_t) nPeriods, mrpExecVol );
    operationAppDataPtr = new LgFrSceInterplantOpAppData (workingExecVolTimeVec);
    witSetOperationAppData( theWitRun, operationName.c_str(), (void *) operationAppDataPtr );
    witFree( mrpExecVol ); 
  } 



  // second loop we compute them
  for (o=0; o<nOperations; o++) {
    std::string operationName (operationList[o]);

    // if it's not an interplant operation, get the next operation
    std::string dPart;
    std::string dPdf;
    std::string sPart;    
    std::string sPdf;
    if ( ! mpHelper.isOperationSpecialInterplant( theWitRun,
		       operationName, dPart, dPdf, sPart, sPdf ))
      continue;

    // if we're here, we have an interplant operation.
    // Get the operation's working mrpExecVol
    
    std::string sourcePdfPart = mpHelper.pdfPartname( sPart, sPdf );
    
    LgFrSceInterplantOpAppData * operationAppDataPtr = 0;
    witGetOperationAppData( theWitRun, operationName.c_str(), (void **) &operationAppDataPtr );
    assert ( operationAppDataPtr != 0 );
    LgFrTimeVecFloat mrpExecVol = operationAppDataPtr->mrpExecVol();
    
    // the only subs on this bom should be to alternative interplant locations
    int nSubBoms;
    witGetBomEntryNSubsBomEntries( theWitRun, operationName.c_str(), 0, &nSubBoms );

    int s = 0; // Pulled out of the for below by RW2STL
    for ( s=0; s<nSubBoms; s++) {
      char * subPdfPart;
      witGetSubsBomEntryConsumedPart( theWitRun,
				      operationName.c_str(), 0, s, &subPdfPart );
      
      // break the subPdfPart name into it's pdf and part pieces
      // to get the interplant operation between the destationPdfPart
      // and the substitute sourcePdfPart. Call it the subIpOp.
      std::string subPart = mpHelper.partname( subPdfPart );
      std::string subPdf = mpHelper.pdf( subPdfPart );
      std::string subIpOp = mpHelper.interPlantOperationName(
				     dPart, dPdf, subPart, subPdf );
      std::string destPart;
      std::string destPdf;
      std::string sourcePart;      
      std::string sourcePdf;
      if ( ! mpHelper.isOperationSpecialInterplant( theWitRun, subIpOp,
						      destPart, destPdf,
						      sourcePart, sourcePdf) ) {
	std::cerr << "\nJumper Arc Error: Interplant operation "
	     <<  operationName.c_str()
	     << " has a substitute "
	     << subPdfPart
	     << " that is not valid. Contact SCE Support\n" << std::endl;
	exit (1);
	}

      // get the execVol of the substitute interplant operation, subIpOp
      LgFrSceInterplantOpAppData * subIpOpAppDataPtr = 0;
      witGetOperationAppData( theWitRun, subIpOp.c_str(),
			      (void **) &subIpOpAppDataPtr );
      assert ( subIpOpAppDataPtr != 0 );
      LgFrTimeVecFloat subIpOpMrpExecVol = subIpOpAppDataPtr->mrpExecVol();
	
      // get the substitute's usageRate
      float subUsageRate;
      witGetSubsBomEntryUsageRate( theWitRun, operationName.c_str(), 0, s, &subUsageRate );
      
      // get the sub vol
      float * subUsageVol;
      myExploder->subUsageVol(operationName, 0, s, &subUsageVol );
      
      // adjust the mrpExecVol of the current interplant operation
      // and the mrpExecVol of the substitute
      // interplant operations
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++) {
	mrpExecVol[t] -= subUsageVol[t];
	subIpOpMrpExecVol[t] += subUsageVol[t] * subUsageRate;
      } // end t

      // set substitute interplant operation's adjusted mrp exec vol in
      // the substitute interplant operations appData
      subIpOpAppDataPtr->mrpExecVol(subIpOpMrpExecVol);
      witFree ( subUsageVol );
      witFree ( subPdfPart );
      
    } // end for s


    // set interplant operation's adjusted mrp exec vol in
    // the interplant operations appData
    operationAppDataPtr->mrpExecVol(mrpExecVol);

  } // end for o

}





//  ---------------------------------------------------------------------
//  Print Operation Execution Volume (as determined by implosion)
    // Prints execVol of operations that are:
    // (1) explicitely defined by the user in the operation Definition File
    // (2) SCE-defined generated from parts defined in the part Definition File
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printOperationOrdersFile(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 std::string fileName,
	 LgFrMultiPlantHelper & mpHelper)
{
   std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
   if (outFile.fail()) {
     (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
			<< "Operation Execution Volume" 
			<< fileName);
   }

   long numRecords = 0;
   long numZeroVec = 0;
   // we're also gonna keep track of the number of unprinted zeroVectors
   // for reconciling the number of input vs output records later.

   // Loop once for each operation
   int nOperations;
   char ** operationList;
   witGetOperations(theWitRun, &nOperations, &operationList);
   int o = 0; // Pulled out of the for below by RW2STL
   for (o=0; o<nOperations; o++ ) {
     std::string thePdf;
     std::string theOperation;
     if (! mpHelper.isOperationNormal(theWitRun, operationList[o], thePdf, theOperation))
	   continue;
     // get the execVol
     float * execVol;
     witGetOperationExecVol(theWitRun, operationList[o], &execVol);
     
     // if the execVol is a Zero vector,
     // then don't print it.
     bool isVecNonZero = false;
     int t = 0; // Pulled out of the for below by RW2STL
     for (t=0; t<nPeriods; t++) {
       if (execVol[t] > 0.01) {
	 isVecNonZero = true;
	 break;
       }
     }
     if (! isVecNonZero) {
       witFree(execVol);
       numZeroVec++;
       continue;
     }
     
     outFile << "\"" 
	     << theOperation << "\",\"" 
	     << thePdf << "\""; 
     
     flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, execVol, 0, 15);
     numRecords++;
     witFree(execVol); 
   }
   
   for (o=0; o<nOperations; o++ ) 
     witFree(operationList[o]);
   witFree(operationList);
   
   outFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,numZeroVec,numRecords+numZeroVec);
}




//  ---------------------------------------------------------------------
//  Print Operation Demand File (as determined by explosion)
    // Prints mrpExecVol of operations that are:
    // (1) explicitely defined by the user in the operation Definition File
    // (2) SCE-defined generated from parts defined in the part Definition File
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printOperationDemandVolFile(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 std::string fileName,
	 LgFrMultiPlantHelper & mpHelper)
{
   std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
   if (outFile.fail()) {
     (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
			<< "Operation Demand " 
			<< fileName);
   }

   LgFrSceBaseExploder * myExploder = this->exploder();          

   long numRecords = 0;
   long numZeroVec = 0;
   // we're also gonna keep track of the number of unprinted zeroVectors
   // for reconciling the number of input vs output records later.

   // Loop once for each operation
   int nOperations;
   char ** operationList;
   witGetOperations(theWitRun, &nOperations, &operationList);
   int o = 0; // Pulled out of the for below by RW2STL
   for (o=0; o<nOperations; o++ ) {
     std::string thePdf;
     std::string theOperation;
     if (! mpHelper.isOperationNormal(theWitRun, operationList[o], thePdf, theOperation))
	   continue;
     // get the execVol
     float * mrpExecVol;
     myExploder->mrpExecVol(operationList[o], &mrpExecVol);
     
     // if the execVol is a Zero vector,
     // then don't print it.
     bool isVecNonZero = false;
     int t = 0; // Pulled out of the for below by RW2STL
     for (t=0; t<nPeriods; t++) {
       if (mrpExecVol[t] > 0.01) {
	 isVecNonZero = true;
	 break;
       }
     }
     if (! isVecNonZero) {
       witFree(mrpExecVol);
       numZeroVec++;
       continue;
     }
     
     outFile << "\"" 
	     << theOperation << "\",\"" 
	     << thePdf << "\""; 
     
     flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, mrpExecVol, 0, 15);
     numRecords++;
     witFree(mrpExecVol); 
   }
   
   for (o=0; o<nOperations; o++ ) 
     witFree(operationList[o]);
   witFree(operationList);
   
   outFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,numZeroVec,numRecords+numZeroVec);
}




//  ---------------------------------------------------------------------
//  Print Backlog schedule         
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printBacklogSchedule(
					       WitRun * const theWitRun,   // WIT environment   
					       const int nPeriods,         // Number of periods 
					       const int nParts,           // Number of parts   
					       char ** partList,           // List of part names
					       std::string fileName,
					       LgFrCalendar & theCal,
					       LgFrMultiPlantHelper & mpHelper)
{
  std::string partName;

  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Backlog Response" 
		       << fileName);
  }

  long numRecords = 0;

  float * shipVol;
  float * demandVol;
  float blog;
  int i, t;

  int nDemands;
  char ** dList;
  int j;


  // Loop once for each part.  
  for ( i=0; i<nParts; i++ ) {
    std::string partname = mpHelper.partname(partList[i]);
    std::string plannerPart;
    std::string geo;
    if ( mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun, partList[i],
						    plannerPart, geo)) {
      witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);
      for (j =0; j<nDemands; j++)  {
	outFile << "\"" << partname << "\",\"" << dList[j] << "\"";
	
	// shipVol
	witGetDemandShipVol(theWitRun, partList[i], dList[j], &shipVol);
	// demandVol
	witGetDemandDemandVol(theWitRun, partList[i], dList[j], &demandVol);
	// backlogVol
	blog = 0.0;
	for (t=0; t<nPeriods; t++)    {
	  blog += (demandVol[t] - shipVol[t]);
	  shipVol[t] = blog;
	}
	
	flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, shipVol, 0, 15);
	witFree(shipVol);
	witFree(demandVol);
	witFree(dList[j]);
	numRecords++;
      }
      witFree(dList);
    }
  }  outFile.close();
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords); 
}


//  ---------------------------------------------------------------------
//  Print Supply Response File 
// SCE 6.1: this method can be called with integer flag tinpIndpOrBoth set to 1,2, or 3
// tinpIndpOrBoth Flag => 1 means TINP only, 2 means INDP only, 3 means both
//    with multiAttribute demands, use 3 which means TINP and INDP are printed together in one file (6.1)
//    for sce5 back compatibility, call this method twice, once for TINP and the other for INDP
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printSupplyPlanSoln(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 int  tinpIndpOrBoth,        // Flag => 1 means TINP only, 2 means INDP only, 3 means both
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper)
{
  std::string partName;
  
  std::ofstream outFile(fileName.c_str());
  if (outFile.fail()) 
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() << "Supply Response" << fileName);


  long numRecords = 0;
  float * shipVol;
  int i;
  int nDemands;
  char ** dList;
  int j;
  
  // for both indp and tinp/gpd outputs, we will always print zero records for vectors, never print zero's for dated
  bool useDatedDemands = this->setOfParameters().getBool("useDatedDemands");  
  bool printIfZero;
  if (useDatedDemands) 
    printIfZero = false;
  else
    printIfZero = true;

  // Loop once for each part.  
  for ( i=0; i<nParts; i++ ) {
    std::string plannerPart;
    std::string geo;


    // TINP (ie, GPD) demands
    if ( (tinpIndpOrBoth == 1) || (tinpIndpOrBoth == 3))   {
      if ( mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun, partList[i], plannerPart, geo)) {
	std::string partname = mpHelper.partname(partList[i]);
	witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);
	for (j =0; j<nDemands; j++)  {
	  witGetDemandShipVol(theWitRun, partList[i], dList[j], &shipVol);
	  flatFileMgr_.writeGpdDemandRecord(outFile, mpHelper, theCal, partname, dList[j], nPeriods, shipVol, printIfZero, 0, 15);
	  witFree(shipVol); 
	  witFree(dList[j]);
	  numRecords++;
	}
	witFree(dList);
      }
    }

    // INDP demands
    if ( (tinpIndpOrBoth == 2) || (tinpIndpOrBoth == 3))   {   
      if ( mpHelper.isPartNormal(theWitRun, partList[i])) {
	std::string partname = mpHelper.partname(partList[i]);
	std::string pdf = mpHelper.pdf(partList[i]);
	witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);
	for (j =0; j<nDemands; j++)  {
	  witGetDemandShipVol(theWitRun, partList[i], dList[j], &shipVol);
	  
#ifdef ENABLE_NEGATIVE_DEMANDS
	  // adjust shipVol for negative demand
	  LgFrSceDemandAppData * demandAppDataPtr;
	  witGetDemandAppData(theWitRun, partList[i], dList[j], (void **) &demandAppDataPtr);
	  if (demandAppDataPtr->doesDemandHaveNegativeValues()) {
	    LgFrTimeVecFloat demandVolTVF = demandAppDataPtr->demandVol();
	    int t = 0;
	    for (t=0; t<nPeriods; t++) {
	      if (demandVolTVF[t] < 0) {
		shipVol[t] += demandVolTVF[t];
	      }
	    }
	  }
#endif
	  

	  flatFileMgr_.writeDemandRecordINDP(outFile, mpHelper, theCal, partname, pdf, dList[j], nPeriods, shipVol, printIfZero, 0, 15);
	  witFree(shipVol); 
	  witFree(dList[j]);
	  numRecords++;
	}
	witFree(dList);
      }
    }
  }
    
  outFile.close();
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}


// this is the old way for backup
#ifdef SCE5 
void 
LgFrScenarioForSce::printSupplyPlanSoln(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper)
{
  std::string partName;
  
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Supply Response" 
		       << fileName);
  }

  long numRecords = 0;
  
  float * shipVol;
  int i;
  
  int nDemands;
  char ** dList;
  int j;
  
  
  // Loop once for each part.  
  for ( i=0; i<nParts; i++ ) {
    std::string plannerPart;
    std::string geo;
    if ( mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun, partList[i],
						    plannerPart, geo)) {
      std::string partname = mpHelper.partname(partList[i]);
      witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);
      for (j =0; j<nDemands; j++)  {
	outFile << "\"" << partname << "\",\"" << dList[j] << "\"";
	
	// shipVol
	witGetDemandShipVol(theWitRun, partList[i], dList[j], &shipVol);
	flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, shipVol, 0, 15);
	witFree(shipVol); 
	witFree(dList[j]);
	numRecords++;
      }
      witFree(dList);
    }
  }    
  outFile.close();
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}
#endif



// SCE5

//  ---------------------------------------------------------------------
//  Print Part Shipment Information         
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printIndepSupplyPlanSoln(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper)
{
  std::string partName;
  
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Independent Supply Response" 
		       << fileName);
  }

  long numRecords = 0;
  
  float * shipVol;
  int i;
  
  int nDemands;
  char ** dList;
  int j;
  
  
  // Loop once for each part.  
  for ( i=0; i<nParts; i++ ) {
    std::string plannerPart;
    std::string geo;
    if ( mpHelper.isPartNormal(theWitRun, partList[i])) {
      std::string partname = mpHelper.partname(partList[i]);
      std::string pdf = mpHelper.pdf(partList[i]);
      witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);
      for (j =0; j<nDemands; j++)  {
	
	// shipVol
	witGetDemandShipVol(theWitRun, partList[i], dList[j], &shipVol);

#ifdef ENABLE_NEGATIVE_DEMANDS
	// adjust shipVOl for negative demand
	LgFrSceDemandAppData * demandAppDataPtr;
	witGetDemandAppData(theWitRun, partList[i], dList[j], (void **) &demandAppDataPtr);
	if (demandAppDataPtr->doesDemandHaveNegativeValues()) {
	  LgFrTimeVecFloat demandVolTVF = demandAppDataPtr->demandVol();
	  int t = 0; 
	  for (t=0; t<nPeriods; t++) {
	    if (demandVolTVF[t] < 0) {
	      shipVol[t] += demandVolTVF[t];
	    }
	  }
	}
#endif

	// for both indp and tinp/gpd outputs, we will always print zero records for vectors, never print zero's for dated
	bool useDatedDemands = this->setOfParameters().getBool("useDatedDemands");  
	bool printIfZero;
	if (useDatedDemands) 
	  printIfZero = false;
	else
	  printIfZero = true;

	flatFileMgr_.writeDemandRecord(outFile, mpHelper, theCal, partname, pdf, dList[j], nPeriods, shipVol, printIfZero, 0, 15);


	witFree(shipVol); 
	witFree(dList[j]);
	numRecords++;
      }
      witFree(dList);
    }
  }    
  outFile.close();
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}




//  ---------------------------------------------------------------------
//  Print Part Shipment Information in human readable format        
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printHumanReadableSupplyPlanSoln(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper)
{
  std::string partName;

  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Formatted Output" 
		       << fileName);
  }

   long numRecords = 0;
   float * witVector;
   float * witVector1;
   float   blog;
   int i, t;
   std::string demandName("WW");
   int nDemands;
   char ** dList;
   int j;

// print some dates

   outFile << "             ";
   for (t=0; t<nPeriods; t++)  {
       LgFrDatetime thisDate(theCal.periodStart(t));
       std::string theDate("    " + thisDate.getDateAsString() + "   ");
       outFile << theDate;
   }
   // Loop once for each part.  
   for ( i=0; i<nParts; i++ ) {
       std::string parti(partList[i]);
       std::string partname = mpHelper.partname(parti);
       std::string plannerPart;
       std::string geo;
       if ( mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun, partList[i],
						       plannerPart, geo)) {
	 witGetPartDemands(theWitRun, parti.c_str(), &nDemands, &dList);
	 for (j =0; j<nDemands; j++)  {
	   
	   outFile << "\n" << partname << " - " << dList[j] << ":\n";
	   
// demand Vol
	   outFile << "    Demand";
	   witGetDemandDemandVol(theWitRun, parti.data(), dList[j], &witVector);
	   flatFileMgr_.writeFloatVector(outFile, nPeriods, witVector, 0, 15);
	   numRecords++;

// shipVol
	   outFile << "    Supply";
	   witGetDemandShipVol(theWitRun, parti.c_str(), dList[j], &witVector1);
	   flatFileMgr_.writeFloatVector(outFile, nPeriods, witVector1, 0, 15);
	   numRecords++;

// backlogVol
	   blog = 0.0;
	   outFile << "     Delta";
	   for (t=0; t<nPeriods; t++)    {
	     blog += (witVector1[t] - witVector[t]);
	     witVector[t] = blog;
	   }
	   flatFileMgr_.writeFloatVector(outFile, nPeriods, witVector, 0, 15);
	   numRecords++;

	   witFree(witVector); 
	   
	   witFree(witVector1); 
	   witFree(dList[j]);
	   
	 }
	 witFree(dList);
       }
   }
   outFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}

//  ---------------------------------------------------------------------
//  Print GEO Supply By PDF  Information         
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printGeoSourceDebugFile(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper)
{
   std::string geo;
   std::string plannerPart;

   std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Geo Source Debug" 
		       << fileName);
  }

   long numRecords = 0;

   int i;
   std::string * sourceList = new std::string[nPeriods];


   // Loop once for each part.  
   for ( i=0; i<nParts; i++ ) {
     // only look at geoPlannerDemandParts
     if (! mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun,
						      partList[i], plannerPart, geo))
       continue;

     int periodWithNoSource = determineGeoToPdfSourceVector(theWitRun, 
							    mpHelper,
							    partList[i],
							    sourceList);
     

     outFile << "\"" << plannerPart << "\",\"" << geo << "\"";
     int t = 0; // Pulled out of the for below by RW2STL
     for (t=0; t<nPeriods; t++)
       outFile << ",\"" << sourceList[t]     << "\"";
     outFile << "\n";
     numRecords++;

     if (periodWithNoSource > -1) {
       // error
       (*sceErrFacility_)("GeoDemandNotSourced",MclArgList() << geo << plannerPart << periodWithNoSource);
     }
   }
   delete [] sourceList;

   outFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords); 
}

//  ---------------------------------------------------------------------
//  Print Part Build Schedule 
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printBuildSchedule(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper)
{
   std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Build Schedule" 
		       << fileName);
  }

   long numRecords = 0; 
   float * execVol;

   // Loop once for each part.  
   int i = 0; // Pulled out of the for below by RW2STL
   for (i=0; i<nParts; i++ ) {
     if (! mpHelper.isPartBuildable(theWitRun, partList[i]))
       continue;
     std::string partname = mpHelper.partname(partList[i]);
     std::string pdf = mpHelper.pdf(partList[i]);
     outFile << "\"" 
	     << partname << "\",\"" 
	     << pdf << "\"";

     // note: the opeation is guaranteed to exist
     // by definition of isPartBuildable
     witGetOperationExecVol(theWitRun, partList[i], &execVol);
     flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, execVol, 0, 15);
     witFree(execVol); 
     numRecords++;
   }

   outFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);

}



//  ---------------------------------------------------------------------
//  Print critical parts list
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printDebugCriticalList(
	 WitRun * const theWitRun,   // WIT environment
	 std::string engine,            // rollover handled differently
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper,
	 LgFrSceCritList & theCritList)
{
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Critical Parts List" 
		       << fileName);
  }

  bool useDatedDemands = this->setOfParameters().getBool("useDatedDemands");

  long numRecords = 0;
  
  // Loop once for each part.  
  int i = 0; 
  for (i=0; i<theCritList.getLength(); i++ ) {
    std::string part = theCritList.getPart(i);
    int pd = theCritList.getPeriod(i);
    if (! mpHelper.isPartNormal(theWitRun, part))   {
      outFile << "\"" 
	      << part << "\",\"-----\",";
      
    }
    else {
      std::string partname = mpHelper.partname(part);
      std::string pdf = mpHelper.pdf(part);
      outFile << "\"" 
	      << partname << "\",\"" 
	      << pdf << "\",";
    }

    if (useDatedDemands) {
      outFile << theCal.yyyymmdd(theCal.periodStart(pd)) << ","
	      << i << "\n";
    }
    else {
      outFile << pd << ","
	      << i << "\n";
    }
    numRecords++;
  }
  outFile.close();
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords); 
}




//  ---------------------------------------------------------------------
//  Print critical parts list
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printCriticalList(
	 WitRun * const theWitRun,   // WIT environment
	 std::string engine,            // rollover handled differently
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper,
	 LgFrSceCritList & theCritList)
{
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Critical Parts List" 
		       << fileName);
  }

  bool useDatedDemands = this->setOfParameters().getBool("useDatedDemands");

  long numRecords = 0;
  
  // Loop once for each part.  
  int i = 0; 
  for (i=0; i<theCritList.getLength(); i++ ) {
    std::string part = theCritList.getPart(i);
    int pd = theCritList.getPeriod(i);
    if (! mpHelper.isPartNormal(theWitRun, part))
      continue;
    // comeback

    std::string partname = mpHelper.partname(part);
    std::string pdf = mpHelper.pdf(part);
    outFile << "\"" 
	    << partname << "\",\"" 
	    << pdf << "\",";

    if (useDatedDemands) {
      outFile << theCal.yyyymmdd(theCal.periodStart(pd)) << ","
	      << i << "\n";
    }
    else {
      outFile << pd << ","
	      << i << "\n";
    }
    numRecords++;
  }
  outFile.close();
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords); 
}






//  ---------------------------------------------------------------------
//  Print pegged critical parts list
//  --------------------------------------------------------------------- 
// SCE 6.1 Version: prints indp and tinp to a common file.   Must also derive GPD from Special Feature Demands.
// Note we could report a different attribute ??? For pegging purposes (???)   Especially if coming from a feature.
// Why not give the feature number too ... can put it in partClass (?)
void 
LgFrScenarioForSce::printPeggedCriticalList(
	 WitRun * const theWitRun,   // WIT environment
	 std::string engine,            // only works for heuristic allocation
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper)
{
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Pegged Critical Parts List" 
		       << fileName);
  }


#ifdef CRIT_PEG_DEBUG
  std::ofstream outFilePCDump("pegCritDump.out");
  if (outFilePCDump.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Pegged Critical Parts List DEBUG_DUMP" 
		       << "pegCritDump.out");
  }
#endif



  long numRecords = 0;

  // Loop once for each part.  
  
  // PEG_CRIT_LIST
  int      lenLists;
  char **  critPartNameList;
  int *    critPerList;
  char **  demPartNameList;
  char **  demandNameList;
  int *    shipPerList;
  
  witGetPgdCritList (theWitRun,
		     &lenLists,
		     &critPartNameList,
		     &critPerList,
		     &demPartNameList,
		     &demandNameList,
		     &shipPerList);

  bool useDatedDemands = this->setOfParameters().getBool("useDatedDemands");
  std::string plannerPart;

  int i = 0; 
  for (i=0; i<lenLists; i++ ) {
    
    if (! mpHelper.isPartNormal(theWitRun, critPartNameList[i])) {
#ifdef CRIT_PEG_DEBUG
      outFilePCDump << (int) 99 << ","
		    << "\"" << demPartNameList[i] << "\","
		    << "\"" << demandNameList[i] << "\","
		    << "\"" << critPartNameList[i] << "\","
		    << shipPerList[i] << ","
		    << critPerList[i] << ","
		    << i << "\n";
#endif
      witFree(critPartNameList[i]);
      witFree(demPartNameList[i]);
      witFree(demandNameList[i]);
      continue;
    }

    std::string critPartName = mpHelper.partname(critPartNameList[i]);
    std::string critPdf = mpHelper.pdf(critPartNameList[i]);


    // figure out how to write demand name
    // Use LeadingString functions from mpHelper
    
    std::string witPartName = demPartNameList[i];
    std::string witDemandName = demandNameList[i]; 
    std::string leadingString; 
    
    // Case 1: normal independend demand
    if (mpHelper.isPartNormal(theWitRun, demPartNameList[i])) {
      std::string demandedPartName = mpHelper.partname(demPartNameList[i]);
      std::string demandedPdf      = mpHelper.pdf(demPartNameList[i]);
      mpHelper.populateLeadingStringINDP(demandedPartName, demandedPdf, witDemandName, leadingString);
      if (useDatedDemands)  {
	std::string demandDate = theCal.yyyymmdd(theCal.periodStart(shipPerList[i]));
	leadingString += "," + demandDate;
      }
    }
    
    // Case 2: normal GPD  demand
    else if (mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun, 
							demPartNameList[i],
							plannerPart, 
							witDemandName)) {
      mpHelper.populateLeadingStringGPD(plannerPart, witDemandName, leadingString);
    }
    
    // Case 3: special Feature Demand
    else {
      std::string featurePart, plannerPart, demandedPdf;
      if (mpHelper.isOperationSpecialFeature(theWitRun, 
					     demPartNameList[i],
					     witDemandName,    
					     plannerPart,    
					     demandedPdf,    
					     featurePart)) {

	mpHelper.populateLeadingStringGPD(plannerPart, witDemandName, leadingString);	
      }
    }


    // Regardless of demand type, append on the critPart and critPdf to leadingString
    leadingString +=  ",\"" 
      + critPartName + "\",\"" 
      + critPdf + "\"";

    if (useDatedDemands)  {
      // convert pds to dates
      //      shipPerList[i];  critPerList[i];
      std::string demandDate = theCal.yyyymmdd(theCal.periodStart(shipPerList[i]));
      std::string critDate = theCal.yyyymmdd(theCal.periodStart(critPerList[i]));
      outFile << leadingString << "," << demandDate << "," << critDate << "," << i << "\n";
    }
    else {
      outFile << leadingString << "," << shipPerList[i] << "," << critPerList[i] << "," << i << "\n";
    }



#ifdef CRIT_PEG_DEBUG
      outFilePCDump << isItBox << ","
		    << "\"" << demPartNameList[i] << "\","
		    << "\"" << demandNameList[i] << "\","
		    << "\"" << critPartNameList[i] << "\","
		    << shipPerList[i] << ","
		    << critPerList[i] << ","
		    << i << "\n";
#endif

    witFree(critPartNameList[i]);
    witFree(demPartNameList[i]);
    witFree(demandNameList[i]);
  }

  witFree(critPartNameList);
  witFree(critPerList);
  witFree(demPartNameList);
  witFree(demandNameList);
  witFree(shipPerList);

  outFile.close();

#ifdef CRIT_PEG_DEBUG
  outFilePCDump.close();
#endif
  
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName, numRecords,000,numRecords); 
}






//  ---------------------------------------------------------------------
//  Print pegged critical parts list
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printPeggedCriticalList(
	 WitRun * const theWitRun,   // WIT environment
	 std::string engine,            // only works for heuristic allocation
	 std::string fileNameBox,
	 std::string fileNameIndp,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper)
{
  std::ofstream outFileBox(fileNameBox.c_str());//, ios::out, 0666);
  if (outFileBox.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Pegged Critical Parts List" 
		       << fileNameBox);
  }

  std::ofstream outFileIndp(fileNameIndp.c_str());//, ios::out, 0666);
  if (outFileIndp.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Independent Pegged Critical Parts List" 
		       << fileNameIndp);
  }

#ifdef CRIT_PEG_DEBUG
  std::ofstream outFilePCDump("pegCritDump.out");
  if (outFilePCDump.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Pegged Critical Parts List DEBUG_DUMP" 
		       << "pegCritDump.out");
  }
#endif



  long numRecordsBox = 0;
  long numRecordsIndp = 0;

  // Loop once for each part.  
  
  // PEG_CRIT_LIST
  int      lenLists;
  char **  critPartNameList;
  int *    critPerList;
  char **  demPartNameList;
  char **  demandNameList;
  int *    shipPerList;
  
  witGetPgdCritList (theWitRun,
		     &lenLists,
		     &critPartNameList,
		     &critPerList,
		     &demPartNameList,
		     &demandNameList,
		     &shipPerList);

  int i = 0; 
  for (i=0; i<lenLists; i++ ) {
    
    if (! mpHelper.isPartNormal(theWitRun, critPartNameList[i])) {
#ifdef CRIT_PEG_DEBUG
      outFilePCDump << (int) 99 << ","
		    << "\"" << demPartNameList[i] << "\","
		    << "\"" << demandNameList[i] << "\","
		    << "\"" << critPartNameList[i] << "\","
		    << shipPerList[i] << ","
		    << critPerList[i] << ","
		    << i << "\n";
#endif
      witFree(critPartNameList[i]);
      witFree(demPartNameList[i]);
      witFree(demandNameList[i]);
      continue;
    }

    std::string critpartname = mpHelper.partname(critPartNameList[i]);
    std::string critpdf = mpHelper.pdf(critPartNameList[i]);


    // figure out how to write demand name
    // if it's normal independend demand (non-GPD, non-special), then print part, pdf, demandID
    // if it's GPD, then print plannerpart, demandSourcePdf, geo
    // if it's special feature part, then map it to a GPD and print plannerpart, demandSourcePdf, geo


    std::string demandedPartName = demPartNameList[i];
    std::string demandedPdf = "";
    std::string demandedCust = demandNameList[i];

    bool isItBox = 0;

    // Case 1: normal independend demand
    if (mpHelper.isPartNormal(theWitRun, demPartNameList[i])) {
      demandedPartName = mpHelper.partname(demPartNameList[i]);
      demandedPdf      = mpHelper.pdf(demPartNameList[i]);
      isItBox = 0;
    }

    // Case 2: normal GPD  demand
    else if (mpHelper.isPartSpecialGeoPlannerDemandPart(theWitRun, 
							demPartNameList[i],
							demandedPartName, 
							demandedCust)) {
      int late;
      float offset;
      std::string fName="internal operation: printPeggedCriticalList";
      std::string dLine="noLine";
      long lineNo = 0;

      std::string demandedPartPdf = mpHelper.demandSource( theWitRun, 
							   demandedPartName,
							   demandedCust,
							   shipPerList[i],
							   late,
							   offset,
							   fName, 
							   dLine, 
							   lineNo, 
							   OPTIONAL_WITH_MESSAGE);
      demandedPartName = mpHelper.partname(demandedPartPdf);
      demandedPdf      = mpHelper.pdf(demandedPartPdf);
      isItBox = 1;
    }

    // Case 3: special Feature Demand
    else {
      std::string featurePart;
      if (mpHelper.isOperationSpecialFeature(theWitRun, 
					     demPartNameList[i],
					     demandedCust,    
					     demandedPartName,    
					     demandedPdf,    
					     featurePart)) {
	
	//	demandedPartName = demPartNameList[i];
	//	demandedPdf = "";
	//	demandedCust = demandNameList[i];
	//	demandedPdf = "";
	//	demandedCust = demandNameList[i];
	isItBox = 1;
      }
    }

    if (isItBox) {
      outFileBox << "\"" << demandedPartName << "\","
		 << "\"" << demandedCust << "\","
		 << "\"" << critpartname << "\","		 << "\"" << critpdf << "\","
		 << shipPerList[i] << ","
		 << critPerList[i] << ","
		 << i << "\n";
      numRecordsBox++;
    }
    else {
      // do it in flatfileMgr!

      flatFileMgr_.writePegCritListRecord(outFileIndp, mpHelper, theCal,
					  demandedPartName,
					  demandedPdf,
					  demandedCust,
					  critpartname,
					  critpdf,
					  shipPerList[i], 
					  critPerList[i], 
					  i);

      //      outFileIndp << "\"" << demandedPartName << "\","
      //	  << "\"" << demandedPdf << "\","
      //	  << "\"" << demandedCust << "\","
      //	  << "\"" << critpartname << "\","
      //	  << "\"" << critpdf << "\","
      //	  << shipPerList[i] << ","
      //	  << critPerList[i] << ","
      //	  << i << "\n";
      numRecordsIndp++;
    }


#ifdef CRIT_PEG_DEBUG
      outFilePCDump << isItBox << ","
		    << "\"" << demPartNameList[i] << "\","
		    << "\"" << demandNameList[i] << "\","
		    << "\"" << critPartNameList[i] << "\","
		    << shipPerList[i] << ","
		    << critPerList[i] << ","
		    << i << "\n";
#endif

    witFree(critPartNameList[i]);
    witFree(demPartNameList[i]);
    witFree(demandNameList[i]);
  }

  witFree(critPartNameList);
  witFree(critPerList);
  witFree(demPartNameList);
  witFree(demandNameList);
  witFree(shipPerList);

  outFileBox.close();
  outFileIndp.close();
#ifdef CRIT_PEG_DEBUG
  outFilePCDump.close();
#endif
  
  flatFileMgr_.sceIOLogManager().printOutputLog(fileNameBox, numRecordsBox,000,numRecordsBox); 
  flatFileMgr_.sceIOLogManager().printOutputLog(fileNameIndp, numRecordsIndp,000,numRecordsIndp); 
}






void
LgFrScenarioForSce::printModelStructureOutputs()
{
    int nPeriods;                         //  Number of periods in model  
    int nParts;                           //  Number of parts in model     
    char ** partList;                     //  List of parts in model       
    int i;                                //  Loop index                  
    //    int nOperations;
    //    char ** opList;                     //  List of parts in model       


    WitRun * const theWitRun = lastProblemSolved().witRun();
    LgFrCalendar & theCal = calendar();
    LgFrMultiPlantHelper & mpHelper = multiPlantHelper();
      
    witGetParts( theWitRun, &nParts, &partList );
    //    witGetOperations( theWitRun, &nOperations, &opList );
    nPeriods = theCal.nPeriods();

    LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();

   // Lowest Level Non Pcf File: optional
   if (setOfParameters.getBool("printLowestLevelNonPcfFile"))  {
     std::string lowestLevelNonPcfFileName =
       setOfParameters.getString("lowestLevelNonPcfFileName");
     printLowestLevelNonPcfParts(theWitRun, mpHelper, nPeriods, nParts, partList,
				 lowestLevelNonPcfFileName);
   }


   // PCF Bogons File: optional
   if (setOfParameters.getBool("printPcfBogonsFile"))  {
     std::string pcfBogonsFileName =
       setOfParameters.getString("pcfBogonsFileName");
     printBogonParts(theWitRun, mpHelper, nPeriods, nParts, partList,
		     pcfBogonsFileName);
   }


   // *************************************    
   // Calculate Value Add Costs 
   // *************************************
   if (setOfParameters.getBool("calculateVac"))  {
     std::cout << "Calculating Value Added Costs  ..." << std::endl;                
     std::string vacFileName = setOfParameters.getString("vacFileName");
     std::string compCostFileName = setOfParameters.getString("compCostFileName");
     calculateAndPrintValueAddedCosts(theWitRun, mpHelper, nPeriods, nParts, partList,
				      vacFileName, compCostFileName);
   }
   






   // Cleanup memory
   for ( i=0; i<nParts; i++) 
      witFree(partList[i]);
   witFree( partList );

   //   for (int o=0; o<nOperations; o++) 
   //      witFree(opList[o]);
   //   witFree( opList );
}



void
LgFrScenarioForSce::runImplosion(
				 LgFrScenarioForSceSmartPointer scenSceSP
				 )
{
  // Set up the critical list data
  LgFrSceCritList & theCritList = sceCritList();
  WitRun * theWitRun = lastProblemSolved().witRun();
  LgFrCalendar * theCal = &calendar();
  LgFrMultiPlantHelper * mpHelper = &multiPlantHelper();
  const std::string & engine = getEngine();

  LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();
  bool doPegging = setOfParameters.getBool("doPegging");
   
  if (engine == "heuristic" || engine == "rollover" ) {

    LgFrSceHeurEngineMgr heurEngineMgr(
				       scenSceSP,
				       theWitRun, 
				       theCal,
				       mpHelper,
				       engine);
    
    heurEngineMgr.implode(theCritList);
    if (doPegging)
      printPIPData(theWitRun,0);
  }

  if (engine == "heuralloc" ) {

    std::string usageName("implosion");
    LgFrSceAllocationEngineMgr allocEngineMgr(
					 scenSceSP,
					 theWitRun, 
					 mpHelper,
					 usageName);
    allocEngineMgr.implode(theCritList);

    if (doPegging)
      printPIPData(theWitRun,0);
  }
     
   // ===================================
   // === Run the Optimization engine ===
   // ===================================
  if (engine == "optimization" ) {
    LgFrSceOptEngineMgr optEngineMgr(theWitRun,
				     theCal,
				     mpHelper,
				     scenSceSP);
     
     optEngineMgr.implode(theCritList);
     if (doPegging)
       printPIPData(theWitRun,0);
  }
}

void
LgFrScenarioForSce::machineFss()
{
  LgFrSceFssMgr fssMgr(lastProblemSolved().witRun(), 
		       &calendar(),
		       &multiPlantHelper(),
		       this);
  fssMgr.machineFss();
}



void
LgFrScenarioForSce::generalSetFss()
{
  LgFrSceFssMgr fssMgr(lastProblemSolved().witRun(), 
		       &calendar(),
		       &multiPlantHelper(),
		       this);

  fssMgr.generalSetFss();

}


void
LgFrScenarioForSce::singletonFss()
{
  LgFrSceFssMgr fssMgr(lastProblemSolved().witRun(), 
		       &calendar(),
		       &multiPlantHelper(),
		       this);
  fssMgr.singletonFss();    
}



// ===============================
// ====== Capacity Usage Schedule ====
// ===============================
void
LgFrScenarioForSce::printCapUsageVolume(WitRun * const theWitRun, bool isItMrp)
{
  LgFrMultiPlantHelper & mpHelper = multiPlantHelper();
  LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();

  std::string  capacityUsageFileName = setOfParameters.getString("capacityUsageFileName");
  int goodRec = 0;

  if (isItMrp) {
    capacityUsageFileName = setOfParameters.getString("mrpCapacityUsageFileName");
  }

  std::string leadingString;

  int nPeriods;

  if (isItMrp) 
    std::cout << "... printing MRP capacity Usage data ..." << std::endl;    
  else
    std::cout << "... printing capacity Usage data ..." << std::endl;    

    
  witGetNPeriods(theWitRun, &nPeriods);
  int nParts;
  char  **partList;
  witGetParts(theWitRun, &nParts, &partList);

  char * capName;


  std::ofstream cUsFile;
  
  // Now open the capUsage OUTPUT file
  cUsFile.open(capacityUsageFileName.c_str(), std::ofstream::out);
  if (cUsFile.fail()) {
    std::string fileErrorString("Capacity Usage File");
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << fileErrorString 
		       << capacityUsageFileName);
  }
  



  // Loop over parts to find capacity
  int i = 0;
  for(i=0; i<nParts; i++){
    capName = partList[i];

    if (! mpHelper.isPartNormalCapacity(theWitRun, capName))  {
      witFree(partList[i]);
      continue;
    }
    
    int nCbom;
    witGetPartNConsumingBomEntries(theWitRun, capName, &nCbom);
    int b,c;
    char * operation;
    for (c=0; c<nCbom; c++) {
      witGetPartConsumingBomEntry(theWitRun, capName, c, &operation, &b);

      float * execVol;
      float * consRate;
      //      float * yieldRate;
      int * impactPeriod;
      float falloutRate;
      witGetOperationExecVol ( theWitRun, operation,&execVol );
      //      witGetOperationYieldRate ( theWitRun, operation,&yieldRate );
      witGetBomEntryConsRate ( theWitRun, operation, b, &consRate );
      witGetBomEntryFalloutRate ( theWitRun, operation, b, &falloutRate );
      witGetBomEntryImpactPeriod ( theWitRun, operation, b, &impactPeriod );
      LgFrTimeVecFloat capUsageVol(nPeriods, 0.0);
      int execPeriod;
      for (execPeriod=0; execPeriod<nPeriods; execPeriod++) {
	int consPeriod = impactPeriod[execPeriod];
	if ( consPeriod == -1 ) 
	  continue;

	capUsageVol[consPeriod] += execVol[execPeriod] * consRate[execPeriod]/(1.0-falloutRate);
      }

      leadingString = "\""
	+ mpHelper.partname(operation) + "\",\""
	+ mpHelper.partname(capName) + "\",\""
	+ mpHelper.pdf(capName) + "\"";
      flatFileMgr_.writeFloatVecToStream(this->calendar(), cUsFile, nPeriods, 0, leadingString, capUsageVol.data());


      goodRec++;
      
      witFree(execVol);
      witFree(consRate);
      witFree(impactPeriod);
      //      witFree(yieldRate);
      witFree(operation);
    }
    witFree(partList[i]);
  }
  witFree(partList);

  cUsFile.close();
}
    






// ===============================
// ====== PIP  Utilitiies ====
// ===============================
void
LgFrScenarioForSce::loadPIPDataIntoAppData(WitRun * const theWitRun)
{
  int nPeriods;
  char  *partName, *demandName;

  LgFrMultiPlantHelper & mpHelper = multiPlantHelper();

  std::cout << "... collecting PIP information ..." << std::endl;    
    
  witGetNPeriods(theWitRun, &nPeriods);
  int nParts;
  char  **partList;
  witGetParts(theWitRun, &nParts, &partList);

  std::string pp, geo;
  std::string fullWitGPDPartName, fullWitGPDDemandName;

  bool PegSubUsageToPrimary = this->setOfParameters().getBool("PegSubUsageToPrimary");
  bool supplierPerspectiveInterplantPeg = this->setOfParameters().getBool("supplierPerspectiveInterplantPeg");

  // Loop over parts to get demands
  int i = 0; // Pulled out of the for below by RW2STL
  for(i=0; i<nParts; i++){
    partName = partList[i];
    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);
    int j = 0; // Pulled out of the for below by RW2STL
    for(j=0; j<nDemands; j++){
      demandName = dList[j];

      // Now for some fancy trickery !!!
      // if the part is a special feature part, then we must map the consVol over to 
      //     the GPD appData
      int isDemandSpecial = 0;
      isDemandSpecial = mpHelper.isDemandSpecialPureOptionBuildDemand(theWitRun, partName, demandName, pp, geo);
      if (! isDemandSpecial) {
	isDemandSpecial = mpHelper.isDemandSpecialCustChoiceFeature(theWitRun, partName, demandName, pp, geo);
      }
 

      // get teh demandAppData, or create a new one if it doesn't exist.
      LgFrSceDemandAppData * demandAppDataPtr = 0 ;

      // if the demand is a specialFeaturePart then get the gpd's appdata
      if (isDemandSpecial) {
	// get a witPartName for the GPD
	fullWitGPDPartName = mpHelper.geoPlannerDemandPartName(pp, geo);

	witGetDemandAppData( theWitRun, fullWitGPDPartName.c_str(), geo.c_str(),
			   (void **) &demandAppDataPtr );
	if ( demandAppDataPtr == 0 ) {
	  demandAppDataPtr = new LgFrSceDemandAppData ((size_t) nPeriods, 1.0);
	  assert ( demandAppDataPtr != 0 );
	  witSetDemandAppData( theWitRun, fullWitGPDPartName.c_str(), geo.c_str(),  
			       (void *) demandAppDataPtr );
	}
      }
      else {
	witGetDemandAppData( theWitRun, partName, demandName,
			     (void **) &demandAppDataPtr );
	if ( demandAppDataPtr == 0 ) {
	  demandAppDataPtr = new LgFrSceDemandAppData ((size_t) nPeriods, 1.0);
	  assert ( demandAppDataPtr != 0 );
	  witSetDemandAppData( theWitRun, partName, demandName,  
			       (void *) demandAppDataPtr );
	}
      }

#ifdef DEBUG_DUMP_FOR_PEGGING
      
      if (isDemandSpecial) {
	std::cout << "specialDemand, being REPORTED..." << std::endl;
	std::cout << "PIP supply is being reported for: "
	     << fullWitGPDPartName << " at geo " << geo << std::endl;
      }
      
      std::cout << "(before)PIPConsVolSched (" << partList[i] << "," << dList[j] << std::endl;
      demandAppDataPtr->PIPConsVolSched().print();
      std::cout << "(before)PIPSupplyVolSched (" << partList[i] << "," << dList[j] << std::endl;
      demandAppDataPtr->PIPSupplyVolSched().print();

      std::cout << std::endl;

#endif

      // Get demands PIP consVol and directly store in appData
      ScePIP::getDemandConsVolPIP(theWitRun,
				  partList[i],
				  dList[j],
				  0,
				  nPeriods-1,
				  demandAppDataPtr->PIPConsVolSched(),
				  WitTRUE,
				  PegSubUsageToPrimary);

      // Get  PIP supplyVol and directly store in appData
      ScePIP::getSupplyVolPIP(theWitRun,
			      partList[i],
			      dList[j],
			      0,
			      nPeriods-1,
			      demandAppDataPtr->PIPSupplyVolSched());

      // Get demands PIP prodVol and directly store in appData
      ScePIP::getProdVolPIP(theWitRun,
			    partList[i],
			    dList[j],
			    0,
			    nPeriods-1,
			    demandAppDataPtr->PIPProdVolSched());


      // Get PIP sideVol  and directly store in appData
      ScePIP::getSideVolPIP(theWitRun,
			    partList[i],
			    dList[j],
			    0,
			    nPeriods-1,
			    demandAppDataPtr->PIPSideVolSched());



      // Get demands PIP  execVol and directly store in appData
      ScePIP::getDemandExecVolPIP(theWitRun,
					  partList[i],
					  dList[j],
					  0,
					  nPeriods-1,
					  demandAppDataPtr->PIPExecVolSched());

      ScePIP::getDemandInterplantVolPIP(theWitRun,
					&mpHelper,
					partList[i],
					dList[j],
					0,
					nPeriods-1,
					demandAppDataPtr->PIPInterplantVolSched(),
					supplierPerspectiveInterplantPeg);



#ifdef DEBUG_DUMP_FOR_PEGGING
      std::cout << "ConsVolPIP(after) JP's Get" << std::endl;
      demandAppDataPtr->PIPConsVolSched().print();
      std::cout << "SupplyVolPIP(after) JP's Get" << std::endl;
      demandAppDataPtr->PIPSupplyVolSched().print();
      std::cout << " ==========================" << std::endl << std::endl;
#endif

      witFree(dList[j]);
    }
    witFree(dList);
    witFree(partList[i]);
  }
  witFree(partList);

}




LgFrMultiPlantHelper&
LgFrScenarioForSce::multiPlantHelper()
{
  return multiPlantHelper_;
}

LgFrSceCritList &
LgFrScenarioForSce::sceCritList()
{
  return critList_;
}


//  ---------------------------------------------------------------------
//  Print Part Component  Information
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printDummyPartDataBaseDumpFile(
    WitRun * const theWitRun,   // WIT environment
    LgFrMultiPlantHelper & mpHelper,
    const int nPeriods,         // Number of periods 
    const int nParts,           // Number of parts   
    char ** partList,           // List of part names
    std::string fileName)
{
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Debug" 
		       << fileName);
  }

  LgFrCalendar theCal = this->calendar();
  
  long numRecords = 0;
  
  float * supplyVol;
  float * cycleTime;
  witBoolean * execFlag;
  float * mrpConsVol;
  
  float * reqVol;
  float * mrpExcessVol;      
  float * mrpResidualVol;      
  
  float * excessVol;      
  float * residualVol;      
  float * stockVol;      
  float * scrapVol;      
  float * consVol;      
  float * prodVol;
  
   witAttr category;
   witBoolean operationExists;

   int i;

  int nonZeroOnly = 0;
  int alwaysPrint = 1;

   LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();
   LgFrSceBaseExploder * myExploder = this->exploder();
   bool doImplode = setOfParameters.getBool("doImplode");
   
   // Loop once for each part.  
   for ( i=0; i<nParts; i++ ) {

     std::string fullPartName(partList[i]);
     // this time, exclude the real parts
     if (mpHelper.isPartNormal(theWitRun, fullPartName))
       continue;

     witGetPartCategory(theWitRun, fullPartName.c_str(), &category);

     witGetPartSupplyVol(theWitRun, fullPartName.c_str(), &supplyVol);

     if (category == WitPRODUCT)
       witGetPartCycleTime(theWitRun, fullPartName.c_str(),  &cycleTime);

     witGetOperationExists(theWitRun, fullPartName.c_str(), &operationExists);    
     if (operationExists) 
       witGetOperationExecutable(theWitRun, fullPartName.c_str(), &execFlag);

     myExploder->mrpConsVol(partList[i], &mrpConsVol);
     myExploder->reqVol(partList[i], &reqVol);
     myExploder->mrpExcessVol(partList[i], &mrpExcessVol);
     myExploder->mrpResidualVol(partList[i], &mrpResidualVol);

     if (doImplode) {
       witGetPartExcessVol(theWitRun, fullPartName.c_str(), &excessVol);
       witGetPartResidualVol(theWitRun, fullPartName.c_str(), &residualVol);
       if (category != WitCAPACITY) 
	 witGetPartStockVol(theWitRun, fullPartName.c_str(), &stockVol);
       witGetPartScrapVol(theWitRun, fullPartName.c_str(), &scrapVol);
       witGetPartConsVol(theWitRun, fullPartName.c_str(), &consVol);
       if (category == WitPRODUCT)
	 witGetPartProdVol(theWitRun, fullPartName.c_str(),  &prodVol);
     }
       
     // ==========================
     // PRINT everything down here
     // ==========================
     std::string pn(fullPartName);
     std::string pdf(mpHelper.pdf(fullPartName));
     
     numRecords += printDbDumpRec(pn, pdf, "supplyVol", supplyVol, outFile, nPeriods, nonZeroOnly);
     if (category == WitPRODUCT)
       numRecords += printDbDumpRec(pn, pdf, "cycleTime", cycleTime, outFile, nPeriods, nonZeroOnly);
     if (operationExists)
       numRecords += printDbDumpRecBool(pn, pdf, "execFlag", execFlag, outFile, nPeriods);
     numRecords += printDbDumpRec(pn, pdf, "mrpConsVol", mrpConsVol, outFile, nPeriods, alwaysPrint);
     numRecords += printDbDumpRec(pn, pdf, "reqVol", reqVol, outFile, nPeriods, nonZeroOnly);
     numRecords += printDbDumpRec(pn, pdf, "mrpExcessVol", mrpExcessVol, outFile, nPeriods, nonZeroOnly);
     numRecords += printDbDumpRec(pn, pdf, "mrpResidualVol", mrpResidualVol, outFile, nPeriods, nonZeroOnly);

     if (doImplode) {
       //       numRecords += printDbDumpRec(pn, pdf, "excessVol", excessVol, outFile, nPeriods, nonZeroOnly);
       numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "excessVol", excessVol, outFile, nPeriods, nonZeroOnly);
       numRecords += printDbDumpRec(pn, pdf, "residualVol", residualVol, outFile, nPeriods, nonZeroOnly);
       if (category != WitCAPACITY) 
	 numRecords += printDbDumpRec(pn, pdf, "stockVol", stockVol, outFile, nPeriods, nonZeroOnly);	 
       numRecords += printDbDumpRec(pn, pdf, "scrapVol", scrapVol, outFile, nPeriods, nonZeroOnly);	 
       //       numRecords += printDbDumpRec(pn, pdf, "consVol", consVol, outFile, nPeriods, nonZeroOnly);	 
       flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "consVol", consVol, outFile, nPeriods, nonZeroOnly);
       if (category == WitPRODUCT)
	 numRecords += printDbDumpRec(pn, pdf, "prodVol", prodVol, outFile, nPeriods, nonZeroOnly);	 
     }

     witFree(supplyVol);
     if (category == WitPRODUCT)
       witFree(cycleTime);
     if (operationExists)
       witFree(execFlag);
     witFree(reqVol);
     witFree(mrpExcessVol);
     witFree(mrpResidualVol);
     if (doImplode) {       
       witFree(excessVol);
       witFree(residualVol);
       if (category != WitCAPACITY)
	 witFree(stockVol);
       witFree(scrapVol);
       witFree(consVol);
       if (category == WitPRODUCT) 
	 witFree(prodVol);
     }

   }

   outFile.close();
   flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}

//  ---------------------------------------------------------------------
//  Print Part DB Dump File
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printPartDataBaseDumpFile(
    WitRun * const theWitRun,   // WIT environment
    LgFrMultiPlantHelper & mpHelper,
    const int nPeriods,         // Number of periods 
    const int nParts,           // Number of parts   
    char ** partList,           // List of part names
    std::string fileName)
{
  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Part Database Dump" 
		       << fileName);
  }


  LgFrCalendar theCal = this->calendar();

  long numRecords = 0;
  long numUniquePairs = 0;
  // we must check this against the number of records in the partDef file


  LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();
  LgFrSceBaseExploder * myExploder = this->exploder();          

  bool doImplode =         setOfParameters.getBool("doImplode");
  std::string siteCustomization = setOfParameters.getString("siteCustomization");

  witAttr category;
  witBoolean operationExists;

  int i,t, p, b, npBops;


  //  - all the execVol stuff...
  float * prodVol      = new float[nPeriods];  
  float * mrpProdVol   = new float[nPeriods];  
  float * bldVol       = new float[nPeriods];  
  float * mrpBldVol    = new float[nPeriods];    
  float * userOpVol    = new float[nPeriods];
  float * mrpUserOpVol = new float[nPeriods];    
  float * intPlVol     = new float[nPeriods];
  float * mrpIntPlVol  = new float[nPeriods];   
  float * altVol       = new float[nPeriods];
  float * mrpAltVol    = new float[nPeriods];    
 
  float * deAggVol     = new float[nPeriods];
  float * mrpDeAggVol  = new float[nPeriods];    

  float * supplyVolSpecifiedByUser;
  float * witStockVol;
  float * cycleTime;
  float * yieldRate;
  witBoolean * execFlag;
  float * witExcessVol;
  float * witResidualVol;
  float * scrapVol;
  float * consVol;
  float * reqVol;
  float * witMrpExcessVol;      
  float * witMrpResidualVol;      

  LgFrTimeVecFloat excessVol((size_t) nPeriods, 0.0);
  LgFrTimeVecFloat residualVol((size_t) nPeriods, 0.0);



  int nDemands;
  char ** dList;

#ifdef ENABLE_NEGATIVE_DEMANDS
  LgFrTimeVecFloat negDemVol;
#endif

  int nonZeroOnly = 0;
  int alwaysPrint = 1;
  
  int alwaysPrintForECAonly = (sce::compareStringsCaseIns( siteCustomization, "eca" ) ? 0 : 1);
  bool combineCapacityIntoPartDB = setOfParameters.getBool("combineCapacityIntoPartDB");

  
  // Loop once for each part.  
  for ( i=0; i<nParts; i++ ) {

    std::string fullPartName(partList[i]);
    if (! mpHelper.isPartNormal(theWitRun, fullPartName))
      continue;

    if (! combineCapacityIntoPartDB) {
      if (mpHelper.isPartNormalCapacity(theWitRun, fullPartName))
	continue;
    }

    witGetPartCategory(theWitRun, fullPartName.c_str(), &category);


    // Get some info about the parts
    witGetPartNProducingBopEntries(theWitRun, fullPartName.c_str(), &npBops);
    int nUserDefinedOps = mpHelper.nUserDefinedOps(theWitRun, fullPartName);
    int nInterplantOps = mpHelper.nInterplantOps(theWitRun, fullPartName);
    int nAggregationOps = mpHelper.nAggregationOps(theWitRun, fullPartName);    

    // See if the default operation exists for this part
    witGetOperationExists(theWitRun, fullPartName.c_str(), &operationExists);    

    // get demands on the part.  These are needed for direct demands
    witGetPartDemands(theWitRun, fullPartName.c_str(), &nDemands, &dList);

#ifdef ENABLE_NEGATIVE_DEMANDS
    // Compute :=======> negDemVol
    int doesPartHaveNegativeDemands = mpHelper.doesPartHaveNegativeDemands(theWitRun, fullPartName);
    if (doesPartHaveNegativeDemands) {
      negDemVol = mpHelper.negDemVol(theWitRun, fullPartName);
    }
#endif     


    // Compute :=======> supplyVol
    witGetPartSupplyVol(theWitRun, fullPartName.c_str(), &supplyVolSpecifiedByUser);
#ifdef ENABLE_NEGATIVE_DEMANDS
    LgFrTimeVecFloat witSupplyVol((size_t) nPeriods, supplyVolSpecifiedByUser);
    // adjust the supplyVol by removing any dummy supply due to  negative demands 
    if (doesPartHaveNegativeDemands) {
      for (t=0; t<nPeriods; t++) {
	supplyVolSpecifiedByUser[t] -= negDemVol[t];
      }
    }
#endif     



    // Compute :======> CycleTime
    if (category == WitPRODUCT) {
      // FINISH_ME: This si a temprorary stopgap for maintaining
      // backward compatability.  In 1.30/1.31, the (eca) user's only
      // way of finding out what were true PCF parts was via
      // cycleTime=-1.  This needs to be worked out, but is OK for now.

      // get the "real" cycleTime in any case
      witGetPartCycleTime(theWitRun, fullPartName.c_str(),  &cycleTime);

#ifndef VARIABLE_PERIODS
      // don't do this for variable periods. 
      // 
      // if the part is a PCF, then use partAppData and
      // unConstrainedStartPeriod to set those periods to -1,
      // after that pick up the cycleTime vector for real
      // eventually, we need to get rid of cycleTime altogether
      // or use an appData for it.
      if (mpHelper.isPartPcf(theWitRun, fullPartName)) {
	LgFrScePartAppData * appDataPtr;
	witGetPartAppData(theWitRun, fullPartName.c_str(), (void **) &appDataPtr);
	assert(appDataPtr != 0);
	int tt = 0; // Pulled out of the for below by RW2STL
	for (tt=0; tt<appDataPtr->unConstrainedStartPeriod(); tt++)
	  cycleTime[tt] = -1.0;
      }
#endif
    }



    // Compute :=======> mrpConsVol
    float * mrpConsVol;
    myExploder->mrpConsVol(fullPartName, &mrpConsVol);
    // add in direct demand vol
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nDemands; j++) {
      float * indepDemandVol;
      witGetDemandDemandVol(theWitRun, fullPartName.c_str(), dList[j], &indepDemandVol);
      int tt = 0; // Pulled out of the for below by RW2STL
      for (tt=0; tt<nPeriods; tt++) {
	mrpConsVol[tt] += indepDemandVol[tt];
      }
      witFree(indepDemandVol);
    }
    //    int matt = 0; // Pulled out of the for below by RW2STL
    //    for (matt=0; matt<nPeriods; matt++) {
    //      mrpConsVol[matt] -= mrpAltVol[matt];
    //    }
    




#ifdef ENABLE_NEGATIVE_DEMANDS
    LgFrTimeVecFloat witMrpConsVol((size_t) nPeriods, mrpConsVol);
    LgFrTimeVecFloat negDemMrpConsVol((size_t) nPeriods, 0.0);
    if (doesPartHaveNegativeDemands) {
      float rolledNegDemVol = 0.0;
      int tt = 0; // Pulled out of the for below by RW2STL
      for (tt=0; tt<nPeriods; tt++) {
	rolledNegDemVol += negDemVol[tt];
	if (mrpConsVol[tt] > rolledNegDemVol) {
	  mrpConsVol[tt] -= rolledNegDemVol;
	  negDemMrpConsVol[tt] += rolledNegDemVol;
	  rolledNegDemVol = 0.0;
	}
	else {
	  // Note: if mrpConsVol < 0 then there are negative usage rates
	  // these can realy screw up our calculation
	  if (mrpConsVol[tt] > 0) {
	    rolledNegDemVol -= mrpConsVol[tt];
	    negDemMrpConsVol[tt] += mrpConsVol[tt];
	    mrpConsVol[tt] = 0.0;
	  }
	}
      }
    }
#endif
      
    // Compute :======> reqVol
    myExploder->reqVol(partList[i], &reqVol);

    // Compute :======> execFlag
    if (operationExists) {
      witGetOperationExecutable(theWitRun, fullPartName.c_str(), &execFlag);

      // Compute :======> yieldRate
      witGetOperationYieldRate(theWitRun, fullPartName.c_str(), &yieldRate);
    }

    // Now compute all the execVol types:
    // MRP --------> mrpProdVol, mrpBldVol, mrpIntPlVol, mrpAltVol, mrpDeAggVol, mrpUserOpVol
    // Implosion --> prodVol, bldVol, intPlVol, altVol, deAggVol, userOpVol
    if (npBops > 0) {
      // first intialize the vectors to zero
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++) {
	prodVol[t] = 0.0;
	mrpProdVol[t] = 0.0;
	bldVol[t] = 0.0;
	mrpBldVol[t] = 0.0;
	altVol[t] = 0.0;
	mrpAltVol[t] = 0.0;

	if (nUserDefinedOps) {
	  userOpVol[t] = 0;
	  mrpUserOpVol[t] = 0;
	}
	if (nInterplantOps) {
	  intPlVol[t] = 0.0;
	  mrpIntPlVol[t] = 0.0;
	}
	if (nAggregationOps) {
	  deAggVol[t] = 0.0;
	  mrpDeAggVol[t] = 0.0;
	}
      }

      // loop through each of the bop entries and get the execVol once for each
      // bop entry (this is a bit inefficient, but too bad).
      for (p=0; p<npBops; p++) {
	bool foundOpCategory = false;          
	std::string cp1, pdf1, sp2, pdf2;
	int early, late;
	float  prodRate;
	char * witOperationName;
	float * tempExecVol;
	float * tempMrpExecVol;
	float * yieldRate;
	float * mrpYieldRate;        
	  
	witGetPartProducingBopEntry(theWitRun, fullPartName.c_str(), p, &witOperationName, &b);
	std::string operationName(witOperationName);
	witGetBopEntryProdRate(theWitRun, witOperationName, b, &prodRate);
	witGetBopEntryEarliestPeriod(theWitRun, witOperationName, b, &early);
	witGetBopEntryLatestPeriod(theWitRun, witOperationName, b, &late);
	myExploder->mrpExecVol(witOperationName, &tempMrpExecVol);          
	if (doImplode) {
	  witGetOperationExecVol(theWitRun, witOperationName, &tempExecVol);
	}
	witGetOperationYieldRate(theWitRun, witOperationName, &yieldRate);
	witGetOperationYieldRate(myExploder->localWitRun(), witOperationName, &mrpYieldRate);        

	
	// Is it the default Operation
	if (operationName == fullPartName 
	    || mpHelper.isOperationForCapacityGeneration(theWitRun, operationName, cp1, pdf1)) {
	  int t = 0; 
	  for (t=0; t<nPeriods; t++) {
	    if (doImplode) {
	      bldVol[t]     += yieldRate[t] * tempExecVol[t];
	    }
	    mrpBldVol[t]  += mrpYieldRate[t] * tempMrpExecVol[t];
	  }
	  foundOpCategory = true;
	}
	  
	// is it an Interplant Operation ?
	if (! foundOpCategory
	    && nInterplantOps
	    && mpHelper.isOperationSpecialInterplant(theWitRun, operationName, cp1, pdf1, sp2, pdf2)) {
	  LgFrTimeVecFloat newExecVol = this->interplantExecVol(operationName);
	  LgFrTimeVecFloat newMrpExecVol = this->interplantMrpExecVol(operationName);
	  int t = 0; // Pulled out of the for below by RW2STL
	  for (t=early; t<=late; t++) {
	    if (doImplode) {
	      intPlVol[t]    += prodRate * newExecVol[t];
	    }
	    mrpIntPlVol[t] += prodRate * newMrpExecVol[t];
	  }
	  foundOpCategory = true;
	}

	// is it an Alternate Operation ?
	if (! foundOpCategory
	    && mpHelper.isOperationSpecialAlternate(theWitRun, operationName, cp1, pdf1, sp2, pdf2)) {
	  int t = 0; // Pulled out of the for below by RW2STL
	  for (t=early; t<=late; t++) {
	    if (doImplode) {
	      altVol[t]    += prodRate * tempExecVol[t];
	    }
	    mrpAltVol[t] += prodRate * tempMrpExecVol[t];
	  }
	  foundOpCategory = true;
	}





	// is it an Aggreration Operation ?
	if (! foundOpCategory
	    && nAggregationOps
	    && mpHelper.isOperationSpecialAggregation(theWitRun, operationName, cp1, pdf1, sp2, pdf2)) {
	  int t = 0; // Pulled out of the for below by RW2STL
	  for (t=early; t<=late; t++) {
	    if (doImplode) {	  
	      deAggVol[t]    += prodRate * yieldRate[t] * tempExecVol[t];
	    }
	    mrpDeAggVol[t] += prodRate * mrpYieldRate[t] * tempMrpExecVol[t];
	  }
	  foundOpCategory = true;
	}

	// is it a UserDefined Operation (it must be, at this point)
	if (! foundOpCategory
	    && nUserDefinedOps) {
	  int * impactPeriod;
	  int * mrpImpactPeriod;          
	  witGetBopEntryImpactPeriod(theWitRun, witOperationName, b, &impactPeriod);
	  witGetBopEntryImpactPeriod(myExploder->localWitRun(), witOperationName, b, &mrpImpactPeriod);
	  int t = 0; // Pulled out of the for below by RW2STL
	  for (t=early; t<=late; t++) {
	    if (impactPeriod[t] > -1)
	      if (doImplode) {	  
		userOpVol[impactPeriod[t]]       += yieldRate[t] * prodRate * tempExecVol[t];
	      }
	    if (mrpImpactPeriod[t] > -1)            
	      mrpUserOpVol[mrpImpactPeriod[t]] += mrpYieldRate[t] * prodRate * tempMrpExecVol[t];
	  }
	  witFree(impactPeriod);
	  witFree(mrpImpactPeriod);
	  foundOpCategory = true;
	}

	// Now assert that the operation resolved into exactly one of the types!!
	if (! foundOpCategory) {
	  std::cerr << "ABOUT TO DIE!!\n"
	       << "witOperationName=" << witOperationName << "\n\n" <<std::endl;
	}
	assert(foundOpCategory);

	// clean-up the memory for this bop
	witFree(witOperationName);
	if (doImplode) {	
	  witFree(tempExecVol);
	}
	witFree(tempMrpExecVol);
	witFree(yieldRate);
	witFree(mrpYieldRate);        
      }

      for (t=0; t<nPeriods; t++) {
	if (doImplode) {	  
	  prodVol[t] = bldVol[t];
	}
	mrpProdVol[t] = mrpBldVol[t];
	if (nUserDefinedOps) {
	  mrpProdVol[t] += mrpUserOpVol[t];
	  if (doImplode) {	  
	    prodVol[t] += userOpVol[t];
	  }
	}
	if (nInterplantOps) {
	  mrpProdVol[t] += mrpIntPlVol[t];
	  if (doImplode) {	  
	    prodVol[t] += intPlVol[t];
	  }
	}
	if (nAggregationOps) {
	  mrpProdVol[t] += mrpDeAggVol[t];
	  if (doImplode) {	  
	    prodVol[t] += deAggVol[t];
	  }
	}
      }
    }
    
     
    LgFrTimeVecFloat stockVol((size_t) nPeriods, 0.0);
    if (doImplode) {


      // Compute :======> scrapVol
      witGetPartScrapVol(theWitRun, fullPartName.c_str(), &scrapVol);

      // Compute :======> consVol
      // FINISH_ME: temporary fix we always print consvol
      witGetPartConsVol(theWitRun, fullPartName.c_str(), &consVol);
      // add in direct demand vol
      for (j=0; j<nDemands; j++) {
	float * indepShipVol;
	witGetDemandShipVol(theWitRun, fullPartName.c_str(), dList[j], &indepShipVol);
	int tt = 0; // Pulled out of the for below by RW2STL
	for (tt=0; tt<nPeriods; tt++) {
	  consVol[tt] += indepShipVol[tt];
	}
	witFree(indepShipVol);
      }
      int att = 0; // Pulled out of the for below by RW2STL
      for (att=0; att<nPeriods; att++) {
	consVol[att] -= altVol[att];
      }
      

#ifdef ENABLE_NEGATIVE_DEMANDS
      LgFrTimeVecFloat witConsVol((size_t) nPeriods, consVol);
      LgFrTimeVecFloat negDemConsVol((size_t) nPeriods, 0.0);
      if (doesPartHaveNegativeDemands) {
	float rolledNegDemVol = 0.0;
	int tt = 0; // Pulled out of the for below by RW2STL
	for (tt=0; tt<nPeriods; tt++) {
	  rolledNegDemVol += negDemVol[tt];
	  if (consVol[tt] > rolledNegDemVol) {
	    consVol[tt] -= rolledNegDemVol;
	    negDemConsVol[tt] += rolledNegDemVol;
	    rolledNegDemVol = 0.0;
	  }
	  else {
	    // Note: if consVol < 0 then there are negative usage rates
	    // these can realy screw up our calculation
	    if (consVol[tt] > 0) {
	      rolledNegDemVol -= consVol[tt];
	      negDemConsVol[tt] += consVol[tt];
	      consVol[tt] = 0.0;
	    }
	  }
	}
      }
#endif



      // Compute :======> StockVol, ExcessVol, ResidualVol
      if (category == WitCAPACITY) {
	witStockVol = new float[nPeriods];
	int t=0;
	for (t=0; t<nPeriods; t++)
	  witStockVol[t] = 0.0;
      }
      else {
	witGetPartStockVol(theWitRun, fullPartName.c_str(), &witStockVol);
      }

      witGetPartExcessVol(theWitRun, fullPartName.c_str(), &witExcessVol);
      witGetPartResidualVol(theWitRun, fullPartName.c_str(), &witResidualVol);
      int ttt = 0; // Pulled out of the for below by RW2STL
      for (ttt=0; ttt<nPeriods; ttt++) {
	if (category != WitCAPACITY) {
	  stockVol[ttt] = witStockVol[ttt];
	}
	excessVol[ttt] = witExcessVol[ttt];
	residualVol[ttt] = witResidualVol[ttt];
      }
#ifdef ENABLE_NEGATIVE_DEMANDS
      LgFrTimeVecFloat negDemandStockVol((size_t) nPeriods, 0.0);
      if (doesPartHaveNegativeDemands) {
	// strategy: 
	// 1) compute witStockVol (ie, wrt the witRun including dummy supply)
	// 2) compute mrpNegDemandStockVol.  This is an abstract sort of thing that
	//              represents the amount of stockVol that is due to negDemand.
	//              its computed from negDemVol and negDemConsVol with a sort
	//              or material balance equation
	// 3) compute stockVol (ie, stockVol if you remove dummy supply)
	// 4) compute residualVol.  We have to manually calculate this.
	// 5) compute excessVol by truncating mrpResidualVol at the supplyVol level
      
	// 1) compute witStockVol 
	//     ....  We already have this from wit ....
	// 2) compute negDemandStockVol.  
	negDemandStockVol[0]  = negDemVol[0] - negDemConsVol[0];
	for (t=1; t<nPeriods; t++) {
	  negDemandStockVol[t] = negDemandStockVol[t-1] + negDemVol[t] - negDemConsVol[t];
	}

	// 3) compute stockVol
	for (t=0; t<nPeriods; t++) 
	  stockVol[t] = witStockVol[t] - negDemandStockVol[t];

	// 4) compute residualVol.  
	// the amount of excess can be at most the ending stock
	float potentialToExcess = stockVol[nPeriods-1];
	// walk backwards in time and look for an increasing stockVol from 
	// period t-1 to 1
	float stockIncrementThisPeriod;
	for (t=nPeriods-1; t>0; t--) {
	  stockIncrementThisPeriod = stockVol[t] - stockVol[t-1];
	  if (stockIncrementThisPeriod > 0.0) {
	    if (stockIncrementThisPeriod > potentialToExcess) {
	      residualVol[t] = potentialToExcess;
	      potentialToExcess = 0.0;
	    }
	    else {
	      residualVol[t] = stockIncrementThisPeriod;
	      potentialToExcess -= stockIncrementThisPeriod;
	    }
	  }
	}
	// now check period 0
	stockIncrementThisPeriod = stockVol[0];
	if (stockIncrementThisPeriod > 0.0) {
	  if (stockIncrementThisPeriod > potentialToExcess) {
	    residualVol[0] = potentialToExcess;
	    potentialToExcess = 0.0;
	  }
	  else {
	    residualVol[0] = stockIncrementThisPeriod;
	    potentialToExcess -= stockIncrementThisPeriod;
	  }
	}


	// 5) compute excessVol by truncating residualVol at the supplyVol level
	for (t=0; t<nPeriods; t++) {
	  if (residualVol[t] > supplyVolSpecifiedByUser[t])
	    excessVol[t] = supplyVolSpecifiedByUser[t];
	  else
	    excessVol[t] = residualVol[t];
      }
    }


#endif
  


    }

    // Compute :======> mrpExcessVol
    myExploder->mrpExcessVol(partList[i], &witMrpExcessVol);
    myExploder->mrpResidualVol(partList[i], &witMrpResidualVol);
    LgFrTimeVecFloat mrpExcessVol((size_t) nPeriods, witMrpExcessVol);
    LgFrTimeVecFloat mrpResidualVol((size_t) nPeriods, witMrpResidualVol);
#ifdef ENABLE_NEGATIVE_DEMANDS
    LgFrTimeVecFloat mrpNegDemandStockVol((size_t) nPeriods, 0.0);
    LgFrTimeVecFloat witMrpStockVol((size_t) nPeriods, 0.0);
    LgFrTimeVecFloat mrpStockVol(witMrpStockVol);
    if (doesPartHaveNegativeDemands) {
      // strategy: 
      // 1) compute witMrpStockVol (ie, wrt the witRun including dummy supply)
      // 2) compute mrpNegDemandStockVol.  This is an abstract sort of thing that
      //              represents the amount of stockVol that is due to negDemand.
      //              its computed from negDemVol and negDemMrpConsVol with a sort
      //              or material balance equation
      // 3) compute mrpStockVol (ie, stockVol if you remove dummy supply)
      // 4) compute mrpResidualVol.  We have to manually calculate this.
      // 5) compute mrpExcessVol by truncating mrpResidualVol at the supplyVol level
      
      // 1) compute witMrpStockVol 
      witMrpStockVol[0] = witSupplyVol[0] + mrpProdVol[0] - witMrpConsVol[0];
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=1; t<nPeriods; t++) {
	witMrpStockVol[t] = witMrpStockVol[t-1] + witSupplyVol[t] + mrpProdVol[t] - witMrpConsVol[t];
      }

      // 2) compute mrpNegDemandStockVol.  
      mrpNegDemandStockVol[0]  = negDemVol[0] - negDemMrpConsVol[0];
      for (t=1; t<nPeriods; t++) {
	mrpNegDemandStockVol[t] = mrpNegDemandStockVol[t-1] + negDemVol[t] - negDemMrpConsVol[t];
      }

      // 3) compute mrpStockVol
      mrpStockVol = witMrpStockVol - mrpNegDemandStockVol;

      // 4) compute mrpResidualVol.  
      // the amount of excess can be at most the ending stock
      float potentialToExcess = mrpStockVol[nPeriods-1];
      // walk backwards in time and look for an increasing stockVol from 
      // period t-1 to 1
      float stockIncrementThisPeriod;
      for (t=nPeriods-1; t>0; t--) {
	stockIncrementThisPeriod = mrpStockVol[t] - mrpStockVol[t-1];
	if (stockIncrementThisPeriod > 0.0) {
	  if (stockIncrementThisPeriod > potentialToExcess) {
	    mrpResidualVol[t] = potentialToExcess;
	    potentialToExcess = 0.0;
	  }
	  else {
	    mrpResidualVol[t] = stockIncrementThisPeriod;
	    potentialToExcess -= stockIncrementThisPeriod;
	  }
	}
      }
      // now check period 0
      stockIncrementThisPeriod = mrpStockVol[0];
      if (stockIncrementThisPeriod > 0.0) {
	if (stockIncrementThisPeriod > potentialToExcess) {
	  mrpResidualVol[0] = potentialToExcess;
	  potentialToExcess = 0.0;
	}
	else {
	  mrpResidualVol[0] = stockIncrementThisPeriod;
	  potentialToExcess -= stockIncrementThisPeriod;
	}
      }


      // 5) compute mrpExcessVol by truncating mrpResidualVol at the supplyVol level
      for (t=0; t<nPeriods; t++) {
	if (mrpResidualVol[t] > supplyVolSpecifiedByUser[t])
	  mrpExcessVol[t] = supplyVolSpecifiedByUser[t];
	else
	  mrpExcessVol[t] = mrpResidualVol[t];
      }
    }
#endif



    // Compute :======> mrpRequirements
    LgFrTimeVecFloat mrpRequirementsVol((size_t) nPeriods, reqVol);
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++) {
      mrpRequirementsVol[t] -= mrpExcessVol[t];
    }


    // ==========================
    // PRINT everything down here
    // ==========================
    std::string pn(mpHelper.partname(fullPartName));
    std::string pdf(mpHelper.pdf(fullPartName));


#ifdef ENABLE_NEGATIVE_DEMANDS
#if 0
    // for debug use only !!!
    if (doesPartHaveNegativeDemands) {
      flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "witMrpConsVol", witMrpConsVol.data(), outFile, nPeriods, alwaysPrint);
      flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "witMrpStockVol", witMrpStockVol.data(), outFile, nPeriods, alwaysPrint);
      flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "mrpNegDemandStockVol", mrpNegDemandStockVol.data(), outFile, nPeriods, alwaysPrint);
      flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "witMrpExcessVol", witMrpExcessVol, outFile, nPeriods, alwaysPrint);

      //      printDbDumpRec(pn, pdf, "negDemConsVol", negDemConsVol.data(), outFile, nPeriods, alwaysPrint);
      flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "witConsVol", witMrpConsVol.data(), outFile, nPeriods, alwaysPrint);
      flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "witStockVol", witMrpStockVol.data(), outFile, nPeriods, alwaysPrint);
      flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "negDemandStockVol", mrpNegDemandStockVol.data(), outFile, nPeriods, alwaysPrint);
      flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "witExcessVol", witMrpExcessVol, outFile, nPeriods, alwaysPrint);

    }
#endif
#endif


    numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "supplyVol", supplyVolSpecifiedByUser, outFile, nPeriods, nonZeroOnly);
#ifdef ENABLE_NEGATIVE_DEMANDS
    if (doesPartHaveNegativeDemands) 
      numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "negDemVol", negDemVol.data(), outFile, nPeriods, nonZeroOnly);
#endif     
    if (category == WitPRODUCT) {
      numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "cycleTime", cycleTime, outFile, nPeriods, nonZeroOnly);
    }
    numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "mrpConsVol", mrpConsVol, outFile, nPeriods, nonZeroOnly);
#ifdef ENABLE_NEGATIVE_DEMANDS
    if (doesPartHaveNegativeDemands) {
      numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "negDemMrpConsVol", negDemMrpConsVol.data(), outFile, nPeriods, nonZeroOnly);
    }
#endif     

    numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "reqVol", reqVol, outFile, nPeriods, nonZeroOnly);
    numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "mrpExcessVol", mrpExcessVol.data(), outFile, nPeriods, nonZeroOnly);
    numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "mrpResidualVol", mrpResidualVol.data(), outFile, nPeriods, nonZeroOnly);
    if (category != WitCAPACITY) 
      numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "mrpStockVol", mrpStockVol.data(), outFile, nPeriods, nonZeroOnly);

    numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "mrpRequirements", mrpRequirementsVol.data(), outFile, nPeriods, nonZeroOnly);

    if (operationExists) {
      //      numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "yield", yieldRate, outFile, nPeriods, alwaysPrint);
    }

    // Now print all the execVol types:
    // MRP --------> mrpProdVol, mrpBldVol, mrpIntPlVol, mrpDeAggVol, mrpUserOpVol
    // Implosion --> prodVol, bldVol, intPlVol, deAggVol, userOpVol
    if (npBops > 0) {
      numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "mrpProdVol", mrpProdVol, outFile, nPeriods, nonZeroOnly);
      numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "mrpBldVol", mrpBldVol, outFile, nPeriods, nonZeroOnly);
      if (nInterplantOps) 
	numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "mrpIntPlVol", mrpIntPlVol, outFile, nPeriods, nonZeroOnly);
      numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "mrpAltVol", mrpAltVol, outFile, nPeriods, nonZeroOnly);
      if (nAggregationOps) 
	numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "mrpDeAggVol", mrpDeAggVol, outFile, nPeriods, nonZeroOnly);
      if (nUserDefinedOps) 
	numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "mrpUserOpVol", mrpUserOpVol, outFile, nPeriods, nonZeroOnly);

      if (doImplode) {        
	numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "prodVol", prodVol, outFile, nPeriods, nonZeroOnly);
	numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "bldVol", bldVol, outFile, nPeriods, nonZeroOnly);
	if (nInterplantOps) 
	  numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "intPlVol", intPlVol, outFile, nPeriods, nonZeroOnly);
	numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "altVol", altVol, outFile, nPeriods, nonZeroOnly);
	if (nAggregationOps) 
	  numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "deAggVol", deAggVol, outFile, nPeriods, nonZeroOnly);
	if (nUserDefinedOps) 
	  numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "userOpVol", userOpVol, outFile, nPeriods, nonZeroOnly);
      }
    }

    if (doImplode) {        
      numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "excessVol", excessVol.data(), outFile, nPeriods, nonZeroOnly);
      numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "residualVol", residualVol.data(), outFile, nPeriods, nonZeroOnly);
      if (category != WitCAPACITY) 
	numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "stockVol", stockVol.data(), outFile, nPeriods, nonZeroOnly);
      numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "scrapVol", scrapVol, outFile, nPeriods, nonZeroOnly);
      numRecords += flatFileMgr_.writeDbDumpRecord(theCal, pn, pdf, "consVol", consVol, outFile, nPeriods, nonZeroOnly);
    }


    if (category == WitPRODUCT)
      witFree(cycleTime);
    witFree(reqVol);
    witFree(witMrpExcessVol);
    witFree(witMrpResidualVol);
    witFree(mrpConsVol);
    if (operationExists) {
      witFree(execFlag);
      witFree(yieldRate);
    }
    if (doImplode) {        
      witFree(witExcessVol);
      witFree(witResidualVol);
      if (category == WitCAPACITY) 
	delete [] witStockVol;
      else
	witFree(witStockVol);
      witFree(scrapVol);
      witFree(consVol);
    }


    // free up the direct demands 
    for (j=0; j<nDemands; j++) {
      witFree(dList[j]);
    }
    witFree(dList);

    witFree(supplyVolSpecifiedByUser);
    numUniquePairs++;
  }
     


  delete [] prodVol;
  delete [] mrpProdVol;
  delete [] mrpBldVol;
  delete [] bldVol;  
  delete [] userOpVol;
  delete [] mrpUserOpVol; 
  delete [] intPlVol;
  delete [] mrpIntPlVol;
  delete [] altVol;
  delete [] mrpAltVol;
  delete [] deAggVol;
  delete [] mrpDeAggVol;
  
    
  outFile.close();
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numUniquePairs);
}




// prints a record of a DB Dump file
// returns 1 if printed, 0 if not
int
LgFrScenarioForSce::printDbDumpRec(
    std::string & pn, 
    std::string & pdf, 
    const char * vecID,
    const float *  floatVec,
    std::ofstream & outFile,   // output stream (already opened)
    int nPeriods, 
    int printIfVectorIsAllZero)
{
    if ((printIfVectorIsAllZero) || (this->flatFileMgr_.isFloatVecNonZero(floatVec, nPeriods))) {
      outFile << "\"" << pn << "\",\""
	      << pdf << "\",";
      outFile << "\"" << vecID << "\"";
      flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, floatVec, 0, 15);
      return 1;
    }
    
    return 0;
}

// prints a (boolean) record of a DB Dump file
// returns 1 
int
LgFrScenarioForSce::printDbDumpRecBool(
    std::string & pn, 
    std::string & pdf, 
    const char * vecID,
    const witBoolean *  boolVec,
    std::ofstream & outFile,   // output stream (already opened)
    int nPeriods)
{
  outFile << "\"" << pn << "\",\""
	  << pdf << "\",";
  outFile << "\"" << vecID << "\"";
  flatFileMgr_.writeCsvBoolVector(outFile, nPeriods, boolVec);
  return 1;
}


// compute an adjusted excessVol due to negative demands (works for mrp or implosion)
LgFrTimeVecFloat 
LgFrScenarioForSce::excessVolAdjustedForNegativeDemands(
    float * witExcessVol,
    float * supplyVol,
    LgFrTimeVecFloat & negDemVol,
    int nPeriods)
{
 LgFrTimeVecFloat adjustedExcessVol((size_t) nPeriods, witExcessVol);

 // ok now do something
 return adjustedExcessVol;
 
}



//  ---------------------------------------------------------------------
//  Print Operation Dump File
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printOperationDataBaseDumpFile(
    WitRun * const theWitRun,   // WIT environment
    LgFrMultiPlantHelper & mpHelper,
    const int nPeriods,         // Number of periods 
    std::string fileName)
{

  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Operation Database Dump" 
		       << fileName);
  }

  long numRecords = 0;

  float * witVector;

  int nOps;
  char ** opList;
  witGetOperations(theWitRun, &nOps, &opList);

  int o;
  LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();
  std::string siteCustomization = setOfParameters.getString("siteCustomization");
  LgFrSceBaseExploder * myExploder = this->exploder();          

  bool doImplode = setOfParameters.getBool("doImplode");


  
  // Loop once for each operation
  for ( o=0; o<nOps; o++ ) {

    std::string operation(opList[o]);
    std::string op, p;
    if (! mpHelper.isOperationNormal(theWitRun, operation, p, op))
      continue;

    // this file does not report on default-operations!!  Only the userDefined ones.
    if (mpHelper.isPartNormal(theWitRun, operation))
      continue;

    
    // ----------------------------
    // INPUT type of parameters     
    // ----------------------------

    // execFlag
    witBoolean * witBoolVec;
    witGetOperationExecutable(theWitRun, operation.c_str(), &witBoolVec);
    outFile << "\"" << mpHelper.operationName(operation) << "\",\""
	    << mpHelper.pdf(operation) << "\",";
    outFile << "\"execFlag\"";
    flatFileMgr_.writeCsvBoolVector(outFile, nPeriods, witBoolVec);
    witFree(witBoolVec);
    numRecords++;

    // yieldRate
    witGetOperationYieldRate(theWitRun, operation.c_str(), &witVector);
    outFile << "\"" << mpHelper.operationName(operation) << "\",\""
	    << mpHelper.pdf(operation) << "\",";
    outFile << "\"yield\"";
    flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, witVector, 0, 15);
    witFree(witVector);
    numRecords++;
     
    // ----------------------
    // MRP type of parameters     
    // ----------------------

    // mrpExecVol
    myExploder->mrpExecVol(operation, &witVector);
    if (this->flatFileMgr_.isFloatVecNonZero(witVector, nPeriods)) {
      outFile << "\"" << mpHelper.operationName(operation) << "\",\""
	      << mpHelper.pdf(operation) << "\",";
      outFile << "\"mrpExecVol\"";
      flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, witVector, 0, 15);
      numRecords++;
    }
    witFree(witVector);

       
    // ----------------------------
    // IMPLOSION type of parameters     
    // ----------------------------
    if (doImplode) {
      // execVol
      witGetOperationExecVol(theWitRun, operation.c_str(), &witVector);
      if (this->flatFileMgr_.isFloatVecNonZero(witVector, nPeriods)) {
	outFile << "\"" << mpHelper.operationName(operation) << "\",\""
		<< mpHelper.pdf(operation) << "\",";
	outFile << "\"execVol\"";
	flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, witVector, 0, 15);
	numRecords++;
      }
      witFree(witVector);

    }
     

  }
 
  outFile.close();

  for ( o=0; o<nOps; o++ ) 
    witFree(opList[o]);
  witFree(opList);

  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}


//  ---------------------------------------------------------------------
//  Print Operation Dump File
//  --------------------------------------------------------------------- 
void 
LgFrScenarioForSce::printOperationExecDumpFile(
    WitRun * const theWitRun,   // WIT environment
    LgFrMultiPlantHelper & mpHelper,
    const int nPeriods,         // Number of periods 
    std::string fileName)
{

  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "Operation Exec Dump" 
		       << fileName);
  }

  std::string leadingString;

  long numRecords = 0;

  float * witVector;

  int nOps;
  char ** opList;
  witGetOperations(theWitRun, &nOps, &opList);

  int o;
  LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();
  LgFrSceBaseExploder * myExploder = this->exploder();          

  bool doImplode = setOfParameters.getBool("doImplode");

  
  // Loop once for each operation
  for ( o=0; o<nOps; o++ ) {

    std::string operation(opList[o]);
    std::string op, p;

    // execFlag
    witBoolean * witBoolVec;
    witGetOperationExecutable(theWitRun, opList[o], &witBoolVec);
    outFile << "\"" << opList[o]<< "\",\",";
    outFile << "\"execFlag\"";
    flatFileMgr_.writeCsvBoolVector(outFile, nPeriods, witBoolVec);

    witFree(witBoolVec);
    numRecords++;

    // yieldRate
    witGetOperationYieldRate(theWitRun, opList[o], &witVector);
    leadingString = "\"" 
      + (std::string) opList[o] + "\",\","
      +  "\"yieldRate\"";
    //     flatFileMgr_.writeCsvFloatVector(outFile, nPeriods, witVector, 0, 15);


    flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, witVector);
    witFree(witVector);
    numRecords++;
     
    // ----------------------
    // MRP type of parameters     
    // ----------------------

    // mrpExecVol
    myExploder->mrpExecVol(operation, &witVector);
    if (this->flatFileMgr_.isFloatVecNonZero(witVector, nPeriods)) {
      leadingString = "\"" 
	+ (std::string) opList[o] + "\",\","
	+ "\"mrpExecVol\"";
      flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, witVector);
      numRecords++;
    }
    witFree(witVector);

       
    // ----------------------------
    // IMPLOSION type of parameters     
    // ----------------------------
    if (doImplode) {
      // execVol
      witGetOperationExecVol(theWitRun, operation.c_str(), &witVector);
      if (this->flatFileMgr_.isFloatVecNonZero(witVector, nPeriods)) {
	leadingString = "\"" 
	  + (std::string) opList[o] + "\",\","
	  +  "\"execVol\"";
	flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, witVector);
	numRecords++;
      }
      witFree(witVector);

    }

  }
 
  outFile.close();

  for ( o=0; o<nOps; o++ ) 
    witFree(opList[o]);
  witFree(opList);

  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}








//  ---------------------------------------------------------------------
//  Print MRP Supply Orders File
//  ---------------------------------------------------------------------
//  PUD 14's are EMLS Planned Order (supply) transactions (ie, MRP orders for supply).
//  The key thing is that the supply shows customer PDF and supplier PDF.
//  If th esupply is created by "build in-house", then the customer and supplier PDF
//  is the same.
//
//  Codes are:
//  "A" = interplant
//  "B" = bin
//  "F" = build in-house
//  "O" = user-defined Operation
//  "D" = de-aggregation
//  "E" = External supply (ie, supplyVol)
void 
LgFrScenarioForSce::printMrpSupplyOrdersFile(
    LgFrMultiPlantHelper & mpHelper,
    const int nPeriods,         // Number of periods 
    std::string fileName)
{

  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "MRP Supply Orders" 
		       << fileName);
  }


  long numRecords = 0;

  float * supplyOrderQty = new float [nPeriods];  

  std::string leadingString;

  LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();
  LgFrSceBaseExploder * myExploder = this->exploder();          

  WitRun *  exploderWitRun = myExploder->localWitRun();

  int nOps;
  char ** opList;
  witGetOperations(exploderWitRun, &nOps, &opList);
  
  
  // Loop once for each operation
  int o = 0; // Pulled out of the for below by RW2STL
  for (o=0; o<nOps; o++ ) {

    std::string fullWitOperation(opList[o]);
    std::string op, p, pn1, pdf1, pn2, pdf2;
    std::string PUD14SourceCode;
    std::string supplierPdf, customerPdf, producedPart;



    // IF the operation is normal, then its either a  "F" source (ie, build in-house)
    //                                     OR a user-defined operation
    if (mpHelper.isOperationNormal(exploderWitRun, fullWitOperation, p, op)) {
      supplierPdf = p;
      customerPdf = p;      
      // if the part exists, then the operation is a build-inhouse
      if (mpHelper.isPartNormal(exploderWitRun, fullWitOperation)) 
	PUD14SourceCode = "F";
      else 
	PUD14SourceCode = "O";
    }

    else if (mpHelper.isOperationSpecialInterplant(exploderWitRun, fullWitOperation, pn1, pdf1, pn2, pdf2)) {
      // FINISH_ME: the case when pn1 Not equal pn2 may need to be reconsidered
      supplierPdf = pdf2;
      customerPdf = pdf1;      
      PUD14SourceCode = "A";
    }

    else if (mpHelper.isOperationSpecialAggregation(exploderWitRun, fullWitOperation, pn1, pdf1, pn2, pdf2)) {
      supplierPdf = pdf2;
      customerPdf = pdf1;      
      PUD14SourceCode = "D";
    }

    else
      continue;

    // regardless of what this thing is, compute how much supply it created for the
    // Produced part

    int nBops;
    witGetOperationNBopEntries(exploderWitRun, fullWitOperation.c_str(), &nBops);
    int b = 0; // Pulled out of the for below by RW2STL
    for (b=0; b<nBops; b++) {
      int * impactPeriod;
      float * mrpExecVol;
      float prodRate;
      float * yieldRate;
      int early, late;
      char * producedPart;
      
      
      witGetBopEntryProducedPart(exploderWitRun, fullWitOperation.c_str(), b, &producedPart);
      // if its an operation that produces capacity, then skip it.
      witAttr category;      
      witGetPartCategory(exploderWitRun, producedPart, &category);
      if (category == WitCAPACITY) {
	witFree(producedPart);
	continue;
      }
      witGetBopEntryProdRate(exploderWitRun, fullWitOperation.c_str(), b, &prodRate);
      witGetBopEntryEarliestPeriod(exploderWitRun, fullWitOperation.c_str(), b, &early);
      witGetBopEntryLatestPeriod(exploderWitRun, fullWitOperation.c_str(), b, &late);
      myExploder->mrpExecVol(fullWitOperation, &mrpExecVol);          
      // note: do a switcheroo to get the real interplant execVol a'la jumper arcs
      if (PUD14SourceCode == "A") {
	LgFrTimeVecFloat realMrpExecVol = this->interplantMrpExecVol(fullWitOperation);
	int t = 0; // Pulled out of the for below by RW2STL
	for (t=0; t<nPeriods; t++)
	  mrpExecVol[t] = realMrpExecVol[t];
      }


      witGetOperationYieldRate(exploderWitRun, fullWitOperation.c_str(), &yieldRate);

      witGetBopEntryImpactPeriod(exploderWitRun, fullWitOperation.c_str(), b, &impactPeriod);

      // first initialize the vector and the total to Zero
      float recordTotal = 0.0;
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++)
	supplyOrderQty[t] = 0.0;

      for (t=early; t<=late; t++) {
	if (impactPeriod[t] > -1) {
	  supplyOrderQty[impactPeriod[t]] = yieldRate[t] * prodRate * mrpExecVol[t];
	  recordTotal += supplyOrderQty[impactPeriod[t]];
	}
      }

      witFree(mrpExecVol);
      witFree(yieldRate);
      witFree(impactPeriod);

      // only print a record if its a non-zero vector.
      if (recordTotal > 0.0001) {
	    leadingString = "\""
	      + mpHelper.partname(producedPart) + "\",\""
	      + supplierPdf + "\",\""
	      + customerPdf + "\",\""          
	      + PUD14SourceCode + "\"";
	flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, supplyOrderQty);

	numRecords++;        
      }
      witFree(producedPart);

    }
     

  }



  int nParts;
  char ** partList;
  witGetParts(exploderWitRun, &nParts, &partList);


  // at this point all supply records will be E source.
  std::string PUD14SourceCode = "E";
  // Loop once for each operation
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++ ) {
    std::string fullWitPart(partList[i]);
    std::string partName, customerPdf;
    float * supplyVol;

    // IF the part is normal, then get its supply Vol and report
    if (mpHelper.isPartNormal(exploderWitRun, fullWitPart)) {
      witGetPartSupplyVol(exploderWitRun, fullWitPart.c_str(), &supplyVol);
      float recordTotal = 0.0;
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t< nPeriods; t++) {
	recordTotal += supplyVol[t];
      }

      customerPdf = mpHelper.pdf(fullWitPart);
      // only print a record if its a non-zero vector.
      if (recordTotal > 0.0001) {
	leadingString = "\"" 
	  + mpHelper.partname(fullWitPart) + "\",\""
	  + customerPdf + "\",\""
	  + customerPdf + "\",\""          
	  + PUD14SourceCode + "\"";
	flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, supplyVol);
	numRecords++;        
      }
      witFree(supplyVol);
    }
  
  }
  
  delete [] supplyOrderQty;
  
  for ( o=0; o<nOps; o++ ) 
    witFree(opList[o]);
  witFree(opList);

  for ( i=0; i<nParts; i++ ) 
    witFree(partList[i]);
  witFree(partList);
  
  outFile.close();
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}


//  ---------------------------------------------------------------------
//  Print MRP Demand Orders File
//  ---------------------------------------------------------------------
//
//  Codes are:
//  "A" = interplant
//  "B" = bin
//  "F" = build in-house (includes features, subs, BB's)
//  "O" = user-defined Operation
//  "D" = de-aggregation
//  "E" = External demand (ie, demandVol)
void 
LgFrScenarioForSce::printMrpDemandOrdersFile(
    LgFrMultiPlantHelper & mpHelper,
    const int nPeriods,         // Number of periods 
    std::string fileName)
{

  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*sceErrFacility_)("CannotOpenFileForWrite",MclArgList() 
		       << "MRP Demand Orders" 
		       << fileName);
  }

  long numRecords = 0;

  LgFrSceSetOfParameters & setOfParameters =  this->setOfParameters();
  LgFrSceBaseExploder * myExploder = this->exploder();          

  WitRun *  exploderWitRun = myExploder->localWitRun();

  float * demandOrderQty = new float [nPeriods];
  float * subDemandOrderQty = new float [nPeriods];  

  std::string leadingString;
  
  int nOps;
  char ** opList;
  witGetOperations(exploderWitRun, &nOps, &opList);

  int nParts;
  char ** partList;
  witGetParts(exploderWitRun, &nParts, &partList);

  // For F source and O source, we need to accumulate the demands into an
  // array.  For the others, we report as we find them.
  // malloc vectors to be a vector of floatVecs
  float ** FsourceDemand = (float **) malloc (nParts * sizeof(float *));
  assert(FsourceDemand != 0);
  float ** OsourceDemand = (float **) malloc (nParts * sizeof(float *));
  assert(OsourceDemand != 0);

  
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    FsourceDemand[i] = (float *) malloc (nPeriods * sizeof(float));
    assert(FsourceDemand[i] != 0);    
    OsourceDemand[i] = (float *) malloc (nPeriods * sizeof(float));
    assert(OsourceDemand[i] != 0);

    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++) {
      FsourceDemand[i][t] = 0.0;
      OsourceDemand[i][t] = 0.0;
    }
    
    // set the index's for these parts using appData.  We need to do this
    // because there will be lots of part searching going on.
    LgFrScePartAppData * appDataPtr;    
    witGetPartAppData(exploderWitRun, partList[i], (void **) &appDataPtr);
    if (appDataPtr == 0) {
      appDataPtr = new LgFrScePartAppData(0, nPeriods, nPeriods);
      appDataPtr->index(i);
      witSetPartAppData(exploderWitRun, partList[i], (void *) appDataPtr);
    }
    else
      appDataPtr->index(i);
  }

  

  
  // Loop once for each operation
  int o = 0; // Pulled out of the for below by RW2STL
  for (o=0; o<nOps; o++ ) {

    std::string fullWitOperation(opList[o]);
    std::string op, p, pn1, pdf1, pn2, pdf2, geo, plannerPart;
    std::string sourceCode;
    std::string supplierPdf, customerPdf;
    std::string gg, mm;


    // IF the operation is normal, then its either a  "F" source (ie, build in-house)
    //                                     OR a user-defined operation
    if (mpHelper.isOperationNormal(exploderWitRun, fullWitOperation, p, op)) {
      customerPdf = p;      
      // if the part exists, then the operation is a build-inhouse
      if (mpHelper.isPartNormal(exploderWitRun, fullWitOperation)) 
	sourceCode = "F";
      else 
	sourceCode = "O";
    }

    // if the operation is a specialStandaloneFeature then we pick this up as a
    // feature demand
    else if (mpHelper.isOperationSpecialStandaloneFeature(exploderWitRun, 
							  fullWitOperation, 
							  mm, 
							  gg, 
							  pn1, 
							  pdf1)) {
      sourceCode = "F";
    }
    

    else if (mpHelper.isOperationSpecialInterplant(exploderWitRun, fullWitOperation, pn1, pdf1, pn2, pdf2)) {
      // FINISH_ME: the case when pn1 Not equal pn2 may need to be reconsidered
      customerPdf = pdf1;      
      sourceCode = "A";
    }

    else if (mpHelper.isOperationSpecialAggregation(exploderWitRun, fullWitOperation, pn1, pdf1, pn2, pdf2)) {
      customerPdf = pdf1;      
      sourceCode = "D";
    }
    else if (mpHelper.isOperationSpecialGeoPlannerDemand(exploderWitRun, fullWitOperation,
							 plannerPart, geo)) {
      customerPdf = geo;      
      sourceCode = "E";
    }

    else
      continue;

    // regardless of what this thing is, compute how much demand it 
    // generated on each of its children

    int nBoms;
    witGetOperationNBomEntries(exploderWitRun, fullWitOperation.c_str(), &nBoms);
    int k = 0; // Pulled out of the for below by RW2STL
    for (k=0; k<nBoms; k++) {
      int * impactPeriod;
      float * mrpExecVol;
      float usageRate;
      int fallout;
      int early, late;
      char * consumedPart;
      
      witGetBomEntryConsumedPart(exploderWitRun, fullWitOperation.c_str(), k, &consumedPart);

      // ignore capacity
      witAttr category;      
      witGetPartCategory(exploderWitRun, consumedPart, &category);
      if (category == WitCAPACITY) {
	witFree(consumedPart);
	continue;
      }
      
      // check the child part. If its not normal, then skip it
      if (! mpHelper.isPartNormal(exploderWitRun, consumedPart)) {
	witFree(consumedPart);        
	continue;
      }
      
      witGetBomEntryUsageRate(exploderWitRun, fullWitOperation.c_str(), k, &usageRate);
      witGetBomEntryEarliestPeriod(exploderWitRun, fullWitOperation.c_str(), k, &early);
      witGetBomEntryLatestPeriod(exploderWitRun, fullWitOperation.c_str(), k, &late);
      witGetBomEntryFallout(exploderWitRun, fullWitOperation.c_str(), k, &fallout);      
      myExploder->mrpExecVol(fullWitOperation, &mrpExecVol);          
      witGetBomEntryImpactPeriod(exploderWitRun, fullWitOperation.c_str(), k, &impactPeriod);

      // Now consider subs (this is a pain in the ass)
      // if the bom entry has subs, then we must pick that demand up and make
      // sure to report it on the sub part.
      // FINISH_ME: use impactPeriod from bom entry.  See BOB about an impact period
      // for subs given that there's seperate offsets for subs.
      // This *should* only occur for real subs and not for any dummies
      int nSubs;
      witGetBomEntryNSubsBomEntries(exploderWitRun, fullWitOperation.c_str(), k, &nSubs);
      int s = 0; // Pulled out of the for below by RW2STL
      for (s=0; s<nSubs; s++) {
	float subUsageRate;
	int subEarly;
	int subLate;
	float subFalloutRate;
	char * subConsumedPart;
	float * subVol;
	witGetSubsBomEntryUsageRate(exploderWitRun, fullWitOperation.c_str(), k, s, &subUsageRate);
	witGetSubsBomEntryEarliestPeriod(exploderWitRun, fullWitOperation.c_str(), k, s, &subEarly);
	witGetSubsBomEntryLatestPeriod(exploderWitRun, fullWitOperation.c_str(), k, s, &subLate);
	witGetSubsBomEntryFalloutRate(exploderWitRun, fullWitOperation.c_str(), k, s, &subFalloutRate);
	witGetSubsBomEntryConsumedPart(exploderWitRun, fullWitOperation.c_str(), k, s, &subConsumedPart);
	myExploder->subUsageVol(fullWitOperation, k, s, &subVol);
	// zero out the subDemandOrderQty vector first
	int t = 0; // Pulled out of the for below by RW2STL
	for (t=0; t<nPeriods; t++)
	  subDemandOrderQty[t] = 0.0;
	for (t=subEarly; t<=subLate; t++) {
	  // decrement mrpExecVol by subVol
	  mrpExecVol[t] -= subVol[t];
	  // now compute the demand on the sub
	  if (impactPeriod[t] > -1) {
	    subDemandOrderQty[impactPeriod[t]] = subVol[t]
				 * subUsageRate / (1.0 - subFalloutRate);
	  }
	}
	// if its a sub off an interplant, then its a jumper arc
	if (sourceCode == "A") {
	  // only print a record if its a non-zero vector.
	  if (flatFileMgr_.isFloatVecNonZero(subDemandOrderQty, nPeriods)) {                      
	    leadingString = "\"" + 
	      mpHelper.partname(subConsumedPart) + "\",\""
	      + mpHelper.pdf(subConsumedPart) + "\",\""
	      + customerPdf + "\",\""          
	      + sourceCode + "\"";
	    flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, subDemandOrderQty);

	    numRecords++;        
	  }
	}

	else if (sourceCode == "F") {
	  LgFrScePartAppData * myAppDataPtr;
	  witGetPartAppData(exploderWitRun, subConsumedPart, (void **) &myAppDataPtr);
	  assert(myAppDataPtr != 0);
	  int index = myAppDataPtr->index();
	  for (t=0; t<nPeriods; t++) 
	    FsourceDemand[index][t] += subDemandOrderQty[t];
	}
	else if (sourceCode == "O") {
	  LgFrScePartAppData * myAppDataPtr;
	  witGetPartAppData(exploderWitRun, subConsumedPart, (void **) &myAppDataPtr);
	  assert(myAppDataPtr != 0);
	  int index = myAppDataPtr->index();
	  for (t=0; t<nPeriods; t++) 
	    OsourceDemand[index][t] += subDemandOrderQty[t];
	}
	// if its a sub off an aggregation or a GPD, then its an error
	else {
	  assert(1 == 0);
	}
	witFree(subConsumedPart);
	witFree(subVol);
      }  // this is the end of analysis for subs

      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++)
	demandOrderQty[t] = 0.0;
      
      for ( t=early; t<=late; t++) {
	if (impactPeriod[t] > -1) {
	  demandOrderQty[impactPeriod[t]] = mrpExecVol[t] * usageRate * 100.0 / (float) (100 - fallout);
	}
      }

      witFree(mrpExecVol);
      witFree(impactPeriod);

      LgFrScePartAppData * myAppDataPtr;
      witGetPartAppData(exploderWitRun, consumedPart, (void **) &myAppDataPtr);
      assert(myAppDataPtr != 0);
      int index = myAppDataPtr->index();

      // interplants, bins, indep demand,  and de-aggs are the same
      if (sourceCode == "A" || sourceCode == "D" || sourceCode == "E" || sourceCode == "B") {
	// only print a record if its a non-zero vector.
	if (flatFileMgr_.isFloatVecNonZero(demandOrderQty, nPeriods)) {                      
	  leadingString = "\"" 
	    + mpHelper.partname(consumedPart) + "\",\""
	    + customerPdf + "\",\""
	    + mpHelper.pdf(consumedPart) + "\",\""          
	    + sourceCode + "\"";
	  flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, demandOrderQty);

	  numRecords++;        
	}
      }
      else if (sourceCode == "F") {
	for (t=0; t<nPeriods; t++) 
	  FsourceDemand[index][t] += demandOrderQty[t];
      }
      else if (sourceCode == "O") {
	for (t=0; t<nPeriods; t++) 
	  OsourceDemand[index][t] += demandOrderQty[t];
      }
      // You should never get here
      else {
	assert(1 == 0);
      }

      witFree(consumedPart);

    }

  }

  // OK, now print the F and O source  
  for ( i=0; i<nParts; i++ ) {
    std::string thePart(partList[i]);
    // if its not normal then skip it
    if (! mpHelper.isPartNormal(exploderWitRun, thePart)) {
      continue;
    }
    if (flatFileMgr_.isFloatVecNonZero(FsourceDemand[i], nPeriods)) {              
      std::string thePdf(mpHelper.pdf(thePart));
      leadingString = "\"" 
	+ mpHelper.partname(thePart) + "\",\""
	+ thePdf + "\",\""
	+ thePdf + "\",\""          
	+ "F" + "\"";
      flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, FsourceDemand[i]);

      numRecords++;
    }

    // as long as you find one period that is non-zero, then print it
    if (flatFileMgr_.isFloatVecNonZero(OsourceDemand[i], nPeriods)) {        
      std::string thePdf(mpHelper.pdf(thePart));
      leadingString =  "\"" 
	+ mpHelper.partname(thePart) + "\",\""
	+ thePdf + "\",\""
	+ thePdf + "\",\""          
	+ "O" + "\"";
      flatFileMgr_.writeFloatVecToStream(this->calendar(), outFile, nPeriods, 0, leadingString, OsourceDemand[i]);

      numRecords++;
    }
    
  }

  
  for (i=0; i<nParts; i++) {
    free(FsourceDemand[i]);
    free(OsourceDemand[i]);
  }
  free(FsourceDemand);
  free(OsourceDemand);  
  
  
  for ( o=0; o<nOps; o++ ) 
    witFree(opList[o]);
  witFree(opList);

  for ( i=0; i<nParts; i++ ) 
    witFree(partList[i]);
  witFree(partList);

  delete [] demandOrderQty;
  delete [] subDemandOrderQty;  
  
  outFile.close();
  flatFileMgr_.sceIOLogManager().printOutputLog(fileName,numRecords,000,numRecords);
}



// ---------------------------------------------------------------
// One stop WIT messaging control
void 
LgFrScenarioForSce::setWitInformationalMessaging(WitRun * const theWitRun,   
						       int onOrOff)
{
  // if they want it off
  if (onOrOff == 0) {
    // shut off ALL info messages
    witSetMesgTimesPrint(theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
    // Let ANY warning message come out once
    witSetMesgTimesPrint(theWitRun, WitTRUE, WitWARNING_MESSAGES, 1);    
    // Suppress a few WARNING messages that we already know about
    witSetMesgTimesPrint(theWitRun, WitTRUE, 338, 0);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 446, 0);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 538, 0);        
    witSetMesgTimesPrint(theWitRun, WitTRUE, 505, 0);        
    witSetMesgTimesPrint(theWitRun, WitTRUE, 745, 0);        
  }     
  else {
    // turn all INFO on
    witSetMesgTimesPrint(theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, UCHAR_MAX);
    // turn all WARNINGS on 
    witSetMesgTimesPrint(theWitRun, WitTRUE, WitWARNING_MESSAGES, UCHAR_MAX);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 338, 0);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 538, 0);           
    witSetMesgTimesPrint(theWitRun, WitTRUE, 505, 0);         
    witSetMesgTimesPrint(theWitRun, WitTRUE, 745, 0);         
  }
}


// return the set of paramters 
LgFrSceSetOfParameters&
LgFrScenarioForSce::setOfParameters()
{
  return parameterSet_;
}

// return the flat file manager
LgFrSceFlatFileMgr&
LgFrScenarioForSce::flatFileMgr()
{
  return flatFileMgr_;
}


/*void
LgFrScenarioForSce::localeSnapshot(
				   RWLocaleSnapshot * localSnapshotPtr)
{
  delete localSnapshotPtr_;
  localSnapshotPtr_ = localSnapshotPtr;
  RWLocale::global(localSnapshotPtr_);
}*/

LgFrSetOfParts&
LgFrScenarioForSce::setOfParts(int k)
{
  // OK to cast away const. This is non-const member function
  return (LgFrSetOfParts &) constSetOfParts(k);
}

const
LgFrSetOfParts&
LgFrScenarioForSce::constSetOfParts(int k)
const
{
  assert (k >= LGFR_DEFAULT_SET && k <= LGFR_INTERPLANT_SET );
  switch (k) {
  case LGFR_DEFAULT_SET:
    return defaultSOP_;
  case LGFR_UNIVERSAL_SET:
    return LgFrScenario::constSetOfParts(LGFR_UNIVERSAL_SET);
  case LGFR_INTERPLANT_SET:
    return interplantSOP_;
  default:
    assert(1==0);
  }
  return defaultSOP_;
}


LgFrSetOfDemands&
LgFrScenarioForSce::setOfDemands(int k)
{
  // OK to cast away const. This is non-const member function
  return (LgFrSetOfDemands &) constSetOfDemands(k);
}

const
LgFrSetOfDemands&
LgFrScenarioForSce::constSetOfDemands(int k)
const
{
  assert (k >= LGFR_DEFAULT_SET && k <= LGFR_UNIVERSAL_SET );
  switch (k) {
    case LGFR_DEFAULT_SET:
       return defaultSOD_;
    case LGFR_UNIVERSAL_SET:
       return LgFrScenario::constSetOfDemands(LGFR_UNIVERSAL_SET);
    default:
       assert(1==0);
     }
  return defaultSOD_;
}

isAReturnType
LgFrScenarioForSce::isA() const
{
    return id_;
}

// Constructor
LgFrScenarioForSce::LgFrScenarioForSce(const LgFrDataInterfaceAbstract&    dif,
					 const LgFrCalendar&         calendar)
:LgFrScenario(dif, calendar),
 defaultSOP_(0),
 defaultSOD_(0),
 interplantSOP_(0),
 multiPlantHelper_(),
 critList_(),
 parameterSet_(),
 flatFileMgr_(),
 nStandaloneFeatureArcs_(0),
 nDemandsWithNonZeroMins_(0),
// localSnapshotPtr_(0),
 engine_("heuristic"),
 id_(__LGFRSCENARIOFORSCE),
 exploderPtr_(0),
 sceErrFacDisp_(0),
 sceMsgFacDisp_(0),
 sceErrFacility_(0),
 sceMsgFacility_(0),
 sceScrub_(new LgFrSceScrubber())
{
  // builder does the rest
}


// Destructor 
LgFrScenarioForSce::~LgFrScenarioForSce()
{
  LgFrSetOfDemands & univSod = this->setOfDemands();
  WitRun * theWitRun  = this->lastProblemSolved().witRun();

  // Use WIT Object Iteration to delete all the AppData's

  witAttr objItrState;
  
  witGetObjItrState(theWitRun, &objItrState);
  if (objItrState != WitINACTIVE) {
    std::cout << "Deleting scenario's witRun, AppData Clones:   ogjItrState is not WitINACTIVE.   Resetting ObjItr" << std::endl;
    witResetObjItr(theWitRun);
  }

  LgFrSceAppData * appDataPtr;
  
  witBoolean areWeDoneYet = WitFALSE;
  
  while ( ! areWeDoneYet) {
    witAdvanceObjItr(theWitRun);
    
    witGetObjItrState(theWitRun, &objItrState);
    
    if (objItrState ==  WitAT_PART) {   
      char * partName;
      witGetObjItrPart(theWitRun, &partName);
      witGetPartAppData(theWitRun, partName, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(partName);
    }

    else if (objItrState == WitAT_DEMAND) {
      char * partName;
      char * demandName;
      witGetObjItrDemand(theWitRun, &partName, &demandName);
      witGetDemandAppData(theWitRun, partName, demandName, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(partName);
      witFree(demandName);
    }
	
    else if (objItrState == WitAT_OPERATION) {
      char * opName;
      witGetObjItrOperation(theWitRun, &opName);
      witGetOperationAppData(theWitRun, opName, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }

    else if (objItrState == WitAT_BOM_ENTRY) {
      char * opName;
      int b;
      witGetObjItrBomEntry(theWitRun, &opName, &b);
      witGetBomEntryAppData(theWitRun, opName, b, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }


    else if (objItrState == WitAT_SUB_ENTRY) {
      char * opName;
      int b, s;
      witGetObjItrSubsBomEntry(theWitRun, &opName, &b, &s);
      witGetSubsBomEntryAppData(theWitRun, opName, b, s, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }

    else if (objItrState == WitAT_BOP_ENTRY) {
      char * opName;
      int bp;
      witGetObjItrBopEntry(theWitRun, &opName, &bp);
      witGetBopEntryAppData(theWitRun, opName, bp, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }
      
    else if (objItrState == WitINACTIVE) {
      areWeDoneYet = WitTRUE;
    }

    else 
      assert(1 == 0);
      
  }
  //  std::cout << "Done deleting appData from scenario's witrun" << std::endl;

  //  delete  exploderPtr_;
  //  std::cout << "Done deleting appData from exploder's witrun" << std::endl;
  
  delete  sceErrFacDisp_;
  delete  sceMsgFacDisp_;
  delete  sceErrFacility_;
  delete  sceMsgFacility_;
  
  delete  sceScrub_;

}



void
LgFrScenarioForSce::deleteAppDataAndScenarioObjects()
{
  WitRun * theWitRun  = this->lastProblemSolved().witRun();

  delete  exploderPtr_;  

  return;



  // Use WIT Object Iteration to delete all the AppData's

  witAttr objItrState;
  
  witGetObjItrState(theWitRun, &objItrState);
  if (objItrState != WitINACTIVE) {
    witResetObjItr(theWitRun);
  }

  LgFrSceAppData * appDataPtr;
  
  witBoolean areWeDoneYet = WitFALSE;
  
  while ( ! areWeDoneYet) {
    witAdvanceObjItr(theWitRun);
    
    witGetObjItrState(theWitRun, &objItrState);
    
    if (objItrState ==  WitAT_PART) {   
      char * partName;
      witGetObjItrPart(theWitRun, &partName);
      witGetPartAppData(theWitRun, partName, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(partName);
    }

    else if (objItrState == WitAT_DEMAND) {
      char * partName;
      char * demandName;
      witGetObjItrDemand(theWitRun, &partName, &demandName);
      witGetDemandAppData(theWitRun, partName, demandName, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(partName);
      witFree(demandName);
    }
	
    else if (objItrState == WitAT_OPERATION) {
      char * opName;
      witGetObjItrOperation(theWitRun, &opName);
      witGetOperationAppData(theWitRun, opName, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }

    else if (objItrState == WitAT_BOM_ENTRY) {
      char * opName;
      int b;
      witGetObjItrBomEntry(theWitRun, &opName, &b);
      witGetBomEntryAppData(theWitRun, opName, b, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }


    else if (objItrState == WitAT_SUB_ENTRY) {
      char * opName;
      int b, s;
      witGetObjItrSubsBomEntry(theWitRun, &opName, &b, &s);
      witGetSubsBomEntryAppData(theWitRun, opName, b, s, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }

    else if (objItrState == WitAT_BOP_ENTRY) {
      char * opName;
      int bp;
      witGetObjItrBopEntry(theWitRun, &opName, &bp);
      witGetBopEntryAppData(theWitRun, opName, bp, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }
      
    else if (objItrState == WitINACTIVE) {
      areWeDoneYet = WitTRUE;
    }

    else 
      assert(1 == 0);
      
  }






  //  delete  sceErrFacDisp_;
  //  delete  sceMsgFacDisp_;
  //  delete  sceErrFacility_;
  //  delete  sceMsgFacility_;
  //  delete  sceScrub_;



}






#if 0
void
LgFrScenarioForSce::deleteAppDataAndScenarioObjects()
{
  WitRun * theWitRun  = this->lastProblemSolved().witRun();

  std::cout << "Deleting Scenario objects including WIT AppData ... " << std::endl;

  // Use WIT Object Iteration to delete all the AppData's

  witAttr objItrState;
  
  witGetObjItrState(theWitRun, &objItrState);
  if (objItrState != WitINACTIVE) {
    witResetObjItr(theWitRun);
  }

  LgFrSceAppData * appDataPtr;
  
  witBoolean areWeDoneYet = WitFALSE;
  
  while ( ! areWeDoneYet) {
    witAdvanceObjItr(theWitRun);
    
    witGetObjItrState(theWitRun, &objItrState);
    
    if (objItrState ==  WitAT_PART) {   
      char * partName;
      witGetObjItrPart(theWitRun, &partName);
      witGetPartAppData(theWitRun, partName, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(partName);
    }

    else if (objItrState == WitAT_DEMAND) {
      char * partName;
      char * demandName;
      witGetObjItrDemand(theWitRun, &partName, &demandName);
      witGetDemandAppData(theWitRun, partName, demandName, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(partName);
      witFree(demandName);
    }
	
    else if (objItrState == WitAT_OPERATION) {
      char * opName;
      witGetObjItrOperation(theWitRun, &opName);
      witGetOperationAppData(theWitRun, opName, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }

    else if (objItrState == WitAT_BOM_ENTRY) {
      char * opName;
      int b;
      witGetObjItrBomEntry(theWitRun, &opName, &b);
      witGetBomEntryAppData(theWitRun, opName, b, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }


    else if (objItrState == WitAT_SUB_ENTRY) {
      char * opName;
      int b, s;
      witGetObjItrSubsBomEntry(theWitRun, &opName, &b, &s);
      witGetSubsBomEntryAppData(theWitRun, opName, b, s, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }

    else if (objItrState == WitAT_BOP_ENTRY) {
      char * opName;
      int bp;
      witGetObjItrBopEntry(theWitRun, &opName, &bp);
      witGetBopEntryAppData(theWitRun, opName, bp, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }
      
    else if (objItrState == WitINACTIVE) {
      areWeDoneYet = WitTRUE;
    }

    else 
      assert(1 == 0);
      
  }


  delete  exploderPtr_;
  
  delete  sceErrFacDisp_;
  delete  sceMsgFacDisp_;
  delete  sceErrFacility_;
  delete  sceMsgFacility_;
  
  delete  sceScrub_;
}
#endif




// Assignment operator.
LgFrScenarioForSce&
LgFrScenarioForSce::operator=(
   const LgFrScenarioForSce& rhs)
{
  // error
  (*sceErrFacility_)("sceLgFr");

/*
   if (this != &rhs) {           // Check for assignment to self
     (this->LgFrScenario::operator= ) (rhs);
     defaultSOP_ = rhs.defaultSOP_;
     defaultSOD_ = rhs.defaultSOD_;
   }
*/
   return *this;
}


// Customized copy constructor
LgFrScenarioForSce::LgFrScenarioForSce(
   const LgFrScenarioForSce& source )
: LgFrScenario( source ),
  defaultSOP_(source.defaultSOP_),
  defaultSOD_(source.defaultSOD_),
  interplantSOP_(source.interplantSOP_),
  multiPlantHelper_(source.multiPlantHelper_),
  nStandaloneFeatureArcs_(source.nStandaloneFeatureArcs_),
  nDemandsWithNonZeroMins_(source.nDemandsWithNonZeroMins_),
  critList_(source.critList_),
  parameterSet_(source.parameterSet_),
  flatFileMgr_(source.flatFileMgr_),
//  localSnapshotPtr_( source.localSnapshotPtr_ ),
  engine_(source.engine_),
  id_(__LGFRSCENARIOFORSCE),
  exploderPtr_(source.exploderPtr_),
  sceScrub_(new LgFrSceScrubber())
{
  // i don't know when the copy constructor is called, so not sure if this 
  // works... the reason i didn't assign copy error facility ptr to the 
  // original error facility ptr is because that might cause errors later
  // if one were to be destroyed...and the common facility along with it.

  // this is to create a suitable message file name
  // to this we will append a number (the integer count of the number
  // of such temporary log files are created)
  // Note: numTempFiles_ is a static data member of LgFrScenarioForSce class
  std::ifstream temp1;
  std::ifstream temp2;

  std::string errFileName = "errorLog" + numTempFiles_;
  std::string msgFileName = "messageLog" + numTempFiles_;

  temp1.open(errFileName.c_str());//, ios::in | ios::nocreate);
  temp2.open(msgFileName.c_str());//, ios::in | ios::nocreate);

  while (temp1.good() || temp2.good()) {   // this means that file exists!
    numTempFiles_++;  // maintain uniqueness among pairs of output files
    errFileName = "errorLog" + numTempFiles_;
    msgFileName = "messageLog" + numTempFiles_;
    temp1.open(errFileName.c_str());//, ios::in | ios::nocreate);
    temp2.open(msgFileName.c_str());//, ios::in | ios::nocreate);
  }

  numTempFiles_++; // need to increment since curr val is used now

  // create display and facility
  sceErrFacDisp_ = new MclPrintfOutputDisplayer(errFileName.c_str(),"w+");
  sceErrFacility_ = new MclFacility("SCE",*sceErrFacDisp_);
  sceErrFacility_->minErrOutLevel(MclLevel::error ());

  sceMsgFacDisp_ = new MclPrintfOutputDisplayer(msgFileName.c_str(),"w+");
  sceMsgFacility_ = new MclFacility("SCE",*sceMsgFacDisp_);
  sceMsgFacility_->minErrOutLevel(MclLevel::error ());
}

// Default constructor
LgFrScenarioForSce::LgFrScenarioForSce()
: LgFrScenario(),
  defaultSOP_(0),
  defaultSOD_(0),
  interplantSOP_(0),
  multiPlantHelper_(),
  critList_(),
  parameterSet_(),
  flatFileMgr_(),
  nStandaloneFeatureArcs_(0),
  nDemandsWithNonZeroMins_(0),
//  localSnapshotPtr_(0),
  engine_("heuristic"),
  id_(__LGFRSCENARIOFORSCE),
  exploderPtr_(0),
  sceErrFacDisp_(0),
  sceMsgFacDisp_(0),
  sceErrFacility_(0),
  sceMsgFacility_(0),
  sceScrub_(new LgFrSceScrubber())
{
  // Nothing to do here
}





#if 0

#ifdef NDEBUG
#undef NDEBUG
#endif

void
LgFrScenarioForSce::test()
{


  
  // Check the LgFrTestDataInterface
  // ===============================  
  LgFrCalendar calendar3;
  LgFrTestDataInterface protoDif3;              // The DIF that is copied into
					// the scenarios
  // Create a scenario using the builder
  LgFrScenarioBuilderForSce builder3;
  LgFrScenarioDirector director3(&builder3);  
  director3.construct(protoDif3, calendar3);
  LgFrScenarioForSceSmartPointer testScenarioPtr3 = builder3.getScenario();

  // Test the deep copy constructor
  LgFrScenarioForSceSmartPointer testScenarioDupPtr3
		 = testScenarioPtr3.deepCopy();
  
  assert(testScenarioDupPtr3->title() != testScenarioPtr3->title());
  assert(testScenarioDupPtr3->dataInterface().isA() == __LGFRTESTDATAINTERFACE);
  assert(testScenarioDupPtr3->setOfParts(LGFR_DEFAULT_SET).size() == 3);
  assert(testScenarioDupPtr3->setOfParts(LGFR_UNIVERSAL_SET).size() == 4);
  assert(testScenarioDupPtr3->setOfParts(LGFR_BUDGET_SET).size() == 1);
  assert(testScenarioDupPtr3->setOfDemands(LGFR_DEFAULT_SET).size() == 2);

  // Test the assignment operator
  LgFrScenarioForSceSmartPointer assignee;
  assignee = testScenarioPtr3;
  
  assert(assignee.title() != testScenarioPtr3->title());
  assert(assignee.dataInterface().isA() == __LGFRTESTDATAINTERFACE);
  assert(assignee.setOfParts(LGFR_DEFAULT_SET).size() == 3);
  assert(assignee.setOfParts(LGFR_UNIVERSAL_SET).size() == 4);
  assert(assignee.setOfParts(LGFR_BUDGET_SET).size() == 1);
  assert(assignee.setOfDemands(LGFR_DEFAULT_SET).size() == 2);




  // Make another deep copy
  LgFrScenarioForSceSmartPointer yetAnotherScenarioPtr
		 = testScenarioDupPtr3.deepCopy();

  // Do some downcasts
  LgFrDataInterface * tsDifPtr3 = &(testScenarioPtr3->dataInterface());
  assert (tsDifPtr3 != 0
	  &&  tsDifPtr3->isA() == __LGFRTESTDATAINTERFACE);
  LgFrTestDataInterface * tsTestDifPtr3 = (LgFrTestDataInterface *) tsDifPtr3;
	  
  LgFrDataInterface * tsDupDifPtr3 = &(testScenarioDupPtr3->dataInterface());
  assert (tsDupDifPtr3 != 0
	  &&  tsDupDifPtr3->isA() == __LGFRTESTDATAINTERFACE);
  LgFrTestDataInterface * tsDupTestDifPtr3
    = (LgFrTestDataInterface *) tsDupDifPtr3;


  // Check for typo's in the downcasting code
  assert (tsTestDifPtr3 != tsDupTestDifPtr3);

  // And test the test dif
  LgFrTestDataInterface::contextTest
    (*testScenarioPtr3, testScenarioPtr3->lastProblemSolved(),
     *tsTestDifPtr3,
     testScenarioPtr3->setOfParts (LGFR_UNIVERSAL_SET),
     testScenarioPtr3->setOfDemands (LGFR_UNIVERSAL_SET),
     *testScenarioDupPtr3, *tsDupTestDifPtr3
     );

  // clean up all the junk that was created.


  // Test the LgFrDataInterfaceFromThinAir 
  // ===================================== 
  LgFrCalendar calendar2(8, SCETime(SCEDate("7/15/94")));
  LgFrDataInterfaceFromThinAir protoDif2; // The DIF that is copied into
					// the scenarios
  // Create a scenario using the builder
  LgFrScenarioBuilderForSce builder2;
  LgFrScenarioDirector director2(&builder2);  
  director2.construct(protoDif2, calendar2);
  LgFrScenarioForSceSmartPointer testScenarioPtr2 = builder2.getScenario();

  // Shouldn't have to downcast.  Add a method in scenCMRP to get the ippCMRP
  LgFrInitialProblemForSce * testIpp2 = (LgFrInitialProblemForSce *)
			&(testScenarioPtr2->initialProblemAndParameters());
 
  // Make a copy of it
  LgFrScenarioForSceSmartPointer testScenarioDupPtr2
    = testScenarioPtr2.deepCopy();

  // Do some downcasts
  LgFrDataInterface * tsDifPtr2 = &(testScenarioPtr2->dataInterface());
  assert (tsDifPtr2 != 0
	  &&  tsDifPtr2->isA() == __LGFRDATAINTERFACEFROMTHINAIR);
  LgFrDataInterfaceFromThinAir * tsTestDifPtr2
    = (LgFrDataInterfaceFromThinAir *) tsDifPtr2;
	  
  LgFrDataInterface * tsDupDifPtr2 = &(testScenarioDupPtr2->dataInterface());
  assert (tsDupDifPtr2 != 0
	  &&  tsDupDifPtr2->isA() == __LGFRDATAINTERFACEFROMTHINAIR);
  LgFrDataInterfaceFromThinAir * tsDupTestDifPtr2
    = (LgFrDataInterfaceFromThinAir *) tsDupDifPtr2;


  // Check for typo's in the downcasting code
  assert (tsTestDifPtr2 != tsDupTestDifPtr2);

  // Check LgFrTestDataInterface
  LgFrDataInterfaceFromThinAir::contextTest
    (*testScenarioPtr2, testScenarioPtr2->lastProblemSolved(),
     *tsTestDifPtr2,
     testScenarioPtr2->setOfParts (LGFR_UNIVERSAL_SET),
     testScenarioPtr2->setOfDemands (LGFR_UNIVERSAL_SET),
     *testScenarioDupPtr2, *tsDupTestDifPtr2
     );
  

  // Test the second Thin Air, LgFrDataInterfaceFromThinAir2
  // ===================================== 
  LgFrCalendar calendarAir2(8, SCETime(SCEDate("7/15/94")));
  LgFrDataInterfaceFromThinAir2 protoDifAir2; // The DIF that is copied into
					// the scenarios
  // Create a scenario using the builder
  LgFrScenarioBuilderForSce builderAir2;
  LgFrScenarioDirector directorAir2(&builderAir2);  
  directorAir2.construct(protoDifAir2, calendarAir2);
  LgFrScenarioForSceSmartPointer testScenarioPtrAir2 = builderAir2.getScenario();

  // Shouldn't have to downcast.  Add a method in scenCMRP to get the ippCMRP
  LgFrInitialProblemForSce * testIppAir2 = (LgFrInitialProblemForSce *)
			&(testScenarioPtrAir2->initialProblemAndParameters());
 
  // Make a deep copy of it
  LgFrScenarioForSceSmartPoitner testScenarioDupPtrAir2
    = testScenarioPtrAir2.deepCopy();  
  // Do some downcasts
  LgFrDataInterface * tsDifPtrAir2 = &(testScenarioPtrAir2->dataInterface());
  assert (tsDifPtrAir2 != 0
	  &&  tsDifPtrAir2->isA() == __LGFRDATAINTERFACEFROMTHINAIR2);
  LgFrDataInterfaceFromThinAir2 * tsTestDifPtrAir2
    = (LgFrDataInterfaceFromThinAir2 *) tsDifPtrAir2;
	  
  LgFrDataInterface * tsDupDifPtrAir2 = &(testScenarioDupPtrAir2->dataInterface());
  assert (tsDupDifPtrAir2 != 0
	  &&  tsDupDifPtrAir2->isA() == __LGFRDATAINTERFACEFROMTHINAIR2);
  LgFrDataInterfaceFromThinAir2 * tsDupTestDifPtrAir2
    = (LgFrDataInterfaceFromThinAir2 *) tsDupDifPtrAir2;


  // Check for typo's in the downcasting code
  assert (tsTestDifPtrAir2 != tsDupTestDifPtrAir2);

  // Check LgFrTestDataInterface
  LgFrDataInterfaceFromThinAir2::contextTest
    (*testScenarioPtrAir2, testScenarioPtrAir2->lastProblemSolved(),
     *tsTestDifPtrAir2,
     testScenarioPtrAir2->setOfParts (LGFR_UNIVERSAL_SET),
     testScenarioPtrAir2->setOfDemands (LGFR_UNIVERSAL_SET),
     *testScenarioDupPtrAir2, *tsDupTestDifPtrAir2
     );
  
  
  // Check LgFrInitialProblemForSce
  // ===============================
  // Must re-create the scenario's because the dif's contextTest() methods
  // mucked around with  the witRun's.
  LgFrCalendar calendar4;
  LgFrTestDataInterface protoDif4;           
  // Create a scenario using the builder
  LgFrScenarioBuilderForSce builder4;
  LgFrScenarioDirector director4(&builder4);  
  director4.construct(protoDif4, calendar4);
  LgFrScenarioForSceSmartPointer testScenarioPtr4 = builder4.getScenario();
  LgFrInitialProblemForSce * testIpp4 = (LgFrInitialProblemForSce *)
			&(testScenarioPtr4->initialProblemAndParameters());
  // Make a deep copy of it
  LgFrScenarioForSceSmartPointer testScenarioDupPtr4
    = testScenarioPtr4.deepCopy();

  // Do some downcasts
  LgFrDataInterface * tsDifPtr4 = &(testScenarioPtr4->dataInterface());


  assert (tsDifPtr4 != 0
	  &&  tsDifPtr4->isA() == __LGFRTESTDATAINTERFACE);
  LgFrTestDataInterface * tsTestDifPtr4
    = (LgFrTestDataInterface *) tsDifPtr4;
	  
  LgFrDataInterface * tsDupDifPtr4 = &(testScenarioDupPtr4->dataInterface());
  assert (tsDupDifPtr4 != 0
	  &&  tsDupDifPtr4->isA() == __LGFRTESTDATAINTERFACE);
  LgFrTestDataInterface * tsDupTestDifPtr4
    = (LgFrTestDataInterface *) tsDupDifPtr4;


  // Check for typo's in the downcasting code
  assert (tsTestDifPtr4 != tsDupTestDifPtr4);
 

  LgFrCalendar calendar5(8, SCETime(SCEDate("7/15/94")));
  LgFrDataInterfaceFromThinAir protoDif5; // The DIF that is copied into
					// the scenarios
  // Create a scenario using the builder
  LgFrScenarioBuilderForSce builder5;
  LgFrScenarioDirector director5(&builder5);  
  director5.construct(protoDif5, calendar5);
  LgFrScenarioForSceSmartPointer testScenarioPtr5 = builder5.getScenario();

  LgFrInitialProblemForSce * testIpp5 = (LgFrInitialProblemForSce *)
			&(testScenarioPtr5->initialProblemAndParameters());
   // Make a deep copy of it
  LgFrScenarioForSceSmartPointer testScenarioDupPtr5
    = testScenarioPtr5.deepCopy();

  // Do some downcasts
  LgFrDataInterface * tsDifPtr5 = &(testScenarioPtr5->dataInterface());
  assert (tsDifPtr5 != 0
	  &&  tsDifPtr5->isA() == __LGFRDATAINTERFACEFROMTHINAIR);
  LgFrDataInterfaceFromThinAir * tsTestDifPtr5
    = (LgFrDataInterfaceFromThinAir *) tsDifPtr5;
	  
  LgFrDataInterface * tsDupDifPtr5 = &(testScenarioDupPtr5->dataInterface());
  assert (tsDupDifPtr5 != 0
	  &&  tsDupDifPtr5->isA() == __LGFRDATAINTERFACEFROMTHINAIR);
  LgFrDataInterfaceFromThinAir * tsDupTestDifPtr5
    = (LgFrDataInterfaceFromThinAir *) tsDupDifPtr5;


  // Check for typo's in the downcasting code
  assert (tsTestDifPtr5 != tsDupTestDifPtr5);
 


 LgFrCalendar calendar6(8, SCETime(SCEDate("7/15/94")));
  LgFrDataInterfaceFromThinAir2 protoDif6; // The DIF that is copied into
					// the scenarios
  // Create a scenario using the builder
  LgFrScenarioBuilderForSce builder6;
  LgFrScenarioDirector director6(&builder6);  
  director6.construct(protoDif6, calendar6);
  LgFrScenarioForSceSmartPointer testScenarioPtr6 = builder6.getScenario();

  LgFrInitialProblemForSce * testIpp6 = (LgFrInitialProblemForSce *)
			&(testScenarioPtr6->initialProblemAndParameters());
   // Make a deep copy of it
  LgFrScenarioForSceSmartPointer testScenarioDupPtr6
    = testScenarioPtr6.deepCopy();

  // Do some downcasts
  LgFrDataInterface * tsDifPtr6 = &(testScenarioPtr6->dataInterface());
  assert (tsDifPtr6 != 0
	  &&  tsDifPtr6->isA() == __LGFRDATAINTERFACEFROMTHINAIR2);
  LgFrDataInterfaceFromThinAir2 * tsTestDifPtr6
    = (LgFrDataInterfaceFromThinAir2 *) tsDifPtr6;
	  
  LgFrDataInterface * tsDupDifPtr6 = &(testScenarioDupPtr6->dataInterface());
  assert (tsDupDifPtr6 != 0
	  &&  tsDupDifPtr6->isA() == __LGFRDATAINTERFACEFROMTHINAIR2);
  LgFrDataInterfaceFromThinAir2 * tsDupTestDifPtr6
    = (LgFrDataInterfaceFromThinAir2 *) tsDupDifPtr6;


  // Check for typo's in the downcasting code
  assert (tsTestDifPtr6 != tsDupTestDifPtr6);
  
  LgFrInitialProblemForSce::contextTest
    (*testIpp4, *tsTestDifPtr4, *testIpp5, *tsTestDifPtr5, *testIpp6, *tsTestDifPtr6 );

  // Test LgFrScheduleFactoryForSce
  LgFrScheduleFactoryForSce::contextTest ( * (testScenarioDupPtr5
					       ->scheduleFactory())  );

  // Test LgFrInitialProblemForScewithPrioritesFromDif
  {
  LgFrDataInterfaceFromCmrpFiles difWithPriorities("../../cmrpData/difTest");
  LgFrCalendar calendar7( 3,SCETime(SCEDate(10,4,1995)),LgFrDay);
  LgFrScenarioBuilderForSce builder7;
  LgFrScenarioDirector director7(&builder7);  
  director7.construct(difWithPriorities, calendar7);
  LgFrScenarioForSceSmartPointer testScenarioPtr7 = builder7.getScenario();
  LgFrInitialProblemForScewithPrioritiesFromDif * testIpp7 = 
			(LgFrInitialProblemForScewithPrioritiesFromDif *)
			&(testScenarioPtr7->initialProblemAndParameters());
  LgFrDataInterface * tsDifPtr7 = &(testScenarioPtr7->dataInterface());
  assert (tsDifPtr7 != 0
	  &&  tsDifPtr7->isA() == __LGFRDATAINTERFACEFROMCMRPFILES);
  LgFrDataInterfaceFromCmrpFiles * tsTestDifPtr7 = 
	 (LgFrDataInterfaceFromCmrpFiles *) tsDifPtr7;
  LgFrInitialProblemForScewithPrioritiesFromDif::contextTest( 
     *testIpp7,
     *tsTestDifPtr7); 
  }

  // Now that source/rhs (testScenarioDupPtr3) has been deleted, 
  // retest the assignee and yetAnotherScenarioPtr

  assert(assignee.title() != testScenarioPtr3->title());
  assert(assignee.dataInterface().isA() == __LGFRTESTDATAINTERFACE);
  assert(assignee.setOfParts(LGFR_DEFAULT_SET).size() == 3);
  assert(assignee.setOfParts(LGFR_UNIVERSAL_SET).size() == 4);
  assert(assignee.setOfParts(LGFR_BUDGET_SET).size() == 1);
  assert(assignee.setOfDemands(LGFR_DEFAULT_SET).size() == 2);

  assert(yetAnotherScenarioPtr->title() != testScenarioPtr3->title());
  assert(yetAnotherScenarioPtr->dataInterface().isA() == __LGFRTESTDATAINTERFACE);
  assert(yetAnotherScenarioPtr->setOfParts(LGFR_DEFAULT_SET).size() == 3);
  assert(yetAnotherScenarioPtr->setOfParts(LGFR_UNIVERSAL_SET).size() == 4);
  assert(yetAnotherScenarioPtr->setOfParts(LGFR_BUDGET_SET).size() == 1);
  assert(yetAnotherScenarioPtr->setOfDemands(LGFR_DEFAULT_SET).size() == 2);


}


 
#endif

