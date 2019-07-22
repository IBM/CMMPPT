// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#include <iostream>

#include <scenario/src/partSS.h>
#include <scenario/src/sOfParts.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/machdep.h>


//estimatedSize
size_t
LgFrPartScheduleStrategy::estimatedSize(
   const LgFrSortingScheduleFloat& schedule )
const
{
  size_t retVal = setOfParts().size();

  if (  schedule.filterStrategy().GetFSType() != __LGFRFILTERSTRATEGY )
     retVal = retVal / 2 + 1;

  retVal = size_tMax(
                    retVal,
                    LgFrAttributeScheduleStrategy::estimatedSize( schedule )
                    );

  return retVal;
}


// Get SetOfParts
const LgFrSetOfParts &
LgFrPartScheduleStrategy::setOfParts()
const
{
   assert (sopPtr_ != 0);
   return *sopPtr_;
}

// Set the  Set Of Parts
void
LgFrPartScheduleStrategy::setOfParts(const LgFrSetOfParts & theSOP)
{
 sopPtr_ = &theSOP;
}

// Assignment operator
LgFrPartScheduleStrategy &
LgFrPartScheduleStrategy::operator=(
			  const LgFrPartScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrAttributeScheduleStrategy::operator= ) (rhs);
    sopPtr_ = rhs.sopPtr_;
  }
  return *this;
}


// Copy constructor
LgFrPartScheduleStrategy::
   LgFrPartScheduleStrategy (
      const LgFrPartScheduleStrategy & source)
:  LgFrAttributeScheduleStrategy( source ),
   sopPtr_( source.sopPtr_ )
{
  // Nothing to do here
}

// Copy constructor associated with a newScenario
LgFrPartScheduleStrategy::
   LgFrPartScheduleStrategy (
      const LgFrPartScheduleStrategy & source,
      LgFrScenario & newScenario )
:  LgFrAttributeScheduleStrategy( source, newScenario ),
   sopPtr_( &(this->LgFrAttributeScheduleStrategy::setOfParts(newScenario)) )
{
  // Nothing to do here
}

// Constructor
LgFrPartScheduleStrategy::
   LgFrPartScheduleStrategy (
     const LgFrSetOfParts & sop,
     LgFrAlgorithm * algorithmPtr )
:  LgFrAttributeScheduleStrategy( algorithmPtr ),
   sopPtr_( &sop )
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrPartScheduleStrategy::
   LgFrPartScheduleStrategy ()
:  LgFrAttributeScheduleStrategy(),
   sopPtr_( 0 )
{
  // Nothing to do here
}

// Destructor
LgFrPartScheduleStrategy::
   ~LgFrPartScheduleStrategy ()
{
   // Nothing to do here
}
