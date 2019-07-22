#ifndef LGFR_USAGE_SCHEDULE_STRATEGY_H
#define LGFR_USAGE_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/schdSnSS.h>


// LgFrUsageScheduleStrategy:
//   This Schedule Strategy is sensitive to FOUR other schedules:
//        1) Supply
//        2) Production
//        3) Stock
//        4) Scrap
//   You MUST insert the four schedules before you try to populate
//   with it.  The schedules must be inserted in the order above.
//
//   For any part:
//     usage[t] = supply[t] + prod[t] + stock[t-1] - stock[t] - scrap[t]
//
//   NOTE: For capacity parts, prod and stock should be zero and similarly,
//         For raw parts, prod should be 0


// Forward References
class LgFrSortingScheduleFloat;
class LgFrSortingPartScheduleFloat;
class LgFrSortingPartScheduleFloatSmartPointer;


//-----------------------------------------------------------------------
//
//  Definition of LgFrUsageScheduleStrategy
//
//-----------------------------------------------------------------------

class LgFrUsageScheduleStrategy : 
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
  LgFrUsageScheduleStrategy &
    operator=(
	      const LgFrUsageScheduleStrategy& rhs);

  // Copy constructor
  LgFrUsageScheduleStrategy (
     const LgFrUsageScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrUsageScheduleStrategy (
     const LgFrUsageScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrUsageScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrUsageScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrUsageScheduleStrategy methods.
  // Expects data from both testScenario to come from
  // /u/implode/wit/data/examples/diner12/wit.data
  static
  void
  contextTest (LgFrScenario                & testScenario,
               LgFrSortingPartScheduleFloatSmartPointer supply,
               LgFrSortingPartScheduleFloatSmartPointer prod,
               LgFrSortingPartScheduleFloatSmartPointer stock,
               LgFrSortingPartScheduleFloatSmartPointer scrap);

  // test quantities in a schedule that uses a LgFrUsageScheduleStrategy.
  // Expects the scenario containing constUsageSched to contain data from
  // /u/implode/wit/data/examples/diner12/wit.data.  Expects an implosion
  // to have been done in the scenario already.
  static
  void
  quantityTest(const LgFrSortingPartScheduleFloat & constUsageSched);

private:

  const int id_;

  // Default Constructor
  LgFrUsageScheduleStrategy ();
};


#endif
