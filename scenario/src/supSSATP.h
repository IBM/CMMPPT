#ifndef LGFR_SUPPLY_SCHEDULE_STRATEGY_FOR_ATP_H
#define LGFR_SUPPLY_SCHEDULE_STRATEGY_FOR_ATP_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/supplySS.h>
#include <scenario/src/wrkgProb.h>
#include <scenario/src/mixInATP.h> // helper class to control state of state machine
#include <scenario/src/stateATP.h>


// Schedule Strategy for a supply schedule.
// The items are parts.
// The timeVecs are the parts supplyVolume.

// This class follows the Strategy Pattern


class LgFrSupplyScheduleStrategyForATP : 
   public LgFrSupplyScheduleStrategy,
   public LgFrScheduleMixInForATP
{

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
  
  // Make clone copy of this object
  virtual
     LgFrAttributeScheduleStrategy *
        clone() 
           const;

  // Make clone copy of this object with new Scenario
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenarioSmartPointer) 
           const;

  // Assignment operator
  LgFrSupplyScheduleStrategyForATP &
    operator=(
	      const LgFrSupplyScheduleStrategyForATP& rhs);

  // Copy constructor
  LgFrSupplyScheduleStrategyForATP (
     const LgFrSupplyScheduleStrategyForATP &);

  // Copy constructor with new Scenario
  LgFrSupplyScheduleStrategyForATP (
     const LgFrSupplyScheduleStrategyForATP &,
     LgFrScenarioSmartPointer);

  // Constructor
  LgFrSupplyScheduleStrategyForATP (
     LgFrScenarioSmartPointer, int desiredState = LGFR_ATP_STATE_COMMITTED_IMPLODED);

  // Destructor
  virtual
     ~LgFrSupplyScheduleStrategyForATP ();

  // test this class
  static void test();

  // test LgFrSupplyScheduleStrategyForATP methods.
  // Expects data for both testScenario and testScenario2 to come from
  // /u/implode/wit/data/examples/diner12/wit.data, but expects them to
  // be two different scenarios.
  static
  void
  contextTest (LgFrScenarioForATPSmartPointer testScnario,
               LgFrWorkingImplosionProblem & wip,            // from testScenario
               LgFrSetOfParts              & sop,            // from testScenario
               LgFrScenarioForATPSmartPointer testScenario2);
private:

  const int id_;

  // Default Constructor
  LgFrSupplyScheduleStrategyForATP ();

  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

};

#endif
