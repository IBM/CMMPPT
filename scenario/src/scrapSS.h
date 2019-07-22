#ifndef LGFR_SCRAP_VOLUME_SCHEDULE_STRATEGY_H
#define LGFR_SCRAP_VOLUME_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/partSS.h>


// Schedule Strategy for a scrap schedule.
// The timeVecs are the parts' scrapVolume.

// Schedules using this strategy will be updated with the new
// scrap solution after running implode.

// This class follows the Strategy Pattern

// Forward references
class LgFrImplode;
class LgFrSortingPartScheduleFloat;
class LgFrLastProblemSolved;

class LgFrScrapScheduleStrategy : 
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
  LgFrScrapScheduleStrategy &
    operator=(
	      const LgFrScrapScheduleStrategy& rhs);

  // Copy constructor with new Scenario
  LgFrScrapScheduleStrategy (
     const LgFrScrapScheduleStrategy &,
     LgFrScenario &);

  // Copy constructor
  LgFrScrapScheduleStrategy (
     const LgFrScrapScheduleStrategy &);

  // Constructor
  LgFrScrapScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrScrapScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrScrapScheduleStrategy methods.
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
  LgFrScrapScheduleStrategy ();

};

#endif
