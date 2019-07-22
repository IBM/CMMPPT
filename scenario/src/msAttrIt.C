#include <scenario/src/msAttrIt.h>
#include <scenario/src/multiSch.h>

// Function call operator
// Advances iterator index.  Returns NULL iff at end of multi-schedule,
// otherwise return the pointer to itself (this) after advancing the index.
LgFrMultiScheduleAttributeIterator*
  LgFrMultiScheduleAttributeIterator::operator()()
{
  const LgFrMultiScheduleFloat* ms = itemIter_.multiSched();
  int size = (ms->entries())-1;
  // Must be a friend of LgFrMultiSchedleFloat
  if ( attrIndex_ < size ) {
    attrIndex_++;
    return this;
  }
  else {
    attrIndex_ = -1;
    return NULL;
  }
}

// Get the itemTimeVecPairFloat at the current attrIndex_
const LgFrItemTimeVecPairFloat * const
  LgFrMultiScheduleAttributeIterator::itemTimeVec()
     const
{
  const LgFrItem& item = itemIter_.item();
  const LgFrMultiScheduleFloat* ms = itemIter_.multiSched();
  
  // return correct itemTimeVec with the current attrIndex_
  // which is corresponding to the priority in the MS and
  // itemTimeVec can be obtained from the schedule and item.
  // If item is not in the schedule, just return NULL.
  const LgFrSortingScheduleFloat&
    attrSched = *(ms->descSchedPairList().at(attrIndex_)->schedule());
  if ( attrSched.contains(item) ) 
    return &(attrSched[item]);
  else return NULL;
}

// Constructor
LgFrMultiScheduleAttributeIterator::LgFrMultiScheduleAttributeIterator
  (const LgFrMultiScheduleItemIterator & msii)
: itemIter_ (msii),
  attrIndex_ (-1)
{
  // Nothing to do here
}

// Assignment operator
LgFrMultiScheduleAttributeIterator &
  LgFrMultiScheduleAttributeIterator::
    operator=(const LgFrMultiScheduleAttributeIterator& rhs)
{ 
  if (this != &rhs) {		// Check for assignment to self
    itemIter_ = rhs.itemIter_;
    attrIndex_ = rhs.attrIndex_;
  }
  return *this;
}

// Copy constructor
LgFrMultiScheduleAttributeIterator::LgFrMultiScheduleAttributeIterator
    (const LgFrMultiScheduleAttributeIterator & rhs)
: itemIter_(rhs.itemIter_),
  attrIndex_(rhs.attrIndex_)
{
  // nothing to do here
}

// Default constructor
LgFrMultiScheduleAttributeIterator::LgFrMultiScheduleAttributeIterator()
: itemIter_(),
  attrIndex_(-1)
{
  // nothing to do here
}

// Destructor
LgFrMultiScheduleAttributeIterator::~LgFrMultiScheduleAttributeIterator()
{
  // nothing to do here
}

// print
void
LgFrMultiScheduleAttributeIterator::print() const
{
  cout << "\nPrinting the multi-schedule attribute iterator class data:" << endl;
  cout << "attrIndex_ = " << attrIndex_ << endl;
}

#ifdef NDEBUG
#undef NDEBUG
#endif

// test this class
void
LgFrMultiScheduleAttributeIterator::test()
{
  // Test default constructor
  LgFrMultiScheduleAttributeIterator defIter;
  assert(defIter.attrIndex_ == -1 );

  // Test assignment
  LgFrMultiScheduleAttributeIterator assignIter;
  assignIter = defIter;
  assert(assignIter.attrIndex_ == -1 );
  
  // Test copy constructor
  LgFrMultiScheduleAttributeIterator copyIter = defIter;
  assert(copyIter.attrIndex_ == -1 );
}

