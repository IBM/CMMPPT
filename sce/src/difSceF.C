//#include <mcl/src/mdsPragma.h>

#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>

#include <math.h>
#include <iostream>




#include <scenario/src/scenario.h>
#include <scenario/src/idnumber.h>
#include <scenario/src/calendar.h>
#include <scenario/src/collflt.h>
#include <scenario/src/difEssSc.h>

#include <sce/src/difSceF.h>


LgFrDataInterfaceImplementation *
createSceFilesDifImp(const std::string filename)
{
   return new LgFrDataInterfaceImplementationFromSceFiles(filename);
}

//----------------------------------------------------------------------------
// To register all global functions for this class
// The following functions are registered by using difWitR globle function:
// unitCost, procCost, obj1ScrapCost,obj1StockCost, supplyVol, yield,
// cycleTime, obj1ProdCost, obj1ProdCos, grossRev, demandVol,
// obj1CumShipReward, obj1ShipReward
// Register difSceF's own global functions for raw parts: constrained and procL
//----------------------------------------------------------------------------
void
registerSceFilesDifImpFunctions(LgFrDataInterfaceImplementation *dImpSCE)
{
  ;

}



//-------------------------------------------------------------------
//
//   Functions not defined in parent class
//
//-------------------------------------------------------------------

std::string
LgFrDataInterfaceImplementationFromSceFiles::filename()
const
{
  return filename_;
}

LgFrSceScrubber 
LgFrDataInterfaceImplementationFromSceFiles::scrubber()
const
{
  return myScrubber_;
}
//-------------------------------------------------------------------
//
//   Functions to obtain global attributes
//
//-------------------------------------------------------------------




// Return type identifier
isAReturnType
LgFrDataInterfaceImplementationFromSceFiles::isA()
const
{
  return __LGFRDATAINTERFACEIMPLEMENTATIONFROMSCEFILES;
}



// Make clone copy of object
LgFrDataInterfaceImplementation *
LgFrDataInterfaceImplementationFromSceFiles::clone()
const
{
  LgFrDataInterfaceImplementationFromSceFiles* newInst =
      new LgFrDataInterfaceImplementationFromSceFiles(*this);
  return (newInst);
}

// Destructor
LgFrDataInterfaceImplementationFromSceFiles::
~LgFrDataInterfaceImplementationFromSceFiles()
{
  // nothing to do here
}

// Assignment operator.
LgFrDataInterfaceImplementationFromSceFiles&
LgFrDataInterfaceImplementationFromSceFiles::operator=(
  const LgFrDataInterfaceImplementationFromSceFiles& rhs)
{
  if (this != &rhs) {           // Check for assignment to self
    //     (this->LgFrDataInterfaceImplementation::operator= ) (rhs);
    LgFrDataInterfaceImplementation::operator=  (rhs);
     
     filename_ = rhs.filename_;
  }
  return *this;
}

// Copy constructor
LgFrDataInterfaceImplementationFromSceFiles::
LgFrDataInterfaceImplementationFromSceFiles(
  const LgFrDataInterfaceImplementationFromSceFiles& source)
:
   LgFrDataInterfaceImplementation(source),
   filename_(source.filename_),
   myScrubber_(source.myScrubber_)
{
  // nothing to do here
}


// Preferred Constructor
LgFrDataInterfaceImplementationFromSceFiles::
LgFrDataInterfaceImplementationFromSceFiles(
   std::string filename )
:
   LgFrDataInterfaceImplementation(),
   filename_(filename),
   myScrubber_()
{
  // nothing to do here
}

LgFrDataInterfaceImplementationFromSceFiles::
LgFrDataInterfaceImplementationFromSceFiles(
   std::string filename, std::string scrubDir,bool scrubMode,bool scrubOnly )
:
   LgFrDataInterfaceImplementation(),
   filename_(filename),
   myScrubber_(LgFrSceScrubber(scrubDir, scrubMode, scrubOnly))
{
  // nothing to do here
}

// Default Constructor
// This is a private function
LgFrDataInterfaceImplementationFromSceFiles::
LgFrDataInterfaceImplementationFromSceFiles()
:
   LgFrDataInterfaceImplementation(),
   filename_(),
   myScrubber_()
{
   // Nothing to do here
}     

#if 0


#ifdef NDEBUG
#undef NDEBUG
#endif
// Tests this class.  Only returns if all tests passed
// Tests this class.  Only returns if all tests passed
// Expects data in testScenario1 to have come from
// /u/implode/wit/data/examples/diner12/wit.data, and expects
// testScenario2 to have been made by copying testScenario1
// testSc.
// Right now, the only scenario that works correctly with a 
// LgFrTestDataInterface is LgFrScenarioforCMRP.  Although the above 
// comment says that data should have come from a file, it is probably
// misleading (WCD, 9/15/94).  The data from the file (which has already
// been put into WIT) is probably ignored.  This should be changed
// to take LgFrScenarioforCMRP's for the 1st and 6th arguments.
void
LgFrDataInterfaceFromSceFiles::contextTest
  (
   LgFrDataInterface    	* tdif1,	// testScenario1's
   LgFrLastProblemSolved        & lps,          // testScenario1's
   LgFrDataInterface    	* ts1dupDif,	// testScenario1dup's
   LgFrDataInterface    	* tdif2,	// testScenario2's
   const LgFrScenario           * scenPtr)

{
}


#endif
