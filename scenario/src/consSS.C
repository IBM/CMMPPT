// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <math.h>

#include <witutil/src/witU5.h>

#include <scenario/src/consSS.h>
#include <scenario/src/explode.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/scenario.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>
#include <scenario/src/partSchP.h>



//---------------------------------------------------------------------------
//
//   LgFrMrpConsVolScheduleStrategy
//
//---------------------------------------------------------------------------


// Populate Schedule with itemTimeVecs
void
LgFrMrpConsVolScheduleStrategy::kernalOfPopulate(
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
           explodePtr_->lastProblemSolved().mrpConsVolume( &part )   );

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );

  }

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrMrpConsVolScheduleStrategy::clone()
const
{
  LgFrMrpConsVolScheduleStrategy* newInst = 
      new LgFrMrpConsVolScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrMrpConsVolScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  LgFrMrpConsVolScheduleStrategy* newInst = 
      new LgFrMrpConsVolScheduleStrategy(*this, newScenario);
  return (newInst);
}
 

// Assignment operator
LgFrMrpConsVolScheduleStrategy &
LgFrMrpConsVolScheduleStrategy::operator=(
	  const LgFrMrpConsVolScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleStrategy::operator= ) (rhs);

    explodePtr_ = rhs.explodePtr_;
  }
  return *this;
}


// Copy constructor
LgFrMrpConsVolScheduleStrategy::
   LgFrMrpConsVolScheduleStrategy (
      const LgFrMrpConsVolScheduleStrategy & source)
:  LgFrPartScheduleStrategy( source ), 
   explodePtr_( source.explodePtr_ ),
   id_(__LGFRMRPCONSVOLSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Copy constructor with newScenario
LgFrMrpConsVolScheduleStrategy::
   LgFrMrpConsVolScheduleStrategy (
      const LgFrMrpConsVolScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleStrategy( 
         source, 
         newScenario ),
   explodePtr_( &(explode(newScenario)) ),
   id_(__LGFRMRPCONSVOLSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Constructor
LgFrMrpConsVolScheduleStrategy::
   LgFrMrpConsVolScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy(
      LgFrAttributeScheduleStrategy::setOfParts(scenario),
      &(explode(scenario)) ), 
   explodePtr_( &(explode(scenario)) ),
   id_(__LGFRMRPCONSVOLSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrMrpConsVolScheduleStrategy::
   LgFrMrpConsVolScheduleStrategy ()
:  LgFrPartScheduleStrategy( LgFrSetOfPartsWithMovableParts(), 0 ),
   explodePtr_( 0 ),
   id_(__LGFRMRPCONSVOLSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrMrpConsVolScheduleStrategy::
   ~LgFrMrpConsVolScheduleStrategy ()
{
  // Nothing to do here
}



//---------------------------------------------------------------------------
//
//   LgFrFullyExplodedMrpConsVolScheduleStrategy
//
//---------------------------------------------------------------------------

// Populate Schedule with itemTimeVecs
void
LgFrFullyExplodedMrpConsVolScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  //-----------------------------------------------------------------
  // Set up to invoke wit utility to compute attribute
  //-----------------------------------------------------------------
  
  // Get a veneer of WitRun being used by LPS
  LgFrLastProblemSolved & lps = explodePtr_->lastProblemSolved();
  LgFrWitRunVeneer witRunVeneer( lps.witRunVeneer() );

  // Set up parmameters for WUtlComputeDependentDemandAndPlanOrders
  int nParts;
  char ** partList;
  witGetParts( 
            witRunVeneer.constWitRun(), 
            &nParts,
            &partList );
  int nPeriods = lps.numberOfPeriods(); 
  float ** femcv      = new float* [nParts]; // fully exploded mrp cons vol

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<nParts; i++ ) {
    femcv     [i] = new float [nPeriods];
  }

  // Invoke wit utility to compute fully exploded mrp cons vol
  WUtlComputeDependentDemandAndPlanOrders(
     witRunVeneer.mutableWitRun(), 
     nParts,     
     partList,
     femcv,  
     0,
     WUTL_CURRENT_PERIOD, // whereToPutOutstandingReqVolForRaw
     WUTL_CURRENT_PERIOD, // whereToPutOutstandingReqVolForProductWithEmptyBom
     WUTL_CURRENT_PERIOD, // whereToPutOutstandingReqVolForProductWithBom
     false,               // includeOrderCancelRecommendationInPlanOrder
     false                // resetDemandVolsWhenDone
     );
  
  const LgFrPart * partPtr;
  LgFrPart part;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat itv( part, tv );

  // Loop once for each part in wit
  for ( i=0; i<nParts; i++ ) {

    // Check to see if i'th part is in set of parts
    partPtr = setOfParts().findValue( partList[i] );
    if ( partPtr == 0 ) continue;
    part = *partPtr;

    // Make itemTimeVec
    itv.item( part );
    itv.timeVecFloat( LgFrTimeVecFloat( nPeriods, femcv[i] ) );

    // Check to see if i'th itemTimeVec is to be filtered out
    if ( schedule.filterStrategy().exclude( itv ) ) continue;

    // Insert itemTimeVec into schedule
    schedule.localInsert( itv );

  }
  
  // Free storage of previously allocated vectors
  for ( i=0; i<nParts; i++ ) {
    delete [] femcv[i];
    portableFree( partList[i] );
  }
  delete [] femcv;
  portableFree( partList );

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrFullyExplodedMrpConsVolScheduleStrategy::clone()
const
{
  LgFrFullyExplodedMrpConsVolScheduleStrategy* newInst = 
      new LgFrFullyExplodedMrpConsVolScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrFullyExplodedMrpConsVolScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  LgFrFullyExplodedMrpConsVolScheduleStrategy* newInst = 
      new LgFrFullyExplodedMrpConsVolScheduleStrategy(*this, newScenario);
  return (newInst);
}
 
// Assignment operator
LgFrFullyExplodedMrpConsVolScheduleStrategy &
LgFrFullyExplodedMrpConsVolScheduleStrategy::operator=(
	  const LgFrFullyExplodedMrpConsVolScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleStrategy::operator= ) (rhs);

    explodePtr_ = rhs.explodePtr_;
  }
  return *this;
}


// Copy constructor
LgFrFullyExplodedMrpConsVolScheduleStrategy::
   LgFrFullyExplodedMrpConsVolScheduleStrategy (
      const LgFrFullyExplodedMrpConsVolScheduleStrategy & source)
:  LgFrPartScheduleStrategy( source ), 
   explodePtr_( source.explodePtr_ ),
   id_(__LGFRFULLYEXPLODEDMRPCONSVOLSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Copy constructor with newScenario
LgFrFullyExplodedMrpConsVolScheduleStrategy::
   LgFrFullyExplodedMrpConsVolScheduleStrategy (
      const LgFrFullyExplodedMrpConsVolScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleStrategy( 
         source, 
         newScenario ),
   explodePtr_( &(explode(newScenario)) ),
   id_(__LGFRFULLYEXPLODEDMRPCONSVOLSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Constructor
LgFrFullyExplodedMrpConsVolScheduleStrategy::
   LgFrFullyExplodedMrpConsVolScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy(
      LgFrAttributeScheduleStrategy::setOfParts(scenario),
      &(explode(scenario)) ), 
   explodePtr_( &(explode(scenario)) ),
   id_(__LGFRFULLYEXPLODEDMRPCONSVOLSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrFullyExplodedMrpConsVolScheduleStrategy::
   LgFrFullyExplodedMrpConsVolScheduleStrategy ()
:  LgFrPartScheduleStrategy( LgFrSetOfPartsWithMovableParts(), 0 ),
   explodePtr_( 0 ),
   id_(__LGFRFULLYEXPLODEDMRPCONSVOLSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrFullyExplodedMrpConsVolScheduleStrategy::
   ~LgFrFullyExplodedMrpConsVolScheduleStrategy ()
{
  // Nothing to do here
}

//---------------------------------------------------------------------------
//
//   Test Methods
//
//---------------------------------------------------------------------------



#ifdef NDEBUG
#undef NDEBUG
#endif




// test LgFrMrpConsVolScheduleStrategy methods.
void
LgFrMrpConsVolScheduleStrategy::test()
{
}


// test LgFrMrpConsVolScheduleStrategy methods.
// testScenario should have been generated using the diner12 dataset
void
LgFrMrpConsVolScheduleStrategy::contextTest
    (LgFrScenario          & testScenario,
     LgFrWorkingImplosionProblem & wip,
     LgFrLastProblemSolved & lps,
     LgFrSetOfParts        & sop)
{
  int i;

  // Test constructor
  LgFrMrpConsVolScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrMrpConsVolScheduleStrategy cs2(cs1);

  // Test assignment operator
  LgFrMrpConsVolScheduleStrategy cs3;
  cs3 = cs2;

  // Test clone method
  LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();

  // Test destructor
  delete cs4Ptr;

  // See if we can break the LgFrModel stuff
  cs4Ptr = cs2.clone();
  *cs4Ptr = cs1;
  delete cs4Ptr;


  // Test populate..
  LgFrSortingPartScheduleFloatSmartPointer 
     schedule(new LgFrSortingPartScheduleFloat); 
  const LgFrSortingPartScheduleFloat & constSchedule = *schedule;
  cs1.populate( *schedule );
  for ( i=0; i<sop.size(); i++ ) {
     assert(  schedule->contains( sop[i] )  );
     assert( constSchedule[sop[i]].timeVecFloat() == 0.0 );
  }

  // Test populate, due to new explosion solution.
  schedule->attributeScheduleStrategy( cs1 );
  lps.mrp(wip);
  cs1.populate( *schedule );
  std::cout << std::endl <<"LgFrMrpConsVolScheduleStrategy" << std::endl
       <<schedule->format() << std::endl;

  for ( i=0; i<sop.size(); i++ ) {
     assert(  schedule->contains( sop[i] )  );
     assert( lps.mrpConsVolume(&sop[i]) <= 
             constSchedule[sop[i]].timeVecFloat() );
  }

}



// test LgFrFullyExplodedMrpConsVolScheduleStrategy methods.
void
LgFrFullyExplodedMrpConsVolScheduleStrategy::test()
{
}


// test LgFrFullyExplodedMrpConsVolScheduleStrategy methods.
// testScenario should have been generated using the diner12 dataset
void
LgFrFullyExplodedMrpConsVolScheduleStrategy::contextTest
    (LgFrScenario          & testScenario,
     LgFrWorkingImplosionProblem & wip,
     LgFrLastProblemSolved & lps,
     LgFrSetOfParts        & sop)
{
  int i;

  // Test constructor
  LgFrFullyExplodedMrpConsVolScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrFullyExplodedMrpConsVolScheduleStrategy cs2(cs1);

  // Test assignment operator
  LgFrFullyExplodedMrpConsVolScheduleStrategy cs3;
  cs3 = cs2;

  // Test clone method
  LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();

  // Test destructor
  delete cs4Ptr;

  // See if we can break the LgFrModel stuff
  cs4Ptr = cs2.clone();
  *cs4Ptr = cs1;
  delete cs4Ptr;


  // Test populate..
  LgFrSortingPartScheduleFloatSmartPointer 
     schedule(new LgFrSortingPartScheduleFloat); 
  const LgFrSortingPartScheduleFloat & constSchedule = *schedule;
  cs1.populate( *schedule );


  // Test populate, due to new explosion solution.
  schedule->attributeScheduleStrategy( cs1 );
  lps.mrp(wip);
  cs1.populate( *schedule );
  std::cout << std::endl <<"LgFrFullyExplodedMrpConsVolScheduleStrategy" << std::endl
       <<schedule->format() << std::endl;

  for ( i=0; i<sop.size(); i++ ) {
     assert(  schedule->contains( sop[i] )  );
     assert( lps.mrpConsVolume(&sop[i]) <= 
             constSchedule[sop[i]].timeVecFloat() );
  }

}

