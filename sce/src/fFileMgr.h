#ifndef LGFRSCEFLATFILEMGR_H
#define LGFRSCEFLATFILEMGR_H

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.


#include <sce/src/sceDefin.h>
#include <stdlib.h>
//#include <iostream.h>
#include <scenario/src/generVc.h>
#include <scenario/src/calendar.h>
#include <wit/src/wit.h>
#include <sce/src/mPlantH.h>
#include <mcl/src/mcl.h>
#include <sce/src/sceIOLog.h>
#include <float.h>
#include <fstream>

// forward declarations

class LgFrSceFlatFileMgr {
  
public:


int 
demandType(const std::string & demandName);


  
void 
writeFloatVecToStream(
		      const LgFrCalendar & theCal,
		      std::ofstream & theStream,    // output stream (already opened)
		      int    vLength,               // length of the vector (usually nPeriods)
		      const int    printIfZero,     // printRecordIfZero: for vectored will cause a vector of zeros to print
                                 	            //                    for dated will cause a zero record to write
		      std::string & leadingString,  // leading string 
		      const float * theVector);     // the vector itself



  
void 
writeFloatVecToStreamDoubleDated(
				 const LgFrCalendar & theCal,
				 std::ofstream & theStream,    // output stream (already opened)
				 int    vLength,               // length of the vector (usually nPeriods)
				 const int    printIfZero,     // printRecordIfZero: for vectored will cause a vector of zeros to print
	                               			 //                    for dated will cause a zero record to write
				 std::string & leadingString,  // leading string 
				 const float * theVector);     // the vector itself
  

// reads:
//   part, pdf, 
// used by:
//    processCapacitySupplyFile61()
int  
readPartPdfAndCheckExistence(
    WitRun * const theWitRun,  
    const int errorAction,  
    std::string & pdf_PartName,    // pdf_PartName to be returned
    std::ifstream & inFile,             // An open stream
    const std::string & fileName,  // For error messaging
    std::string & dataLine,
    long & lineNo,
    const LgFrMultiPlantHelper & mpHelper);

int  
readCsvPartFloatDatedRecord(
           WitRun * const theWitRun,   // WIT environment    
           const int nPeriods,         // Number of periods  
           std::string & pdf_PartName,   // pdf_PartName to be returned
           const LgFrCalendar & theCal,
           float & theVal,
           int & start,
           int & end,
           std::ifstream & inFile,             // An open stream
           const std::string & fileName,  // For error messaging
           std::string& dataLine,
           long & lineNo,
           const LgFrMultiPlantHelper & mpHelper);


int  
readCsvOperationFloatRecord(
			    WitRun * const theWitRun,   // WIT environment    
			    const int nPeriods,         // Number of periods  
			    std::string & pdf_OperationName,   // pdf_PartName to be returned
			    const LgFrCalendar & theCal,
			    float & theVal,
			    std::ifstream & inFile,             // An open stream
			    const std::string & fileName,  // For error messaging
			    std::string & dataLine,
			    long & lineNo,
			    const LgFrMultiPlantHelper & mpHelper);


int  
readCsvOperationFloatDatedRecord(
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
           const LgFrMultiPlantHelper & mpHelper);

int  
readCsvPart2FloatsDatedRecord(
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
           const LgFrMultiPlantHelper & mpHelper);

int  
readCsvOperation2FloatsDatedRecord(
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
         const LgFrMultiPlantHelper & mpHelper);

int  
readCsvPartFloatVector(
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
         const float lowerLimit = 0.0,        // data lower limits
         const float upperLimit = FLT_MAX);   // data upper limits

int  
readCsvOperationFloatVector(
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
         const float lowerLimit = 0.0,        // data lower limits
         const float upperLimit = FLT_MAX);   // data upper limits
  

// Returns --  -1 if data element is missing
//              0 if OK
// If mandatory and something is wrong, then program terminates
// with Nasty messages  
int 
nextInt(
        int & theValue,
        SCETokenizer & next,
        const std::string & fileName,
        const std::string & dataLine,
        const long & lineNo,
        const bool Mandatory,
        const int lowerLimit,
        const int upperLimit);


// Returns --  -1 if data element is missing
//              0 if OK
// If mandatory and something is wrong, then program terminates
// with Nasty messages
int 
nextFloat(
          float & theValue,
          SCETokenizer & next,
          const std::string & fileName,
          const std::string & dataLine,
          const long & lineNo,
          const bool Mandatory = true,
          const float lowerLimit = 0.0,
          const float upperLimit = FLT_MAX);
  

int  
readDatedDemandFloatRecord(
			   float & theVal,
			   std::string & requestDate,
			   const bool Mandatory,
			   const float lowerLimit,
			   const float upperLimit,
			   SCETokenizer & next,
			   const std::string & fileName,   
			   const std::string & dataLine,
			   const long & lineNo,
			   const LgFrMultiPlantHelper & mpHelper,
			   const LgFrCalendar & theCal);

  
bool
isFloatVecNonZero(
                  const float * theVector,    // the vector itself
                  const int vLength) ;  // output stream (already opened)

void 
writeFloatVector(
                 std::ofstream & theStream,   // output stream (already opened)
                 int    vLength,         // lenght of the vector (nPeriods?)
                 float * theVector,      // the vector itself
                 int precision,          // number of digits to
                                         //the right of the decimal
                 int totalSpace);


// returns --- -1 if value is missing (not an error)
//              0 if OK
int
nextStartEndDates(
    int & start,
    int & end,
    SCETokenizer & next,
    const std::string & fileName,
    long & lineNo,
    const std::string & dataLine,
    const LgFrCalendar & theCal);

void
writeCsvBoolVector(
                   std::ofstream & theStream,    // output stream (already opened)
                   int    vLength,          // lenght of the vector (nPeriods?)
                   const witBoolean * theVector); // the vector itself


//  -------------------------------------------------------------------------------------
// **** writePartPdfRecord Vector or Dated_Demands ... Or  multi_attribute_demand_class  ... automatically!!
//  -------------------------------------------------------------------------------------
int
writePartPdfRecord(
    LgFrCalendar & theCal,
    std::string & partname, 
    std::string & pdf, 
    const float *  theVector,
    std::ofstream & theStream,   // output stream (already opened)
    int nPeriods, 
    int printIfZero);


//  -------------------------------------------------------------------------------------
// **** writeDbDump Vector or Dated_Demands ... Or  multi_attribute_demand_class  ... automatically!!
//  -------------------------------------------------------------------------------------
int
writeDbDumpRecord(
    LgFrCalendar & theCal,
    std::string & partname, 
    std::string & pdf, 
    const char * attributeKeyword,
    const float *  theVector,
    std::ofstream & outFile,   // output stream (already opened)
    int nPeriods, 
    int printIfZero);


  // SCE5

//  -------------------------------------------------------------------------------------
// **** writeDemandVector
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void 
writeDemandRecord(
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
    int totalSpace);         // total length of the 'formatted' field


void 
writeGpdDemandRecord(
		     std::ofstream & theStream,    // output stream (already opened)
		     LgFrMultiPlantHelper & mpHelper,
		     LgFrCalendar & theCal,
		     const std::string & partname,
		     const std::string & demandName,
		     int    vLength,               // length of the vector (nPeriods?)
		     const float * theVector,      // the vector itself
		     bool printIfZero,
		     int precision,                // number of digits to the right of the decimal
		     int totalSpace);               // total length of the 'formatted' field
  


  // SCE 6.1: Needed for Demand2Order
void 
writeDemandRecordINDP(std::ofstream & theStream,   // output stream (already opened)
		      LgFrMultiPlantHelper & mpHelper,
		      LgFrCalendar & theCal,
		      const std::string & partname,
		      const std::string & pdf,
		      const std::string & demandName,
		      int    vLength,         // lenght of the vector (nPeriods?)
		      const float * theVector,      // the vector itself
		      bool printIfZero,
		      int precision,          // number of digits to the right of the decimal
		      int totalSpace);         // total length of the 'formatted' field


//  -------------------------------------------------------------------------------------
// **** writePegCritlistRecord
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void 
writePegCritListRecord(
    std::ofstream & theStream,   // output stream (already opened)
    LgFrMultiPlantHelper & mpHelper,
    LgFrCalendar & theCal,
    const std::string & partname,
    const std::string & pdf,
    const std::string & demandName,
    const std::string & critpartname,
    const std::string & critpdf,
    int    demandPd, 
    int    critPd, 
    int    theRank);      // the rank in pegged 


//  -------------------------------------------------------------------------------------
// **** writeGpdPegRecord
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void writeGpdPegRecord(LgFrCalendar & theCal,
		       LgFrMultiPlantHelper & mpHelper,
		       const std::string & plannerPart, 
		       const std::string & geo, 
		       const std::string & peggedPart,
		       const std::string & peggedPdf, 
		       const float * theVector,
		       std::ofstream & theStream,   // output stream (already opened)
		       int nPeriods,
		       int printIfZero);


//  -------------------------------------------------------------------------------------
// **** writeIndpPegRecord
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void writeIndpPegRecord(LgFrCalendar & theCal,
		   LgFrMultiPlantHelper & mpHelper,
		   const std::string & demandedPart, 
		   const std::string & demandedPdf, 
		   const std::string & demandId, 
		   const std::string & peggedPart,
		   const std::string & peggedPdf, 
		   const float * theVector,
		   std::ofstream & theStream,   // output stream (already opened)
		   int nPeriods,
		   int printIfZero);


//  -------------------------------------------------------------------------------------
// **** writeIndpInterplantPegRecord
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void 
writeIndpInterplantPegRecord(LgFrCalendar & theCal,
			     LgFrMultiPlantHelper & mpHelper,
			     const std::string & demandedPart, 
			     const std::string & sourceLoc, 
			     const std::string & demandId, 
			     const std::string & pn1,
			     const std::string & pdf1, 
			     const std::string & pn2,
			     const std::string & pdf2, 
			     const float * theVector,
			     std::ofstream & theStream, 
			     int nPeriods,
			     int printIfZero);
  



//  -------------------------------------------------------------------------------------
// **** writeGpdInterplantPegRecord
// Print to either multi_attribute_demand_class OR Dated_Demands format ... automatically
//  -------------------------------------------------------------------------------------
void 
writeGpdInterplantPegRecord(LgFrCalendar & theCal,
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
			    int printIfZero);




void 
writeCsvFloatVector(
                    std::ofstream & theStream,   // output stream (already opened)
                    int    vLength,         // lenght of the vector (nPeriods?)
                    const float * theVector,      // the vector itself
                    int precision,          // number of digits to the right
                                            // of the decimal
                    int totalSpace);
  
// For PURELY_OPTIONAL and OPTIONAL_WITH_MESSAGE:
//     returns -1 if data is missing
// For MANDATORY:
//     program terminates if data is missing
std::string
nextQuotedString(
                 SCETokenizer & next,
                 int & result,
                 const std::string & fileName,
                 const std::string & dataLine,
                 long lineNo,
                 const int doYouNeedIt);

  // multi_attribute_demand_class
std::string
nextQuotedDemandString(
		       SCETokenizer & next,
		       int & result,
		       const std::string & fileName,
		       const std::string & dataLine,
		       long lineNo,
		       const int doYouNeedIt);
  


// append an given illegal character to the set
void
appendToTheSetOfIllegalCharacters( const std::string newIllegalChar );

// return the number entries (counting duplicates)
// in the set of illegal characters
int
sizeOfTheSetOfIllegalCharacters ();
  
// screen a string for any illegal characters
// 1 = an illegal character was found (contains = true)
// 0 = no illegal characters were found (contains = false)
int
containsIllegalCharacters (const std::string theString);
  
void
setGlobalCalendarFormat(const std::string & calendarFormat);

std::string
getGlobalCalendarFormat();

  // sets for private data
void useDatedDemands(bool useDatedDemands);
void useMultiAttributeDemand(bool useMultiAttributeDemand);
void numDemandAttributes(int numDemandAttributes);
void demandTypeKeyFieldNum(int demandTypeKeyFieldNum);
void multiAttributeDemandSeparator(const std::string & multiAttributeDemandSeparator);


LgFrDatetime
parseDateString(
    const std::string & dateString,
    const LgFrCalendar & theCal);

// return a reference to the i/o log manager
LgFrSceIOLogMgr&
sceIOLogManager();


// These are necessary for borrowing scenario's error facility for error
//  messaging originating from the flat file manager.
void
setErrFacility(MclFacility * existingFacility);

MclFacility *
getErrFacility();



// Assignment operator.
LgFrSceFlatFileMgr&
operator=(
          const LgFrSceFlatFileMgr& rhs);


// Destructor
~LgFrSceFlatFileMgr();


// Copy constructor
LgFrSceFlatFileMgr(
               const LgFrSceFlatFileMgr& source);

// Default Constructor
LgFrSceFlatFileMgr();


// Self-test
static void test();

private:
  //  illegal set of characters
  LgFrStringVector setOfIllegalCharacters_ ;

  // parseDateString relies on this member data.
  // The ip invokes a fFileMgr public method to
  // set this data, based on the value in the setOfParameters
  std::string globalCalendarFormat_;  

  // multi_attribute_demand_class
  bool useDatedDemands_;
  bool useMultiAttributeDemand_;
  int numDemandAttributes_;
  int demandTypeKeyFieldNum_;
  std::string multiAttributeDemandSeparator_;


  // Used for producing i/o log files
  LgFrSceIOLogMgr sceIOLogMgr_;

  // Necessary for error messaging by flat file manager!!
  MclFacility * sceErrFacility_;
};

#endif


