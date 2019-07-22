// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <scenario/src/calendar.h>
#include <scenario/src/dif.h>
#include <scenario/src/difCmrpF.h>
#include <scenario/src/difTest.h>
#include <scenario/src/scBdCMRP.h>
#include <scenario/src/fctyCMRP.h>
#include <scenario/src/scenCMRP.h>
#include <scenario/src/schdDeSt.h>

LgFrScenarioBuilderForCMRP::LgFrScenarioBuilderForCMRP()
: LgFrScenarioBuilder(new LgFrCMRPFactory())
{
}

LgFrScenarioBuilderForCMRP::~LgFrScenarioBuilderForCMRP()
{
}


void
LgFrScenarioBuilderForCMRP::buildScenario(const LgFrDataInterfaceAbstract& dif,
					  const LgFrCalendar& calendar)
{
  std::cout << "Creating Scenario For CMRP" << std::endl;
  scenarioPtr_ = LgFrScenarioForCMRPSmartPointer(new LgFrScenarioForCMRP(dif, calendar));
  this->finishBuildingScenario (scenarioPtr_);

  // Instantiate volatile schedules.
  // This is to avoid a problem where the sequence: Read data, implode,
  // explode, financial impact caused the implosion output attributes
  // to be missing from the financial impact multiSchedule.

  LgFrSortingPartScheduleFloatSmartPointer volatileSchedule =
     this->getScenario()->
           scheduleFactory()->newPartSchedule( LGFRProcurementVol );

}

LgFrScenarioForCMRPSmartPointer
LgFrScenarioBuilderForCMRP::getScenario()
{
    if (scenarioPtr_.null())  {
      std::cerr << "No scenario pointer here" << std::endl;
      assert(0);
    }
    return scenarioPtr_;
}

void
LgFrScenarioBuilderForCMRP::factoryType() const
{
  factoryPtr_->factoryType();
}

LgFrInitialProblemAndParameters*
LgFrScenarioBuilderForCMRP::createIPP(LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createIPP(scenario);
}

LgFrScheduleFactory*
LgFrScenarioBuilderForCMRP::createScheduleFactory(
    LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createScheduleFactory(scenario);
}


//****************************************************************************

LgFrScenarioBuilderForCMRPwithPrioritiesFromDif::
  LgFrScenarioBuilderForCMRPwithPrioritiesFromDif()
: LgFrScenarioBuilder(new LgFrCMRPwithPrioritiesFromDifFactory)
{
}

LgFrScenarioBuilderForCMRPwithPrioritiesFromDif::
  ~LgFrScenarioBuilderForCMRPwithPrioritiesFromDif()
{
}


void
LgFrScenarioBuilderForCMRPwithPrioritiesFromDif::
  buildScenario(
      const LgFrDataInterfaceAbstract& dif,
      const LgFrCalendar& calendar)
{
  std::cout << "Creating Scenario For CMRPwithPrioritiesFromDif" << std::endl;
  scenarioPtr_ = LgFrScenarioForCMRPSmartPointer(new LgFrScenarioForCMRP(dif, calendar));
  this->finishBuildingScenario (scenarioPtr_);
}

LgFrScenarioForCMRPSmartPointer
LgFrScenarioBuilderForCMRPwithPrioritiesFromDif::getScenario()
{
    if (scenarioPtr_.null())  {
      std::cerr << "No scenario pointer here" << std::endl;
      assert(0);
    }
  return scenarioPtr_;
}

void
LgFrScenarioBuilderForCMRPwithPrioritiesFromDif::factoryType() const
{
  factoryPtr_->factoryType();
}

LgFrInitialProblemAndParameters*
LgFrScenarioBuilderForCMRPwithPrioritiesFromDif::
  createIPP(LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createIPP(scenario);
}

LgFrScheduleFactory*
LgFrScenarioBuilderForCMRPwithPrioritiesFromDif::
  createScheduleFactory(LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createScheduleFactory(scenario);
}

//****************************************************************************
#ifdef NDEBUG
#undef NDEBUG
#endif

void
LgFrScenarioBuilderForCMRP::test()
{
  LgFrCalendar calendar;
  LgFrDataInterfaceImplementation* difTestImpPtr
      = createDifTestImp();
  difTestImpPtr->title("Test problem for LgFrDataInterfaceImplementation");
  registerDifTestImpFunctions(difTestImpPtr);

  LgFrDataInterface dif(difTestImpPtr);
  delete difTestImpPtr;
  difTestImpPtr = 0;

  LgFrScenarioBuilderForCMRP factory;
  factory.factoryType();
  factory.buildScenario(dif, calendar);
  LgFrScenarioForCMRPSmartPointer b = factory.getScenario();
  std::cout << std::endl;
}


void
LgFrScenarioBuilderForCMRPwithPrioritiesFromDif::test()
{
  LgFrCalendar calendar(3,LgFrDatetime("7/15/94"));
//    LgFrDataInterfaceImplementation * difImpPtr
//	= new LgFrDataInterfaceImplementationFromCmrpFiles(
//                      "../../cmrpData/difTest");
  LgFrDataInterfaceImplementationFromCmrpFiles difImp(
                      CMRP_DATA_PATH "difTest"); 
  registerCmrpFilesDifImpFunctions(&difImp);
  LgFrDataInterface dif(&difImp);
  LgFrScenarioBuilderForCMRPwithPrioritiesFromDif factory;
  factory.factoryType();
  factory.buildScenario(dif, calendar);

  LgFrScenarioForCMRPSmartPointer foo = factory.getScenario();
  std::cout << std::endl;
}

