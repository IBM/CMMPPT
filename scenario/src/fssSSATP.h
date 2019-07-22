#ifndef LGFR_FOCUSSED_SHORTAGE_SCHEDULE_SCHEDULE_STRATEGY_FOR_ATP_H
#define LGFR_FOCUSSED_SHORTAGE_SCHEDULE_SCHEDULE_STRATEGY_FOR_ATP_H


#include <scenario/src/fssSS.h>
#include <scenario/src/mixInATP.h>
#include <scenario/src/stateATP.h>


// Schedules using this strategy will be updated with the new
// shipment solution after running implode.

// This class follows the Strategy Pattern


class LgFrFocussedShortageScheduleScheduleStrategyForATP : 
   public LgFrFocussedShortageScheduleScheduleStrategy,
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
  LgFrFocussedShortageScheduleScheduleStrategyForATP &
    operator=(
	      const LgFrFocussedShortageScheduleScheduleStrategyForATP& rhs);

  // Copy constructor
  LgFrFocussedShortageScheduleScheduleStrategyForATP (
     const LgFrFocussedShortageScheduleScheduleStrategyForATP &);

  // Copy constructor with new Scenario
  LgFrFocussedShortageScheduleScheduleStrategyForATP (
     const LgFrFocussedShortageScheduleScheduleStrategyForATP &,
     LgFrScenarioSmartPointer);

  // Constructor
  LgFrFocussedShortageScheduleScheduleStrategyForATP (
     LgFrScenarioSmartPointer, int desiredState = LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY );

  // Destructor
  virtual
     ~LgFrFocussedShortageScheduleScheduleStrategyForATP ();

  // test this class
  static void test();

  // test LgFrFocussedShortageScheduleScheduleStrategyForATP methods.
  // Expects the data in testScenario to come from
  // "/u/implode/wit/data/examples/diner12/wit.data"

  static
  void
  contextTest (LgFrScenarioForATPSmartPointer testScenario,
               LgFrLastProblemSolved & lps,             // testScenario's
               LgFrSetOfParts        & sop,             // testScenario's
               LgFrSetOfDemands      & sod);            // testScenario's

private:
  const int id_;

  // Default Constructor
  LgFrFocussedShortageScheduleScheduleStrategyForATP ();

  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

};

#endif
