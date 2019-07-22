// file: sceHelper.h
// author: tom

#ifndef  LGFRMULTIPLANTHELPER_H
#define  LGFRMULTIPLANTHELPER_H

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.

#include <scenario/src/part.h>
#include <scenario/src/timeVecs.h>
#include <mcl/src/mcl.h>



// forward References
class WitRun;
#if 1 
// #ifdef VARIABLE_PERIODS
class LgFrCalendar;
#endif

//  SCE partname parser class
class LgFrMultiPlantHelper {

public:


  // ----  APP DATA HELPER FUNCTIONS --------------
  // these functions make it a bit more convenient to get info
  // about a part that is normally stored in part app data
// Return 1 if a part is marked as a PCF part, 0 if not
int
isPartPcf(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);

#ifdef MARK_BOGONS
// Return 1 if a part is a PCF bogon
int
isPartBogon(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);
#endif

// returns convenient way to get the nAltPart Ops 
int
nAlternatePartOps(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);

// returns convenient way to get the nInterplant Ops 
int
nInterplantOps(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);




// returns convenient way to get the nUserDefinedOps
int
nUserDefinedOps(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);

  // returns convenient way to get the nAggregationOps
int
nAggregationOps(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);

#ifdef ENABLE_NEGATIVE_DEMANDS
// Return 1 if part has any negative demands
int
doesPartHaveNegativeDemands(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);

// Set the Part's doesPartHaveNegativeDemands
void
setDoesPartHaveNegativeDemands(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    int doesPartHaveNegativeDemands);

LgFrTimeVecFloat
negDemVol(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);

#endif

// plannerPartName/Geo ---> mfgPart/Pdf  source 
// THIS IS A BETTER ONE. !!!!!!
// feed it a part (without a PDF prepended) and a geo (ie, a demand name)
// and it returns a PDF_Part
// You also feed it a start period, and it figures the latest period for 
// which this pdf is sourced for this demand.  It also finds the time-offset
// for this pdf source.
std::string
demandSource(
    WitRun * const theWitRun, 
    const std::string & plannerPartName,
    const std::string & geo,
    const int         start,
          int       & late,
          float     & offset,
    const std::string & fileName, 
    const std::string & dataLine, 
    const long lineNo, 
    const int messageLogic);

  // VARIABLE_PERIODS
std::string
demandSource(
    WitRun * const theWitRun, 
    const std::string & plannerPartName,
    const std::string & geo,
    const int         start,
          int       & late,
    LgFrTimeVecFloat     & offsetTV,
    const std::string & fileName, 
    const std::string & dataLine, 
    const long lineNo, 
    const int messageLogic);

  // SCE 6.1
  // feed it a GPD planner part and geo (with D2o), and it returns the topMfgPart of the machine, and 
  //    populates the sourceLocPdf reference
std::string
mfgTopPartForGpd(WitRun * const      theWitRun, 
		 const std::string & plannerPartName,
		 const std::string & d2oGeo,
		       std::string & sourceLocPdf);
  
  // SCE 6.1
  // returns SourceLoc from geo (with D2O)
std::string
sourceLoc(const std::string &  d2oGeo)
  const;



  // SCE 6.1: this one returns geoPreD2O + requestDate
std::string
expandDemandKeyforD2O(const std::string &  geoPreD2O, const std::string & requestDate)
  const;

  // SCE 6.1: this one returns sourceLoc + geoPreD2O + requestDate
std::string
expandDemandKeyforD2O(const std::string &  geoPreD2O, 
		      const std::string & requestDate,
		      const std::string & sourceLoc)
		      
  const;


// multi_attribute_demand
std::string
compressedDemandName(
		     const std::string * demandAttributes[])
const;



// returns a String Suitable for printing as leadingString
// returns 0 if unsuccessful, 1 if successful 
int
populateLeadingString(
	      const std::string & partname,
	      const std::string & pdf,
	      const std::string & compressedDemandName,
	      std::string & leadingString);
  

// returns a String Suitable for printing as leadingString for TINP Demand records
// returns 0 if unsuccessful, 1 if successful 
int
populateLeadingStringGPD(const std::string & partname,
			 const std::string & geo,
			 std::string & leadingString);



// returns a String Suitable for printing as leadingString for INDP Demand Records
// note that it does not expect part and pdf to be in the compressed name
// returns 0 if unsuccessful, 1 if successful 
// Note: if "useDemand2OrderINDP"=TRUE then we add the requestDate from the demandKey
//        else it is the responsibility of the calling function to append the right request (and commit) date
int
populateLeadingStringINDP(const std::string & partname,
			  const std::string & pdf,
			  const std::string & demandName,
			  std::string & leadingString);
  



// multi_attribute_demand
void
compressedDemandName(std::string & demandName, 
		     const std::string & customerLoc,
		     const std::string & demandClass,
		     const std::string & demandLevel,
		     const std::string & partClass);

int
uncompressedDemandNames(
	const std::string & compressedDemandName,
	std::string & customerLoc,
	std::string & demandClass,
	std::string & demandLevel,
	std::string & partClass);


// multi_attribute_demand
void
compressedDemandNameWitPdf(std::string & demandName, 
			   const std::string & sourceLoc,
			   const std::string & customerLoc,
			   const std::string & demandClass,
			   const std::string & demandLevel,
			   const std::string & partClass);
  
  
  
  
int
uncompressedDemandNamesWithPdf(const std::string & compressedDemandName,
			       std::string & sourceLoc,
			       std::string & customerLoc,
			       std::string & demandClass,
			       std::string & demandLevel,
			       std::string & partClass);
  
  

// Return a PDF_partname, given the partname, pdf
std::string
pdfPartname(
	    const std::string & mfgPartName,
	    const std::string & pdf)
const;

// Return a PDF_operationNname, given the operationName, pdf
std::string
pdfOperationName(
    const std::string & operationName,
    const std::string & pdf)
const;
  
// Return a demand_Geo name, given the demand, geo
std::string
geoPlannerDemandPartName(
    const std::string & plannerPart,
    const std::string & geo)
const;


  // Return a demand_Geo_SpecialUB part name, given the demand, geo
std::string
geoPlannerDemandUbPartName(
    const std::string & plannerPart,
    const std::string & geo)
const;


// Return a aggregation operation name, given the realPart, realPartPdf
// aggPart, and aggPdf
std::string
aggregationOperationName(
    const std::string & realPart,
    const std::string & realPartPdf,
    const std::string & aggPart,
    const std::string & aggPdf)
const;
  
// Return a interplant operation name, given the parent, parentPdf, 
// child, childPdf
std::string
interPlantOperationName(
    const std::string & parentMfgPart,
    const std::string & parentPdf,
    const std::string & childMfgPart,
    const std::string & childPdf)
const;

// Return an alternate part  name, given the primePart, primePdf, altPart, altPdf 
std::string
alternatePartOperationName(
    const std::string & primePart,
    const std::string & primePdf,
    const std::string & altPart,
    const std::string & altPdf)
const;



  // Return an operation namefor the purpose of a modelling trick in Smart Exploder
std::string
operationForCapacityGeneration(
    const std::string & theFullCapacityName)
const;
  

  // Return a phantom part name, given the part and pdf
std::string
phantomPartName(
    const std::string & thePart,
    const std::string & pdf)
const;

// Return a global nullSubstitute  part name, given  pdf
std::string
globalNullSubstitute(
    const std::string & pdf)
const;



#ifdef ALTERNATE_AS_SUB
// Return a Global Alt Dummy part name
std::string
globalAltDummyPartName(
		       const std::string & primePdf)
  const;

// Returns 1 if part is a special Global Alt Dummy, 0 otherwise
// If returns 1, also sets the pdf
int
isPartSpecialGlobalAltDummy(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & pdf);
#endif











// Return a globalMaxWithoutNullSubstitute part name, given a pdf.
//
// When the maxWithoutRatio = 0.0 (so we have a "mandatory feature")
// the substitute on the gpd part to feature part arc is a global
// maxWithoutNullSubstitute, whose supply is zero. The reason for 
// employing a "global" instead a distinct null sub in this case
// is to reduce the problem size (many parts in the ECA run have
// the maxWithoutRatio equal to a vector of all zeros).

std::string
globalMaxWithoutNullSubstitute(
    const std::string & pdf)
const;  
  
  // This is the maxWithoutNullSubstitute to use when you want a unique nullSub
  // for the unique pair (geo_plannerPart_"specialGeoPlannerDemand", pdf_featurePart)
  //
  // Because the pdf of the featurePart is function of the pdf sourcing of the plannerPart
  // (and we can get it if we need it), the pdf is not encode in the maxWithoutNullSubstitute
  // part name.
  //
  // Return a  nullSubstitute  part name, given  geo, planner part and feature.
  // NOTE: this should be used for features with MaxWithout < featratio
std::string
maxWithoutNullSubstitute(
    const std::string & geo,
    const std::string & plannerPart,
    const std::string & featurePart)
const;



// Return a unique standalone Feature Part Name
std::string
standaloneFeaturePartName(
    const std::string & machine,
    const std::string & geo,
    const std::string & featurePart,
    const std::string & pdf)
const;

  
// Return a unique bbCategory part name
std::string
bbCategoryPartName(
    const std::string & plannerTopLevelBuild,
    const std::string & geo,
    const std::string & bbCategory)
const;


// Return a unique  part name for LT Feature Set Capacity Part
std::string
LTbbCapacityPartName(
    const std::string & plannerTopLevelBuild,
    const std::string & geo,
    const std::string & bbCategory)
const;


// Return a unique  part name for GT Feature Set Capacity Part
std::string
GTbbCapacityPartName(
    const std::string & plannerTopLevelBuild,
    const std::string & geo,
    const std::string & bbCategory)
const;


// Return a unique part name for an Option Dummy part
std::string
optionDummyPartName(
    const std::string & plannerTopLevelBuild,
    const std::string & geo,
    const std::string & bbCategory,
    const std::string & option)
const;


// Return a unique part name for an Option Ratio Supply part
std::string
optionRatioSupplyPartName(
    const std::string & plannerTopLevelBuild,
    const std::string & geo,
    const std::string & bbCategory,
    const std::string & option)
const;




// Return a unique standalone Feature Part Name
std::string
custChoiceFeaturePartName(
    const std::string & machine,
    const std::string & geo,
    const std::string & featurePart,
    const std::string & pdf)
const;

// Returns 1 if part is a special Cust Choice Feature, 0 otherwise
// If returns 1, also sets the geo, the plannerPart, pdf, and featurePart
int
isPartSpecialCustChoiceFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & geo,    
    std::string & machine,    
    std::string & pdf,    
    std::string & featurePart);


// Returns 1 if operation is a special Customer Choice Feature, 0 otherwise
// If returns 1, also sets the geo, the plannerPart, pdf, and featurePart
int
isOperationSpecialCustChoiceFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitOperation,
    std::string & geo,    
    std::string & machine,    
    std::string & pdf,    
    std::string & featurePart);

// Returns 1 if operation is EITHER:
//     special Standalone Feature, 
//  OR  special Customer Choice Feature,
// 0 otherwise
// If returns 1, also sets the geo, the plannerPart, pdf, and featurePart
int
isOperationSpecialFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitOperation,
    std::string & geo,    
    std::string & machine,    
    std::string & pdf,    
    std::string & featurePart);



// Returns 1 if part is a special Standalone Feature, 0 otherwise
// (overloaded function.  this one just checks ...
int
isPartSpecialCustChoiceFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);

// Returns 1 if part is EITHER:
//     special Standalone Feature, 
//  OR  special Customer Choice Feature,
// 0 otherwise
// (overloaded function.  this one just checks ...
int
isPartSpecialFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);


// Return 1 if good, 0 if bad
int
isPartValid(
    WitRun * const theWitRun, 
    const std::string & fullWitPartName, 
    const std::string & filename, 
    const std::string & dataLine, 
    const long lineNo, 
    const int messageLogic)
const;


// Return 1 if good, 0 if bad
int
isPartValid(
    WitRun * const theWitRun, 
    const std::string & partname, 
    const std::string & pdf, 
    const std::string & filename, 
    const std::string & dataLine, 
    const long lineNo, 
    const int messageLogic)
const;



// Return 1 if a part is Buildable (ie, should be in the Build Schedule)
int
isPartBuildable(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);

  
// Return 1 if a normal (no special characters) part, 0 if not
int
isPartNormal(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
const;

// Return 1 if a "normal" (no special characters) operation, 0 otherwise.
// If method returns 1, the method also sets the pdf and operation

// "Normal" here means that either (1) the operation was defined
//  explicitedly by the user via the Operation Definition File or
// (2) SCE generated the operation from a part that was explicitedly
// defined by the user via the Part Definition File. 
int
isOperationNormal(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & thePdf,
    std::string & theOperationName )
const;

//
// Return 1 if the operation is "User Defined", 0 ow.
// If method returns 1, the method also sets the pdf and operation
int
isOperationUserDefined(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & thePdf,
    std::string & theOperationName)
const;

  
// Return 1 if a normal (no special characters) capacity, 0 if not
int
isPartNormalCapacity(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
const;

  
// Returns 1 if part is a specialGeoPlannerDemand part, 0 otherwise
// If returns 1, also sets the plannerPart and geo
int
isPartSpecialGeoPlannerDemandPart(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerPart,
    std::string & geo);

// Returns 1 if part is a specialGeoPlannerDemand part, 0 otherwise
// If returns 1, also sets the plannerPart and geo
int
isOperationSpecialGeoPlannerDemand(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & plannerPart,
    std::string & geo);

  

// Returns 1 if part is a specialGeoPlannerDemandUB part, 0 otherwise
// If returns 1, also sets the plannerPart and geo
int
isPartSpecialGeoPlannerDemandUbPart(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerPart,
    std::string & geo);
  


// Returns 1 if operation is a special interplant operation, 0 otherwise
// If returns 1, also sets the destination and source part and pdf
int
isOperationSpecialInterplant(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & destination,
    std::string & destinationPdf,
    std::string & source,
    std::string & sourcePdf);

// Returns 1 if operation is a special alternate operation, 0 otherwise
// If returns 1, also sets the primePart, primePdf, altPart, and altPdf
int
isOperationSpecialAlternate(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & primePart,
    std::string & primePdf,
    std::string & altPart,
    std::string & altPdf);



// Returns 1 if operation is a special aggregation operation, 0 otherwise
// If returns 1, also sets the realPart and aggPart and pdf's
int
isOperationSpecialAggregation(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & realPart,
    std::string & realPartPdf,
    std::string & aggPart,
    std::string & aggPdf);


  // Return 1 if the operation is SmartExpldoer Capacity creator
int
isOperationForCapacityGeneration(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & theCapacity,
    std::string & pdf);
  
  

  // Returns 1 if part is a special nullSubstitiute, 0 otherwise
  // Use this when all you need to know if the part is a global or
  // max without null substitute.
  //
  // This will return true for both types of nullSubstitute parts.  
int
isPartSpecialNullSubstitute(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);

    
// Returns 1 if part is a special Global nullSubstitiute, 0 otherwise
// If returns 1, also sets the pdf
int
isPartSpecialGlobalNullSubstitute(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & pdf);
  
// Returns 1 if part is a special GlobalMaxWithoutNullSubstitiute,
//         0 otherwise
// If returns 1, also sets the pdf
int
isPartSpecialGlobalMaxWithoutNullSubstitute(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & pdf);
  
// Returns 1 if part is a special MaxWithout nullSubstitiute, 0 otherwise
// If returns 1, also sets the geo, the plannerPart, and featurePart
int
isPartSpecialMaxWithoutNullSubstitute(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & geo,
    std::string & plannerPart,
    std::string & featurePart);

// Returns 1 if part is a special CustChoiceNullSubstitiute,
//         0 otherwise
// If returns 1, also sets the pdf
int
isPartSpecialCustChoiceNullSubstitute(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & geo,
    std::string & plannerPart,
    std::string & featurePart);



// Returns 1 if part is a special Standalone Feature, 0 otherwise
// If returns 1, also sets the pdf, and featurePart
int
isPartSpecialStandaloneFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & geo,    
    std::string & machine,    
    std::string & pdf,    
    std::string & featurePart);

// Returns 1 if operation is a special Standalone Feature, 0 otherwise
// If returns 1, also sets the geo, the plannerPart, pdf, and featurePart
int
isOperationSpecialStandaloneFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitOperation,
    std::string & geo,    
    std::string & machine,    
    std::string & pdf,    
    std::string & featurePart);
  

// Returns 1 if part is a special Standalone Feature, 0 otherwise
// (overloaded function.  this one just checks ...
int
isPartSpecialStandaloneFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);




  
  
// Returns 1 if part is a special phantom part, 0 otherwise
int
isPartSpecialPhantom(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);

// Returns 1 if part is a special BB category Set  part, 0 otherwise
// If returns 1, also sets the plannerTopLevelBuild, geo, and bbCategory
int
isPartSpecialBbCategory(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerTopLevelBuild,
    std::string & geo,
    std::string & bbCategory);



// Returns 1 if part is a special feature Set LT capacity part, 0 otherwise
// If returns 1, also sets the part and pdf
int
isPartSpecialLTbbCapacity(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerTopLevelBuild,
    std::string & geo,
    std::string & bbCategory);

// Returns 1 if part is a special feature Set LT capacity part, 0 otherwise
// If returns 1, also sets the part and pdf
int
isPartSpecialGTbbCapacity(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerTopLevelBuild,
    std::string & geo,
    std::string & bbCategory);

// Returns 1 if part is a special Option dummy, 0 otherwise
// If returns 1, also sets the plannerTopLevelBuild, geo, bbCategory,
// and mfgOption part.
int
isPartSpecialOptionDummy(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerTopLevelBuild,
    std::string & geo,
    std::string & bbCategory,
    std::string & mfgOptionPart);
// Returns 1 if part is a special Option dummy, 0 otherwise
// If returns 1, also sets the plannerTopLevelBuild, geo, bbCategory,
// and mfgOption part.
int
isOperationSpecialOptionDummy(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & plannerTopLevelBuild,
    std::string & geo,
    std::string & bbCategory,
    std::string & mfgOptionPart);


// Returns 1 if part is a special Option Ratio Supply, 0 otherwise
// If returns 1, also sets the plannerTopLevelBuild, geo, bbCategory,
// and mfgOption part.
int
isPartSpecialOptionRatioSupply(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerTopLevelBuild,
    std::string & geo,
    std::string & bbCategory,
    std::string & mfgOptionPart);

// returns 1 if operation is valid, 0 otherwise 
int
isOperationValid(
    WitRun * const theWitRun, 
    const std::string & operationName,
    const std::string & pdf,
    const std::string & filename, 
    const std::string & dataLine, 
    const long lineNo, 
    const int messageLogic)
const;

// returns 1 if operation is valid, 0 otherwise 
int
isOperationValid(
    WitRun * const theWitRun, 
    const std::string & pdfOperationName, 
    const std::string & fileName, 
    const std::string & dataLine, 
    const long lineNo, 
    const int messageLogic)
const;

  // returns 1 if pdf-operation is unique
  // from pdf-parts in WIT model, 0 otherwise
int
isPdfOperationUnique(
    WitRun * const theWitRun, 
    const std::string & pdfOperationName, 
    const std::string & fileName, 
    const std::string & dataLine, 
    const long lineNo, 
    const int messageLogic)
const;
 

// Return a geo_plannerPart_specialPureOptionBuildDemandName, given the
// fullWitName of a specialGeoPlannerDemand part
std::string
pureOptionBuildDemandName(
    const std::string & fullWitPartName);

// Returns 1 if demand is a specialPureOptionBuildDemand, 0 otherwise
// If returns 1, also sets the plannerPart and geo
int
isDemandSpecialPureOptionBuildDemand(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    const std::string & fullWitDemandName,
    std::string & plannerPart,
    std::string & geo);

std::string
custChoiceFeatureDemandName(
    const std::string & fullWitPartName);

int
isDemandSpecialCustChoiceFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitPartName,
    const std::string & fullWitDemandName,
    std::string & plannerPart,
    std::string & geo);


std::string
custChoiceNullSubstitute(
    const std::string & geo,
    const std::string & plannerPart,
    const std::string & featurePart)
const;

// Return the witPartIndex as currently set in app data.
// returns -1 if the part is bad, or, index is not set
int
witPartIndex(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname);

// Return the witOperationIndex
// returns -1 if the operation is bad, or, index is not set
  // NOTE: currently only set for special CC feature parts
int
witOperationIndex(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName);


// Creates a LgFrScePartAppData for every part in the witRun (unless one already exists)
// And set the index to be synchronized with witGetParts.

void
setAllWitPartIndices(
    WitRun * const theWitRun);

// set the Customer Choice Operation AppData indices to synch with a witGetOperation()
// It sets them only for Special Customer Choice feature parts
void
setCCOperationIndices(
    WitRun * const theWitRun);


#if 1 
  // #ifdef VARIABLE_PERIODS
// set the CycleTimeDays exactly as input from Cycle Time File
void
setCycleTimeDays(
		 WitRun * const theWitRun,
		 const std::string & fullWitPartname,
		 LgFrTimeVecFloat &  cycleTimeDays);

// get the CycleTimeDays (exactly as input from Cycle Time File)
LgFrTimeVecFloat
getCycleTimeDays(
		 WitRun * const theWitRun,
		 const std::string & fullWitPartname);
void
setCycleTime(
	     WitRun * const theWitRun,
	     const std::string & fullWitPartname,
	     LgFrTimeVecFloat &  cycleTime);

// get the CycleTimeDays (exactly as input from Cycle Time File)
LgFrTimeVecFloat
getCycleTime(
	     WitRun * const theWitRun,
	     const std::string & fullWitPartname);

float
calculateVariableOffset(
			float remainingCycleTimeDaysToAllocate, 
			int t, 
			LgFrTimeVecFloat & workingDays,
			int nPeriods);
float
calculateVariableOffset(float remainingCycleTimeDaysToAllocate, 
			int t, 
			const LgFrCalendar & theCal,
			int nPeriods);
#endif



  // returns the feature ratio of a customer choice feature.
  // Note that the partname MUST be a specialCustChoicePartName
LgFrTimeVecFloat 
custChoiceFeatureRatio(
    WitRun * const theWitRun, 
    const std::string & specialCustChoicePartName);

// GET 
// this helper goes into wit and finds the right appdata 
// for the Dummysupply Adjustment to be used in MRP
LgFrTimeVecFloat 
custChoiceDummySupplyVolForMrpAdjustment(
    WitRun * const theWitRun, 
    const std::string & specialCustChoicePartName);


// SET 
// this helper goes into wit and finds the right appdata 
// for the Dummysupply Adjustment to be used in MRP
void 
custChoiceDummySupplyVolForMrpAdjustment(
    WitRun * const theWitRun,
    const std::string & specialCustChoicePartName,
    LgFrTimeVecFloat & dummySupplyVolForMrpAdjustment);



// set/get's for private data
  std::string  pdfSeparator() const;
  void         pdfSeparator(const std::string & pdfSeparator);
  std::string  defaultPdf() const;
  void         defaultPdf(const std::string & defaultPdf);
  bool         useMultiAttributeDemand() const;
  void         useMultiAttributeDemand(const bool useMultiAttributeDemand);
  int          numDemandAttributes() const;
  void         numDemandAttributes(const int numDemandAttributes);
  std::string  multiAttributeDemandSeparator() const;
  void         multiAttributeDemandSeparator(const std::string & multiAttributeDemandSeparator);
  bool         useDemand2OrderINDP() const;
  void         useDemand2OrderINDP(const bool useDemand2OrderINDP);
  bool         truncOffsetToEol() const;
  void         truncOffsetToEol(const bool truncOffsetToEol);



// Get the  PDF for a PDF_PART
std::string 
pdf(const LgFrPart & part)
const;


std::string 
pdf(const std::string & pdfPart)
const;


std::string 
pdf(const char * pdfPart)
const;


  // return just the partname of a pdf_part

std::string 
partname(const std::string & pdfPart)
const;

  // return just the operationName of a pdf_operationName

std::string 
operationName(const std::string & pdfOperation)
const;
  
// Are the parts equivalent, irrespective of the PDF?

bool
isPartnameEqual(const std::string & left, const std::string & right)
const;

// Returns 1 if operation is a special interplant operation and
//              sets the full pdf-part name for the source part
// Otherwise, returns 0
int
interplantSourcePdfPartName(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & sourcePdfPartName);


// returns 1 if the fullWitPartName is produced by more than one interplant operations
//         0 otherwise
int
isPartMultiInterplantSourced(
    WitRun * const theWitRun, 
    const std::string & fullWitPartName,
    int npBops);



// 
// returns 1 if the "duplicate" interplant record's usage rate is the same
//              as the usage rate on the previous record (ie. the current
//              usage rate on the interplant bom)
//         0 otherwise
int
isDuplicateInterplantRecordUsageRateValid(
    WitRun * const theWitRun, 
    const std::string & sourcePdfPartName,
    const std::string & interplantOperationName,
    float duplicateRecordUsageRate );

//
// returns 1 if the duplicate interplant record's effectivity
//              dates do not overlap with the effectivity
//              dates of interplant operation's existing bops
//         0 otherwise
int
isDuplicateInterplantRecordEffectivityDatesValid(
    WitRun * const theWitRun, 
    const std::string & interplantOperationName,
    int duplicateRecordStart,
    int duplicateRecordEnd );


// 
// returns 1 if the "duplicate" interplant record's usage rate is the same
//              as the usage rate on the previous record (ie. the current
//              usage rate on the interplant bom)
//         0 otherwise
int
isDuplicateAlternatePartRecordUsageRateValid(
    WitRun * const theWitRun, 
    const std::string & fullPrimePartName,
    const std::string & alternatePartOperationName,
    float duplicateRecordUsageRate );

//
// returns 1 if the duplicate interplant record's effectivity
//              dates do not overlap with the effectivity
//              dates of interplant operation's existing bops
//         0 otherwise
int
isDuplicateAlternatePartRecordEffectivityDatesValid(
    WitRun * const theWitRun, 
    const std::string & alternatePartOperationName,
    int duplicateRecordStart,
    int duplicateRecordEnd );





// necessarey for error facility sharing with scenario
void
setErrFacility(MclFacility * existingFacility);

MclFacility *
getErrFacility();

  
// default constructor
LgFrMultiPlantHelper();

// general constructor
LgFrMultiPlantHelper(
    const std::string & pdfSeparator,
    const std::string & defaultPdf,
    const bool useMultiAttributeDemand,
    const int numDemandAttributes,
    const std::string & multiAttributeDemandSeparator,
    const bool useDemand2OrderINDP);

// Return a copy in the heap
LgFrMultiPlantHelper*
clone()
const;


// copy constructor
LgFrMultiPlantHelper(const LgFrMultiPlantHelper& source);

// assignment operator
LgFrMultiPlantHelper&
operator=(const LgFrMultiPlantHelper& rhs);

// destructor
virtual
~LgFrMultiPlantHelper();

     
// self-test
static void test();

private:
  std::string pdfSeparator_;
  std::string defaultPdf_;
  bool useMultiAttributeDemand_;
  int numDemandAttributes_;
  std::string multiAttributeDemandSeparator_;
  bool useDemand2OrderINDP_;
  bool truncOffsetToEol_;
  MclFacility * sceErrFacility_;
};
#endif
