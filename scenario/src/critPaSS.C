// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/critPaSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/scenario.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/machdep.h>
#include <scenario/src/partSchP.h>


// Populate Schedule with itemTimeVecs
void
LgFrCriticalPartScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  LgFrPart part, critPart;
  LgFrTimeVecFloat tv, critTimeVec;
  LgFrItemTimeVecPairFloat itv( part, tv );
  LgFrSortingPartScheduleFloat lpsCritSched;
  const LgFrSortingPartScheduleFloat & constLpsCritSched = lpsCritSched;
  
  // Generate complete critical part schedule
  int lenCritList;
  char ** critPartList;
  int * critPeriod = 0;
  implodePtr_->lastProblemSolved().criticalList( 
     &lenCritList,
     &critPartList,
     &critPeriod);

  // Generate unfiltered critical schedule
  int nPeriods( implodePtr_->lastProblemSolved().numberOfPeriods() );
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<lenCritList; i++ )
  {
     critPart = *(setOfParts().findValue(critPartList[i]));

     // Get part's timeVec if it is in the schedule
     if ( !lpsCritSched.findValue(critPart, critTimeVec) ) {
        // Part is not in schedule, so set timeVec to zero
        critTimeVec = LgFrTimeVecFloat(nPeriods,0.0);
     }

     // Update timeVec with period
     critTimeVec[ critPeriod[ i ] ] = (float) -lenCritList + i;

     // Update schedule with part and timeVec
     lpsCritSched.insertKeyAndValue( critPart, critTimeVec );
     portableFree( critPartList[i] );

  }
  portableFree( critPartList );
  portableFree( critPeriod );

  // Loop once for each critical part and apply filter
  size_t j = 0; // Pulled out of the for below by RW2STL
  for ( j=0; j<lpsCritSched.size(); j++ ) {

     // Make itemTimeVec
     itv = constLpsCritSched[j];

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
LgFrCriticalPartScheduleStrategy::clone()
const
{
  LgFrCriticalPartScheduleStrategy* newInst = 
      new LgFrCriticalPartScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrAttributeScheduleStrategy *
LgFrCriticalPartScheduleStrategy::clone(LgFrScenario& newScenario)
const
{
  LgFrCriticalPartScheduleStrategy* newInst = 
      new LgFrCriticalPartScheduleStrategy(*this, newScenario);
  return (newInst);

}

// Assignment operator
LgFrCriticalPartScheduleStrategy &
LgFrCriticalPartScheduleStrategy::operator=(
	  const LgFrCriticalPartScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleStrategy::operator= ) (rhs);

    implodePtr_ = rhs.implodePtr_;
  }
  return *this;
}


// Copy constructor
LgFrCriticalPartScheduleStrategy::
   LgFrCriticalPartScheduleStrategy (
      const LgFrCriticalPartScheduleStrategy & source)
:  LgFrPartScheduleStrategy( source ), 
   implodePtr_( source.implodePtr_ ),
   id_(__LGFRCRITICALPARTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Copy constructor with new Scenario
LgFrCriticalPartScheduleStrategy::
   LgFrCriticalPartScheduleStrategy (
      const LgFrCriticalPartScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleStrategy( 
         source, 
         newScenario ),
   implodePtr_( &(implode(newScenario)) ),
   id_(__LGFRCRITICALPARTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Constructor
LgFrCriticalPartScheduleStrategy::
   LgFrCriticalPartScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy( 
       LgFrAttributeScheduleStrategy::setOfParts(scenario),
      &(implode(scenario)) ),
   implodePtr_( &(implode(scenario)) ),
   id_(__LGFRCRITICALPARTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrCriticalPartScheduleStrategy::
   LgFrCriticalPartScheduleStrategy ()
:  LgFrPartScheduleStrategy( LgFrSetOfPartsWithMovableParts(), 0 ),
   implodePtr_( 0 ),
   id_(__LGFRCRITICALPARTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrCriticalPartScheduleStrategy::
   ~LgFrCriticalPartScheduleStrategy ()
{
  // Nothing to do here
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrCriticalPartScheduleStrategy methods.
void
LgFrCriticalPartScheduleStrategy::test()
{
}

// test LgFrCriticalPartScheduleStrategy methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrCriticalPartScheduleStrategy::contextTest(
                                              LgFrScenario          & testScenario,
                                              LgFrLastProblemSolved & lps)
{
  int i;

  // Test constructor
  LgFrCriticalPartScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrCriticalPartScheduleStrategy cs2(cs1);

  // Test assignment operator
  LgFrCriticalPartScheduleStrategy cs3;
  cs3 = cs2;

  {
    // Test clone method
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();

    // Test destructor
    delete cs4Ptr;
  }
  
  // Test populate via Model Client notification after new implode
  // solution.
  lps.computeCriticalList(true);
  LgFrSortingPartScheduleFloatSmartPointer schedule(new LgFrSortingPartScheduleFloat);
  const LgFrSortingPartScheduleFloat& constSchedule = *schedule;
  schedule->attributeScheduleStrategy(cs1);
  testScenario.newImplosionSolution(); 
  // cout <<endl <<"LgFrCriticalPartScheduleStrategy" <<endl
  //      <<schedule.format() <<endl;

  for ( i=0; i<schedule->size(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }

  // Test populate with filter
  LgFrZeroTimeVecFilterStrategy zeroFilter;
  schedule->filterStrategy( zeroFilter);
  
  cs1.populate( *schedule );
  // cout <<endl 
  //      <<"LgFrCriticalPartScheduleStrategy with zero filter" 
  //      <<endl
  //      <<schedule->format() <<endl;

  for ( i=0; i<schedule->size(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }
}

