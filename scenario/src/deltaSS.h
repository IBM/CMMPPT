#ifndef LGFR_TWO_SCHEDULE_DELTA_PART_SCHEDULE_STRATEGY_H
#define LGFR_TWO_SCHEDULE_DELTA_PART_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/schdSnSS.h>


// LgFrTwoScheduleDeltaPartScheduleStrategy:
//   This Schedule Strategy is sensitive to two other
//   schedules.  The schedule produced is the difference, 
//   Schedule1 - Schedule2.

// This class follows the Strategy Pattern

// Forward References
class LgFrSortingScheduleFloat;
class LgFrSortingPartScheduleFloatSmartPointerToConst;


//-----------------------------------------------------------------------
//
//  Definition of LgFrTwoScheduleDeltaPartScheduleStrategy
//
//-----------------------------------------------------------------------

class LgFrTwoScheduleDeltaPartScheduleStrategy : 
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
  LgFrTwoScheduleDeltaPartScheduleStrategy &
    operator=(
	      const LgFrTwoScheduleDeltaPartScheduleStrategy& rhs);

  // Copy constructor
  LgFrTwoScheduleDeltaPartScheduleStrategy (
     const LgFrTwoScheduleDeltaPartScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrTwoScheduleDeltaPartScheduleStrategy (
     const LgFrTwoScheduleDeltaPartScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrTwoScheduleDeltaPartScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrTwoScheduleDeltaPartScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrTwoScheduleDeltaPartScheduleStrategy methods.
  // Expects data from both testScenario to come from
  // /u/implode/wit/data/examples/diner12/wit.data
  static
  void
  contextTest (LgFrScenario                & testScnario,
               LgFrSortingPartScheduleFloatSmartPointer supply);
private:

  const int id_;

  // Default Constructor
  LgFrTwoScheduleDeltaPartScheduleStrategy ();
};


#endif
