#include <iostream.h>
#include <scenario/src/mixInATP.h>
#include <scenario/src/idnumber.h>
#include <scenario/src/scenATP.h>
#include <scenario/src/scATPP.h>

// Get the scenario
LgFrScenarioForATPSmartPointer
LgFrScheduleMixInForATP::scenario()
{
  return scenarioPtr_;
}

LgFrScenarioForATPSmartPointerToConst
LgFrScheduleMixInForATP::scenario() const
{
  return scenarioPtr_;
}

// Set the scenario
void
LgFrScheduleMixInForATP::scenario(LgFrScenarioForATPSmartPointerToConst scenarioPtr)
{
  scenarioPtr_ = scenarioPtr;
}

// Get the desired state
int
LgFrScheduleMixInForATP::desiredState() const
{
  return desiredState_;
}

// Set the desired state
void
LgFrScheduleMixInForATP::desiredState(int state)
{
  desiredState_ = state;
}

// Get the current state
int
LgFrScheduleMixInForATP::currentState() const
{
  return scenarioPtr_->currentState();
}

// Send a token discrete finite automata (state machine) controller
void
LgFrScheduleMixInForATP::sendToken(int token)
{
    scenarioPtr_->inputToken (token);
}

// Assignment operator
LgFrScheduleMixInForATP&
LgFrScheduleMixInForATP::operator=(const LgFrScheduleMixInForATP& rhs)
{
  scenarioPtr_ = rhs.scenarioPtr_;
  desiredState_ = rhs.desiredState_;
  return *this;
}

// Copy constructor
LgFrScheduleMixInForATP::LgFrScheduleMixInForATP(const LgFrScheduleMixInForATP& rhs)
{
  scenarioPtr_ = rhs.scenarioPtr_;
  desiredState_ = rhs.desiredState_;
}

// Constructor
LgFrScheduleMixInForATP::LgFrScheduleMixInForATP(LgFrScenarioSmartPointerToConst scenario, int desiredState)
: scenarioPtr_(scenario), desiredState_(desiredState)
{
}

// Default Constructor
LgFrScheduleMixInForATP::LgFrScheduleMixInForATP()
{
}

// Destructor
LgFrScheduleMixInForATP::~LgFrScheduleMixInForATP()
{
}

