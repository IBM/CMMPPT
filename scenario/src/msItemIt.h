#ifndef LGFR_MULTISCHEDULE_ITEMITERATOR_H
#define LGFR_MULTISCHEDULE_ITEMITERATOR_H

// This is the major iterator for a multi-schedule.
// MSItemIterator will iterate through items in the selection schedule.

#include <scenario/src/multiSch.h>

class LgFrMultiScheduleItemIterator {

friend class LgFrMultiScheduleAttributeIterator;
  
public:

  // Function call operator
  // Advances iterator one row.  Returns NULL iff at end of multi-schedule,
  // pointer to *this otherwise.
  LgFrMultiScheduleItemIterator *
    operator()();  

  // Return the current item
  const LgFrItem& item() const;
       
  // Assignment operator
  LgFrMultiScheduleItemIterator &
    operator=(const LgFrMultiScheduleItemIterator& rhs);

  // Copy constructor
  LgFrMultiScheduleItemIterator
    (const LgFrMultiScheduleItemIterator &);

  // Constructor
  LgFrMultiScheduleItemIterator (const LgFrMultiScheduleFloat* ms,
                                 const int startingIndex = 0 );

  // Default constructor
  LgFrMultiScheduleItemIterator ();

  // Destructor
  virtual
    ~LgFrMultiScheduleItemIterator ();

  // print
  virtual void print() const;

  // test this class
  static void test();
  
// ---------------------------------------------------------------------
//  PROTECTED MEMBERS
// ---------------------------------------------------------------------
protected:
  // Return the multi-schedule pointer
  const LgFrMultiScheduleFloat* multiSched() const;
  
// ---------------------------------------------------------------------
//  PRIVATE MEMBERS
// --------------------------------------------------------------------- 
private:
  const LgFrMultiScheduleFloat* ms_;
  int index_;

};

#endif
