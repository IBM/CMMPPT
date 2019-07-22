#ifndef LGFR_DEMAND_PRIORITY_SCHEDULE_STRATEGY_H
#define LGFR_DEMAND_PRIORITY_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/demandSS.h>
#include <scenario/src/wrkgProb.h>

// Schedule Strategy for a priority schedule.
// The items are demands 
// The timeVecs are the demands' priority

// Note: WIT represents priority as integers.
//       LgFr is using floats.

// Note: WIT will generate priorities from objective
//       function coefficients.  This schedule
//       strategy does not subscribe to lps, so
//       the this SS priorities will not be updated
//       when WIT generates the priorities.

// This class follows the Strategy Pattern

// Forward references
class LgFrSortingDemandScheduleFloat;

class LgFrPriorityScheduleStrategy : 
   public LgFrDemandScheduleStrategy {

public:

  // Populate Schedule with itemTimeVecs
  virtual
     void
        kernalOfPopulate(
           LgFrSortingScheduleFloat& )
              const; 

  // Update (propogate) a demand change to wip 
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

  // Make clone copy of this object with a new Scenario
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrPriorityScheduleStrategy &
    operator=(
	      const LgFrPriorityScheduleStrategy& rhs);

  // Copy constructor
  LgFrPriorityScheduleStrategy (
     const LgFrPriorityScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrPriorityScheduleStrategy (
     const LgFrPriorityScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrPriorityScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrPriorityScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrPriorityScheduleStrategy methods.
  // Expects the data in testScenario to come from
  // "/u/implode/wit/data/examples/diner12/wit.data"
  static
  void
  contextTest
    (LgFrScenario                & testScenario,
     LgFrWorkingImplosionProblem & wip,			// testScenario's 
     LgFrSetOfDemands            & sod);		// testScenario's 

protected:

  LgFrWorkingImplosionProblem * wipPtr_;

  // Default Constructor
  LgFrPriorityScheduleStrategy ();

private:
  const int id_;
	
};

#endif
