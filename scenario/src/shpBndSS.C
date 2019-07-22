#include <iostream.h>

#include <scenario/src/shpRewSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/scenario.h>
#include <scenario/src/machdep.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/shpBndSS.h>

// Populate Schedule with itemTimeVecs
void
LgFrShipSoftLowerBoundScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  LgFrDemand demand;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat itv( demand, tv );

  // Loop once for each demand
  for ( size_t i=0; i<setOfDemands().entries(); i++ ) {

     demand = setOfDemands()[i];

     // Make itemTimeVec
     itv.item( demand );
     itv.timeVecFloat(
           wipPtr_->cumShipSoftLowerBound( setOfDemands()[i] ) . undoCum()
           );

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
     
  }

}

// Update (propogate) a demand change to wip 
void 
LgFrShipSoftLowerBoundScheduleStrategy::update(
          const LgFrItemTimeVecPairFloat & newITV)
{
  wipPtr_->cumShipSoftLowerBound(
                                 newITV.demand(),
                                 newITV.timeVecFloat() . scan( LgFrAdd )
                                 );
}

// Demand ShipSoftLowerBounds are mutable
RWBoolean
LgFrShipSoftLowerBoundScheduleStrategy::isMutable()
const
{
 return TRUE;
}

// Demand ShipSoftLowerBounds TimeVecs mustn't contain any negative values
LgFrValidity
LgFrShipSoftLowerBoundScheduleStrategy::validity(
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
LgFrShipSoftLowerBoundScheduleStrategy::clone()
const
{
  LgFrShipSoftLowerBoundScheduleStrategy* newInst = 
      new LgFrShipSoftLowerBoundScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object with a new Scenario
LgFrAttributeScheduleStrategy *
LgFrShipSoftLowerBoundScheduleStrategy::clone(LgFrScenario & newScenario)
const
{
  LgFrShipSoftLowerBoundScheduleStrategy* newInst = 
      new LgFrShipSoftLowerBoundScheduleStrategy(*this, newScenario);
  return (newInst);
}

// Return schedule strategy identifier
isAReturnType
LgFrShipSoftLowerBoundScheduleStrategy::isA()
const
{
   return id_;
}

// Assignment operator
LgFrShipSoftLowerBoundScheduleStrategy &
LgFrShipSoftLowerBoundScheduleStrategy::operator=(
	  const LgFrShipSoftLowerBoundScheduleStrategy& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrDemandScheduleStrategy::operator= ) (rhs);
    wipPtr_ = rhs.wipPtr_;
  }
  return *this;
}


// Copy constructor
LgFrShipSoftLowerBoundScheduleStrategy::
   LgFrShipSoftLowerBoundScheduleStrategy (
      const LgFrShipSoftLowerBoundScheduleStrategy & source)
:  LgFrDemandScheduleStrategy( source ),
   wipPtr_( source.wipPtr_ ), 
   id_(__LGFRSHIPSOFTLOWERBOUNDSCHEDULESTRATEGY)
{
  assert( id_ == source.id_ );
}

// Copy constructor with new Scenario
LgFrShipSoftLowerBoundScheduleStrategy::
   LgFrShipSoftLowerBoundScheduleStrategy (
      const LgFrShipSoftLowerBoundScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrDemandScheduleStrategy(
         source, 
         newScenario ),
   wipPtr_( &(workingImplosionProblem(newScenario)) ),
   id_(__LGFRSHIPSOFTLOWERBOUNDSCHEDULESTRATEGY)
{
  assert( id_ == source.id_ );
}

// Constructor
LgFrShipSoftLowerBoundScheduleStrategy::
   LgFrShipSoftLowerBoundScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrDemandScheduleStrategy( 
      LgFrAttributeScheduleStrategy::setOfDemands(scenario),
                               &(userInputModification(scenario)) ),
   wipPtr_( &(workingImplosionProblem(scenario)) ),
   id_(__LGFRSHIPSOFTLOWERBOUNDSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrShipSoftLowerBoundScheduleStrategy::
   LgFrShipSoftLowerBoundScheduleStrategy ()
:  LgFrDemandScheduleStrategy( LgFrSetOfDemands() ),
   wipPtr_( NULL ),
   id_(__LGFRSHIPSOFTLOWERBOUNDSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrShipSoftLowerBoundScheduleStrategy::
   ~LgFrShipSoftLowerBoundScheduleStrategy ()
{
  // Nothing to do here
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrShipSoftLowerBoundScheduleStrategy methods.
void
LgFrShipSoftLowerBoundScheduleStrategy::test()
{
}

// test LgFrShipSoftLowerBoundScheduleStrategy methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrShipSoftLowerBoundScheduleStrategy::contextTest
    (LgFrScenario                & testScenario,
     LgFrWorkingImplosionProblem & wip,			// testScenario's 
     LgFrSetOfDemands            & sod)			// testScenario's 
{
  int i;

  // Test constructor
  LgFrShipSoftLowerBoundScheduleStrategy cs1(testScenario);
  assert( cs1.id_ == __LGFRSHIPSOFTLOWERBOUNDSCHEDULESTRATEGY );

  // Test isA method
  assert( cs1.isA() == __LGFRSHIPSOFTLOWERBOUNDSCHEDULESTRATEGY );

  // Test copy constructor
  LgFrShipSoftLowerBoundScheduleStrategy cs2(cs1);
  assert( cs2.isA() == __LGFRSHIPSOFTLOWERBOUNDSCHEDULESTRATEGY );

  // Test assignment operator
  LgFrShipSoftLowerBoundScheduleStrategy cs3;
  cs3 = cs2;
  assert( cs3.isA() == cs2.isA() );

  {
    // Test clone method
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( cs4Ptr->isA() == cs1.isA() );

    // Test destructor
    delete cs4Ptr;
  }
  
  // Test populate. 
  LgFrSortingDemandScheduleFloatSmartPointer
    schedule(new LgFrSortingDemandScheduleFloat); 
  const LgFrSortingDemandScheduleFloat & constSchedule = *schedule;
  cs1.populate( *schedule );
  for ( i=0; i<sod.entries(); i++ ) {
     assert( sod[i] == constSchedule[i].demand() );
     assert( wip.cumShipSoftLowerBound(sod[i])
             == constSchedule[i].timeVecFloat() );
  }

  // Test the update() method
  LgFrDemand aDemand(sod[0]);
  LgFrItemTimeVecPairFloat anITV(aDemand, 
                                constSchedule[0].timeVecFloat() + 2.);
  cs1.update(anITV);
  cs1.populate(*schedule);
  assert(anITV.timeVecFloat() == constSchedule[aDemand].timeVecFloat());

}
