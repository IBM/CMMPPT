#ifndef SCENARIOFORSCE_H
#define SCENARIOFORSCE_H

// RW2STL - inserted:
#include <string>
#include <fstream>
// End of RW2STL - inserted includes.

//#include <sce/src/sceErr.h>

#include <scenario/src/scenario.h>
#include <sce/src/sceDefin.h>
#include <sce/src/sceParam.h>
#include <sce/src/mPlantH.h>
#include <sce/src/sceCrtLs.h>
#include <sce/src/fFileMgr.h>
#include <sce/src/aEngMgr.h>

#include <sce/src/scrub.h>

#include <mcl/src/mcl.h>
#include <sce/src/message.h>

#ifdef SCE_ODBC
#include <sce/src/db.h>
#endif

// Set codes.  Not using enums because subclasses can have more sets
// The following were set by the base class
#define LGFR_DEFAULT_SET       0
#define LGFR_UNIVERSAL_SET     1
#define LGFR_INTERPLANT_SET    2



// forward declarations
class LgFrInitialProblemAndParameters;
class SCETokenizer;
class LgFrSceBaseExploder;

// added to avoid syntax error - vra
class LgFrScenarioForSceSmartPointer;

class LgFrScenarioForSce : public LgFrScenario
{

  // friends (need access to scenario-specific set of Parts)
friend class LgFrInitialProblemForSce;
friend class LgFrScenarioForSceSmartPointerToConst;
friend class LgFrScenarioForSceSmartPointer;

  // Friend to allow heuristic engine manager to add demands
friend class LgFrSceHeurEngineMgr;

  public:



// SCE 6.1
int
printAPegFile(WitRun * const theWitRun, 
	      LgFrMultiPlantHelper & mpHelper,
	      LgFrCalendar & theCal,
	      int pegType, 
	      int tinpIndpOrBoth,
	      std::string & pegFileName,
	      int nParts,
	      char ** partList,
	      int nPeriods);


  virtual int GetScenarioType() const { return __LGFRSCENARIOFORSCE; }

  
  void 
  printTotalDemandFile(WitRun * const theWitRun,   // WIT environment
                       LgFrSceBaseExploder * exploder,
                       const int nPeriods,         // Number of periods 
                       const int nParts,           // Number of parts   
                       char ** partList,           // List of part names
                       std::string fileName,
                       LgFrMultiPlantHelper & mpHelper);

// prints a record of a DB Dump file
// returns 1 if printed, 0 if not
int
printDbDumpRec(
    std::string & pn, 
    std::string & pdf, 
    const char * vecID,
    const float *  floatVec,
    std::ofstream & outFile,   // output stream (already opened)
    int nPeriods, 
    int printIfVectorIsAllZero);

// prints a (boolean) record of a DB Dump file
// returns 1 
int
printDbDumpRecBool(
    std::string & pn, 
    std::string & pdf, 
    const char * vecID,
    const witBoolean *  boolVec,
    std::ofstream & outFile,   // output stream (already opened)
    int nPeriods);


  
  
  // return a exploder object
  LgFrSceBaseExploder *
  exploder();

  // this is a quick and dirty implementation of exploderFactory
  // Eventually, it should be moved to exploders.h (or something).
  LgFrSceBaseExploder *
  createAppropriateSceExploder(LgFrScenarioForSceSmartPointer sceScenSP);

  void   
  runExplosion(LgFrScenarioForSceSmartPointer scenSceSP);

  void   
  printExplosionResults();
  
  void
  printModelStructureOutputs();

  int
  nStandaloneFeatureArcs()
  const;
  
  void
  nStandaloneFeatureArcs(int nFeats);

  void
  incrementStandaloneFeatureArcs();
  
  int
  nDemandsWithNonZeroMins()
  const;

  void
  nDemandsWithNonZeroMins(int nMins);

  void
  incNDemandsWithNonZeroMins();

  void
  decNDemandsWithNonZeroMins();

  void
  setEngine(const std::string & engine);

  std::string
  getEngine();

  // moving to FFmgr
  //  void 
  //  LgFrScenarioForSce::writeFloatVector(
  // std::ofstream & theStream,   // output stream (already opened)
  // int    vLength,         // lenght of the vector (nPeriods?)
  // float * theVector,      // the vector itself
  // int precision,          // number of digits to the right of the decimal
  // int totalSpace);



  int 
  determineGeoToPdfSourceVector(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & geoPlannerDemandPartName,
    std::string * sourceList);

  void 
  analyzeRunAndPrintResults();
  
  void
  printHumanReadableSupplyPlanSoln(
    WitRun * const theWitRun,   // WIT environment
    const int nPeriods,         // Number of periods
    const int nParts,           // Number of parts
    char ** partList,           // List of part names
    std::string fileName,         // Name of output filename
    LgFrCalendar & theCal,
    LgFrMultiPlantHelper & mpHelper);


  void
  printBacklogSchedule(
                       WitRun * const theWitRun,   // WIT environment
                       const int nPeriods,         // Number of periods
                       const int nParts,           // Number of parts
                       char ** partList,           // List of part names
                       std::string fileName,
                       LgFrCalendar & theCal,
                       LgFrMultiPlantHelper & mpHelper);


  void 
  printIndepSupplyPlanSoln(
			   WitRun * const theWitRun,   // WIT environment   
			   const int nPeriods,         // Number of periods 
			   const int nParts,           // Number of parts   
			   char ** partList,           // List of part names
			   std::string fileName,
			   LgFrCalendar & theCal,
			   LgFrMultiPlantHelper & mpHelper);


  void
  printSupplyPlanSoln(
    WitRun * const theWitRun,   // WIT environment
    const int nPeriods,         // Number of periods
    const int nParts,           // Number of parts
    char ** partList,           // List of part names
    const int tinpIndpOrBoth,
    std::string fileName,         // Name of output filename
    LgFrCalendar & theCal,
    LgFrMultiPlantHelper & mpHelper);


 void 
setHeuristicSearchIncrement(WitRun * const theWitRun,   
			    std::string greedySearchIncrementMethod, 
			    float anIncValue);
  

  void printBuildSchedule(
         WitRun * const theWitRun,   // WIT environment
         const int nPeriods,         // Number of periods
         const int nParts,           // Number of parts
         char ** partList,           // List of part names
         std::string fileName,
         LgFrCalendar & theCal,
         LgFrMultiPlantHelper & mpHelper);



  //  ---------------------------------------------------------------------
  //  Print Operation Orders File (as determined by implosion)
  //  Prints execVol of operations that are:
  //  (1) explicitely defined by the user in the operation Definition File
  //  (2) SCE-defined generated from parts defined in the part Definition File
  //  --------------------------------------------------------------------- 
  void printOperationOrdersFile(
         WitRun * const theWitRun,   // WIT environment
         const int nPeriods,         // Number of periods
         std::string fileName,
         LgFrMultiPlantHelper & mpHelper);

  //  ---------------------------------------------------------------------
  //  Print Operation Demand File (as determined by explosion)
  //  Prints mrpExecVol of operations that are:
  //  (1) explicitely defined by the user in the operation Definition File
  //  (2) SCE-defined generated from parts defined in the part Definition File
  //  --------------------------------------------------------------------- 
  void 
  printOperationDemandVolFile(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 std::string fileName,
	 LgFrMultiPlantHelper & mpHelper);


#ifdef ENABLE_NEGATIVE_DEMANDS
  LgFrTimeVecFloat excessVolAdjustedForNegativeDemands(
    float * witExcessVol,
    float * supplyVol,
    LgFrTimeVecFloat & negDemVol,
    int nPeriods);
#endif

  

  //  ---------------------------------------------------------------------
  //  Print DEBUG version critical parts list ... prints out specialParts 
  //      good for debugging
  //  ---------------------------------------------------------------------
  void  printDebugCriticalList(
			       WitRun * const theWitRun,   // WIT environment
			       std::string engine,
			       const int nPeriods,         // Number of periods
			       const int nParts,           // Number of parts
			       char ** partList,           // List of part names
			       std::string fileName,
			       LgFrCalendar & theCal,
			       LgFrMultiPlantHelper & mpHelper,
			       LgFrSceCritList & theCritList);



  //  ---------------------------------------------------------------------
  //  Print critical parts list
  //  ---------------------------------------------------------------------
  void  printCriticalList(
         WitRun * const theWitRun,   // WIT environment
         std::string engine,
         const int nPeriods,         // Number of periods
         const int nParts,           // Number of parts
         char ** partList,           // List of part names
         std::string fileName,
         LgFrCalendar & theCal,
         LgFrMultiPlantHelper & mpHelper,
         LgFrSceCritList & theCritList);

  //  ---------------------------------------------------------------------
  //  Print pegged critical parts list
  //  ---------------------------------------------------------------------
  void printPeggedCriticalList(
	 WitRun * const theWitRun,   // WIT environment
	 std::string engine,            // only works for heuristic allocation
	 std::string fileNameBox,
	 std::string fileNameIndp,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper);

  // SCE 6.1 for use with multiAttribute.   It prints to one and only one file for both INDP and TINP
  //  ---------------------------------------------------------------------
  //  Print pegged critical parts list
  //  ---------------------------------------------------------------------
  void printPeggedCriticalList(
	 WitRun * const theWitRun,   // WIT environment
	 std::string engine,            // only works for heuristic allocation
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper);





  void 
  printShadowPrices(
		    WitRun * const theWitRun,   // WIT environment   
		    const int nPeriods,         // Number of periods 
		    int nParts,
		    char ** partList,
		    std::string fileName,
		    LgFrMultiPlantHelper & mpHelper);
  




  //  ---------------------------------------------------------------------
  //  Print Interplant Volumes (can be used for imploion or explosion)
  //  --------------------------------------------------------------------- 
  void   printInterplantVolumesFile(
				    WitRun * const theWitRun,
				    const int nPeriods,      
				    int nOps,
				    char ** opList,
				    bool isThisMrp,
				    std::string fileName,
				    LgFrMultiPlantHelper & mpHelper);

//  ---------------------------------------------------------------------
//  Print AlternatePart Volumes (can be used by implosion or explosion)
//  --------------------------------------------------------------------- 
void 
printAlternateDbFile(
	 WitRun * const theWitRun,   // WIT environment   
	 const int nPeriods,         // Number of periods 
	 int nOps,
	 char ** opList,
	 bool isThisMrp,
	 std::string fileName,
	 LgFrMultiPlantHelper & mpHelper);



  // two helper methods to retrieve the adjusted  interplant exec Vols
  LgFrTimeVecFloat interplantExecVol(std::string & operationName);
  LgFrTimeVecFloat interplantMrpExecVol(std::string & operationName);

  //  ---------------------------------------------------------------------
  //  initializes the adjusted interplant execVols for mrp (sets to 0 for implosion)
  //  ---------------------------------------------------------------------
  void initializeInterplantOperationsAppData(
					     WitRun * const theWitRun,
					     const int nPeriods,                              
					     const int nOperations,
					     char ** operationList);

  //  ---------------------------------------------------------------------
  //  sets the adjusted interplant execVols for  implosion
  //  ---------------------------------------------------------------------
  void setInterplantOperationsAppDataForImplosion(
						    WitRun * const theWitRun,
						    const int nPeriods,                              
						    const int nOperations,
						    char ** operationList);


  void printCapUsageVolume(WitRun * const theWitRun, 
			   bool isItMrp);


  //  ---------------------------------------------------------------------
  //  Print Substitute Prod vols
  //  ---------------------------------------------------------------------
  void printSubstituteProductionVolume(
    WitRun * const theWitRun,   // WIT environment
    const int nPeriods,         // Number of periods
    const int nParts,           // Number of parts
    char ** partList,           // List of part names
    std::string fileName,
    LgFrCalendar & theCal,
    LgFrMultiPlantHelper & mpHelper);

  void printMrpSubstituteProductionVolume(
    WitRun * const theWitRun,   // the main witrun
    const int nPeriods,         // Number of periods 
    const int nParts,           // Number of parts   
    char ** partList,           // List of part names
    std::string fileName,
    LgFrMultiPlantHelper & mpHelper);

  void printOpSubstituteProductionVolume(
    WitRun * const theWitRun,   // WIT environment
    const int nPeriods,         // Number of periods
    const int nOperations,           // Number of operation
    char ** opList,           // List of operation names
    std::string fileName,
    LgFrCalendar & theCal,
    LgFrMultiPlantHelper & mpHelper);


//  ---------------------------------------------------------------------
//  Print Substitute Prod Volumes
//  Generalized procedure: Use it for printing:
//     subUsage, mrpSubUsage, opSubUsage, mrpOpSubUsage
//  As more and more of these sub usage beasts are needed in the future, 
//  keep expanding the boolean types (for example isChildACapacity ...)
//  --------------------------------------------------------------------- 
void 
printSubstituteUsageVolume(
         WitRun * const theWitRun,
	 bool parentMustBeUserDefinedOperation,
	 bool engineMustBeImplode,
         const int nPeriods,      
         const int nOperations, 
         char ** opList,        
         std::string fileName,
	 LgFrMultiPlantHelper & mpHelper);

void 
printSubDbDumpFile(
	 WitRun * const theWitRun,
	 bool parentMustBeUserDefinedOperation,
	 bool engineMustBeImplode,
	 const int nPeriods,      
	 const int nOperations, 
	 char ** opList,        
	 std::string fileName,
	 LgFrMultiPlantHelper & mpHelper);

#if 0 
  // obsolete ...  BUILDING_BLOCKS_VIA_MULTI_BOPS
  void printFeatureAnalysisReportViaMultiRoute(
         WitRun * const theWitRun,   // WIT environment
         const int nPeriods,         // Number of periods
         const int nParts,           // Number of parts
         char ** partList,           // List of part names
         std::string fileName,
         LgFrCalendar & theCal,
         LgFrMultiPlantHelper & mpHelper,
         int printDataAsPercentage);
#endif

  void printFeatureAnalysisReportViaProportionalRoute(
	 WitRun * const theWitRun,
	 const int nPeriods,
	 const int nOperations,
	 char ** operationList,
	 std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper,
	 int printDataAsPercentage);

  void printFeatureAnalysisReport(
         WitRun * const theWitRun,   // WIT environment
         const int nPeriods,         // Number of periods
         const int nParts,           // Number of parts
         char ** partList,           // List of part names
         std::string fileName,
         LgFrCalendar & theCal,
         LgFrMultiPlantHelper & mpHelper,
         int printDataAsPercentage);


  void printStandAloneFeatureAnalysisReport(
         WitRun * const theWitRun,   // WIT environment
         const int nPeriods,         // Number of periods
         const int nParts,           // Number of parts
         char ** partList,           // List of part names
         std::string fileName,
         LgFrCalendar & theCal,
         LgFrMultiPlantHelper & mpHelper,
         int printDataAsPercentage);

  //  Dump Capacity   Information
  void
  printCapacityDataBaseDumpFile(
    WitRun * const theWitRun,   // WIT environment
    LgFrMultiPlantHelper & mpHelper,
    const int nPeriods,         // Number of periods
    const int nParts,           // Number of parts
    char ** partList,           // List of part names
    std::string fileName);


  //  Dump Operation Information
  void
  printOperationDataBaseDumpFile(
    WitRun * const theWitRun,   // WIT environment
    LgFrMultiPlantHelper & mpHelper,
    const int nPeriods,         // Number of periods
    std::string fileName);

void 
  printOperationExecDumpFile(
    WitRun * const theWitRun,   // WIT environment
    LgFrMultiPlantHelper & mpHelper,
    const int nPeriods,         // Number of periods 
    std::string fileName);



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
  //  "F" = build in-house
  //  "O" = user-defined Operation
  //  "D" = de-aggregation
  //  "E" = external (ie, supplyvol)
  void 
  printMrpSupplyOrdersFile(
                 LgFrMultiPlantHelper & mpHelper,
                 const int nPeriods,         // Number of periods 
                 std::string fileName);


  //  ---------------------------------------------------------------------
  //  Print MRP Demand Orders File
  //  ---------------------------------------------------------------------
  //
  //  Codes are:
  //  "A" = interplant
  //  "F" = build in-house (includes subs, features, and BB's)
  //  "O" = user-defined Operation
  //  "D" = de-aggregation
  //  "E" = external (ie, supplyvol)
  void 
  printMrpDemandOrdersFile(
                 LgFrMultiPlantHelper & mpHelper,
                 const int nPeriods,         // Number of periods 
                 std::string fileName);
  
  
  
  //  ---------------------------------------------------------------------
  //  Print GEO Supply By PDF  Information         
  //  --------------------------------------------------------------------- 
  void 
  printGeoSourceDebugFile(
         WitRun * const theWitRun,   // WIT environment   
         const int nPeriods,         // Number of periods 
         const int nParts,           // Number of parts   
         char ** partList,           // List of part names
         std::string fileName,
	 LgFrCalendar & theCal,
	 LgFrMultiPlantHelper & mpHelper);
  


//  ---------------------------------------------------------------------
//  Print Lowest Level, unconstrained parts (non-pcf)
//  --------------------------------------------------------------------- 
  void 
  printLowestLevelNonPcfParts(
	 WitRun * const theWitRun,   // WIT environment
	 LgFrMultiPlantHelper & mpHelper,
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName);

#ifdef MARK_BOGONS
//  ---------------------------------------------------------------------
//  Print PCF Bogons (parts which were specified as PCF but got rejected due to child parts)
//  --------------------------------------------------------------------- 
void 
printBogonParts(
	 WitRun * const theWitRun,   // WIT environment
	 LgFrMultiPlantHelper & mpHelper,
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string fileName);
#endif

void 
calculateAndPrintValueAddedCosts(
	 WitRun * const theWitRun,   // WIT environment
	 LgFrMultiPlantHelper & mpHelper,
	 const int nPeriods,         // Number of periods 
	 const int nParts,           // Number of parts   
	 char ** partList,           // List of part names
	 std::string vacFileName,
	 std::string compCostFileName);




  //  Print Overall Problem Statistics
  void
  printOverallProblemStatistics(
         WitRun * const theWitRun,   // WIT environment
         LgFrMultiPlantHelper & mpHelper,
         const int nPeriods,         // Number of periods
         const int nParts,           // Number of parts
         char ** partList,           // List of part names
         std::string fileName);


  void
  printOverallOptimizationValues(
    WitRun * const theWitRun,   // WIT environment
    const int nPeriods,         // Number of periods
    const int nParts,           // Number of parts
    char ** partList,           // List of part names
    std::string & engine,
    std::string fileName);

  void
  runImplosion( LgFrScenarioForSceSmartPointer );

  void
  machineFss();
  
  void
  generalSetFss();
  
  void
  singletonFss();


  void
  loadPIPDataIntoAppData(WitRun * const theWitRun);

  void
  printPIPData(WitRun * const theWitRun, int isItMrp);

  
  LgFrMultiPlantHelper &
  multiPlantHelper();

  LgFrSceCritList &
  sceCritList();

  void 
  printDummyPartDataBaseDumpFile(
                                 WitRun * const theWitRun,   // WIT environment
                                 LgFrMultiPlantHelper & mpHelper,
                                 const int nPeriods,         // Number of periods 
                                 const int nParts,           // Number of parts   
                                 char ** partList,           // List of part names
                                 std::string fileName);
  
  void 
  printPartDataBaseDumpFile(
                            WitRun * const theWitRun,   // WIT environment
                            LgFrMultiPlantHelper & mpHelper,
                            const int nPeriods,         // Number of periods 
                            const int nParts,           // Number of parts   
                            char ** partList,           // List of part names
                            std::string fileName);

  void 
  setWitInformationalMessaging(WitRun * const theWitRun,
                               int onOrOff);

  
  virtual
     isAReturnType
        isA()
           const;

// return the set of paramters 
  LgFrSceSetOfParameters &
  setOfParameters();

// return the flat file manager
  LgFrSceFlatFileMgr &
  flatFileMgr();


  // RW locale
//  virtual
//    void
//        localeSnapshot(
//                   RWLocaleSnapshot * localSnapshotPtr);

  // Return a const setOfParts which belongs to this scenario
  virtual
     const 
        LgFrSetOfParts&
           constSetOfParts(int k = LGFR_DEFAULT_SET)
              const;



#ifdef SCE_ODBC 
    // --------- Adding new methoads for updating DB with the sce output
  
   
  void analyzeRunAndUpdateDB();
  
  void insertDemandSupplyVol(SceDBLink&,                 // to hold supplyVol
                             WitRun * const theWitRun,   // WIT environment
                             const int nPeriods,         // Number of periods
                             const int nParts,           // Number of parts
                             char ** partList,           // List of part names
                             LgFrCalendar & theCal,
                             LgFrMultiPlantHelper & mpHelper);

  void insertDemandBacklogSchedule(SceDBLink&,                 // to hold supplyVol
                             WitRun * const theWitRun,   // WIT environment
                             const int nPeriods,         // Number of periods
                             const int nParts,           // Number of parts
                             char ** partList,           // List of part names
                             LgFrCalendar & theCal,
                             LgFrMultiPlantHelper & mpHelper);

  void insertCapacitySupplyVol(SceDBLink&,             // to hold supplyVol
                               WitRun * const theWitRun,   // WIT environment
                               const int nPeriods,         // Number of periods
                               const int nParts,           // Number of parts
                               char ** partList,           // List of part names
                               LgFrMultiPlantHelper & mpHelper);

  void insertCapacityMrpConsVol(SceDBLink& mrpConsVol,        
                                WitRun * const theWitRun, 
                                const int nPeriods,        
                                const int nParts,      
                                char ** partList,     
                                LgFrMultiPlantHelper & mpHelper);

  void insertCapacityReqVolAndMrpExcessVolAndMrpRequirements(
                                             SceDBLink& reqVol,
                                             SceDBLink& mrpExcessVol,
                                             SceDBLink& mrpReqVol,    
                                             WitRun * const theWitRun, 
                                             const int nPeriods,        
                                             const int nParts,      
                                             char ** partList,     
                                             LgFrMultiPlantHelper & mpHelper);
  
void insertCapacityExcessVol(
                             SceDBLink& excessVol,    
                             WitRun * const theWitRun, 
                             const int nPeriods,        
                             const int nParts,      
                             char ** partList,     
                             LgFrMultiPlantHelper & mpHelper);
  
void insertCapacityScrapVol(
                             SceDBLink& scrapVol,    
                             WitRun * const theWitRun, 
                             const int nPeriods,        
                             const int nParts,      
                             char ** partList,     
                             LgFrMultiPlantHelper & mpHelper);

void insertCapacityConsVol(
                             SceDBLink& consVol,    
                             WitRun * const theWitRun, 
                             const int nPeriods,        
                             const int nParts,      
                             char ** partList,     
                             LgFrMultiPlantHelper & mpHelper);

void insertIPOrders(
                    SceDBLink& ipOrders,  
                    WitRun * const theWitRun,   // WIT environment   
                    const int nPeriods,         // Number of periods 
                    const int nParts,           // Number of parts   
                    char ** partList,           // List of part names
                    LgFrCalendar & theCal,
                    LgFrMultiPlantHelper & mpHelper);

void insertSupplyResponse(
                          SceDBLink& supply,
                          WitRun * const theWitRun,   // WIT environment   
                          const int nPeriods,         // Number of periods 
                          const int nParts,           // Number of parts   
                          char ** partList,           // List of part names
                          LgFrCalendar & theCal,
                          LgFrMultiPlantHelper & mpHelper);

void insertPartPdfTotalDemand(
                         SceDBLink& demand,
                         WitRun * const theWitRun,   // WIT environment
                         LgFrSceBaseExploder * exploder,
                         const int nPeriods,         // Number of periods 
                         const int nParts,           // Number of parts   
                         char ** partList,           // List of part names
                         LgFrMultiPlantHelper & mpHelper);

  
void insertPartPdfProdVol(
                          SceDBLink & prodVol,
                          WitRun * const theWitRun,   // WIT environment   
                          const int nPeriods,         // Number of periods 
                          const int nParts,           // Number of parts   
                          char ** partList,           // List of part names
                          LgFrCalendar & theCal,
                          LgFrMultiPlantHelper & mpHelper);
  
void insertPartPdfSupplyVol(
                     SceDBLink& supplyVol,
                     WitRun * const theWitRun,   
                     const int nPeriods,  
                     const int nParts,    
                     char ** partList,     
                     LgFrMultiPlantHelper & mpHelper);

void insertPartPdfCycleTime(
                     SceDBLink& cycleTime,
                     WitRun * const theWitRun,   
                     const int nPeriods,  
                     const int nParts,    
                     char ** partList,     
                     LgFrMultiPlantHelper & mpHelper);
  
void insertPartPdfMrpConsVol(
                     SceDBLink& mrpConsVol,
                     WitRun * const theWitRun,   
                     const int nPeriods,  
                     const int nParts,    
                     char ** partList,     
                     LgFrMultiPlantHelper & mpHelper);

void insertPartPdfReqVolAndMrpExcessVolAndMrpRequirements(
                     SceDBLink& reqVol,
                     SceDBLink& mrpExcessVol,
                     SceDBLink& mrpRequirements,
                     WitRun * const theWitRun,   
                     const int nPeriods,  
                     const int nParts,    
                     char ** partList,     
                     LgFrMultiPlantHelper & mpHelper);

void insertPartPdfMrpProdVol(
                     SceDBLink& mrpProdVol,
                     WitRun * const theWitRun,   
                     const int nPeriods,  
                     const int nParts,    
                     char ** partList,     
                     LgFrMultiPlantHelper & mpHelper);

void insertPartPdfExcessVol(
                     SceDBLink& excessVol,
                     WitRun * const theWitRun,   
                     const int nPeriods,  
                     const int nParts,    
                     char ** partList,     
                     LgFrMultiPlantHelper & mpHelper);

void insertPartPdfStockVol(
                     SceDBLink& stockVol,
                     WitRun * const theWitRun,   
                     const int nPeriods,  
                     const int nParts,    
                     char ** partList,     
                     LgFrMultiPlantHelper & mpHelper);

void insertPartPdfScrapVol(
                     SceDBLink& scrapVol,
                     WitRun * const theWitRun,   
                     const int nPeriods,  
                     const int nParts,    
                     char ** partList,     
                     LgFrMultiPlantHelper & mpHelper);  
  
void insertPartPdfConsVol(
                     SceDBLink& consVol,
                     WitRun * const theWitRun,   
                     const int nPeriods,  
                     const int nParts,    
                     char ** partList,     
                     LgFrMultiPlantHelper & mpHelper);
  
void updateAttrFromPartDataBaseDump(
                     WitRun * const theWitRun,   
                     const int nPeriods,  
                     const int nParts,    
                     char ** partList,     
                     LgFrMultiPlantHelper & mpHelper);

 
   //--------------------------------------------------------------------------
#endif

  
#if 0
  // Temporary for testing cmrpTest
  LgFrLastProblemSolved &
    lps()
  {
     return LgFrScenario::lastProblemSolved();
  }
#endif

  // Return a const setOfDemands which belongs to this scenario
  virtual
     const
        LgFrSetOfDemands&
           constSetOfDemands(int k = LGFR_DEFAULT_SET)
              const;


  //--------------------------------------------------------------------------
  //   NOTE:  The following are defined in message.C  NOT in scenSce.C
  //   all methods and objects of the LgFrScenarioForSce class related to
  //   messaging are in the message.* files.
  //--------------------------------------------------------------------------

  // return pointer to facility
  MclFacility * getErrFacility();  
  MclFacility * getMsgFacility();

  // initialize the error facility messages to facility
  void initErrFacility(std::string buffername);

  // initialize the non-error messaging facility
  void initMsgFacility(std::string buffername);

  // add messages to a given facilty by reading messages from a file
  int readMessages(MclFacility * theFacil, const char * msgFileName);

  // add default messages to facility
  void loadDefaultMsgs(MclFacility * theFacil, const messageStructure msgArray[], unsigned int upTill);

  //----------------------End of Messaging Methods----------------------------

  // scrubber

  LgFrSceScrubber * scrubber(){ return sceScrub_; }

  // Constructor
  LgFrScenarioForSce ( const LgFrDataInterfaceAbstract&,
		        const LgFrCalendar&);

  // Assignment operator.
  LgFrScenarioForSce&
    operator=(
              const LgFrScenarioForSce& rhs);

  void deleteAppDataAndScenarioObjects();


  // Destructor
  ~LgFrScenarioForSce();


  // Copy constructor
  LgFrScenarioForSce(
               const LgFrScenarioForSce& source);

  // Default Constructor
  LgFrScenarioForSce();

  // Self-test
  static void test();

 protected:


  // Return a non-const setOfParts which belongs to this scenario
  virtual 
     LgFrSetOfParts&
        setOfParts(int k = LGFR_DEFAULT_SET);


  // Return a non-const setOfDemands which belongs to this scenario
  virtual
     LgFrSetOfDemands&
        setOfDemands(int k= LGFR_DEFAULT_SET);
  
 private:

  LgFrSetOfPartsWithMovableParts defaultSOP_;     // Set of default Parts
  LgFrSetOfDemands defaultSOD_;                   // Set of default Demands
  
  LgFrSetOfPartsWithMovableParts interplantSOP_;  // Set of interplant parts
  LgFrMultiPlantHelper multiPlantHelper_;

  LgFrSceCritList critList_;
  LgFrSceSetOfParameters parameterSet_;           // the Sce parameter set
  LgFrSceFlatFileMgr flatFileMgr_;                // flat file utilities

  // the message facilities that allow for error/non-error messaging
  MclFacility * sceErrFacility_;
  MclFacility * sceMsgFacility_;
  MclPrintfOutputDisplayer * sceErrFacDisp_;
  MclPrintfOutputDisplayer * sceMsgFacDisp_;

  // this is to keep track of the number of temporary Error/Msg files
  // created by the copy constructor, so we can avoid conflicts
  static unsigned int numTempFiles_;

  static const messageStructure errorMsgs_[];
  static const messageStructure nonerrMsgs_[];

  // scrubber

  LgFrSceScrubber * sceScrub_;
  
//  RWLocaleSnapshot * localSnapshotPtr_;
  std::string engine_;

   // ID tag for casting smart pointers
  int id_;
  
  int nStandaloneFeatureArcs_;
  int nDemandsWithNonZeroMins_;
  LgFrSceBaseExploder * exploderPtr_;
};

#endif

