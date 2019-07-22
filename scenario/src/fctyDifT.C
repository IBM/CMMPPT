#include <iostream.h>
#include <assert.h>
#include <scenario/src/fctyDifT.h>
#include <scenario/src/schFctry.h>
#include <scenario/src/ipDifT.h>

LgFrDifTestingFactory::LgFrDifTestingFactory()
{
}

LgFrDifTestingFactory::~LgFrDifTestingFactory()
{
}

LgFrInitialProblemAndParameters*
LgFrDifTestingFactory::createIPP(LgFrScenarioSmartPointer scenario) const
{
  return new LgFrInitialProblemForDifTesting(scenario);
}


LgFrScheduleFactory*
LgFrDifTestingFactory::createScheduleFactory(LgFrScenarioSmartPointer scenario) const
{
  return new LgFrScheduleFactory(scenario);
}

void
LgFrDifTestingFactory::factoryType() const
{
  cout << "DifTesting factory" << endl;
}
