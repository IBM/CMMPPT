#ifndef LGFR_STOCK_VOLUME_SCHEDULE_STRATEGY_H
#define LGFR_STOCK_VOLUME_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/partSS.h>


// Schedule Strategy for a stock schedule.
// The timeVecs are the parts' stockVolume.

// Schedules using this strategy will be updated with the new
// stock solution after running implode.

// This class follows the Strategy Pattern

// Forward references
class LgFrImplode;
class LgFrSortingPartScheduleFloat;
class LgFrLastProblemSolved;

class LgFrStockScheduleStrategy : 
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
  LgFrStockScheduleStrategy &
    operator=(
	      const LgFrStockScheduleStrategy& rhs);

  // Copy constructor with new Scenario
  LgFrStockScheduleStrategy (
     const LgFrStockScheduleStrategy &,
     LgFrScenario &);

  // Copy constructor
  LgFrStockScheduleStrategy (
     const LgFrStockScheduleStrategy &);

  // Constructor
  LgFrStockScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrStockScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrStockScheduleStrategy methods.
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
  LgFrStockScheduleStrategy ();

};

#endif
