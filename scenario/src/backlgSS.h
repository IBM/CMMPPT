#ifndef LGFR_BACKLOG_VOLUME_SCHEDULE_STRATEGY_H
#define LGFR_BACKLOG_VOLUME_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/demandSS.h>


// Schedule Strategy for a backlog schedule.
// The items are demands
// The timeVecs are the demands' backlog

// Schedules using this strategy will be updated with the new
// shipment solution after running implode.

// This class follows the Strategy Pattern

// Forward references
class LgFrScenario;
class LgFrImplode;
class LgFrSortingDemandScheduleFloat;
class LgFrLastProblemSolved;

class LgFrBacklogScheduleStrategy : 
   public LgFrDemandScheduleStrategy {

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
  LgFrBacklogScheduleStrategy &
    operator=(
	      const LgFrBacklogScheduleStrategy& rhs);

  // Copy constructor
  LgFrBacklogScheduleStrategy (
     const LgFrBacklogScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrBacklogScheduleStrategy (
     const LgFrBacklogScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrBacklogScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrBacklogScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrBacklogScheduleStrategy methods.
  // Expects the data in testScenario to come from
  // "/u/implode/wit/data/examples/diner12/wit.data"
  static
  void
  contextTest
    (
     LgFrScenario          & testScenario,
     LgFrLastProblemSolved & lps,			// testScenario's 
     LgFrSetOfDemands      & sod);			// testScenario's 

protected:

  // Default Constructor
  LgFrBacklogScheduleStrategy ();

private:

  LgFrImplode * implodePtr_;
  const int id_;

};

#endif
