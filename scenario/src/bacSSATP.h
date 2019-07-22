#ifndef LGFR_BACKLOG_SCHEDULE_STRATEGY_FOR_ATP_H
#define LGFR_BACKLOG_SCHEDULE_STRATEGY_FOR_ATP_H

#include <scenario/src/backlgSS.h>
#include <scenario/src/mixInATP.h>
#include <scenario/src/stateATP.h>

// Schedule Strategy for a backlog schedule.
// The items are demands
// The timeVecs are the demands' backlog 

// Schedules using this strategy will be updated with the new
// shipment solution after running implode.

// This class follows the Strategy Pattern


class LgFrBacklogScheduleStrategyForATP : 
   public LgFrBacklogScheduleStrategy,
   public LgFrScheduleMixInForATP
{

public:

  // Populate Schedule with itemTimeVecs
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
  LgFrBacklogScheduleStrategyForATP &
    operator=(
	      const LgFrBacklogScheduleStrategyForATP& rhs);

  // Copy constructor
  LgFrBacklogScheduleStrategyForATP (
     const LgFrBacklogScheduleStrategyForATP &);

  // Copy constructor with new Scenario
  LgFrBacklogScheduleStrategyForATP (
     const LgFrBacklogScheduleStrategyForATP &,
     LgFrScenarioSmartPointer);

  // Constructor
  LgFrBacklogScheduleStrategyForATP (
     LgFrScenarioSmartPointer, int desiredState = STATE_INIT );

  // Destructor
  virtual
     ~LgFrBacklogScheduleStrategyForATP ();

  // test this class
  static void test();

  // test LgFrBacklogScheduleStrategyForATP methods.
  // Expects the data in testScenario to come from
  // "/u/implode/wit/data/examples/diner12/wit.data"
  static
  void
  contextTest
    (LgFrScenarioForATPSmartPointer	testScenario,
     LgFrLastProblemSolved		& lps,		// testScenario's 
     LgFrSetOfDemands			& sod);		// testScenario's 


private:
  const int id_;

  // Default Constructor
  LgFrBacklogScheduleStrategyForATP ();

  // clone method that you should not use
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario&) 
           const;

};

#endif
