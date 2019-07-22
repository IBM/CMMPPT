#ifndef LGFR_DEMAND_SCHEDULE_STRATEGY_H
#define LGFR_DEMAND_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/schdStrt.h>

// Base Class for implementing Schedule Strategies used for populating
// a part schedule by LgFrSortingSchedule

// This class follows the Strategy Pattern

// Forward references
class LgFrSetOfDemands;

class LgFrDemandScheduleStrategy : 
   public LgFrAttributeScheduleStrategy {

public:

  // Set the  Set Of Demands
  void
     setOfDemands(const LgFrSetOfDemands &);


  // Assignment operator
  LgFrDemandScheduleStrategy &
    operator=(
	      const LgFrDemandScheduleStrategy& rhs);

  // Copy constructor
  LgFrDemandScheduleStrategy (
     const LgFrDemandScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrDemandScheduleStrategy (
     const LgFrDemandScheduleStrategy &,
     LgFrScenario & );

  // Constructor
  LgFrDemandScheduleStrategy (
     const LgFrSetOfDemands &,
     LgFrAlgorithm * algorithmPtr = 0 );

  // Destructor
  virtual
     ~LgFrDemandScheduleStrategy ();

protected:

  // Get Set Of Demands
  const LgFrSetOfDemands &
     setOfDemands()
        const;

  // Estimate size of schedule using this ass.
  // The populate method uses this to set an initial size of the
  // schedule.
  virtual
     size_t
        estimatedSize(
           const LgFrSortingScheduleFloat& )
              const;
private:

  const LgFrSetOfDemands * sodPtr_;

  // Default Constructor
  LgFrDemandScheduleStrategy ();
};

#endif
