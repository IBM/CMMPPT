#ifndef LGFR_EXCESS_VOLUME_SCHEDULE_STRATEGY_H
#define LGFR_EXCESS_VOLUME_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/partSS.h>


// Schedule Strategy for a excess schedule.
// The timeVecs are the parts' excessVolume.

// Schedules using this strategy will be updated with the new
// excess solution after running implode.

// This class follows the Strategy Pattern

// Forward references
class LgFrImplode;
class LgFrSortingPartScheduleFloat;
class LgFrLastProblemSolved;

class LgFrExcessScheduleStrategy : 
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
  LgFrExcessScheduleStrategy &
    operator=(
	      const LgFrExcessScheduleStrategy& rhs);

  // Copy constructor with new Scenario
  LgFrExcessScheduleStrategy (
     const LgFrExcessScheduleStrategy &,
     LgFrScenario &);

  // Copy constructor
  LgFrExcessScheduleStrategy (
     const LgFrExcessScheduleStrategy &);

  // Constructor
  LgFrExcessScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrExcessScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrExcessScheduleStrategy methods.
  // Expects the data in testScenario to come from
  // "/u/implode/wit/data/examples/diner12/wit.data"
  static
  void
  contextTest
      (LgFrScenario          & testScenario,
       LgFrLastProblemSolved & lps,		// testScenario's 
       LgFrSetOfParts        & sop);		// testScenario's

private:

  LgFrImplode * implodePtr_;
  const int id_;

  // Default Constructor
  LgFrExcessScheduleStrategy ();

};

#endif
