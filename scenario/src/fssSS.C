// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/fssSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/scenario.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>



// Populate Schedule with itemTimeVecs
void
LgFrFocussedShortageScheduleScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  LgFrPart part;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat itv( part, tv );

  // Get Focussed shortage schedule
  const LgFrSortingPartScheduleFloat & completeFss = 
                                 fssCtrlPtr_->focussedShortageSchedule();

  // Loop once for each part short part in the completeFss
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<completeFss.size(); i++ ) {

      // Make itemTimeVec
      itv = completeFss[i];

      // Check to see if j'th itemTimevec is in schedule's set of Parts
      // and is not to be filtered out
      if (   setOfParts()  .  contains ( itv.part() )
	     &&
	     !  schedule.filterStrategy().exclude( itv )   ) 
	  {
	      // Insert itemTimeVec into schedule
	      schedule.localInsert( itv );
	  }
  }

}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrFocussedShortageScheduleScheduleStrategy::clone()
const
{
  LgFrFocussedShortageScheduleScheduleStrategy* newInst = 
      new LgFrFocussedShortageScheduleScheduleStrategy(*this);
  return (newInst);
}


// Make clone copy of object with newScenario
LgFrAttributeScheduleStrategy *
LgFrFocussedShortageScheduleScheduleStrategy::clone(
        LgFrScenario& newScenario)
const
{ 
//  return (0);
  LgFrFocussedShortageScheduleScheduleStrategy* newInst = 
      new LgFrFocussedShortageScheduleScheduleStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrFocussedShortageScheduleScheduleStrategy &
LgFrFocussedShortageScheduleScheduleStrategy::operator=(
	  const LgFrFocussedShortageScheduleScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleStrategy::operator= ) (rhs);

    fssCtrlPtr_ = rhs.fssCtrlPtr_;
  }
  return *this;
}


// Copy constructor
LgFrFocussedShortageScheduleScheduleStrategy::
   LgFrFocussedShortageScheduleScheduleStrategy (
      const LgFrFocussedShortageScheduleScheduleStrategy & source)
:  LgFrPartScheduleStrategy( source ), 
   fssCtrlPtr_( source.fssCtrlPtr_ ),
   id_(__LGFRFOCUSSEDSHORTAGESCHEDULESCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Copy constructor with new Scenario
LgFrFocussedShortageScheduleScheduleStrategy::
   LgFrFocussedShortageScheduleScheduleStrategy (
      const LgFrFocussedShortageScheduleScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleStrategy( 
         source, 
         newScenario ),
   fssCtrlPtr_( &(focussedShortageScheduleControl(newScenario)) ),
   id_(__LGFRFOCUSSEDSHORTAGESCHEDULESCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Constructor
LgFrFocussedShortageScheduleScheduleStrategy::
   LgFrFocussedShortageScheduleScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy( 
      LgFrAttributeScheduleStrategy::setOfParts(scenario),
      &(focussedShortageScheduleControl(scenario)) ),
   fssCtrlPtr_( &(focussedShortageScheduleControl(scenario)) ),
   id_(__LGFRFOCUSSEDSHORTAGESCHEDULESCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrFocussedShortageScheduleScheduleStrategy::
   LgFrFocussedShortageScheduleScheduleStrategy ()
:  LgFrPartScheduleStrategy( LgFrSetOfPartsWithMovableParts(), 0 ),
   fssCtrlPtr_( 0 ),
   id_(__LGFRFOCUSSEDSHORTAGESCHEDULESCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrFocussedShortageScheduleScheduleStrategy::
   ~LgFrFocussedShortageScheduleScheduleStrategy ()
{
  // Nothing to do here
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrFocussedShortageScheduleScheduleStrategy methods.
void
LgFrFocussedShortageScheduleScheduleStrategy::test()
{
}

// test LgFrFocussedShortageScheduleScheduleStrategy methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrFocussedShortageScheduleScheduleStrategy::contextTest
    (LgFrScenario          & testScenario,
     LgFrLastProblemSolved & lps,             // testScenario's
     LgFrSetOfParts        & sop,             // testScenario's
     LgFrSetOfDemands      & sod)             // testScenario's
{
  int i;

  // Test constructor
  LgFrFocussedShortageScheduleScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrFocussedShortageScheduleScheduleStrategy cs2(cs1);

  // Test assignment operator
  LgFrFocussedShortageScheduleScheduleStrategy cs3;
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
  // cout <<endl <<"LgFrFocussedShortageScheduleScheduleStrategy" <<endl
  //      <<schedule.format() <<endl;

  for ( i=0; i<sop.size(); i++ ) {
    if ( lps.shortageVolume(&sop[i]) == 0.0 ) {
      assert( !schedule.contains(sop[i]) );
    }
    else {
      assert( lps.shortageVolume(&sop[i]) == 
	     constSchedule[sop[i]].timeVecFloat() );
    }
  }

  // Test populate with filter
  LgFrSortingPartScheduleFloat schedule1;
  const LgFrSortingPartScheduleFloat & constSchedule1 = schedule1;
  schedule1.filterStrategy( LgFrZeroTimeVecFilterStrategy() );
  schedule1.attributeScheduleStrategy( cs1 );

  schedule1.populate();   // equivalent to cs1.populate( schedule1 );
  //  cout <<endl 
  //       <<"LgFrFocussedShortageScheduleScheduleStrategy with zero filter" 
  //       <<endl
  //       <<schedule1.format() <<endl;

  assert( schedule1.size() == schedule.size() );
  for ( i=0; i<schedule1.size(); i++ ) {
     assert(  !(constSchedule1[i].timeVecFloat() == 0.0)  );
     assert(  constSchedule1[i].timeVecFloat() == 
                                constSchedule[i].timeVecFloat()  );
  }
}
