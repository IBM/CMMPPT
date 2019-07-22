// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/shipSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/scenario.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>
#include <scenario/src/demdSchP.h>

// Populate Schedule with itemTimeVecs
void
LgFrShipmentScheduleStrategy::kernalOfPopulate(
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
           implodePtr_->lastProblemSolved().shipmentVolume( &demand )   );

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );

     
  }

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrShipmentScheduleStrategy::clone()
const
{
  LgFrShipmentScheduleStrategy* newInst = 
      new LgFrShipmentScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object with newScenario
LgFrAttributeScheduleStrategy *
LgFrShipmentScheduleStrategy::clone(LgFrScenario& newScenario)
const
{ 
  LgFrShipmentScheduleStrategy* newInst = 
      new LgFrShipmentScheduleStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrShipmentScheduleStrategy &
LgFrShipmentScheduleStrategy::operator=(
	  const LgFrShipmentScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrDemandScheduleStrategy::operator= ) (rhs);

    implodePtr_ = rhs.implodePtr_;
  }
  return *this;
}


// Copy constructor
LgFrShipmentScheduleStrategy::
   LgFrShipmentScheduleStrategy (
      const LgFrShipmentScheduleStrategy & source)
:  LgFrDemandScheduleStrategy( source ), 
   implodePtr_( source.implodePtr_ ),
   id_(__LGFRSHIPMENTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Copy constructor with newScenario
LgFrShipmentScheduleStrategy::
   LgFrShipmentScheduleStrategy (
      const LgFrShipmentScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrDemandScheduleStrategy( 
         source, 
         newScenario ),
   implodePtr_( &(implode(newScenario)) ),
   id_(__LGFRSHIPMENTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Constructor
LgFrShipmentScheduleStrategy::
   LgFrShipmentScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrDemandScheduleStrategy( 
      LgFrAttributeScheduleStrategy::setOfDemands(scenario),
      &(implode(scenario)) ),
   implodePtr_( &implode(scenario) ),
   id_(__LGFRSHIPMENTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrShipmentScheduleStrategy::
   LgFrShipmentScheduleStrategy ()
:  LgFrDemandScheduleStrategy( LgFrSetOfDemands() ),
   implodePtr_( 0 ),
   id_(__LGFRSHIPMENTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrShipmentScheduleStrategy::
   ~LgFrShipmentScheduleStrategy ()
{
  // Nothing to do here
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrShipmentScheduleStrategy methods.
void
LgFrShipmentScheduleStrategy::test()
{
}

// test LgFrShipmentScheduleStrategy methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrShipmentScheduleStrategy::contextTest
  (
   LgFrScenario			& testScenario,
   LgFrScenario			& testScenario2,
   LgFrLastProblemSolved	& lps,			// testScenario's 
   LgFrSetOfDemands		& sod)			// testScenario's 
{
  int i;

  // Test constructor
  LgFrShipmentScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrShipmentScheduleStrategy cs2(cs1);


  // Test copy constructor with new scenario - while
  //   testScenario1 and testScenario2 are similar, they
  //   are separate objects -> they don't share parts!!
  LgFrShipmentScheduleStrategy cs13(cs1, testScenario2);
  assert(cs13.implodePtr_ != cs2.implodePtr_);

  // Test assignment operator
  LgFrShipmentScheduleStrategy cs3;
  cs3 = cs2;

  {
    // Test clone method
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();

    // Test destructor
    delete cs4Ptr;
  }

  // Test populate.  Should be populated with zero timeVec because
  // a Shipment schedule does not exist.
  LgFrSortingDemandScheduleFloatSmartPointer schedule;
  const LgFrSortingDemandScheduleFloat & constSchedule = *schedule;

  LgFrSortingDemandScheduleFloatSmartPointer schedule2;
  const LgFrSortingDemandScheduleFloat & constSchedule2 = *schedule2;

  cs1.populate( *schedule );
  cs13.populate( *schedule2 );

  for ( i=0; i<sod.size(); i++ ) {
     assert(  constSchedule.contains( sod[i] )  );
     assert( constSchedule[sod[i]].timeVecFloat() == 0.0 );
  }
  assert(constSchedule == constSchedule2);

  // Test popluate with zeroFilter.  Should produce an empty schedule
  LgFrZeroTimeVecFilterStrategy zeroFilter;
  schedule->filterStrategy( zeroFilter );
  cs1.populate( *schedule );
  schedule2->filterStrategy( zeroFilter );
  cs13.populate( *schedule2 );
  assert( schedule->size()==0 );

  // Test populate, due to new implosion solution.
  {
  LgFrFilterStrategy defaultFilter;
  schedule->filterStrategy( defaultFilter );
  schedule2->filterStrategy( defaultFilter );
  }
  schedule->attributeScheduleStrategy( cs1 );
  schedule2->attributeScheduleStrategy( cs13 );
  testScenario.newImplosionSolution();
  testScenario2.newImplosionSolution();
  // cs1.populate( *schedule );
  // cout <<endl <<"LgFrShipmentScheduleStrategy" <<endl
  //      <<schedule->format() <<endl;

  for ( i=0; i<sod.size(); i++ ) {
     assert(  schedule->contains( sod[i] )  );
     assert( lps.shipmentVolume(&sod[i]) == 
                constSchedule[sod[i]].timeVecFloat() );
  }

  // Test populate with filter
  schedule->filterStrategy( zeroFilter);
  schedule->attributeScheduleStrategy( cs1 );
  testScenario.newImplosionSolution();

  cs1.populate( *schedule );
  // cout <<endl <<"LgFrShipmentScheduleStrategy with zero filter" <<endl
  //      <<schedule->format() <<endl;

  for ( i=0; i<schedule->size(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }
}
