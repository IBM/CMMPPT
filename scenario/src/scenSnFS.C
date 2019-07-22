// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#include <assert.h>

#include <scenario/src/scenSnFS.h>
#include <scenario/src/idnumber.h>

// Set the scenario to be used when filtering,
void
LgFrScenarioSensitiveFilterStrategy::scenario(
   const LgFrScenario & scenario)
{
   scenarioPtr_ = &scenario;
}


// Get the scenario pointer to be used when filtering
const LgFrScenario *
LgFrScenarioSensitiveFilterStrategy::scenarioPtr()
  const
{
  return (scenarioPtr_);
}


// Make clone copy of object
LgFrFilterStrategy *
LgFrScenarioSensitiveFilterStrategy::clone()
const
{
  LgFrScenarioSensitiveFilterStrategy* newInst = 
      new LgFrScenarioSensitiveFilterStrategy(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrFilterStrategy *
LgFrScenarioSensitiveFilterStrategy::clone(LgFrScenario & newScenario)
const
{
  LgFrScenarioSensitiveFilterStrategy* newInst = 
      new LgFrScenarioSensitiveFilterStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrScenarioSensitiveFilterStrategy &
LgFrScenarioSensitiveFilterStrategy::operator=(
			  const LgFrScenarioSensitiveFilterStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrFilterStrategy::operator= ) (rhs);
     scenarioPtr_ = rhs.scenarioPtr_;
  }
  return *this;
}


// Copy constructor
LgFrScenarioSensitiveFilterStrategy::
   LgFrScenarioSensitiveFilterStrategy (
      const LgFrScenarioSensitiveFilterStrategy & source)
:  
   LgFrFilterStrategy( source ),
   scenarioPtr_(source.scenarioPtr_),
   id_(__LGFRSCENARIOSENSITIVEFILTERSTRATEGY)
{
}

// Copy constructor with new Scenario
LgFrScenarioSensitiveFilterStrategy::
   LgFrScenarioSensitiveFilterStrategy (
      const LgFrScenarioSensitiveFilterStrategy & source,
      LgFrScenario & newScenario)
:  
   LgFrFilterStrategy( source, newScenario ),
   scenarioPtr_(&newScenario),
   id_(__LGFRSCENARIOSENSITIVEFILTERSTRATEGY)
{
}

// Preferred Constructor
LgFrScenarioSensitiveFilterStrategy::LgFrScenarioSensitiveFilterStrategy(
  const LgFrScenario & scenario)
:  
   LgFrFilterStrategy(), 
   scenarioPtr_(&scenario),
   id_(__LGFRSCENARIOSENSITIVEFILTERSTRATEGY)
{
    // Nothing else to do
}

// Default Constructor
LgFrScenarioSensitiveFilterStrategy::
   LgFrScenarioSensitiveFilterStrategy ()
:  
   LgFrFilterStrategy(),
   scenarioPtr_(0),
   id_(__LGFRSCENARIOSENSITIVEFILTERSTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrScenarioSensitiveFilterStrategy::
   ~LgFrScenarioSensitiveFilterStrategy ()
{
   // Nothing to do here
}


// test LgFrScenarioSensitiveFilterStrategy methods.
void
LgFrScenarioSensitiveFilterStrategy::test()
{

  // Test default constructor
  LgFrScenarioSensitiveFilterStrategy cs1;

  // Test copy constructor
  LgFrScenarioSensitiveFilterStrategy cs2(cs1);

  // Test assignment operator
  LgFrScenarioSensitiveFilterStrategy cs3;
  cs3 = cs2;

  // Test clone method
  LgFrFilterStrategy *cs4Ptr = cs1.clone();

  // Test destructor
  delete cs4Ptr;

}

// test LgFrScenarioSensitiveFilterStrategy methods.
void
LgFrScenarioSensitiveFilterStrategy::contextTest(
   LgFrScenario & scenario1,
   LgFrScenario & scenario2)
{
  LgFrScenarioSensitiveFilterStrategy cs1;

  // Test setting FS's scenario
  cs1.scenario( scenario1);

  // Test copy constructor
  LgFrScenarioSensitiveFilterStrategy cs2(cs1);
  assert( cs2.scenarioPtr_ == cs1.scenarioPtr_ );

  // Test copy constructor with a new scenario
  LgFrScenarioSensitiveFilterStrategy cs2a(cs1, scenario2);
  assert( cs2a.scenarioPtr_ != cs1.scenarioPtr_ );

  // Test assignment operator
  LgFrScenarioSensitiveFilterStrategy cs3;
  LgFrScenarioSensitiveFilterStrategy cs3a (cs1);
  cs3 = cs2;
  assert( cs3.scenarioPtr_ == cs2.scenarioPtr_ );

  // Test clone method
  LgFrFilterStrategy *cs4Ptr = cs1.clone();
  assert( ( (LgFrScenarioSensitiveFilterStrategy *) cs4Ptr)
	  ->scenarioPtr_ == cs1.scenarioPtr_ );
  
  // Cast the clone back down to an instance of this class
  LgFrScenarioSensitiveFilterStrategy *cs5Ptr = 
         (LgFrScenarioSensitiveFilterStrategy *) cs4Ptr; 
  assert( cs5Ptr->scenarioPtr_ == cs1.scenarioPtr_ );

  // Test destructor
  delete cs4Ptr;
}
