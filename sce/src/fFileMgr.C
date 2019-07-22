\
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <mcl/src/mcl.h>

#include <sce/src/fFileMgr.h>


// =============================
// =============================
//           SCE 6.1
// =============================
// =============================

// SCE5
//             WRITE Stuff First
//  -------------------------------------------------------------------------------------
// **** writeDemandRecord
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void 
LgFrSceFlatFileMgr::writeDemandRecord(
    std::ofstream & theStream,   // output stream (already opened)
    LgFrMultiPlantHelper & mpHelper,
    LgFrCalendar & theCal,
    const std::string & partname,
    const std::string & pdf,
    const std::string & demandName,
    int    vLength,         // lenght of the vector (nPeriods?)
    const float * theVector,      // the vector itself
    bool printIfZero,
    int precision,          // number of digits to the right of the decimal
    int totalSpace)         // total length of the 'formatted' field
{

  std::string leadingString;


  mpHelper.populateLeadingString(partname, pdf, demandName, leadingString);

  writeFloatVecToStream(theCal, theStream, vLength, printIfZero, leadingString, theVector);
}




// SCE 6.1
//  -------------------------------------------------------------------------------------
// **** writeDemandRecordINDP
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void 
LgFrSceFlatFileMgr::writeDemandRecordINDP(
    std::ofstream & theStream,   // output stream (already opened)
    LgFrMultiPlantHelper & mpHelper,
    LgFrCalendar & theCal,
    const std::string & partname,
    const std::string & pdf,
    const std::string & demandName,
    int    vLength,         // lenght of the vector (nPeriods?)
    const float * theVector,      // the vector itself
    bool printIfZero,
    int precision,          // number of digits to the right of the decimal
    int totalSpace)         // total length of the 'formatted' field
{

  std::string leadingString;

  // 6.1 enables muti-attribute and dated 
  //        which requestDate do we pull and write???
  //          if D2O on INDP?
  //          then the original requestDate is part of the multi-attribute Key and the following will get it
  //          else if D2O on INDP is false, the we should just use the commit date ...
  mpHelper.populateLeadingStringINDP(partname, pdf, demandName, leadingString);
  if (useDatedDemands_)
    writeFloatVecToStreamDoubleDated(theCal, theStream, vLength, printIfZero, leadingString, theVector);
  else
    writeFloatVecToStream(theCal, theStream, vLength, printIfZero, leadingString, theVector);

}

// SCE 6.1
//  -------------------------------------------------------------------------------------
// **** writeGpdDemandRecord
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void 
LgFrSceFlatFileMgr::writeGpdDemandRecord(
    std::ofstream & theStream,    // output stream (already opened)
    LgFrMultiPlantHelper & mpHelper,
    LgFrCalendar & theCal,
    const std::string & partname,
    const std::string & demandName,
    int    vLength,               // length of the vector (nPeriods?)
    const float * theVector,      // the vector itself
    bool printIfZero,
    int precision,                // number of digits to the right of the decimal
    int totalSpace)               // total length of the 'formatted' field
{

  std::string leadingString;

  mpHelper.populateLeadingStringGPD(partname, demandName, leadingString);

  writeFloatVecToStream(theCal, theStream, vLength, printIfZero, leadingString, theVector);
}



//  ---------------------------------------------------------------------
//  Utility to print a float vector to an output stream
//  --------------------------------------------------------------------- 
void 
LgFrSceFlatFileMgr::writeCsvFloatVector(
    std::ofstream & theStream,   // output stream (already opened)
    int    vLength,         // lenght of the vector (nPeriods?)
    const float * theVector,      // the vector itself
    int precision,          // number of digits to the right of the decimal
    int totalSpace)         // total length of the 'formatted' field
{
   int t = 0;
   for (t=0; t<vLength; t++) {
     if ((theVector[t] < 0.0001) && (theVector[t] > -0.0001))
       theStream << "," << (float) 0.0;
     else
       theStream << "," << theVector[t];
   }
   theStream << "\n";
}

//  ---------------------------------------------------------------------
//  SCE 6.1 
//  Print a FloatVec to Stream as either Dated or Vectored
//  Use this as a replacement for 	  writeCsvFloatVector();
//  --------------------------------------------------------------------- 
void 
LgFrSceFlatFileMgr::writeFloatVecToStream(
					 const LgFrCalendar & theCal,
					 std::ofstream & theStream,    // output stream (already opened)
					 int    vLength,               // length of the vector (usually nPeriods)
					 const int    printIfZero,     // printRecordIfZero: for vectored will cause a vector of zeros to print
					                               //                    for dated will cause a zero record to write
					 std::string & leadingString,  // leading string 
					 const float * theVector)      // the vector itself
{
  int t;
  if (useDatedDemands_)   {
    t = 0;
    for (t=0; t<vLength; t++) {
      if (printIfZero || ((theVector[t] >  0.0001) ||  (theVector[t] < -0.0001)))
	theStream << leadingString << "," << theCal.yyyymmdd(theCal.periodStart(t)) << "," << theVector[t] << "\n";
    }
    
  }

  // here is the case of  vectored output
  else {
    bool doWePrintThisRecord = (printIfZero ? true : false);
    int t = 0; 
    
    // loop through once to see if there is a non-zero
    if (! doWePrintThisRecord) {
      for (t=0; t<vLength; t++) {
	if ((theVector[t] > 0.0001) || (theVector[t] < -0.0001)) {
	  doWePrintThisRecord = true;
	  break;
	}
      }
    }
     
    if (doWePrintThisRecord) {
      theStream << leadingString;
      for (t=0; t<vLength; t++) {
	float theVal = theVector[t];
	if ((theVal < 0.0001) && (theVal > -0.0001)) {
	  theVal = 0.0;
	}
	theStream << "," << theVal;
      }
      theStream << "\n";
    }
  }
}


//  ---------------------------------------------------------------------
//  Print a FloatVec to Stream as in Dated mode only.   This method prints the commit date twice in order
//  to fit the datamodel for INDP type dmeands when D2O logic is off.   In this case we don't know the request date and fake it
//  by printing the commit date
//  --------------------------------------------------------------------- 
void 
LgFrSceFlatFileMgr::writeFloatVecToStreamDoubleDated(
					 const LgFrCalendar & theCal,
					 std::ofstream & theStream,    // output stream (already opened)
					 int    vLength,               // length of the vector (usually nPeriods)
					 const int    printIfZero,     // printRecordIfZero: for vectored will cause a vector of zeros to print
					                               //                    for dated will cause a zero record to write
					 std::string & leadingString,  // leading string 
					 const float * theVector)      // the vector itself
{
  int t;

  t = 0;
  for (t=0; t<vLength; t++) {
    if (printIfZero || ((theVector[t] >  0.0001) ||  (theVector[t] < -0.0001)))
      theStream << 
	leadingString << "," << 
	theCal.yyyymmdd(theCal.periodStart(t)) << "," << 
	theCal.yyyymmdd(theCal.periodStart(t)) << "," << 
	theVector[t] << "\n";
  }
  

}



// SCE 6.1 
// parse out the demandType (TINP, INDP) 
// used by processDemandFile()
int 
LgFrSceFlatFileMgr::demandType(const std::string & demandName)
{
  
  // multi_attribute_demand
  assert (useMultiAttributeDemand_);

  SCETokenizer next(demandName);
  std::string token;

  int tokCnt=0;
  for (tokCnt=0; tokCnt<demandTypeKeyFieldNum_; tokCnt++) {
    token =  next(multiAttributeDemandSeparator_.c_str());
    
    if (token.empty()) {
      return DEMAND_TYPE_ERROR;
    }
  }
  if (token == "INDP")
    return DEMAND_TYPE_INDP;
  if (token == "TINP")
    return DEMAND_TYPE_TINP;
  
  return DEMAND_TYPE_ERROR;
}




// =============================================
//    Read Stuff Second 
// =============================================


// ----------------------------------------------------------------
// DATED_DEMANDS
// returns -1 if record is rejected (illegal date, illegal floatValue, or floatValue out of range)
// returns 0 if  record is out of range and ignored, 
// returns 1 if  record is in range and positive
//
//  **** pastDueDemands and pastFutureDemands ****
//         for now we assume 
//               ====> past_due_demand are INCLUDED in Period 0  (these are dates that fall before period 0)
//               ====> PAST_FUTURE_DEMANDS are IGNORED   (these are dates that fall beyond the last period)
//         FINISH_ME: create a param to enable different global assumptions on how
//                    the logic of maybe pass in a pastDueDemands param?
int  
LgFrSceFlatFileMgr::readDatedDemandFloatRecord(
					       float & theVal,
					       // int & thePeriod,
					       std::string  & requestDate,
					       const bool Mandatory,
					       const float lowerLimit,
					       const float upperLimit,
					       SCETokenizer & next,
					       const std::string & fileName,   
					       const std::string & dataLine,
					       const long & lineNo,
					       const LgFrMultiPlantHelper & mpHelper,
					       const LgFrCalendar & theCal)
{
  char dataDelim[] = " \",\n"; 
  int nPeriods = theCal.nPeriods();

  double d;
  
  std::string token;
  
  // read the date
  //  if the line is blank then return -1 (invalid datedDemand, reject)
  if ((requestDate = next(dataDelim)).empty())   
    return -1;
  
  // check the date, if it is invalid then return -1 (invalid datedDemand, reject)
  LgFrDatetime theDate(parseDateString(requestDate, theCal));
  if (! theDate.isValid())  {
    (*sceErrFacility_)("InvalidDate",MclArgList() << "start date" << fileName << (int) lineNo << dataLine);
    return -1;
  }
  
  // Now get the float
  token = next(dataDelim);
  if (token.empty())   {
    if (Mandatory)  {
      (*sceErrFacility_)("MissingData",MclArgList() << "float" << fileName << (int) lineNo << dataLine);
    }
    return -1;
  }
  
  // convert the  token into a float
  if (! sce::fromString( token, d))  {
    (*sceErrFacility_)("InvalidDataType",MclArgList() << "float" << fileName << (int) lineNo << dataLine);
  }
  
  theVal = (float) d;
  

  if (theVal < lowerLimit) {
    (*sceErrFacility_)("OutOfRangeFloatWithBound",MclArgList() << fileName << (int) lineNo << dataLine << theVal << "Lower limit" << lowerLimit << "");
  }
  if (theVal > upperLimit) {
    (*sceErrFacility_)("OutOfRangeFloatWithBound",MclArgList() << fileName << (int) lineNo << dataLine << theVal << "Upper limit" << upperLimit << "");
  }
  
  // If you get here then everything is OK
  return 1;
}



// Multi-Attribute Demand Classes.   
//    try this ... can go either way
//    demandName = flatFileManager.nextDemandGeoOrMultiAttributeQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);

//  --------------------------------------------------------------------- 
// This method is a test of the potential for SCE to deal directly with
// used by ==>  
//             processDemandFile
//             process61TopLevelToOptionalFeatureFile
//             processIndependentDemandFile (with ifdef MULTI_ATTRIBUTE_DEMAND_CLASS)
//             
std::string
LgFrSceFlatFileMgr::nextQuotedDemandString(
    SCETokenizer & next,
    int & result,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo,
    const int doYouNeedIt)
{


  result = 0;

  // multi_attribute_demand
  if (useMultiAttributeDemand_) {

    std::string finalToken = "";

    int tokCnt=0;
    for (tokCnt=0; tokCnt<numDemandAttributes_; tokCnt++) {

      // get the first token, including junk like spaces and tabs
      std::string token = next(",\n");
      
      token = sce::extractQuotedPart( token );
      
      if (doYouNeedIt == MANDATORY && token.empty()) {
	(*sceErrFacility_)("MissingData",MclArgList() 
			   << "quoted string in MultiAttributeDemand" 
			   << fileName 
			   << (int) lineNo 
			   << dataLine);
	// this will exit
      }
      
      if ( token.empty())    {
	if (doYouNeedIt == OPTIONAL_WITH_MESSAGE)
	  (*sceErrFacility_)("MissingDataWarning",MclArgList() 
			     << "quoted string in MultiAttributeDemand" 
			     << fileName 
			     << (int) lineNo 
			     << dataLine);
	result = -1;
      }
      
      if (tokCnt > 0)
	finalToken += multiAttributeDemandSeparator_ + token;
      else 
	finalToken += token;
    }
    return finalToken;
  }



  // single attribute demand
  else {
    // get the next token, including junk like quotes, spaces, and tabs
    std::string token = next(",\n");
    
    token = sce::extractQuotedPart( token );
    
    if (doYouNeedIt == MANDATORY && token.empty()) {
      (*sceErrFacility_)("MissingData",MclArgList() 
			 << "quoted string" 
			 << fileName 
			 << (int) lineNo 
			 << dataLine);
      // this will exit
    }
    
    if ( token.empty())    {
      if (doYouNeedIt == OPTIONAL_WITH_MESSAGE)
	(*sceErrFacility_)("MissingDataWarning",MclArgList() 
			   << "quoted string" 
			   << fileName 
			   << (int) lineNo 
			   << dataLine);
      result = -1;
    }
    return token;
  }

}



// reads:
//   part, pdf, 
// used by:
//    processCapacitySupplyFile61()
int  
LgFrSceFlatFileMgr::readPartPdfAndCheckExistence(
    WitRun * const theWitRun,  
    const int errorAction,  
    std::string & pdf_PartName,    // pdf_PartName to be returned
    std::ifstream & inFile,             // An open stream
    const std::string & fileName,  // For error messaging
    std::string & dataLine,
    long & lineNo,
    const LgFrMultiPlantHelper & mpHelper)
{
    std::string pdf;
    std::string mfgPartName;
    int result = 0;

    std::getline( inFile, dataLine );
    lineNo++;
    dataLine = sce::strip( dataLine );
    if ((dataLine[0] == '*') || dataLine.empty())
	return -2;

    SCETokenizer next(dataLine);
    if ( containsIllegalCharacters( dataLine )) {
      (*sceErrFacility_)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
      return 1;
    }

    // get the partname, pdf, and pdf_partname
    mfgPartName = nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);
    pdf = nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);
    pdf_PartName = mpHelper.pdfPartname(mfgPartName, pdf);
    if (! mpHelper.isPartValid(theWitRun, mfgPartName, pdf, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
      return 1;
    return 0;
}


// reads:
//   part, pdf, floatQty, startDate, endDate
// used by:
//    processCycleTimeFile()
//    processCycleTimeFile() the old way with ifdef VARIABLE_PERIODS
//    processYieldFile
int  
LgFrSceFlatFileMgr::readCsvPartFloatDatedRecord(
    WitRun * const theWitRun,   // WIT environment    
    const int nPeriods,         // Number of periods  
    std::string & pdf_PartName,   // pdf_PartName to be returned
    const LgFrCalendar & theCal,
    float & theVal,
    int & start,
    int & end,
    std::ifstream & inFile,             // An open stream
    const std::string & fileName,  // For error messaging
    std::string & dataLine,
    long & lineNo,
    const LgFrMultiPlantHelper & mpHelper)
{

    std::string pdf;
    std::string mfgPartName;
    int result = 0;

    std::getline( inFile, dataLine );
    lineNo++;
    dataLine = sce::strip( dataLine );
    if ((dataLine[0] == '*') || dataLine.empty())
	return -2;
    SCETokenizer next(dataLine);
    if ( containsIllegalCharacters( dataLine )) {
      (*sceErrFacility_)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
    }

    // get the partname, pdf, and pdf_partname
    mfgPartName = nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);
    pdf = nextQuotedString(next, result, fileName, dataLine, lineNo, MANDATORY);
    pdf_PartName = mpHelper.pdfPartname(mfgPartName, pdf);
    
    if (! mpHelper.isPartValid(theWitRun, mfgPartName, pdf, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE))
      return 1;
    
    
    result = nextFloat(theVal, next, fileName, dataLine, lineNo, true, -FLT_MAX);
    
    
    if (result >= 0)
      result = nextStartEndDates(start, end, next, fileName, lineNo, dataLine, theCal);

    // if dates were messed up, then skip the record
    if (result == 1)
	return 1;

    return 0;
}



// =============================
// =============================
//     END new      SCE 6.1
// =============================
// =============================




// ----------------------------------------------------------------
void
LgFrSceFlatFileMgr::setErrFacility(MclFacility * existingFacility)
{
  // in effect, borrowing an existing message facility
  sceErrFacility_ = existingFacility;
}

MclFacility *
LgFrSceFlatFileMgr::getErrFacility()
{
  return sceErrFacility_;
}









// ----------------------------------------------------------------
int  
LgFrSceFlatFileMgr::readCsvOperationFloatDatedRecord(
    WitRun * const theWitRun,   // WIT environment    
    const int nPeriods,         // Number of periods  
    std::string & pdf_OperationName,   // pdf_PartName to be returned
    const LgFrCalendar & theCal,
    float & theVal,
    int & start,
    int & end,
    std::ifstream & inFile,             // An open stream
    const std::string & fileName,  // For error messaging
    std::string & dataLine,
    long & lineNo,
    const LgFrMultiPlantHelper & mpHelper)
{

    std::string pdf;
    std::string operationName;
    int result = 0;

    std::getline( inFile, dataLine );
    lineNo++;

//  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine[0] == '*') || dataLine.empty())
	return -2;

// check if the line contains any illegal characters
    if ( containsIllegalCharacters( dataLine )) {
      (*sceErrFacility_)("IllegalCharacter",MclArgList() << fileName << (int)lineNo <<dataLine);
    }


    SCETokenizer next(dataLine);

// get the pdf operation name
    operationName = nextQuotedString(next, result, fileName, dataLine, 
                                   lineNo, MANDATORY);
    pdf = nextQuotedString(next, result, fileName, dataLine, 
                           lineNo, MANDATORY);
    pdf_OperationName = mpHelper.pdfOperationName(operationName, pdf);
    
    if (! mpHelper.isOperationValid(theWitRun, 
			       pdf_OperationName, 
			       fileName, 
			       dataLine, 
			       lineNo, 
			       OPTIONAL_WITH_MESSAGE))
	return 1;


    result = nextFloat(theVal, next, fileName, dataLine, lineNo, true, -FLT_MAX);


    if (result >= 0)
	nextStartEndDates(start, end, next, fileName, lineNo, dataLine, theCal);

    // if dates were messed up, then skip the record
    if (result == 1)
	return 1;


    return 0;
}




// ----------------------------------------------------------------
int  
LgFrSceFlatFileMgr::readCsvOperationFloatRecord(
    WitRun * const theWitRun,   // WIT environment    
    const int nPeriods,         // Number of periods  
    std::string & pdf_OperationName,   // pdf_PartName to be returned
    const LgFrCalendar & theCal,
    float & theVal,
    std::ifstream & inFile,             // An open stream
    const std::string & fileName,  // For error messaging
    std::string & dataLine,
    long & lineNo,
    const LgFrMultiPlantHelper & mpHelper)
{

    std::string pdf;
    std::string operationName;
    int result = 0;

    std::getline( inFile, dataLine );
    lineNo++;

//  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine[0] == '*') || dataLine.empty())
	return -2;

// check if the line contains any illegal characters
    if ( containsIllegalCharacters( dataLine )) {
      (*sceErrFacility_)("IllegalCharacter",MclArgList() << fileName << (int)lineNo <<dataLine);
    }


    SCETokenizer next(dataLine);

// get the pdf operation name
    operationName = nextQuotedString(next, result, fileName, dataLine, 
                                   lineNo, MANDATORY);
    pdf = nextQuotedString(next, result, fileName, dataLine, 
                           lineNo, MANDATORY);
    pdf_OperationName = mpHelper.pdfOperationName(operationName, pdf);
    
    if (! mpHelper.isOperationValid(theWitRun, 
			       pdf_OperationName, 
			       fileName, 
			       dataLine, 
			       lineNo, 
			       OPTIONAL_WITH_MESSAGE))
	return 1;


    result = nextFloat(theVal, next, fileName, dataLine, lineNo, true, 0.0);



    return 0;
}






// ----------------------------------------------------------------
int  
LgFrSceFlatFileMgr::readCsvPart2FloatsDatedRecord(
    WitRun * const theWitRun,   // WIT environment    
    const int nPeriods,         // Number of periods  
    std::string & pdf_PartName,   // pdf_PartName to be returned
    const LgFrCalendar & theCal,
    float & theVal,
    float & theVal2,
    int & start,
    int & end,
    std::ifstream & inFile,             // An open stream
    const std::string & fileName,  // For error messaging
    std::string & dataLine,
    long & lineNo,
    const LgFrMultiPlantHelper & mpHelper)
{
 
    std::string pdf;
    std::string mfgPartName;
    int result = 0;
    int result2 = 0;
    std::getline( inFile, dataLine );
    lineNo++;



    
//  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine[0] == '*') || dataLine.empty())
	return -2;

    SCETokenizer next(dataLine);

// check if the line contains any illegal characters
    if ( containsIllegalCharacters( dataLine )) {
      (*sceErrFacility_)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
    }
    
// get the pdf partname

    mfgPartName = nextQuotedString(next, result, fileName, dataLine, 
				   lineNo, MANDATORY);
    pdf = nextQuotedString(next, result, fileName, dataLine, 
			   lineNo, MANDATORY);
    pdf_PartName = mpHelper.pdfPartname(mfgPartName, pdf);
    
    
    
    
    if (! mpHelper.isPartValid(theWitRun, 
			       mfgPartName, 
			       pdf,
			       fileName, 
			       dataLine, 
			       lineNo, 
			       OPTIONAL_WITH_MESSAGE))
      return 1;
    
    result = nextFloat(theVal, next, fileName, dataLine, lineNo, true, -FLT_MAX);
    result2 = nextFloat(theVal2, next, fileName, dataLine, lineNo, true, -FLT_MAX);

    if (result >= 0 && result2 >=0)
	nextStartEndDates(start, end, next, fileName, lineNo, dataLine, theCal);

    // if dates were messed up, then skip the record
    if (result == 1 || result2 ==1)
	return 1;


    return 0;
}


// ----------------------------------------------------------------
int  
LgFrSceFlatFileMgr::readCsvOperation2FloatsDatedRecord(
    WitRun * const theWitRun,   // WIT environment    
    const int nPeriods,         // Number of periods  
    std::string & pdf_OperationName,   // pdf_OperationName to be returned
    const LgFrCalendar & theCal,
    float & theVal,
    float & theVal2,
    int & start,
    int & end,
    std::ifstream & inFile,             // An open stream
    const std::string & fileName,  // For error messaging
    std::string & dataLine,
    long & lineNo,
    const LgFrMultiPlantHelper & mpHelper)
{

    std::string pdf;
    std::string operationName;
    int result = 0;
    int result2 = 0;
    std::getline( inFile, dataLine );
    lineNo++;

//  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine[0] == '*') || dataLine.empty())
	return -2;

    SCETokenizer next(dataLine);

// check if the line contains any illegal characters
    if ( containsIllegalCharacters( dataLine )) {
      (*sceErrFacility_)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
    }


// get the pdf operation name
    operationName = nextQuotedString(next, result, fileName, dataLine, 
                                   lineNo, MANDATORY);
    pdf = nextQuotedString(next, result, fileName, dataLine, 
                           lineNo, MANDATORY);
    pdf_OperationName = mpHelper.pdfOperationName(operationName, pdf);



    
    if (! mpHelper.isOperationValid(theWitRun, 
			       pdf_OperationName, 
			       fileName, 
			       dataLine, 
			       lineNo, 
			       OPTIONAL_WITH_MESSAGE))
	return 1;


    result = nextFloat(theVal, next, fileName, dataLine, lineNo, true, -FLT_MAX);
    result2 = nextFloat(theVal2, next, fileName, dataLine, lineNo, true, -FLT_MAX);

    if (result >= 0 && result2 >=0)
	nextStartEndDates(start, end, next, fileName, lineNo, dataLine, theCal);

    // if dates were messed up, then skip the record
    if (result == 1 || result2 ==1)
	return 1;


    return 0;
}


// ----------------------------------------------------------------
int  
LgFrSceFlatFileMgr::readCsvPartFloatVector(
    WitRun * const theWitRun,            // WIT environment    
    const int nPeriods,                  // Number of periods  
    std::string & pdf_PartName,            // pdf_PartName to be returned
    float *  floatVec,                   // floatVector to be returned
    std::ifstream & inFile,                    // An open stream
    const std::string & fileName,          // for messaging
    std::string& dataLine,
    long & lineNo,                       // for messaging
    const LgFrMultiPlantHelper & mpHelper,
    const int doYouNeedIt,               // Use one of MANDATORY, 
                                         // PURELY_OPTIONAL
                                         // or OPTIONAL_WITH_MESSAGE
    const float lowerLimit,              // data lower limits
    const float upperLimit)              // data upper limits
{
  int t = 0;
 int result;

 std::getline( inFile, dataLine );
 lineNo ++;

 dataLine = sce::strip( dataLine );

//  skip blanks or comments *
 if ((dataLine[0] == '*') || dataLine.empty())
     return -2;
     
 SCETokenizer next(dataLine);

// check if the line contains any illegal characters
    if ( containsIllegalCharacters( dataLine )) {
      (*sceErrFacility_)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
    }

 
 // get the partname
 std::string mfgPartName =  nextQuotedString(next, result, fileName, 
					   dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
 if (result < 0)
     return -1;


// get the pdf
 std::string pdf(nextQuotedString(next, result, fileName, dataLine, 
				lineNo, MANDATORY));

 pdf_PartName = mpHelper.pdfPartname(mfgPartName, pdf);
 
// test to see that its a valid partName
 if (! mpHelper.isPartValid( theWitRun, mfgPartName, pdf, fileName, 
			     dataLine, lineNo, doYouNeedIt))
     return 0;

// read the float values
 for (t = 0; t<nPeriods; t++) {
     if (-1 == nextFloat(floatVec[t], next, fileName, dataLine, lineNo,
			 false, lowerLimit, upperLimit))
	 break;
 }
 
 return t;

}


// ----------------------------------------------------------------
int  
LgFrSceFlatFileMgr::readCsvOperationFloatVector(
    WitRun * const theWitRun,            // WIT environment    
    const int nPeriods,                  // Number of periods  
    std::string & pdf_OperationName,       // pdf_OperationName to be returned
    float *  floatVec,                   // floatVector to be returned
    std::ifstream & inFile,                    // An open stream
    const std::string & fileName,          // for messaging
    std::string& dataLine,
    long & lineNo,                       // for messaging
    const LgFrMultiPlantHelper & mpHelper,
    const int doYouNeedIt,               // Use one of MANDATORY, 
                                         // PURELY_OPTIONAL
                                         // or OPTIONAL_WITH_MESSAGE
    const float lowerLimit,              // data lower limits
    const float upperLimit)              // data upper limits
{

 int t = 0;
 int result;

 std::getline( inFile, dataLine );
 lineNo ++;

 dataLine = sce::strip( dataLine );

//  skip blanks or comments *
 if ((dataLine[0] == '*') || dataLine.empty())
     return -2;
     
 SCETokenizer next(dataLine);

// check if the line contains any illegal characters
    if ( containsIllegalCharacters( dataLine )) {
      (*sceErrFacility_)("IllegalCharacter",MclArgList() << fileName << (int)lineNo << dataLine);
    }

 
 // get the operation name
 std::string operationName =  nextQuotedString(next, result, fileName, 
					   dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
 if (result < 0)
     return -1;


// get the pdf
 std::string pdf(nextQuotedString(next, result, fileName, dataLine, 
				lineNo, MANDATORY));

 pdf_OperationName = mpHelper.pdfOperationName(operationName, pdf);

 
// test to see that its a valid partName
 if (! mpHelper.isOperationValid( theWitRun, pdf_OperationName, fileName, 
			     dataLine, lineNo, doYouNeedIt))
     return 0;

// read the float values
 for (t = 0; t<nPeriods; t++) {
     if (-1 == nextFloat(floatVec[t], next, fileName, dataLine, lineNo,
			 false, -FLT_MAX))
	 break;
 }
 
 return t;

}

//  -----------------------------------------------------------------
// This method tokenizes based on four characters delimiters:
// a space, double quote, comma, and end-of-line.
//
int 
LgFrSceFlatFileMgr::nextFloat(
    float & theValue,
    SCETokenizer & next,
    const std::string & fileName,
    const std::string & dataLine,
    const long & lineNo,
    const bool Mandatory,
    const float lowerLimit,
    const float upperLimit)
{
  // the four char delimiters
  char dataDelim[] = " \",\n"; 
  double d;
  
  std::string token = next(dataDelim);
  if (token.empty())   {
    if (Mandatory)  {
      (*sceErrFacility_)("MissingData",MclArgList() << "float" << fileName << (int) lineNo << dataLine);
    }
    return -1;
  }
  
  // convert the  token into a float
  if (! sce::fromString( token, d))  {
    (*sceErrFacility_)("InvalidDataType",MclArgList() << "float" << fileName << (int) lineNo << dataLine);
  }
  
  theValue = (float) d;
  
  // Check to see if value is in range
  if (theValue < lowerLimit) {
    (*sceErrFacility_)("OutOfRangeFloatWithBound",MclArgList() << fileName << (int) lineNo << dataLine << 
		       theValue << "Lower limit" << lowerLimit << "");
  }
  if (theValue > upperLimit) {
    (*sceErrFacility_)("OutOfRangeFloatWithBound",MclArgList() << fileName << (int) lineNo << dataLine 
		       << theValue << "Upper limit" << upperLimit << "");
  }
  
  // If you get here then everything is OK
  return 0;
  
}


//  --------------------------------------------------------------
int 
LgFrSceFlatFileMgr::nextInt(
    int & theValue,
    SCETokenizer & next,
    const std::string & fileName,
    const std::string & dataLine,
    const long & lineNo,
    const bool Mandatory,
    const int lowerLimit,
    const int upperLimit)
{
  // four char delimiters
  char dataDelim[] = " \",\n"; 
  long lng;
  
  std::string token = next(dataDelim);
  if (token.empty())   {
    if (Mandatory)  {
      (*sceErrFacility_)("MissingData",MclArgList() 
			 << "int" 
			 << fileName 
			 << (int) lineNo
			 << dataLine);
    }
    return -1;
  }
  
  // convert the  token into an int
  if (!sce::fromString( token, (token, lng)))  {
    (*sceErrFacility_)("InvalidDataType",MclArgList() 
		       << "integer" 
		       << fileName 
		       << (int) lineNo 
		       << dataLine);
  }
  
  theValue = (int) lng;
  
  // Check to see if value is in range
  if (theValue < lowerLimit) {
    (*sceErrFacility_)("OutOfRangeIntWithBound",MclArgList() 
		       << fileName 
		       << (int) lineNo 
		       << dataLine 
		       << theValue 
		       << "Lower limit" 
		       << lowerLimit 
		       << "");
  }
  if (theValue > upperLimit) {
    (*sceErrFacility_)("OutOfRangeIntWithBound",MclArgList() 
		       << fileName 
		       << (int) lineNo 
		       << dataLine 
		       << theValue 
		       << "Upper limit" 
		       << upperLimit 
		       << "");
  }
  
  // If you get here then everything is OK
  return 0;
  
}




//  ---------------------------------------------------------------------
//  Utility to determine if a floatVec is non-zero
//  --------------------------------------------------------------------- 

bool
LgFrSceFlatFileMgr::isFloatVecNonZero(
    const float * theVector,      // the vector itself
    const int vLength)   // output stream (already opened)
{
  int t = 0; // Pulled out of the for below by RW2STL
  for (t=0; t<vLength; t++) {
    if (theVector[t] < -0.0001 || theVector[t] > 0.0001)
      return true;
  }
   return false;
}




//  -------------------------------------------------------------

// nextStartEndDates: 
//    return codes:
//        1 if record is rejected
//       -1 if record not populated and OK  (defaults are used)
//        0 if record populated and OK
// ---------------------------------------------------------
int 
LgFrSceFlatFileMgr::nextStartEndDates(
    int & start,
    int & end,
    SCETokenizer & next,
    const std::string & fileName,
    long & lineNo,
    const std::string & dataLine,
    const LgFrCalendar & theCal)
{


    char dataDelim[] = " \",\n"; 

    int nPeriods = theCal.nPeriods();
    std::string token;

    start = 0;
    end = nPeriods - 1;

// read the start and end dates and make sure they're ok

//  if the line is blank then defaults are assumed.
    if ((token = next(dataDelim)).empty())   
	return -1;

    LgFrDatetime startDate(parseDateString(token, theCal));
    if (! startDate.isValid())  { (*sceErrFacility_)("InvalidDate",MclArgList() << "start date" 
						     << fileName << (int) lineNo << dataLine);
      return 1;
    }
    start = theCal.earlyEffectivePeriod(startDate);
    
    if ((token = next(dataDelim)).empty())   
	return -1;

    LgFrDatetime endDate(parseDateString(token, theCal));
    if (! endDate.isValid())  {
      (*sceErrFacility_)("InvalidDate",MclArgList() << "late effectivity date" << fileName 
			 << (int) lineNo << dataLine);
      return 1;
    }
    end = theCal.lateEffectivePeriod(endDate);

    if (startDate > endDate) {
      (*sceErrFacility_)("BadDate",MclArgList() << fileName << (int) lineNo << dataLine);
      return 1;
    }

    if (start > end) {
      (*sceErrFacility_)("CollapsedEffectiveInterval",MclArgList() << fileName << (int) lineNo << dataLine);
      return 1;
    }

    return 0;
}

#if 0

//  -------------------------------------------------------------

// nextStartEndDates: 
//    return codes:
//        1 if record is rejected
//       -1 if record not populated and OK  (defaults are used)
//        0 if record populated and OK
// ---------------------------------------------------------
int 
LgFrSceFlatFileMgr::nextStartEndDates(
    int & start,
    int & end,
    SCETokenizer & next,
    const std::string & fileName,
    long & lineNo,
    const std::string & dataLine,
    const LgFrCalendar & theCal)
{


    char dataDelim[] = " \",\n"; 

    int nPeriods = theCal.nPeriods();
    std::string token;

    start = 0;
    end = nPeriods - 1;

// read the start and end dates and make sure they're ok

//  if the line is blank then defaults are assumed.
    if ((token = next(dataDelim)).empty())   
	return -1;

    LgFrDatetime startDate(parseDateString(token, theCal));
    if (! startDate.isValid())  {
      (*sceErrFacility_)("InvalidDate",MclArgList() 
			 << "start date" 
			 << fileName 
			 << (int) lineNo 
			 << dataLine);
      return 1;
    }
    start = theCal.earlyEffectivePeriod(startDate);
    
    if ((token = next(dataDelim)).empty())   
	return -1;

    LgFrDatetime endDate(parseDateString(token, theCal));
    if (! endDate.isValid())  {
      (*sceErrFacility_)("InvalidDate",MclArgList() 
			 << "late effectivity date" 
			 << fileName 
			 << (int) lineNo 
			 << dataLine);
      return 1;
    }
    end = theCal.lateEffectivePeriod(endDate);

    if (startDate > endDate) {
      (*sceErrFacility_)("BadDate",MclArgList() 
			 << fileName 
			 << (int) lineNo 
			 << dataLine);
      return 1;
    }

    return 0;
}
#endif


//  ---------------------------------------------------------------------
//  Utility to print a float vector to an output stream
//  --------------------------------------------------------------------- 
void 
LgFrSceFlatFileMgr::writeFloatVector(
    std::ofstream & theStream,   // output stream (already opened)
    int    vLength,         // lenght of the vector (nPeriods?)
    float * theVector,      // the vector itself
    int precision,          // number of digits to the right of the decimal
    int totalSpace)         // total length of the 'formatted' field
{
   std::string blankVector(totalSpace, ' ');

   int t = 0; // Pulled out of the for below by RW2STL
   for (t=0; t<vLength; t++) 
   {
      std::string sNum(sce::stringFrom(theVector[t], precision));
      int blankSpace = totalSpace - sNum.length();
      sNum.insert(0, blankVector.c_str(), blankSpace);
      theStream << sNum;
   }
   theStream << "\n";
}

//  ---------------------------------------------------------------------
//  Utility to print a Wit Boolean vector to an output stream
//  --------------------------------------------------------------------- 
void 
LgFrSceFlatFileMgr::writeCsvBoolVector(
    std::ofstream & theStream,   // output stream (already opened)
    int    vLength,         // lenght of the vector (nPeriods?)
    const witBoolean * theVector)      // the vector itself
{
   int t = 0; // Pulled out of the for below by RW2STL
   for (t=0; t<vLength; t++) 
     theStream << "," << theVector[t];

   theStream << "\n";
}



//  -------------------------------------------------------------------------------------
// **** writePartPdfRecord  Vector or Dated_Demands ... Or  multi_attribute_demand_class  ... automatically!!
//  -------------------------------------------------------------------------------------
int
LgFrSceFlatFileMgr::writePartPdfRecord(
    LgFrCalendar & theCal,
    std::string & partname, 
    std::string & pdf, 
    const float *  theVector,
    std::ofstream & theStream,   // output stream (already opened)
    int nPeriods, 
    int printIfZero)
{

  std::string leadingString = "\"" 
    + partname + "\",\"" 
    + pdf + "\"";

  writeFloatVecToStream(theCal, theStream, nPeriods, printIfZero, leadingString, theVector); 

  return 0;
}




//  -------------------------------------------------------------------------------------
// **** writeDbDump Vector or Dated_Demands ... Or  multi_attribute_demand_class  ... automatically!!
//  -------------------------------------------------------------------------------------
int
LgFrSceFlatFileMgr::writeDbDumpRecord(
    LgFrCalendar & theCal,
    std::string & partname, 
    std::string & pdf, 
    const char * attributeKeyword,
    const float *  theVector,
    std::ofstream & theStream,   // output stream (already opened)
    int nPeriods, 
    int printIfZero)
{

  std::string leadingString = "\"" 
    + partname + "\",\"" 
    + pdf + "\",\""
    + attributeKeyword + "\"";


  writeFloatVecToStream(theCal, theStream, nPeriods, printIfZero, leadingString, theVector); 
  return 0;
}






//  -------------------------------------------------------------------------------------
// **** writePegCritlistRecord
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void 
LgFrSceFlatFileMgr::writePegCritListRecord(
    std::ofstream & theStream,   // output stream (already opened)
    LgFrMultiPlantHelper & mpHelper,
    LgFrCalendar & theCal,
    const std::string & partname,
    const std::string & sourceLoc,
    const std::string & demandName,
    const std::string & critpartname,
    const std::string & critpdf,
    int    demandPd, 
    int    critPd, 
    int    theRank)      // the vector itself
{

  std::string leadingString;


  mpHelper.populateLeadingString(partname,
				 sourceLoc,
				 demandName,
				 leadingString);

  leadingString += ",\"" 
    + critpartname + "\",\""
    + critpdf      + "\"";

    
  if (useDatedDemands_)  {
    // convert pds to dates
    std::string demandDate = theCal.yyyymmdd(theCal.periodStart(demandPd));
    std::string critDate = theCal.yyyymmdd(theCal.periodStart(critPd));
    theStream << leadingString << ","  << demandDate << "," << critDate << "," << theRank << "\n";
  }
  else {
    theStream << leadingString << "," << demandPd << "," << critPd << "," << theRank << "\n";
  }
  
  
}




//  -------------------------------------------------------------------------------------
// **** writeGpdPegRecord
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void 
LgFrSceFlatFileMgr::writeGpdPegRecord(LgFrCalendar & theCal,
				      LgFrMultiPlantHelper & mpHelper,
				      const std::string & plannerPart, 
				      const std::string & geo, 
				      const std::string & peggedPart,
				      const std::string & peggedPdf, 
				      const float * theVector,
				      std::ofstream & theStream,   // output stream (already opened)
				      int nPeriods,
				      int printIfZero)
		   
{
  std::string leadingString;

  // start off the leading string with the GPD Keys
  mpHelper.populateLeadingStringGPD(plannerPart, geo, leadingString);

  leadingString += ",\"" 
      + peggedPart + "\",\""
      + peggedPdf + "\"";

  writeFloatVecToStream(theCal, theStream, nPeriods, printIfZero, leadingString, theVector);
}


//  -------------------------------------------------------------------------------------
// **** writeIndpPegRecord
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void 
LgFrSceFlatFileMgr::writeIndpPegRecord(LgFrCalendar & theCal,
				      LgFrMultiPlantHelper & mpHelper,
				      const std::string & demandedPart, 
				      const std::string & demandedPdf, 
				      const std::string & demandName, 
				      const std::string & peggedPart,
				      const std::string & peggedPdf, 
				      const float * theVector,
				      std::ofstream & theStream,   // output stream (already opened)
				      int nPeriods,
				      int printIfZero)
{
  std::string leadingString;

  // Finish_me: Need to close on the design point for D2O logic for INDP demands
  // if D2O on INDP?
  //          then the original requestDate is part of the multi-attribute Key and the following will get it
  // else if D2O on INDP is false, the we should just use the commit date ...

  bool D2O_on_INDP = false;

  mpHelper.populateLeadingStringINDP(demandedPart, demandedPdf, demandName, leadingString);

  if (D2O_on_INDP) {
    int t;
    float pegQty;
    
    t = 0;
    for (t=0; t<nPeriods; t++) {
      pegQty = theVector[t];
      if (printIfZero || (pegQty >  0.0001) ||  (pegQty < -0.0001))    {
	theStream << 
	  leadingString << "," << 
	  theCal.yyyymmdd(theCal.periodStart(t)) << ",\"" << 
	  peggedPart << "\",\"" <<
	  peggedPdf << "\"," <<
	  theCal.yyyymmdd(theCal.periodStart(t)) << "," << 
	  pegQty << "\n";
      }
    }
  }

  else {
    leadingString += ",\"" 
      + peggedPart + "\",\""
      + peggedPdf + "\"";
    writeFloatVecToStream(theCal, theStream, nPeriods, printIfZero, leadingString, theVector);
  }
}


//  -------------------------------------------------------------------------------------
// **** writeIndpInterplantPegRecord
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void 
LgFrSceFlatFileMgr::writeIndpInterplantPegRecord(LgFrCalendar & theCal,
						LgFrMultiPlantHelper & mpHelper,
						 const std::string & demandedPart, 
						 const std::string & demandedPdf, 
						 const std::string & demandName, 
						 const std::string & pn1,
						 const std::string & pdf1, 
						 const std::string & pn2,
						 const std::string & pdf2, 
						 const float * theVector,
						 std::ofstream & theStream, 
						 int nPeriods,
						 int printIfZero)
{
  
  std::string leadingString;
  
  // Finish_me: Need to close on the design point for D2O logic for INDP demands
  // if D2O on INDP?
  //          then the original requestDate is part of the multi-attribute Key and the following will get it
  // else if D2O on INDP is false, the we should just use the commit date ...

  bool D2O_on_INDP = false;

  mpHelper.populateLeadingStringINDP(demandedPart, demandedPdf, demandName, leadingString);
  
  if (D2O_on_INDP) {
    int t;
    float pegQty;

    t = 0;
    for (t=0; t<nPeriods; t++) {
      pegQty = theVector[t];
      if (printIfZero || (pegQty >  0.0001) ||  (pegQty < -0.0001))    {
	theStream << 
	  leadingString << "," << 
	  theCal.yyyymmdd(theCal.periodStart(t)) << ",\"" << 
	  pn1 <<  "\",\"" <<
	  pdf1  << "\",\"" << 
	  pn2 << "\",\"" <<
	  pdf2 << "\"," << 
	  theCal.yyyymmdd(theCal.periodStart(t)) << "," << 
	  pegQty << "\n";
      }
    }
  }
  else {
    leadingString += ",\"" 
      + pn1  + "\",\""
      + pdf1 + "\",\""
      + pn2  + "\",\""
      + pdf2 + "\"";
    
    writeFloatVecToStream(theCal, theStream, nPeriods, printIfZero, leadingString, theVector);
  }
    
}
  
  
  

//  -------------------------------------------------------------------------------------
// **** writeGpdInterplantPegRecord
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void 
LgFrSceFlatFileMgr::writeGpdInterplantPegRecord(LgFrCalendar & theCal,
						LgFrMultiPlantHelper & mpHelper,
						const std::string & plannerPart, 
						const std::string & geo, 
						const std::string & peggedOrderingPart,
						const std::string & peggedOrderingPdf, 
						const std::string & peggedSupplyingPart,
						const std::string & peggedSupplyingPdf, 
						const float * theVector,
						std::ofstream & theStream,   // output stream (already opened)
						int nPeriods,
						int printIfZero)
{

  std::string leadingString;


  mpHelper.populateLeadingStringGPD(plannerPart, geo, leadingString);



  leadingString += ",\"" 
    + peggedOrderingPart  + "\",\"" 
    + peggedOrderingPdf   + "\",\""
    + peggedSupplyingPart + "\",\"" 
    + peggedSupplyingPdf  + "\""; 
  
  writeFloatVecToStream(theCal, theStream, nPeriods, printIfZero, leadingString, theVector);
}






//  --------------------------------------------------------------------- 
// This method tokenizes based on the comma and end-of-line characters.
// It then strips off everything preceeding the first " and everything following
// the last ", and the result is returned.

std::string
LgFrSceFlatFileMgr::nextQuotedString(
    SCETokenizer & next,
    int & result,
    const std::string & fileName,
    const std::string & dataLine,
    long lineNo,
    const int doYouNeedIt)
{

  result = 0;

  // get the first token, including junk like spaces and tabs
  std::string token = next(",\n");
  
  token = sce::extractQuotedPart( token );

  if (doYouNeedIt == MANDATORY && token.empty()) {
    (*sceErrFacility_)("MissingData",MclArgList() << "quoted string" << fileName << (int) lineNo << dataLine);
    // this will exit
  }

  if ( token.empty())    {
    if (doYouNeedIt == OPTIONAL_WITH_MESSAGE)
      (*sceErrFacility_)("MissingDataWarning",MclArgList() << "quoted string" << fileName << (int) lineNo << dataLine);
    result = -1;
  }
    

  return token;
}








// append an given illegal character to the set
void
LgFrSceFlatFileMgr::appendToTheSetOfIllegalCharacters( const std::string newIllegalChar )
{
  setOfIllegalCharacters_.push_back( newIllegalChar );
}

// return the number entries (counting duplicates)
// in the set of illegal characters
int
LgFrSceFlatFileMgr::sizeOfTheSetOfIllegalCharacters ()
{
  return setOfIllegalCharacters_.size();
}


// screen a string for any illegal characters
// 1 = an illegal character was found (contains = true)
// 0 = no illegal characters were found (contains = false)
int
LgFrSceFlatFileMgr::containsIllegalCharacters (const std::string theString)
{
  int nIllegalChars = setOfIllegalCharacters_.size();
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nIllegalChars; i++) {
    // if there's an illegal char embedded in theString, bail out
      if ( theString.find_first_of( setOfIllegalCharacters_[i] ) != std::string::npos)
      return 1;
  }
  // if you make it here, there were no illegal chars embedded in theString
  return 0;

}


// set methods for private member data (don't need gets for these as they can come from SetOfParameters)
void
LgFrSceFlatFileMgr::useDatedDemands(bool useDatedDemands)  {
  useDatedDemands_ = useDatedDemands;
}

void
LgFrSceFlatFileMgr::useMultiAttributeDemand(bool useMultiAttributeDemand)  {
  useMultiAttributeDemand_ = useMultiAttributeDemand;
}

void 
LgFrSceFlatFileMgr::numDemandAttributes(int numDemandAttributes)  {
  numDemandAttributes_ = numDemandAttributes;
}

void 
LgFrSceFlatFileMgr::demandTypeKeyFieldNum(int demandTypeKeyFieldNum)  {
  demandTypeKeyFieldNum_ = demandTypeKeyFieldNum;
}



void 
LgFrSceFlatFileMgr::multiAttributeDemandSeparator(const std::string & multiAttributeDemandSeparator)  {
  multiAttributeDemandSeparator_ = multiAttributeDemandSeparator;
}




void
LgFrSceFlatFileMgr::setGlobalCalendarFormat(const std::string & calendarFormat)
{
    globalCalendarFormat_ = calendarFormat;
    sce::toUpper(globalCalendarFormat_);
}

std::string 
LgFrSceFlatFileMgr::getGlobalCalendarFormat()
{
    return globalCalendarFormat_;
}

LgFrDatetime
LgFrSceFlatFileMgr::parseDateString(
    const std::string & dateString, 
    const LgFrCalendar & theCal)
{
  // date strings come in 3 flavors
  // U = mm/dd/yyyy
  // E = dd/mm/yyyy
  // G = yyyymmdd with no slashes. The default.
  // and can equal PAST or FUTURE
  //

                            
  // Note: this routine will recognize PAST and FUTURE as valid dates
  // regardless of the calendarFormat.
  // NOTE: Sorry for hardcoding these dates, but its not easy to get a 
  // default for PAST and FUTURE otherwise.  In fact, its impossible
  // to get the max legal LgFrDatetime without knowing how its implemented!
  if ( ! sce::compareStringsCaseIns( dateString, "PAST" )) {
    std::string pastDate("19701225");
    return theCal.yyyymmdd(pastDate);
  }

  if ( ! sce::compareStringsCaseIns( dateString, "FUTURE" )) {
    std::string futureDate("20361231");
    return theCal.yyyymmdd(futureDate);
  }

  // Ensure that 
  //    (a) the date is of length 8 (FINISH_ME: multi-byte issues for future??)
  //    (b) the string consists only of numerals
  //    (b) date is no earlier than Jan 1, 1970 (SCE's self imposed lower limit)
  //    (c) date is no later  than Dec 31, 2036 (SCE's self imposed upper limit)
  // If any of these are violated, then we create a KNOWN bad date (our worst 
  // fears are that a bad date somehow turns good in RW's eyes)
  bool allDigits = true;
  for( size_t i = 0; i < dateString.size(); i++ )
      if( !isdigit( dateString[i] ) )
      {
          allDigits = false;
          break;
      }
  if (( dateString.length() != 8 ) 
      || (!allDigits)
      || (dateString < "19700101")) {
    std::string badDate("99999999");
    return theCal.yyyymmdd(badDate);
  }

  // If the date is greater than 20361231, then make it future date (valid)
  if (dateString > "20361231") {
    std::string futureDate("20361231");
    return theCal.yyyymmdd(futureDate);
  }

   
  if (globalCalendarFormat_ != "G")  {
    LgFrDatetime theDate(dateString.c_str());
    return theDate;
  }
    
  return theCal.yyyymmdd(dateString);

}

// method to return the i/o log manager
LgFrSceIOLogMgr&
LgFrSceFlatFileMgr::sceIOLogManager()
{
  return sceIOLogMgr_;
}


// Destructor 
LgFrSceFlatFileMgr::~LgFrSceFlatFileMgr()
{
}


// Assignment operator.
LgFrSceFlatFileMgr&
LgFrSceFlatFileMgr::operator=(
   const LgFrSceFlatFileMgr& rhs)
{
   if (this != &rhs) {           // Check for assignment to self
     setOfIllegalCharacters_ = rhs.setOfIllegalCharacters_;
     useMultiAttributeDemand_ = rhs.useMultiAttributeDemand_;
     numDemandAttributes_ = rhs.numDemandAttributes_;
     demandTypeKeyFieldNum_ = rhs.demandTypeKeyFieldNum_;
     multiAttributeDemandSeparator_ = rhs.multiAttributeDemandSeparator_;
     sceIOLogMgr_ = rhs.sceIOLogMgr_;
     sceErrFacility_ = rhs.sceErrFacility_;
   }

   return *this;
} 


//  copy constructor
LgFrSceFlatFileMgr::LgFrSceFlatFileMgr(
				       const LgFrSceFlatFileMgr& source )
: setOfIllegalCharacters_ (source.setOfIllegalCharacters_),
  globalCalendarFormat_(source.globalCalendarFormat_),
  useMultiAttributeDemand_(source.useMultiAttributeDemand_),
  numDemandAttributes_(source.numDemandAttributes_),
  demandTypeKeyFieldNum_(source.demandTypeKeyFieldNum_),
  multiAttributeDemandSeparator_(source.multiAttributeDemandSeparator_),
  sceIOLogMgr_(source.sceIOLogMgr_),
  sceErrFacility_(source.sceErrFacility_)
{
}

// Default constructor
LgFrSceFlatFileMgr::LgFrSceFlatFileMgr()
: setOfIllegalCharacters_ (0),
  globalCalendarFormat_("G"),
  useMultiAttributeDemand_(0),
  numDemandAttributes_(1),
  demandTypeKeyFieldNum_(1),
  multiAttributeDemandSeparator_("%"),
  sceIOLogMgr_(),
  sceErrFacility_(0)
{
}

#if 0

#ifdef NDEBUG
#undef NDEBUG
#endif

void
LgFrSceFlatFileMgr::test()
{


  

} 
#endif
