#include <iostream.h>
#include <math.h>

#include <scenario/src/supSSATP.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/scenATP.h>
#include <scenario/src/machdep.h>

// Populate Schedule with itemTimeVecs
void
LgFrSupplyScheduleStrategyForATP::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  LgFrPart part;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat itv( part, tv );

  // Loop once for each part (in setOfParts)
  const LgFrSortingPartScheduleFloat* masterSupplySchedulePtr =
    scenario()->masterPartVolumes();
  size_t scheduleSize = setOfParts().entries();
  for ( size_t i=0; i < scheduleSize; i++ ) {

    // setOfParts() is either raw, product, or capacity.
    //   this is selected by the user
    part = setOfParts()[i];
    assert( masterSupplySchedulePtr->contains( part ) );

     // Get itemTimeVec from master supply schedule
     itv = (*masterSupplySchedulePtr)[ part ];
    
     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
   }
}
 

// Update (propogate) a supply change to wip and to the master schedule
void 
LgFrSupplyScheduleStrategyForATP::update(
          const LgFrItemTimeVecPairFloat & newITV)
{
  
  // update master volumes schedule
  LgFrSortingPartScheduleFloat* supplySchedule
    = scenario()->masterPartVolumes();
  supplySchedule->insertKeyAndValue(newITV.part(), newITV.timeVecFloat());

  // check the state machine to see if we can do the update
  if (currentState() == LGFR_ATP_STATE_COMMITTED_IMPLODED ||
      currentState() == LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY)
    wipPtr_->timeVec(newITV.part(), newITV.timeVecFloat());
  
  // send token to state machine
  sendToken(LGFR_ATP_TOKEN_SUPPLY_CHANGED);
  
  // Inform dependent objects that the supply volume has changed.
  // (We don't have to do this anymore as master volumes schedule
  // will do it for us via the updateFrom method.)
}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrSupplyScheduleStrategyForATP::clone()
const
{
  LgFrSupplyScheduleStrategyForATP* newInst = 
      new LgFrSupplyScheduleStrategyForATP(*this);
  return (newInst);
}

// Make clone copy of object with a new scenario
LgFrAttributeScheduleStrategy *
LgFrSupplyScheduleStrategyForATP::clone(LgFrScenarioSmartPointer newScenario)
const
{
    return new LgFrSupplyScheduleStrategyForATP(*this, newScenario);
}

// Make clone copy of object with a new scenario
LgFrAttributeScheduleStrategy *
LgFrSupplyScheduleStrategyForATP::clone(LgFrScenario& newScenario)
const
{
    cerr << "should not call this method" << endl;
    return (LgFrAttributeScheduleStrategy *)NULL;
}


// Return comparison strategy identifier
isAReturnType
LgFrSupplyScheduleStrategyForATP::isA()
const
{
   return id_;
}

// Assignment operator
LgFrSupplyScheduleStrategyForATP &
LgFrSupplyScheduleStrategyForATP::operator=(
	  const LgFrSupplyScheduleStrategyForATP& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrSupplyScheduleStrategy::operator= ) (rhs);
    scenario(rhs.scenario());
  // register ASS with the supply volume schedule
  scenario()->masterPartVolumes()->addDependent(this);    
  }
  return *this;
}


// Copy constructor
LgFrSupplyScheduleStrategyForATP::
   LgFrSupplyScheduleStrategyForATP (
      const LgFrSupplyScheduleStrategyForATP & source)
:  LgFrSupplyScheduleStrategy( source ), 
   LgFrScheduleMixInForATP( source.scenario(), source.desiredState() ),
   id_(__LGFRSUPPLYSCHEDULESTRATEGYFORATP)
{
  assert( id_ == source.id_ );
  // register ASS with the supply volume schedule
  scenario()->masterPartVolumes()->addDependent(this);    
}

// Copy constructor with newScenario
LgFrSupplyScheduleStrategyForATP::
   LgFrSupplyScheduleStrategyForATP (
      const LgFrSupplyScheduleStrategyForATP & source,
      LgFrScenarioSmartPointer newScenario)
:  LgFrSupplyScheduleStrategy( 
         source, 
         *newScenario ),
   LgFrScheduleMixInForATP( newScenario, source.desiredState() ),
   id_(__LGFRSUPPLYSCHEDULESTRATEGYFORATP)
{
  assert( id_ == source.id_ );
  // register ASS with the supply volume schedule
  scenario()->masterPartVolumes()->addDependent(this);    
}

// Constructor
LgFrSupplyScheduleStrategyForATP::
   LgFrSupplyScheduleStrategyForATP (
     LgFrScenarioSmartPointer scenario, int desiredState )
:  LgFrScheduleMixInForATP(scenario, desiredState),
   LgFrSupplyScheduleStrategy(*scenario),
   id_(__LGFRSUPPLYSCHEDULESTRATEGYFORATP)
{
  // register ASS with the supply volume schedule
  ((LgFrScenarioForATP&)(*scenario)).masterPartVolumes()->addDependent(this);
}

// Default Constructor
// This is a private member function.
LgFrSupplyScheduleStrategyForATP::
   LgFrSupplyScheduleStrategyForATP ()
:  LgFrSupplyScheduleStrategy(),
   LgFrScheduleMixInForATP(),
   id_(__LGFRSUPPLYSCHEDULESTRATEGYFORATP)
{
}

// Destructor
LgFrSupplyScheduleStrategyForATP::
   ~LgFrSupplyScheduleStrategyForATP ()
{
  // remove subscription to the supply volume schedule
  scenario()->masterPartVolumes()->removeDependent(this);      
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrSupplyScheduleStrategyForATP methods.
void
LgFrSupplyScheduleStrategyForATP::test()
{
}


// test LgFrSupplyScheduleStrategyForATP methods.
// Expects data for both testScenario and testScenario2 to come from
// /u/implode/wit/data/examples/diner12/wit.data, but expects them to
// be two different scenarios.
void
LgFrSupplyScheduleStrategyForATP::contextTest
(LgFrScenarioForATPSmartPointer  testScenario,
     LgFrWorkingImplosionProblem & wip,            // from testScenario
     LgFrSetOfParts              & sop,            // from testScenario
 LgFrScenarioForATPSmartPointer              /* testScenario2 */ )
{
  int i;
  float fltEps = .0001f;

  // Test constructor
  LgFrSupplyScheduleStrategyForATP supplySS1(testScenario);
  assert( supplySS1.id_ == __LGFRSUPPLYSCHEDULESTRATEGYFORATP );

  // Test isA method
  assert( supplySS1.isA() == __LGFRSUPPLYSCHEDULESTRATEGYFORATP );

/*
  // Test copy constructor
  LgFrSupplyScheduleStrategyForATP supplySS1copy(supplySS1);
  assert( supplySS1copy.isA() == __LGFRSUPPLYSCHEDULESTRATEGYFORATP );
*/

/*
  // Test copy constructor with another scenario
  LgFrSupplyScheduleStrategyForATP supplySS1copy2(supplySS1, testScenario2);
  assert( supplySS1copy2.isA() == __LGFRSUPPLYSCHEDULESTRATEGYFORATP );
*/  

/*
  // Test assignment operator
  {
    LgFrSupplyScheduleStrategyForATP supplySS3;
    supplySS3 = supplySS1copy;
    assert( supplySS3.isA() == supplySS1copy.isA() );
  }

  // Test clone method
  {
    LgFrAttributeScheduleStrategy *supplySS1ClonePtr = supplySS1.clone();
    assert( supplySS1ClonePtr->isA() == supplySS1.isA() );

    // Test destructor
    delete supplySS1ClonePtr;
  }
*/

  // Test populate
  LgFrSortingPartScheduleFloat schedule;
  schedule.attributeScheduleStrategy(supplySS1);
  const  LgFrSortingPartScheduleFloat & constSchedule = schedule;
  // only 1 set of parts now 
  supplySS1.setOfParts(supplySS1.scenario()->setOfParts(LGFR_DEFAULT_SET));
  schedule.populate();
//   cout << endl << "LgFrSupplyScheduleStrategyForATP using default set of parts"
//        << endl << schedule.format() <<endl;

  // the sop and the parts initially in the schedule are the same.  it doesn't
  //  always have to be this way
  for ( i=0; i< schedule.entries(); i++ ) {
     assert( sop[i] == constSchedule[i].part() );
     assert( wip.timeVec(sop[i]) == constSchedule[i].timeVecFloat() );
  }

  // check update
  LgFrPart aPart(sop[0]);
  int vectorLength = constSchedule[0].timeVecFloat().length();
  LgFrItemTimeVecPairFloat itv1(aPart, LgFrTimeVecFloat(vectorLength, 0.0));
  supplySS1.update(itv1);
  cout << "finished populate with zero filter" << endl;
                                 
  // Test populate with filter
  LgFrZeroTimeVecFilterStrategy zeroFilter;
  schedule.filterStrategy( zeroFilter);

  schedule.populate();
  cout << endl << "LgFrSupplyScheduleStrategyForATP with zero filter" << endl
      << schedule.format() << endl << "************ end of schedule ************" << endl;

  for ( i=0; i< schedule.entries(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }


  // Test update()
  // put some items back into the schedule
  LgFrItemTimeVecPairFloat itv2(aPart, LgFrTimeVecFloat(vectorLength, 5.0));
  supplySS1.update(itv2);
  schedule.populate();
  LgFrItemTimeVecPairFloat itv3(aPart, constSchedule[0].timeVecFloat() + 2.5);

  supplySS1.desiredState(1);  // don't update
  supplySS1.update(itv3);
  supplySS1.desiredState(2);  // don't update
  supplySS1.update(itv3);
  schedule.populate();
  cout << endl << "LgFrSupplyScheduleStrategyForATP after update" << endl
      << schedule.format() << endl;                                
  assert(itv3.timeVecFloat() == constSchedule[0].timeVecFloat());

/*  
  // test clone(newScenario) -- as follows: compare the data in constSchedule
  // with the data in another scenario
  // that was initialized with the same data testScenario was initialized with.
  // The data in constSchedule was just changed, in the code under the comment
  // Test update()

  LgFrSortingPartScheduleFloat schedule22; 
  const LgFrSortingPartScheduleFloat & constSchedule22 = schedule22;

  LgFrAttributeScheduleStrategy * cs22Ptr = supplySS1.clone(testScenario2);
  assert( cs22Ptr->isA() == supplySS1.isA());  // just checks to make sure
				// that these are the right class
  cs22Ptr->setOfParts(cs22Ptr->.scenario()->
                         setOfParts(LGFR_DEFAULT_SET));
  schedule22.attributeScheduleStrategy(*cs22Ptr);
                                
  schedule.populate();
  schedule22.populate();
  cout << endl << "schedule contains" << endl
      << schedule.format() << endl;                                
  cout << endl << "schedule22 contains" << endl
      << schedule22.format() << endl;                                

  // this works as we have added 2.5 to the items in  the timevec
  //   for supplySS1

  assert( fabs( (constSchedule[0].timeVecFloat())[0] - 2.5 -
               (constSchedule22[0].timeVecFloat())[0]) <= fltEps);

  // now increase the supply in Scenario2 by 3.5
  LgFrItemTimeVecPairFloat anITV2(aPart, 
                     constSchedule22[0].timeVecFloat() + 3.5);
  cs22Ptr->update(anITV2);
  schedule.populate();
  schedule22.populate();
  assert( fabs( (constSchedule22[0].timeVecFloat())[0] - 1.0 -
               (constSchedule[0].timeVecFloat())[0]) <= fltEps);

  delete cs22Ptr;
*/
}

