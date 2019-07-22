#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
#include <scenario/src/difEssSc.h>
#include <scenario/src/scenario.h>

LgFrPart
LgFrDataInterfaceEssentialsFromScenario::createAndInsertPart(
    const std::string& partName)
const
{
    LgFrSetOfParts& sop 
	= ((LgFrScenario*)scenarioPtr_)->setOfParts(LGFR_UNIVERSAL_SET);
    return sop.newPart(partName);
}

LgFrDemand
LgFrDataInterfaceEssentialsFromScenario::createAndInsertDemand(
    const std::string& demandName,
    const std::string& partName)
const
{
    LgFrSetOfDemands& sod 
	= ((LgFrScenario*)scenarioPtr_)->setOfDemands(LGFR_UNIVERSAL_SET);
    LgFrSetOfParts& sop 
	= ((LgFrScenario*)scenarioPtr_)->setOfParts(LGFR_UNIVERSAL_SET);
    const LgFrPart* part = sop.findValue(partName);   
    if (part == 0) {
      std::cerr << "Error in " << __FILE__ << " at line " << __LINE__ << '\n';
      std::cerr << "Trying to create a demand for a part that was not in the\n";
      std::cerr << "Universal Set of Parts.\n";
      std::cerr << "The name of the part that could not be found is " 
	   << partName << '\n';
      std::cerr << "The name of the demand being created was " << demandName
	<< std::endl;
      abort();
    }
    return sod.newDemand(demandName, part);
}

const LgFrCalendar&
LgFrDataInterfaceEssentialsFromScenario::calendar()
const
{
    return scenarioPtr_->calendar();
}

// Return a mutable pointer to LastSolvedProblem's WitRun.
WitRun*
LgFrDataInterfaceEssentialsFromScenario::mutableWitRun()
{	
    return ((LgFrScenario*)scenarioPtr_)->lastProblemSolved().witRun();
}

// Make a clone copy in same scenario
LgFrDataInterfaceEssentials*
LgFrDataInterfaceEssentialsFromScenario::clone()
const
{
  return new LgFrDataInterfaceEssentialsFromScenario(*this);
}

// Make a clone copy in new scenario
LgFrDataInterfaceEssentials*
LgFrDataInterfaceEssentialsFromScenario::clone(const LgFrScenario* newScenPtr)
const
{
  return new LgFrDataInterfaceEssentialsFromScenario(*this, newScenPtr);
}

// Copy constructor with same scenario
LgFrDataInterfaceEssentialsFromScenario::LgFrDataInterfaceEssentialsFromScenario(
    const LgFrDataInterfaceEssentialsFromScenario& copy)
    : LgFrDataInterfaceEssentials(copy)
{
  scenarioPtr_ = copy.scenarioPtr_;
}

// Copy constructor with new scenario
LgFrDataInterfaceEssentialsFromScenario::LgFrDataInterfaceEssentialsFromScenario(
    const LgFrDataInterfaceEssentialsFromScenario& copy,
    const LgFrScenario* newScenPtr)
    : LgFrDataInterfaceEssentials(copy)
{
  scenarioPtr_ = newScenPtr;
}


// Default constructor
LgFrDataInterfaceEssentialsFromScenario::LgFrDataInterfaceEssentialsFromScenario()
    : LgFrDataInterfaceEssentials(),
      scenarioPtr_(0)
{
// Nothing to do here

}

// Constructor
LgFrDataInterfaceEssentialsFromScenario::LgFrDataInterfaceEssentialsFromScenario(
    const LgFrScenario* scenPtr)
    : LgFrDataInterfaceEssentials(),
      scenarioPtr_(scenPtr)
{
// Nothing to do here
}

// Destructor
LgFrDataInterfaceEssentialsFromScenario::~LgFrDataInterfaceEssentialsFromScenario()
{
// Nothing to do here
}
