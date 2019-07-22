#ifndef LGFR_SUPPLY_SCHEDULE_STRATEGY_H
#define LGFR_SUPPLY_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/partSS.h>
#include <scenario/src/wrkgProb.h>

// Schedule Strategy for a supply schedule.
// The items are parts.
// The timeVecs are the parts supplyVolume.

// This class follows the Strategy Pattern

// Forward References
class LgFrSortingScheduleFloat;

class LgFrSupplyScheduleStrategy : 
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

  // Is the proposed vector of values for this schedule all valid?
  virtual
     LgFrValidity
        validity( const LgFrTimeVecFloat& proposedTimeVec )
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
  LgFrSupplyScheduleStrategy &
    operator=(
	      const LgFrSupplyScheduleStrategy& rhs);

  // Copy constructor
  LgFrSupplyScheduleStrategy (
     const LgFrSupplyScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrSupplyScheduleStrategy (
     const LgFrSupplyScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrSupplyScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrSupplyScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrSupplyScheduleStrategy methods.
  // Expects data for both testScenario and testScenario2 to come from
  // /u/implode/wit/data/examples/diner12/wit.data, but expects them to
  // be two different scenarios.
  static
  void
  contextTest (LgFrScenario                & testScnario,
               LgFrWorkingImplosionProblem & wip,            // from testScenario
               LgFrSetOfParts              & sop,            // from testScenario
               LgFrScenario                & testScenario2);

protected:
  // Default Constructor
  LgFrSupplyScheduleStrategy ();
  LgFrWorkingImplosionProblem * wipPtr_;
  
private:
  const int id_;
};

#endif
