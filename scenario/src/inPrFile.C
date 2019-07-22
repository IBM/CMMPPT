// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
#include <assert.h>


#include <wit/src/wit.h>
#include <scenario/src/inPrFile.h>
#include <scenario/src/dif.h>
#include <scenario/src/difSFile.h>
#include <scenario/src/lastPrbS.h>
#include <scenario/src/scenario.h>
#include <scenario/src/calendar.h>

// Populates the WitRun's data structures using the data in
// fileName_.
void
LgFrInitialProblemFromFile::feedInto(
				     LgFrLastProblemSolved & lps)
{
  WitRun* const eater = lps.witRun();


  // Put the data into WIT
  if (fileName_.empty()) {
    assert (!(this->scenario().null()));
    LgFrDataInterfaceAbstract* difPtr  =  this->dataInterfacePtr();
    assert (difPtr != 0);
    if (difPtr->GetDIType() != __LGFRDATAINTERFACESPECIALPURPOSEFROMFILE)
     std::cerr << "Warning: LgFrInitialProblemFromFile is downcasting some kind of\n"
          << "LgFrDataInterface (DIF) to an LGFRDATAINTERFACESPECIALPURPOSEFROMFILE (DIFSFile).\n"
          << "The DIF had better be a subclass of DIFFFile" << std::endl;
    LgFrDataInterfaceSpecialPurposeFromFile*
	ddifPtr  =  (LgFrDataInterfaceSpecialPurposeFromFile*) difPtr;
    fileName_ = ddifPtr->fileName();
    ddifPtr->readDataFile (eater);
    ddifPtr->massageWitRun (eater);
  }
  else
    witReadData (eater, fileName_.c_str());

  // fix up calendar if the number of periods in the calendar is not
  //   equal to the number of periods in the data file.  for the case
  //   of loading data directly from a file we don't have a choice in
  //   the number of periods we want to see - we must see ALL of them.
  scenario()->calendar().nPeriods(lps.numberOfPeriods());

  // Initialize Scenario's Universal Set of Parts and Universal Set of Demands
  // (In most cases, LgFrDataInterface does this, but InitialProblemFromFile 
  // doesn't use really use LgFrDataInterface to read the data.
  LgFrSetOfParts   & usop = this->setOfParts (LGFR_UNIVERSAL_SET);
  LgFrSetOfDemands & usod = this->setOfDemands (LGFR_UNIVERSAL_SET);
  this->createPartsAndFillSoP (lps, usop);
  this->createDemandsAndFillSoD (lps, usop, usod);
    
  // Initialize the Default Sets of Parts and Demands
  LgFrSetOfParts   & dsop = this->setOfParts (LGFR_DEFAULT_SET);
  LgFrSetOfDemands & dsod = this->setOfDemands (LGFR_DEFAULT_SET);
  dsop = usop;                  // OK because LgFrSetOfParts and LgFrSetOfDemands 
  dsod = usod;                  // instances check for assignment to themselves

}

// Returns a description of the problem that is suitable for use as a
// window heading. This can not be used to set the title; only to query
// it.
std::string
LgFrInitialProblemFromFile::title(
    const LgFrDataInterfaceEssentials * )
     const
{
  if (fileName_.empty()) return (std::string("(unnamed)"));
  else return (fileName_);
}

// Returns the file name of the wit.data file that the initial parameters
// are coming from.
std::string
LgFrInitialProblemFromFile::fileName()
     const
{
  return (fileName_);
}

// Make scenario clone copy of object
LgFrInitialProblemAndParameters *
LgFrInitialProblemFromFile::clone(
   LgFrScenarioSmartPointer newScenario)
const
{
  LgFrInitialProblemAndParameters* newInst =
      new LgFrInitialProblemFromFile(
             this->fileName(),
             newScenario );
  return (newInst);
}


// Constructor for given filename and scenario
// Create an instance for the file named in the first parameter
// Assign parameter to fileName_ 
LgFrInitialProblemFromFile::LgFrInitialProblemFromFile(
						       const std::string& fileName,
						       LgFrScenarioSmartPointer scenario )
: LgFrInitialProblemAndParameters(scenario), fileName_ (fileName)
{
  // All the work is done in the initialization part
}

LgFrInitialProblemFromFile::~LgFrInitialProblemFromFile()
{
  // Nothing to do, so do nothing but return
}

// Assignment operator.
LgFrInitialProblemFromFile&
LgFrInitialProblemFromFile::operator=(
            const LgFrInitialProblemFromFile& rhs)
{
  fileName_ = rhs.fileName();
  return (*this);
}

// Copy constructor
// Since this uses RWCstring, Rogue Wave can take care of managing the
// string's storage space.
LgFrInitialProblemFromFile::LgFrInitialProblemFromFile(
            const LgFrInitialProblemFromFile& source)
: fileName_ (source.fileName())
{
  // All the work is done in the initialization part
}

// Default Constructor
// Initialize file name to null string.
LgFrInitialProblemFromFile::LgFrInitialProblemFromFile()
: fileName_()
{
  // All the work is done in the initialization part
}


void
LgFrInitialProblemFromFile::createPartsAndFillSoP (LgFrLastProblemSolved & lps,
                                                   LgFrSetOfParts & sop)
{
  // Get the list of parts WIT read in
#if 0  
  int nParts(0);
  char** partList;
  lps.parts (&nParts, &partList);
#else
  LgFrStringVector partList( lps.parts() );
  int nParts( partList.size() );
#endif  

  // Make sop the right size
  //sop.resize(nParts);

  // Fill in sop
  int partNo = 0; // Pulled out of the for below by RW2STL
  for (partNo = 0;  partNo < nParts;  ++partNo) {
    sop.insert(LgFrPart(partList[partNo]));
#if 0    
    portableFree( partList[partNo] );
#endif    
  }
#if 0  
  portableFree (partList);
#endif  
}


void
freeDemandCharPtrPtrList (char** curPartsDemands,
			  const int& n)
{
  int k;			// Number of demand being processed
  for (k = 0;  k < n;  ++k)
    portableFree (curPartsDemands[k]);
  portableFree (curPartsDemands);
}


void
LgFrInitialProblemFromFile::createDemandsAndFillSoD (LgFrLastProblemSolved & lps,
                                                     LgFrSetOfParts & sop,
                                                     LgFrSetOfDemands & sod)
{
  size_t noParts		// Number of parts in this scenario
    = sop.size();
  int noTotalDemands;		// Number of demands in this scenario
  int curPartNo;		// Index number of part being processed
  int noDemandsByCurPart;	// Number of demands by current part
  char** curPartsDemands;	// List of all demands for current part

  // Find out how many demands there are
  noTotalDemands = 0;
  for (curPartNo = 0;  curPartNo < noParts;  ++curPartNo) {
    lps.demands(& (sop[curPartNo]),
		 &noDemandsByCurPart,
		 &curPartsDemands);
    noTotalDemands += (noDemandsByCurPart);
    freeDemandCharPtrPtrList (curPartsDemands, noDemandsByCurPart);
  }

  // Make the demand list the right size
  //sod.resize(noTotalDemands);

  // Create all of the demand objects and put them in sod
  for (curPartNo = 0;  curPartNo < noParts;  ++curPartNo) {
    lps.demands(& (sop[curPartNo]),
		 &noDemandsByCurPart,
		 &curPartsDemands);
    int k = 0; // Pulled out of the for below by RW2STL
    for (k = 0;  k < noDemandsByCurPart;  ++k) {
      LgFrDemand dP (curPartsDemands[k],	// Create demand
		     & (sop[curPartNo]));
      sod.insert(dP);			// Copy into set
    }
    freeDemandCharPtrPtrList (curPartsDemands, noDemandsByCurPart);
  }
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// Test this class
void
LgFrInitialProblemFromFile::test()
{
  char*  testFileName = "wit.data";
  char*  testFile2Name = "wit2.data";

  // Test constructors
  LgFrInitialProblemFromFile defInst;
  LgFrInitialProblemFromFile fileInst (testFileName, 
	  LgFrScenarioSmartPointer::nullScenarioSmartPointer);
  const LgFrInitialProblemFromFile constFileInst (testFileName, 
	  LgFrScenarioSmartPointer::nullScenarioSmartPointer);
  LgFrInitialProblemFromFile copyInst (constFileInst);

  // Test the assignment method.
  defInst = constFileInst;

  // Test the fileName method, also checks to see if the constructors
  // and operator= did reasonably well
  assert (std::string (testFileName) == fileInst.fileName());
  assert (fileInst.fileName() == constFileInst.fileName());
  assert (fileInst.fileName() == copyInst.fileName());
  assert (defInst.fileName() == constFileInst.fileName());  

  // Test the title method
  assert (std::string (testFileName) == fileInst.title());
  assert (fileInst.title() == constFileInst.title());
  assert (fileInst.title() == copyInst.title());
  assert (fileInst.title() == defInst.title());
}
