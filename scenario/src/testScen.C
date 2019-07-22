// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
// Used for testing
// This class is a subclass of Scenario and allows LgFr*::test()
// methods access to the scenarios lastProblemSolved, 
// setOfParts and setOfDemands.

#include <stdlib.h>

#include <scenario/src/testScen.h>
#include <scenario/src/scenario.h>
#include <scenario/src/calendar.h>
#include <scenario/src/difSFile.h>
#include <scenario/src/scBdTest.h>
#include <scenario/src/scenDrct.h>

// Return the setOfParts which belongs to this scenario
LgFrSetOfParts&
LgFrTestScenario::setOfParts(int k)
{
   return LgFrScenario::setOfParts(k);
}

// Return the setOfDemands which belongs to this scenario
LgFrSetOfDemands&
LgFrTestScenario::setOfDemands(int k)
{
   return LgFrScenario::setOfDemands(k);
}

// Return the LastProblemSolved which belongs to this scenario
LgFrLastProblemSolved&
LgFrTestScenario::lastProblemSolved()
{
   return LgFrScenario::lastProblemSolved();
}

const LgFrLastProblemSolved&
LgFrTestScenario::lastProblemSolved()
  const
{
   return LgFrScenario::lastProblemSolved();
}

// Assignment operator.
LgFrTestScenario&
LgFrTestScenario::operator=(
   const LgFrTestScenario& rhs)
{
    std::cerr << "should not use LgFrTestScenario::operator=(const LgFrTestScenario&)" << std::endl;
    assert(0);
    return *this; 
}


// Constructor
LgFrTestScenario::LgFrTestScenario (const LgFrDataInterfaceAbstract& dif,
                                    const LgFrCalendar& cal)

: LgFrScenario(dif, cal), id_(__LGFRTESTSCENARIO)
{
  // Nothing to do here
}

// Copy Constructor
LgFrTestScenario::LgFrTestScenario (const LgFrTestScenario& rhs)
    : LgFrScenario(rhs)
{
    // Nothing to do here
}

// Destructor 
LgFrTestScenario::~LgFrTestScenario()
{
  // Nothing to do here
}


// Default constructor
LgFrTestScenario::LgFrTestScenario()
: LgFrScenario(), id_(__LGFRTESTSCENARIO)
{
  // Nothing to do here
}


#ifdef NDEBUG
#undef NDEBUG
#endif
void
LgFrTestScenario::test()
{
  // Make objects needed by the tests
  LgFrDataInterfaceSpecialPurposeFromFile
    simpleProb ("../../../../wit/wit-data/wally/simple/wit.data");
  LgFrCalendar calInst;

  // Create an instance using the builder
  LgFrScenarioBuilderForTest builder;
  LgFrScenarioDirector director(&builder);
  director.construct(simpleProb, calInst);
  LgFrTestScenarioSmartPointer testScenarioPtr = builder.getScenario();

  // Test the constructors and setOfParts() and setOfDemands()
  LgFrTestScenarioSmartPointer nullScenario(new LgFrTestScenario);
  LgFrTestScenarioSmartPointer simpleScenarioDup = testScenarioPtr.deepCopy();
  LgFrTestScenarioSmartPointer assignee(new LgFrTestScenario);
  assert(testScenarioPtr->setOfParts().size() == 7);
  assert(testScenarioPtr->setOfDemands().size() == 3);
  assert(simpleScenarioDup->setOfParts().size() == 7);
  assert(simpleScenarioDup->setOfDemands().size() == 3);
  assert(nullScenario->title() != assignee->title());
  assert(nullScenario->title() != testScenarioPtr->title());
  assert(nullScenario->title() != simpleScenarioDup->title());
  assert(testScenarioPtr->title() != simpleScenarioDup->title());

  // Test the assignment operator - shallow copy
  assignee = testScenarioPtr;
  assert(assignee->setOfParts().size() == 7);
  assert(assignee->setOfDemands().size() == 3);
  assert(testScenarioPtr->title() == assignee->title());

  // Test lastProblemSolved()
  assert (      assignee->lastProblemSolved().numberOfPeriods() ==
          testScenarioPtr->lastProblemSolved().numberOfPeriods()   ); 

  // Test workingImplosionProblem()
  LgFrDemand lhsDemand = (       assignee->setOfDemands() )[2];
  LgFrDemand rhsDemand = ( testScenarioPtr->setOfDemands() )[2];
  assert ( lhsDemand.name() == rhsDemand.name() );
  assert (      assignee->workingImplosionProblem().timeVec( lhsDemand ) ==
          testScenarioPtr->workingImplosionProblem().timeVec( rhsDemand ) ); 

}
