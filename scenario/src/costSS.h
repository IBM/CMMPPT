#ifndef LGFR_COST_SCHEDULE_STRATEGY_H
#define LGFR_COST_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/schdSnSS.h>


// LgFrPartScheduleTimesUnitCostScheduleStrategy:
//    The independent schedule is a part schedule.
//    The resulting dependent schedule is a part schedule times the unitCost 

// Forward References
class LgFrSortingScheduleFloat;
class LgFrSortingPartScheduleFloatSmartPointer;


//-----------------------------------------------------------------------
//
//  Definition of LgFrPartScheduleTimesUnitCostScheduleStrategy
//
//-----------------------------------------------------------------------

class LgFrPartScheduleTimesUnitCostScheduleStrategy : 
   public LgFrPartScheduleSensitiveScheduleStrategy {

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
  LgFrPartScheduleTimesUnitCostScheduleStrategy &
    operator=(
	      const LgFrPartScheduleTimesUnitCostScheduleStrategy& rhs);

  // Copy constructor
  LgFrPartScheduleTimesUnitCostScheduleStrategy (
     const LgFrPartScheduleTimesUnitCostScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrPartScheduleTimesUnitCostScheduleStrategy (
     const LgFrPartScheduleTimesUnitCostScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrPartScheduleTimesUnitCostScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrPartScheduleTimesUnitCostScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrPartScheduleTimesUnitCostScheduleStrategy methods.
  // Expects data from both testScenario to come from
  // /u/implode/wit/data/examples/diner12/wit.data
  static
  void
  contextTest (LgFrScenario                & testScnario,
               LgFrSortingPartScheduleFloatSmartPointer supply);
private:

  LgFrLastProblemSolved * lpsPtr_;
  const int id_;

  // Default Constructor
  LgFrPartScheduleTimesUnitCostScheduleStrategy();
};


#endif
