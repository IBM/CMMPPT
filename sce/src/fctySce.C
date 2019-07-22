
// RW2STL - inserted:
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
#include <iostream>
#include <assert.h>

#include <mcl/src/mcl.h>
#include <scenario/src/schFCMRP.h>

#include <sce/src/ipSce.h>
#include <sce/src/fctySce.h>
#include <sce/src/schFSce.h>


LgFrSceFactory::LgFrSceFactory()
{
}

LgFrSceFactory::~LgFrSceFactory()
{
}


LgFrInitialProblemAndParameters*
LgFrSceFactory::createIPP(LgFrScenarioSmartPointer scenario) const
{
  return new LgFrInitialProblemForSce(scenario);
}


LgFrScheduleFactory*
LgFrSceFactory::createScheduleFactory(LgFrScenarioSmartPointer scenario) const
{
  return new LgFrScheduleFactoryForSce(scenario);
}

void
LgFrSceFactory::factoryType() const
{
    std::cout << "Sce factory" << std::endl;
}

