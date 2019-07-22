#ifndef LGFR_ITEM_TIME_VEC_PAIR_COMPARE_STRATEGY_H
#define LGFR_ITEM_TIME_VEC_PAIR_COMPARE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/model.h>

#include <scenario/src/idnumber.h>

// Base Class for implementing Comparison Strategies used for sorting
// by LgFrSortingSchedule

// This class follows the Strategy Pattern

// Forward References
class LgFrItemTimeVecPairFloat;
class LgFrScenario;
class LgFrSetOfParts;
class LgFrSetOfDemands;
class LgFrLastProblemSolved;
class LgFrWorkingImplosionProblem;
class LgFrImplode;
class LgFrFocussedShortageScheduleControl;

class LgFrPairCompareFloat  :
  public LgFrSubscription
{
public:

  // Compare two ItemTimeVecPairFloat's
  // return 0 if equal
  //        <0 if first less than second
  //        >0 if first greater than second
  virtual
    int
      itemTimeVecPairCompareFloat(
         const LgFrItemTimeVecPairFloat &,
         const LgFrItemTimeVecPairFloat &)
            const;

  // ModelClient updateFrom method - use the default

  // Make clone copy of this object
  virtual
     LgFrPairCompareFloat *
        clone() 
           const;

  // Make clone copy of this object with a new Scenario
  virtual
     LgFrPairCompareFloat *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrPairCompareFloat &
    operator=(
	      const LgFrPairCompareFloat& rhs);

  // Copy constructor
  LgFrPairCompareFloat (
     const LgFrPairCompareFloat &);

  // Copy constructor associated with a new Scenario
  LgFrPairCompareFloat (
     const LgFrPairCompareFloat &,
     LgFrScenario &,
     LgFrModel * algorithmPtr = 0 );

  // Default Constructor
  LgFrPairCompareFloat ( 
     LgFrModel * algorithmPtr = 0 );

  // Destructor
  ~LgFrPairCompareFloat ();

  // test this class
  static void test();

  // in scenario.h, class LgFrScenario declares these methods to be "friend".
  // see the comment in schdStrt.h.
  //// protected:

  // Return the setOfParts which belongs to a scenario
  const LgFrSetOfParts&
     constSetOfParts(
        int k,
        LgFrScenario &);

  // Return the setOfDemands which belongs to a scenario
  const LgFrSetOfDemands&
     constSetOfDemands(
        int k,
        LgFrScenario &);

  // Return the LastWorkingProblem which belongs to a scenario
  LgFrLastProblemSolved&
     lastProblemSolved(
        LgFrScenario &);

  // Return the WorkingImplosionProblem which belongs to a scenario
  LgFrWorkingImplosionProblem&
     workingImplosionProblem(
        LgFrScenario &);

  // Return the Implode which belongs to a scenario
  LgFrImplode&
     implode(
        LgFrScenario &);

  // Return the focussed shortage schedule which belongs to a scenario
  LgFrFocussedShortageScheduleControl&
     focussedShortageScheduleControl(
        LgFrScenario &);


private:

  LgFrModel * algorithmPtr_;  // Pointer to algorithm that PairCompare depends
                            // on.  If there are no algorithm
                            // dependencies then algorithmPtr_ is null.
  const int id_;
};

#endif
