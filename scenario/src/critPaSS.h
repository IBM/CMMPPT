#ifndef LGFR_CRITICAL_PART_SCHEDULE_STRATEGY_H
#define LGFR_CRITICAL_PART_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/partSS.h>

// Schedule Strategy for a critical part schedule.
// The items are parts.
// The timeVecs are the parts criticalness.
// The criticalness of the most critical part-period pair is 1.
// If a part-period pair is not critical, then its criticalness is 0.

// Definition of a critical part schedule:
// If the i'th
// critical part is critical in period j, 
// then timeVec[j] = -lenCritList + i for the
// part.
//
// For example consider a 5 period problem with
//   critical list:   Part2  period 4
//                    Part1  period 0
//                    Part2  period 3
//                    Part3  period 2
//   The returned part schedule would be
//      Part2   0  0  0 -2 -4
//      Part1  -3  0  0  0  0
//      Part3   0  0 -1  0  0

// Schedules using this strategy will be updated with a new
// critical part schedule after running implode.

// This class follows the Strategy Pattern

// Forward References
class LgFrImplode;
class LgFrSortingScheduleFloat;
class LgFrLastProblemSolved;

class LgFrCriticalPartScheduleStrategy : 
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
  LgFrCriticalPartScheduleStrategy &
    operator=(
	      const LgFrCriticalPartScheduleStrategy& rhs);

  // Copy constructor
  LgFrCriticalPartScheduleStrategy (
     const LgFrCriticalPartScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrCriticalPartScheduleStrategy (
     const LgFrCriticalPartScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrCriticalPartScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrCriticalPartScheduleStrategy ();

  // test this class
  static void test();
     
  // test LgFrCriticalPartScheduleStrategy methods.
  // Expects the data in testScen to come from
  // "/u/implode/wit/data/examples/diner12/wit.data"
  static
  void
  contextTest(
              LgFrScenario          & testScenario,
              LgFrLastProblemSolved & lps);


private:

  LgFrImplode * implodePtr_;
  const int id_;

  // Default Constructor
  LgFrCriticalPartScheduleStrategy ();

};

#endif
