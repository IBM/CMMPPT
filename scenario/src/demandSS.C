// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#include <iostream>

#include <scenario/src/scenario.h>
#include <scenario/src/algorthm.h>
#include <scenario/src/demandSS.h>
#include <scenario/src/sOfDemds.h>


//estimatedSize
size_t
LgFrDemandScheduleStrategy::estimatedSize(
   const LgFrSortingScheduleFloat& schedule )
const
{
  size_t retVal = setOfDemands().size();

  if (  schedule.filterStrategy().GetFSType() != __LGFRFILTERSTRATEGY )
     retVal = retVal / 2 + 1;

  retVal = size_tMax( 
                    retVal, 
                    LgFrAttributeScheduleStrategy::estimatedSize( schedule ) 
                    );

  return retVal;
}


// Get SetOfDemands
const LgFrSetOfDemands &
LgFrDemandScheduleStrategy::setOfDemands()
const
{
   assert (sodPtr_ != 0);
   return *sodPtr_;
}

// Set the  Set Of Demands
void
LgFrDemandScheduleStrategy::setOfDemands(const LgFrSetOfDemands & theSOD)
{
 sodPtr_ = &theSOD;
}

// Assignment operator
LgFrDemandScheduleStrategy &
LgFrDemandScheduleStrategy::operator=(
			  const LgFrDemandScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrAttributeScheduleStrategy::operator= ) (rhs);
    sodPtr_ = rhs.sodPtr_;
  }
  return *this;
}


// Copy constructor
LgFrDemandScheduleStrategy::
   LgFrDemandScheduleStrategy (
      const LgFrDemandScheduleStrategy & source)
:  LgFrAttributeScheduleStrategy( source ),
   sodPtr_( source.sodPtr_ )
{
  // Nothing to do here
}

// Copy constructor with new Scenario
LgFrDemandScheduleStrategy::
   LgFrDemandScheduleStrategy (
      const LgFrDemandScheduleStrategy & source,
      LgFrScenario & newScenario )
:  LgFrAttributeScheduleStrategy( source, newScenario ),
   sodPtr_( &(this->LgFrAttributeScheduleStrategy::setOfDemands(newScenario)) )
{
  // Nothing to do here
}

// Constructor
LgFrDemandScheduleStrategy::
   LgFrDemandScheduleStrategy (
     const LgFrSetOfDemands & sod,
     LgFrAlgorithm * algorithmPtr )
:  LgFrAttributeScheduleStrategy( algorithmPtr ),
   sodPtr_( &sod )
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrDemandScheduleStrategy::
   LgFrDemandScheduleStrategy ()
:  LgFrAttributeScheduleStrategy(),
   sodPtr_( 0 )
{
  // Nothing to do here
}

// Destructor
LgFrDemandScheduleStrategy::
   ~LgFrDemandScheduleStrategy ()
{
   // Nothing to do here
}
