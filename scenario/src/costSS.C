// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#include <iostream>

#include <scenario/src/costSS.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/scenario.h>
#include <scenario/src/demdSchd.h>
#include <scenario/src/demdPnPC.h>


// #include <scenario/src/itemTimV.h>
// #include <scenario/src/demdSchd.h>
// #include <scenario/src/demandSS.h>
// #include <scenario/src/sortSchd.h>
// #include <scenario/src/zeroTvFS.h>
// #include <scenario/src/machdep.h>
// NOTE!!! SHOULD WE DEEP COPY LPS??

//-----------------------------------------------------------------------
//
//  Implementation of LgFrPartScheduleTimesUnitCostScheduleStrategy
//
//-----------------------------------------------------------------------


// Populate Schedule with itemTimeVecs
void
LgFrPartScheduleTimesUnitCostScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  assert( schedules().size() == 1 && 
          "PartScheduleTimesUnitCost Schedule Strategy depends on exactly 1 schedule");

  LgFrPart part;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat itv( part, tv );

  const LgFrSortingScheduleFloat & indepSched = *(schedules()[0]);

  // Loop once for each item in independent schedule
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<indepSched.size(); i++ ) {

     // get next itemTimeVec
     itv = indepSched[i];

     float unitCost = lpsPtr_->unitCost(&(itv.part()));

     LgFrTimeVecFloat newtv;
     newtv = itv.timeVecFloat() * unitCost;
       
     // Assign the new timeVec
     itv.timeVecFloat(newtv);

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
  }

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrPartScheduleTimesUnitCostScheduleStrategy::clone()
const
{
    LgFrPartScheduleTimesUnitCostScheduleStrategy* temp = 
	new LgFrPartScheduleTimesUnitCostScheduleStrategy(*this);
    assert( schedules().size() == 1 );
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
LgFrPartScheduleTimesUnitCostScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  
  return new LgFrPartScheduleTimesUnitCostScheduleStrategy(*this, newScenario);
}


// Assignment operator
LgFrPartScheduleTimesUnitCostScheduleStrategy &
LgFrPartScheduleTimesUnitCostScheduleStrategy::operator=(
	  const LgFrPartScheduleTimesUnitCostScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleSensitiveScheduleStrategy::operator= ) (rhs);
    lpsPtr_ = rhs.lpsPtr_;
  }
  return *this;
}


// Copy constructor
LgFrPartScheduleTimesUnitCostScheduleStrategy::
   LgFrPartScheduleTimesUnitCostScheduleStrategy (
      const LgFrPartScheduleTimesUnitCostScheduleStrategy & source)
:  LgFrPartScheduleSensitiveScheduleStrategy( source ),
   lpsPtr_(source.lpsPtr_),
   id_(__LGFRPARTSCHEDULETIMESCOSTSCHEDULESTRATEGY)
{
}

// Copy constructor with newScenario
LgFrPartScheduleTimesUnitCostScheduleStrategy::
   LgFrPartScheduleTimesUnitCostScheduleStrategy (
      const LgFrPartScheduleTimesUnitCostScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleSensitiveScheduleStrategy( 
         source, 
         newScenario ),
   lpsPtr_(&lastProblemSolved(newScenario)),
   id_(__LGFRPARTSCHEDULETIMESCOSTSCHEDULESTRATEGY)
{
}

// Constructor
LgFrPartScheduleTimesUnitCostScheduleStrategy::
   LgFrPartScheduleTimesUnitCostScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleSensitiveScheduleStrategy( scenario ),
   lpsPtr_(&lastProblemSolved(scenario)),
   id_(__LGFRPARTSCHEDULETIMESCOSTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrPartScheduleTimesUnitCostScheduleStrategy::
   LgFrPartScheduleTimesUnitCostScheduleStrategy ()
:  LgFrPartScheduleSensitiveScheduleStrategy(),
   lpsPtr_(0),
   id_(__LGFRPARTSCHEDULETIMESCOSTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrPartScheduleTimesUnitCostScheduleStrategy::
   ~LgFrPartScheduleTimesUnitCostScheduleStrategy ()
{
   // Nothing to do here
}


//-------------------------------------------------------------------------
//
//		Test Methods
//
//-------------------------------------------------------------------------


//-----------------------------------------------------------------------
//
//  Test of LgFrPartScheduleTimesUnitCostScheduleStrategy
//
//-----------------------------------------------------------------------

#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrPartScheduleTimesUnitCostScheduleStrategy methods.

void
LgFrPartScheduleTimesUnitCostScheduleStrategy::test()
{
  LgFrPartScheduleTimesUnitCostScheduleStrategy cs1;
  assert( cs1.schedules().size() == 0 );
}


// test LgFrPartScheduleTimesUnitCostScheduleStrategy methods.
// Expects testScenario to contain data from
// /u/implode/wit/data/examples/diner12/wit.data
// Expects supplySchedule to be a supply schedule for testScenario
// (containing a LgFrSupplyScheduleStrategy)
void
LgFrPartScheduleTimesUnitCostScheduleStrategy::contextTest
    (
     LgFrScenario & testScenario,
     LgFrSortingPartScheduleFloatSmartPointer supplySchedule    
     )
{
  int i;
  LgFrPartScheduleTimesUnitCostScheduleStrategy cs1(testScenario);

  // Test inserting single dependent schedule
  cs1.insertPartSchedule( supplySchedule );
  assert( cs1.schedules()[0] == supplySchedule );

  // Test copy constructor
  LgFrPartScheduleTimesUnitCostScheduleStrategy cs2(cs1);
  assert( cs1.lpsPtr_ == cs2.lpsPtr_);
  assert( cs2.schedules()[0] == supplySchedule );

  // Test assignment operator
  LgFrPartScheduleTimesUnitCostScheduleStrategy cs3;
  cs3 = cs2;
  assert( cs1.lpsPtr_ == cs3.lpsPtr_);
  assert( cs3.schedules()[0] == supplySchedule );

  // Test clone method - shallow copy lps, deep copy schedules
  {
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( ((LgFrPartScheduleTimesUnitCostScheduleStrategy *) cs4Ptr)->lpsPtr_
            == cs1.lpsPtr_);
    assert( (  (LgFrPartScheduleTimesUnitCostScheduleStrategy *) cs4Ptr)
            ->schedules().size() == 1 );
    assert( (  (LgFrPartScheduleTimesUnitCostScheduleStrategy *) cs4Ptr)
            ->schedules()[0] 
            != supplySchedule );
    assert( *((  (LgFrPartScheduleTimesUnitCostScheduleStrategy *) cs4Ptr)
            ->schedules()[0]) 
            == *supplySchedule );
    
    // Test destructor
    delete cs4Ptr;
  }


  // Test Kernal of populate.  The assertion in the loop is correct because
  // every material and capacity in diner12 has a unit cost of 1.
  const LgFrSortingPartScheduleFloat & constSupplySchedule = *supplySchedule;
  LgFrSortingPartScheduleFloat SupTimesUnitCost;
  const LgFrSortingPartScheduleFloat & constSupTimesUnitCost 
      = SupTimesUnitCost;
  SupTimesUnitCost.attributeScheduleStrategy( cs1 );
  SupTimesUnitCost.populate();
  for ( i=0; i<supplySchedule->size(); i++ ) 
     assert( constSupTimesUnitCost[i].timeVecFloat() == 
               constSupplySchedule[i].timeVecFloat());

  // Test subscription mechanism
  LgFrPart part2 = constSupplySchedule[2].part();
  supplySchedule->insertKeyAndValue( part2, LgFrTimeVecFloat(2, 3.1415f) );
  for ( i=0; i<supplySchedule->size(); i++ ) 
     assert( constSupTimesUnitCost[i].timeVecFloat() == 
               constSupplySchedule[i].timeVecFloat() );

  // Test scenario copy constructor
  LgFrPartScheduleTimesUnitCostScheduleStrategy scenCtor(cs1,testScenario);
  assert( scenCtor.schedules().size() ==  
               cs1.schedules().size() );

  // Test cloning w/scenario 
  LgFrAttributeScheduleStrategy *cs6Ptr = cs1.clone(testScenario);
  assert( ((LgFrPartScheduleTimesUnitCostScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != supplySchedule );
  assert( ((LgFrPartScheduleTimesUnitCostScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != cs1.schedules()[0] );
  assert( *(((LgFrPartScheduleTimesUnitCostScheduleStrategy *)cs6Ptr)->schedules()[0])
          == *(cs1.schedules()[0]) );

  delete cs6Ptr;
}

