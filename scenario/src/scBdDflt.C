// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <scenario/src/calendar.h>
#include <scenario/src/scBdDflt.h>
#include <scenario/src/fctyDflt.h>
#include <scenario/src/scenario.h>
#include <scenario/src/difSFile.h>



void
LgFrScenarioBuilderForDefault::buildScenario(const LgFrDataInterfaceAbstract& dif,
					  const LgFrCalendar& calendar)
{
  std::cout << "Creating Scenario" << std::endl;
  scenarioPtr_ = LgFrScenarioSmartPointer(new LgFrScenario(dif, calendar));
  this->finishBuildingScenario (scenarioPtr_);
}


LgFrScenarioSmartPointer
LgFrScenarioBuilderForDefault::getScenario()
{
    if (scenarioPtr_.null())  {
      std::cerr << "No scenario pointer here" << std::endl;
      assert(0);
    }
    return scenarioPtr_;
}

void
LgFrScenarioBuilderForDefault::factoryType() const
{
  factoryPtr_->factoryType();
}

LgFrInitialProblemAndParameters*
LgFrScenarioBuilderForDefault::createIPP(LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createIPP(scenario);
}

LgFrScheduleFactory*
LgFrScenarioBuilderForDefault::createScheduleFactory(LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createScheduleFactory(scenario);
}


// Constructor
LgFrScenarioBuilderForDefault::LgFrScenarioBuilderForDefault()
: LgFrScenarioBuilder(new LgFrDefaultFactory)
{
  // Nothing to do here
}

// Destructor
LgFrScenarioBuilderForDefault::~LgFrScenarioBuilderForDefault()
{
}


#ifdef NDEBUG
#undef NDEBUG
#endif

void
LgFrScenarioBuilderForDefault::test()
{
  LgFrCalendar calendar;
  LgFrDataInterfaceSpecialPurposeFromFile
    dif ("../../../../wit/wit-data/brenda/denise/wit.data");
  LgFrScenarioBuilderForDefault factory;
  factory.factoryType();
  factory.buildScenario(dif, calendar);
  LgFrScenarioSmartPointer sp = factory.getScenario();
  std::cout << std::endl;
}
