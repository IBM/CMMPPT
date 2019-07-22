// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#include <iostream>

#include <scenario/src/demVolSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/scenario.h>
#include <scenario/src/machdep.h>
#include <scenario/src/demdSchP.h>



// Populate Schedule with itemTimeVecs
void
LgFrDemandVolumeScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  LgFrDemand demand;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat itv( demand, tv );

  // Loop once for each demand
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<setOfDemands().size(); i++ ) {

     demand = setOfDemands()[i];

     // Make itemTimeVec
     itv.item( demand );
     itv.timeVecFloat(
           wipPtr_->timeVec( setOfDemands()[i] )   );


     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );

     
  }

}

// Update (propogate) a demand change to wip 
void 
LgFrDemandVolumeScheduleStrategy::update(
          const LgFrItemTimeVecPairFloat & newITV)
{
  wipPtr_->timeVec(newITV.demand(), newITV.timeVecFloat());

  // inform dependent objects that the demandVol has changed. 
  //  we don't have to do this anymore as wip (through
  //  LgFrUserInputModification) will do it for us
  //  via the updateFrom method.
	
}

// Demand Volumes are mutable
bool
LgFrDemandVolumeScheduleStrategy::isMutable()
const
{
 return true;
}

// Demand Volume TimeVecs mustn't contain any negative values
LgFrValidity
LgFrDemandVolumeScheduleStrategy::validity(
    const LgFrTimeVecFloat& proposedTimeVec)
const
{
  int i;
  // return false upon locating the first negative value
  for ( i=0; i<proposedTimeVec.length(); i++ ) {
    if ( proposedTimeVec[i] < 0.0 ) {
      return INVALID;
    }
  }
  return VALID;
}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrDemandVolumeScheduleStrategy::clone()
const
{
  LgFrDemandVolumeScheduleStrategy* newInst = 
      new LgFrDemandVolumeScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object with a new Scenario
LgFrAttributeScheduleStrategy *
LgFrDemandVolumeScheduleStrategy::clone(LgFrScenario & newScenario)
const
{
  LgFrDemandVolumeScheduleStrategy* newInst = 
      new LgFrDemandVolumeScheduleStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrDemandVolumeScheduleStrategy &
LgFrDemandVolumeScheduleStrategy::operator=(
	  const LgFrDemandVolumeScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrDemandScheduleStrategy::operator= ) (rhs);
    wipPtr_ = rhs.wipPtr_;
  }
  return *this;
}


// Copy constructor
LgFrDemandVolumeScheduleStrategy::
   LgFrDemandVolumeScheduleStrategy (
      const LgFrDemandVolumeScheduleStrategy & source)
:  LgFrDemandScheduleStrategy( source ),
   wipPtr_( source.wipPtr_ ), 
   id_(__LGFRDEMANDVOLUMESCHEDULESTRATEGY)
{
}

// Copy constructor with new Scenario
LgFrDemandVolumeScheduleStrategy::
   LgFrDemandVolumeScheduleStrategy (
      const LgFrDemandVolumeScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrDemandScheduleStrategy(
         source, 
         newScenario ),
   wipPtr_( &(workingImplosionProblem(newScenario)) ),
   id_(__LGFRDEMANDVOLUMESCHEDULESTRATEGY)
{
}

// Constructor
LgFrDemandVolumeScheduleStrategy::
   LgFrDemandVolumeScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrDemandScheduleStrategy( 
      LgFrAttributeScheduleStrategy::setOfDemands(scenario),
                               &(userInputModification(scenario)) ),
   wipPtr_( &(workingImplosionProblem(scenario)) ),
   id_(__LGFRDEMANDVOLUMESCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrDemandVolumeScheduleStrategy::
   LgFrDemandVolumeScheduleStrategy ()
:  LgFrDemandScheduleStrategy( LgFrSetOfDemands() ),
   wipPtr_( 0 ),
   id_(__LGFRDEMANDVOLUMESCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrDemandVolumeScheduleStrategy::
   ~LgFrDemandVolumeScheduleStrategy ()
{
  // Nothing to do here
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrDemandVolumeScheduleStrategy methods.
void
LgFrDemandVolumeScheduleStrategy::test()
{
}

// test LgFrDemandVolumeScheduleStrategy methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrDemandVolumeScheduleStrategy::contextTest
    (LgFrScenario                & testScenario,
     LgFrWorkingImplosionProblem & wip,			// testScenario's 
     LgFrSetOfDemands            & sod)			// testScenario's 
{
  int i;

  // Test constructor
  LgFrDemandVolumeScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrDemandVolumeScheduleStrategy cs2(cs1);

  // Test assignment operator
  LgFrDemandVolumeScheduleStrategy cs3;
  cs3 = cs2;

  {
    // Test clone method
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();

    // Test destructor
    delete cs4Ptr;
  }
  
  // Test populate. 
  LgFrSortingDemandScheduleFloatSmartPointer schedule;
  const LgFrSortingDemandScheduleFloat & constSchedule = *schedule;
  cs1.populate( *schedule );
  for ( i=0; i<sod.size(); i++ ) {
     assert( sod[i] == constSchedule[i].demand() );
     assert( wip.timeVec(sod[i]) == constSchedule[i].timeVecFloat() );
  }

  // Test popluate with zeroFilter.  Should produce an empty schedule
  LgFrZeroTimeVecFilterStrategy zeroFilter;
  schedule->filterStrategy( zeroFilter );
  cs1.populate( *schedule );

  for ( i=0; i<schedule->size(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }

  // Test the update() method
  LgFrDemand aDemand(sod[0]);
  LgFrItemTimeVecPairFloat anITV(aDemand, 
                                constSchedule[0].timeVecFloat() + 2.5);
  cs1.update(anITV);
  cs1.populate(*schedule);
  assert(anITV.timeVecFloat() == constSchedule[0].timeVecFloat());

}
