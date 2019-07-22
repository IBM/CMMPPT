#ifndef LGFR_APPLY_OFFSET_SCHEDULE_STRATEGY_H
#define LGFR_APPLY_OFFSET_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/schdSnSS.h>


// LgFrApplyOffsetScheduleStrategy:
//   This Schedule Strategy is sensitive to two other
//   schedules.  The first will be offset by the second.
//   The second  must be some sort of cycleTime of leadTime 
//   schedule.  The offsets will be rounded to the nearest
//   integer.

// This class follows the Strategy Pattern

// Forward References
class LgFrSortingScheduleFloat;
class LgFrSortingPartScheduleFloat;
class LgFrSortingPartScheduleFloatSmartPointer;


//-----------------------------------------------------------------------
//
//  Definition of LgFrApplyOffsetScheduleStrategy
//
//-----------------------------------------------------------------------

class LgFrApplyOffsetScheduleStrategy : 
   public LgFrPartScheduleSensitiveScheduleStrategy {

public:

  // Populate Schedule with itemTimeVecs
  virtual
     void
        kernalOfPopulate(
           LgFrSortingScheduleFloat& )
              const;
  
  // Make clone copy of this object
  virtual
     LgFrAttributeScheduleStrategy *
        clone() 
           const;

  // Make clone copy of this object with new Scenario
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrApplyOffsetScheduleStrategy &
    operator=(
	      const LgFrApplyOffsetScheduleStrategy& rhs);

  // Copy constructor
  LgFrApplyOffsetScheduleStrategy (
     const LgFrApplyOffsetScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrApplyOffsetScheduleStrategy (
     const LgFrApplyOffsetScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrApplyOffsetScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrApplyOffsetScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrApplyOffsetScheduleStrategy methods.
  // Expects data from both testScenario to come from
  // /u/implode/wit/data/examples/diner12/wit.data
  static
  void
  contextTest (LgFrScenario                & testScnario,
               LgFrSortingPartScheduleFloatSmartPointer supply,
               LgFrSortingPartScheduleFloatSmartPointer cycleTime);
private:

  const int id_;

  // Default Constructor
  LgFrApplyOffsetScheduleStrategy ();
};


#endif
