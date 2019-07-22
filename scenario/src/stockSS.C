// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/stockSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/scenario.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>
#include <scenario/src/partSchP.h>



// Populate Schedule with itemTimeVecs
void
LgFrStockScheduleStrategy::kernalOfPopulate(
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

     if ( implodePtr_->lastProblemSolved().category( &part )  !=  
                LgFrCAPACITY ) {
        // Make itemTimeVec
        itv.item( part );
        itv.timeVecFloat(
              implodePtr_->lastProblemSolved().stockVolume( &part )   );

        // Check to see if i'th itemTimeVec is to be filtered out
        if ( schedule.filterStrategy().exclude( itv ) ) continue;

        // Insert itemTimeVec into schedule
        schedule.localInsert( itv );
     }
     
  }

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrStockScheduleStrategy::clone()
const
{
  LgFrStockScheduleStrategy* newInst = 
      new LgFrStockScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrAttributeScheduleStrategy *
LgFrStockScheduleStrategy::clone(LgFrScenario& newScenario)
const
{
  LgFrStockScheduleStrategy* newInst = 
      new LgFrStockScheduleStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrStockScheduleStrategy &
LgFrStockScheduleStrategy::operator=(
	  const LgFrStockScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleStrategy::operator= ) (rhs);

    implodePtr_ = rhs.implodePtr_;
  }
  return *this;
}


// Copy constructor
LgFrStockScheduleStrategy::
   LgFrStockScheduleStrategy (
      const LgFrStockScheduleStrategy & source)
:  LgFrPartScheduleStrategy( source ), 
   implodePtr_( source.implodePtr_ ),
   id_(__LGFRSTOCKSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Copy constructor with new Scenario
LgFrStockScheduleStrategy::
   LgFrStockScheduleStrategy (
      const LgFrStockScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleStrategy( 
         source, 
         newScenario ),
   implodePtr_( &(implode(newScenario)) ),
   id_(__LGFRSTOCKSCHEDULESTRATEGY)
{
  // Nothing to do here
}


// Constructor
LgFrStockScheduleStrategy::
   LgFrStockScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy( 
       LgFrAttributeScheduleStrategy::setOfParts(scenario), 
      &(implode(scenario)) ),
   implodePtr_( &(implode(scenario)) ),
   id_(__LGFRSTOCKSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrStockScheduleStrategy::
   LgFrStockScheduleStrategy ()
:  LgFrPartScheduleStrategy( LgFrSetOfPartsWithMovableParts(), 0 ),
   implodePtr_( 0 ),
   id_(__LGFRSTOCKSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrStockScheduleStrategy::
   ~LgFrStockScheduleStrategy ()
{
  // Nothing to do here
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrStockScheduleStrategy methods.
void
LgFrStockScheduleStrategy::test()
{
}

// test LgFrStockScheduleStrategy methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrStockScheduleStrategy::contextTest
    (LgFrScenario          & testScenario,
     LgFrLastProblemSolved & /* lps */ ,	// testScenario's 
     LgFrSetOfParts        & sop)		// testScenario's
{
  int i;

  // Test constructor
  LgFrStockScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrStockScheduleStrategy cs2(cs1);

  // Test assignment operator
  LgFrStockScheduleStrategy cs3;
  cs3 = cs2;

  {
    // Test clone method
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();

    // Test destructor
    delete cs4Ptr;
  }

  const LgFrPart * egg = sop.findValue("Egg");

  // Test populate.  Should be populated with zero timeVec because
  // a Stock schedule does not exist.
  LgFrSortingPartScheduleFloatSmartPointer schedule;
  const LgFrSortingPartScheduleFloat & constSchedule = *schedule;
  cs1.populate( *schedule );
  assert(  schedule->contains(*egg));
  assert((constSchedule[*egg].timeVecFloat())[0] - 5.0 < .0001 );
  assert((constSchedule[*egg].timeVecFloat())[1] - 4.0 < .0001 );


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
  // cout <<endl <<"LgFrStockScheduleStrategy" <<endl
  //      <<schedule->format() <<endl;


  assert(  schedule->contains(*egg));
  assert((constSchedule[*egg].timeVecFloat())[0] - 5.0 < .0001 );
  assert((constSchedule[*egg].timeVecFloat())[1] - 4.0 < .0001 );
  

  // Test populate with filter
  schedule->filterStrategy( zeroFilter);
  schedule->attributeScheduleStrategy( cs1 );
  testScenario.newImplosionSolution();

  cs1.populate( *schedule );
  // cout <<endl <<"LgFrStockScheduleStrategy with zero filter" <<endl
  //      <<schedule.format() <<endl;

  for ( i=0; i<schedule->size(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }
  
}
