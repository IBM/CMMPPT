// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#include <iostream>

#include <scenario/src/priorSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/scenario.h>
#include <scenario/src/machdep.h>
#include <scenario/src/demdSchP.h>


// Populate Schedule with itemTimeVecs
void
LgFrPriorityScheduleStrategy::kernalOfPopulate(
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
           wipPtr_->priority( setOfDemands()[i] )   );

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
     
  }

}

// Update (propogate) a demand change to wip 
void 
LgFrPriorityScheduleStrategy::update(
          const LgFrItemTimeVecPairFloat & newITV)
{
  wipPtr_->priority(newITV.demand(), newITV.timeVecFloat());
}

// Demand Priorities are mutable
bool
LgFrPriorityScheduleStrategy::isMutable()
const
{
 return true;
}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrPriorityScheduleStrategy::clone()
const
{
  LgFrPriorityScheduleStrategy* newInst = 
      new LgFrPriorityScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object with a new Scenario
LgFrAttributeScheduleStrategy *
LgFrPriorityScheduleStrategy::clone(LgFrScenario & newScenario)
const
{
  LgFrPriorityScheduleStrategy* newInst = 
      new LgFrPriorityScheduleStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrPriorityScheduleStrategy &
LgFrPriorityScheduleStrategy::operator=(
	  const LgFrPriorityScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrDemandScheduleStrategy::operator= ) (rhs);
    wipPtr_ = rhs.wipPtr_;
  }
  return *this;
}


// Copy constructor
LgFrPriorityScheduleStrategy::
   LgFrPriorityScheduleStrategy (
      const LgFrPriorityScheduleStrategy & source)
:  LgFrDemandScheduleStrategy( source ),
   wipPtr_( source.wipPtr_ ), 
   id_(__LGFRPRIORITYSCHEDULESTRATEGY)
{
}

// Copy constructor with new Scenario
LgFrPriorityScheduleStrategy::
   LgFrPriorityScheduleStrategy (
      const LgFrPriorityScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrDemandScheduleStrategy(
         source, 
         newScenario ),
   wipPtr_( &(workingImplosionProblem(newScenario)) ),
   id_(__LGFRPRIORITYSCHEDULESTRATEGY)
{
}

// Constructor
LgFrPriorityScheduleStrategy::
   LgFrPriorityScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrDemandScheduleStrategy( 
      LgFrAttributeScheduleStrategy::setOfDemands(scenario),
                               &(userInputModification(scenario)) ),
   wipPtr_( &(workingImplosionProblem(scenario)) ),
   id_(__LGFRPRIORITYSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrPriorityScheduleStrategy::
   LgFrPriorityScheduleStrategy ()
:  LgFrDemandScheduleStrategy( LgFrSetOfDemands() ),
   wipPtr_( 0 ),
   id_(__LGFRPRIORITYSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrPriorityScheduleStrategy::
   ~LgFrPriorityScheduleStrategy ()
{
  // Nothing to do here
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrPriorityScheduleStrategy methods.
void
LgFrPriorityScheduleStrategy::test()
{
}

// test LgFrPriorityScheduleStrategy methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrPriorityScheduleStrategy::contextTest
    (LgFrScenario                & testScenario,
     LgFrWorkingImplosionProblem & wip,			// testScenario's 
     LgFrSetOfDemands            & sod)			// testScenario's 
{
  int i;

  // Test constructor
  LgFrPriorityScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrPriorityScheduleStrategy cs2(cs1);

  // Test assignment operator
  LgFrPriorityScheduleStrategy cs3;
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
     assert( wip.priority(sod[i]) == constSchedule[i].timeVecFloat() );
  }

  // Test the update() method
  LgFrDemand aDemand(sod[0]);
  LgFrItemTimeVecPairFloat anITV(aDemand, 
                                constSchedule[0].timeVecFloat() + 2.);
  cs1.update(anITV);
  cs1.populate(*schedule);
  assert(anITV.timeVecFloat() == constSchedule[aDemand].timeVecFloat());

}
