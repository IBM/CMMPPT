// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <scenario/src/calendar.h>
#include <scenario/src/scBdTest.h>
#include <scenario/src/fctyTest.h>
#include <scenario/src/testScen.h>
#include <scenario/src/difSFile.h>

LgFrScenarioBuilderForTest::LgFrScenarioBuilderForTest()
: LgFrScenarioBuilder(new LgFrTestFactory)
{
}

LgFrScenarioBuilderForTest::~LgFrScenarioBuilderForTest()
{
}


void
LgFrScenarioBuilderForTest::buildScenario(
    const LgFrDataInterfaceAbstract& dif,
    const LgFrCalendar& calendar)
{
  std::cout << "Creating Scenario For Test" << std::endl;
  scenarioPtr_ = LgFrTestScenarioSmartPointer(new LgFrTestScenario(dif, calendar));
  this->finishBuildingScenario (scenarioPtr_);
}


LgFrTestScenarioSmartPointer
LgFrScenarioBuilderForTest::getScenario()
{
    if (scenarioPtr_.null())  {
      std::cerr << "No scenario pointer here" << std::endl;
      assert(0);
    }
    return scenarioPtr_;
}

void
LgFrScenarioBuilderForTest::factoryType() const
{
  factoryPtr_->factoryType();
}

LgFrInitialProblemAndParameters*
LgFrScenarioBuilderForTest::createIPP(LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createIPP(scenario);
}

LgFrScheduleFactory*
LgFrScenarioBuilderForTest::createScheduleFactory(LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createScheduleFactory(scenario);
}


#ifdef NDEBUG
#undef NDEBUG
#endif

void
LgFrScenarioBuilderForTest::test()
{
  LgFrCalendar calendar;
  LgFrDataInterfaceSpecialPurposeFromFile
      dif("../../../../wit/wit-data/standard/obj1/wit.data");
  LgFrScenarioBuilderForTest factory;
  factory.factoryType();
  factory.buildScenario(dif, calendar);
  LgFrTestScenarioSmartPointer testScenPtr = factory.getScenario();
  assert (!testScenPtr.null());
  
  // Check result
  LgFrLastProblemSolved & testScenLps = testScenPtr->lastProblemSolved();
  int nper = testScenLps.numberOfPeriods();
  assert (nper == 8);

  std::cout << std::endl;
}
