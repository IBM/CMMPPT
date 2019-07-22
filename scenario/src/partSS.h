#ifndef LGFR_PART_SCHEDULE_STRATEGY_H
#define LGFR_PART_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/schdStrt.h>

// #include <scenario/src/sOfParts.h>

// Base Class for implementing Schedule Strategies used for populating
// a part schedule by LgFrSortingSchedule

// This class follows the Strategy Pattern

// Forward references
class LgFrSetOfParts;

class LgFrPartScheduleStrategy : 
   public LgFrAttributeScheduleStrategy {

public:

  // Set the  Set Of Parts
  void
    setOfParts(const LgFrSetOfParts &);

  // Assignment operator
  LgFrPartScheduleStrategy &
    operator=(
	      const LgFrPartScheduleStrategy& rhs);

  // Copy constructor
  LgFrPartScheduleStrategy (
     const LgFrPartScheduleStrategy &);

  // Copy constructor associated with a new Scenario
  LgFrPartScheduleStrategy (
     const LgFrPartScheduleStrategy &,
     LgFrScenario & );


  // Constructor
  LgFrPartScheduleStrategy (
     const LgFrSetOfParts &,
     LgFrAlgorithm * algorithmPtr = 0 );


  // Destructor
  virtual
     ~LgFrPartScheduleStrategy ();

protected:

  // Estimate size of schedule using this ass.
  // The populate method uses this to set an initial size of the
  // schedule.
  virtual
     size_t
        estimatedSize(
           const LgFrSortingScheduleFloat& )
              const;

  // Get Set Of Parts
  const LgFrSetOfParts &
     setOfParts()
        const;

private:

  const LgFrSetOfParts * sopPtr_;

  // Default Constructor
  LgFrPartScheduleStrategy ();

};

#endif
