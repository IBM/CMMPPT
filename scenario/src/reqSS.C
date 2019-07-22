// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.

#include <math.h>
#include <scenario/src/reqSS.h>
#include <scenario/src/explode.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/scenario.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>
#include <scenario/src/partSchP.h>

#include <iostream>

//---------------------------------------------------------------------------
//
//   LgFrRequirementScheduleStrategy
//
//---------------------------------------------------------------------------


// Populate Schedule with itemTimeVecs
void
LgFrRequirementScheduleStrategy::kernalOfPopulate(
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
           explodePtr_->lastProblemSolved().requirementVolume( &part )   );

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );

  }

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrRequirementScheduleStrategy::clone()
const
{
  LgFrRequirementScheduleStrategy* newInst = 
      new LgFrRequirementScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrRequirementScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  LgFrRequirementScheduleStrategy* newInst = 
      new LgFrRequirementScheduleStrategy(*this, newScenario);
  return (newInst);
}
 

// Assignment operator
LgFrRequirementScheduleStrategy &
LgFrRequirementScheduleStrategy::operator=(
	  const LgFrRequirementScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleStrategy::operator= ) (rhs);

    explodePtr_ = rhs.explodePtr_;
  }
  return *this;
}


// Copy constructor
LgFrRequirementScheduleStrategy::
   LgFrRequirementScheduleStrategy (
      const LgFrRequirementScheduleStrategy & source)
:  LgFrPartScheduleStrategy( source ), 
   explodePtr_( source.explodePtr_ ),
   id_(__LGFRREQUIREMENTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Copy constructor with newScenario
LgFrRequirementScheduleStrategy::
   LgFrRequirementScheduleStrategy (
      const LgFrRequirementScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleStrategy( 
         source, 
         newScenario ),
   explodePtr_( &(explode(newScenario)) ),
   id_(__LGFRREQUIREMENTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Constructor
LgFrRequirementScheduleStrategy::
   LgFrRequirementScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy(
      LgFrAttributeScheduleStrategy::setOfParts(scenario),
      &(explode(scenario)) ), 
   explodePtr_( &(explode(scenario)) ),
   id_(__LGFRREQUIREMENTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrRequirementScheduleStrategy::
   LgFrRequirementScheduleStrategy ()
:  LgFrPartScheduleStrategy( LgFrSetOfPartsWithMovableParts(), 0 ),
   explodePtr_( 0 ),
   id_(__LGFRREQUIREMENTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrRequirementScheduleStrategy::
   ~LgFrRequirementScheduleStrategy ()
{
  // Nothing to do here
}


//---------------------------------------------------------------------------
//
//   LgFrCmrpRequirementScheduleStrategy
//
//---------------------------------------------------------------------------


// Populate Schedule with itemTimeVecs
void
LgFrCmrpRequirementScheduleStrategy::kernalOfPopulate(
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
           explodePtr_->lastProblemSolved().requirementVolume( &part )   );

     // If a product with zero bom entries then add mrpProdVol
     if ( explodePtr_->lastProblemSolved().category( &part ) == LgFrPRODUCT 
          &&
          explodePtr_->lastProblemSolved().nBomEntries( &part ) == 0 ) {
        
        itv.timeVecFloat(
           itv.timeVecFloat()
           +
           explodePtr_->lastProblemSolved().mrpProductionVolume( &part )   );
         
     }
 

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );

  }

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrCmrpRequirementScheduleStrategy::clone()
const
{
  LgFrCmrpRequirementScheduleStrategy* newInst = 
      new LgFrCmrpRequirementScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrCmrpRequirementScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  LgFrCmrpRequirementScheduleStrategy* newInst = 
      new LgFrCmrpRequirementScheduleStrategy(*this, newScenario);
  return (newInst);
}
 

// Assignment operator
LgFrCmrpRequirementScheduleStrategy &
LgFrCmrpRequirementScheduleStrategy::operator=(
	  const LgFrCmrpRequirementScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleStrategy::operator= ) (rhs);

    explodePtr_ = rhs.explodePtr_;
  }
  return *this;
}


// Copy constructor
LgFrCmrpRequirementScheduleStrategy::
   LgFrCmrpRequirementScheduleStrategy (
      const LgFrCmrpRequirementScheduleStrategy & source)
:  LgFrPartScheduleStrategy( source ), 
   explodePtr_( source.explodePtr_ ),
   id_(__LGFRCMRPREQUIREMENTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Copy constructor with newScenario
LgFrCmrpRequirementScheduleStrategy::
   LgFrCmrpRequirementScheduleStrategy (
      const LgFrCmrpRequirementScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleStrategy( 
         source, 
         newScenario ),
   explodePtr_( &(explode(newScenario)) ),
   id_(__LGFRCMRPREQUIREMENTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Constructor
LgFrCmrpRequirementScheduleStrategy::
   LgFrCmrpRequirementScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy(
      LgFrAttributeScheduleStrategy::setOfParts(scenario),
      &(explode(scenario)) ), 
   explodePtr_( &(explode(scenario)) ),
   id_(__LGFRCMRPREQUIREMENTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrCmrpRequirementScheduleStrategy::
   LgFrCmrpRequirementScheduleStrategy ()
:  LgFrPartScheduleStrategy( LgFrSetOfPartsWithMovableParts(), 0 ),
   explodePtr_( 0 ),
   id_(__LGFRCMRPREQUIREMENTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrCmrpRequirementScheduleStrategy::
   ~LgFrCmrpRequirementScheduleStrategy ()
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

// test LgFrRequirementScheduleStrategy methods.
void
LgFrRequirementScheduleStrategy::test()
{
}


// test LgFrRequirementScheduleStrategy methods.
// testScenario should have been generated using the diner12 dataset
void
LgFrRequirementScheduleStrategy::contextTest
    (LgFrScenario          & testScenario,
     LgFrWorkingImplosionProblem & wip,
     LgFrLastProblemSolved & lps,
     LgFrSetOfParts        & sop)
{
  int i;

  // Test constructor
  LgFrRequirementScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrRequirementScheduleStrategy cs2(cs1);

  // Test assignment operator
  LgFrRequirementScheduleStrategy cs3;
  cs3 = cs2;

  // Test clone method
  LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();

  // Test destructor
  delete cs4Ptr;

  // See if we can break the LgFrModel stuff
  cs4Ptr = cs2.clone();
  *cs4Ptr = cs1;
  delete cs4Ptr;


  // Test populate.  Should be populated with zero timeVec because
  // a requirement schedule does not exist.
  LgFrSortingPartScheduleFloatSmartPointer schedule(new LgFrSortingPartScheduleFloat); 
  const LgFrSortingPartScheduleFloat & constSchedule = *schedule;
  cs1.populate( *schedule );
  for ( i=0; i<sop.size(); i++ ) {
     assert(  schedule->contains( sop[i] )  );
     assert( constSchedule[sop[i]].timeVecFloat() == 0.0 );
  }

  // Test popluate with zeroFilter.  Should produce an empty schedule
  LgFrZeroTimeVecFilterStrategy zeroFilter2;
  schedule->filterStrategy( zeroFilter2 );
  cs1.populate( *schedule );
  assert( schedule->size()==0 );

  // Test populate, due to new explosion solution.
  {
  LgFrFilterStrategy defaultFilter;
  schedule->filterStrategy( defaultFilter );
  }
  schedule->attributeScheduleStrategy( cs1 );
  lps.mrp(wip);
  cs1.populate( *schedule );
  std::cout <<std::endl <<"LgFrRequirementScheduleStrategy" <<std::endl
      <<schedule->format() << std::endl;

  for ( i=0; i<sop.size(); i++ ) {
     assert(  schedule->contains( sop[i] )  );
     assert( lps.requirementVolume(&sop[i]) == 
             constSchedule[sop[i]].timeVecFloat() );
  }

  // Test populate with filter
  schedule->filterStrategy( zeroFilter2);
  schedule->attributeScheduleStrategy( cs1 );
  testScenario.newImplosionSolution();

  cs1.populate( *schedule );
  // std::cout <<std::endl <<"LgFrRequirementScheduleStrategy with zero filter" <<std::endl
  //      <<schedule->format() <<std::endl;

  for ( i=0; i<schedule->size(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }

}


// test LgFrCmrpRequirementScheduleStrategy methods.
void
LgFrCmrpRequirementScheduleStrategy::test()
{
}


// test LgFrCmrpRequirementScheduleStrategy methods.
// testScenario should have been generated using the diner12 dataset
void
LgFrCmrpRequirementScheduleStrategy::contextTest
    (LgFrScenario          & testScenario,
     LgFrWorkingImplosionProblem & wip,
     LgFrLastProblemSolved & lps,
     LgFrSetOfParts        & sop)
{
  int i;

  // Test constructor
  LgFrCmrpRequirementScheduleStrategy cs1(testScenario);

  // Test copy constructor
  LgFrCmrpRequirementScheduleStrategy cs2(cs1);

  // Test assignment operator
  LgFrCmrpRequirementScheduleStrategy cs3;
  cs3 = cs2;

  // Test clone method
  LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();

  // Test destructor
  delete cs4Ptr;

  // See if we can break the LgFrModel stuff
  cs4Ptr = cs2.clone();
  *cs4Ptr = cs1;
  delete cs4Ptr;


  // Test populate.  Should be populated with zero timeVec because
  // a requirement schedule does not exist.
  LgFrSortingPartScheduleFloatSmartPointer schedule(new LgFrSortingPartScheduleFloat); 
  const LgFrSortingPartScheduleFloat & constSchedule = *schedule;
  cs1.populate( *schedule );
  for ( i=0; i<sop.size(); i++ ) {
     assert(  schedule->contains( sop[i] )  );
     assert( constSchedule[sop[i]].timeVecFloat() == 0.0 );
  }

  // Test popluate with zeroFilter.  Should produce an empty schedule
  LgFrZeroTimeVecFilterStrategy zeroFilter2;
  schedule->filterStrategy( zeroFilter2 );
  cs1.populate( *schedule );
  assert( schedule->size()==0 );

  // Test populate, due to new explosion solution.
  {
  LgFrFilterStrategy defaultFilter;
  schedule->filterStrategy( defaultFilter );
  }
  schedule->attributeScheduleStrategy( cs1 );
  lps.mrp(wip);
  cs1.populate( *schedule );
  std::cout <<std::endl <<"LgFrCmrpRequirementScheduleStrategy" <<std::endl
       <<schedule->format() <<std::endl;

  for ( i=0; i<sop.size(); i++ ) {
     assert(  schedule->contains( sop[i] )  );
     assert( lps.requirementVolume(&sop[i]) <= 
             constSchedule[sop[i]].timeVecFloat() );
  }

  // Test populate with filter
  schedule->filterStrategy( zeroFilter2);
  schedule->attributeScheduleStrategy( cs1 );
  testScenario.newImplosionSolution();

  cs1.populate( *schedule );
  // std::cout <<std::endl <<"LgFrCmrpRequirementScheduleStrategy with zero filter" <<std::endl
  //      <<schedule->format() <<std::endl;

  for ( i=0; i<schedule->size(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }

}

