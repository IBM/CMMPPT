// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
// File: LgFrInitialProblemAndParameters.C
// Author: wally
// Created: 17 November 1993 7:20:21 am
// System: WITUSE

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#include <scenario/src/initProb.h>
#include <scenario/src/difAbstr.h>
#include <scenario/src/scenario.h>

// return the scenario ptr
LgFrScenarioSmartPointer
LgFrInitialProblemAndParameters::scenario()
const
{
  return scenarioPtr_;
}


LgFrDataInterfaceAbstract *
LgFrInitialProblemAndParameters::dataInterfacePtr()
     const
{
  return
    (
     (scenarioPtr_.null())
     ?  0
     :  ((LgFrScenarioSmartPointerToConst)scenarioPtr_)->dataInterface()
    );
}


LgFrSetOfParts &
LgFrInitialProblemAndParameters::setOfParts(int k)
{
  assert (!scenarioPtr_.null());
  return scenarioPtr_->setOfParts(k);
}


LgFrSetOfDemands &
LgFrInitialProblemAndParameters::setOfDemands(int k)
{
  assert (!scenarioPtr_.null());
  return scenarioPtr_->setOfDemands(k);
}


// Destructor
LgFrInitialProblemAndParameters::~LgFrInitialProblemAndParameters()
{
  // Nothing to do
}

// constructor
LgFrInitialProblemAndParameters::LgFrInitialProblemAndParameters(
    LgFrScenarioSmartPointer scenarioPtr)
:
   scenarioPtr_(scenarioPtr)
{
  // do nothing
}

// default constructor
LgFrInitialProblemAndParameters::LgFrInitialProblemAndParameters()
:
   scenarioPtr_(0)
{
  // do nothing
}

// copy constructor
LgFrInitialProblemAndParameters::LgFrInitialProblemAndParameters(
                  const LgFrInitialProblemAndParameters& source)
:
scenarioPtr_(source.scenarioPtr_)
{
}

// assignment operator
LgFrInitialProblemAndParameters&
LgFrInitialProblemAndParameters::operator=(
                   const LgFrInitialProblemAndParameters& rhs)
{
  scenarioPtr_ = rhs.scenarioPtr_;
  return *this;
}

