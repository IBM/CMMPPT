#ifndef LGFR_SHIPMENT_VOLUME_SCHEDULE_STRATEGY_FOR_ATP_H
#define LGFR_SHIPMENT_VOLUME_SCHEDULE_STRATEGY_FOR_ATP_H

#include <scenario/src/shipSS.h>
#include <scenario/src/mixInATP.h>
#include <scenario/src/stateATP.h>

// Schedule Strategy for a shipment schedule.
// The items are demands
// The timeVecs are the demands' shipmentVolume

// Schedules using this strategy will be updated with the new
// shipment solution after running implode.

// This class follows the Strategy Pattern


class LgFrShipmentScheduleStrategyForATP : 
   public LgFrShipmentScheduleStrategy,
   public LgFrScheduleMixInForATP
{

public:

  // Override base class populate method.
  virtual
     void
        populate(
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
        clone(LgFrScenarioSmartPointer) 
           const;



  // Return Comparison Strategy Identifier
  virtual
     isAReturnType
        isA()
           const;

  // Assignment operator
  LgFrShipmentScheduleStrategyForATP &
    operator=(
	      const LgFrShipmentScheduleStrategyForATP& rhs);

  // Copy constructor
  LgFrShipmentScheduleStrategyForATP (
     const LgFrShipmentScheduleStrategyForATP &);

  // Copy constructor with new Scenario
  LgFrShipmentScheduleStrategyForATP (
     const LgFrShipmentScheduleStrategyForATP &,
     LgFrScenarioSmartPointer);

  // Constructor
  LgFrShipmentScheduleStrategyForATP (
     LgFrScenarioSmartPointer, int desiredState = STATE_INIT );

  // Destructor
  virtual
     ~LgFrShipmentScheduleStrategyForATP ();

  // test this class
  static void test();

  // test LgFrShipmentScheduleStrategyForATP methods.
  // Expects the data in testScenario to come from
  // "/u/implode/wit/data/examples/diner12/wit.data"
  static
  void
  contextTest
    (LgFrScenarioForATPSmartPointer	testScenario,
     LgFrLastProblemSolved		& lps,			// testScenario's 
     LgFrSetOfDemands			& sod);			// testScenario's 


private:
  const int id_;

  // Default Constructor
  LgFrShipmentScheduleStrategyForATP ();

  // should not be used
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

};

#endif
