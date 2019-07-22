// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif
#include <scenario/src/difSFile.h>
#include <scenario/src/difImpl.h>
#include <scenario/src/difTest.h>
#include <scenario/src/scenDrct.h>
#include <scenario/src/scenBld.h>
// #include <scenario/src/scBdCMRP.h>
#include <scenario/src/scBdTest.h>
#include <scenario/src/scBdDflt.h>
#include <scenario/src/scenCMRP.h>

LgFrScenarioDirector::LgFrScenarioDirector(LgFrScenarioBuilder* builderPtr)
: builderPtr_(builderPtr)
{
}

LgFrScenarioDirector::~LgFrScenarioDirector()
{
}


void
LgFrScenarioDirector::construct(
    const LgFrDataInterfaceAbstract& dif,
    const LgFrCalendar& calendar)
{
  builderPtr_->buildScenario(dif, calendar);
}

#ifdef NDEBUG
#undef NDEBUG
#endif

void
LgFrScenarioDirector::test()
{
  LgFrCalendar calendar;
  LgFrDataInterfaceSpecialPurposeFromFile
      difFile("../../../../wit/wit-data/examples/diner/wit.data");

  {
    LgFrScenarioBuilderForDefault builder;
    LgFrScenarioDirector director(&builder);
    director.construct(difFile, calendar);
    LgFrScenarioSmartPointer testScenario = builder.getScenario();
  }

  {
    LgFrScenarioBuilderForTest builder;
    LgFrScenarioDirector director(&builder);
    director.construct(difFile, calendar);
    LgFrTestScenarioSmartPointer testScenario = builder.getScenario();
  }

#if 0
  {
    LgFrDataInterfaceImplementation* testDifImpPtr
        = createDifTestImp();
    testDifImpPtr->title("Test problem for LgFrDataInterfaceImplementation");
    registerDifTestImpFunctions(testDifImpPtr);

    LgFrDataInterface dif(testDifImpPtr);
    delete testDifImpPtr;  // free it from heap since dif constr clone's it's own copy
    testDifImpPtr = 0;


    LgFrScenarioBuilderForCMRP builder;
    LgFrScenarioDirector director(&builder);
    director.construct(dif, calendar);
    LgFrScenarioForCMRPSmartPointer testScenario = builder.getScenario();


  }
#endif

  
}









