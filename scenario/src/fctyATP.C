#include <iostream.h>
#include <assert.h>
#include <scenario/src/fctyATP.h>
#include <scenario/src/schFcATP.h>
#include <scenario/src/ipATP.h>

LgFrATPFactory::LgFrATPFactory()
{
}

LgFrATPFactory::~LgFrATPFactory()
{
}


LgFrInitialProblemAndParameters*
LgFrATPFactory::createIPP(LgFrScenarioSmartPointer scenario) const
{
  return new LgFrInitialProblemForATP(scenario);
}

LgFrScheduleFactory*
LgFrATPFactory::createScheduleFactory(LgFrScenarioSmartPointer scenario) const
{
  return new LgFrScheduleFactoryForATP(scenario);
}

void
LgFrATPFactory::factoryType() const
{
  cout << "ATP factory" << endl;
}

