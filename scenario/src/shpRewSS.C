
#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/shpRewSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/scenario.h>
#include <scenario/src/machdep.h>
#include <scenario/src/demdSchP.h>

// Populate Schedule with itemTimeVecs
void
LgFrObj1ShipRewardScheduleStrategy::kernalOfPopulate(
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
           wipPtr_->obj1ShipReward( setOfDemands()[i] )   );

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
     
  }

}

// Update (propogate) a demand change to wip 
void 
LgFrObj1ShipRewardScheduleStrategy::update(
          const LgFrItemTimeVecPairFloat & newITV)
{
  wipPtr_->obj1ShipReward(newITV.demand(), newITV.timeVecFloat());
}

// Demand Obj1ShipRewards are mutable
bool
LgFrObj1ShipRewardScheduleStrategy::isMutable()
const
{
 return true;
}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrObj1ShipRewardScheduleStrategy::clone()
const
{
  LgFrObj1ShipRewardScheduleStrategy* newInst = 
      new LgFrObj1ShipRewardScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object with a new Scenario
LgFrAttributeScheduleStrategy *
LgFrObj1ShipRewardScheduleStrategy::clone(LgFrScenario & newScenario)
const
{
  LgFrObj1ShipRewardScheduleStrategy* newInst = 
      new LgFrObj1ShipRewardScheduleStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrObj1ShipRewardScheduleStrategy &
LgFrObj1ShipRewardScheduleStrategy::operator=(
	  const LgFrObj1ShipRewardScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrDemandScheduleStrategy::operator= ) (rhs);
    wipPtr_ = rhs.wipPtr_;
  }
  return *this;
}


// Copy constructor
LgFrObj1ShipRewardScheduleStrategy::
   LgFrObj1ShipRewardScheduleStrategy (
      const LgFrObj1ShipRewardScheduleStrategy & source)
:  LgFrDemandScheduleStrategy( source ),
   wipPtr_( source.wipPtr_ ), 
   id_(__LGFROBJ1SHIPREWARDSCHEDULESTRATEGY)
{
}

// Copy constructor with new Scenario
LgFrObj1ShipRewardScheduleStrategy::
   LgFrObj1ShipRewardScheduleStrategy (
      const LgFrObj1ShipRewardScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrDemandScheduleStrategy(
         source, 
         newScenario ),
   wipPtr_( &(workingImplosionProblem(newScenario)) ),
   id_(__LGFROBJ1SHIPREWARDSCHEDULESTRATEGY)
{
}

// Constructor
LgFrObj1ShipRewardScheduleStrategy::
   LgFrObj1ShipRewardScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrDemandScheduleStrategy( 
      LgFrAttributeScheduleStrategy::setOfDemands(scenario),
                               &(userInputModification(scenario)) ),
   wipPtr_( &(workingImplosionProblem(scenario)) ),
   id_(__LGFROBJ1SHIPREWARDSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrObj1ShipRewardScheduleStrategy::
   LgFrObj1ShipRewardScheduleStrategy ()
:  LgFrDemandScheduleStrategy( LgFrSetOfDemands() ),
   wipPtr_( 0 ),
   id_(__LGFROBJ1SHIPREWARDSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrObj1ShipRewardScheduleStrategy::
   ~LgFrObj1ShipRewardScheduleStrategy ()
{
  // Nothing to do here
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrObj1ShipRewardScheduleStrategy methods.
void
LgFrObj1ShipRewardScheduleStrategy::test()
{
}

// test LgFrObj1ShipRewardScheduleStrategy methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrObj1ShipRewardScheduleStrategy::contextTest
    (LgFrScenario                & testScenario,
     LgFrWorkingImplosionProblem & wip,			// testScenario's 
     LgFrSetOfDemands            & sod)			// testScenario's 
{
  int i;

  // Test constructor
  LgFrObj1ShipRewardScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrObj1ShipRewardScheduleStrategy cs2(cs1);

  // Test assignment operator
  LgFrObj1ShipRewardScheduleStrategy cs3;
  cs3 = cs2;

  {
    // Test clone method
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();

    // Test destructor
    delete cs4Ptr;
  }
  
  // Test populate. 
  LgFrSortingDemandScheduleFloatSmartPointer schedule(new LgFrSortingDemandScheduleFloat); 
  const LgFrSortingDemandScheduleFloat & constSchedule = *schedule;
  cs1.populate( *schedule );
  for ( i=0; i<sod.size(); i++ ) {
     assert( sod[i] == constSchedule[i].demand() );
     assert( wip.obj1ShipReward(sod[i]) == constSchedule[i].timeVecFloat() );
  }

  // Test the update() method
  LgFrDemand aDemand(sod[0]);
  LgFrItemTimeVecPairFloat anITV(aDemand, 
                                constSchedule[0].timeVecFloat() + 2.);
  cs1.update(anITV);
  cs1.populate(*schedule);
  assert(anITV.timeVecFloat() == constSchedule[aDemand].timeVecFloat());

}
