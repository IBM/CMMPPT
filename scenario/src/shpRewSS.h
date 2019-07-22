#ifndef LGFR_DEMAND_OBJ1SHIPREWARD_SCHEDULE_STRATEGY_H
#define LGFR_DEMAND_OBJ1SHIPREWARD_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/demandSS.h>
#include <scenario/src/wrkgProb.h>

// Schedule Strategy for a obj1ShipReward schedule.
// The items are demands 
// The timeVecs are the demands' obj1ShipReward

// This class follows the Strategy Pattern

// Forward references
class LgFrSortingDemandScheduleFloat;

class LgFrObj1ShipRewardScheduleStrategy : 
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
  LgFrObj1ShipRewardScheduleStrategy &
    operator=(
	      const LgFrObj1ShipRewardScheduleStrategy& rhs);

  // Copy constructor
  LgFrObj1ShipRewardScheduleStrategy (
     const LgFrObj1ShipRewardScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrObj1ShipRewardScheduleStrategy (
     const LgFrObj1ShipRewardScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrObj1ShipRewardScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrObj1ShipRewardScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrObj1ShipRewardScheduleStrategy methods.
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
  LgFrObj1ShipRewardScheduleStrategy ();

private:
  const int id_;
	
};

#endif
