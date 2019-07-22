#include <iostream.h>
#include <math.h>

#include <scenario/src/demSSATP.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/scenATP.h>
#include <scenario/src/machdep.h>

// Populate Schedule with itemTimeVecs
void
LgFrDemandVolumeScheduleStrategyForATP::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  LgFrDemand demand;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat itv( demand, tv );

  const LgFrSortingDemandScheduleFloat* masterDemandSchedulePtr
     = scenario()->masterDemandVolumes();

  // Loop once for demand (in setOfDemands)
  int numberOfDemands = setOfDemands().entries();
  for ( size_t i = 0; i < numberOfDemands; i++ ) {

    // setOfDemands() is either committed, new opportunities, or
    //   firm forecast.  this is selected by the user
    demand = setOfDemands()[i];
    assert( masterDemandSchedulePtr->contains( demand ) );

     // Get itemTimeVec from master schedule
     itv = (*masterDemandSchedulePtr)[ demand ];

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
  }
}
 

// Update (propogate) a demand change to wip and to the master schedule
void 
LgFrDemandVolumeScheduleStrategyForATP::update(
          const LgFrItemTimeVecPairFloat & newITV)
{
  // update master volumes schedule
  LgFrSortingDemandScheduleFloat* demandSchedule = scenario()->masterDemandVolumes();
  demandSchedule->insertKeyAndValue(newITV.demand(), newITV.timeVecFloat());
  
/*
 *  the stuff below is for the general case where the user is allowed
 *    to change any type of demand
 */
  
 LgFrScenarioForATPSmartPointer atpScenario = scenario();
 LgFrSetOfDemands committedDemands =
   atpScenario->setOfDemands(LGFR_COMMITTED_DEMANDS);
  if (committedDemands.contains(newITV.demand())) {
    cout << "Committed demand changed" << endl;
    if (
        currentState() == LGFR_ATP_STATE_COMMITTED_IMPLODED           ||
        currentState() == LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY
       )
      wipPtr_->timeVec(newITV.demand(), newITV.timeVecFloat());        
    sendToken(LGFR_ATP_TOKEN_COMMITTED_DEMANDS_CHANGED);
  }
  else {
    LgFrSetOfDemands newopportunitiesDemands =
      atpScenario->setOfDemands(LGFR_NEWOPPOR_DEMANDS);
    if (newopportunitiesDemands.contains(newITV.demand()))  {
      cout << "New Opportunities demand changed" << endl;
      if (currentState() == LGFR_ATP_STATE_NEWOPP_IMPLODED)
        wipPtr_->timeVec(newITV.demand(), newITV.timeVecFloat());        
      sendToken(LGFR_ATP_TOKEN_NEWOPP_DEMANDS_CHANGED);
    }
    else  {
      LgFrSetOfDemands firmforecastDemands =
        atpScenario->setOfDemands(LGFR_FIRMFORECAST_DEMANDS);
      if (firmforecastDemands.contains(newITV.demand())) {
        cout << "Firm Forecast demand changed" << endl;
        if (currentState() == LGFR_ATP_STATE_ATP_IMPLODED)
          wipPtr_->timeVec(newITV.demand(), newITV.timeVecFloat());        
        sendToken(LGFR_ATP_TOKEN_ATP_DEMANDS_CHANGED);
      }
    }
  }
 
  // inform dependent objects that the demandVol has changed. 
  //  we don't have to do this anymore as wip (through
  //  LgFrUserInputModification) will do it for us
  //  via the updateFrom method.

}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrDemandVolumeScheduleStrategyForATP::clone()
const
{
  LgFrDemandVolumeScheduleStrategyForATP* newInst = 
      new LgFrDemandVolumeScheduleStrategyForATP(*this);
  return (newInst);
}

// Make clone copy of object with a new scenario
LgFrAttributeScheduleStrategy *
LgFrDemandVolumeScheduleStrategyForATP::clone(LgFrScenarioSmartPointer newScenario)
const
{
  LgFrDemandVolumeScheduleStrategyForATP* newInst = 
      new LgFrDemandVolumeScheduleStrategyForATP(*this, newScenario);
  return (newInst);
}


// Make clone copy of object with a new scenario
LgFrAttributeScheduleStrategy *
LgFrDemandVolumeScheduleStrategyForATP::clone(LgFrScenario& )
const
{
  cerr << "ERROR in " << __FILE__ << " at line " << __LINE__ << '\n';
  cerr << "should not use " <<
    "LgFrDemandVolumeScheduleStrategyForATP::clone(LgFrScenario&)" << endl;
  return (LgFrAttributeScheduleStrategy *)NULL;
}


// Return comparison strategy identifier
isAReturnType
LgFrDemandVolumeScheduleStrategyForATP::isA()
const
{
   return id_;
}

// Assignment operator
LgFrDemandVolumeScheduleStrategyForATP &
LgFrDemandVolumeScheduleStrategyForATP::operator=(
	  const LgFrDemandVolumeScheduleStrategyForATP& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrDemandVolumeScheduleStrategy::operator= ) (rhs);
    scenario(rhs.scenario());    
    // register ASS with the demand volume schedule
    scenario()->masterDemandVolumes()->addDependent(this);    
  }
  
  return *this;
}


// Copy constructor
LgFrDemandVolumeScheduleStrategyForATP::
   LgFrDemandVolumeScheduleStrategyForATP (
      const LgFrDemandVolumeScheduleStrategyForATP & source)
:  LgFrDemandVolumeScheduleStrategy( source ), 
   LgFrScheduleMixInForATP( source.scenario(), source.desiredState() ),
   id_(__LGFRDEMANDVOLUMESCHEDULESTRATEGYFORATP)
{
  assert( id_ == source.id_ );
  // register ASS with the demand volume schedule
  scenario()->masterDemandVolumes()->addDependent(this);    
}

// Copy constructor with newScenario
LgFrDemandVolumeScheduleStrategyForATP::
   LgFrDemandVolumeScheduleStrategyForATP (
      const LgFrDemandVolumeScheduleStrategyForATP & source,
      LgFrScenarioSmartPointer newScenario)
:  LgFrDemandVolumeScheduleStrategy(source, *newScenario ),
   LgFrScheduleMixInForATP( newScenario, source.desiredState() ),
   id_(__LGFRDEMANDVOLUMESCHEDULESTRATEGYFORATP)
{
  assert( id_ == source.id_ );
  // register ASS with the demand volume schedule
  scenario()->masterDemandVolumes()->addDependent(this);    
}

// Constructor
LgFrDemandVolumeScheduleStrategyForATP::
   LgFrDemandVolumeScheduleStrategyForATP (
     LgFrScenarioSmartPointer scenario, int desiredState )
:  LgFrScheduleMixInForATP(scenario, desiredState),
   LgFrDemandVolumeScheduleStrategy(*scenario),
   id_(__LGFRDEMANDVOLUMESCHEDULESTRATEGYFORATP)
{
  // register ASS with the demand volume schedule
  ((LgFrScenarioForATP &)(*scenario)).masterDemandVolumes()->addDependent(this);    
}

// Default Constructor
// This is a private member function.
LgFrDemandVolumeScheduleStrategyForATP::
   LgFrDemandVolumeScheduleStrategyForATP ()
:  LgFrDemandVolumeScheduleStrategy(),
   LgFrScheduleMixInForATP(),
   id_(__LGFRDEMANDVOLUMESCHEDULESTRATEGYFORATP)
{
}

// Destructor
LgFrDemandVolumeScheduleStrategyForATP::
   ~LgFrDemandVolumeScheduleStrategyForATP ()
{
  // unregister ASS with the demand volume schedule
  scenario()->masterDemandVolumes()->removeDependent(this);    
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrDemandVolumeScheduleStrategyForATP methods.
void
LgFrDemandVolumeScheduleStrategyForATP::test()
{
}


// test LgFrDemandVolumeScheduleStrategyForATP methods.
// Expects data for both testScenario and testScenario2 to come from
// /u/implode/wit/data/examples/diner12/wit.data, but expects them to
// be two different scenarios.
void
LgFrDemandVolumeScheduleStrategyForATP::contextTest
(LgFrScenarioForATPSmartPointer   testScenario,
     LgFrWorkingImplosionProblem & ,               // from testScenario
     LgFrSetOfDemands            & sod,            // from testScenario
     LgFrScenarioForATPSmartPointer )
{
  int i;
  float fltEps = .0001f;

  // Test constructor
  LgFrDemandVolumeScheduleStrategyForATP demandSS1(testScenario);
  assert( demandSS1.id_ == __LGFRDEMANDVOLUMESCHEDULESTRATEGYFORATP );

  // Test isA method
  assert( demandSS1.isA() == __LGFRDEMANDVOLUMESCHEDULESTRATEGYFORATP );

  // Test copy constructor
  LgFrDemandVolumeScheduleStrategyForATP demandSS1copy(demandSS1);
  assert( demandSS1copy.isA() == __LGFRDEMANDVOLUMESCHEDULESTRATEGYFORATP );

/*  
  // Test copy constructor with another scenario
  LgFrDemandVolumeScheduleStrategyForATP demandSS1copy2(demandSS1, testScenario2);
  assert( demandSS1copy2.isA() == __LGFRDEMANDVOLUMESCHEDULESTRATEGYFORATP );
*/
  
  // Test assignment operator
  {
    LgFrDemandVolumeScheduleStrategyForATP demandSS3;
    demandSS3 = demandSS1copy;
    assert( demandSS3.isA() == demandSS1copy.isA() );
  }

  // Test clone method
  LgFrAttributeScheduleStrategy *demandSS1ClonePtr = demandSS1.clone();
  assert( demandSS1ClonePtr->isA() == demandSS1.isA() );

  // Test destructor
  delete demandSS1ClonePtr;


  // Test populate
  LgFrSortingDemandScheduleFloat schedule;
  schedule.attributeScheduleStrategy(demandSS1);
  const  LgFrSortingDemandScheduleFloat & constSchedule = schedule;
  schedule.populate();
  cout << endl << "All Demands for ATP" << endl
       << schedule.format() <<endl;
  
  // check out committed demands
  demandSS1.setOfDemands(demandSS1.scenario()->
                         setOfDemands(LGFR_COMMITTED_DEMANDS));
  schedule.populate();
  cout << endl << "Committed Demands for ATP" << endl
       << schedule.format() <<endl;
  

    // check out new opportunity demands
  demandSS1.setOfDemands(demandSS1.scenario()->
                         setOfDemands(LGFR_NEWOPPOR_DEMANDS));
  schedule.populate();
  cout << endl << "New Opportunities Demands for ATP" << endl
       << schedule.format() <<endl;
  

    // check out firm forecast demands
  demandSS1.setOfDemands(demandSS1.scenario()->
                         setOfDemands(LGFR_FIRMFORECAST_DEMANDS));
  schedule.populate();
  cout << endl << "Firm Forecast Demands for ATP" << endl
       << schedule.format() <<endl;
  

  // the sod and the demands initially in the schedule are the same.  it doesn't
  //  always have to be this way
  cout << endl << "Set of demands are " << endl;
  for (i = 0; i < sod.entries(); i++) {
    (sod[i]).print();
    cout << endl;
  }
  cout << "done printing out set of demands" << endl;

  // Test populate with filter. Should produce an empty schedule
  LgFrZeroTimeVecFilterStrategy zeroFilter;
  schedule.filterStrategy( zeroFilter );
  schedule.populate();
  cout << endl << "LgFrDemandVolumeScheduleStrategyForATP with zero filter" << endl
      << schedule.format() << endl << "************ end of schedule ************" << endl;

  for ( i=0; i< schedule.entries(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }


  // Test update()
  // put some items back into the schedule
  // committed demand
  demandSS1.setOfDemands(demandSS1.scenario()->
                         setOfDemands(LGFR_COMMITTED_DEMANDS));
  schedule.populate();
  LgFrDemand aDemand(sod[0]);
  LgFrItemTimeVecPairFloat itv3(aDemand, constSchedule[0].timeVecFloat() + 2.5);
  demandSS1.desiredState(1);  // don't update
  demandSS1.update(itv3);
  cout << endl << "LgFrDemandVolumeScheduleStrategyForATP Committed Demand" << endl
       << schedule.format() <<endl;

  
  demandSS1.desiredState(2);  // update
  demandSS1.update(itv3);
  schedule.populate();
  cout << endl << "LgFrDemandVolumeScheduleStrategyForATP Committed Demand after update" << endl
      << schedule.format() << endl;                                
  assert(itv3.timeVecFloat() == constSchedule[0].timeVecFloat());
  
/*  can't use the clone method properly so we don't need to test it  
  // test clone(newScenario) -- as follows: compare the data in constSchedule
  // with the data in another scenario
  // that was initialized with the same data testScenario was initialized with.
  // The data in constSchedule was just changed, in the code under the comment
  // Test update()


  LgFrSortingDemandScheduleFloat schedule22; 
  const LgFrSortingDemandScheduleFloat & constSchedule22 = schedule22;

  LgFrAttributeScheduleStrategy * cs22Ptr = demandSS1.clone(testScenario2);
  assert( cs22Ptr->isA() == demandSS1.isA());  // just checks to make sure
				// that these are the right class
  cs22Ptr->setOfDemands(cs22Ptr->.scenario()->
                         setOfDemands(LGFR_COMMITTED_DEMANDS));
  schedule22.attributeScheduleStrategy(*cs22Ptr);
  schedule.populate();
  schdeule22.populate();
  cout << endl << "schedule contains" << endl
      << schedule.format() << endl;                                
  cout << endl << "schedule22 contains" << endl
      << schedule22.format() << endl;                                

  // this works as we have added 2.5 to the items in  the timevec
  //   for demandSS1

  assert( fabs( (constSchedule[0].timeVecFloat())[0] - 2.5 -
               (constSchedule22[0].timeVecFloat())[0]) <= fltEps);

  // now increase the demand in Scenario2 by 3.5
  LgFrItemTimeVecPairFloat anITV2(aDemand, 
                     constSchedule22[0].timeVecFloat() + 3.5);
  cs22Ptr->update(anITV2);
  schedule.populate();
  schedule22.populate();
  assert( fabs( (constSchedule22[0].timeVecFloat())[0] - 1.0 -
               (constSchedule[0].timeVecFloat())[0]) <= fltEps);

  delete cs22Ptr;
*/
  
}




