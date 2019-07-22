// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <math.h>
#include <scenario/src/prodSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>
#include <scenario/src/scenario.h>
#include <scenario/src/partSchP.h>


// Populate Schedule with itemTimeVecs
void
LgFrProductionScheduleStrategy::kernalOfPopulate(
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
     if ( implodePtr_->lastProblemSolved().category( &part )  ==  
          LgFrPRODUCT ) {

        // Make itemTimeVec
        itv.item( part );
        itv.timeVecFloat(
              implodePtr_->lastProblemSolved().productionVolume( &part )   );

        // Check to see if i'th itemTimeVec is to be filtered out
        if ( schedule.filterStrategy().exclude( itv ) ) continue;

        // Insert itemTimeVec into schedule
        schedule.localInsert( itv );

     }
  }

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrProductionScheduleStrategy::clone()
const
{
  LgFrProductionScheduleStrategy* newInst = 
      new LgFrProductionScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrProductionScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  LgFrProductionScheduleStrategy* newInst = 
      new LgFrProductionScheduleStrategy(*this, newScenario);
  return (newInst);
}
 

// Assignment operator
LgFrProductionScheduleStrategy &
LgFrProductionScheduleStrategy::operator=(
	  const LgFrProductionScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleStrategy::operator= ) (rhs);

    implodePtr_ = rhs.implodePtr_;
  }
  return *this;
}


// Copy constructor
LgFrProductionScheduleStrategy::
   LgFrProductionScheduleStrategy (
      const LgFrProductionScheduleStrategy & source)
:  LgFrPartScheduleStrategy( source ), 
   implodePtr_( source.implodePtr_ ),
   id_(__LGFRPRODUCTIONSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Copy constructor with newScenario
LgFrProductionScheduleStrategy::
   LgFrProductionScheduleStrategy (
      const LgFrProductionScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleStrategy( 
         source, 
         newScenario ),
   implodePtr_( &(implode(newScenario)) ),
   id_(__LGFRPRODUCTIONSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Constructor
LgFrProductionScheduleStrategy::
   LgFrProductionScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy(
      LgFrAttributeScheduleStrategy::setOfParts(scenario),
      &(implode(scenario)) ), 
   implodePtr_( &(implode(scenario)) ),
   id_(__LGFRPRODUCTIONSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrProductionScheduleStrategy::
   LgFrProductionScheduleStrategy ()
:  LgFrPartScheduleStrategy( LgFrSetOfPartsWithMovableParts(), 0 ),
   implodePtr_( 0 ),
   id_(__LGFRPRODUCTIONSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrProductionScheduleStrategy::
   ~LgFrProductionScheduleStrategy ()
{
  // Nothing to do here
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrProductionScheduleStrategy methods.
void
LgFrProductionScheduleStrategy::test()
{
}

// test LgFrProductionScheduleStrategy methods.
// test LgFrSupplyScheduleStrategy methods.
// Expects data for both testScenario and testScenario2 to come from
// /u/implode/wit/data/examples/diner12/wit.data, but expects them to
// be two different scenarios.void
void
LgFrProductionScheduleStrategy::contextTest
    (LgFrScenario                & testScenario,
     LgFrLastProblemSolved       & lps,
     LgFrSetOfParts              & sop,
     LgFrScenario                & testScenario2,
     LgFrLastProblemSolved       & lps2)
{
  int i;
  float floatEpsilon = .00001f;

  // Test constructor
  LgFrProductionScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrProductionScheduleStrategy cs2(cs1);

  // Test copy constructor with new scenario - while
  //   testScenario1 and testScenario2 are similar, they
  //   are separate objects -> they don't share parts!!
  LgFrProductionScheduleStrategy cs13(cs1, testScenario2);
  assert(cs13.implodePtr_ != cs2.implodePtr_);

  // Test assignment operator
  LgFrProductionScheduleStrategy cs3;
  cs3 = cs2;

  {
    // Test clone method
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();

    // Test destructor
    delete cs4Ptr;

    // See if we can break the LgFrModel stuff
    cs4Ptr = cs2.clone();
    *cs4Ptr = cs1;
    delete cs4Ptr;
  }

  // Test populate.  Should be populated with zero timeVec because
  // a production schedule does not exist.
  LgFrSortingPartScheduleFloatSmartPointer schedule; 
  const LgFrSortingPartScheduleFloat & constSchedule = *schedule;
  cs1.populate( *schedule );

  LgFrSortingPartScheduleFloatSmartPointer schedule2; 
  const LgFrSortingPartScheduleFloat & constSchedule2 = *schedule2;
  cs13.populate( *schedule2 );

  for ( i=0; i<sop.size(); i++ ) {
     if ( lps.category( &(sop[i]) ) == LgFrPRODUCT ) {
        assert(  schedule->contains( sop[i] )  );
        assert( constSchedule[sop[i]].timeVecFloat() == 0.0 );
     }
     else {
        assert(  !schedule->contains( sop[i] )  );
     }
  }

  // Test popluate with zeroFilter.  Should produce an empty schedule
  LgFrZeroTimeVecFilterStrategy zeroFilter2;
  schedule->filterStrategy( zeroFilter2 );
  schedule2->filterStrategy( zeroFilter2 );
  cs1.populate( *schedule );
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

  assert(constSchedule == constSchedule2);

  // cs1.populate( *schedule );
  // cout <<endl <<"LgFrProductionScheduleStrategy" <<endl
  //      <<schedule->format() <<endl;

  for ( i=0; i<sop.size(); i++ ) {
     if ( lps.category( &sop[i] ) == LgFrPRODUCT ) {
        assert(  schedule->contains( sop[i] )  );
        assert( lps.productionVolume(&sop[i]) == 
                constSchedule[sop[i]].timeVecFloat() );
     }
     else {
        assert(  !schedule->contains( sop[i] )  );
     }
  }

  // Test populate with filter
  schedule->filterStrategy( zeroFilter2);
  schedule->attributeScheduleStrategy( cs1 );
  testScenario.newImplosionSolution();

  cs1.populate( *schedule );
  // cout <<endl <<"LgFrProductionScheduleStrategy with zero filter" <<endl
  //      <<schedule->format() <<endl;

  for ( i=0; i<schedule->size(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }

  // test clone(newScenario) -- as follows:  
  // run OPTimplode in one scenario and HEURimplode in the other.  Then check for
  // different prodScheds
  LgFrSortingPartScheduleFloat schedule22; 
  const LgFrSortingPartScheduleFloat & constSchedule22 = schedule22;

#if 0
  LgFrZeroTimeVecFilterStrategy zeroFilter3;
  cs1.filterStrategy( zeroFilter3 );
  assert( cs1.filterStrategy().isA() == __LGFRZEROTIMEVECFILTERSTRATEGY );
#endif

  LgFrAttributeScheduleStrategy * cs22Ptr = cs1.clone(testScenario2);

#if 0
  assert( cs22Ptr->filterStrategy().isA() == __LGFRZEROTIMEVECFILTERSTRATEGY );
#endif


     // can only do this test if opt_implode exists
#ifdef OPT_IMPLODE
#ifdef DONE_FIXING_FRAMEWORK
// THE FOLLOWING CALLS TO heurImplode AND optImplode SHOULD BE REPLACED WITH CALLS
// THAT SET IT IN WORKING IMPLOSION PROBLEM AND CALL 
// scenario's newImplosionSolution METHOD.
  // now run heurimplode and optimplode in each scenario
  lps2.heurImplode();
  lps.optImplode();

  cs1.populate(schedule);
  cs22Ptr->populate(schedule22);

  // compare the production schedules for hamSand and plainSand.  The
  // plain_Sand's should be the same, whereas the ham_Sand's should be
  // different.
  LgFrPart hamSand(*(sop.findValue("Ham_Sand")));
  LgFrPart plainSand(*(sop.findValue("Plain_Sand")));

  assert(  fabs((constSchedule[hamSand].timeVecFloat())[0] -
                (constSchedule22[hamSand].timeVecFloat())[0]) 
               > floatEpsilon);  

  assert(  fabs((constSchedule[plainSand].timeVecFloat())[0] -
                (constSchedule22[plainSand].timeVecFloat())[0])    
             <= floatEpsilon);
#endif
#endif
  delete cs22Ptr;

}
