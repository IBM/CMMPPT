// File:   ipSce.h

#ifndef LGFRINITIALPROBLEMFORSCE_H
#define LGFRINITIALPROBLEMFORSCE_H

// RW2STL - inserted:
#include <string>
#include <fstream>
// End of RW2STL - inserted includes.

// Initial problem for the Supply Chain Engine


#include <scenario/src/machdep.h>
#include <sce/src/sceDefin.h>

#include <scenario/src/initProb.h>
#include <scenario/src/scenP.h>
#include <sce/src/scenSceP.h>
#include <sce/src/modelHlp.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>




#include <iostream>
#include <fstream>
#include <limits.h>
#include <float.h>


// forward references
class WitRun;
class SCETokenizer;
class LgFrSceCritList;
class LgFrCalendar;
class LgFrMultiPlantHelper;
class LgFrSetOfParts;
class LgFrSetOfDemands;
class LgFrDataInterface;
class LgFrDataInterfaceEssencials;
class LgFrSceScrubber;
// class LgFrTestDataInterface;
// class LgFrDataInterfaceFromThinAir;
// class LgFrDataInterfaceFromThinAir2;
// class LgFrDataInterfaceFromCmrpFiles;


class LgFrInitialProblemForSce: public LgFrInitialProblemAndParameters {

public:


void
process61TopLevelToOptionalFeatureFile(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


  // Populates the LgFrWitRunVeneer's data structures using the data
  // from a dataInterface object.  It also populates any CMRP-specific
  // setOfParts.
virtual
void
feedInto(LgFrLastProblemSolved & lps);

  // Returns a description of the problem that is suitable for
  // use as a window heading. This can not be used to set the
  // title; only to query it.
virtual
std::string
title(const LgFrDataInterfaceEssentials* difEssenPtr = 0)
       const;

// we're going to give out the actual pointer.  Be careful!!  
virtual  
LgFrScenarioForSceSmartPointer
sceScenario()
  const;
  
  // Create a copy of the *this on the heap and return a pointer to it
  // Even thought this returns a LgFrInitialProblemAndParameters*, the pointer
  // must point to a LgFrInitialProblemForSce.
virtual
LgFrInitialProblemAndParameters*
clone()
       const;

  // Make a clone copy of this object with new Scenario
virtual
LgFrInitialProblemAndParameters*
clone ( LgFrScenarioSmartPointer )
        const;

// the preferred constructor
LgFrInitialProblemForSce(LgFrScenarioSmartPointer);

  // Destructor
~LgFrInitialProblemForSce();

  // Assignment operator.
LgFrInitialProblemForSce&
operator=(
          const LgFrInitialProblemForSce& rhs);


  // Copy constructor
LgFrInitialProblemForSce(
              const LgFrInitialProblemForSce& source);

  // Default Constructor
LgFrInitialProblemForSce();


void
completeTheInitialization(
    std::string & parameterFilename,
    LgFrCalendar & theCal,
    WitRun * theWitRun);

int
setTheCalendar(
    LgFrCalendar & theCal);

void
buildTheWitModelForSce(
    WitRun * const theWitRun,
    LgFrCalendar & theCal,
    LgFrMultiPlantHelper & mpHelper);

void 
openInputFileAndScrubOutputFile(
    const std::string & fileName,
    std::string fileDescription,
    std::ifstream & inFile,
    std::ofstream & outFile,
    LgFrSceScrubber * myScrubber,
    MclFacility * theErrFacil);


void
processPartDefinitionFile(
    
    WitRun * const theWitRun,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


void
processCapacityDefinitionFile(
    
    WitRun * const theWitRun,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void 
processSourceDefFile(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


void  
processOperationDefinitionFile(
    
    WitRun * const theWitRun,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);
  
void
processProdVolLBFile(
    
    WitRun * const theWitRun,   //
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processProdVolUBFile(
    
    WitRun * const theWitRun,   //
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

//
void
processOperationLBFile(
    
    WitRun * const theWitRun,   //
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processOperationUBFile(
    
    WitRun * const theWitRun,   //
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processSupplyFile(
    WitRun * const theWitRun,   //
    const LgFrCalendar & theCal,
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


void 
processSupplyFile61(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processCapacitySupplyFile61(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,    
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


void
processCapacitySupplyFile(
    
    WitRun * const theWitRun,
    const int nPeriods,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);



void
processPartsConditionFile(
    
    WitRun * const theWitRun,   //
    const LgFrCalendar & theCal,    
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


void 
scrubPartsConditionFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,    
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void 
scrubSupplyAvailabilityFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,    
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void 
scrubDemandVolumeFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,    
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);
 
void 
scrubMinDemandVolumeFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,    
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void 
processMinIndepDemandVolFile(
    WitRun * const theWitRun,  
    const int nPeriods,        
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


void 
deleteUnnecessaryDemandAppData(
    WitRun * const theWitRun,
    const int nPeriods,         
    LgFrMultiPlantHelper & mpHelper);

void
processStandardBomFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processBillOfCapacitiesFile(
    
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processOperationBomFile(
    
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processOperationBopFile(
    
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


  
void
processAggregationFile(
    
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


void processInterPlantFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


void processOperationSubBomFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);



void processStandardBomSubFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void processTopLevelToFeatureSetFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void processTopLevelToFeatureSetFile61(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


void processFeatureSetToOptionRatioFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


void processFeatureSetToOptionRatioFile61(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void processTopLevelToOptionalFeatureFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processMachineFeatureOffsetFile(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processMachineFeatureSupplyFile(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


int
preProcessNegativeDemands (
    WitRun * const theWitRun,
    LgFrMultiPlantHelper & mpHelper );



void processOptionRevenueFile(
    
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper,
    const float defaultOptionShipReward,
    const float periodicInterestRate);


  // SCE 6.1
void 
processDemandFile(
		  WitRun * const theWitRun,   
		  const LgFrCalendar & theCal,
		  const int nPeriods,         
		  const std::string & fileName,
		  LgFrMultiPlantHelper & mpHelper);

  // SCE 6.1
int
addGeoSourceAndDemandVolFromTINP(
				 WitRun * const theWitRun,
				 const std::string &  dataLine,
				 const std::string & plannerPartName,
				 const std::string & geo,
				 const std::string & pdf,
				 const std::string & requestDate,
				 float demandQty,
				 const std::string & fileName,
				 const long  lineNo,
				 const LgFrCalendar & theCal,
				 LgFrMultiPlantHelper & mpHelper);

// SCE 6.1
int 
addDemandVolFromINDP(WitRun * const theWitRun,
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
		     LgFrMultiPlantHelper & mpHelper);


void
processGeoSourceFile(
    
    WitRun * const theWitRun,
    const std::string & geoSourceFile,
    const LgFrCalendar & theCal,
    LgFrMultiPlantHelper & mpHelper);


void 
processIndependentDemandFile(
    WitRun * const theWitRun,   
    const LgFrCalendar & theCal,
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);
  
void 
processIndepDemandObjectiveFile(
    WitRun * const theWitRun,   // 
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper,
    const float periodicInterestRate);


void
processDemandVolumeFile(
    WitRun * const theWitRun,
    const int nPeriods,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);



void
processMinDemandVolFile(
    WitRun * const theWitRun,   //
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processBacklogYieldFile(
    WitRun * const theWitRun,   //
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processGreedySearchIncFile(
    WitRun * const theWitRun,   //
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void 
processIndpGreedySearchIncFile(
    WitRun * const theWitRun,   // 
    const int nPeriods,         //
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


// --------------------------------------------------------------------- 
void 
processBuildAheadFile(
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

// --------------------------------------------------------------------- 
void 
processScrapCostFile(
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

// --------------------------------------------------------------------- 
void 
processStockCostFile(
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processCycleTimeFile(
    
    WitRun * const theWitRun,
    const int nPeriods,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processYieldFile(
    
    WitRun * const theWitRun,
    const int nPeriods,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

void
processOperationYieldFile(
    
    WitRun * const theWitRun,
    const int nPeriods,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);
  
void
processLotSizeFile(
    
    WitRun * const theWitRun,
    const int nPeriods,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


void 
processOperationExecCostFile(
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);



void 
processOperationIntegerExecFile(
    WitRun * const theWitRun,  
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);


void 
processOperationExecPenaltyFile(
    WitRun * const theWitRun,   
    const int nPeriods,         
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);



void
processOperationLotSizeFile(
    WitRun * const theWitRun,
    const int nPeriods,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);
  

void
processInterPlantLotSizeFile(
    WitRun * const theWitRun,
    const int nPeriods,
    const LgFrCalendar & theCal,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

  
void
processDemandObjectiveFile(
    WitRun * const theWitRun,
    const int nPeriods,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper,
    const float interestRate);

void 
processPipPriorityFile(
		       WitRun * const theWitRun,  
		       const int nPeriods,        
		       const std::string & fileName,
		       LgFrMultiPlantHelper & mpHelper);

void 
processPipIndpPriorityFile(
			   WitRun * const theWitRun,   // 
			   const int nPeriods,         //
			   const std::string & fileName,
			   LgFrMultiPlantHelper & mpHelper);


void 
shutOffAllLotSizing(
    WitRun * const theWitRun,
    const int nPeriods);


int
readCsvDemandFloatVector(
    WitRun * const theWitRun,            // WIT environment
    const int nPeriods,                  // Number of periods
    std::string & partName,                // partName to be returned
    std::string & demandName,              // demandName to be returned
    float *  floatVec,                   // floatVector to be returned
    std::ifstream & inFile,                    // An open stream
    const std::string & fileName,          // for messaging
    std::string & dataLine,
    long & lineNo,
    LgFrMultiPlantHelper & mpHelper,
    bool Mandatory,           // Are all elements mandatory?
    float lowerLimit=0.0,          // data lower limits
    float upperLimit=FLT_MAX,      // data upper limits
    bool addIfFound = (bool) false); // Add demand stream to WIT if not there

int
readCsvDemandFloatScalar(
    WitRun * const theWitRun,            // WIT environment
    const int nPeriods,                  // Number of periods
    std::string & partName,                // partName to be returned
    std::string & demandName,              // demandName to be returned
    float theValue,                   // floatVector to be returned
    std::ifstream & inFile,                    // An open stream
    const std::string & fileName,          // for messaging
    std::string & dataLine,
    long & lineNo,
    LgFrMultiPlantHelper & mpHelper,
    bool Mandatory,           // Are all elements mandatory?
    float lowerLimit=0.0,          // data lower limits
    float upperLimit=FLT_MAX,      // data upper limits
    bool addIfFound = (bool) false); // Add demand stream to WIT if not there

void
setSubsNetAllowed(bool subNetAllowed, 
		  LgFrMultiPlantHelper & mpHelper,
		  WitRun * const theWitRun  );
  

  void
setEngine(const std::string & engine);

std::string
getEngine();

bool
doesInputFileExist(
    const std::string & targetFile);

int
doesDemandExist(
    WitRun * const theWitRun,
    const std::string &  partName,
    const std::string &  demandName);




// Add part with default attributes to wit for NORMAL parts
// Return --->  0  if succesful
//             -1  if un-succesful with warning
//             >0  if Severe error
int
sceAddNormalPart(
    WitRun * const theWitRun,
    const std::string & part,
    const std::string & pdf,
    const bool buildNstn,
    const int * buildAheadUB,
    LgFrMultiPlantHelper & mpHelper,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);

int  
sceAddNormalCapacityPart(
    WitRun * const theWitRun,
    const std::string & capacity,
    const std::string & pdf,
    const int & mandatoryCapacityFlag,
    LgFrMultiPlantHelper & mpHelper,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);

int
sceAddNormalOperation(
    WitRun * const theWitRun,
    const std::string & operationName,
    const std::string & pdf,
    LgFrMultiPlantHelper & mpHelper,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);

// Add BOP entries and any default attributes
// to wit for NORMAL BOPs
// Return --->  0  if succesful
//             -1  if un-succesful with warning
//             +1  if un-sucessful & input record should be ignored
int
sceAddNormalBopStructure(
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
    float preference,
    int isObjectInEffectForOptExplosion,
    int isBuildable,
    float pipShare,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);

// Add Operatoin BOM entries and any default attributes
// to wit for NORMAL Operation BOMs
// Return --->  0  if succesful
//             -1  if un-succesful with warning
//             +1  if un-sucessful & input record should be ignored
int
sceAddNormalOperationBomStructure(
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
    long lineNo);


// Add part(s) and any structure to wit for special aggregate part
// REturn --->  0  if succesful
//             -1  if un-succesful with warning
//             >0  if Severe error
int
sceAddAggregatePart(
    WitRun * const theWitRun,
    const std::string & part,
    const std::string & pdf,
    LgFrMultiPlantHelper & mpHelper,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);



// Add part(s) and any structure to wit for BB Category parts (BB/feature set)
// REturn --->  0  if succesful
//             -1  if un-succesful with warning
//             >0  if Severe error
int
sceAddBbCategoryPartAndStructureViaProportionalRoute(
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
    long lineNo);

// Add part(s) and any structure to wit for BB Category parts (BB/feature set)
// REturn --->  0  if succesful
//             -1  if un-succesful with warning
//             >0  if Severe error
int
sceAddBbCategoryPartAndStructureViaProportionalRoute61(
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
    long lineNo);


int
sceAddBbCategoryPartAndStructure(
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
    long lineNo);


int
sceAddPartToRepresentDemand(
    WitRun * const theWitRun,
    const std::string & fullPartname,
    LgFrMultiPlantHelper & mpHelper,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);


int
sceAddPartToRepresentDemandUpperBound(
    WitRun * const theWitRun,
    const std::string & fullPartname,
    LgFrMultiPlantHelper & mpHelper,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);


// Add a substitute BOM record
int
sceAddSubstitute(
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
    int  isObjectInEffectForOptExplosionFlag,
    int isSubstituteBuildable,
    bool subNetAllowed,
    float groupSharingExecPenaltyThresh,
    float offsetDelta,
    float prpRtg_Share,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);

// Add  Alternate Part Operation and structure
int
sceAddAlternatePartStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    int nPeriods,
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
    float groupSharingExecPenaltyThresh,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);


int
sceSetSourcingStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & part, 
    const std::string & pdf,
    const std::string & source_code, 
    float pref_cost,
    int early,
    int late,
    int nPeriods,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);






// Normal BOM entry (mandatory)
int
sceAddNormalBomStructure(
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
    int propRtg_flag,
    float propRtg_share,
    int nPeriods,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);

// Normal BOC entry (mandatory)
int
sceAddNormalBocStructure(
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
    long lineNo);


// Add aggregation part and structure
int
sceAddAggregateBomStructure(
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
    long lineNo);


int
addInterPlantBomStructure(WitRun * const theWitRun, 
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
                          long lineNo);

//  SCE 6.1
int
sceAddBBtoOptionBomStructureViaProportionalRoute61(
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
    long                 lineNo);

int
sceAddBBtoOptionBomStructureViaProportionalRoute(
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
    long lineNo);
int
sceAddBBtoOptionBomStructure(
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
    long lineNo);

// This routine looks through all the parts in the WIT model
// and for any part with (1) multiple bop entries and 
// (2) a childless default operation, it
// "turns off" the effectivity of the default operation.
int
scePreProcessDefaultOperations(
    WitRun * const theWitRun,
    const int nPeriods );


  // copy over the priorities from the GPD to all the specialFeatureDemands
  // associated with this gpd.  
int 
sceSetPrioritiesForSpecialFeatureDemands(
    WitRun * const theWitRun,
    LgFrMultiPlantHelper & mpHelper);
  

// This routine looks through all the parts in the WIT model
// and for any part with multiple bop entries "turns on" the lowest
// preference bop in the appropriate effectivity period and
// "turns off" all the other bops.
int
scePreProcessMultiBopEntries(
    WitRun * const theWitRun,
    bool useMultiRouteImplosion);


// preprocess multiple interplant entries
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
// data into SCE.
// (1) that interplant operations are effective over
// the entire planning horizon (valid for ECA; this 
// simplifies the implementation b/c effectivity
// dates and the corresponding multiple bop/boms they produce
// can be ignored. This assumption is for June6th SCEv2.0
// expediency and should be relaxed in the future).
// (2) that the transit time for alternative interplants is equal
// to the transit time of the preferred interplant (not valid for ECA
// but currently offsets on subBomEntries are not available in WIT).

int
scePreProcessMultiInterPlantEntries (
                              WitRun * const theWitRun,
                              LgFrMultiPlantHelper & mpHelper );


#ifdef BUILDING_BLOCKS_VIA_MULTI_BOPS  
// This routine uses MRP to explode the demand vols down to
// the featureSetPart
int
scePreProcessViaMrpViaMultiRoute(
    WitRun * const theWitRun,
    int nPeriods,
    const LgFrCalendar & theCal,
    LgFrMultiPlantHelper & mpHelper);
#else
// This routine uses MRP to explode the demand vols down to
// the featureSetPart
int
scePreProcessViaMrp(
    WitRun * const theWitRun,
    int nPeriods,
    const LgFrCalendar & theCal,
    LgFrMultiPlantHelper & mpHelper);
#endif


int 
checkAndFixIllegalSubNetAllowedCases(
				     WitRun * const theWitRun, 
				     LgFrMultiPlantHelper & mpHelper);




#ifdef BUILDING_BLOCKS_VIA_MULTI_BOPS
int
sceSetOptionRevenueViaMultiRoute(
    WitRun * const theWitRun,
    LgFrMultiPlantHelper & mpHelper,
    const std::string & plannerTopLevelPart,
    const std::string & geo,
    const std::string & mfgOptionPart,
    float * featureRev,
    float periodicInterestRate,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);
#else
int
sceSetOptionRevenue(
    WitRun * const theWitRun,
    LgFrMultiPlantHelper & mpHelper,
    const std::string & plannerTopLevelPart,
    const std::string & geo,
    const std::string & mfgOptionPart,
    float * featureRev,
    float periodicInterestRate,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);
#endif

int
sceAddStdAloneOptionBomStructure(
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
    long lineNo);


  // SCE 5.X
int
sceAddCustChoiceFeatureStructure(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar & theCal,
    const std::string & plannerTopLevelPart, 
    const std::string & geo,
    const std::string & mfgOptionPart, 
    float usageRate,
    float * featureRatio,
    float * defaultGatingOptionSubPenalty,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);

  // SCE 6.1
int
sceAddCustChoiceFeatureStructure61(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar   & theCal,
    const std::string    & plannerTopLevelPart, 
    const std::string    & geo,
    const std::string    & featurePart, 
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
    long                   lineNo);

int
sceAddStdAloneOptionBomStructure61(
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
    long                   lineNo);


int
sceAddMachineFeatureSupplyVol(
    WitRun * const theWitRun, 
    LgFrMultiPlantHelper & mpHelper,
    const std::string & plannerTopLevelPart, 
    const std::string & geo,
    const std::string & mfgOptionPart, 
    float *  supplyVol,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);



int
sceSetMachineFeatureOffset(
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
    long lineNo);

void
removeGpdBopOffsets(
    WitRun * const theWitRun,
    LgFrMultiPlantHelper & mpHelper);


  // Add BOM entry for a DemandPart --> to a Real Part
int
sceAddDemandBomStructure(
    WitRun * const theWitRun,
    LgFrMultiPlantHelper & mpHelper,
    const LgFrCalendar & theCal,
    const std::string & fullParentPartname,
    const std::string & fullChildPartName,
    int early,
    int late,
    float transitTime,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);


  // Add BOM entry for a DemandPart --> to a Demand UpperBound Part
int
sceAddDemandUbBomStructure(
    WitRun * const theWitRun,
    LgFrMultiPlantHelper & mpHelper,
    const std::string & fullParentPartname,
    const std::string & fullChildPartname,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo);


  
  
void
printOverallHeuristicValues(
    WitRun * const theWitRun,   // WIT environment
    const int nPeriods,         // Number of periods
    const int nParts,           // Number of parts
    char ** partList,           // List of part names
    std::string fileName);


// ---------------------------------------------------------------
void 
printImpactPeriodFile(
    WitRun * const theWitRun,
    const LgFrCalendar & theCal,    
    const int nPeriods,         
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);



//  Utility to determine if a floatVec is non-zero
bool
isFloatVecNonZero(
    float * theVector,       // the vector itself
    const int vLength);      // length of vector



void
setWitInformationalMessaging(WitRun * const theWitRun,
                             int onOrOff);



private:
  bool implementSmartFeatures_;
  LgFrSceHeurModelHelper * heurModelHelper_;
  LgFrSceLpModelHelper * lpModelHelper_;
  bool groupSharingViaExecPenalties_;
};

#endif

