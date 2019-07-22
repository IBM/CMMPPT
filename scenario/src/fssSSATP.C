#include <iostream.h>

#include <scenario/src/fssSSATP.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/scenATP.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>


#include <scenario/src/sOfDemds.h>


// Override base class populate method.  Explicitly invoke base
// class populate method if current state contains data for this class.


void
LgFrFocussedShortageScheduleScheduleStrategyForATP::populate(
   LgFrSortingScheduleFloat& sched )
const
{
  // only populate schedule if the state machine is in the state
  //  that this ASS really corresponds to
  int futureState = 0;

  // add the mapping from tokens to future state here
  if (scenario()->dfa()->GetCurrToken()->type == LGFR_ATP_TOKEN_GO_FROM_CI_TO_FSS)
    futureState = LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY;
  
  if (futureState == desiredState())
    LgFrAttributeScheduleStrategy::populate( sched );
}



// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrFocussedShortageScheduleScheduleStrategyForATP::clone()
const
{
  LgFrFocussedShortageScheduleScheduleStrategyForATP* newInst = 
      new LgFrFocussedShortageScheduleScheduleStrategyForATP(*this);
  return (newInst);
}

// Make clone copy of object with newScenario
LgFrAttributeScheduleStrategy *
LgFrFocussedShortageScheduleScheduleStrategyForATP::clone(LgFrScenarioSmartPointer newScenario)
const
{ 
  return new LgFrFocussedShortageScheduleScheduleStrategyForATP(*this, newScenario);
}


LgFrAttributeScheduleStrategy *
LgFrFocussedShortageScheduleScheduleStrategyForATP::clone(LgFrScenario& newScenario)
const
{ 
  // deep copy may not be working correctly
    cerr << "ERROR: " << __FILE__ << ", " << __LINE__
      << ", this method should not be called" << endl;
    return (LgFrAttributeScheduleStrategy *)NULL;
}

// Return schedule strategy identifier
isAReturnType
LgFrFocussedShortageScheduleScheduleStrategyForATP::isA()
const
{
   return id_;
}

// Assignment operator
LgFrFocussedShortageScheduleScheduleStrategyForATP &
LgFrFocussedShortageScheduleScheduleStrategyForATP::operator=(
	  const LgFrFocussedShortageScheduleScheduleStrategyForATP& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrFocussedShortageScheduleScheduleStrategy::operator= ) (rhs);
    scenario(rhs.scenario());
  }
  return *this;
}


// Copy constructor
LgFrFocussedShortageScheduleScheduleStrategyForATP::
   LgFrFocussedShortageScheduleScheduleStrategyForATP (
      const LgFrFocussedShortageScheduleScheduleStrategyForATP & source)
:  LgFrFocussedShortageScheduleScheduleStrategy( source ),
   LgFrScheduleMixInForATP( source.scenario(), source.desiredState() ),
   id_(__LGFRFOCUSSEDSHORTAGESCHEDULESCHEDULESTRATEGYFORATP)
{
  // Nothing to do here
}

// Copy constructor with newScenario
LgFrFocussedShortageScheduleScheduleStrategyForATP::
   LgFrFocussedShortageScheduleScheduleStrategyForATP (
      const LgFrFocussedShortageScheduleScheduleStrategyForATP & source,
      LgFrScenarioSmartPointer newScenario)
:  LgFrFocussedShortageScheduleScheduleStrategy(source, *newScenario ),
   LgFrScheduleMixInForATP( newScenario, source.desiredState() ),
   id_(__LGFRFOCUSSEDSHORTAGESCHEDULESCHEDULESTRATEGYFORATP)
{
  assert( id_ == source.id_);
}

// Constructor
LgFrFocussedShortageScheduleScheduleStrategyForATP::
   LgFrFocussedShortageScheduleScheduleStrategyForATP (
     LgFrScenarioSmartPointer scenario, int desiredState )
:  LgFrFocussedShortageScheduleScheduleStrategy(*scenario),
   LgFrScheduleMixInForATP(scenario, desiredState),
   id_(__LGFRFOCUSSEDSHORTAGESCHEDULESCHEDULESTRATEGYFORATP)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrFocussedShortageScheduleScheduleStrategyForATP::
   LgFrFocussedShortageScheduleScheduleStrategyForATP ()
:  LgFrFocussedShortageScheduleScheduleStrategy(),
   LgFrScheduleMixInForATP(),
   id_(__LGFRFOCUSSEDSHORTAGESCHEDULESCHEDULESTRATEGYFORATP)
{
  // Nothing to do here
}

// Destructor
LgFrFocussedShortageScheduleScheduleStrategyForATP::
   ~LgFrFocussedShortageScheduleScheduleStrategyForATP ()
{
  // Nothing to do here
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrFocussedShortageScheduleScheduleStrategyForATP methods.
void
LgFrFocussedShortageScheduleScheduleStrategyForATP::test()
{
}



// test LgFrFocussedsedShortageScheduleScheduleStrategy methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrFocussedShortageScheduleScheduleStrategyForATP::contextTest
    (LgFrScenarioForATPSmartPointer testScenario,
     LgFrLastProblemSolved & /* lps */ ,      // testScenario's
     LgFrSetOfParts        & /* sop */ ,      // testScenario's
     LgFrSetOfDemands      & /* sod */  )     // testScenario's
{
  // Test constructor
  LgFrFocussedShortageScheduleScheduleStrategyForATP cs1(testScenario);
  assert( cs1.id_ == __LGFRFOCUSSEDSHORTAGESCHEDULESCHEDULESTRATEGYFORATP );

  // Test isA method
  assert( cs1.isA() == __LGFRFOCUSSEDSHORTAGESCHEDULESCHEDULESTRATEGYFORATP );

  // Test copy constructor
  LgFrFocussedShortageScheduleScheduleStrategyForATP cs2(cs1);
  assert( cs2.isA() == __LGFRFOCUSSEDSHORTAGESCHEDULESCHEDULESTRATEGYFORATP );

  // Test assignment operator
//  LgFrFocussedsedShortageScheduleScheduleStrategyForATP cs3;
//  cs3 = cs2;
//  assert( cs3.isA() == cs2.isA() );

  {
    // Test clone method
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( cs4Ptr->isA() == cs1.isA() );

    // Test destructor
    delete cs4Ptr;
  }
  
  // Set a focus horizon
//  testScenario->focussedShortageScheduleControl().
//    focusHorizon( *sod.findValue("Michael_Faraday","Screen_L"), 1 );

  // Test populate due to new implosion solution.
  LgFrSortingPartScheduleFloat schedule;
  const LgFrSortingPartScheduleFloat & constSchedule = schedule;
  schedule.attributeScheduleStrategy( cs1 );

/*
  // this should not populate
  cout << "In State Init and should not populate schedule" << endl;
  testScenario->changeState(STATE_INIT);
  testScenario->newImplosionSolution();
  cout <<endl <<"LgFrFocussedsedShortageScheduleScheduleStrategy" <<endl
       <<schedule.format() <<endl;
*/

  // couldn't get this to work

/*  
  cout << "In State Committed Imploded Recovery and should populate schedule" << endl;
  testScenario->changeState(LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY);  
  cout <<endl <<"LgFrFocussedsedShortageScheduleScheduleStrategy" <<endl
       <<schedule.format() <<endl;
  
  for ( i=0; i<sop.entries(); i++ ) {
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

  schedule1.populate();  
  //  cout <<endl 
  //       <<"LgFrFocussedsedShortageScheduleScheduleStrategy with zero filter" 
  //       <<endl
  //       <<schedule1.format() <<endl;

  assert( schedule1.entries() == schedule.entries() );
  for ( i=0; i<schedule1.entries(); i++ ) {
    assert(  !(constSchedule1[i].timeVecFloat() == 0.0)  );
    assert(  constSchedule1[i].timeVecFloat() == 
             constSchedule[i].timeVecFloat()  );
  }

  */  
}
