//#include <mcl/src/mdsPragma.h>
// RW2STL - inserted:
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
#include <mcl/src/mcl.h>
#include <scenario/src/calendar.h>
#include <scenario/src/dif.h>

#include <sce/src/fctySce.h>
#include <sce/src/scBdSce.h>
#include <sce/src/scenSce.h>
#include <sce/src/difSceF.h>

LgFrScenarioBuilderForSce::LgFrScenarioBuilderForSce()
: LgFrScenarioBuilder(new LgFrSceFactory)
{
}

LgFrScenarioBuilderForSce::~LgFrScenarioBuilderForSce()
{
}


void
LgFrScenarioBuilderForSce::buildScenario(const LgFrDataInterfaceAbstract& dif,
					 const LgFrCalendar& calendar)
{
  scenarioPtr_ = 
     LgFrScenarioForSceSmartPointer(
                                   new LgFrScenarioForSce(dif, calendar)
                                   );
  // initializes the error msg facility -- pass the name of the output file
  scenarioPtr_->initErrFacility("sceERROR.log");
  scenarioPtr_->initMsgFacility("sceMESSAGE.log");

  // this is to share the scenario's error facility with other classes
  //  that need to be able to create error messages
  scenarioPtr_->flatFileMgr().setErrFacility(scenarioPtr_->getErrFacility());
  scenarioPtr_->multiPlantHelper().setErrFacility(scenarioPtr_->getErrFacility());
  scenarioPtr_->setOfParameters().setErrFacility(scenarioPtr_->getErrFacility());

  LgFrSceScrubber * myScrubber = scenarioPtr_ -> scrubber();
  LgFrDataInterfaceImplementation * difSceFImpPtr =
    ((LgFrDataInterface*)&dif) -> dataInterfaceImplementation();
  (*myScrubber) = ((LgFrDataInterfaceImplementationFromSceFiles*)
                   difSceFImpPtr) -> scrubber();
  
  this->finishBuildingScenario(scenarioPtr_);
}

LgFrScenarioForSceSmartPointer
LgFrScenarioBuilderForSce::getScenario()
{
    if (scenarioPtr_.null())  {
      std::cerr << "No scenario pointer here" << std::endl;
      assert(0);
    }
    return scenarioPtr_;
}

void
LgFrScenarioBuilderForSce::factoryType() const
{
  factoryPtr_->factoryType();
}

LgFrInitialProblemAndParameters*
LgFrScenarioBuilderForSce::createIPP(LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createIPP(scenario);
}

LgFrScheduleFactory*
LgFrScenarioBuilderForSce::createScheduleFactory(LgFrScenarioSmartPointer scenario) const
{
  return factoryPtr_->createScheduleFactory(scenario);
}


#if 0
#ifdef NDEBUG
#undef NDEBUG
#endif

void
LgFrScenarioBuilderForSce::test()
{
  LgFrCalendar calendar;
  LgFrTestDataInterface dif;
  LgFrScenarioBuilderForATP factory;
  factory.factoryType();
  factory.buildScenario(dif, calendar);
  
  delete factory.getScenario();
  cout << std::endl;
}
#endif
