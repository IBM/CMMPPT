// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/cumSS.h>
#include <scenario/src/deltaSS.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/scenario.h>


//-----------------------------------------------------------------------
//
//  Implementation of LgFrTwoScheduleDeltaPartScheduleStrategy
//
//-----------------------------------------------------------------------


// Populate Schedule with itemTimeVecs
void
LgFrTwoScheduleDeltaPartScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  assert( schedules().size() == 2 && 
          "Two Schedule Delta Part Schedule Strategy depends on exactly 2 schedules");

  LgFrPart part;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat itv( part, tv );

  const LgFrSortingScheduleFloat & indepSched1 = *(schedules()[0]);
  const LgFrSortingScheduleFloat & indepSched2 = *(schedules()[1]);

  LgFrSortingScheduleFloat tempSchedule = indepSched1 - indepSched2;
  const  LgFrSortingScheduleFloat & tempSched = tempSchedule;

  // Loop once for each item in independent schedule
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<tempSched.size(); i++ ) {

     // get next itemTimeVec
     itv = tempSched[i];

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
  }

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrTwoScheduleDeltaPartScheduleStrategy::clone()
const
{
    LgFrTwoScheduleDeltaPartScheduleStrategy* temp = 
	new LgFrTwoScheduleDeltaPartScheduleStrategy(*this);
    // hack up the schedules
    // need to deep copy the independent schedules
    size_t i = 0; // Pulled out of the for below by RW2STL
    for ( i=0; i< temp->independentSchedules_.size(); i++ )  {
      // cast away const here to get subscription stuff to work
	temp->independentSchedules_[i]->removeDependentFromHeterogeneousList(temp );
	temp->independentSchedules_[i] = independentSchedules_[i].deepCopy();
	temp->independentSchedules_[i]->addDependent(temp );
    }
    return temp;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrTwoScheduleDeltaPartScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  
  return new LgFrTwoScheduleDeltaPartScheduleStrategy(*this, newScenario);
}


// Assignment operator
LgFrTwoScheduleDeltaPartScheduleStrategy &
LgFrTwoScheduleDeltaPartScheduleStrategy::operator=(
	  const LgFrTwoScheduleDeltaPartScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleSensitiveScheduleStrategy::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrTwoScheduleDeltaPartScheduleStrategy::
   LgFrTwoScheduleDeltaPartScheduleStrategy (
      const LgFrTwoScheduleDeltaPartScheduleStrategy & source)
:  LgFrPartScheduleSensitiveScheduleStrategy( source ), 
   id_(__LGFRTWOSCHEDULEDELTAPARTSCHEDULESTRATEGY)
{
}

// Copy constructor with newScenario
LgFrTwoScheduleDeltaPartScheduleStrategy::
   LgFrTwoScheduleDeltaPartScheduleStrategy (
      const LgFrTwoScheduleDeltaPartScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleSensitiveScheduleStrategy( 
         source, 
         newScenario ),
   id_(__LGFRTWOSCHEDULEDELTAPARTSCHEDULESTRATEGY)
{
}

// Constructor
LgFrTwoScheduleDeltaPartScheduleStrategy::
   LgFrTwoScheduleDeltaPartScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleSensitiveScheduleStrategy( scenario ),
   id_(__LGFRTWOSCHEDULEDELTAPARTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrTwoScheduleDeltaPartScheduleStrategy::
   LgFrTwoScheduleDeltaPartScheduleStrategy ()
:  LgFrPartScheduleSensitiveScheduleStrategy(),
   id_(__LGFRTWOSCHEDULEDELTAPARTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrTwoScheduleDeltaPartScheduleStrategy::
   ~LgFrTwoScheduleDeltaPartScheduleStrategy ()
{
   // Nothing to do here
}

//-----------------------------------------------------------------------
//
//  Test of LgFrTwoScheduleDeltaPartScheduleStrategy
//
//-----------------------------------------------------------------------

#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrTwoScheduleDeltaPartScheduleStrategy methods.

void
LgFrTwoScheduleDeltaPartScheduleStrategy::test()
{
  LgFrTwoScheduleDeltaPartScheduleStrategy cs1;
  assert( cs1.schedules().size() == 0 );
}


// test LgFrTwoScheduleDeltaPartScheduleStrategy methods.
// Expects testScenario to contain data from
// /u/implode/wit/data/examples/diner12/wit.data
// Expects supplySchedule to be a supply schedule for testScenario
// (containing a LgFrSupplyScheduleStrategy)
void
LgFrTwoScheduleDeltaPartScheduleStrategy::contextTest
    (
     LgFrScenario & testScenario,
     LgFrSortingPartScheduleFloatSmartPointer supplySchedule
     )
{
    std::cerr << "entering LgFrTwoScheduleDeltaPartScheduleStrategy::contextTest" << std::endl;
    int i;
    LgFrSortingPartScheduleFloatSmartPointer supplySchedule2(new LgFrSortingPartScheduleFloat);
    *supplySchedule2 = *supplySchedule + *supplySchedule;

  LgFrTwoScheduleDeltaPartScheduleStrategy cs1;

  // Test inserting two dependent schedule
  cs1.insertPartSchedule( supplySchedule ); 
  cs1.insertPartSchedule( supplySchedule2 ); 


  assert( cs1.schedules()[0] == supplySchedule );
  assert( cs1.schedules()[1] == supplySchedule2 );

  // Test copy constructor
  LgFrTwoScheduleDeltaPartScheduleStrategy cs2(cs1);
  assert( cs2.schedules()[0] == supplySchedule );
  assert( cs2.schedules()[1] == supplySchedule2 );

  // Test assignment operator
  LgFrTwoScheduleDeltaPartScheduleStrategy cs3;
  cs3 = cs2;
  assert( cs3.schedules()[0] == supplySchedule );
  assert( cs3.schedules()[1] == supplySchedule2 );

  // Test clone method
  {
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( (  (LgFrTwoScheduleDeltaPartScheduleStrategy *) cs4Ptr)
            ->schedules().size() == 2 );

    assert( (  (LgFrTwoScheduleDeltaPartScheduleStrategy *) cs4Ptr)
            ->schedules()[0] 
            != supplySchedule );
    assert( *((  (LgFrTwoScheduleDeltaPartScheduleStrategy *) cs4Ptr)
            ->schedules()[0])
            == *supplySchedule );

    assert( (  (LgFrTwoScheduleDeltaPartScheduleStrategy *) cs4Ptr)
            ->schedules()[1] 
            != supplySchedule2 );
    assert( *((  (LgFrTwoScheduleDeltaPartScheduleStrategy *) cs4Ptr)
            ->schedules()[1])
            == *supplySchedule2 );
    
    // Test destructor
    delete cs4Ptr;
  }


  // Test Kernal of populate
  const LgFrSortingPartScheduleFloat & constSupplySchedule = *supplySchedule;
  const LgFrSortingPartScheduleFloat & constSupplySchedule2 = *supplySchedule2;

  LgFrSortingPartScheduleFloat deltaSup;
  const LgFrSortingPartScheduleFloat & constDeltaSup = deltaSup;
  deltaSup.attributeScheduleStrategy( cs1 );
  deltaSup.populate();
  for ( i=0; i<supplySchedule->size(); i++ ) 
     assert(         constDeltaSup[i].timeVecFloat() == 
               (constSupplySchedule[i].timeVecFloat() - 
                constSupplySchedule2[i].timeVecFloat()));

  // Test subscription mechanism
  LgFrPart part2 = constSupplySchedule[2].part();
  supplySchedule->insertKeyAndValue(  part2, LgFrTimeVecFloat(2, 3.1f) );
//  supplySchedule2.insertKeyAndValue( part2, LgFrTimeVecFloat(2, 1.5) );
//  std::cerr << "supplySchedule[" <<  part2.name() << "][0] = " 
//                     << (constSupplySchedule[part2].timeVecFloat())[0] << "\n";
//  std::cerr << "supplySchedule2[" << part2.name()  << "][0] = " 
//                     << (constSupplySchedule2[part2].timeVecFloat())[0] << "\n";
//  std::cerr << "deltaSup[" << part2.name() << "][0] = " 
//                     << (constDeltaSup[part2].timeVecFloat())[0] << "\n";
//  std::cerr << "supplySchedule[" <<  part2.name() << "][0] = " 
//                     << (constSupplySchedule[part2].timeVecFloat())[0] << "\n";
//  std::cerr << "supplySchedule2[" << part2.name()  << "][0] = " 
//                     << (constSupplySchedule2[part2].timeVecFloat())[0] << "\n";

  assert(         constDeltaSup[part2].timeVecFloat() == 
               (constSupplySchedule[part2].timeVecFloat() - 
                constSupplySchedule2[part2].timeVecFloat()));

  // Test subscription mechanism


  // Test scenario copy constructor
  LgFrTwoScheduleDeltaPartScheduleStrategy scenCtor(cs1,testScenario);
  assert( scenCtor.schedules().size() ==  
               cs1.schedules().size() );

  // Test copy constructor when strategy owns the schedule
  LgFrTwoScheduleDeltaPartScheduleStrategy cs5(scenCtor);
  assert( scenCtor.schedules().size() ==  
               cs5.schedules().size() );


  // Test cloning w/scenario 
  LgFrAttributeScheduleStrategy *cs6Ptr = cs1.clone(testScenario);
  assert( ((LgFrTwoScheduleDeltaPartScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != supplySchedule );
  assert( ((LgFrTwoScheduleDeltaPartScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != cs1.schedules()[0] );
  assert( *(((LgFrTwoScheduleDeltaPartScheduleStrategy *)cs6Ptr)->schedules()[0])
          == *(cs1.schedules()[0]) );

  delete cs6Ptr;
}



