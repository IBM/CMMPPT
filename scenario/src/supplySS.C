// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/supplySS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/scenario.h>
#include <scenario/src/machdep.h>
#include <scenario/src/partSchP.h>


// Populate Schedule with itemTimeVecs
void
LgFrSupplyScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  LgFrPart part;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat itv( part, tv );

  // Loop once for each part
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<setOfParts().size(); i++ ) {

     // Make itemTimeVec
     itv.item( setOfParts()[i] );
     itv.timeVecFloat(
           wipPtr_->timeVec( setOfParts()[i] )   );

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
  }

}

// Update (propogate) a supply change to wip 
void 
LgFrSupplyScheduleStrategy::update(
          const LgFrItemTimeVecPairFloat & newITV)
{
  wipPtr_->timeVec(newITV.part(), newITV.timeVecFloat());

  // inform dependent objects that the demandVol has changed. 
  //  we don't have to do this anymore as wip (through
  //  LgFrUserInputModification) will do it for us
  //  via the updateFrom method.
}

// Supply Volumes are mutable
bool
LgFrSupplyScheduleStrategy::isMutable()
const
{
 return true;
}

// Supply Volume TimeVecs mustn't contain any negative values
LgFrValidity
LgFrSupplyScheduleStrategy::validity(const LgFrTimeVecFloat& proposedTimeVec)
const
{
  int i;
  // return false upon locating the first negative value
  for ( i=0; i<proposedTimeVec.length(); i++ ) {
    if ( proposedTimeVec[i] < 0.0 ) {
      return INVALID;
    } 
  }
  return VALID;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrSupplyScheduleStrategy::clone()
const
{
  LgFrSupplyScheduleStrategy* newInst = 
      new LgFrSupplyScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrSupplyScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  LgFrSupplyScheduleStrategy* newInst = 
      new LgFrSupplyScheduleStrategy(*this, newScenario);
  return (newInst);
}


// Assignment operator
LgFrSupplyScheduleStrategy &
LgFrSupplyScheduleStrategy::operator=(
	  const LgFrSupplyScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleStrategy::operator= ) (rhs);
    wipPtr_ = rhs.wipPtr_;
  }
  return *this;
}


// Copy constructor
LgFrSupplyScheduleStrategy::
   LgFrSupplyScheduleStrategy (
      const LgFrSupplyScheduleStrategy & source)
:  LgFrPartScheduleStrategy( source ), 
   wipPtr_( source.wipPtr_ ),
   id_(__LGFRSUPPLYSCHEDULESTRATEGY)
{
}

// Copy constructor with newScenario
LgFrSupplyScheduleStrategy::
   LgFrSupplyScheduleStrategy (
      const LgFrSupplyScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleStrategy( 
         source, 
         newScenario ),
   wipPtr_( &(workingImplosionProblem(newScenario)) ),
   id_(__LGFRSUPPLYSCHEDULESTRATEGY)
{
}

// Constructor
LgFrSupplyScheduleStrategy::
   LgFrSupplyScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy( 
      LgFrAttributeScheduleStrategy::setOfParts(scenario),
                             &(userInputModification(scenario)) ),
   wipPtr_( &(workingImplosionProblem(scenario)) ),
   id_(__LGFRSUPPLYSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrSupplyScheduleStrategy::
   LgFrSupplyScheduleStrategy ()
:  LgFrPartScheduleStrategy( LgFrSetOfPartsWithMovableParts(), 0 ),
   wipPtr_( 0 ),
   id_(__LGFRSUPPLYSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrSupplyScheduleStrategy::
   ~LgFrSupplyScheduleStrategy ()
{
   // Nothing to do here
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrSupplyScheduleStrategy methods.
void
LgFrSupplyScheduleStrategy::test()
{
}


// test LgFrSupplyScheduleStrategy methods.
// Expects data for both testScenario and testScenario2 to come from
// /u/implode/wit/data/examples/diner12/wit.data, but expects them to
// be two different scenarios.
void
LgFrSupplyScheduleStrategy::contextTest
    (LgFrScenario                & testScenario,
     LgFrWorkingImplosionProblem & wip,            // from testScenario
     LgFrSetOfParts              & sop,            // from testScenario
     LgFrScenario                & testScenario2)
{
  int i;
  float fltEps = .0001f;

  // Test constructor
  LgFrSupplyScheduleStrategy supplySS1(testScenario);

  // Test copy constructor
  LgFrSupplyScheduleStrategy supplySS1copy(supplySS1);

  // Test copy constructor with a new scenario
  LgFrSupplyScheduleStrategy supplySS1copy2(supplySS1, testScenario2);

  // Test assignment operator
  {
    LgFrSupplyScheduleStrategy supplySS3;
    supplySS3 = supplySS1copy;
  }

  // Test clone method
  {
    LgFrAttributeScheduleStrategy *supplySS1ClonePtr = supplySS1.clone();

    // Test destructor
    delete supplySS1ClonePtr;
  }

  // Test populate
  LgFrSortingPartScheduleFloatSmartPointer schedule;
  const  LgFrSortingPartScheduleFloat & constSchedule = *schedule;
  supplySS1.populate( *schedule );
  // cout <<endl <<"LgFrSupplyScheduleStrategy" <<endl
  //      <<schedule.format() <<endl;

  for ( i=0; i<sop.size(); i++ ) {
     assert( sop[i] == constSchedule[i].part() );
     assert( wip.timeVec(sop[i]) == constSchedule[i].timeVecFloat() );
  }

  // Test populate with filter
  LgFrZeroTimeVecFilterStrategy zeroFilter;
  schedule->filterStrategy( zeroFilter);
  
  supplySS1.populate( *schedule );
  // cout <<endl <<"LgFrSupplyScheduleStrategy with zero filter" <<endl
  //      <<schedule.format() <<endl;

  for ( i=0; i<schedule->size(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }


  // Test update()
  LgFrPart aPart(sop[0]);
  LgFrItemTimeVecPairFloat anITV(aPart, constSchedule[0].timeVecFloat() + 2.5);
  supplySS1.update(anITV);
  supplySS1.populate(*schedule);
  assert(anITV.timeVecFloat() == constSchedule[0].timeVecFloat());


  // test clone(newScenario) -- as follows: compare the data in constSchedule
  // with the data in another scenario
  // that was initialized with the same data testScenario was initialized with.
  // The data in constSchedule was just changed, in the code under the comment
  // Test update()

  LgFrSortingPartScheduleFloat schedule22; 
  const LgFrSortingPartScheduleFloat & constSchedule22 = schedule22;

#if 0
  LgFrZeroTimeVecFilterStrategy zeroFilter3;
  supplySS1.filterStrategy( zeroFilter3 );
  assert( supplySS1.filterStrategy().isA() == __LGFRZEROTIMEVECFILTERSTRATEGY );
#endif

  LgFrAttributeScheduleStrategy * cs22Ptr = supplySS1.clone(testScenario2);

#if 0
  assert( cs22Ptr->filterStrategy().isA() == __LGFRZEROTIMEVECFILTERSTRATEGY );
#endif

  supplySS1.populate(*schedule);
  cs22Ptr->populate(schedule22);

  assert( ((constSchedule[0].timeVecFloat())[0] - 2.5 -
            (constSchedule22[0].timeVecFloat())[0]) <= fltEps);

  // now increase the supply in Scenario2 by 3.5
  LgFrItemTimeVecPairFloat anITV2(aPart, 
                     constSchedule22[0].timeVecFloat() + 3.5);
  cs22Ptr->update(anITV2);
  supplySS1.populate(*schedule);
  cs22Ptr->populate(schedule22);
  assert( (constSchedule22[0].timeVecFloat())[0] - 1.0 -
          (constSchedule[0].timeVecFloat())[0] <= fltEps);

  delete cs22Ptr;
}

