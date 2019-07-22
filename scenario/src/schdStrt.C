// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <scenario/src/algorthm.h>
#include <scenario/src/schdStrt.h>
#include <scenario/src/scenario.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/filtStrt.h>


// Populate Schedule with itemTimeVecs
// Do generic part of schedule population and invoke
// derived class method to do the class specific portions.
void
LgFrAttributeScheduleStrategy::populate(
   LgFrSortingScheduleFloat& schedule )
const
{
  // Remove any itemTimeVecs from schedule
  schedule.localClear();

  // Resize to estimated size.  This is done for better performance,
  // by avoiding resize calls when inserting itemTimeVecs in kernalOfPopulate.
  schedule.reserve( estimatedSize( schedule ) );

  // Do the inner part of populate, the derived class specific portion
  kernalOfPopulate( schedule );

  // Sort the schedule
  schedule.sort( 
     schedule.compareStrategy(), 
     schedule.isSortAscending() );

}

//estimatedSize
size_t
LgFrAttributeScheduleStrategy::estimatedSize(
   const LgFrSortingScheduleFloat& schedule )
const
{
  size_t retVal = schedule.capacity();

  return retVal;
}

// isMutable
bool
LgFrAttributeScheduleStrategy::isMutable()
const
{
 return false;
}

// validity
// Defaults to NOTKNOWN (which should usually be treated as if VALID)
// (Note that, on OS/2, UNKNOWN is changed to "7" by math.h or related
// functionality, which makes it a bad choice for a type.)
LgFrValidity
LgFrAttributeScheduleStrategy::validity( 
    const LgFrTimeVecFloat& proposedTimeVec)
const
{
  return NOTKNOWN;
}

// Print an error message and go on.  
// If *this is mutable, this method has to be overriden by a method
// in the subclass to propate a change in the sortingSchedule into the
// appropriate parts of the underlying system, e.g. workingImplosionProblem.
void
LgFrAttributeScheduleStrategy::update (
				       const LgFrItemTimeVecPairFloat & /* pair */ )
{
  std::cerr << "Warning: In " << __FILE__ << ", line " << __LINE__ << ",\n"
    << "trying to update an attribute that is not updatable" << std::endl;
}


// Return the setOfParts which belongs to a scenario
LgFrSetOfParts&
LgFrAttributeScheduleStrategy::setOfParts(
   LgFrScenario & scenario)
{
   return scenario.setOfParts();
}

// Return the setOfDemands which belongs to a scenario
LgFrSetOfDemands&
LgFrAttributeScheduleStrategy::setOfDemands(
   LgFrScenario & scenario)
{
   return scenario.setOfDemands();
}

// Return the WorkingImplosionProblem which belongs to a scenario
LgFrWorkingImplosionProblem&
LgFrAttributeScheduleStrategy::workingImplosionProblem(
   LgFrScenario & scenario)
{
   return scenario.workingImplosionProblem();
}

const LgFrWorkingImplosionProblem&
LgFrAttributeScheduleStrategy::workingImplosionProblem(
   const LgFrScenario & scenario)
const
{
   return scenario.workingImplosionProblem();
}

// Return the LastProblemSolved which belongs to a scenario
LgFrLastProblemSolved&
LgFrAttributeScheduleStrategy::lastProblemSolved(
   LgFrScenario & scenario)
{
   return scenario.lastProblemSolved();
}

const LgFrLastProblemSolved&
LgFrAttributeScheduleStrategy::lastProblemSolved(
   const LgFrScenario & scenario)
const
{
   return scenario.lastProblemSolved();
}


// Return the Implode which belongs to a scenario
LgFrImplode&
LgFrAttributeScheduleStrategy::implode(
   LgFrScenario & scenario)
{
   return scenario.implode();
}


// Return the Explode which belongs to a scenario
LgFrExplode&
LgFrAttributeScheduleStrategy::explode(
   LgFrScenario & scenario)
{
   return scenario.explode();
}

// Return the UserInputModification which belongs to a scenario
LgFrUserInputModification&
LgFrAttributeScheduleStrategy::userInputModification(
   LgFrScenario & scenario)
{
   return scenario.userInputModification();
}

// Return the focussed shortage schedule which belongs to a scenario
LgFrFocussedShortageScheduleControl&
LgFrAttributeScheduleStrategy::focussedShortageScheduleControl(
   LgFrScenario & scenario)
{
   return scenario.focussedShortageScheduleControl();
}


// Assignment operator
LgFrAttributeScheduleStrategy &
LgFrAttributeScheduleStrategy::operator=(
			  const LgFrAttributeScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self

     (this->LgFrSubscription::operator= ) (rhs);
     if ( algorithmPtr_ != 0 ) algorithmPtr_->removeDependent( this );
     algorithmPtr_ = rhs.algorithmPtr_;
     if ( algorithmPtr_ != 0 ) algorithmPtr_->addDependent( this );

  }
  return *this;
}


// Copy constructor
LgFrAttributeScheduleStrategy::
   LgFrAttributeScheduleStrategy (
      const LgFrAttributeScheduleStrategy & source)
:  
   LgFrSubscription(),   
   algorithmPtr_( source.algorithmPtr_ )
{
  if ( algorithmPtr_ != 0 ) algorithmPtr_->addDependent( this );
}

// Copy constructor associated with a new Scenario
LgFrAttributeScheduleStrategy::
   LgFrAttributeScheduleStrategy (
      const LgFrAttributeScheduleStrategy & source,
      LgFrScenario &  newScenario )
:  
   LgFrSubscription(),
   algorithmPtr_( 0 )
{
  if ( source.algorithmPtr_ != 0 ) {
    algorithmPtr_ = source.algorithmPtr_->counterPart( newScenario );
    algorithmPtr_->addDependent( this );
  }
}

// Constructor
LgFrAttributeScheduleStrategy::
   LgFrAttributeScheduleStrategy(
      LgFrAlgorithm * algorithmPtr )
:  
   LgFrSubscription(),
   algorithmPtr_(algorithmPtr)
{
   if ( algorithmPtr_ != 0 ) algorithmPtr_->addDependent( this );
}


// Destructor
LgFrAttributeScheduleStrategy::
   ~LgFrAttributeScheduleStrategy ()
{
   if ( algorithmPtr_ != 0 ) algorithmPtr_->removeDependent( this );
}
