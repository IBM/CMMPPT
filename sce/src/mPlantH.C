
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
// file: mPlantH.C
// author: tom

#include <stdlib.h>
#include <assert.h>
#include <mcl/src/mcl.h>
#include <scenario/src/machdep.h>
#include <sce/src/sceDefin.h>
#include <sce/src/mPlantH.h>
#include <wit/src/wit.h>
#include <sce/src/appData.h>
#include <sce/src/dmApData.h>
#if 1  
// #ifdef VARIABLE_PERIODS
#include <scenario/src/calendar.h>
#endif

#define MULTIPLANTHELPER_FLT_EPS  0.00001

#ifndef ELIMINATE_OLD_MAIN



// plannerPartName/Geo ---> mfgPart/Pdf  source 
// feed it a part (without a PDF prepended) and a geo.c_str() (ie, a demand name)
// and it returns a PDF_Part
// You also feed it a start period, and it figures the latest period for 
// which this pdf is sourced for this demand
std::string
LgFrMultiPlantHelper::demandSource(
    WitRun * const theWitRun, 
    const std::string & plannerPartName,
    const std::string & geo,
    const int         start,
          int       & late,
          float     & offset,
    const std::string & fileName, 
    const std::string & dataLine, 
    const long lineNo, 
    const int messageLogic)
{
    
    std::string mfgTopLevelBuild;
    int bomStart = 0;
    int bomEnd = 0;

// create a geoPlannerPart name
    std::string geoPlannerDemandPartName = this->geoPlannerDemandPartName(plannerPartName, geo);
    

    // and test to see if it exists (it should)
    int result = this->isPartValid(theWitRun, geoPlannerDemandPartName, fileName, 
				   dataLine, lineNo, messageLogic);

    if (! result)
	return mfgTopLevelBuild;

    // loop through the BOM entries for this special demand part and find
    // the bom which has effectivity within the period "start".  Return the
    // pdf of this mfgTopLevelBuild
    // We also return the time-offset for this demand source
    int nBom;
    witGetOperationNBomEntries(theWitRun, geoPlannerDemandPartName.c_str(), &nBom);

    char * child;
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nBom; j++)  {
      // if the child is not a normal part, then we can skip it.  (GPD's only 
      // source to normal parts)
      // first check to see if the child is a specialDemand UpperBound part, OR
      // a specialBb category part, if so, then skip it.
      witGetBomEntryConsumedPart(theWitRun, geoPlannerDemandPartName.c_str(), j, &child);
      std::string pp;
      std::string g;
      std::string bb;

      if (! this->isPartNormal(theWitRun, child)) {
	witFree(child);
	continue;
      }
      witGetBomEntryEarliestPeriod(theWitRun, geoPlannerDemandPartName.c_str(), j, &bomStart);
      witGetBomEntryLatestPeriod(theWitRun, geoPlannerDemandPartName.c_str(), j, &bomEnd);
      if (start >= bomStart && start <= bomEnd)   {
        mfgTopLevelBuild = child;
        witGetBomEntryUsageTime(theWitRun, geoPlannerDemandPartName.c_str(), j, &offset);
      }
      witFree(child);
      if (! mfgTopLevelBuild.empty())
        break;
    }

    // What to do if you can't source it?????
    if (mfgTopLevelBuild.empty())   {
      (*sceErrFacility_)("MissingPDFSource",MclArgList() << plannerPartName << geo << start 
			 << fileName << (int)lineNo << dataLine);
    }



    // set late as the bomEnd
    late = bomEnd;
    return mfgTopLevelBuild;
}


// =============================
// =============================
//           SCE 6.1
// =============================
// =============================
// VARIABLE_PERIODS ...should erase all others ...note that the transit time offset returned is already adjusted for variable periods
// plannerPartName/Geo ---> mfgPart/Pdf  source 
// THIS IS A BETTER ONE. !!!!!!
// feed it a part (without a PDF prepended) and a geo.c_str() (ie, a demand name)
// and it returns a PDF_Part
// You also feed it a start period, and it figures the latest period for 
// which this pdf is sourced for this demand
std::string
LgFrMultiPlantHelper::demandSource(
				   WitRun * const    theWitRun, 
				   const std::string & plannerPartName,
				   const std::string & geo,
				   const int         start,
				   int               & late,
				   LgFrTimeVecFloat  & offsetTV,
				   const std::string & fileName, 
				   const std::string & dataLine, 
				   const long        lineNo, 
				   const int         messageLogic)
{
  std::string mfgTopLevelBuild;
  int bomStart = 0;
  int bomEnd = 0;
  
  // create a geoPlannerPart name
  std::string geoPlannerDemandPartName = this->geoPlannerDemandPartName(plannerPartName, geo);
  
  
  // and test to see if it exists (it should)
  int result = this->isPartValid(theWitRun, geoPlannerDemandPartName, fileName, dataLine, lineNo, messageLogic);
  
  if (! result)
    return mfgTopLevelBuild;
  
  // loop through the BOM entries for this special demand part and find
  // the bom which has effectivity within the period "start".  Return the
  // pdf of this mfgTopLevelBuild
  // We also return the time-offset for this demand source
  int nBom;
  witGetOperationNBomEntries(theWitRun, geoPlannerDemandPartName.c_str(), &nBom);
  
  char * child;
  int j = 0; 
  for (j=0; j<nBom; j++)  {
    // if the child is not a normal part, then we can skip it.  (GPD's only 
    // source to normal parts)
    // first check to see if the child is a specialDemand UpperBound part, OR
    // a specialBb category part, if so, then skip it.
    witGetBomEntryConsumedPart(theWitRun, geoPlannerDemandPartName.c_str(), j, &child);
    std::string pp;
    std::string g;
    std::string bb;
    
    if (! this->isPartNormal(theWitRun, child)) {
      witFree(child);
      continue;
    }
    witGetBomEntryEarliestPeriod(theWitRun, geoPlannerDemandPartName.c_str(), j, &bomStart);
    witGetBomEntryLatestPeriod(theWitRun, geoPlannerDemandPartName.c_str(), j, &bomEnd);
    if (start >= bomStart && start <= bomEnd)   {
      mfgTopLevelBuild = child;
      float * offset;
      int ttt=0;
      witGetBomEntryOffset(theWitRun, geoPlannerDemandPartName.c_str(), j, &offset);
      for (ttt=start; ttt<=late; ttt++) 
	offsetTV[ttt] = offset[ttt];
      witFree(offset);
    }
    witFree(child);
    if (! mfgTopLevelBuild.empty())
      break;
  }
  
  // What to do if you can't source it?????
  if (mfgTopLevelBuild.empty())   {
    (*sceErrFacility_)("MissingPDFSource",MclArgList() << plannerPartName << geo << start << fileName << (int)lineNo << dataLine);
  }
  // set late as the bomEnd
  late = bomEnd;
  return mfgTopLevelBuild;
}


// feed it a GPD planner part and geo (with D2o), and it returns the topMfgPart of the machine, and 
//    populates the sourceLocPdf reference
std::string
LgFrMultiPlantHelper::mfgTopPartForGpd(WitRun * const      theWitRun, 
				       const std::string & plannerPartName,
				       const std::string & d2oGeo,
				       std::string & sourceLocPdf)
{

  sourceLocPdf = this->sourceLoc(d2oGeo);
  return this->pdfPartname(plannerPartName, sourceLocPdf);
}



// multi_attribute_demand  ... DEMAND2ORDER function
// append the requestDate onto the end of the dmeandName string 
std::string
LgFrMultiPlantHelper::expandDemandKeyforD2O(const std::string &  geoPreD2O, const std::string & requestDate)
const
{
  // start with the original demand key
  std::string expandedDemandName(geoPreD2O);
  expandedDemandName = expandedDemandName + multiAttributeDemandSeparator_ + requestDate;
 return expandedDemandName;
}


// SCE 6.1
// multi_attribute_demand  ... DEMAND2ORDER function
// append the requestDate onto the end of the dmeandName string 
std::string
LgFrMultiPlantHelper::expandDemandKeyforD2O(const std::string &  geoPreD2O, 
					    const std::string & requestDate,
					    const std::string & sourceLoc)
const
{
  // start with the original demand key
  std::string expandedDemandName(geoPreD2O);
  expandedDemandName = 
    sourceLoc + multiAttributeDemandSeparator_ + 
    expandedDemandName + multiAttributeDemandSeparator_ + 
    requestDate;
 return expandedDemandName;
}


std::string
LgFrMultiPlantHelper::sourceLoc(const std::string &  d2oGeo)
const
{
  SCETokenizer next(d2oGeo);

  return next(multiAttributeDemandSeparator_.c_str());
}


// multi_attribute_demand
std::string
LgFrMultiPlantHelper::compressedDemandName(const std::string * demandAttributes[])
const
{
  std::string demandName("");
  int t = 0;
  for (t=0; t<numDemandAttributes_-1; t++)  {
   
    demandName = demandName + multiAttributeDemandSeparator_;
  }
  demandName = demandName + (*demandAttributes)[numDemandAttributes_-1];
 return demandName;
}


// multi_attribute_demand
void
LgFrMultiPlantHelper::compressedDemandName(std::string & demandName, 
					   const std::string & customerLoc,
					   const std::string & demandClass,
					   const std::string & demandLevel,
					   const std::string & partClass)
{
  demandName = 
    customerLoc + multiAttributeDemandSeparator_
    + demandClass + multiAttributeDemandSeparator_
    + demandLevel + multiAttributeDemandSeparator_
    + partClass;
}

// returns 0 if unsuccessful, 1 if successful 
int
LgFrMultiPlantHelper::uncompressedDemandNames(
	const std::string & compressedDemandName,
	std::string & customerLoc,
	std::string & demandClass,
	std::string & demandLevel,
	std::string & partClass)
{
  SCETokenizer next(compressedDemandName);

  // customerLoc
  customerLoc =  next(multiAttributeDemandSeparator_.c_str());
  if (customerLoc.empty())
    return 0;

  // demandClass
  demandClass =  next(multiAttributeDemandSeparator_.c_str());
  if (demandClass.empty())
    return 0;

  // demandLevel
  demandLevel =  next(multiAttributeDemandSeparator_.c_str());
  if (demandLevel.empty())
    return 0;

  // partClass
  partClass =  next(multiAttributeDemandSeparator_.c_str());
  if (partClass.empty())
    return 0;


  return 1;
}



// SCE 6.1
// returns a String Suitable for printing as leadingString
// note that it does not expect part and pdf to be in the
// compressed name
// returns 0 if unsuccessful, 1 if successful 
int
LgFrMultiPlantHelper::populateLeadingString(
					    const std::string & partname,
					    const std::string & pdf,
					    const std::string & compressedDemandName,
					    std::string & leadingString)
{
  if (useMultiAttributeDemand_) {
    SCETokenizer next(compressedDemandName);
    
    leadingString = "\"" 
      + partname + "\",\""
      + pdf + "\",";
    
    int a;
    for (a=0; a<numDemandAttributes_; a++) {
      std::string nextToken = next(multiAttributeDemandSeparator_.c_str());
      if (nextToken.empty()) {
	return 0;
      }
      if (a>0)  {
	leadingString += ",";
      }
      leadingString += "\""  + nextToken + "\"";
    }
    return 1;
  }

  else {
    leadingString = "\"" 
      + partname + "\",\"" 
      + pdf + "\",\""       + compressedDemandName + "\"";
  }
}




// SCE 6.1
// returns a String Suitable for printing as leadingString for INDP Demand Records
// note that it does not expect part and pdf to be in the compressed name
// returns 0 if unsuccessful, 1 if successful 
// Note: if "useDemand2OrderINDP"=TRUE then we add the requestDate from the demandKey
//        else it is the responsibility of the calling function to append the right request (and commit) date
int
LgFrMultiPlantHelper::populateLeadingStringINDP(
					       const std::string & partname,
					       const std::string & pdf,
					       const std::string & demandName,
					       std::string & leadingString)
{
  if (useMultiAttributeDemand_) {
    SCETokenizer next(demandName);
    
    leadingString = "\"" 
      + partname + "\",\""
      + pdf + "\",";
    
    int a;
    // !!! Demand to Order has extra key so we go numDemandAttributes_+1
    for (a=0; a<numDemandAttributes_; a++) {
      std::string nextToken = next(multiAttributeDemandSeparator_.c_str());
      if (nextToken.empty()) {
	return 0;
      }
      if (a>0)  {
	leadingString += ",";
      }
      leadingString += "\""  + nextToken + "\"";
    }
    if (useDemand2OrderINDP_)  {
      // pull the request date off the token and write it without quotes
      std::string nextToken = next(multiAttributeDemandSeparator_.c_str());
      if (nextToken.empty()) {
	return 0;
      }
      leadingString += "," + nextToken;
    }
    return 1;

  }

  else {
    leadingString = "\"" 
      + partname + "\",\"" 
      + pdf + "\",\"" 
      + demandName + "\"";
  }
}
  




// returns a String Suitable for printing as leadingString
// note that it does not expect part and pdf to be in the
// compressed name
// returns 0 if unsuccessful, 1 if successful 
int
LgFrMultiPlantHelper::populateLeadingStringGPD(
					       const std::string & partname,
					       const std::string & geo,
					       std::string & leadingString)
{
  if (useMultiAttributeDemand_) {
    SCETokenizer next(geo);
    
    leadingString = "\"" 
      + partname + "\",";
    
    int a;
    // !!! Demand to Order has extra key so we go numDemandAttributes_+1
    for (a=0; a<=numDemandAttributes_; a++) {
      std::string nextToken = next(multiAttributeDemandSeparator_.c_str());
      if (nextToken.empty()) {
	return 0;
      }
      if (a>0)  {
	leadingString += ",";
      }
      leadingString += "\""  + nextToken + "\"";
    }
    // pull the request date off the token and write it without quotes
    std::string nextToken = next(multiAttributeDemandSeparator_.c_str());
    if (nextToken.empty()) {
      return 0;
    }
    leadingString += "," + nextToken;
    
    return 1;
  }

  else {
    leadingString = "\"" 
      + partname + "\",\"" 
      + geo + "\"";
  }
}
  
  

// multi_attribute_demand
void
LgFrMultiPlantHelper::compressedDemandNameWitPdf(std::string & demandName, 
						 const std::string & sourceLoc,
						 const std::string & customerLoc,
						 const std::string & demandClass,
						 const std::string & demandLevel,
						 const std::string & partClass)
{
  demandName = 
    sourceLoc + multiAttributeDemandSeparator_
    + customerLoc + multiAttributeDemandSeparator_
    + demandClass + multiAttributeDemandSeparator_
    + demandLevel + multiAttributeDemandSeparator_
    + partClass;
}

// returns 0 if unsuccessful, 1 if successful 
int
LgFrMultiPlantHelper::uncompressedDemandNamesWithPdf(const std::string & compressedDemandName,
						     std::string & sourceLoc,
						     std::string & customerLoc,
						     std::string & demandClass,
						     std::string & demandLevel,
						     std::string & partClass)
{
  SCETokenizer next(compressedDemandName);

  // sourceLoc
  sourceLoc =  next(multiAttributeDemandSeparator_.c_str());
  if (sourceLoc.empty())
    return 0;

  // customerLoc
  customerLoc =  next(multiAttributeDemandSeparator_.c_str());
  if (customerLoc.empty())
    return 0;

  // demandClass
  demandClass =  next(multiAttributeDemandSeparator_.c_str());
  if (demandClass.empty())
    return 0;

  // demandLevel
  demandLevel =  next(multiAttributeDemandSeparator_.c_str());
  if (demandLevel.empty())
    return 0;

  // partClass
  partClass =  next(multiAttributeDemandSeparator_.c_str());
  if (partClass.empty())
    return 0;


  return 1;
}


#if 1
// #ifdef VARIABLE_PERIODS
// set the CycleTimeDays exactly as input from Cycle Time File
void
LgFrMultiPlantHelper::setCycleTimeDays(
				       WitRun * const theWitRun,
				       const std::string & fullWitPartname,
				       LgFrTimeVecFloat &  cycleTimeDays)
{
  LgFrScePartAppData * partAppDataPtr;
  witGetPartAppData (theWitRun, fullWitPartname.c_str(), (void **) &partAppDataPtr );
  // if no app data exists, then create one.
  if (partAppDataPtr == 0) {
    int nPeriods;
    witGetNPeriods(theWitRun, &nPeriods);
    partAppDataPtr = new LgFrScePartAppData(0, nPeriods, nPeriods);
  }
  partAppDataPtr->cycleTimeDays(cycleTimeDays);
  witSetPartAppData(theWitRun, fullWitPartname.c_str(), (void *) partAppDataPtr);
} 

// get the CycleTimeDays (exactly as input from Cycle Time File)
LgFrTimeVecFloat
LgFrMultiPlantHelper::getCycleTimeDays(
				       WitRun * const theWitRun,
				       const std::string & fullWitPartname)
{
  LgFrScePartAppData * partAppDataPtr;
  witGetPartAppData (theWitRun, fullWitPartname.c_str(), (void **)  &partAppDataPtr );
  // if no app data exists, then return 0 vec
  if (partAppDataPtr == 0) {
    int nPeriods;
    witGetNPeriods(theWitRun, &nPeriods);
    LgFrTimeVecFloat zeroVec((size_t) nPeriods, 0.0);
    return zeroVec;
  }
  return partAppDataPtr->cycleTimeDays();
} 


// set the calculated CycleTime Periods based on Variable Period Logic
// this does NOT set it in WIT ... only in partAppData
void
LgFrMultiPlantHelper::setCycleTime(
				       WitRun * const theWitRun,
				       const std::string & fullWitPartname,
				       LgFrTimeVecFloat &  cycleTime)
{
  LgFrScePartAppData * partAppDataPtr;
  witGetPartAppData (theWitRun, fullWitPartname.c_str(), (void **) &partAppDataPtr );
  // if no app data exists, then create one.
  if (partAppDataPtr == 0) {
    int nPeriods;
    witGetNPeriods(theWitRun, &nPeriods);
    partAppDataPtr = new LgFrScePartAppData(0, nPeriods, nPeriods);
  }
  partAppDataPtr->cycleTime(cycleTime);
  witSetPartAppData(theWitRun, fullWitPartname.c_str(), (void *) partAppDataPtr);
} 

// get the CycleTime from App Data
LgFrTimeVecFloat
LgFrMultiPlantHelper::getCycleTime(
				   WitRun * const theWitRun,
				   const std::string & fullWitPartname)
{
  LgFrScePartAppData * partAppDataPtr;
  witGetPartAppData (theWitRun, fullWitPartname.c_str(), (void **)  &partAppDataPtr );
  // if no app data exists, then return 0 vec
  if (partAppDataPtr == 0) {
    int nPeriods;
    witGetNPeriods(theWitRun, &nPeriods);
    LgFrTimeVecFloat zeroVec((size_t) nPeriods, 0.0);
    return zeroVec;
  }
  return partAppDataPtr->cycleTime();
} 


float
LgFrMultiPlantHelper::calculateVariableOffset(float cycleTimeDaysToAllocate, 
					      int t, 
					      LgFrTimeVecFloat & workingDays,
					      int nPeriods)
{
  float computedCycleTime = 0.0;
  
  
  // two seperate loops going either forward or back in time depending on whether 
  // offset is positive or negative.   Whoa, I think this is wrong.  the positive offset
  // should go from period tt=t down to 0, not, tt=t up to nPeriods.   Lkewise for negatives
  // it should go from tt=t to nperiods-1.  also need to put in logic to truncOffsetToEol
  
  // CASE 1: Zero Offset
  if ((0.0001 < cycleTimeDaysToAllocate) && (cycleTimeDaysToAllocate < 0.0001)) {
    return 0.0f;
  }



  // CASE 2: Positive Offset
  if (cycleTimeDaysToAllocate >= 0.0001) {
    int tt =0;
    for (tt=t; tt>=0; tt--)  {
      if (cycleTimeDaysToAllocate <= workingDays[tt]) {
	// float percentThreshRoll = .20;
	// FINISH_ME: we can adjust fractional to integer a this point based on rules
	computedCycleTime += cycleTimeDaysToAllocate/workingDays[tt];
	return computedCycleTime;
      }
      else   {
	computedCycleTime += 1.0;
	cycleTimeDaysToAllocate -= workingDays[tt];
      }
    }
    
    // if you get to the beginning of horizon and still haven't worked off the cycleTimeDaysToAllocate, 
    // then we use the first period with workingDays > 0 as the assumed workingDays for past periods.
    if (cycleTimeDaysToAllocate > 0.0)  {
      // didn't work it all off.  Can't build in this period
      // lets go back to last period with positive working days and use that as basis for future period lengths
      for (tt=0; tt < nPeriods; tt++)  {
	if (workingDays[tt] > 0) {
	  computedCycleTime += cycleTimeDaysToAllocate/workingDays[tt]; 
	  return computedCycleTime;
	}
      }
      return nPeriods+1;
    }
  }

  // CASE 3: Negative Offset
  else  {
    int tt =0;
    for (tt=t; tt<nPeriods; tt++)  {
      if (-cycleTimeDaysToAllocate <= workingDays[tt]) {
	// float percentThreshRoll = .20;
	// FINISH_ME: we can adjust fractional to integer a this point based on rules
	computedCycleTime += cycleTimeDaysToAllocate/workingDays[tt];
	if (truncOffsetToEol_) 
	  if (t - computedCycleTime > nPeriods - 1) {
	    computedCycleTime = t - (nPeriods - 1);
	  }
	
	return computedCycleTime;
      }
      else   {
	computedCycleTime -= 1.0;
	cycleTimeDaysToAllocate += workingDays[tt];
      }
    }
    
    // if you get to beginning of horizon and still ahven't worked off the days, then we need to 
    // estimate this last offset.   
    if (cycleTimeDaysToAllocate < 0.0)  {
      for (tt=nPeriods-1; tt>=0; tt--)  {
	if (workingDays[tt] > 0) {
	  computedCycleTime += cycleTimeDaysToAllocate/workingDays[tt]; 
	  if (truncOffsetToEol_) 
	    if (t - computedCycleTime > nPeriods - 1) {
	      computedCycleTime = t - (nPeriods - 1);
	    }
	  return computedCycleTime;
	}
      }
      return -nPeriods;
    }
  }
  
}



float
LgFrMultiPlantHelper::calculateVariableOffset(float cycleTimeDaysToAllocate, 
					      int t, 
					      const LgFrCalendar & theCal,
					      int nPeriods)
{
  int tt;

  LgFrTimeVecFloat workingDays(nPeriods, 0.0);
  for (tt=0; tt < nPeriods; tt++)  {
    workingDays[tt] = theCal.workUnits(tt);
  }

  return this->calculateVariableOffset(cycleTimeDaysToAllocate,  t, workingDays, nPeriods);
}
#endif









// =============================
// =============================
//  END of  SCE 6.1
// =============================
// =============================





// default constructor:
LgFrMultiPlantHelper::LgFrMultiPlantHelper()
: pdfSeparator_("? \t\n"),
  defaultPdf_("WW"),
  useMultiAttributeDemand_(0),
  numDemandAttributes_(4),
  multiAttributeDemandSeparator_("% \t\n"),
  useDemand2OrderINDP_(0),
  truncOffsetToEol_(0),
  sceErrFacility_(0)
{
    // all the work is done in initializer
}


// fairly general constructor:
LgFrMultiPlantHelper::LgFrMultiPlantHelper(
	     const std::string & pdfSeparator,
	     const std::string & defaultPdf,
	     const bool useMultiAttributeDemand,
	     const int numDemandAttributes,
	     const std::string & multiAttributeDemandSeparator,
	     const bool useDemand2OrderINDP)
: pdfSeparator_(pdfSeparator),
  defaultPdf_(defaultPdf),
  useMultiAttributeDemand_(useMultiAttributeDemand),
  numDemandAttributes_(numDemandAttributes),
  multiAttributeDemandSeparator_(multiAttributeDemandSeparator),
  useDemand2OrderINDP_(useDemand2OrderINDP),
  truncOffsetToEol_(0),
  sceErrFacility_(0)
{
  // all the work is done in initializer
}

// necessary methods for sharing scenario's error facility
void
LgFrMultiPlantHelper::setErrFacility(MclFacility * existingFacility)
{
  sceErrFacility_ = existingFacility;
}

MclFacility*
LgFrMultiPlantHelper::getErrFacility()
{
  return sceErrFacility_;
}


#ifdef ENABLE_NEGATIVE_DEMANDS
// Return 1 if part has any negative demands
int
LgFrMultiPlantHelper::doesPartHaveNegativeDemands(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
  LgFrScePartAppData * appDataPtr;
  witGetPartAppData(theWitRun,  fullWitPartname.c_str(), (void **) &appDataPtr);
  if (appDataPtr == 0) 
    return 0;

  return appDataPtr->doesPartHaveNegativeDemands();
}

// Set the Part's doesPartHaveNegativeDemands
void
LgFrMultiPlantHelper::setDoesPartHaveNegativeDemands(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    int doesPartHaveNegativeDemands)
{
  LgFrScePartAppData * appDataPtr;
  witGetPartAppData(theWitRun,  fullWitPartname.c_str(), (void **) &appDataPtr);
  // if hte part does not have an appData, then create one
  if (appDataPtr == 0) {
    int nPeriods;
    witGetNPeriods(theWitRun, &nPeriods);
    appDataPtr = new LgFrScePartAppData(0, nPeriods, nPeriods);
    witSetPartAppData(theWitRun, fullWitPartname.c_str(), (void *) appDataPtr);
  }      
  appDataPtr->doesPartHaveNegativeDemands(doesPartHaveNegativeDemands);
}

LgFrTimeVecFloat
LgFrMultiPlantHelper::negDemVol(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
  int nPeriods;
  int nDemands;
  char ** dList;
  
  float * witDemandVol;

  witGetNPeriods(theWitRun, &nPeriods);

  LgFrTimeVecFloat negDemVol((size_t) nPeriods, 0.0);
  LgFrSceDemandAppData * appDataPtr = 0;
  
  if (this->doesPartHaveNegativeDemands(theWitRun, fullWitPartname)) {
    int didWeFindAnAppData = 0;
    witGetPartDemands(theWitRun, fullWitPartname.c_str(), &nDemands, &dList);
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nDemands; j++) {
      witGetDemandDemandVol(theWitRun, fullWitPartname.c_str(), dList[j], &witDemandVol);
      witGetDemandAppData(theWitRun, fullWitPartname.c_str(), dList[j], (void **) &appDataPtr);
      if (appDataPtr != 0) {
	didWeFindAnAppData = 1;
	LgFrTimeVecFloat appDataDemandVol(appDataPtr->demandVol());
	int t = 0; // Pulled out of the for below by RW2STL
	for (t=0; t<nPeriods; t++) {
	  negDemVol[t] += witDemandVol[t] - appDataDemandVol[t];
	}
      }
      witFree(witDemandVol);
      witFree(dList[j]);
    }
    witFree(dList);
    if (! didWeFindAnAppData) {
      std::cerr << "SCE9955F PROGRAMMER ERROR: part with negative demands does not\n"
	   << "have a demand with non-0 appData\n"
	   << "Please report this error to SCE Development\n"
	   << "SCE Terminates with return code: 8\n\n";
      exit(8);
    }
  }

  return negDemVol;
}

#endif

// Return 1 if a part is marked as a PCF part, 0 if not
int
LgFrMultiPlantHelper::isPartPcf(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{


  witBoolean exists;
  
  witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

  if (! exists)
    return 0;


  LgFrScePartAppData * appDataPtr;
  witGetPartAppData(theWitRun,  fullWitPartname.c_str(), (void **) &appDataPtr);
  if (appDataPtr == 0) 
    return 0;

  return appDataPtr->isPartPcf();
}


#ifdef MARK_BOGONS
// Return 1 if a part is a PCF bogon
int
LgFrMultiPlantHelper::isPartBogon(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
  LgFrScePartAppData * appDataPtr;
  witGetPartAppData(theWitRun,  fullWitPartname.c_str(), (void **) &appDataPtr);
  if (appDataPtr == 0) 
    return 0;

  return appDataPtr->isPartBogon();
}
#endif



// Return the witPartIndex
// returns -1 if the part is bad, or, index is not set
int
LgFrMultiPlantHelper::witPartIndex(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
  LgFrScePartAppData * appDataPtr;
  witGetPartAppData(theWitRun,  fullWitPartname.c_str(), (void **) &appDataPtr);
  if (appDataPtr == 0) 
    return -1;

  return appDataPtr->index();
}


// Return the witOperationIndex
// returns -1 if the operation is bad, or, index is not set
int
LgFrMultiPlantHelper::witOperationIndex(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName)
{
  LgFrSceCCOperationAppData * appDataPtr;
  witGetOperationAppData(theWitRun,  fullWitOperationName.c_str(), (void **) &appDataPtr);
  if (appDataPtr == 0) 
    return -1;

  return appDataPtr->index();
}





// (Re)set the partAppData indices to synch with a witGetParts()
// It sets them for all parts and re-sets if an appData already exists
void
LgFrMultiPlantHelper::setAllWitPartIndices(
    WitRun * const theWitRun)
{
  int nParts;
  char ** partList;

  witGetParts (theWitRun, &nParts, &partList );
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    LgFrScePartAppData * partAppDataPtr;
    witGetPartAppData (theWitRun, partList[i], (void **) &partAppDataPtr );
    // if no app data exists, then create one.
    if (partAppDataPtr == 0) {
      int nPeriods;
      witGetNPeriods(theWitRun, &nPeriods);
      partAppDataPtr = new LgFrScePartAppData(0, nPeriods, nPeriods);
      witSetPartAppData(theWitRun, partList[i], (void *) partAppDataPtr);
    }
    partAppDataPtr->index(i);      
  } 

  for (i=0; i<nParts; i++)
    witFree (partList[i]);
  witFree (partList);
}






// set the Customer Choice Operation AppData indices to synch with a witGetOperation()
// It sets them only for Special Customer Choice feature parts
void
LgFrMultiPlantHelper::setCCOperationIndices(
    WitRun * const theWitRun)
{
  int nOperations;
  char ** opList;

  std::string geo;
  std::string machine;
  std::string pdf;
  std::string featurePart;



  witGetOperations (theWitRun, &nOperations, &opList );
  int o = 0; // Pulled out of the for below by RW2STL
  for (o=0; o<nOperations; o++) {
    if (! this->isOperationSpecialCustChoiceFeature(theWitRun, opList[o], geo, machine,pdf,featurePart)) {
      witFree(opList[o]);
      continue;
    }
    
    LgFrSceCCOperationAppData * ccOpAppDataPtr;
    witGetOperationAppData (theWitRun, opList[o], (void **) &ccOpAppDataPtr );
    // if no app data exists, then create one.
    if (ccOpAppDataPtr == 0) {
      ccOpAppDataPtr = new LgFrSceCCOperationAppData(o);
      witSetOperationAppData(theWitRun, opList[o], (void *) ccOpAppDataPtr);
    }
    ccOpAppDataPtr->index(o); 
    witFree(opList[o]);     
  } 

  witFree (opList);
}



  // returns the feature ratio of a customer choice feature.
  // Note that the partname MUST be a specialCustChoicePartName
LgFrTimeVecFloat 
LgFrMultiPlantHelper::custChoiceFeatureRatio(
    WitRun * const theWitRun, 
    const std::string & specialCustChoiceFeature)
{
  std::string geo, plannerTopLevelPart, pppdf, featurePart;
  assert(this->isPartSpecialCustChoiceFeature(theWitRun, specialCustChoiceFeature, geo,
					      plannerTopLevelPart, pppdf,
					      featurePart));

  // Now get the unique bomEntry between this specialCustChoiceFeature and GPD
  int ncbe;
  witGetPartNConsumingBomEntries(theWitRun, specialCustChoiceFeature.c_str(), &ncbe);
  // FINISH_ME: this does not work for multiple geo sourcing 
  assert(ncbe == 1);
  char * gpdWitOperationName;
  int bomIndex;
  witGetPartConsumingBomEntry(theWitRun, specialCustChoiceFeature.c_str(), 0, &gpdWitOperationName, &bomIndex);
  LgFrSceCustChoiceBomAppData * bomAppDataPtr;
  witGetBomEntryAppData(theWitRun, gpdWitOperationName, bomIndex, (void **) &bomAppDataPtr);
  assert(bomAppDataPtr != 0);

  witFree(gpdWitOperationName);
  return bomAppDataPtr->featRatio();
}

// GET 
// this helper goes into wit and finds the right appdata 
// for the Dummysupply Adjustment to be used in MRP
LgFrTimeVecFloat 
LgFrMultiPlantHelper::custChoiceDummySupplyVolForMrpAdjustment(
    WitRun * const theWitRun, 
    const std::string & specialCustChoiceFeature)
{
  std::string geo, plannerTopLevelPart, pppdf, featurePart;
  assert(this->isPartSpecialCustChoiceFeature(theWitRun, specialCustChoiceFeature, geo,
					      plannerTopLevelPart, pppdf,
					      featurePart));

  // Now get the unique bomEntry between this specialCustChoiceFeature and GPD
  int ncbe;
  witGetPartNConsumingBomEntries(theWitRun, specialCustChoiceFeature.c_str(), &ncbe);
  // FINISH_ME: this does not work for multiple geo sourcing 
  assert(ncbe == 1);
  char * gpdWitOperationName;
  int bomIndex;
  witGetPartConsumingBomEntry(theWitRun, specialCustChoiceFeature.c_str(), 0, &gpdWitOperationName, &bomIndex);
  LgFrSceCustChoiceBomAppData * bomAppDataPtr;
  witGetBomEntryAppData(theWitRun, gpdWitOperationName, bomIndex, (void **) &bomAppDataPtr);
  assert(bomAppDataPtr != 0);

  witFree(gpdWitOperationName);
  return bomAppDataPtr->dummySupplyVolForMrpAdjustment();
}


// SET 
// this helper goes into wit and finds the right appdata 
// for the Dummysupply Adjustment to be used in MRP
void 
LgFrMultiPlantHelper::custChoiceDummySupplyVolForMrpAdjustment(
    WitRun * const theWitRun, 
    const std::string & specialCustChoiceFeature,
    LgFrTimeVecFloat & dummySupplyVolForMrpAdjustment)
{
  std::string geo, plannerTopLevelPart, pppdf, featurePart;
  assert(this->isPartSpecialCustChoiceFeature(theWitRun, specialCustChoiceFeature, geo,
					      plannerTopLevelPart, pppdf,
					      featurePart));

  // Now get the unique bomEntry between this specialCustChoiceFeature and GPD
  int ncbe;
  witGetPartNConsumingBomEntries(theWitRun, specialCustChoiceFeature.c_str(), &ncbe);
  // FINISH_ME: this does not work for multiple geo sourcing 
  assert(ncbe == 1);
  char * gpdWitOperationName;
  int bomIndex;
  witGetPartConsumingBomEntry(theWitRun, specialCustChoiceFeature.c_str(), 0, &gpdWitOperationName, &bomIndex);
  LgFrSceCustChoiceBomAppData * bomAppDataPtr;
  witGetBomEntryAppData(theWitRun, gpdWitOperationName, bomIndex, (void **) &bomAppDataPtr);
  assert(bomAppDataPtr != 0);

  witFree(gpdWitOperationName);
  bomAppDataPtr->dummySupplyVolForMrpAdjustment(dummySupplyVolForMrpAdjustment);
}



// returns convenient way to get the nInterplant Ops 
int
LgFrMultiPlantHelper::nInterplantOps(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
  LgFrScePartAppData * appDataPtr;
  witGetPartAppData(theWitRun,  fullWitPartname.c_str(), (void **) &appDataPtr);
  if (appDataPtr == 0) 
    return 0;
  return appDataPtr->nInterplantOps();
}  

// returns convenient way to get the nAlternatePart Ops 
int
LgFrMultiPlantHelper::nAlternatePartOps(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
  LgFrScePartAppData * appDataPtr;
  witGetPartAppData(theWitRun,  fullWitPartname.c_str(), (void **) &appDataPtr);
  if (appDataPtr == 0) 
    return 0;
  return appDataPtr->nAlternatePartOps();
}  





// returns convenient way to get the nUserDefinedOps
int
LgFrMultiPlantHelper::nUserDefinedOps(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
  LgFrScePartAppData * appDataPtr;
  witGetPartAppData(theWitRun,  fullWitPartname.c_str(), (void **) &appDataPtr);
  if (appDataPtr == 0) 
    return 0;
  return appDataPtr->nUserDefinedOps();
}  

  // returns convenient way to get the nAggregationOps
int
LgFrMultiPlantHelper::nAggregationOps(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
  LgFrScePartAppData * appDataPtr;
  witGetPartAppData(theWitRun,  fullWitPartname.c_str(), (void **) &appDataPtr);
  if (appDataPtr == 0) 
    return 0;
  return appDataPtr->nAggregationOps();
}  








std::string
LgFrMultiPlantHelper::pdfPartname(
				  const std::string & part,
				  const std::string & pdf)
const
{
    return pdf + pdfSeparator_ + this->partname(part);
}

std::string
LgFrMultiPlantHelper::pdfOperationName(
    const std::string & operation,
    const std::string & pdf)
const
{
    return pdf + pdfSeparator_ + this->operationName(operation);
}

// Return a demand_Geo name, given the demand, geo
std::string
LgFrMultiPlantHelper::geoPlannerDemandPartName(
    const std::string & plannerPart,
    const std::string & geo)
const
{
    return 
	  geo 
	+ pdfSeparator_ 
	+ plannerPart 
	+ pdfSeparator_ 
	+ "specialGeoPlannerDemand";
}

// Return a demand_GeoUB name, given the demand, geo
std::string
LgFrMultiPlantHelper::geoPlannerDemandUbPartName(
    const std::string & plannerPart,
    const std::string & geo)
const
{
    return 
	  geo 
	+ pdfSeparator_ 
	+ plannerPart 
	+ pdfSeparator_ 
	+ "specialGeoPlannerDemandUB";
}


// Return a interplant operation name, given the parent, parentPdf, 
// child, childPdf
std::string
LgFrMultiPlantHelper::interPlantOperationName(
    const std::string & parentMfgPart,
    const std::string & parentPdf,
    const std::string & childMfgPart,
    const std::string & childPdf)
const
{
    return 
	parentPdf     +
	pdfSeparator_ +	
	parentMfgPart +
	pdfSeparator_ +
	childPdf      +
	pdfSeparator_ +
	childMfgPart  + 
	pdfSeparator_ +
	"specialInterPlantOperation";
}


// Return a alternate operation name, given the parent, parentPdf, 
// child, childPdf
std::string
LgFrMultiPlantHelper::alternatePartOperationName(
    const std::string & primePart,
    const std::string & primePdf,
    const std::string & altPart,
    const std::string & altPdf)
const
{
    return 
	primePdf     +
	pdfSeparator_ +	
	primePart +
	pdfSeparator_ +
	altPdf      +
        pdfSeparator_ +
        altPart  + 
        pdfSeparator_ +
	"specialAlternatePartOperation";
}





#if 0
// this is obsolete, replaced by operationForCapacityGeneration
  // Return an operation name for the purpose of a modelling trick in Smart Exploder
std::string
LgFrMultiPlantHelper::operationForSmartExploderCapacity(
    const std::string & theFullCapacityName)
const
{
    return 
	theFullCapacityName  +
	pdfSeparator_ +	
	"specialSmartExplodeCapacityOperation";
}
#endif

  // Return an operation name for generating capacity
std::string
LgFrMultiPlantHelper::operationForCapacityGeneration(
    const std::string & theFullCapacityName)
const
{
    return 
	theFullCapacityName  +
	pdfSeparator_ +	
	"specialOperationForCapacityGeneration";
}



// Return a aggregation operation name, given the realPart, realPartPdf
// aggPart, and aggPdf
std::string
LgFrMultiPlantHelper::aggregationOperationName(
    const std::string & realPart,
    const std::string & realPartPdf,
    const std::string & aggPart,
    const std::string & aggPdf)
const
{
    return 
	realPartPdf   +
	pdfSeparator_ +	
	realPart      +
	pdfSeparator_ +
	aggPdf        +
	pdfSeparator_ +
	aggPart       + 
	pdfSeparator_ +
	"specialAggregationOperation";
}


// Return a phantom part name, given the part and pdf
std::string
LgFrMultiPlantHelper::phantomPartName(
    const std::string & thePart,
    const std::string & pdf)
const
{
    return 
	pdf           +
	pdfSeparator_ +	
	thePart       +
	"specialPhantomPart";
}

// Return a global nullSubstitute  part name, given a pdf
std::string
LgFrMultiPlantHelper::globalNullSubstitute(
    const std::string & pdf)
const
{
    return 
	pdf           +
	pdfSeparator_ +
      	"nullSub"     +
	pdfSeparator_ +	      
	"specialGlobalNullSubstitute";
}

// Return a globalMaxWithoutNullSubstitute part name, given a pdf.
//
// When the maxWithoutRatio = 0.0 (so we have a "mandatory feature")
// the substitute on the gpd part to feature part arc is a global
// maxWithoutNullSubstitute, whose supply is zero. The reason for 
// employing a "global" instead a distinct null sub in this case
// is to reduce the problem size (many parts in the ECA run have
// the maxWithoutRatio equal to a vector of all zeros).

std::string
LgFrMultiPlantHelper::globalMaxWithoutNullSubstitute(
    const std::string & pdf)
const
{
    return 
	pdf           +
	pdfSeparator_ +
      	"nullSub"     +
	pdfSeparator_ +	      
	"specialGlobalMaxWithoutNullSubstitute";
}


// CUSTOMER_CHOICE_FEATURES 
// This is the nullSubstitute to use for customer choice features
//
// Return a  nullSubstitute  part name, given  geo, planner part and feature.
std::string
LgFrMultiPlantHelper::custChoiceNullSubstitute(
    const std::string & geo,
    const std::string & plannerPart,
    const std::string & featurePart)
const
{
    return 
	geo           +
	pdfSeparator_ +
        plannerPart   + 
	pdfSeparator_ +       
      	featurePart   +
	pdfSeparator_ +	      
	"specialCustChoiceNullSubstitute";
}


// This is the maxWithOutNullSubstitute to use when you want a unique nullSub
// for the unique pair (geo_plannerPart_"specialGeoPlannerDemand", pdf_featurePart)
//
// Because the pdf of the featurePart is function of the pdf sourcing of the plannerPart
// (and we can get it if we need it), the pdf is not encode in the maxWithoutNullSubstitute
// part name.
//
// Return a  nullSubstitute  part name, given  geo, planner part and feature.
// NOTE: this should be used for features with MaxWithout < featratio

std::string
LgFrMultiPlantHelper::maxWithoutNullSubstitute(
    const std::string & geo,
    const std::string & plannerPart,
    const std::string & featurePart)
const
{
    return 
	geo           +
	pdfSeparator_ +
        plannerPart   + 
	pdfSeparator_ +       
      	featurePart   +
	pdfSeparator_ +	      
	"specialMaxWithoutNullSubstitute";
}




// Return a unique standalone Feature Part Name
std::string
LgFrMultiPlantHelper::custChoiceFeaturePartName(
    const std::string & machine,
    const std::string & geo,
    const std::string & featurePart,
    const std::string & pdf)
const
{
    return 
        geo           +
	pdfSeparator_ +	      
        machine       +
	pdfSeparator_ +	      
      	pdf           +
	pdfSeparator_ +	      
      	featurePart   +
	pdfSeparator_ +	      
	"specialCustChoiceFeature";
}

// Returns 1 if part is a special Cust Choice Feature, 0 otherwise
// If returns 1, also sets the geo, the plannerPart, pdf, and featurePart
int
LgFrMultiPlantHelper::isPartSpecialCustChoiceFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & geo,    
    std::string & machine,    
    std::string & pdf,    
    std::string & featurePart)
{
  witBoolean exists;
  
  witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

  if (exists == WitFALSE)
    return 0;
    
  SCETokenizer next(fullWitPartname);

  // geo
  geo =  next(pdfSeparator_.c_str());
  if (geo.empty())
    return 0;

  // machine
  machine =  next(pdfSeparator_.c_str());
  if (machine.empty())
    return 0;

  // pdf
  pdf =  next(pdfSeparator_.c_str());
  if (pdf.empty())
    return 0;

  // featurePartName
  featurePart =  next(pdfSeparator_.c_str());
  if (featurePart.empty())
    return 0;

  std::string anythingSpecial = next(pdfSeparator_.c_str());
  if (anythingSpecial == "specialCustChoiceFeature")
    return 1;

  return 0; 
  
}


// Returns 1 if operation is a special Customer Choice Feature, 0 otherwise
// If returns 1, also sets the geo, the plannerPart, pdf, and featurePart
int
LgFrMultiPlantHelper::isOperationSpecialCustChoiceFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitOperation,
    std::string & geo,    
    std::string & machine,    
    std::string & pdf,    
    std::string & featurePart)
{
  witBoolean exists;
  
  witGetOperationExists(theWitRun, fullWitOperation.c_str(), &exists);

  if (exists == WitFALSE)
    return 0;
    
  SCETokenizer next(fullWitOperation);

  // geo
  geo =  next(pdfSeparator_.c_str());
  if (geo.empty())
    return 0;

  // machine
  machine =  next(pdfSeparator_.c_str());
  if (machine.empty())
    return 0;

  // pdf
  pdf =  next(pdfSeparator_.c_str());
  if (pdf.empty())
    return 0;

  // featurePartName
  featurePart =  next(pdfSeparator_.c_str());
  if (featurePart.empty())
    return 0;

  std::string anythingSpecial = next(pdfSeparator_.c_str());
  if (anythingSpecial == "specialCustChoiceFeature")
    return 1;

  return 0; 
  
}



// Returns 1 if operation is EITHER:
//     special Standalone Feature, 
//  OR  special Customer Choice Feature,
// 0 otherwise
// If returns 1, also sets the geo, the plannerPart, pdf, and featurePart
int
LgFrMultiPlantHelper::isOperationSpecialFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitOperation,
    std::string & geo,    
    std::string & machine,    
    std::string & pdf,    
    std::string & featurePart)
{
  witBoolean exists;
  
  witGetOperationExists(theWitRun, fullWitOperation.c_str(), &exists);

  if (exists == WitFALSE)
    return 0;
    
  SCETokenizer next(fullWitOperation);

  // geo
  geo =  next(pdfSeparator_.c_str());
  if (geo.empty())
    return 0;

  // machine
  machine =  next(pdfSeparator_.c_str());
  if (machine.empty())
    return 0;

  // pdf
  pdf =  next(pdfSeparator_.c_str());
  if (pdf.empty())
    return 0;

  // featurePartName
  featurePart =  next(pdfSeparator_.c_str());
  if (featurePart.empty())
    return 0;

  std::string anythingSpecial = next(pdfSeparator_.c_str());
  if ((anythingSpecial == "specialStandaloneFeature")
      ||
      (anythingSpecial == "specialCustChoiceFeature"))
    return 1;

  return 0; 
  
}



// Returns 1 if part is a special Standalone Feature, 0 otherwise
// (overloaded function.  this one just checks ...
int
LgFrMultiPlantHelper::isPartSpecialCustChoiceFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
  witBoolean exists;
  
  witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

  if (exists == WitFALSE)
    return 0;

    
  SCETokenizer next(fullWitPartname);


  // geo
  std::string geo =  next(pdfSeparator_.c_str());
  if (geo.empty())
    return 0;

  // machine
  std::string machine =  next(pdfSeparator_.c_str());
  if (machine.empty())
    return 0;

  // pdf
  std::string pdf =  next(pdfSeparator_.c_str());
  if (pdf.empty())
    return 0;
  
  // featurePartName
  std::string featurePart =  next(pdfSeparator_.c_str());
  if (featurePart.empty())
    return 0;

  std::string anythingSpecial = next(pdfSeparator_.c_str());
  if (anythingSpecial == "specialCustChoiceFeature")
    return 1;

  return 0; 
  
}

// Returns 1 if part is EITHER:
//     special Standalone Feature, 
//  OR  special Customer Choice Feature,
// 0 otherwise
// (overloaded function.  this one just checks ...
int
LgFrMultiPlantHelper::isPartSpecialFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
  witBoolean exists;
  
  witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

  if (exists == WitFALSE)
    return 0;

    
  SCETokenizer next(fullWitPartname);


  // geo
  std::string geo =  next(pdfSeparator_.c_str());
  if (geo.empty())
    return 0;

  // machine
  std::string machine =  next(pdfSeparator_.c_str());
  if (machine.empty())
    return 0;

  // pdf
  std::string pdf =  next(pdfSeparator_.c_str());
  if (pdf.empty())
    return 0;
  
  // featurePartName
  std::string featurePart =  next(pdfSeparator_.c_str());
  if (featurePart.empty())
    return 0;

  std::string anythingSpecial = next(pdfSeparator_.c_str());
  if ((anythingSpecial == "specialCustChoiceFeature")
      ||
      (anythingSpecial == "specialStandaloneFeature"))
    return 1;

  return 0; 
  
}




// Return a unique standalone Feature Part Name
std::string
LgFrMultiPlantHelper::standaloneFeaturePartName(
    const std::string & machine,
    const std::string & geo,
    const std::string & featurePart,
    const std::string & pdf)
const
{
    return 
        geo           +
	pdfSeparator_ +	      
        machine       +
	pdfSeparator_ +	      
      	pdf           +
	pdfSeparator_ +	      
      	featurePart   +
	pdfSeparator_ +	      
	"specialStandaloneFeature";
}



// Return a unique bbCategory part name
std::string
LgFrMultiPlantHelper::bbCategoryPartName(
    const std::string & plannerTopLevelBuild,
    const std::string & geo,
    const std::string & bbCategory)
const
{
   return 
	geo     +
	pdfSeparator_ +	
	plannerTopLevelBuild +
	pdfSeparator_ +
        bbCategory +
	pdfSeparator_ +
	"specialBbCategory";
}


// Return a unique  part name for LT Feature Set Capacity Part
std::string
LgFrMultiPlantHelper::LTbbCapacityPartName(
    const std::string & plannerTopLevelBuild,
    const std::string & geo,
    const std::string & bbCategory)
const
{
   return 
	geo         +
	pdfSeparator_  +	
	plannerTopLevelBuild +
	pdfSeparator_  +
	bbCategory +
	pdfSeparator_  +
	"specialLTFeatureCapacity";
}


// Return a unique  part name for GT Feature Set Capacity Part
std::string
LgFrMultiPlantHelper::GTbbCapacityPartName(
    const std::string & plannerTopLevelBuild,
    const std::string & geo,
    const std::string & bbCategory)
const
{
   return 
	geo         +
	pdfSeparator_  +	
	plannerTopLevelBuild +
	pdfSeparator_  +
	bbCategory +
	pdfSeparator_  +
	"specialGTFeatureCapacity";
}


// Return a unique part name for an Option Dummy part
std::string
LgFrMultiPlantHelper::optionDummyPartName(
    const std::string & plannerTopLevelBuild,
    const std::string & geo,
    const std::string & bbCategory,
    const std::string & option)
const
{
   return 
	geo         +
	pdfSeparator_  +	
	plannerTopLevelBuild +
	pdfSeparator_  +
	bbCategory +
	pdfSeparator_  +
	option         +
	pdfSeparator_  +
	"specialOptionDummy";
}


// Return a unique part name for an Option Ratio Supply part
std::string
LgFrMultiPlantHelper::optionRatioSupplyPartName(
    const std::string & plannerTopLevelBuild,
    const std::string & geo,
    const std::string & bbCategory,
    const std::string & option)
const
{
   return 
	geo         +
	pdfSeparator_  +	
	plannerTopLevelBuild +
	pdfSeparator_  +
	bbCategory +
	pdfSeparator_  +
	option         +
	pdfSeparator_  +
	"specialOptionRatioSupply";
}


#if 0
// Return the partType of the part
// 1 = Normal
// 2 = Aggragate  dummy
// 3 = BB/feature set dummy
int
LgFrMultiPlantHelper::partType(const std::string & pdfPart)
const
{
    return 1;   // that's it for now
}

// Return the partType of the part
// 1 = Normal
// 2 = Aggragate  dummy
// 3 = BB/feature set dummy
int
LgFrMultiPlantHelper::partType(const std::string & part, const std::string & pdf)
const
{
    return this->partType(pdf + this->pdfSeparator_ + pdf);
}
#endif

// Return 1 if good, 0 if bad
int
LgFrMultiPlantHelper::isPartValid(
    WitRun * const theWitRun, 
    const std::string & partName, 
    const std::string & fileName, 
    const std::string & dataLine, 
    const long lineNo, 
    const int messageLogic)
const
{

    witBoolean exists;

    witGetPartExists(theWitRun, partName.c_str(), &exists);

    if (exists == WitFALSE && messageLogic == MANDATORY)   {
      (*sceErrFacility_)("UnrecognizedPartError",MclArgList() 
			 << partName 
			 << fileName 
			 << (int)lineNo 
			 << dataLine);
      // exits
    }

    if (exists == WitFALSE && messageLogic == OPTIONAL_WITH_MESSAGE)   {
      (*sceErrFacility_)("UnrecognizedPartWarning",MclArgList() << partName << fileName << (int)lineNo << dataLine);
      return 0;
    }

    if (exists == WitFALSE)
	return 0;

    return 1;
}

// Return 1 if good, 0 if bad
int
LgFrMultiPlantHelper::isPartValid(
    WitRun * const theWitRun, 
    const std::string & part,
    const std::string & pdf,
    const std::string & filename, 
    const std::string & dataLine, 
    const long lineNo, 
    const int messageLogic)
const
{
    std::string pdfPart = pdf + pdfSeparator_ + part;
    witBoolean exists;
    exists = this->isPartValid(theWitRun, pdfPart, filename, dataLine, lineNo, PURELY_OPTIONAL);

    if (exists == WitFALSE && messageLogic == MANDATORY)   {
      (*sceErrFacility_)("UnrecognizedPartPdfError",MclArgList() 
			 << part << pdf << filename << (int)lineNo << dataLine);
      // exits
    }

    if (exists == WitFALSE && messageLogic == OPTIONAL_WITH_MESSAGE)   {
      (*sceErrFacility_)("UnrecognizedPartPdfWarning",MclArgList() 
			 << part << pdf << filename << (int)lineNo << dataLine);
      return 0;
    }

    if (exists == WitFALSE)
	return 0;

    return 1;
}





// Return 1 if a normal (no special characters) part, 0 if not

// SCE tidbit:
//             "normal" parts in SCE have two pieces to their name
//             all other special modeling parts have three or more
//             pieces to their name. --RLH
int
LgFrMultiPlantHelper::isPartNormal(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
const
{
    witBoolean exists;

    witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitPartname);
    std::string thePdf = next(pdfSeparator_.c_str());
    assert (! thePdf.empty());

    std::string thePartName = next(pdfSeparator_.c_str());
    assert (! thePartName.empty());

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial.empty())  
	return 1;

    return 0;
}


//
// Return 1 if a "normal" (no special characters) operation, 0 otherwise.
// If method returns 1, the method also sets the pdf and operation

// "Normal" here means that either (1) the operation was defined
//  explicitedly by the user via the Operation Definition File or
// (2) SCE generated the operation from a part that was explicitedly
// defined by the user via the Part Definition File.

// SCE tidbit:
//             "normal" operations in SCE have two pieces to their name
//             all other special modeling operation have three or more
//             pieces to their name. --RLH

// Note: if you want to find out if the operation is specifically a
//       UserDefined Operation, then use isOperationUserDefined()
int
LgFrMultiPlantHelper::isOperationNormal(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & thePdf,
    std::string & theOperationName)
const
{
    witBoolean exists;

    witGetOperationExists(theWitRun, fullWitOperationName.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitOperationName);
    thePdf = next(pdfSeparator_.c_str());
    if (thePdf.empty())
      return 0;

    theOperationName = next(pdfSeparator_.c_str());
    if (theOperationName.empty())
      return 0;

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial.empty())  
	return 1;

    return 0;
}

//
// Return 1 if the operation is "User Defined", 0 ow.
// If method returns 1, the method also sets the pdf and operation
int
LgFrMultiPlantHelper::isOperationUserDefined(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & thePdf,
    std::string & theOperationName)
const
{
  if (this->isOperationNormal(theWitRun, fullWitOperationName, thePdf, theOperationName)) {
    if (this->isPartNormal(theWitRun, fullWitOperationName))
      return 0;
    else
      return 1;
  }
  else
    return 0;
}


// Return 1 if a normal (no special characters) capacity, 0 if not
int
LgFrMultiPlantHelper::isPartNormalCapacity(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
const
{
    witBoolean exists;
    witAttr category;

    witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    witGetPartCategory(theWitRun, fullWitPartname.c_str(), &category);

    if (category != WitCAPACITY)
      return 0;

    
    SCETokenizer next(fullWitPartname);
    std::string thePdf = next(pdfSeparator_.c_str());
    assert (! thePdf.empty());

    std::string thePartName = next(pdfSeparator_.c_str());
    assert (! thePartName.empty());

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial.empty())  
	return 1;

    return 0;
}


// Returns 1 if part is a specialGeoPlannerDemand part, 0 otherwise
// If returns 1, also sets the plannerPart and geo
int
LgFrMultiPlantHelper::isPartSpecialGeoPlannerDemandPart(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerPart,
    std::string & geo)
{
    witBoolean exists;

    witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitPartname);

    // geo
    geo =  next(pdfSeparator_.c_str());
    if (geo.empty())
	return 0;

    // plannerPart
    plannerPart =  next(pdfSeparator_.c_str());
    if (plannerPart.empty())
	return 0;


    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialGeoPlannerDemand")
	return 1;

    return 0; 

}


// Returns 1 if part is a specialGeoPlannerDemandUB part, 0 otherwise
// If returns 1, also sets the plannerPart and geo
int
LgFrMultiPlantHelper::isPartSpecialGeoPlannerDemandUbPart(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerPart,
    std::string & geo)
{
    witBoolean exists;

    witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitPartname);

    // geo
    geo =  next(pdfSeparator_.c_str());
    if (geo.empty())
	return 0;

    // plannerPart
    plannerPart =  next(pdfSeparator_.c_str());
    if (plannerPart.empty())
	return 0;


    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialGeoPlannerDemandUB")
	return 1;

    return 0; 

}


// Returns 1 if operation is a special aggregation operation, 0 otherwise
// If returns 1, also sets the realPart and aggPart and pdf's
int
LgFrMultiPlantHelper::isOperationSpecialAggregation(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & realPart,
    std::string & realPartPdf,
    std::string & aggPart,
    std::string & aggPdf)
{
    witBoolean exists;

    witGetOperationExists(theWitRun, fullWitOperationName.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitOperationName);

    // parentPdf
    realPartPdf =  next(pdfSeparator_.c_str());
    if (realPartPdf.empty())
	return 0;

    // parent
    realPart =  next(pdfSeparator_.c_str());
    if (realPart.empty())
	return 0;

    // childPdf
    aggPdf =  next(pdfSeparator_.c_str());
    if (aggPdf.empty())
	return 0;

    // child
    aggPart =  next(pdfSeparator_.c_str());
    if (aggPart.empty())
	return 0;

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialAggregationOperation")
	return 1;

    return 0; 
}


// Returns 1 if operation is a special interplant operation, 0 otherwise
// If returns 1, also sets the destination and source part and pdf
int
LgFrMultiPlantHelper::isOperationSpecialInterplant(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & destination,
    std::string & destinationPdf,
    std::string & source,
    std::string & sourcePdf)
{
    witBoolean exists;

    witGetOperationExists(theWitRun, fullWitOperationName.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitOperationName);

    // parentPdf
    destinationPdf =  next(pdfSeparator_.c_str());
    if (destinationPdf.empty())
	return 0;

    // parent
    destination =  next(pdfSeparator_.c_str());
    if (destination.empty())
	return 0;

    // childPdf
    sourcePdf =  next(pdfSeparator_.c_str());
    if (sourcePdf.empty())
	return 0;

    // child
    source =  next(pdfSeparator_.c_str());
    if (source.empty())
	return 0;

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialInterPlantOperation")
	return 1;

    return 0; 
}


// Returns 1 if operation is a special alternate part operation, 0 otherwise
// If returns 1, also sets the primePart, primePdf, altPart, and altPdf
int
LgFrMultiPlantHelper::isOperationSpecialAlternate(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & primePart,
    std::string & primePdf,
    std::string & altPart,
    std::string & altPdf)
{
    witBoolean exists;

    witGetOperationExists(theWitRun, fullWitOperationName.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitOperationName);

    // primePdf
    primePdf =  next(pdfSeparator_.c_str());
    if (primePdf.empty())
	return 0;

    // primePart
    primePart =  next(pdfSeparator_.c_str());
    if (primePart.empty())
	return 0;

    // altPdf
    altPdf =  next(pdfSeparator_.c_str());
    if (altPdf.empty())
	return 0;

    // altPart
    altPart =  next(pdfSeparator_.c_str());
    if (altPart.empty())
	return 0;

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialAlternatePartOperation")
	return 1;

    return 0; 
}






// +
// Returns 1 if operation is a special interplant operation and
//              sets the full pdf-part name for the source part
// Otherwise, returns 0
int
LgFrMultiPlantHelper::interplantSourcePdfPartName(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & sourcePdfPartName)
{
    std::string  destination;
    std::string  destinationPdf;
    std::string  source;
    std::string  sourcePdf;

    if ( isOperationSpecialInterplant( theWitRun, fullWitOperationName,
                                       destination, destinationPdf,
                                       source, sourcePdf) ) {
      sourcePdfPartName = pdfPartname( source, sourcePdf);
      return 1;
    }
     
    sourcePdfPartName = "";
    return 0; 
}



// Returns 1 if part is a specialGeoPlannerDemand part, 0 otherwise
// If returns 1, also sets the plannerPart and geo
int
LgFrMultiPlantHelper::isOperationSpecialGeoPlannerDemand(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & plannerPart,
    std::string & geo)
{
    witBoolean exists;

    witGetOperationExists(theWitRun, fullWitOperationName.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitOperationName);

    // geo
    geo =  next(pdfSeparator_.c_str());
    if (geo.empty())
	return 0;

    // plannerPart
    plannerPart =  next(pdfSeparator_.c_str());
    if (plannerPart.empty())
	return 0;


    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialGeoPlannerDemand")
	return 1;

    return 0; 

}

#if 0
// this is obsolete, replaced by operationForCapacityGeneration
// Return 1 if the operation is SmartExpldoer Capacity creator
int
LgFrMultiPlantHelper::isOperationSpecialSmartExploderCapacity(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & theCapacity,
    std::string & pdf)
{
    witBoolean exists;

    witGetOperationExists(theWitRun, fullWitOperationName.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitOperationName);

    // Pdf
    pdf =  next(pdfSeparator_.c_str());
    if (pdf.empty())
	return 0;

    // capacity
    theCapacity =  next(pdfSeparator_.c_str());
    if (theCapacity.empty())
	return 0;

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialSmartExplodeCapacityOperation")
	return 1;

    return 0; 
  
}
#endif



  // Return 1 if the operation is SmartExpldoer Capacity creator
int
LgFrMultiPlantHelper::isOperationForCapacityGeneration(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & theCapacity,
    std::string & pdf)
{
    witBoolean exists;

    witGetOperationExists(theWitRun, fullWitOperationName.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitOperationName);

    // Pdf
    pdf =  next(pdfSeparator_.c_str());
    if (pdf.empty())
	return 0;

    // capacity
    theCapacity =  next(pdfSeparator_.c_str());
    if (theCapacity.empty())
	return 0;

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialOperationForCapacityGeneration")
	return 1;

    return 0; 
  
}





// Returns 1 if the is part is a globalNullSubstitute
//                          or a maxWithoutNullSubstitute,
//                          or a globalMaxWithoutNullSubstitute,
//         0 otherwise
// b/c the tokens in the names for the different types of null subs are NOT analogous, 
// no information encoded in the partname is returned. If information
// encoded in the name of the nullSubstitute is needed, then you'll have to ask
// ::isPartSpecialGlobalNullSubstitute or ::isPartSpecialMaxWithoutNullSubstitute
// or ::isPartSpecialGlobalMaxWithoutNullSubstitute
// to get the null substitute name decoded properly. 
//
// Use this method if you just need to know if the part is one of the three types of null subs.
//
int
LgFrMultiPlantHelper::isPartSpecialNullSubstitute(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
  std::string d1,d2,d3;

  if (isPartSpecialGlobalMaxWithoutNullSubstitute(theWitRun, fullWitPartname, d1))
    return 1;

  if (isPartSpecialMaxWithoutNullSubstitute(theWitRun, fullWitPartname, d1,d2,d3))
    return 1;

  if (isPartSpecialGlobalNullSubstitute(theWitRun, fullWitPartname, d1))
    return 1;

  // CUSTOMER_CHOICE_FEATURES  
  if (isPartSpecialCustChoiceNullSubstitute(theWitRun, fullWitPartname, d1,d2,d3))
    return 1;

  return 0;

}

// Returns 1 if part is a special Global nullSubstitiute, 0 otherwise
// If returns 1, also sets the pdf
int
LgFrMultiPlantHelper::isPartSpecialGlobalNullSubstitute(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & pdf)
{
  witBoolean exists;
  
  witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

  if (exists == WitFALSE)
    return 0;
    
  SCETokenizer next(fullWitPartname);

  // pdf
  pdf =  next(pdfSeparator_.c_str());
  if (pdf.empty())
    return 0;

  // dummyPartName -- this is "nullSub", if the method returns 1.
  std::string dpn =  next(pdfSeparator_.c_str());
  if (dpn.empty())
    return 0;

  std::string anythingSpecial = next(pdfSeparator_.c_str());
  if (anythingSpecial == "specialGlobalNullSubstitute")
    return 1;

  return 0; 

}


#ifdef ALTERNATE_AS_SUB
// Return a Global Alt Dummy part name
std::string
LgFrMultiPlantHelper::globalAltDummyPartName(
					     const std::string & primePdf)
const
{
    return 
	primePdf     +
	pdfSeparator_ +	
	primePdf     +
	pdfSeparator_ +	
	"specialGlobalAltDummy";
}



// Returns 1 if part is a special Global Alt Dummy, 0 otherwise
// If returns 1, also sets the pdf
int
LgFrMultiPlantHelper::isPartSpecialGlobalAltDummy(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & pdf)
{
  witBoolean exists;
  
  witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

  if (exists == WitFALSE)
    return 0;
    
  SCETokenizer next(fullWitPartname);

  // pdf
  pdf =  next(pdfSeparator_.c_str());
  if (pdf.empty())
    return 0;

  // pdf (appears twice)
  pdf =  next(pdfSeparator_.c_str());
  if (pdf.empty())
    return 0;


  // dummyPartName -- 
  std::string dpn =  next(pdfSeparator_.c_str());
  if (dpn.empty())
    return 0;

  std::string anythingSpecial = next(pdfSeparator_.c_str());
  if (anythingSpecial == "specialGlobalAltDummy")
    return 1;

  return 0; 

}

#endif















// Returns 1 if part is a special GlobalMaxWithoutNullSubstitiute,
//         0 otherwise
// If returns 1, also sets the pdf
int
LgFrMultiPlantHelper::isPartSpecialGlobalMaxWithoutNullSubstitute(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & pdf)
{
  witBoolean exists;
  
  witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

  if (exists == WitFALSE)
    return 0;
    
  SCETokenizer next(fullWitPartname);

  // pdf
  pdf =  next(pdfSeparator_.c_str());
  if (pdf.empty())
    return 0;

  // dummyPartName -- this is "nullSub", if the method returns 1.
  std::string dpn =  next(pdfSeparator_.c_str());
  if (dpn.empty())
    return 0;

  std::string anythingSpecial = next(pdfSeparator_.c_str());
  if (anythingSpecial == "specialGlobalMaxWithoutNullSubstitute")
    return 1;

  return 0; 

}


// Returns 1 if part is a special MaxWithout nullSubstitiute, 0 otherwise
// If returns 1, also sets the geo, the plannerPart, and featurePart
int
LgFrMultiPlantHelper::isPartSpecialMaxWithoutNullSubstitute(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & geo,
    std::string & plannerPart,
    std::string & featurePart)
{
  witBoolean exists;
  
  witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

  if (exists == WitFALSE)
    return 0;
    
  SCETokenizer next(fullWitPartname);

  // geo
  geo = next(pdfSeparator_.c_str());
  if (geo.empty())
    return 0;

  // plannerPartName
  plannerPart =  next(pdfSeparator_.c_str());
  if (plannerPart.empty())
    return 0;

  // featurePartName
  featurePart =  next(pdfSeparator_.c_str());
  if (featurePart.empty())
    return 0;

  std::string anythingSpecial = next(pdfSeparator_.c_str());
  if (anythingSpecial == "specialMaxWithoutNullSubstitute")
    return 1;

  return 0; 

}

// CUSTOMER_CHOICE_FEATURES
// Returns 1 if part is a special CustChoiceNullSubstitiute,
//         0 otherwise
// If returns 1, also sets the pdf
int
LgFrMultiPlantHelper::isPartSpecialCustChoiceNullSubstitute(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & geo,
    std::string & plannerPart,
    std::string & featurePart)
{
  witBoolean exists;
  
  witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

  if (exists == WitFALSE)
    return 0;
    
  SCETokenizer next(fullWitPartname);

  // geo
  geo = next(pdfSeparator_.c_str());
  if (geo.empty())
    return 0;

  // plannerPartName
  plannerPart =  next(pdfSeparator_.c_str());
  if (plannerPart.empty())
    return 0;

  // featurePartName
  featurePart =  next(pdfSeparator_.c_str());
  if (featurePart.empty())
    return 0;

  std::string anythingSpecial = next(pdfSeparator_.c_str());
  if (anythingSpecial == "specialCustChoiceNullSubstitute")
    return 1;

  return 0; 

}






// Returns 1 if part is a special Standalone Feature, 0 otherwise
// If returns 1, also sets the geo, the plannerPart, pdf, and featurePart
int
LgFrMultiPlantHelper::isPartSpecialStandaloneFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & geo,    
    std::string & machine,    
    std::string & pdf,    
    std::string & featurePart)
{
  witBoolean exists;
  
  witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

  if (exists == WitFALSE)
    return 0;
    
  SCETokenizer next(fullWitPartname);

  // geo
  geo =  next(pdfSeparator_.c_str());
  if (geo.empty())
    return 0;

  // machine
  machine =  next(pdfSeparator_.c_str());
  if (machine.empty())
    return 0;

  // pdf
  pdf =  next(pdfSeparator_.c_str());
  if (pdf.empty())
    return 0;

  // featurePartName
  featurePart =  next(pdfSeparator_.c_str());
  if (featurePart.empty())
    return 0;

  std::string anythingSpecial = next(pdfSeparator_.c_str());
  if (anythingSpecial == "specialStandaloneFeature")
    return 1;

  return 0; 
  
}


// Returns 1 if operation is a special Standalone Feature, 0 otherwise
// If returns 1, also sets the geo, the plannerPart, pdf, and featurePart
int
LgFrMultiPlantHelper::isOperationSpecialStandaloneFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitOperation,
    std::string & geo,    
    std::string & machine,    
    std::string & pdf,    
    std::string & featurePart)
{
  witBoolean exists;
  
  witGetOperationExists(theWitRun, fullWitOperation.c_str(), &exists);

  if (exists == WitFALSE)
    return 0;
    
  SCETokenizer next(fullWitOperation);

  // geo
  geo =  next(pdfSeparator_.c_str());
  if (geo.empty())
    return 0;

  // machine
  machine =  next(pdfSeparator_.c_str());
  if (machine.empty())
    return 0;

  // pdf
  pdf =  next(pdfSeparator_.c_str());
  if (pdf.empty())
    return 0;

  // featurePartName
  featurePart =  next(pdfSeparator_.c_str());
  if (featurePart.empty())
    return 0;

  std::string anythingSpecial = next(pdfSeparator_.c_str());
  if (anythingSpecial == "specialStandaloneFeature")
    return 1;

  return 0; 
  
}



// Returns 1 if part is a special Standalone Feature, 0 otherwise
// (overloaded function.  this one just checks ...
int
LgFrMultiPlantHelper::isPartSpecialStandaloneFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
  witBoolean exists;
  
  witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

  if (exists == WitFALSE)
    return 0;

    
  SCETokenizer next(fullWitPartname);


  // geo
  std::string geo =  next(pdfSeparator_.c_str());
  if (geo.empty())
    return 0;

  // machine
  std::string machine =  next(pdfSeparator_.c_str());
  if (machine.empty())
    return 0;

  // pdf
  std::string pdf =  next(pdfSeparator_.c_str());
  if (pdf.empty())
    return 0;
  
  // featurePartName
  std::string featurePart =  next(pdfSeparator_.c_str());
  if (featurePart.empty())
    return 0;

  std::string anythingSpecial = next(pdfSeparator_.c_str());
  if (anythingSpecial == "specialStandaloneFeature")
    return 1;

  return 0; 
  
}



// Returns 1 if part is a special phantom part, 0 otherwise
int
LgFrMultiPlantHelper::isPartSpecialPhantom(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
    witBoolean exists;

    witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitPartname);

    // Pdf
    std::string pdf =  next(pdfSeparator_.c_str());
    if (pdf.empty())
	return 0;

    // part
    std::string aPart =  next(pdfSeparator_.c_str());
    if (aPart.empty())
	return 0;

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialPhantomPart")
	return 1;

    return 0; 
}


// Returns 1 if part is a special BB category Set  part, 0 otherwise
// If returns 1, also sets the plannerTopLevelBuild, geo, and bbCategory
int
LgFrMultiPlantHelper::isPartSpecialBbCategory(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerTopLevelBuild,
    std::string & geo,
    std::string & bbCategory)
{
    witBoolean exists;

    witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitPartname);

    // geo
    geo =  next(pdfSeparator_.c_str());
    if (geo.empty())
	return 0;

    // plannerTopLevelBuild
    plannerTopLevelBuild =  next(pdfSeparator_.c_str());
    if (plannerTopLevelBuild.empty())
	return 0;

    // bbCategory
    bbCategory =  next(pdfSeparator_.c_str());
    if (bbCategory.empty())
	return 0;

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialBbCategory")
	return 1;

    return 0;
}






// Returns 1 if part is a special feature Set LT capacity part, 0 otherwise
// If returns 1, also sets the part and pdf
int
LgFrMultiPlantHelper::isPartSpecialLTbbCapacity(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerTopLevelBuild,
    std::string & geo,
    std::string & bbCategory)
{
    witBoolean exists;

    witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitPartname);

    // geo
    geo =  next(pdfSeparator_.c_str());
    if (geo.empty())
	return 0;

    // plannerTopLevelBuild
    plannerTopLevelBuild =  next(pdfSeparator_.c_str());
    if (plannerTopLevelBuild.empty())
	return 0;

    // bbCategory
    bbCategory =  next(pdfSeparator_.c_str());
    if (bbCategory.empty())
	return 0;

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialLTFeatureCapacity")
	return 1;

    return 0;
}

// Returns 1 if part is a special feature Set LT capacity part, 0 otherwise
// If returns 1, also sets the part and pdf
int
LgFrMultiPlantHelper::isPartSpecialGTbbCapacity(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerTopLevelBuild,
    std::string & geo,
    std::string & bbCategory)
{
    witBoolean exists;

    witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitPartname);

    // geo
    geo =  next(pdfSeparator_.c_str());
    if (geo.empty())
	return 0;

    // plannerTopLevelBuild
    plannerTopLevelBuild =  next(pdfSeparator_.c_str());
    if (plannerTopLevelBuild.empty())
	return 0;

    // bbCategory
    bbCategory =  next(pdfSeparator_.c_str());
    if (bbCategory.empty())
	return 0;

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialGTFeatureCapacity")
	return 1;

    return 0;

}

// Returns 1 if part is a special Option dummy, 0 otherwise
// If returns 1, also sets the plannerTopLevelBuild, geo, bbCategory,
// and mfgOption part.
int
LgFrMultiPlantHelper::isPartSpecialOptionDummy(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerTopLevelBuild,
    std::string & geo,
    std::string & bbCategory,
    std::string & mfgOptionPart)
{
    witBoolean exists;

    witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitPartname);

    // geo
    geo =  next(pdfSeparator_.c_str());
    if (geo.empty())
	return 0;

    // plannerTopLevelBuild
    plannerTopLevelBuild =  next(pdfSeparator_.c_str());
    if (plannerTopLevelBuild.empty())
	return 0;

    // bbCategory
    bbCategory =  next(pdfSeparator_.c_str());
    if (bbCategory.empty())
	return 0;

    // bbCategory
    mfgOptionPart =  next(pdfSeparator_.c_str());
    if (mfgOptionPart.empty())
	return 0;


    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialOptionDummy")
	return 1;

    return 0;

}


// Returns 1 if Operation is a special Option dummy, 0 otherwise
// If returns 1, also sets the plannerTopLevelBuild, geo, bbCategory,
// and mfgOption part.
int
LgFrMultiPlantHelper::isOperationSpecialOptionDummy(
    WitRun * const theWitRun, 
    const std::string & fullWitOperationName,
    std::string & plannerTopLevelBuild,
    std::string & geo,
    std::string & bbCategory,
    std::string & mfgOptionPart)
{
    witBoolean exists;

    witGetOperationExists(theWitRun, fullWitOperationName.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitOperationName);

    // geo
    geo =  next(pdfSeparator_.c_str());
    if (geo.empty())
	return 0;

    // plannerTopLevelBuild
    plannerTopLevelBuild =  next(pdfSeparator_.c_str());
    if (plannerTopLevelBuild.empty())
	return 0;

    // bbCategory
    bbCategory =  next(pdfSeparator_.c_str());
    if (bbCategory.empty())
	return 0;

    // bbCategory
    mfgOptionPart =  next(pdfSeparator_.c_str());
    if (mfgOptionPart.empty())
	return 0;


    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialOptionDummy")
	return 1;

    return 0;

}




// Returns 1 if part is a special Option Ratio Supply, 0 otherwise
// If returns 1, also sets the plannerTopLevelBuild, geo, bbCategory,
// and mfgOption part.
int
LgFrMultiPlantHelper::isPartSpecialOptionRatioSupply(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname,
    std::string & plannerTopLevelBuild,
    std::string & geo,
    std::string & bbCategory,
    std::string & mfgOptionPart)
{
    witBoolean exists;

    witGetPartExists(theWitRun, fullWitPartname.c_str(), &exists);

    if (exists == WitFALSE)
	return 0;

    
    SCETokenizer next(fullWitPartname);

    // geo
    geo =  next(pdfSeparator_.c_str());
    if (geo.empty())
	return 0;

    // plannerTopLevelBuild
    plannerTopLevelBuild =  next(pdfSeparator_.c_str());
    if (plannerTopLevelBuild.empty())
	return 0;

    // bbCategory
    bbCategory =  next(pdfSeparator_.c_str());
    if (bbCategory.empty())
	return 0;

    // bbCategory
    mfgOptionPart =  next(pdfSeparator_.c_str());
    if (mfgOptionPart.empty())
	return 0;


    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialOptionRatioSupply")
	return 1;

    return 0;

}



// Return 1 if a part is Buildable (ie, should be in the Build Schedule)
int
LgFrMultiPlantHelper::isPartBuildable(
    WitRun * const theWitRun, 
    const std::string & fullWitPartname)
{
    if (! this->isPartNormal(theWitRun, fullWitPartname))
      return 0;

    if (this->isPartNormalCapacity(theWitRun, fullWitPartname))
	return 0;
    
    if (this->isPartPcf(theWitRun, fullWitPartname))
      return 0;

    witBoolean exists;
    witGetOperationExists(theWitRun, fullWitPartname.c_str(), &exists);
    if (! exists)
      return 0;

    return 1;
}

int
LgFrMultiPlantHelper::isOperationValid(
    WitRun * const theWitRun, 
    const std::string & pdfOperationName, 
    const std::string & fileName, 
    const std::string & dataLine, 
    const long lineNo, 
    const int messageLogic)
const
{

    witBoolean exists;

    witGetOperationExists(theWitRun, pdfOperationName.c_str(), &exists);

    if (exists == WitFALSE && messageLogic == MANDATORY)   {
      (*sceErrFacility_)("UnrecognizedOperationError",MclArgList() << pdfOperationName << fileName << (int)lineNo << dataLine);
      // exits
    }

    if (exists == WitFALSE && messageLogic == OPTIONAL_WITH_MESSAGE)   {
      (*sceErrFacility_)("UnrecognizedOperationWarning",MclArgList() << pdfOperationName << fileName << (int)lineNo << dataLine);
      return 0;
    }

    if (exists == WitFALSE)
	return 0;

    return 1;
}

// Return 1 if good, 0 if bad
int
LgFrMultiPlantHelper::isOperationValid(
    WitRun * const theWitRun, 
    const std::string & operationName,
    const std::string & pdf,
    const std::string & filename, 
    const std::string & dataLine, 
    const long lineNo, 
    const int messageLogic)
const
{
    std::string pdfOperation = pdf + pdfSeparator_ + operationName;
    witBoolean exists;
    exists = this->isOperationValid(theWitRun, pdfOperation, filename, dataLine, lineNo, PURELY_OPTIONAL);

    if (exists == WitFALSE && messageLogic == MANDATORY)   {
      (*sceErrFacility_)("UnrecognizedOperationPdfError",MclArgList() 
			 << operationName << pdf << filename << (int)lineNo << dataLine);
      // exits
    }

    if (exists == WitFALSE && messageLogic == OPTIONAL_WITH_MESSAGE)   {
      (*sceErrFacility_)("UnrecognizedOperationPdfWarning",MclArgList() 
			 << operationName << pdf << filename << (int)lineNo << dataLine);
      return 0;
    }

    if (exists == WitFALSE)
	return 0;

    return 1;
}


int
LgFrMultiPlantHelper::isPdfOperationUnique(
    WitRun * const theWitRun, 
    const std::string & pdfOperationName, 
    const std::string & fileName, 
    const std::string & dataLine, 
    const long lineNo, 
    const int messageLogic)
const
{

    witBoolean exists;

    // check to see if a there is a pdf_part using the pdf_operation name.
    witGetPartExists(theWitRun, pdfOperationName.c_str(), &exists);

    if (exists == WitTRUE && messageLogic == MANDATORY)   {
      (*sceErrFacility_)("DuplicatePartPDFPairError",MclArgList() << pdfOperationName << fileName << (int)lineNo << dataLine);
      // exits
    }

    if (exists == WitTRUE && messageLogic == OPTIONAL_WITH_MESSAGE)   {
      (*sceErrFacility_)("DuplicatePartPDFPairWarning",MclArgList() << pdfOperationName << fileName << (int)lineNo << dataLine);
      return 0;
    }

    if (exists == WitTRUE)
	return 0;

    return 1;
}


// Return a geo_plannerPart_specialPureOptionBuildDemandName, given the
// fullWitName of a specialGeoPlannerDemand part
std::string
LgFrMultiPlantHelper::pureOptionBuildDemandName(
    const std::string & fullWitPartName)

{
    std::string  plannerPart;
    std::string  geo;

    SCETokenizer next(fullWitPartName);

    // geo
    geo =  next(pdfSeparator_.c_str());
    assert (! (geo.empty()));

    // plannerPart
    plannerPart =  next(pdfSeparator_.c_str());
    assert (! (plannerPart.empty()));

    return 
	  geo 
	+ pdfSeparator_ 
	+ plannerPart 
	+ pdfSeparator_ 
	+ "specialPureOptionBuildDemandName";
}




// Returns 1 if demand is a specialPureOptionBuildDemand, 0 otherwise
// If returns 1, also sets the plannerPart and geo
int
LgFrMultiPlantHelper::isDemandSpecialPureOptionBuildDemand(
    WitRun * const theWitRun, 
    const std::string & fullWitPartName,
    const std::string & fullWitDemandName,
    std::string & plannerPart,
    std::string & geo)
{
    witBoolean partExists;
    witBoolean demandExists;

    // check to see if the part exists
    witGetPartExists(theWitRun, fullWitPartName.c_str(), &partExists);

    if (partExists == WitFALSE)
	return 0;

    // check to see if the demand exists
    int  lenDemandList;
    char ** demandList;
    witGetPartDemands ( theWitRun, fullWitPartName.c_str(), &lenDemandList, &demandList);

    // loop through the demand list searching for a match
    // if you find one, set the boolean to True and break out of the loop
    demandExists = WitFALSE;
    int i = 0; // Pulled out of the for below by RW2STL
    for (i=0; i<lenDemandList; i++){
      if (fullWitDemandName == demandList[i]) {
        demandExists = WitTRUE;
        break;
      }
    }
    
    // we're done with demandList -- get rid of it
    for (i=0; i<lenDemandList; i++)
      witFree(demandList[i]);
    witFree(demandList);
    
    if (demandExists == WitFALSE) 
      return 0;

    // parse the fullWitDemandName to get the geo and plannerPart
    SCETokenizer next(fullWitDemandName);

    // geo
    geo =  next(pdfSeparator_.c_str());
    if (geo.empty()) {
      return 0;
    }
    
    // plannerPart
    plannerPart =  next(pdfSeparator_.c_str());
    if (plannerPart.empty()){
      return 0;
    }
	

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialPureOptionBuildDemandName"){
      return 1;
    }
    

    return 0; 

}


// CUSTOMER_CHOICE_FEATURES
// Return a geo_plannerPart_specialCustChoiceDemandName, given the
// fullWitName of a specialGeoPlannerDemand part
std::string
LgFrMultiPlantHelper::custChoiceFeatureDemandName(
    const std::string & fullWitPartName)

{
    std::string  plannerPart;
    std::string  geo;

    SCETokenizer next(fullWitPartName);

    // geo
    geo =  next(pdfSeparator_.c_str());
    assert (! (geo.empty()));

    // plannerPart
    plannerPart =  next(pdfSeparator_.c_str());
    assert (! (plannerPart.empty()));

    return 
	  geo 
	+ pdfSeparator_ 
	+ plannerPart 
	+ pdfSeparator_ 
	+ "specialCustChoiceFeatureDemand";
}


// Returns 1 if demand is a specialPureOptionBuildDemand, 0 otherwise
// If returns 1, also sets the plannerPart and geo
int
LgFrMultiPlantHelper::isDemandSpecialCustChoiceFeature(
    WitRun * const theWitRun, 
    const std::string & fullWitPartName,
    const std::string & fullWitDemandName,
    std::string & plannerPart,
    std::string & geo)
{
    witBoolean partExists;
    witBoolean demandExists;

    // check to see if the part exists
    witGetPartExists(theWitRun, fullWitPartName.c_str(), &partExists);

    if (partExists == WitFALSE)
	return 0;

    // check to see if the demand exists
    int  lenDemandList;
    char ** demandList;
    witGetPartDemands ( theWitRun, fullWitPartName.c_str(), &lenDemandList, &demandList);

    // loop through the demand list searching for a match
    // if you find one, set the boolean to True and break out of the loop
    demandExists = WitFALSE;
    int i = 0; // Pulled out of the for below by RW2STL
    for (i=0; i<lenDemandList; i++){
      if (fullWitDemandName == demandList[i]) {
        demandExists = WitTRUE;
        break;
      }
    }
    
    // we're done with demandList -- get rid of it
    for (i=0; i<lenDemandList; i++)
      witFree(demandList[i]);
    witFree(demandList);
    
    if (demandExists == WitFALSE) 
      return 0;

    // parse the fullWitDemandName to get the geo and plannerPart
    SCETokenizer next(fullWitDemandName);

    // geo
    geo =  next(pdfSeparator_.c_str());
    if (geo.empty()) {
      return 0;
    }
    
    // plannerPart
    plannerPart =  next(pdfSeparator_.c_str());
    if (plannerPart.empty()){
      return 0;
    }
	

    std::string anythingSpecial = next(pdfSeparator_.c_str());
    if (anythingSpecial == "specialCustChoiceFeatureDemand"){
      return 1;
    }
    

    return 0; 

}






// set/get the pdf-partname delimiter string
std::string
LgFrMultiPlantHelper::pdfSeparator()
const
{
    return pdfSeparator_;
}

void
LgFrMultiPlantHelper::pdfSeparator(
    const std::string & pdfSeparator)
{
    pdfSeparator_ = pdfSeparator;
}

// set/get the default PDF string
std::string
LgFrMultiPlantHelper::defaultPdf()
const
{
    return defaultPdf_;
}

void
LgFrMultiPlantHelper::defaultPdf(
    const std::string & defaultPdf)
{
    defaultPdf_ = defaultPdf;
}


// multi_attribute_demand
void
LgFrMultiPlantHelper::multiAttributeDemandSeparator(
    const std::string & multiAttributeDemandSeparator)
{
    multiAttributeDemandSeparator_ = multiAttributeDemandSeparator;
}

std::string
LgFrMultiPlantHelper::multiAttributeDemandSeparator()
const
{
    return multiAttributeDemandSeparator_;
}






void
LgFrMultiPlantHelper::useMultiAttributeDemand(
    const bool useMultiAttributeDemand)
{
    useMultiAttributeDemand_ = useMultiAttributeDemand;
}

bool
LgFrMultiPlantHelper::useMultiAttributeDemand()
const
{
    return useMultiAttributeDemand_;
}


void
LgFrMultiPlantHelper::useDemand2OrderINDP(
    const bool useDemand2OrderINDP)
{
    useDemand2OrderINDP_ = useDemand2OrderINDP;
}

bool
LgFrMultiPlantHelper::useDemand2OrderINDP()
const
{
    return useDemand2OrderINDP_;
}






void
LgFrMultiPlantHelper::truncOffsetToEol(
    const bool truncOffsetToEol)
{
    truncOffsetToEol_ = truncOffsetToEol;
}

// set/get the default PDF string
bool
LgFrMultiPlantHelper::truncOffsetToEol()
const
{
    return truncOffsetToEol_;
}


void
LgFrMultiPlantHelper::numDemandAttributes(
    const int numDemandAttributes)
{
    numDemandAttributes_ = numDemandAttributes;
}

int
LgFrMultiPlantHelper::numDemandAttributes()
const
{
    return numDemandAttributes_;
}




// Get the  PDF for a PDF_PART
std::string 
LgFrMultiPlantHelper::pdf(const LgFrPart & part)
const
{
    return this->pdf(part.name());
}



// Parse the string looking for two pieces, the PDF and
// the partname.  If you only find one piece, then assume
// that this is a partname and that the PDF is the default.
std::string 
LgFrMultiPlantHelper::pdf(const std::string & pdfPart)
const
{
    SCETokenizer next(pdfPart);
    std::string thePdf = next(pdfSeparator_.c_str());
    assert (! thePdf.empty());

    std::string thePartName = next(pdfSeparator_.c_str());
    if (thePartName.empty())
	return defaultPdf_;

    return thePdf;
}


std::string 
LgFrMultiPlantHelper::pdf(const char * pdfPart)
const
{
    return this->pdf(std::string(pdfPart));
}

// Parse the string looking for two pieces, the PDF and
// the partname.  If you only find one piece, then assume
// that this is THE partname`
std::string 
LgFrMultiPlantHelper::partname(const std::string & pdfPart)
const
{
    SCETokenizer next(pdfPart);
    std::string thePdf = next(pdfSeparator_.c_str());
    assert (! thePdf.empty());

    std::string thePartName = next(pdfSeparator_.c_str());
    if (thePartName.empty())
	return thePdf;

    return thePartName;
}


// Return just the operationName of a pdf_operationName.
// Parse the string looking for two pieces, the PDF and
// the operationName.  If you only find one piece, then assume
// that this is THE operationName.
std::string 
LgFrMultiPlantHelper::operationName(const std::string & pdfOperation)
const
{
    return this->partname(pdfOperation);
}

// Are the parts equivalent, irrespective of the PDF?


bool
LgFrMultiPlantHelper::isPartnameEqual(const std::string & left, const std::string & right)
const
{
    return (this->partname(left) == this->partname(right));
}

// 
// returns 1 if the duplicate interplant record's usage rate is the same
//              as the usage rate on the previous record (ie. the current
//              usage rate on the interplant bom)
//         0 otherwise
int
LgFrMultiPlantHelper::isDuplicateInterplantRecordUsageRateValid(
    WitRun * const theWitRun, 
    const std::string & sourcePdfPartName,
    const std::string & interplantOperationName,
    float duplicateRecordUsageRate )
{
  int nBoms;
  witGetOperationNBomEntries( theWitRun,
                              interplantOperationName.c_str(),
                              &nBoms );
  assert ( nBoms == 1 );

  float currentUsageRate;
  witGetBomEntryUsageRate( theWitRun,
                           interplantOperationName.c_str(),
                           0,
                           &currentUsageRate );

  // if usage rates are not equal, then return 0
  if (( duplicateRecordUsageRate > (currentUsageRate + MULTIPLANTHELPER_FLT_EPS)) ||
      ( duplicateRecordUsageRate < (currentUsageRate - MULTIPLANTHELPER_FLT_EPS))) {
    return (0);
  }

  // the usage rate are equal
  return (1) ;
}

//
// returns 1 if the duplicate interplant record's effectivity
//              dates do not overlap with the effectivity
//              dates of interplant operation's existing bops
//         0 otherwise
int
LgFrMultiPlantHelper::isDuplicateInterplantRecordEffectivityDatesValid(
    WitRun * const theWitRun, 
    const std::string & interplantOperationName,
    int duplicateRecordStart,
    int duplicateRecordEnd )
{
  if ( duplicateRecordStart > duplicateRecordEnd )
    return 0;
  
  int nbBops;
  witGetOperationNBopEntries( theWitRun,
                              interplantOperationName.c_str(),
                              &nbBops );
  
  size_t b = 0; // Pulled out of the for below by RW2STL
  for ( b=0; b<nbBops; b++) {
    int currentBopStart;
    int currentBopEnd;
    
    witGetBopEntryEarliestPeriod( theWitRun,
                                  interplantOperationName.c_str(),
                                  b,
                                  &currentBopStart);
    
    witGetBopEntryLatestPeriod( theWitRun,
                                interplantOperationName.c_str(),
                                b,
                                &currentBopEnd);

    // For now, if there's a bop in there with effectivity turned off,
    // we have problems...
    assert ( currentBopStart <= currentBopEnd );

    // if they're not "disjoint", the record's invalid. Return 0.
    if (!( ((duplicateRecordStart < currentBopStart) && 
           (duplicateRecordEnd < currentBopStart))  // before
          ||                                         // or
          ((duplicateRecordStart > currentBopEnd) && // after 
          (duplicateRecordEnd > currentBopEnd))) )

      return 0;
  }
  // none of the records overlapped. the record's valid. Return 1.
  return 1;
}


// 
// returns 1 if the duplicate interplant record's usage rate is the same
//              as the usage rate on the previous record (ie. the current
//              usage rate on the interplant bom)
//         0 otherwise
int
LgFrMultiPlantHelper::isDuplicateAlternatePartRecordUsageRateValid(
    WitRun * const theWitRun, 
    const std::string & fullAltPartPdfName,
    const std::string & alternatePartOperationName,
    float duplicateRecordUsageRate )
{
  int nBoms;
  witGetOperationNBomEntries( theWitRun,
                              alternatePartOperationName.c_str(),
                              &nBoms );
  assert ( nBoms == 1 );

  float currentUsageRate;
  witGetBomEntryUsageRate( theWitRun,
                           alternatePartOperationName.c_str(),
                           0,
                           &currentUsageRate );

  // if usage rates are not equal, then return 0
  if (( duplicateRecordUsageRate > (currentUsageRate + MULTIPLANTHELPER_FLT_EPS)) ||
      ( duplicateRecordUsageRate < (currentUsageRate - MULTIPLANTHELPER_FLT_EPS))) {
    return (0);
  }

  // the usage rate are equal
  return (1) ;
}

//
// returns 1 if the duplicate alternatePart record's effectivity
//              dates do not overlap with the effectivity
//              dates of alternatePart operation's existing bops
//         0 otherwise
int
LgFrMultiPlantHelper::isDuplicateAlternatePartRecordEffectivityDatesValid(
    WitRun * const theWitRun, 
    const std::string & alternatePartOperationName,
    int duplicateRecordStart,
    int duplicateRecordEnd )
{
  if ( duplicateRecordStart > duplicateRecordEnd )
    return 0;
  
  int nbBops;
  witGetOperationNBopEntries( theWitRun,
                              alternatePartOperationName.c_str(),
                              &nbBops );
  
  size_t b = 0; // Pulled out of the for below by RW2STL
  for ( b=0; b<nbBops; b++) {
    int currentBopStart;
    int currentBopEnd;
    
    witGetBopEntryEarliestPeriod( theWitRun,
                                  alternatePartOperationName.c_str(),
                                  b,
                                  &currentBopStart);
    
    witGetBopEntryLatestPeriod( theWitRun,
                                alternatePartOperationName.c_str(),
                                b,
                                &currentBopEnd);

    // For now, if there's a bop in there with effectivity turned off,
    // we have problems...
    assert ( currentBopStart <= currentBopEnd );

    // if they're not "disjoint", the record's invalid. Return 0.
    if (!( ((duplicateRecordStart < currentBopStart) && 
           (duplicateRecordEnd < currentBopStart))  // before
          ||                                         // or
          ((duplicateRecordStart > currentBopEnd) && // after 
          (duplicateRecordEnd > currentBopEnd))) )

      return 0;
  }
  // none of the records overlapped. the record's valid. Return 1.
  return 1;
}







    
// Return a copy in the heap
LgFrMultiPlantHelper*
LgFrMultiPlantHelper::clone()
     const
{
  return new LgFrMultiPlantHelper(*this);
}



 // copy constructor
LgFrMultiPlantHelper::LgFrMultiPlantHelper(const LgFrMultiPlantHelper& source)
: pdfSeparator_(source.pdfSeparator_),
  defaultPdf_(source.defaultPdf_),
  useMultiAttributeDemand_(source.useMultiAttributeDemand_),
  truncOffsetToEol_(source.truncOffsetToEol_),
  numDemandAttributes_(source.numDemandAttributes_),
  multiAttributeDemandSeparator_(source.multiAttributeDemandSeparator_),
  useDemand2OrderINDP_(source.useDemand2OrderINDP_),
  sceErrFacility_(source.sceErrFacility_)
{
  // nothing to do
}

// assignment operator
LgFrMultiPlantHelper&
LgFrMultiPlantHelper::operator=(const LgFrMultiPlantHelper& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    pdfSeparator_ = rhs.pdfSeparator_;
    defaultPdf_ = rhs.defaultPdf_;
    useMultiAttributeDemand_ = rhs.useMultiAttributeDemand_;
    truncOffsetToEol_ = rhs.truncOffsetToEol_;
    numDemandAttributes_ = rhs.numDemandAttributes_;
    multiAttributeDemandSeparator_ = rhs.multiAttributeDemandSeparator_;
    useDemand2OrderINDP_ = rhs.useDemand2OrderINDP_;
    sceErrFacility_ = rhs.sceErrFacility_;
  }
  return *this;
}

// destructor
LgFrMultiPlantHelper::~LgFrMultiPlantHelper()
{
  // nothing to do
}


#ifdef NDEBUG
#undef NDEBUG
#endif



// self-test
void
LgFrMultiPlantHelper::test()
{
  
 LgFrMultiPlantHelper multiPlantHelper;

 assert(multiPlantHelper.pdfSeparator() == "_ \t\n");
 assert(multiPlantHelper.defaultPdf() == "WW");

 std::string testPart1("POK_xx123");
 std::string testPart2("FUJ_xx123");
 std::string testPart3("xx123");
 std::string testPart4("WW_yy123");

 assert(multiPlantHelper.partname(testPart1) == 
	multiPlantHelper.partname(testPart2));
 assert(multiPlantHelper.isPartnameEqual(testPart1, testPart2));
 assert(multiPlantHelper.isPartnameEqual(testPart1, testPart3));
 assert(multiPlantHelper.isPartnameEqual(testPart1.c_str(), testPart3));
 assert(multiPlantHelper.isPartnameEqual(testPart1.c_str(), testPart3.c_str()));
 assert(multiPlantHelper.isPartnameEqual(testPart1, testPart3.c_str()));

 assert(multiPlantHelper.pdf(testPart1) == "POK");
 assert(multiPlantHelper.pdf(testPart3) == "WW");
 assert(multiPlantHelper.pdf(testPart3.c_str()) == "WW");

 
 
 }

#endif
