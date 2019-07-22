// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/schdSnSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/supplySS.h>
#include <scenario/src/scenario.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/demdSchd.h>
#include <scenario/src/demdSchP.h>

//------------------------------------------------------------------------
//
//   	Implementation of LgFrPartScheduleSensitiveScheduleStrategy
//
//------------------------------------------------------------------------


//estimatedSize
size_t
LgFrPartScheduleSensitiveScheduleStrategy::estimatedSize(
   const LgFrSortingScheduleFloat& schedule )
const
{
  size_t retVal = 0;

  // Set ret value to maximum size of independent schedules
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=1;  i<schedules().size();  i++ ) 
    retVal = size_tMax(
                      retVal,
                      (schedules()[i])->size()
                      );

  if (  schedule.filterStrategy().GetFSType() != __LGFRFILTERSTRATEGY )
     retVal = retVal / 2 + 1;

  retVal = size_tMax(
                    retVal,
                    LgFrAttributeScheduleStrategy::estimatedSize( schedule )
                    );

  return retVal;
}


void
LgFrPartScheduleSensitiveScheduleStrategy::combiningFunction (
    LgFrTimeVecFloatOpFunction func)    
{
    combiningFunction_ = func;
}

// Populate Schedule with itemTimeVecs
void
LgFrPartScheduleSensitiveScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  LgFrSortingScheduleFloat unfilteredSchedule;
  const LgFrSortingScheduleFloat& constUnfilteredSchedule = unfilteredSchedule;
  LgFrItemTimeVecPairFloat itv;

  // Combine the schedules using combiningFunction_
  size_t len = schedules().size();
  switch (len) {
  case 0:
      break;
  case 2:	// special case for better performance
      unfilteredSchedule = (schedules()[0])->op (combiningFunction_,
                                                 *schedules()[1] );
      break;
  default:     
      {
      unfilteredSchedule = *schedules()[0];
      size_t i = 0; // Pulled out of the for below by RW2STL
      for ( i=1;  i<len;  i++ ) 
	  unfilteredSchedule = 
	      unfilteredSchedule.op (combiningFunction_,
	                             *schedules()[i]);
      }
      break;
  }

  // Loop once for each itemTimeVec in the
  // unfiltered schedule and apply filter
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<unfilteredSchedule.size(); i++ ) {

     // Make itemTimeVec
     itv = constUnfilteredSchedule[i];

     // Check to see if j'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
  }

}



// Insert independent Schedule 
void
LgFrPartScheduleSensitiveScheduleStrategy::insertPartSchedule(
   LgFrSortingPartScheduleFloatSmartPointer schedule )
{
   localInsert( schedule );
}

void
LgFrPartScheduleSensitiveScheduleStrategy::insertDemandSchedule(
   LgFrSortingDemandScheduleFloatSmartPointer  )
{
   std::cerr <<"ERROR: LgFrPartScheduleSensitiveScheduleStrategy::insertDemandSchedule"
        <<std::endl;
   std::cerr <<"Base class does not allow mixing demand and part schedules" <<std::endl;
   std::cerr <<"This method is to be overriden in derived classes which" <<std::endl;
   std::cerr <<"support mixing demand and part schedules" <<std::endl;
   abort();

   // In derived classes which support mixing of demand and part schedules
   // code as:
   //    localInsert( schedule );
}


// Insert independent Schedule 
// This is a protected function.
void
LgFrPartScheduleSensitiveScheduleStrategy::localInsert(
   LgFrSortingScheduleFloatSmartPointer schedule )
{
  independentSchedules_.push_back(schedule);
  size_t n = independentSchedules_.size();
  independentSchedules_[n-1] -> addDependent( this );
}

// Get independent Schedule
// This is a protected function.
const LgFrOrderedVectorSortingScheduleFloatSmartPointer &
LgFrPartScheduleSensitiveScheduleStrategy::schedules()
const
{
   return independentSchedules_;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrPartScheduleSensitiveScheduleStrategy::clone()
const
{
  LgFrPartScheduleSensitiveScheduleStrategy* temp = new LgFrPartScheduleSensitiveScheduleStrategy(*this);

  // need to deep copy the independent schedules
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<temp->independentSchedules_.size(); i++ )  {
      // cast away const here to get subscription stuff to work
      temp->independentSchedules_[i]->removeDependentFromHeterogeneousList(temp );
      temp->independentSchedules_[i] = independentSchedules_[i].deepCopy();
      temp->independentSchedules_[i]->addDependent(temp );
  }
  return temp;
}

// Make clone copy of object with new Scenario
LgFrAttributeScheduleStrategy *
LgFrPartScheduleSensitiveScheduleStrategy::clone(LgFrScenario & newScenario)
const
{
  return new LgFrPartScheduleSensitiveScheduleStrategy(*this, newScenario);
}

// Assignment operator
LgFrPartScheduleSensitiveScheduleStrategy &
LgFrPartScheduleSensitiveScheduleStrategy::operator=(
			  const LgFrPartScheduleSensitiveScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrPartScheduleStrategy::operator= ) (rhs);
     
     size_t i = 0; // Pulled out of the for below by RW2STL
     for ( i=0; i<independentSchedules_.size(); i++ ) 
        independentSchedules_[i]->removeDependentFromHeterogeneousList( this );

     independentSchedules_.clear();

     for ( i=0; i<rhs.independentSchedules_.size(); i++ ) 
        // Insert does cloning and addDependent as required.
        localInsert( rhs.independentSchedules_[i] );

  }
  return *this;
}

// Copy constructor - shallow copy
LgFrPartScheduleSensitiveScheduleStrategy::
   LgFrPartScheduleSensitiveScheduleStrategy (
      const LgFrPartScheduleSensitiveScheduleStrategy & source)
:  
   LgFrPartScheduleStrategy( source ),
   independentSchedules_(),
   combiningFunction_(source.combiningFunction_),
   id_(__LGFRPARTSCHEDULESENSITIVESCHEDULESTRATEGY)
{
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<source.independentSchedules_.size(); i++ ) 
     localInsert( source.independentSchedules_[i] );
}

// Copy constructor with new Scenario - deep copy
LgFrPartScheduleSensitiveScheduleStrategy::
   LgFrPartScheduleSensitiveScheduleStrategy (
      const LgFrPartScheduleSensitiveScheduleStrategy & source, 
      LgFrScenario & newScenario)
:  
   LgFrPartScheduleStrategy( 
      source, 
      newScenario ),
   independentSchedules_( ),
   combiningFunction_(source.combiningFunction_),
   id_(__LGFRPARTSCHEDULESENSITIVESCHEDULESTRATEGY)
{
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<source.independentSchedules_.size(); i++ ) 
     localInsert( source.independentSchedules_[i].deepCopy() );
}

// Constructor
LgFrPartScheduleSensitiveScheduleStrategy::
   LgFrPartScheduleSensitiveScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy(
      LgFrAttributeScheduleStrategy::setOfParts(scenario),
      &(implode(scenario)) ),
   independentSchedules_( ),
   combiningFunction_(&LgFrTimeVecFloat::add),
   id_(__LGFRPARTSCHEDULESENSITIVESCHEDULESTRATEGY)
{
  // Nothing to do here
}

static 
  const 
     LgFrSetOfPartsWithMovableParts 
        sopForDefaultConstructor = 
          LgFrSetOfPartsWithMovableParts();

// Default Constructor
// This is a private member function.
LgFrPartScheduleSensitiveScheduleStrategy::
   LgFrPartScheduleSensitiveScheduleStrategy ()
:  LgFrPartScheduleStrategy( sopForDefaultConstructor, 0 ),
   independentSchedules_( ),
   combiningFunction_(&LgFrTimeVecFloat::add),
   id_(__LGFRPARTSCHEDULESENSITIVESCHEDULESTRATEGY)
{
  // Nothing to do here
}


// Destructor
LgFrPartScheduleSensitiveScheduleStrategy::
   ~LgFrPartScheduleSensitiveScheduleStrategy ()
{
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<independentSchedules_.size(); i++ ) 
     independentSchedules_[i]->removeDependentFromHeterogeneousList( this );

  // Removing all of the items from independentSchedules_ explicitly
  // is redundant if the destructor of independentSchedules_ does it,
  // but this way we are sure it is done.
  independentSchedules_.clear();	
}

//------------------------------------------------------------------------
//
//   	Implementation of LgFrDemandScheduleSensitiveScheduleStrategy
//
//------------------------------------------------------------------------


//estimatedSize
size_t
LgFrDemandScheduleSensitiveScheduleStrategy::estimatedSize(
   const LgFrSortingScheduleFloat& schedule )
const
{
  size_t retVal = 0;

  // Set ret value to maximum size of independent schedules
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=1;  i<schedules().size();  i++ )
    retVal = size_tMax(
                      retVal,
                      (schedules()[i])->size()
                      );

  if (  schedule.filterStrategy().GetFSType() != __LGFRFILTERSTRATEGY )
     retVal = retVal / 2 + 1;

  retVal = size_tMax(
                    retVal,
                    LgFrAttributeScheduleStrategy::estimatedSize( schedule )
                    );

  return retVal;
}


// Populate Schedule with itemTimeVecs
void
LgFrDemandScheduleSensitiveScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{

  LgFrSortingScheduleFloat unfilteredSchedule;
  const LgFrSortingScheduleFloat & constUnfilteredSchedule = unfilteredSchedule;
  LgFrItemTimeVecPairFloat itv;

  // Loop once for each independent schedule, adding it to the
  // unfilteredSchedule
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<schedules().size(); i++ ) 
     unfilteredSchedule = 
        (LgFrSortingScheduleFloat) 
           ( unfilteredSchedule + *schedules()[i] );

  // Loop once for each itemTimeVec in the
  // unfiltered schedule and apply filter
  for ( i=0; i<unfilteredSchedule.size(); i++ ) {

     // Make itemTimeVec
     itv = constUnfilteredSchedule[i];

     // Check to see if j'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
  }

}

// Insert independent Schedule 
void
LgFrDemandScheduleSensitiveScheduleStrategy::insertPartSchedule(
   LgFrSortingPartScheduleFloatSmartPointer  )
{
   std::cerr <<"ERROR: LgFrDemandScheduleSensitiveScheduleStrategy::insertDemandSchedule"
        <<std::endl;
   std::cerr <<"Base class does not allow mixing demand and part schedules" <<std::endl;
   std::cerr <<"This method is to be overriden in derived classes which" <<std::endl;
   std::cerr <<"support mixing demand and part schedules" <<std::endl;
   abort();

   // In derived classes which support mixing of demand and part schedules
   // code as:
   //    localInsert( schedule );
}

void
LgFrDemandScheduleSensitiveScheduleStrategy::insertDemandSchedule(
   LgFrSortingDemandScheduleFloatSmartPointer schedule )
{
   localInsert( schedule );
}


// Insert independent Schedule 
// This is a protected function.
void
LgFrDemandScheduleSensitiveScheduleStrategy::localInsert(
   LgFrSortingScheduleFloatSmartPointer schedule )
{
  independentSchedules_.push_back(schedule);
  size_t n = independentSchedules_.size();
  independentSchedules_[n-1] -> addDependent( this );
}


// Get independent Schedule
// This is a protected function.
const LgFrOrderedVectorSortingScheduleFloatSmartPointer &
LgFrDemandScheduleSensitiveScheduleStrategy::schedules()
const
{
   return independentSchedules_;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrDemandScheduleSensitiveScheduleStrategy::clone()
const
{
  LgFrDemandScheduleSensitiveScheduleStrategy* temp = new LgFrDemandScheduleSensitiveScheduleStrategy(*this);

  // need to deep copy the independent schedules


  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<temp->independentSchedules_.size(); i++ )  {
      // cast away const to get things to work here

      // clear out shallow copies of independent schedules
      temp->independentSchedules_[i]->removeDependentFromHeterogeneousList(temp );
      temp->independentSchedules_[i] = independentSchedules_[i].deepCopy();
      temp->independentSchedules_[i]->addDependent(temp );
  }
  return temp;
}

// Make clone copy of object with new Scenario
LgFrAttributeScheduleStrategy *
LgFrDemandScheduleSensitiveScheduleStrategy::clone(LgFrScenario & newScenario)
const
{
  return new LgFrDemandScheduleSensitiveScheduleStrategy(*this, newScenario);
}

// Assignment operator
LgFrDemandScheduleSensitiveScheduleStrategy &
LgFrDemandScheduleSensitiveScheduleStrategy::operator=(
			  const LgFrDemandScheduleSensitiveScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrDemandScheduleStrategy::operator= ) (rhs);
     
     size_t i = 0; // Pulled out of the for below by RW2STL
     for ( i=0; i<independentSchedules_.size(); i++ ) 
        independentSchedules_[i]->removeDependentFromHeterogeneousList( this );

     independentSchedules_.clear();

     for ( i=0; i<rhs.independentSchedules_.size(); i++ ) 
        // Insert does cloning and addDependent as required.
        localInsert( rhs.independentSchedules_[i] );
  }
  return *this;
}

// Copy constructor - shallow copy
LgFrDemandScheduleSensitiveScheduleStrategy::
   LgFrDemandScheduleSensitiveScheduleStrategy (
      const LgFrDemandScheduleSensitiveScheduleStrategy & source)
:  
   LgFrDemandScheduleStrategy( source ),
   independentSchedules_(),
   id_(__LGFRDEMANDSCHEDULESENSITIVESCHEDULESTRATEGY)
{
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<source.independentSchedules_.size(); i++ ) 
     localInsert( source.independentSchedules_[i] );
}

// Copy constructor with new Scenario - deep copy independent schedules!
LgFrDemandScheduleSensitiveScheduleStrategy::
   LgFrDemandScheduleSensitiveScheduleStrategy (
      const LgFrDemandScheduleSensitiveScheduleStrategy & source, 
      LgFrScenario & newScenario)
:  
   LgFrDemandScheduleStrategy( 
      source, 
      newScenario ),
   independentSchedules_( ),
   id_(__LGFRDEMANDSCHEDULESENSITIVESCHEDULESTRATEGY)
{
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<source.independentSchedules_.size(); i++ ) 
     localInsert( source.independentSchedules_[i].deepCopy() );
}
   

// Constructor
LgFrDemandScheduleSensitiveScheduleStrategy::
   LgFrDemandScheduleSensitiveScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrDemandScheduleStrategy(
      LgFrAttributeScheduleStrategy::setOfDemands(scenario),
      &(implode(scenario)) ),
   independentSchedules_( ),
   id_(__LGFRDEMANDSCHEDULESENSITIVESCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrDemandScheduleSensitiveScheduleStrategy::
   LgFrDemandScheduleSensitiveScheduleStrategy ()
:  LgFrDemandScheduleStrategy( LgFrSetOfDemands(), 0 ),
   independentSchedules_( ),
   id_(__LGFRDEMANDSCHEDULESENSITIVESCHEDULESTRATEGY)
{
  // Nothing to do here
}


// Destructor
LgFrDemandScheduleSensitiveScheduleStrategy::
   ~LgFrDemandScheduleSensitiveScheduleStrategy ()
{
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<independentSchedules_.size(); i++ ) 
     independentSchedules_[i]->removeDependentFromHeterogeneousList( this );

  // Removing all of the items from independentSchedules_ explicitly
  // is redundant if the destructor of independentSchedules_ does it,
  // but this way we are sure it is done.
  independentSchedules_.clear();	
}

//------------------------------------------------------------------------
//
//  		Test Methods
//
//------------------------------------------------------------------------

#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrPartScheduleSensitiveScheduleStrategy methods.

void
LgFrPartScheduleSensitiveScheduleStrategy::test()
{
  LgFrPartScheduleSensitiveScheduleStrategy cs1;
  assert( cs1.independentSchedules_.size() == 0 );
}


// test LgFrPartScheduleSensitiveScheduleStrategy methods.
// Expects testScenario to contain data from
// /u/implode/wit/data/examples/diner12/wit.data
// Expects supplySchedule to be a supply schedule for testScenario
// (containing a LgFrSupplyScheduleStrategy)
void
LgFrPartScheduleSensitiveScheduleStrategy::contextTest
    (
     LgFrScenario & testScenario,
     LgFrSortingPartScheduleFloatSmartPointer supplySchedule    
     )
{
  LgFrPartScheduleSensitiveScheduleStrategy cs1;

  // Test inserting single dependent schedule
  cs1.insertPartSchedule( supplySchedule );
  assert( cs1.independentSchedules_[0] == supplySchedule );

  // Test copy constructor
  LgFrPartScheduleSensitiveScheduleStrategy cs2(cs1);
  assert( cs2.independentSchedules_[0] == supplySchedule );

  // Test inserting schedules
  assert( cs2.independentSchedules_.size() == 1 );
  cs2.insertPartSchedule( supplySchedule );
  assert( cs2.independentSchedules_[0] == supplySchedule );
  assert( cs2.independentSchedules_[1] == supplySchedule );
  assert( cs2.independentSchedules_.size() == 2 );

  // Test assignment operator
  LgFrPartScheduleSensitiveScheduleStrategy cs3;
  cs3 = cs2;
  assert( cs3.independentSchedules_[1] == supplySchedule );

  // Test clone method
  {
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( (  (LgFrPartScheduleSensitiveScheduleStrategy *) cs4Ptr)
            ->independentSchedules_.size() == 1 );
    // check equality of content - deep copy of schedules
    assert( *((  (LgFrPartScheduleSensitiveScheduleStrategy *) cs4Ptr))
            ->independentSchedules_[0]
            == *supplySchedule );
    
    // Test destructor
    delete cs4Ptr;
  }


  // Test Kernal of populate
  LgFrSortingPartScheduleFloatSmartPointer sup(new LgFrSortingPartScheduleFloat);
  sup->attributeScheduleStrategy( cs1 );
  sup->populate();
// check that the schedules are equivalent
  assert( *sup == *supplySchedule );  


  // Test kernalOfPopulate when this class is being used to subtract two
  // schedules
  {
      // Set up the strategy
      LgFrSortingPartScheduleFloatSmartPointer supplySchedule2
	 (new LgFrSortingPartScheduleFloat(*supplySchedule + *supplySchedule));
      LgFrPartScheduleSensitiveScheduleStrategy cs1;
      cs1.insertPartSchedule( supplySchedule ); 
      cs1.insertPartSchedule( supplySchedule2 );
      cs1.combiningFunction (&LgFrTimeVecFloat::subtract);

      // Set up the schedule that uses the strategy
      LgFrSortingPartScheduleFloat deltaSup;
      deltaSup.attributeScheduleStrategy( cs1 );
      deltaSup.populate();

      // Compare deltaSup to what it should be
      const LgFrSortingPartScheduleFloat & constSupplySchedule
	  = *supplySchedule;
      const LgFrSortingPartScheduleFloat & constSupplySchedule2 
	  = *supplySchedule2;

      const LgFrSortingPartScheduleFloat & constDeltaSup = deltaSup;
      int i = 0; // Pulled out of the for below by RW2STL
      for (i=0; i<supplySchedule->size(); i++ ) 
	  assert(         constDeltaSup[i].timeVecFloat() == 
			  (constSupplySchedule[i].timeVecFloat() - 
			   constSupplySchedule2[i].timeVecFloat()
			  )
	        );
  }

  // Test subscription mechanism
  const LgFrSortingPartScheduleFloat& constSupplySchedule = *supplySchedule;
  LgFrPart part2 = constSupplySchedule[2].part();
  supplySchedule->insertKeyAndValue( part2, LgFrTimeVecFloat(2, 3.1415f) );
// check that the schedules are equivalent
  assert( *sup == *supplySchedule );

  // Test scenario copy constructor - deep copies the dependent schedules over
  LgFrPartScheduleSensitiveScheduleStrategy scenCtor(cs1,testScenario);
  assert( scenCtor.independentSchedules_.size() ==  
               cs1.independentSchedules_.size() );

  // Test cloning w/scenario - this deep copies the dependent schedules over
  LgFrAttributeScheduleStrategy *cs6Ptr = cs1.clone(testScenario);
  // schedules do not have same address but have the same content
  assert( ((LgFrPartScheduleSensitiveScheduleStrategy *)cs6Ptr)->independentSchedules_[0] 
          != supplySchedule );
  assert( *(((LgFrPartScheduleSensitiveScheduleStrategy *)cs6Ptr)->independentSchedules_[0])
          == *supplySchedule );
  assert( ((LgFrPartScheduleSensitiveScheduleStrategy *)cs6Ptr)->independentSchedules_[0] 
          != cs1.independentSchedules_[0] );
  assert( *(((LgFrPartScheduleSensitiveScheduleStrategy *)cs6Ptr)->independentSchedules_[0])
          == *(cs1.independentSchedules_[0]) );


  // Test insertion methods that give ownershipp to the ASS.  If there are
  // memory leaks from this block only, there is something wrong with them or
  // with the destructor.
#ifdef DONE_TESTING_COPYING
  {
    LgFrPartScheduleSensitiveScheduleStrategy * cs8ptr
      = new LgFrPartScheduleSensitiveScheduleStrategy (testScenario);
    
    LgFrSortingPartScheduleFloat * ns1 = new
      LgFrSortingPartScheduleFloat(supplySchedule); // deleting cs8ptr should
                                                    // delete this
    cs8ptr->insertPartSchedule( *ns1 );
    assert( cs8ptr->independentSchedules_[0] == ns1 );
    LgFrSortingPartScheduleFloat * ns2 = new
      LgFrSortingPartScheduleFloat(supplySchedule); // deleting cs8ptr should
                                                    // delete this
    cs8ptr->insertPartSchedule( *ns2 );
    assert( cs8ptr->independentSchedules_[1] == ns2 );
    assert( cs8ptr->independentSchedules_.size() == 2 );
    delete cs8ptr;
  }
#endif
  
  delete cs6Ptr;
}


// test LgFrDemandScheduleSensitiveScheduleStrategy methods.

void
LgFrDemandScheduleSensitiveScheduleStrategy::test()
{
  LgFrDemandScheduleSensitiveScheduleStrategy cs1;
  assert( cs1.independentSchedules_.size() == 0 );
}


// test LgFrDemandScheduleSensitiveScheduleStrategy methods.
// Expects testScenario to contain data from
// /u/implode/wit/data/examples/diner12/wit.data
// Expects demandVolSchedule to be a supply schedule for testScenario
// (containing a LgFrSupplyScheduleStrategy)
void
LgFrDemandScheduleSensitiveScheduleStrategy::contextTest
    (
     LgFrScenario & testScenario,
     LgFrSortingDemandScheduleFloatSmartPointer demandVolSchedule    
     )
{
  LgFrDemandScheduleSensitiveScheduleStrategy cs1;

  // Test inserting single dependent schedule
  cs1.insertDemandSchedule( demandVolSchedule );
  assert( cs1.independentSchedules_[0] == demandVolSchedule );

  // Test copy constructor
  LgFrDemandScheduleSensitiveScheduleStrategy cs2(cs1);
  assert( cs2.independentSchedules_[0] == demandVolSchedule );

  // Test inserting schedules
  assert( cs2.independentSchedules_.size() == 1 );
  cs2.insertDemandSchedule( demandVolSchedule );
  assert( cs2.independentSchedules_[0] == demandVolSchedule );
  assert( cs2.independentSchedules_[1] == demandVolSchedule );
  assert( cs2.independentSchedules_.size() == 2 );

  // Test assignment operator
  LgFrDemandScheduleSensitiveScheduleStrategy cs3;
  cs3 = cs2;
  assert( cs3.independentSchedules_[1] == demandVolSchedule );

  // Test clone method - deep copy schedules
  {
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( (  (LgFrDemandScheduleSensitiveScheduleStrategy *) cs4Ptr)
            ->independentSchedules_.size() == 1 );
    // check contents - deep copied schedules
    assert( *((  (LgFrDemandScheduleSensitiveScheduleStrategy *) cs4Ptr))
            ->independentSchedules_[0] 
            == *demandVolSchedule );
    
    // Test destructor
    delete cs4Ptr;
  }


  // Test Kernal of populate
  LgFrSortingDemandScheduleFloat sup;
  sup.attributeScheduleStrategy( cs1 );
  sup.populate();
  assert( sup == *demandVolSchedule );

  // Test subscription mechanism
  const LgFrSortingDemandScheduleFloat & constSupplySchedule = *demandVolSchedule;
  LgFrDemand part2 = constSupplySchedule[2].demand();
  demandVolSchedule->insertKeyAndValue( part2, LgFrTimeVecFloat(2, 3.1415f) );
  assert( sup == *demandVolSchedule );

  // Test scenario copy constructor - deep copy
  LgFrDemandScheduleSensitiveScheduleStrategy scenCtor(cs1,testScenario);
  assert( scenCtor.independentSchedules_.size() ==  
               cs1.independentSchedules_.size() );

  // Test cloning w/scenario  - deep copy
  LgFrAttributeScheduleStrategy *cs6Ptr = cs1.clone(testScenario);
  assert( ((LgFrDemandScheduleSensitiveScheduleStrategy *)cs6Ptr)->independentSchedules_[0] 
          != demandVolSchedule );
  assert( ((LgFrDemandScheduleSensitiveScheduleStrategy *)cs6Ptr)->independentSchedules_[0] 
          != cs1.independentSchedules_[0] );
  assert( *(((LgFrDemandScheduleSensitiveScheduleStrategy *)cs6Ptr)->independentSchedules_[0])
          == *(cs1.independentSchedules_[0]) );

  delete cs6Ptr;


}
