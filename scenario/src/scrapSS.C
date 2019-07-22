// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/scrapSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/scenario.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>
#include <scenario/src/partSchP.h>



// Populate Schedule with itemTimeVecs
void
LgFrScrapScheduleStrategy::kernalOfPopulate(
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

     // Make itemTimeVec
     itv.item( part );
     itv.timeVecFloat(
           implodePtr_->lastProblemSolved().scrapVolume( &part )   );

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );

     
  }

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrScrapScheduleStrategy::clone()
const
{
  LgFrScrapScheduleStrategy* newInst = 
      new LgFrScrapScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrAttributeScheduleStrategy *
LgFrScrapScheduleStrategy::clone(LgFrScenario& newScenario)
const
{
  LgFrScrapScheduleStrategy* newInst = 
      new LgFrScrapScheduleStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrScrapScheduleStrategy &
LgFrScrapScheduleStrategy::operator=(
	  const LgFrScrapScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleStrategy::operator= ) (rhs);

    implodePtr_ = rhs.implodePtr_;
  }
  return *this;
}


// Copy constructor
LgFrScrapScheduleStrategy::
   LgFrScrapScheduleStrategy (
      const LgFrScrapScheduleStrategy & source)
:  LgFrPartScheduleStrategy( source ), 
   implodePtr_( source.implodePtr_ ),
   id_(__LGFRSCRAPSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Copy constructor with new Scenario
LgFrScrapScheduleStrategy::
   LgFrScrapScheduleStrategy (
      const LgFrScrapScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleStrategy( 
         source, 
         newScenario ),
   implodePtr_( &(implode(newScenario)) ),
   id_(__LGFRSCRAPSCHEDULESTRATEGY)
{
  // Nothing to do here
}


// Constructor
LgFrScrapScheduleStrategy::
   LgFrScrapScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy( 
       LgFrAttributeScheduleStrategy::setOfParts(scenario), 
      &(implode(scenario)) ),
   implodePtr_( &(implode(scenario)) ),
   id_(__LGFRSCRAPSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrScrapScheduleStrategy::
   LgFrScrapScheduleStrategy ()
:  LgFrPartScheduleStrategy( LgFrSetOfPartsWithMovableParts(), 0 ),
   implodePtr_( 0 ),
   id_(__LGFRSCRAPSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrScrapScheduleStrategy::
   ~LgFrScrapScheduleStrategy ()
{
  // Nothing to do here
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrScrapScheduleStrategy methods.
void
LgFrScrapScheduleStrategy::test()
{
}

// test LgFrScrapScheduleStrategy methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrScrapScheduleStrategy::contextTest
    (LgFrScenario          & testScenario,
     LgFrLastProblemSolved & lps,		// testScenario's 
     LgFrSetOfParts        & sop)		// testScenario's
{
  int i;

  // Test constructor
  LgFrScrapScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrScrapScheduleStrategy cs2(cs1);

  // Test assignment operator
  LgFrScrapScheduleStrategy cs3;
  cs3 = cs2;

  {
    // Test clone method
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();

    // Test destructor
    delete cs4Ptr;
  }

  // Test populate.  Should be populated with zero timeVec because
  // a Scrap schedule does not exist.
  LgFrSortingPartScheduleFloatSmartPointer schedule;
  const LgFrSortingPartScheduleFloat & constSchedule = *schedule;
  cs1.populate( *schedule );
  for ( i=0; i<sop.size(); i++ ) {
     assert(  schedule->contains( sop[i] )  );
     assert( constSchedule[sop[i]].timeVecFloat() == 0.0 );
  }

  // Test popluate with zeroFilter.  Should produce an empty schedule
  LgFrZeroTimeVecFilterStrategy zeroFilter;
  schedule->filterStrategy( zeroFilter );
  cs1.populate( *schedule );
  assert( schedule->size()==0 );

  // Test populate, due to new implosion solution.
  {
    LgFrFilterStrategy defaultFilter;
    schedule->filterStrategy( defaultFilter );
  }
  schedule->attributeScheduleStrategy( cs1 );
  testScenario.newImplosionSolution();
  // cs1.populate( *schedule );
  // cout <<endl <<"LgFrScrapScheduleStrategy" <<endl
  //      <<schedule->format() <<endl;

  for ( i=0; i<sop.size(); i++ ) {
     assert(  schedule->contains( sop[i] )  );
     assert( lps.scrapVolume(&sop[i]) == 
                constSchedule[sop[i]].timeVecFloat() );
  }

  // Test populate with filter
  schedule->filterStrategy( zeroFilter);
  schedule->attributeScheduleStrategy( cs1 );
  testScenario.newImplosionSolution();

  cs1.populate( *schedule );
  // cout <<endl <<"LgFrScrapScheduleStrategy with zero filter" <<endl
  //      <<schedule->format() <<endl;

  for ( i=0; i<schedule->size(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }
  
}
