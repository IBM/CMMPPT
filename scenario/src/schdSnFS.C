// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/schdSnFS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/supplySS.h>
#include <scenario/src/scenario.h>
#include <scenario/src/partSchP.h>

// Do Filtering
bool
LgFrScheduleSensitiveFilterStrategy::exclude(
   const LgFrItemTimeVecPairFloat & itvpf)
const
{
   return ! schedulePtr_->contains( itvpf.item() );
}


// Set Schedule containing values to be used for filtering
void
LgFrScheduleSensitiveFilterStrategy::schedule(
   LgFrSortingScheduleFloatSmartPointer schedule )
{
   if ( !schedulePtr_.null() ) schedulePtr_->removeDependentFromHeterogeneousList( this );
   schedulePtr_ = schedule;
   schedulePtr_->addDependent( this );
}


// Get Schedule containing values to used for filtering
LgFrSortingScheduleFloatSmartPointerToConst
LgFrScheduleSensitiveFilterStrategy::schedule()
const
{
   return schedulePtr_;
}

// Make clone copy of object
LgFrFilterStrategy*
LgFrScheduleSensitiveFilterStrategy::clone()
const
{
  // shallow copy the dependencies
  LgFrScheduleSensitiveFilterStrategy* temp = 
      new LgFrScheduleSensitiveFilterStrategy(*this);



  // remove dependency of schedule
  temp->schedulePtr_->removeDependentFromHeterogeneousList(temp);

  // make a deep copy of the sensitive schedule
  // cast away const
  temp->schedulePtr_ = ((LgFrSortingScheduleFloatSmartPointer&)schedulePtr_).deepCopy();

  // add dependency of new deep copied schedule
  temp->schedulePtr_->addDependent(temp);

  return temp;
}

// Make clone copy of object with new Scenario
LgFrFilterStrategy*
LgFrScheduleSensitiveFilterStrategy::clone(LgFrScenario & newScenario)
const
{
  // shallow copy the dependencies but deep copy the schedule
  LgFrScheduleSensitiveFilterStrategy* temp = 
      new LgFrScheduleSensitiveFilterStrategy(*this, newScenario);
  return temp;
}

// Assignment operator
LgFrScheduleSensitiveFilterStrategy &
LgFrScheduleSensitiveFilterStrategy::operator=(
			  const LgFrScheduleSensitiveFilterStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrFilterStrategy::operator= ) (rhs);
     if ( !schedulePtr_.null() )   {
        schedulePtr_->removeDependentFromHeterogeneousList( this );
      }

     schedulePtr_ = rhs.schedulePtr_;

     if ( !schedulePtr_.null() ) schedulePtr_->addDependent( this );
  }
  return *this;
}

// Copy constructor
LgFrScheduleSensitiveFilterStrategy::
   LgFrScheduleSensitiveFilterStrategy (
      const LgFrScheduleSensitiveFilterStrategy & source)
:  
   LgFrFilterStrategy( source ),
   schedulePtr_( source.schedulePtr_ ),
   id_(__LGFRSCHEDULESENSITIVEFILTERSTRATEGY)
{
  if ( !schedulePtr_.null() ) schedulePtr_->addDependent( this );
}

// Copy constructor with new Scenario
LgFrScheduleSensitiveFilterStrategy::
   LgFrScheduleSensitiveFilterStrategy (
      const LgFrScheduleSensitiveFilterStrategy & source, 
      LgFrScenario & newScenario)
:  
   LgFrFilterStrategy( source, newScenario ),
   schedulePtr_(0),
   id_(__LGFRSCHEDULESENSITIVEFILTERSTRATEGY)
{
  if ( !(source.schedulePtr_.null())  )  {
     schedulePtr_ = (LgFrSortingScheduleFloatSmartPointerToConst)(source.schedulePtr_->clone(newScenario));
     schedulePtr_->addDependent( this );
   }
}


// Default Constructor
LgFrScheduleSensitiveFilterStrategy::
   LgFrScheduleSensitiveFilterStrategy ()
:  
   LgFrFilterStrategy(),
   id_(__LGFRSCHEDULESENSITIVEFILTERSTRATEGY),
   schedulePtr_(0)
{
  // Nothing to do here
}

// Destructor
LgFrScheduleSensitiveFilterStrategy::
   ~LgFrScheduleSensitiveFilterStrategy ()
{
   if ( !schedulePtr_.null() ) schedulePtr_->removeDependentFromHeterogeneousList( this );
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrScheduleSensitiveFilterStrategy methods.
void
LgFrScheduleSensitiveFilterStrategy::test()
{
  LgFrScheduleSensitiveFilterStrategy cs1;
  assert( cs1.schedulePtr_.null() );
}


// test LgFrScheduleSensitiveFilterStrategy methods.
// Expects testScenario to contain data from
// /u/implode/wit/data/brenda/denise/wit.data
// Expects criticalSchedule to be a critical supply schedule for testScenario
// (containing a LgFrCriticalPartScheduleStrategy)
// Expects supplySchedule to be a supply schedule for testScenario
// (containing a LgFrSupplyScheduleStrategy)
void
LgFrScheduleSensitiveFilterStrategy::contextTest
    (
     LgFrScenario & testScenario,
     LgFrLastProblemSolved & lps,
     LgFrSortingPartScheduleFloatSmartPointer criticalSchedule, // these values will be
                                                      // used for filtering
                                                      // a supply
     LgFrSortingPartScheduleFloatSmartPointer supplySchedule    // this schedule will be
                                                      // filtered by
                                                      // criticalSchedule
     )
{
std::cerr << "entering LgFrScheduleSensitiveFilterStrategy::contextTest" << std::endl;
  int i;
  LgFrScheduleSensitiveFilterStrategy cs1;

  // Test setting getting schedule
  assert( cs1.schedule().null() );
  cs1.schedule( criticalSchedule );
  assert( cs1.schedule() == criticalSchedule );

  // Test copy constructor
  LgFrScheduleSensitiveFilterStrategy cs2(cs1);
  assert( cs2.schedule() == criticalSchedule );

  // Test assignment operator
  LgFrScheduleSensitiveFilterStrategy cs3;
  cs3 = cs2;
  assert( cs3.schedule() == criticalSchedule );

  // Test clone method
  {
    LgFrFilterStrategy *cs4Ptr = cs1.clone();

    // since clone is a deep copy, need to compare the actual schedules
    //  item by item
    assert( *(  (LgFrScheduleSensitiveFilterStrategy *) cs4Ptr)
            ->schedule() 
            == *criticalSchedule );
    
    // Test destructor
    delete cs4Ptr;
  }

  // Filter supply scheulde by critical value
  supplySchedule->filterStrategy(cs3);
  supplySchedule->populate();

  // Supply schedule should now be empty, because there aren't any critical
  // parts. ( critical parts only exist after an implosion ).
  assert( supplySchedule->size() == 0 );

  // Do implosion and populate criticalSchedule and supplySchedule.
  lps.computeCriticalList(true);
  testScenario.newImplosionSolution();

  // Test exclude
  LgFrPart tmpPart ("abclksjd");
  LgFrItemTimeVecPairFloat itvp1( tmpPart,
                                  LgFrTimeVecFloat(2,2.) );
  assert( cs1.exclude( itvp1 ) );
  
  assert( criticalSchedule->size()>5 );
  {
    LgFrSortingPartScheduleFloatSmartPointerToConst constCriticalSchedule = criticalSchedule;
    LgFrItemTimeVecPairFloat itvp2( (*constCriticalSchedule)[4].part(), 
                                    LgFrTimeVecFloat(2,2.) );
    assert( !cs1.exclude( itvp2 ) );
  }


  // Test to make sure supply is filtered to exclude non-critical parts
  {
    LgFrSortingPartScheduleFloatSmartPointerToConst constSupplySchedule = supplySchedule;
    for ( i=0; i<supplySchedule->size(); i++ ){
      assert( criticalSchedule->contains( (*constSupplySchedule)[i].item() ) );
    }
  }
  // std::cerr <<"Critical schedule"
  //      <<std::endl
  //      <<criticalSchedule.format()
  //      <<std::endl;
  // std::cerr <<"Supply schedule filtered by Critical parts"
  //      <<std::endl
  //      <<supplySchedule.format(criticalSchedule)
  //      <<std::endl;

  // Test scenario copy constructor - deep copy 
  LgFrScheduleSensitiveFilterStrategy scenCtor(cs1,testScenario);
  assert( !scenCtor.schedulePtr_.null() );
  assert( scenCtor.schedulePtr_ != cs1.schedulePtr_ );
  assert( *(scenCtor.schedulePtr_) == *(cs1.schedulePtr_) );

  // Test copy constructor when filter strategy owns the schedule
  LgFrScheduleSensitiveFilterStrategy cs5(scenCtor);
  assert( cs5.schedule() != criticalSchedule );


  // Test cloning w/scenario 
  //  since cloning is a deep copy, we have to check the schedule
  //    not just the Smart Pointer equality
  LgFrFilterStrategy *cs6Ptr = cs1.clone(testScenario);

// check that the schedules are equivalent but not exactly the same
  assert( *(((LgFrScheduleSensitiveFilterStrategy *)cs6Ptr)->schedule())
          == *criticalSchedule );
  assert( ((LgFrScheduleSensitiveFilterStrategy *)cs6Ptr)->schedule()
          != criticalSchedule );
  assert( *(((LgFrScheduleSensitiveFilterStrategy *)cs6Ptr)->schedule()) 
          == *(cs1.schedule()) );
  assert( ((LgFrScheduleSensitiveFilterStrategy *)cs6Ptr)->schedule()
          != cs1.schedule() );

  delete cs6Ptr;
std::cerr << "exiting LgFrScheduleSensitiveFilterStrategy::contextTest" << std::endl;
}

