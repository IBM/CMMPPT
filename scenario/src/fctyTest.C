#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>
#include <assert.h>

#include <scenario/src/fctyTest.h>
#include <scenario/src/inPrFile.h>


LgFrTestFactory::LgFrTestFactory()
{
}

LgFrTestFactory::~LgFrTestFactory()
{
}


LgFrInitialProblemAndParameters*
LgFrTestFactory::createIPP(LgFrScenarioSmartPointer scenario) const
{
  return new LgFrInitialProblemFromFile("", scenario);
}


LgFrScheduleFactory*
LgFrTestFactory::createScheduleFactory(LgFrScenarioSmartPointer scenario) const
{
  return new LgFrScheduleFactory(scenario);
}


void
LgFrTestFactory::factoryType() const
{
    std::cout << "Test factory" << std::endl;
}

