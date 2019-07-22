#include <scenario/src/calendar.h>
#include <scenario/src/dif.h>
#include <scenario/src/difImpl.h>
#include <scenario/src/difPWatp.h>
#include <scenario/src/scBdATP.h>
#include <scenario/src/fctyATP.h>
#include <scenario/src/scenATP.h>

LgFrScenarioBuilderForATP::LgFrScenarioBuilderForATP()
: LgFrScenarioBuilder(new LgFrATPFactory)
{
}

LgFrScenarioBuilderForATP::~LgFrScenarioBuilderForATP()
{
}


void
LgFrScenarioBuilderForATP::buildScenario(
    const LgFrDataInterfaceAbstract& dif,
    const LgFrCalendar& calendar)
{
  cout << "Creating Scenario For ATP" << endl;
  scenarioPtr_ = LgFrScenarioForATPSmartPointer(new LgFrScenarioForATP(dif, calendar));
  this->finishBuildingScenario (scenarioPtr_);
}

LgFrScenarioForATPSmartPointer
LgFrScenarioBuilderForATP::getScenario()
{
  assert( (!scenarioPtr_.null()) && ("no scenario pointer here") );
  return scenarioPtr_;
}

void
LgFrScenarioBuilderForATP::factoryType() const
{
  factoryPtr_->factoryType();
}

LgFrInitialProblemAndParameters*
LgFrScenarioBuilderForATP::createIPP(LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createIPP(scenario);
}

LgFrScheduleFactory*
LgFrScenarioBuilderForATP::createScheduleFactory(LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createScheduleFactory(scenario);
}


#ifdef NDEBUG
#undef NDEBUG
#endif

void
LgFrScenarioBuilderForATP::test()
{
  LgFrCalendar calendar;
  LgFrDataInterfaceImplementation * difPWImpPtr
      = new LgFrDataInterfaceImplementation("internal data set pw2");
  // Register all functions
  registerPWatpDifImpFunctions(difPWImpPtr);
  LgFrDataInterface dif(difPWImpPtr);

  LgFrScenarioBuilderForATP factory;
  factory.factoryType();
  factory.buildScenario(dif, calendar);
  
  cout << endl;
}
