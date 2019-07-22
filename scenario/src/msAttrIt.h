#ifndef LGFR_MULTISCHEDULE_ATTRIBUTEITERATOR_H
#define LGFR_MULTISCHEDULE_ATTRIBUTEITERATOR_H

// This is the minor iterator for a multi-schedule.
// MSAttributeIterator will iterate through the priority list in a ms,
// returning itemTimeVec's for the schedule in the ms and item
// that are used to construct it.

#include <scenario/src/msItemIt.h>

class LgFrItem;
class LgFrItemTimeVecPairFloat;

class LgFrMultiScheduleAttributeIterator {

public:

  // Function call operator
  // Advances iterator index.  Returns NULL iff at end of multi-schedule,
  // otherwise return the pointer to itself (this) after advancing the index.
  LgFrMultiScheduleAttributeIterator*
    operator()();  

  // Return the itemTimeVec for the current index.
  const LgFrItemTimeVecPairFloat * const
  itemTimeVec() const;

  // Constructor
  LgFrMultiScheduleAttributeIterator (const LgFrMultiScheduleItemIterator&);
  
  // Assignment operator
  LgFrMultiScheduleAttributeIterator &
    operator=(const LgFrMultiScheduleAttributeIterator& rhs);

  // Copy constructor
  LgFrMultiScheduleAttributeIterator
    (const LgFrMultiScheduleAttributeIterator &);

  // Default constructor
  LgFrMultiScheduleAttributeIterator ();

  // Destructor
  virtual
    ~LgFrMultiScheduleAttributeIterator ();

  // print
  virtual void print() const;

  // test this class
  static void test();

// ---------------------------------------------------------------------
//  PRIVATE MEMBERS
// --------------------------------------------------------------------- 
private:
  LgFrMultiScheduleItemIterator itemIter_;
  int attrIndex_;
  
};

#endif
