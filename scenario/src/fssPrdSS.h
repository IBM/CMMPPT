#ifndef LGFR_FOCUSSED_SHORTAGE_SCHEDULE_PRODUCTION_SCHEDULE_STRATEGY_H
#define LGFR_FOCUSSED_SHORTAGE_SCHEDULE_PRODUCTION_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/partSS.h>


// Schedule Strategy for a focussed shortage schedule's production schedule.
// The items are parts.
// The timeVecs are the parts' focussed shortage schedule's production schedule.

// Schedules using this strategy will be updated with the new
// focussed shortage schedule when the fss has changed due to 
// an new implosion solution.

// This class follows the Strategy Pattern

// Forward references
class LgFrFocussedShortageScheduleControl;
class LgFrSortingPartScheduleFloat;
class LgFrLastProblemSolved;

class LgFrFocussedShortageScheduleProductionScheduleStrategy : 
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
  LgFrFocussedShortageScheduleProductionScheduleStrategy &
    operator=(
	      const LgFrFocussedShortageScheduleProductionScheduleStrategy& rhs);

  // Copy constructor
  LgFrFocussedShortageScheduleProductionScheduleStrategy (
     const LgFrFocussedShortageScheduleProductionScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrFocussedShortageScheduleProductionScheduleStrategy (
     const LgFrFocussedShortageScheduleProductionScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrFocussedShortageScheduleProductionScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrFocussedShortageScheduleProductionScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrFocussedShortageScheduleProductionScheduleStrategy methods.
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
  LgFrFocussedShortageScheduleProductionScheduleStrategy ();

private:
  const int id_;
  
};

#endif
