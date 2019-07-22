#ifndef LGFR_DEMAND_VOLUME_SCHEDULE_STRATEGY_H
#define LGFR_DEMAND_VOLUME_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/demandSS.h>
#include <scenario/src/wrkgProb.h>

// Schedule Strategy for a DemandVolume schedule.
// The items are demands 
// The timeVecs are the demands' demandVolume

// This class follows the Strategy Pattern

// Forward references
class LgFrSortingDemandScheduleFloat;

class LgFrDemandVolumeScheduleStrategy : 
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

  // Make clone copy of this object with a new Scenario
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrDemandVolumeScheduleStrategy &
    operator=(
	      const LgFrDemandVolumeScheduleStrategy& rhs);

  // Copy constructor
  LgFrDemandVolumeScheduleStrategy (
     const LgFrDemandVolumeScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrDemandVolumeScheduleStrategy (
     const LgFrDemandVolumeScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrDemandVolumeScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrDemandVolumeScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrDemandVolumeScheduleStrategy methods.
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
  LgFrDemandVolumeScheduleStrategy ();

private:
  const int id_;
	
};

#endif
