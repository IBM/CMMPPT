#ifndef LGFR_PRODCUTION_VOLUME_SCHEDULE_STRATEGY_H
#define LGFR_PRODCUTION_VOLUME_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/partSS.h>


// Schedule Strategy for a production schedule.
// The items are parts with category PRODUCT.
// The timeVecs are the parts' productionVolume.

// Schedules using this strategy will be updated with the new
// production solution after running implode.

// This class follows the Strategy Pattern

// Forward references
class LgFrImplode;
class LgFrSortingPartScheduleFloat;
class LgFrLastProblemSolved;


class LgFrProductionScheduleStrategy : 
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

  // Make clone copy of this object
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrProductionScheduleStrategy &
    operator=(
	      const LgFrProductionScheduleStrategy& rhs);

  // Copy constructor
  LgFrProductionScheduleStrategy (
     const LgFrProductionScheduleStrategy &);

  // Copy constructor with newScenario
  LgFrProductionScheduleStrategy (
     const LgFrProductionScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrProductionScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrProductionScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrProductionScheduleStrategy methods.
  // test LgFrSupplyScheduleStrategy methods.
  // Expects data for both testScenario and testScenario2 to come from
  // /u/implode/wit/data/examples/diner12/wit.data, but expects them to
  // be two different scenarios.void  static
  static
  void
  contextTest (LgFrScenario                & testScenario,
               LgFrLastProblemSolved       & lps,
               LgFrSetOfParts              & sop,
               LgFrScenario                & testScenario2,
               LgFrLastProblemSolved       & lps2);
private:

  LgFrImplode * implodePtr_;
  const int id_;

  // Default Constructor
  LgFrProductionScheduleStrategy ();
};

#endif
