#include <scenario/src/calendar.h>
#include <scenario/src/scBdDifT.h>
#include <scenario/src/fctyDifT.h>
#include <scenario/src/scenDifT.h>
#include <scenario/src/scDifTP.h>

void
LgFrScenarioBuilderForDifTesting::buildScenario(const LgFrDataInterfaceAbstract& dif,
					  const LgFrCalendar& calendar)
{
  cout << "Creating Scenario" << endl;
  scenarioPtr_ = LgFrScenarioForDifTestingSmartPointer(new LgFrScenarioForDifTesting(dif, calendar));
  this->finishBuildingScenario (scenarioPtr_);
}


LgFrScenarioForDifTestingSmartPointer
LgFrScenarioBuilderForDifTesting::getScenario()
{
    if (scenarioPtr_.null())  {
      cerr << "No scenario pointer here" << endl;
      assert(0);
    }
    return scenarioPtr_;
}

void
LgFrScenarioBuilderForDifTesting::factoryType() const
{
  factoryPtr_->factoryType();
}

LgFrInitialProblemAndParameters*
LgFrScenarioBuilderForDifTesting::createIPP(LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createIPP(scenario);
}

LgFrScheduleFactory*
LgFrScenarioBuilderForDifTesting::createScheduleFactory(LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createScheduleFactory(scenario);
}


// Constructor
LgFrScenarioBuilderForDifTesting::LgFrScenarioBuilderForDifTesting()
: LgFrScenarioBuilder(new LgFrDifTestingFactory)
{
  // Nothing to do here
}

// Destructor
LgFrScenarioBuilderForDifTesting::~LgFrScenarioBuilderForDifTesting()
{
}
