// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
#include <scenario/src/abFctyAp.h>
#include <scenario/src/dif.h>
#include <scenario/src/scenBld.h>
#include <scenario/src/scenario.h>


void
LgFrScenarioBuilder::finishBuildingScenario (LgFrScenarioSmartPointer scenarioPtr_)
{
  LgFrScheduleFactory * sfp = this->createScheduleFactory (scenarioPtr_); 
  scenariosScheduleFactory  (scenarioPtr_, sfp);

  scenariosInitialProblemAndParameters  (scenarioPtr_,
                                         this->createIPP (scenarioPtr_)  );

  scenariosInitialProblemAndParameters(scenarioPtr_)
    . feedInto(scenariosLastProblemSolved(scenarioPtr_)
              );

  scenariosUniqueName  (scenarioPtr_,
                        scenariosDeriveUniqueName (scenarioPtr_)
                       );

  scenarioPtr_->dataInterface()->deleteExtraResources();
}

// Set
void
LgFrScenarioBuilder::scenariosInitialProblemAndParameters (
    LgFrScenarioSmartPointer sP,
    LgFrInitialProblemAndParameters * ippP)
{
  sP->initialProblemAndParameters(ippP);
}

// Get
LgFrInitialProblemAndParameters&
LgFrScenarioBuilder::scenariosInitialProblemAndParameters (
    LgFrScenarioSmartPointer sP)
{
  return sP->initialProblemAndParameters();
}

// Get
LgFrLastProblemSolved&
LgFrScenarioBuilder::scenariosLastProblemSolved (LgFrScenarioSmartPointer sP)
{
  return sP->lastProblemSolved();
}

// Compute and Get
std::string
LgFrScenarioBuilder::scenariosDeriveUniqueName (LgFrScenarioSmartPointer sP)
{
  return sP->deriveUniqueName();
}

// Set
void
LgFrScenarioBuilder::scenariosScheduleFactory (LgFrScenarioSmartPointer sP,
                                               LgFrScheduleFactory * ippP)
{
  sP->scheduleFactory(ippP);
}

// Set
void
LgFrScenarioBuilder::scenariosUniqueName (LgFrScenarioSmartPointer sP,
                                          std::string ippP)
{
  sP->uniqueName(ippP);
}

LgFrScenarioBuilder::LgFrScenarioBuilder(LgFrApplicationAbstractFactory* factory) : factoryPtr_(factory)
{
}

LgFrScenarioBuilder::~LgFrScenarioBuilder()
{
  delete factoryPtr_;
}
