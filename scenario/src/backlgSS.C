
#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/backlgSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/scenario.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>
#include <scenario/src/demdSchP.h>

// Populate Schedule with itemTimeVecs
void
LgFrBacklogScheduleStrategy::kernalOfPopulate(
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
           implodePtr_->lastProblemSolved().backlogVolume( &demand )   );

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );

  }

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrBacklogScheduleStrategy::clone()
const
{
  LgFrBacklogScheduleStrategy* newInst = 
      new LgFrBacklogScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrAttributeScheduleStrategy *
LgFrBacklogScheduleStrategy::clone(LgFrScenario& newScenario)
const
{
  LgFrBacklogScheduleStrategy* newInst = 
      new LgFrBacklogScheduleStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrBacklogScheduleStrategy &
LgFrBacklogScheduleStrategy::operator=(
	  const LgFrBacklogScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self

    (this->LgFrDemandScheduleStrategy::operator= ) (rhs);

    implodePtr_ = rhs.implodePtr_;

  }
  return *this;
}


// Copy constructor
LgFrBacklogScheduleStrategy::
   LgFrBacklogScheduleStrategy (
      const LgFrBacklogScheduleStrategy & source)
:  LgFrDemandScheduleStrategy( source ), 
   implodePtr_( source.implodePtr_ ),
   id_(__LGFRBACKLOGSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Copy constructor with newScenario
LgFrBacklogScheduleStrategy::
   LgFrBacklogScheduleStrategy (
      const LgFrBacklogScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrDemandScheduleStrategy( 
         source, 
         newScenario ),
   implodePtr_( &(implode(newScenario)) ),
   id_(__LGFRBACKLOGSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Constructor
LgFrBacklogScheduleStrategy::
   LgFrBacklogScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrDemandScheduleStrategy( 
      LgFrAttributeScheduleStrategy::setOfDemands(scenario), 
      &(implode(scenario)) ),
   implodePtr_( &(implode(scenario)) ),
   id_(__LGFRBACKLOGSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrBacklogScheduleStrategy::
   LgFrBacklogScheduleStrategy ()
:  LgFrDemandScheduleStrategy( LgFrSetOfDemands(), 0 ),
   implodePtr_( 0 ),
   id_(__LGFRBACKLOGSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrBacklogScheduleStrategy::
   ~LgFrBacklogScheduleStrategy ()
{
  // Nothing to do here
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrBacklogScheduleStrategy methods.
void
LgFrBacklogScheduleStrategy::test()
{
}


// test LgFrBacklogScheduleStrategy methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrBacklogScheduleStrategy::contextTest
  (
   LgFrScenario          & testScenario,
   LgFrLastProblemSolved & lps,			// testScenario's 
   LgFrSetOfDemands      & sod)			// testScenario's 
{
  int i;

  // Test constructor
  LgFrBacklogScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrBacklogScheduleStrategy cs2(cs1);

  // Test assignment operator
  LgFrBacklogScheduleStrategy cs3;
  cs3 = cs2;

  {
    // Test clone method
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();

    // Test destructor
    delete cs4Ptr;
  }

  // Test populate.  Should be populated with demandVolume timeVec because
  // a shimpment schedule does not yet exist (an implode has not yet been
  // done).
  LgFrSortingDemandScheduleFloatSmartPointer schedule;
  const LgFrSortingDemandScheduleFloat & constSchedule = *schedule;
  cs1.populate( *schedule );
  for ( i=0; i<sod.size(); i++ ) {
     assert(  schedule->contains( sod[i] )  );
     assert(  constSchedule [sod[i]].timeVecFloat() 
	    == lps.demandVolume(&sod[i]).scan(LgFrAdd) );
  }

  // Test populate, due to new implosion solution.
  schedule->attributeScheduleStrategy( cs1 );
  testScenario.newImplosionSolution();
  // cs1.populate( schedule );
  // std::cout << std::endl <<"LgFrBacklogScheduleStrategy" << std::endl
  //      <<schedule->format() << std::endl;

  for ( i=0; i<sod.size(); i++ ) {
     assert(  schedule->contains( sod[i] )  );
     assert( lps.backlogVolume(&sod[i]) == 
                constSchedule[sod[i]].timeVecFloat() );
  }

  // Test populate with filter
  schedule->filterStrategy( LgFrZeroTimeVecFilterStrategy() );
  schedule->attributeScheduleStrategy( cs1 );
  testScenario.newImplosionSolution();

  cs1.populate( *schedule );
  // std::cout << std::endl <<"LgFrBacklogScheduleStrategy with zero filter" << std::endl
  //      <<schedule->format() << std::endl;

  for ( i=0; i<schedule->size(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }
}

