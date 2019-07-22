#ifndef LGFR_DEMAND_VOLUME_SCHEDULE_STRATEGY_FOR_ATP_H
#define LGFR_DEMAND_VOLUME_SCHEDULE_STRATEGY_FOR_ATP_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/demVolSS.h>
#include <scenario/src/wrkgProb.h>
#include <scenario/src/mixInATP.h> // helper class to control state of state machine
#include <scenario/src/stateATP.h>

// Schedule Strategy for a demand schedule.
// The items are parts.
// The timeVecs are the parts demandVolume.

// This class follows the Strategy Pattern


class LgFrDemandVolumeScheduleStrategyForATP : 
   public LgFrDemandVolumeScheduleStrategy,
   public LgFrScheduleMixInForATP
{

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
  LgFrDemandVolumeScheduleStrategyForATP &
    operator=(
	      const LgFrDemandVolumeScheduleStrategyForATP& rhs);

  // Copy constructor
  LgFrDemandVolumeScheduleStrategyForATP (
     const LgFrDemandVolumeScheduleStrategyForATP &);

  // Copy constructor with new Scenario
  LgFrDemandVolumeScheduleStrategyForATP (
     const LgFrDemandVolumeScheduleStrategyForATP &,
     LgFrScenarioSmartPointer);

  // Constructor
  LgFrDemandVolumeScheduleStrategyForATP (
     LgFrScenarioSmartPointer, int desiredState = STATE_INIT);

  // Destructor
  virtual
     ~LgFrDemandVolumeScheduleStrategyForATP ();

  // test this class
  static void test();

  // test LgFrDemandVolumeScheduleStrategyForATP methods.
  // Expects data for both testScenario and testScenario2 to come from
  // /u/implode/wit/data/examples/diner12/wit.data, but expects them to
  // be two different scenarios.
  static
  void
  contextTest (LgFrScenarioForATPSmartPointer testScnario,
               LgFrWorkingImplosionProblem & wip,            // from testScenario
               LgFrSetOfDemands            & sod,            // from testScenario
               LgFrScenarioForATPSmartPointer testScenario2);
private:
  const int id_;

  // Default Constructor
  LgFrDemandVolumeScheduleStrategyForATP ();

    // should not use this method
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

};

#endif
