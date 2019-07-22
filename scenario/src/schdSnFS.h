#ifndef LGFR_SCHEDULE_SENSITIVE_FILTER_STRATEGY_H
#define LGFR_SCHEDULE_SENSITIVE_FILTER_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/filtStrt.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/sortSchP.h>

// Filter strategy for excluding items from a schedule based on the
// contents of another schedule.

// Forward references
class LgFrSortingScheduleFloat;
class LgFrTimeVecCompareFloat;
class LgFrLastProblemSolved;
class LgFrSortingPartScheduleFloatSmartPointer;

class LgFrScheduleSensitiveFilterStrategy : public LgFrFilterStrategy {
public:

  virtual int GetFSType() const { return __LGFRSCHEDULESENSITIVEFILTERSTRATEGY; }

  // Return true if itemTimeVec is not in schedule.
  virtual
    bool
      exclude(
        const LgFrItemTimeVecPairFloat &)
          const;

  // Set Schedule containing data to be used when filtering.
  virtual
     void
        schedule(
           LgFrSortingScheduleFloatSmartPointer);

  // Get Schedule containing values to be used when filtering
  virtual
     LgFrSortingScheduleFloatSmartPointerToConst
        schedule()
           const;

  // Note:  The following methods could be added.
  //         They would allow one to filter only the backlogs greater
  //         than some number, or filter only the first 50 critical parts.
  //         The object LgFrTimeVecFilterFloat would have to be created.
  //         See LgFrScheduleSensitivePairCompareFloat::timeVecCompareFloat
  //         for a similar example.
  // virtual
  //    void
  //       timeVecFilterFloat(
  //          const LgFrTimeVecFilterFloat & );
  //
  // virtual
  //    const LgFrTimeVecFilterFloat *
  //      timeVecFilterFloat()
  //         const;


  // Make clone copy of this object
  virtual
     LgFrFilterStrategy *
        clone() 
           const;

  // Make clone copy of this object with new Scenario
  virtual
     LgFrFilterStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrScheduleSensitiveFilterStrategy &
    operator=(
	      const LgFrScheduleSensitiveFilterStrategy& rhs);

  // Copy constructor
  LgFrScheduleSensitiveFilterStrategy (
     const LgFrScheduleSensitiveFilterStrategy &);

  // Copy constructor with newScenario
  LgFrScheduleSensitiveFilterStrategy (
     const LgFrScheduleSensitiveFilterStrategy &,
     LgFrScenario &);

  // Default Constructor
  LgFrScheduleSensitiveFilterStrategy ();

  // Destructor
  ~LgFrScheduleSensitiveFilterStrategy ();

  // test this class
  static void test();

  // Test LgFrScheduleSensitiveFilterStrategy methods.
  // Expects testScenario to contain data from
  // /u/implode/wit/data/brenda/denise/wit.data
  // Expects criticalSchedule to be a critical supply schedule for testScenario
  // (containing a LgFrCriticalPartScheduleStrategy).
  // Expects supplySchedule to be a supply schedule for testScenario
  // (containing a LgFrSupplyScheduleStrategy).
  static
  void
  contextTest ( LgFrScenario & testScenario,
                LgFrLastProblemSolved & lps,
                LgFrSortingPartScheduleFloatSmartPointer criticalSchedule,
                LgFrSortingPartScheduleFloatSmartPointer supplySchedule
               );

private:

  LgFrSortingScheduleFloatSmartPointer   schedulePtr_;
  const int id_;

};

#endif
