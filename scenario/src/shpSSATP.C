#include <iostream.h>

#include <scenario/src/shpSSATP.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/scenATP.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>

// Override base class populate method.  Explicitly invoke base
// class populate method if current state contains data for this class.
void
LgFrShipmentScheduleStrategyForATP::populate(
   LgFrSortingScheduleFloat& sched )
const
{
  // only populate schedule if the state machine is in the state
  //  that this ASS really corresponds to
  if (scenario()->currentState() == desiredState())
     LgFrAttributeScheduleStrategy::populate( sched );
}



// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrShipmentScheduleStrategyForATP::clone()
const
{
  LgFrShipmentScheduleStrategyForATP* newInst = 
      new LgFrShipmentScheduleStrategyForATP(*this);
  return (newInst);
}

// Make clone copy of object with newScenario
LgFrAttributeScheduleStrategy *
LgFrShipmentScheduleStrategyForATP::clone(LgFrScenarioSmartPointer newScenario)
const
{ 
    return new LgFrShipmentScheduleStrategyForATP(*this, newScenario);
}


LgFrAttributeScheduleStrategy *
LgFrShipmentScheduleStrategyForATP::clone(LgFrScenario& newScenario)
const
{ 
    cerr << "should not use this method" << endl;
    exit(-1);
    return (LgFrAttributeScheduleStrategy *) NULL; // never gets here
}


// Return schedule strategy identifier
isAReturnType
LgFrShipmentScheduleStrategyForATP::isA()
const
{
   return id_;
}

// Assignment operator
LgFrShipmentScheduleStrategyForATP &
LgFrShipmentScheduleStrategyForATP::operator=(
	  const LgFrShipmentScheduleStrategyForATP& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrShipmentScheduleStrategy::operator= ) (rhs);
    scenario(rhs.scenario());
  }
  return *this;
}


// Copy constructor
LgFrShipmentScheduleStrategyForATP::
   LgFrShipmentScheduleStrategyForATP (
      const LgFrShipmentScheduleStrategyForATP & source)
:  LgFrShipmentScheduleStrategy( source ),
   LgFrScheduleMixInForATP( source.scenario(), source.desiredState() ),
   id_(__LGFRSHIPMENTSCHEDULESTRATEGYFORATP)
{
  // Nothing to do here
}

// Copy constructor with newScenario
LgFrShipmentScheduleStrategyForATP::
   LgFrShipmentScheduleStrategyForATP (
      const LgFrShipmentScheduleStrategyForATP & source,
      LgFrScenarioSmartPointer newScenario)
:  LgFrShipmentScheduleStrategy(source, *newScenario ),
   LgFrScheduleMixInForATP( newScenario, source.desiredState() ),
   id_(__LGFRSHIPMENTSCHEDULESTRATEGYFORATP)
{
  assert( id_ == source.id_);
}

// Constructor
LgFrShipmentScheduleStrategyForATP::
   LgFrShipmentScheduleStrategyForATP (
     LgFrScenarioSmartPointer scenario, int desiredState )
:  LgFrShipmentScheduleStrategy(*scenario),
   LgFrScheduleMixInForATP(scenario, desiredState),
   id_(__LGFRSHIPMENTSCHEDULESTRATEGYFORATP)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrShipmentScheduleStrategyForATP::
   LgFrShipmentScheduleStrategyForATP ()
:  LgFrShipmentScheduleStrategy(),
   LgFrScheduleMixInForATP(),
   id_(__LGFRSHIPMENTSCHEDULESTRATEGYFORATP)
{
  // Nothing to do here
}

// Destructor
LgFrShipmentScheduleStrategyForATP::
   ~LgFrShipmentScheduleStrategyForATP ()
{
  // Nothing to do here
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrShipmentScheduleStrategyForATP methods.
void
LgFrShipmentScheduleStrategyForATP::test()
{
}

// test LgFrShipmentScheduleStrategyForATP methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrShipmentScheduleStrategyForATP::contextTest
  (
   LgFrScenarioForATPSmartPointer testScenario,
   LgFrLastProblemSolved	& lps,			// testScenario's 
   LgFrSetOfDemands		& sod)			// testScenario's 
{
  int i;

  // Test constructor
  LgFrShipmentScheduleStrategyForATP cs1(testScenario);
  assert( cs1.id_ == __LGFRSHIPMENTSCHEDULESTRATEGYFORATP );

  // Test isA method
  assert( cs1.isA() == __LGFRSHIPMENTSCHEDULESTRATEGYFORATP );

  // Test copy constructor
  LgFrShipmentScheduleStrategyForATP cs2(cs1);
  assert( cs2.isA() == __LGFRSHIPMENTSCHEDULESTRATEGYFORATP );

  // Test assignment operator
  LgFrShipmentScheduleStrategyForATP cs3;
  cs3 = cs2;
  assert( cs3.isA() == cs2.isA() );

  {
    // Test clone method
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( cs4Ptr->isA() == cs1.isA() );

    // Test destructor
    delete cs4Ptr;
  }

  // Test populate.  Should be populated with zero timeVec because
  // a Shipment schedule does not exist.
  // play with state machine to get into State 0
  cs1.scenario()->changeState(0);
  cs1.setOfDemands(cs1.scenario()->
                         setOfDemands(LGFR_COMMITTED_DEMANDS));
  cs1.desiredState(0);  
  LgFrSortingDemandScheduleFloat schedule;
  schedule.attributeScheduleStrategy(cs1);
  const LgFrSortingDemandScheduleFloat & constSchedule = schedule;


  schedule.populate();
  cout << "Committed schedule contains : " << schedule.format() << endl;
  cs1.setOfDemands(cs1.scenario()->
                         setOfDemands(LGFR_NEWOPPOR_DEMANDS));
  schedule.populate();
  cout << "New opportunities schedule contains : " << schedule.format() << endl;  
  cs1.setOfDemands(cs1.scenario()->
                         setOfDemands(LGFR_DEFAULT_SET));
  schedule.populate();
  cout << "Default schedule contains : " << schedule.format() << endl;  

  cout << endl << "Default Set of demands are " << endl;
  for (i = 0; i < sod.entries(); i++) {
    (sod[i]).print();
    cout << endl;
  }
  
  for ( i=0; i<constSchedule.entries(); i++ ) {
     assert( constSchedule[i].timeVecFloat() == 0.0 );
  }

  // Test popluate with zeroFilter.  Should produce an empty schedule
  LgFrZeroTimeVecFilterStrategy zeroFilter;
  schedule.filterStrategy( zeroFilter );
  schedule.populate();
  assert( schedule.entries()==0 );

  // Test populate, due to new implosion solution.
  {
  LgFrFilterStrategy defaultFilter;
  schedule.filterStrategy( defaultFilter );
  }
  schedule.attributeScheduleStrategy( cs1 );
  testScenario->newImplosionSolution();
  schedule.populate();
  cout <<endl <<"LgFrShipmentScheduleStrategyForATP" <<endl
       <<schedule.format() <<endl;

  for ( i=0; i<sod.entries(); i++ ) {
     assert(  schedule.contains( sod[i] )  );
     assert( lps.shipmentVolume(&sod[i]) == 
                constSchedule[sod[i]].timeVecFloat() );
  }

  // Test populate with filter
  schedule.filterStrategy( zeroFilter);
  schedule.attributeScheduleStrategy( cs1 );
  testScenario->newImplosionSolution();

  schedule.populate();
  // cout <<endl <<"LgFrShipmentScheduleStrategyForATP with zero filter" <<endl
  //      <<schedule.format() <<endl;

  for ( i=0; i<schedule.entries(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }
}

