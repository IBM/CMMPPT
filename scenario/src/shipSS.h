#ifndef LGFR_SHIPMENT_VOLUME_SCHEDULE_STRATEGY_H
#define LGFR_SHIPMENT_VOLUME_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/demandSS.h>

// Schedule Strategy for a shipment schedule.
// The items are demands
// The timeVecs are the demands' shipmentVolume

// Schedules using this strategy will be updated with the new
// shipment solution after running implode.

// This class follows the Strategy Pattern

// Forward references
class LgFrImplode;
class LgFrSortingDemandScheduleFloat;
class LgFrLastProblemSolved;

class LgFrShipmentScheduleStrategy : 
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
  LgFrShipmentScheduleStrategy &
    operator=(
	      const LgFrShipmentScheduleStrategy& rhs);

  // Copy constructor
  LgFrShipmentScheduleStrategy (
     const LgFrShipmentScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrShipmentScheduleStrategy (
     const LgFrShipmentScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrShipmentScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrShipmentScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrShipmentScheduleStrategy methods.
  // Expects the data in testScenario to come from
  // "/u/implode/wit/data/examples/diner12/wit.data"
  static
  void
  contextTest
    (LgFrScenario			& testScenario,
     LgFrScenario			& testScenario2,
     LgFrLastProblemSolved		& lps,			// testScenario's 
     LgFrSetOfDemands			& sod);			// testScenario's 


protected:

  LgFrImplode * implodePtr_;

  // Default Constructor
  LgFrShipmentScheduleStrategy ();


private:
  const int id_;
};

#endif
