#include <rw/rstream.h>

#include <wit/src/wit.h>

#include <scenario/src/ipDifT.h>
#include <scenario/src/dif.h>
#include <scenario/src/scenDifT.h>
#include <scenario/src/scDifTP.h>

// Returns a description of the problem that is suitable for use as a
// window heading. This can not be used to set the title; only to query
// it.
RWCString
LgFrInitialProblemForDifTesting::title(
    const LgFrDataInterfaceEssentials * difEssenPtr)
     const
{
  RWCString theTitle("Constrained MRP: ");
  theTitle.append(difTestingScenario()->dataInterface()->title(difEssenPtr));
  return theTitle;
}

// get the scenario
LgFrScenarioForDifTestingSmartPointer
LgFrInitialProblemForDifTesting::difTestingScenario()
     const
{
  return LgFrScenarioForDifTestingSmartPointer(scenario());
}


// Populates the WitRun's data structures 
void
LgFrInitialProblemForDifTesting::feedInto(
				     LgFrLastProblemSolved & )
{
  // Don't invoke dif or populate witRun.  This allows the dif methods
  // to be tested.
}

// Make scenario clone copy of object
LgFrInitialProblemAndParameters *
LgFrInitialProblemForDifTesting::clone(
   LgFrScenarioSmartPointer newScenario)
const
{
  LgFrInitialProblemAndParameters* newInst =
      new LgFrInitialProblemForDifTesting(newScenario);
  return (newInst);
}



LgFrInitialProblemForDifTesting::~LgFrInitialProblemForDifTesting()
{
  // Nothing to do, so do nothing but return
}

// Assignment operator.
LgFrInitialProblemForDifTesting&
LgFrInitialProblemForDifTesting::operator=(
            const LgFrInitialProblemForDifTesting& rhs)
{
  return *new LgFrInitialProblemForDifTesting(rhs.difTestingScenario());
}

// Copy constructor
LgFrInitialProblemForDifTesting::LgFrInitialProblemForDifTesting(
            const LgFrInitialProblemForDifTesting& source)
:
   LgFrInitialProblemAndParameters(source.scenario())
{
  // All the work is done in the initialization part
}

// Default Constructor
LgFrInitialProblemForDifTesting::LgFrInitialProblemForDifTesting()
:
LgFrInitialProblemAndParameters(LgFrScenarioSmartPointer::nullScenarioSmartPointer)
{
  // All the work is done in the initialization part
}

// the preferred Constructor
LgFrInitialProblemForDifTesting::LgFrInitialProblemForDifTesting(LgFrScenarioSmartPointer scenario)
:
    LgFrInitialProblemAndParameters(scenario)
{
  // All the work is done in the initialization part
}
