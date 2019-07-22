// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/offsetSS.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/scenario.h>
#include <scenario/src/demdSchd.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/demdPnPC.h>

// #include <scenario/src/itemTimV.h>
// #include <scenario/src/demdSchd.h>
// #include <scenario/src/demandSS.h>
// #include <scenario/src/sortSchd.h>
// #include <scenario/src/zeroTvFS.h>
// #include <scenario/src/machdep.h>


//-----------------------------------------------------------------------
//
//  Implementation of LgFrApplyOffsetScheduleStrategy
//
//-----------------------------------------------------------------------


// Populate Schedule with itemTimeVecs
void
LgFrApplyOffsetScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  assert( schedules().size() == 2 && 
          "ApplyOffset Schedule Strategy depends on exactly 2 schedules");

  LgFrPart part;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat dataITV( part, tv );
  LgFrTimeVecFloat offsetTV;
  LgFrItemTimeVecPairFloat shiftedITV;

  const LgFrSortingScheduleFloat & dataSched = *(schedules()[0]);
  const LgFrSortingScheduleFloat & offsetSched = *(schedules()[1]);
  bool result;

  // Loop once for each item in independent schedule
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<dataSched.size(); i++ ) {

     // get next itemTimeVec
     dataITV = dataSched[i];
     
     // find the corresponding offset timeVec in offsetSched
     result = offsetSched.findValue(dataITV.part(), offsetTV);
    
     // If item DOES appear in in offsetSched then shift the dataVec
     if (result)  {
        shiftedITV  = dataITV;
        shiftedITV.timeVecFloat(dataITV.timeVecFloat().applyOffset(offsetTV));

        // Check to see if i'th itemTimeVec is to be filtered out
        if ( schedule.filterStrategy().exclude( shiftedITV ) ) continue;

        // Insert itemTimeVec into schedule
        schedule.localInsert( shiftedITV );
     }
     // If item DOES NOT appear then insert the dataVec unshifted
     else {
        // Check to see if i'th itemTimeVec is to be filtered out
        if ( schedule.filterStrategy().exclude( dataITV ) ) continue;

        // Insert itemTimeVec into schedule
        schedule.localInsert( dataITV );
     }
  }

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrApplyOffsetScheduleStrategy::clone()
const
{
    LgFrApplyOffsetScheduleStrategy* temp = 
	new LgFrApplyOffsetScheduleStrategy(*this);
    assert( schedules().size() == 2 );
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
LgFrApplyOffsetScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  
  return new LgFrApplyOffsetScheduleStrategy(*this, newScenario);
}


// Assignment operator
LgFrApplyOffsetScheduleStrategy &
LgFrApplyOffsetScheduleStrategy::operator=(
	  const LgFrApplyOffsetScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleSensitiveScheduleStrategy::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrApplyOffsetScheduleStrategy::
   LgFrApplyOffsetScheduleStrategy (
      const LgFrApplyOffsetScheduleStrategy & source)
:  LgFrPartScheduleSensitiveScheduleStrategy( source ), 
   id_(__LGFRAPPLYOFFSETSCHEDULESTRATEGY)
{
}

// Copy constructor with newScenario
LgFrApplyOffsetScheduleStrategy::
   LgFrApplyOffsetScheduleStrategy (
      const LgFrApplyOffsetScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleSensitiveScheduleStrategy( 
         source, 
         newScenario ),
   id_(__LGFRAPPLYOFFSETSCHEDULESTRATEGY)
{
}

// Constructor
LgFrApplyOffsetScheduleStrategy::
   LgFrApplyOffsetScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleSensitiveScheduleStrategy( scenario ),
   id_(__LGFRAPPLYOFFSETSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrApplyOffsetScheduleStrategy::
   LgFrApplyOffsetScheduleStrategy ()
:  LgFrPartScheduleSensitiveScheduleStrategy(),
   id_(__LGFRAPPLYOFFSETSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrApplyOffsetScheduleStrategy::
   ~LgFrApplyOffsetScheduleStrategy ()
{
   // Nothing to do here
}

//-----------------------------------------------------------------------
//
//  Test of LgFrApplyOffsetScheduleStrategy
//
//-----------------------------------------------------------------------

#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrApplyOffsetScheduleStrategy methods.

void
LgFrApplyOffsetScheduleStrategy::test()
{
  LgFrApplyOffsetScheduleStrategy cs1;
  assert( cs1.schedules().size() == 0 );
}


// test LgFrApplyOffsetScheduleStrategy methods.
// Expects testScenario to contain data from
// /u/implode/wit/data/examples/diner12/wit.data
// Expects supplySchedule to be a supply schedule for 
// testScenario and cycleTimeSchedule to be a cycleTime
// schedule with 
// (containing a LgFrSupplyScheduleStrategy)
void
LgFrApplyOffsetScheduleStrategy::contextTest
    (
     LgFrScenario & testScenario,
     LgFrSortingPartScheduleFloatSmartPointer supplySchedule,
     LgFrSortingPartScheduleFloatSmartPointer cycleTimeSchedule
     )
{
std::cerr << "entering LgFrApplyOffsetScheduleStrategy::contextTest" << std::endl;
  int i;
  LgFrApplyOffsetScheduleStrategy cs1;

  // Test inserting single dependent schedule
  cs1.insertPartSchedule( supplySchedule );
  cs1.insertPartSchedule( cycleTimeSchedule );
  assert( cs1.schedules()[0] == supplySchedule );
  assert( cs1.schedules()[1] == cycleTimeSchedule );

  // Test copy constructor
  LgFrApplyOffsetScheduleStrategy cs2(cs1);
  assert( cs2.schedules()[0] == supplySchedule );
  assert( cs2.schedules()[1] == cycleTimeSchedule );

  // Test assignment operator
  LgFrApplyOffsetScheduleStrategy cs3;
  cs3 = cs2;
  assert( cs3.schedules()[0] == supplySchedule );
  assert( cs3.schedules()[1] == cycleTimeSchedule );

  // Test clone method
  {
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( (  (LgFrApplyOffsetScheduleStrategy *) cs4Ptr)
            ->schedules().size() == 2 );

    assert( (  (LgFrApplyOffsetScheduleStrategy *) cs4Ptr)
            ->schedules()[0] 
            != supplySchedule );
    assert( *((  (LgFrApplyOffsetScheduleStrategy *) cs4Ptr)
            ->schedules()[0])
            == *supplySchedule );

    assert( (  (LgFrApplyOffsetScheduleStrategy *) cs4Ptr)
            ->schedules()[1] 
            != cycleTimeSchedule );
    assert( *((  (LgFrApplyOffsetScheduleStrategy *) cs4Ptr)
            ->schedules()[1])
            == *cycleTimeSchedule );
     
    // Test destructor
    delete cs4Ptr;
  }


  // Test Kernal of populate
  const LgFrSortingPartScheduleFloat & constSupplySchedule = *supplySchedule;
  const LgFrSortingPartScheduleFloat & constCycleTimeSchedule = *cycleTimeSchedule;

  LgFrSortingPartScheduleFloat offsetSup;
  const LgFrSortingPartScheduleFloat & constOffsetSup = offsetSup;
  offsetSup.attributeScheduleStrategy( cs1 );
  offsetSup.populate();

  // initial cycleTime is all 0.0's
  for ( i=0; i<supplySchedule->size(); i++ ) 
     assert(constOffsetSup[i].timeVecFloat() == 
                      constSupplySchedule[i].timeVecFloat() );

  // Test subscription mechanism
  LgFrPart part2 = constSupplySchedule[2].part();
  supplySchedule->insertKeyAndValue( part2, LgFrTimeVecFloat(2, 3.1415f) );
  assert(constOffsetSup[part2].timeVecFloat() ==
            LgFrTimeVecFloat(2, 3.1415f));

  // Test scenario copy constructor
  LgFrApplyOffsetScheduleStrategy scenCtor(cs1,testScenario);
  assert( scenCtor.schedules().size() ==  
               cs1.schedules().size() );

  // Test cloning w/scenario
  LgFrAttributeScheduleStrategy *cs6Ptr = cs1.clone(testScenario);
  assert( ((LgFrApplyOffsetScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != supplySchedule );
  assert( ((LgFrApplyOffsetScheduleStrategy *)cs6Ptr)->schedules()[1] 
          != cycleTimeSchedule );
  assert( ((LgFrApplyOffsetScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != cs1.schedules()[0] );
  assert( ((LgFrApplyOffsetScheduleStrategy *)cs6Ptr)->schedules()[1] 
          != cs1.schedules()[1] );
  assert( *(((LgFrApplyOffsetScheduleStrategy *)cs6Ptr)->schedules()[0])
          == *(cs1.schedules()[0]) );
  assert( *(((LgFrApplyOffsetScheduleStrategy *)cs6Ptr)->schedules()[1])
          == *(cs1.schedules()[1]) );

  delete cs6Ptr;
}
