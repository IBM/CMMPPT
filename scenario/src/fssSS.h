#ifndef LGFR_FOCUSSED_SHORTAGE_SCHEDULE_SCHEDULE_STRATEGY_H
#define LGFR_FOCUSSED_SHORTAGE_SCHEDULE_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/partSS.h>


// Schedule Strategy for a focussed shortage schedule.
// The items are parts.
// The timeVecs are the parts' focussed shortage schedule.

// Schedules using this strategy will be updated with the new
// focussed shortage schedule when the fss has changed due to 
// an new implosion solution.

// This class follows the Strategy Pattern

// Forward references
class LgFrFocussedShortageScheduleControl;
class LgFrSortingPartScheduleFloat;
class LgFrLastProblemSolved;

class LgFrFocussedShortageScheduleScheduleStrategy : 
   public LgFrPartScheduleStrategy {

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
  LgFrFocussedShortageScheduleScheduleStrategy &
    operator=(
	      const LgFrFocussedShortageScheduleScheduleStrategy& rhs);

  // Copy constructor
  LgFrFocussedShortageScheduleScheduleStrategy (
     const LgFrFocussedShortageScheduleScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrFocussedShortageScheduleScheduleStrategy (
     const LgFrFocussedShortageScheduleScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrFocussedShortageScheduleScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrFocussedShortageScheduleScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrFocussedShortageScheduleScheduleStrategy methods.
  // Expects the data in testScenario to come from
  // "/u/implode/wit/data/examples/diner12/wit.data"
  static
  void
  contextTest (LgFrScenario          & testScenario,
               LgFrLastProblemSolved & lps,             // testScenario's
               LgFrSetOfParts        & sop,             // testScenario's
               LgFrSetOfDemands      & sod);            // testScenario's


protected:
  LgFrFocussedShortageScheduleControl * fssCtrlPtr_;

  // Default Constructor
  LgFrFocussedShortageScheduleScheduleStrategy ();

private:
  const int id_;
  
};

#endif
