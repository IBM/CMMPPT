#ifndef LGFR_CYCLETIME_SCHEDULE_STRATEGY_H
#define LGFR_CYCLETIME_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/partSS.h>

// This file defines two classes:
//      LgFrCycleTimeScheduleStrategy
//      LgFrFrozenZoneScheduleStrategy

// Schedule Strategy for a cycleTime schedule.
// The items are parts.
// The timeVecs are the parts cycleTimeVolume.

// This class follows the Strategy Pattern

// Forward References
class LgFrSortingScheduleFloat;
class LgFrWorkingImplosionProblem;
class LgFrLastProblemSolved;

// ---------------------------------------
//    class LgFrCycleTimeScheduleStrategy
// ---------------------------------------

class LgFrCycleTimeScheduleStrategy : 
   public LgFrPartScheduleStrategy {


public:

  // Populate Schedule with itemTimeVecs
  virtual
     void
        kernalOfPopulate(
           LgFrSortingScheduleFloat& )
              const;
  
  // Update (propogate) a supply change to wip 
  virtual
     void 
        update(
            const LgFrItemTimeVecPairFloat &);
  
  // Is the data associated with this strategy user-mutable?
  virtual
     bool
        isMutable()
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
  LgFrCycleTimeScheduleStrategy &
    operator=(
	      const LgFrCycleTimeScheduleStrategy& rhs);

  // Copy constructor
  LgFrCycleTimeScheduleStrategy (
     const LgFrCycleTimeScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrCycleTimeScheduleStrategy (
     const LgFrCycleTimeScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrCycleTimeScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrCycleTimeScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrCycleTimeScheduleStrategy methods.
  // Expects data for both testScenario and testScenario2 to come from
  // /u/implode/wit/data/examples/diner12/wit.data, but expects them to
  // be two different scenarios.
  static
  void
  contextTest (LgFrScenario                & testScnario,
	       LgFrWorkingImplosionProblem & wip,            // from testScenario
               LgFrLastProblemSolved       & lps,            // from testScenario
               LgFrSetOfParts              & sop);           // from testScenario

private:

  LgFrScenario * scenarioPtr_;
  const int id_;

  // Default Constructor
  LgFrCycleTimeScheduleStrategy ();

};




// ---------------------------------------
//    class LgFrFrozenZoneScheduleStrategy
// ---------------------------------------

class LgFrFrozenZoneScheduleStrategy : 
   public LgFrPartScheduleStrategy {


public:

  // Populate Schedule with itemTimeVecs
  virtual
     void
        kernalOfPopulate(
           LgFrSortingScheduleFloat& )
              const;
  
  // Is the data associated with this strategy user-mutable?
  virtual
     bool
        isMutable()
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
  LgFrFrozenZoneScheduleStrategy &
    operator=(
	      const LgFrFrozenZoneScheduleStrategy& rhs);

  // Copy constructor
  LgFrFrozenZoneScheduleStrategy (
     const LgFrFrozenZoneScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrFrozenZoneScheduleStrategy (
     const LgFrFrozenZoneScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrFrozenZoneScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrFrozenZoneScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrFrozenZoneScheduleStrategy methods.
  // Expects data for both testScenario and testScenario2 to come from
  // /u/implode/wit/data/examples/diner12/wit.data, but expects them to
  // be two different scenarios.
  static
  void
  contextTest (LgFrScenario                & testScnario,
               LgFrWorkingImplosionProblem & wip,            // from testScenario
               LgFrSetOfParts              & sop);           // from testScenario

private:

  LgFrScenario * scenarioPtr_;
  const int id_;

  // Default Constructor
  LgFrFrozenZoneScheduleStrategy ();

};









#endif
