#include <scenario/src/msItemIt.h>
#include <scenario/src/sortSchP.h>
#include <scenario/src/partSchP.h>

// Return the multi-schedule pointer
const LgFrMultiScheduleFloat*
  LgFrMultiScheduleItemIterator::multiSched()
     const
{
  return ms_;
}

// Return the current item 
const LgFrItem&
  LgFrMultiScheduleItemIterator::item()
     const
{
  // Get correct item for itemIndex_ from the selection schedule
  // in the multi-schedule
  LgFrSortingScheduleFloatSmartPointer selectionSched = 
      ms_->schedule(ms_->itemSelectionSchedule());

  // This method needs to return a const ref of item which comes from itv.
  // Therefore, we have to cast the (const) selectionSched to non-const
  // in order to access the protected operator[index] which would return
  // a (non-const) ref of itv.
  // Remember to have const ref at left hand side for safety.
  const LgFrItemTimeVecPairFloat& selectITV =
    ( (LgFrSortingScheduleFloat&)(*selectionSched) ) [index_];
  const LgFrItem& item = selectITV.item();
  return item;
}

// Function call operator
LgFrMultiScheduleItemIterator*
  LgFrMultiScheduleItemIterator::operator()()
{
  int size =  (ms_->schedule(ms_->itemSelectionSchedule())->entries())-1;
  // must be a friend of LgFrMultiSchedleFloat
  if ( index_ < size ) {
    index_ ++;
    return this;
  }
  else {
    index_ = -1;
    return NULL;
  }
}  

// Assignment operator
LgFrMultiScheduleItemIterator &
  LgFrMultiScheduleItemIterator::
  operator=(const LgFrMultiScheduleItemIterator& rhs)
{ 
  if (this != &rhs) {		// Check for assignment to self
    ms_ = rhs.ms_;
    index_ = rhs.index_;
  }
  return *this;
}

// Copy constructor
LgFrMultiScheduleItemIterator::LgFrMultiScheduleItemIterator
    (const LgFrMultiScheduleItemIterator & rhs)
: ms_(rhs.ms_),
  index_(rhs.index_)
{
  // nothing to do here
}

// Constructor
LgFrMultiScheduleItemIterator::LgFrMultiScheduleItemIterator
  (const LgFrMultiScheduleFloat* ms,
   const int startingIndex )
: ms_(ms),
  index_(startingIndex - 1)
{
  // nothing to do here
}

// Default constructor
LgFrMultiScheduleItemIterator::LgFrMultiScheduleItemIterator()
: ms_(NULL),
  index_(-1)
{
  // nothing to do here
}

// Destructor
LgFrMultiScheduleItemIterator::~LgFrMultiScheduleItemIterator()
{
  // nothing to do here
}

// print
void
LgFrMultiScheduleItemIterator::print() const
{
  cout << "\nPrinting the multi-schedule item iterator class data:" << endl;
  cout << "index_ = " << index_ << endl;
}

#ifdef NDEBUG
#undef NDEBUG
#endif

// test this class
void
LgFrMultiScheduleItemIterator::test()
{
  // Test default constructor
  LgFrMultiScheduleItemIterator defIter;
  assert(defIter.index_ == -1 );
  assert(defIter.ms_ == NULL);

  // Test assignment
  LgFrMultiScheduleItemIterator assignIter;
  assignIter = defIter;
  assert(assignIter.index_ == -1 );
  assert(assignIter.ms_ == NULL);
  
  // Test copy constructor
  LgFrMultiScheduleItemIterator copyIter = defIter;
  assert(copyIter.index_ == -1 );
  assert(copyIter.ms_ == NULL);
  
}
#if 0
// test this class with multi-schedule (contextTest)

void
LgFrMultiScheduleItemIterator::contextTest(const LgFrMultiScheduleFloat& ms)
{

  // Make objects needed by the tests
  LgFrFileDataInterface
    obj1Prob ("../../../../wit/wit-data/standard/obj1/wit.data");
  LgFrFileDataInterface
    diner12Prob ("../../../../wit/wit-data/examples/diner12/wit.data");
  LgFrCalendar calInst;
  LgFrScenarioBuilderForInteractive builder;
  LgFrScenarioDirector director(&builder);

  // Create an instance using the builder
  director.construct(obj1Prob, calInst);
  LgFrInteractiveScenario * testScenarioPtr = builder.getScenario();
//  assert (testScenarioPtr->setOfParts().entries() == 10);

  // Implode
  testScenarioPtr->newImplosionSolution();		// run implosion
  
  // Create multi...
  LgFrMultiPartScheduleFloat msp (testScenarioPtr);

  // Put in a schedule
  msp.newSchedule (LGFRSupplyVolume);
  msp.newSchedule (LGFRProductionVolume);
  assert ( msp.entries ()  ==  2);

  // Create an item (major) iterator
  LgFrMultiScheduleItemIterator majorI (&msp);
  LgFrMultiScheduleItemIterator* nextItemIterPtr;
  LgFrMultiScheduleItemIterator* lastItemIterPtr;

  // Iterate through all of the (major) rows
  while ( nextItemIterPtr = majorI()) {
    nextItemIterPtr->print();
//    const LgFrPart* part = (const LgFrPart*)nextItemIterPtr->item();
//    part->print(); 
    
    // Create an attribute (minor) iterator
    LgFrMultiScheduleAttributeIterator minorI (*nextItemIterPtr);
    LgFrMultiScheduleAttributeIterator* nextAttrIterPtr;
    const LgFrItemTimeVecPairFloat* curITV;
    while ( nextAttrIterPtr = minorI() ) {   // Not end of list
      nextAttrIterPtr->print();
      if ( curITV = nextAttrIterPtr->itemTimeVec() ) {
        curITV->print();
      }
    }
    lastItemIterPtr = nextItemIterPtr;
  }

  // Test alternate constructor
  LgFrMultiScheduleItemIterator majorI2 (&msp,1);
  assert( lastItemIterPtr == majorI() );
  assert( NULL = majorI() );
  
  // Try changing a schedule
//  msp.changeValueInSchedule (item, LGFRSupplyVolume, 1, 17);

  msp.clear();


}
#endif
