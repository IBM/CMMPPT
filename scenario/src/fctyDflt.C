#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>
#include <assert.h>

#include <scenario/src/fctyDflt.h>
#include <scenario/src/inPrFile.h>

LgFrDefaultFactory::LgFrDefaultFactory()
{
}

LgFrDefaultFactory::~LgFrDefaultFactory()
{
}

LgFrInitialProblemAndParameters*
LgFrDefaultFactory::createIPP(LgFrScenarioSmartPointer scenario) const
{
  return new LgFrInitialProblemFromFile("",
                                        scenario);
}


LgFrScheduleFactory*
LgFrDefaultFactory::createScheduleFactory(LgFrScenarioSmartPointer scenario) const
{
  return new LgFrScheduleFactory(scenario);
}


void
LgFrDefaultFactory::factoryType() const
{
    std::cout << "Default factory" << std::endl;
}

