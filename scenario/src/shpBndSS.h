#ifndef LGFR_DEMAND_CUMSHIPBOUNDS_SCHEDULE_STRATEGY_H
#define LGFR_DEMAND_CUMSHIPBOUNDS_SCHEDULE_STRATEGY_H

#include <scenario/src/demandSS.h>
#include <scenario/src/wrkgProb.h>

// Schedule Strategy for a ShipSoftLowerBound schedule.
// The items are demands 
// The timeVecs are the demands' ShipSoftLowerBound

// This class follows the Strategy Pattern

// Forward references
class LgFrSortingDemandScheduleFloat;

class LgFrShipSoftLowerBoundScheduleStrategy : 
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
     RWBoolean
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

  // Return Comparison Strategy Identifier
  virtual
     isAReturnType
        isA()
           const;

  // Assignment operator
  LgFrShipSoftLowerBoundScheduleStrategy &
    operator=(
	      const LgFrShipSoftLowerBoundScheduleStrategy& rhs);

  // Copy constructor
  LgFrShipSoftLowerBoundScheduleStrategy (
     const LgFrShipSoftLowerBoundScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrShipSoftLowerBoundScheduleStrategy (
     const LgFrShipSoftLowerBoundScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrShipSoftLowerBoundScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrShipSoftLowerBoundScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrShipSoftLowerBoundScheduleStrategy methods.
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
  LgFrShipSoftLowerBoundScheduleStrategy ();

private:
  const int id_;
	
};

#endif
