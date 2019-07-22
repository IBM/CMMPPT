#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>
#include <assert.h>
#include <scenario/src/fctyCMRP.h>
#include <scenario/src/schFCMRP.h>
#include <scenario/src/ipCMRP.h>

LgFrCMRPFactory::LgFrCMRPFactory()
{
}

LgFrCMRPFactory::~LgFrCMRPFactory()
{
}


LgFrInitialProblemAndParameters*
LgFrCMRPFactory::createIPP(LgFrScenarioSmartPointer scenario) const
{
  return new LgFrInitialProblemForCMRP(scenario);
}


LgFrScheduleFactory*
LgFrCMRPFactory::createScheduleFactory(LgFrScenarioSmartPointer scenario) const
{
  return new LgFrScheduleFactoryForCMRP(scenario);
}

void
LgFrCMRPFactory::factoryType() const
{
  std::cout << "CMRP factory" << std::endl;
}

//****************************************************************************


LgFrCMRPwithPrioritiesFromDifFactory::LgFrCMRPwithPrioritiesFromDifFactory()
{
}

LgFrCMRPwithPrioritiesFromDifFactory::~LgFrCMRPwithPrioritiesFromDifFactory()
{
}


LgFrInitialProblemAndParameters*
LgFrCMRPwithPrioritiesFromDifFactory::createIPP(LgFrScenarioSmartPointer scenario) const
{
  return new LgFrInitialProblemForCMRPwithPrioritiesFromDif(scenario);
}


LgFrScheduleFactory*
LgFrCMRPwithPrioritiesFromDifFactory::createScheduleFactory(
   LgFrScenarioSmartPointer scenario) const
{
  return new LgFrScheduleFactoryForCMRP(scenario);
}

void
LgFrCMRPwithPrioritiesFromDifFactory::factoryType() const
{
  std::cout << "CMRPwithPrioritiesFromDif factory" << std::endl;
}

