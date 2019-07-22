// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/usageSS.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/scenario.h>
#include <scenario/src/outInter.h>
#include <scenario/src/demdSchd.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/demdPnPC.h>
#include <math.h>


//-----------------------------------------------------------------------
//
//  Implementation of LgFrUsageScheduleStrategy
//
//-----------------------------------------------------------------------


// Populate Schedule with itemTimeVecs
void
LgFrUsageScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  assert( schedules().size() == 4 && 
          "Usage Schedule Strategy depends on exactly 4 schedules");


  const LgFrSortingScheduleFloat & supplySched = *(schedules()[0]);
  const LgFrSortingScheduleFloat & prodSched   = *(schedules()[1]);
  const LgFrSortingScheduleFloat & stockSched  = *(schedules()[2]);
  const LgFrSortingScheduleFloat & scrapSched  = *(schedules()[3]);

  // Loop once for each item in data schedule
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<supplySched.size(); i++ ) {

     // for each part:
     // usage[t] = supply[t] + prod[t] + stock[t-1] - stock[t] - scrap[t] 
     //   NOTE:  For CAPACITY parts, prod[.] = stock[.] = 0  (this must be
     //          reflected in the schedule).
     //          For RAW parts, prod[.] = 0
     
     // use the timeVec shift operator << applied to stock.

     // get next part
     LgFrPart aPart = supplySched[i].part();

     // initially set usageTV to supplyTV
     LgFrTimeVecFloat usageTV(supplySched[i].timeVecFloat());

     // if there is a prodTV for this part then add it to usageTV
     if (prodSched.contains(aPart)) {
        const LgFrTimeVecFloat & prodTV =  
                         prodSched[aPart].timeVecFloat();
        assert(prodTV.length() == usageTV.length());
        usageTV = usageTV + prodTV;
      }

     // if there is a stock for this part, then shift it and 
     // add the shifted TV and subtract the stockTV
     if (stockSched.contains(aPart)) {
        const LgFrTimeVecFloat & stockTV =  
                         stockSched[aPart].timeVecFloat();
        assert(stockTV.length() == usageTV.length());
        LgFrTimeVecFloat shiftedStockTV = stockTV >> 1;
        usageTV = usageTV + shiftedStockTV - stockTV;
      }

     // if there is a scrapTV for this part then add it to usageTV
     if (scrapSched.contains(aPart)) {
        const LgFrTimeVecFloat & scrapTV =  
                         scrapSched[aPart].timeVecFloat();
        assert(scrapTV.length() == usageTV.length());
        usageTV = usageTV  - scrapTV;
      }

     // Make the thing into an itemTimeVec
     LgFrItemTimeVecPairFloat usageITV(aPart, usageTV);

     // Check to see if the  itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( usageITV ) ) continue;
 
     // Insert itemTimeVec into schedule
     schedule.localInsert( usageITV );
   }
}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrUsageScheduleStrategy::clone()
const
{
    LgFrUsageScheduleStrategy* temp = 
	new LgFrUsageScheduleStrategy(*this);
    assert( schedules().size() == 4 );
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
LgFrUsageScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  
  return new LgFrUsageScheduleStrategy(*this, newScenario);
}


// Assignment operator
LgFrUsageScheduleStrategy &
LgFrUsageScheduleStrategy::operator=(
	  const LgFrUsageScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleSensitiveScheduleStrategy::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrUsageScheduleStrategy::
   LgFrUsageScheduleStrategy (
      const LgFrUsageScheduleStrategy & source)
:  LgFrPartScheduleSensitiveScheduleStrategy( source ), 
   id_(__LGFRUSAGESCHEDULESTRATEGY)
{
}

// Copy constructor with newScenario
LgFrUsageScheduleStrategy::
   LgFrUsageScheduleStrategy (
      const LgFrUsageScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleSensitiveScheduleStrategy( 
         source, 
         newScenario ),
   id_(__LGFRUSAGESCHEDULESTRATEGY)
{
}

// Constructor
LgFrUsageScheduleStrategy::
   LgFrUsageScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleSensitiveScheduleStrategy( scenario ),
   id_(__LGFRUSAGESCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrUsageScheduleStrategy::
   LgFrUsageScheduleStrategy ()
:  LgFrPartScheduleSensitiveScheduleStrategy(),
   id_(__LGFRUSAGESCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrUsageScheduleStrategy::
   ~LgFrUsageScheduleStrategy ()
{
   // Nothing to do here
}

//-----------------------------------------------------------------------
//
//  Test of LgFrUsageScheduleStrategy
//
//-----------------------------------------------------------------------

#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrUsageScheduleStrategy methods.

void
LgFrUsageScheduleStrategy::test()
{
  LgFrUsageScheduleStrategy cs1;
  assert( cs1.schedules().size() == 0 );
}


// test LgFrUsageScheduleStrategy methods.
// Expects testScenario to contain data from
// /u/implode/wit/data/examples/diner12/wit.data
// Expects a supply, prod, stock, and scrap Schedule.
void
LgFrUsageScheduleStrategy::contextTest
              (LgFrScenario                & testScenario,
               LgFrSortingPartScheduleFloatSmartPointer supplySchedule,
               LgFrSortingPartScheduleFloatSmartPointer prodSchedule,
               LgFrSortingPartScheduleFloatSmartPointer stockSchedule,
               LgFrSortingPartScheduleFloatSmartPointer scrapSchedule)

{
  LgFrUsageScheduleStrategy cs1;

  // Test inserting single dependent schedule
  cs1.insertPartSchedule( supplySchedule );
  cs1.insertPartSchedule( prodSchedule );
  cs1.insertPartSchedule( stockSchedule );
  cs1.insertPartSchedule( scrapSchedule );
  assert( cs1.schedules()[0] == supplySchedule );
  assert( cs1.schedules()[1] == prodSchedule );
  assert( cs1.schedules()[2] == stockSchedule );
  assert( cs1.schedules()[3] == scrapSchedule );

  // Test copy constructor
  LgFrUsageScheduleStrategy cs2(cs1);
  assert( cs2.schedules()[0] == supplySchedule );
  assert( cs2.schedules()[1] == prodSchedule );
  assert( cs2.schedules()[2] == stockSchedule );
  assert( cs2.schedules()[3] == scrapSchedule );

  // Test assignment operator
  LgFrUsageScheduleStrategy cs3;
  cs3 = cs2;
  assert( cs3.schedules()[0] == supplySchedule );
  assert( cs3.schedules()[1] == prodSchedule );
  assert( cs3.schedules()[2] == stockSchedule );
  assert( cs3.schedules()[3] == scrapSchedule );

  // Test clone method
  {
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( (  (LgFrUsageScheduleStrategy *) cs4Ptr)
            ->schedules().size() == 4 );

    assert( (  (LgFrUsageScheduleStrategy *) cs4Ptr)
            ->schedules()[0] 
            != supplySchedule );
    assert( *((  (LgFrUsageScheduleStrategy *) cs4Ptr)
            ->schedules()[0])
            == *supplySchedule );

    assert( (  (LgFrUsageScheduleStrategy *) cs4Ptr)
            ->schedules()[1] 
            != prodSchedule );
    assert( *((  (LgFrUsageScheduleStrategy *) cs4Ptr)
            ->schedules()[1]) 
            == *prodSchedule );

    assert( (  (LgFrUsageScheduleStrategy *) cs4Ptr)
            ->schedules()[2] 
            != stockSchedule );
    assert( *((  (LgFrUsageScheduleStrategy *) cs4Ptr)
            ->schedules()[2])
            == *stockSchedule );

    assert( (  (LgFrUsageScheduleStrategy *) cs4Ptr)
            ->schedules()[3] 
            != scrapSchedule );
    assert( *((  (LgFrUsageScheduleStrategy *) cs4Ptr)
            ->schedules()[3])
            == *scrapSchedule );
     
    // Test destructor
    delete cs4Ptr;
  }


  // Test Kernal of populate
  const LgFrSortingPartScheduleFloat & constSupplySchedule = *supplySchedule;
  const LgFrSortingPartScheduleFloat & constProdSchedule   = *prodSchedule;
  const LgFrSortingPartScheduleFloat & constStockSchedule  = *stockSchedule;
  const LgFrSortingPartScheduleFloat & constScrapSchedule  = *scrapSchedule;

  testScenario.newImplosionSolution();

  LgFrSortingPartScheduleFloat usageSched;
  usageSched.attributeScheduleStrategy( cs1 );
  usageSched.populate();
  LgFrUsageScheduleStrategy::quantityTest(usageSched);

  // Test subscription mechanism
//  LgFrPart part2 = constSupplySchedule[2].part();
//  supplySchedule.insertKeyAndValue( part2, LgFrTimeVecFloat(2, 3.1415) );
//  assert(constUsageSched[part2].timeVecFloat() ==
//            LgFrTimeVecFloat(2, 3.1415));

  // Test scenario copy constructor
  LgFrUsageScheduleStrategy scenCtor(cs1,testScenario);
  assert( scenCtor.schedules().size() ==  
               cs1.schedules().size() );

  // Test cloning w/scenario 
  LgFrAttributeScheduleStrategy *cs6Ptr = cs1.clone(testScenario);
  assert( ((LgFrUsageScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != supplySchedule );
  assert( ((LgFrUsageScheduleStrategy *)cs6Ptr)->schedules()[1] 
          != prodSchedule );
  assert( ((LgFrUsageScheduleStrategy *)cs6Ptr)->schedules()[2] 
          != stockSchedule );
  assert( ((LgFrUsageScheduleStrategy *)cs6Ptr)->schedules()[3] 
          != scrapSchedule );
  assert( ((LgFrUsageScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != cs1.schedules()[0] );
  assert( ((LgFrUsageScheduleStrategy *)cs6Ptr)->schedules()[1] 
          != cs1.schedules()[1] );
  assert( ((LgFrUsageScheduleStrategy *)cs6Ptr)->schedules()[2] 
          != cs1.schedules()[2] );
  assert( ((LgFrUsageScheduleStrategy *)cs6Ptr)->schedules()[3] 
          != cs1.schedules()[3] );
  assert( *(((LgFrUsageScheduleStrategy *)cs6Ptr)->schedules()[0])
          == *(cs1.schedules()[0]) );
  assert( *(((LgFrUsageScheduleStrategy *)cs6Ptr)->schedules()[1])
          == *(cs1.schedules()[1]) );
  assert( *(((LgFrUsageScheduleStrategy *)cs6Ptr)->schedules()[2])
          == *(cs1.schedules()[2]) );
  assert( *(((LgFrUsageScheduleStrategy *)cs6Ptr)->schedules()[3])
          == *(cs1.schedules()[3]) );

  delete cs6Ptr;
}


// test quantities in a schedule that uses a LgFrUsageScheduleStrategy.
// Expects the scenario containing constUsageSched to contain data from
// /u/implode/wit/data/examples/diner12/wit.data.  Expects an implosion
// to have been done in the scenario already.
void
LgFrUsageScheduleStrategy::quantityTest(
    const LgFrSortingPartScheduleFloat & constUsageSched)
{ 
    float fltEps = 0.0001f;

    // test a CAPACITY (Skillet) part
    assert(fabs((constUsageSched[0].timeVecFloat())[0] - 12.0) < fltEps);

    // test a RAW part (Chs)
    assert(fabs((constUsageSched[4].timeVecFloat())[0] - 12.0) < fltEps);

    // test a Product (Plain_Sand)
    assert(fabs((constUsageSched[10].timeVecFloat())[0] - 12.0) < fltEps);
}
