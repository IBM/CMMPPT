// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
// Schedule Strategy for a focussed shortage schedule's production schedule.
// The items are parts.
// The timeVecs are the parts' focussed shortage schedule's production schedule.
#include <iostream>

#include <scenario/src/fssPrdSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/scenario.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>



// Populate Schedule with itemTimeVecs
void
LgFrFocussedShortageScheduleProductionScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  LgFrPart part;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat itv( part, tv );

  // Loop once for each part
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<setOfParts().size(); i++ ) {

     part = setOfParts()[i];

     // Is i'th part a product?
     if ( fssCtrlPtr_->implodePtr()->lastProblemSolved().category( &part )
          == LgFrPRODUCT ) {

        // Make itemTimeVec
        itv.item( part );
        itv.timeVecFloat(
              fssCtrlPtr_->fssProductionVolume( &part )   );

        // Check to see if i'th itemTimeVec is to be filtered out
        if ( schedule.filterStrategy().exclude( itv ) ) continue;

        // Insert itemTimeVec into schedule
        schedule.localInsert( itv );

     }
  }

}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrFocussedShortageScheduleProductionScheduleStrategy::clone()
const
{
  LgFrFocussedShortageScheduleProductionScheduleStrategy* newInst = 
      new LgFrFocussedShortageScheduleProductionScheduleStrategy(*this);
  return (newInst);
}


// Make clone copy of object with newScenario
LgFrAttributeScheduleStrategy *
LgFrFocussedShortageScheduleProductionScheduleStrategy::clone(
        LgFrScenario& newScenario)
const
{ 
//  return (0);
  LgFrFocussedShortageScheduleProductionScheduleStrategy* newInst = 
      new LgFrFocussedShortageScheduleProductionScheduleStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrFocussedShortageScheduleProductionScheduleStrategy &
LgFrFocussedShortageScheduleProductionScheduleStrategy::operator=(
	  const LgFrFocussedShortageScheduleProductionScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleStrategy::operator= ) (rhs);

    fssCtrlPtr_ = rhs.fssCtrlPtr_;
  }
  return *this;
}


// Copy constructor
LgFrFocussedShortageScheduleProductionScheduleStrategy::
   LgFrFocussedShortageScheduleProductionScheduleStrategy (
      const LgFrFocussedShortageScheduleProductionScheduleStrategy & source)
:  LgFrPartScheduleStrategy( source ), 
   fssCtrlPtr_( source.fssCtrlPtr_ ),
   id_(__LGFRFOCUSSEDSHORTAGESCHEDULEPRODUCTIONSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Copy constructor with new Scenario
LgFrFocussedShortageScheduleProductionScheduleStrategy::
   LgFrFocussedShortageScheduleProductionScheduleStrategy (
      const LgFrFocussedShortageScheduleProductionScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleStrategy( 
         source, 
         newScenario ),
   fssCtrlPtr_( &(focussedShortageScheduleControl(newScenario)) ),
   id_(__LGFRFOCUSSEDSHORTAGESCHEDULEPRODUCTIONSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Constructor
LgFrFocussedShortageScheduleProductionScheduleStrategy::
   LgFrFocussedShortageScheduleProductionScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy( 
      LgFrAttributeScheduleStrategy::setOfParts(scenario),
      &(focussedShortageScheduleControl(scenario)) ),
   fssCtrlPtr_( &(focussedShortageScheduleControl(scenario)) ),
   id_(__LGFRFOCUSSEDSHORTAGESCHEDULEPRODUCTIONSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrFocussedShortageScheduleProductionScheduleStrategy::
   LgFrFocussedShortageScheduleProductionScheduleStrategy ()
:  LgFrPartScheduleStrategy( LgFrSetOfPartsWithMovableParts(), 0 ),
   fssCtrlPtr_( 0 ),
   id_(__LGFRFOCUSSEDSHORTAGESCHEDULEPRODUCTIONSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrFocussedShortageScheduleProductionScheduleStrategy::
   ~LgFrFocussedShortageScheduleProductionScheduleStrategy ()
{
  // Nothing to do here
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrFocussedShortageScheduleProductionScheduleStrategy methods.
void
LgFrFocussedShortageScheduleProductionScheduleStrategy::test()
{
}

// test LgFrFocussedShortageScheduleProductionScheduleStrategy methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrFocussedShortageScheduleProductionScheduleStrategy::contextTest
    (LgFrScenario          & testScenario,
     LgFrLastProblemSolved & lps,             // testScenario's
     LgFrSetOfParts        & sop,             // testScenario's
     LgFrSetOfDemands      & sod)             // testScenario's
{
  int i;

  // Test constructor
  LgFrFocussedShortageScheduleProductionScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrFocussedShortageScheduleProductionScheduleStrategy cs2(cs1);

  // Test assignment operator
  LgFrFocussedShortageScheduleProductionScheduleStrategy cs3;
  cs3 = cs2;

  {
    // Test clone method
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();

    // Test destructor
    delete cs4Ptr;
  }
  
  // Set a focus horizon
  testScenario.focussedShortageScheduleControl().
     focusHorizon( *sod.findValue("demand","Ham_Sand"), 1 );

  // Test populate due to new implosion solution.
  LgFrSortingPartScheduleFloat schedule;
  const LgFrSortingPartScheduleFloat & constSchedule = schedule;
  schedule.attributeScheduleStrategy( cs1 );
  testScenario.newImplosionSolution();
  //  cout <<endl <<"LgFrFocussedShortageScheduleProductionScheduleStrategy" <<endl
  //      <<schedule.format() <<endl;

  for ( i=0; i<sop.size(); i++ )
    if ( lps.category( &sop[i] ) == LgFrPRODUCT )
      assert( lps.fssProductionVolume(&sop[i]) == 
	     constSchedule[sop[i]].timeVecFloat() );

  // Test populate with filter
  LgFrSortingPartScheduleFloat schedule1;
  const LgFrSortingPartScheduleFloat & constSchedule1 = schedule1;
  schedule1.filterStrategy( LgFrZeroTimeVecFilterStrategy() );
  schedule1.attributeScheduleStrategy( cs1 );

  schedule1.populate();   // equivalent to cs1.populate( schedule1 );
  //    cout <<endl 
  //         <<"LgFrFocussedShortageScheduleProductionScheduleStrategy with zero filter" 
  //         <<endl
  //         <<schedule1.format() <<endl;

  for ( i=0; i<schedule1.size(); i++ ) {
     assert(  !(constSchedule1[i].timeVecFloat() == 0.0)  );
  }
}
